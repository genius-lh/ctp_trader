#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 

#include "cmn_log.h"
#include "cmn_util.h"
#include "trader_strategy_engine.h"
#include "trader_data.h"
#include "trader_msg_struct.h"
#include "trader_trader_api.h"

#include "trader_strategy_limit.h"

#include "trader_strategy.h"

static int trader_strategy_init(trader_strategy* self);
static int trader_strategy_on_tick(trader_strategy* self, trader_tick* tick_data);
static int trader_strategy_on_order(trader_strategy* self, trader_order* order_data);
static int trader_strategy_on_trade(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_on_status(trader_strategy* self, instrument_status* status_data);
static int trader_strategy_on_timer_status(trader_strategy* self);

static int trader_strategy_tick_not_finished(trader_strategy* self);
static int trader_strategy_tick_finished(trader_strategy* self);
static int trader_strategy_tick_open(trader_strategy* self);
static int trader_strategy_tick_auto(trader_strategy* self, int ms);

// 重置仓位
static int trader_strategy_reset_position(trader_strategy* self, char buy_sell, int volume);
// 查询仓位
static int trader_strategy_query_position(trader_strategy* self, char buy_sell, trade_position* p_position);

// 更新仓位
static int trader_strategy_update_position(trader_strategy* self, char buy_sell, int volume, double t1_price, double t2_price);

static int trader_strategy_order_update(trader_strategy* self,  trader_order* order_data);

static int trader_strategy_order_t1_open_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t1_close_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_open_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_close_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_open_failed(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_close_failed(trader_strategy* self,  trader_order* order_data);


static int trader_strategy_trade_t1_open(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t2_open(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t1_close(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t2_close(trader_strategy* self, trader_trade* trade_data);

static int trader_strategy_insert_t1_open(trader_strategy* self, char long_short);
static int trader_strategy_insert_t1_close(trader_strategy* self, char long_short);
static int trader_strategy_insert_t2_open(trader_strategy* self, trader_strategy_trade* strategy_trade);
static int trader_strategy_insert_t2_close(trader_strategy* self, char long_short, int vol, double t1price, double t2price);

static int trader_strategy_t1_close_imp(trader_strategy* self, char long_short, char offset, int vol);
static int trader_strategy_t2_close_imp(trader_strategy* self, trader_strategy_trade* strategy_trade);

static int trader_strategy_cancel_order(trader_strategy* self, char* user_order_id);
static int trader_strategy_timed_cancel(trader_strategy* self, char* user_order_id);

static int trader_strategy_release_order(trader_strategy* self, char* user_order_id);

static int trader_strategy_print_tick(trader_strategy* self);

static int trader_strategy_check_closing(trader_strategy* self, trader_tick* tick_data);
static int trader_strategy_tick_trigger(trader_strategy* self, trader_tick* tick_data);

static int trader_strategy_print_order_time(trader_strategy* self, trader_order* order_data);

static trader_strategy_method* trader_strategy_method_get();

trader_strategy_method* trader_strategy_method_get()
{
  static trader_strategy_method trader_strategy_method_st = {
    trader_strategy_init,
    trader_strategy_on_tick,
    trader_strategy_on_order,
    trader_strategy_on_trade,
    trader_strategy_on_status,
    trader_strategy_on_timer_status,
    trader_strategy_reset_position,
    trader_strategy_query_position
  };

  return &trader_strategy_method_st;
}

int trader_strategy_init(trader_strategy* self)
{
  // T1 行情
  memset(&self->oT1Tick, 0, sizeof(self->oT1Tick));
  // T2 行情
  memset(&self->oT2Tick, 0, sizeof(self->oT2Tick));
  
  // 订单队列
  self->mapOrder = cmn_util_map_new();
  
  // 计划队列
  self->mapStrategyPlan = cmn_util_map_new();

  // 待成交队列
  TAILQ_INIT(&self->listNotFinishedOrder);

  // T1成交队列
  self->mapHalfTraded = cmn_util_map_new();
  
  // 当前策略持仓
  // 多单
  self->nPositionBuy = 0;
  // 空单
  self->nPositionSell = 0;
  self->nOrderBuy = 0;
  self->nOrderSell = 0;
  self->nFronzeBuy = 0;
  self->nFronzeSell = 0;
  self->nFailedBuyOpen = 0;
  self->nFailedSellOpen = 0;
  self->nFailedBuyClose = 0;
  self->nFailedSellClose = 0;

  trader_strategy_reset_position(self, TRADER_POSITION_BUY, 0);
  trader_strategy_reset_position(self, TRADER_POSITION_SELL, 0);
  
  return 0;
}


int trader_strategy_on_tick(trader_strategy* self, trader_tick* tick_data)
{
  
  if(!strcmp(self->T1, tick_data->InstrumentID)){
    memcpy(&self->oT1Tick, tick_data, sizeof(self->oT1Tick));
  }else if(!strcmp(self->T2, tick_data->InstrumentID)){
    memcpy(&self->oT2Tick, tick_data, sizeof(self->oT2Tick));
  }else{
    // 非本策略关注合约，直接忽略
    //CMN_DEBUG("Not Focused[%s]!\n", tick_data->InstrumentID);
    return 0;
  }
  
  if(self->used){
    CMN_INFO("SID[%02d]tick[%s]UpdateTime[%s]UpdateMillisec[%d]\n", self->idx, tick_data->InstrumentID, tick_data->UpdateTime, tick_data->UpdateMillisec);
  }

  if((0 == self->oT1Tick.AskVolume1)
  || (0 == self->oT1Tick.BidVolume1)
  || (0 == self->oT2Tick.AskVolume1)
  || (0 == self->oT2Tick.BidVolume1)){
    // 行情不正常
    CMN_INFO("T1.InstrumentID=[%s]T2.InstrumentID=[%s]\n"
      "T1.AskVolume1=[%d]T1.BidVolume1=[%d]\n"
      "T2.AskVolume1=[%d]T2.BidVolume1=[%d]\n",
      self->T1,
      self->T2,
      self->oT1Tick.AskVolume1,
      self->oT1Tick.BidVolume1,
      self->oT2Tick.AskVolume1,
      self->oT2Tick.BidVolume1);
    return 0;
  }
  
  // 不报单时段
  /*
  if((0 == memcmp(tick_data->UpdateTime, "11:29:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "11:29:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "11:29:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "11:30:00", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "14:59:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "14:59:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "14:59:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "15:00:00", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "00:59:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "00:59:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "00:59:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "01:00:00", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "02:29:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "02:29:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "02:29:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "02:30:00", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "22:59:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "22:59:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "22:59:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "23:00:00", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "23:29:57", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "23:29:58", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "23:29:59", 8))
  ||(0 == memcmp(tick_data->UpdateTime, "23:30:00", 8))
  ){
    return 0;
  }
  */
  

/*
  if(strcmp(self->oT1Tick.TradingDay, self->oT2Tick.TradingDay) 
  ||strcmp(self->oT1Tick.UpdateTime, self->oT2Tick.UpdateTime)
  ||(self->oT1Tick.UpdateMillisec != self->oT2Tick.UpdateMillisec)){
    // 等待第二个合约的信息
    CMN_DEBUG("Waiting for another tick[%s]!\n", tick_data->InstrumentID);
    return 0;
  }
*/

  // 收盘判断
  trader_strategy_check_closing(self, tick_data);

  // 策略执行判断  
  int ret = 0;
  ret = trader_strategy_tick_trigger(self, tick_data);
  if(!ret){
    return 0;
  }

  // 开盘自动策略
  trader_strategy_tick_auto(self, tick_data->UpdateMillisec);
  
  // 待成交队列处理
  trader_strategy_tick_not_finished(self);

  // 成交队列处理
  trader_strategy_tick_finished(self);

  // 新建委托判断
  trader_strategy_tick_open(self);
  
  return 0;
}

int trader_strategy_on_order(trader_strategy* self, trader_order* order_data)
{
  // 更新订单状态
  trader_strategy_order_update(self, order_data);

  // 判断订单状态
  if(TRADER_ORDER_OS_CANCELED == order_data->OrderStatus){
    // 判断订单类型
    char cOrderType;
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, order_data->UserOrderLocalID);
    
    if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
      // T1撤单完成，更新计划仓位
      trader_strategy_order_t1_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
      // T1撤单完成，更新计划仓位
      trader_strategy_order_t1_close_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
      // T2撤单完成，重新下单T2
      trader_strategy_order_t2_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
      // T2撤单完成，更新计划仓位
      trader_strategy_order_t2_close_cancel(self, order_data);
    }else{
      CMN_ERROR("cannot reached!\n");
    }
  }

  // 提交订单失败
  if(TRADER_ORDER_OS_ACCEPTEDNOREPLY == order_data->OrderStatus){
    char cOrderType;
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, order_data->UserOrderLocalID);
    if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
      // T1撤单完成，更新计划仓位
      trader_strategy_order_t1_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
      // T1撤单完成，更新计划仓位
      trader_strategy_order_t1_close_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
      // 订单提交失败导致单腿
      trader_strategy_order_t2_open_failed(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
      // 订单提交失败导致单腿
      trader_strategy_order_t2_close_failed(self, order_data);
    }else{
      CMN_ERROR("cannot reached!\n");
    }

  }
  

  if((TRADER_ORDER_OS_CANCELED == order_data->OrderStatus)
  ||(TRADER_ORDER_OS_ACCEPTEDNOREPLY == order_data->OrderStatus)
  ||(TRADER_ORDER_OS_ALLTRADED == order_data->OrderStatus)){
    trader_strategy_release_order(self, order_data->UserOrderLocalID);
  }
  
  return 0;
}

int trader_strategy_on_trade(trader_strategy* self, trader_trade* trade_data)
{
  // 判断订单类型
  char cOrderType;
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, trade_data->UserOrderLocalID);

  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    // 开仓T1 成交，下单T2
    trader_strategy_trade_t1_open(self, trade_data);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    // 平仓T1 成交，下单T2
    trader_strategy_trade_t1_close(self, trade_data);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    // 开仓T2 成交，更新仓位信息
    trader_strategy_trade_t2_open(self, trade_data);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    // 平仓T2 成交，更新仓位信息，更新收益信息
    trader_strategy_trade_t2_close(self, trade_data);
  }else{
    CMN_ERROR("cannot reached!\n");
  }

  // 保存成交记录
  gettimeofday(&trade_data->UpdateTime, NULL);
  self->pEngine->pMethod->xSaveTrade(self->pEngine, trade_data);

  return 0;
}

int trader_strategy_on_status(trader_strategy* self, instrument_status* status_data)
{  
  if(INSTRUMENT_STATUS_CONTINOUS != status_data->InstrumentStatus){
    return 0;
  }

  if(!self->used){
    return 0;
  }

  int nT1Len = strnlen(self->T1, sizeof(self->T1));
  int nT2Len = strnlen(self->T2, sizeof(self->T2));
  int nIIDLen = strnlen(status_data->InstrumentID, sizeof(status_data->InstrumentID));

  if((0 == nT1Len) || (0 == nT2Len)){
    return 0;
  }
  
  do{
    if(nIIDLen < nT1Len){
      nT1Len = nIIDLen;
    }
    
    if(nIIDLen < nT2Len){
      nT2Len = nIIDLen;
    }

    if(0 == memcmp(self->T1, status_data->InstrumentID, nT1Len)){
      break;
    }

    if(0 == memcmp(self->T2, status_data->InstrumentID, nT2Len)){
      break;
    }
    // 非本策略关注合约，直接忽略
    //CMN_DEBUG("Not Focused[%s]!\n", tick_data->InstrumentID);
    return 0;
  }while(0);
  
  if((0 == self->oT1Tick.AskVolume1)
  || (0 == self->oT1Tick.BidVolume1)
  || (0 == self->oT2Tick.AskVolume1)
  || (0 == self->oT2Tick.BidVolume1)){
    // 行情不正常
    CMN_INFO("T1.AskVolume1=[%d]T1.BidVolume1=[%d]T2.AskVolume1=[%d]T2.BidVolume1=[%d]\n",
      self->oT1Tick.AskVolume1,
      self->oT1Tick.BidVolume1,
      self->oT2Tick.AskVolume1,
      self->oT2Tick.BidVolume1);
    return 0;
  }

  //中午收盘判断
  if((0 == memcmp(self->oT1Tick.UpdateTime, "11:29:59", 8))
  ||(0 == memcmp(self->oT2Tick.UpdateTime, "11:29:59", 8))){
    CMN_INFO("下午开盘\n");
    return 0;
  }
  
  CMN_INFO("status_data->InstrumentID[%s]\n", status_data->InstrumentID);
  
  // 成交队列处理
  trader_strategy_tick_finished(self);

  // 新建委托判断
  trader_strategy_tick_open(self);
  
  return 0;
}

int trader_strategy_on_timer_status(trader_strategy* self)
{
  if(!self->used){
    return 0;
  }

  // 成交队列处理
  trader_strategy_tick_finished(self);

  // 新建委托判断
  trader_strategy_tick_open(self);

  return 0;
}

// 重置仓位
int trader_strategy_reset_position(trader_strategy* self, char buy_sell, int volume)
{
  CMN_DEBUG("BEFORE\n"
    "nFailedBuyOpen[%d]\n"
    "nFailedSellOpen[%d]\n"
    "nFailedBuyClose[%d]\n"
    "nFailedSellClose[%d]\n"
    "nPositionBuy[%d]\n"
    "nPositionSell[%d]\n"
    "nOrderBuy[%d]\n"
    "nOrderSell[%d]\n"
    "nFronzeBuy[%d]\n"
    "nFronzeSell[%d]\n",
    self->nFailedBuyOpen,
    self->nFailedSellOpen,
    self->nFailedBuyClose,
    self->nFailedSellClose,
    self->nPositionBuy,
    self->nPositionSell,
    self->nOrderBuy,
    self->nOrderSell,
    self->nFronzeBuy,
    self->nFronzeSell
  );
  int nTemp = 0;
  trade_position* pPosition;
  if(TRADER_POSITION_BUY == buy_sell){
    nTemp = volume - self->nPositionBuy;
    self->nPositionBuy = volume;
    self->nOrderBuy = 0;
    self->nFronzeBuy = 0;
    self->nFailedBuyOpen = 0;
    self->nFailedBuyClose = 0;
    pPosition = &self->oBuyPosition;
    
  }else{
    nTemp = volume - self->nPositionSell;
    self->nPositionSell = volume;
    self->nOrderSell = 0;
    self->nFronzeSell = 0;
    self->nFailedSellOpen = 0;
    self->nFailedSellClose = 0;
    pPosition = &self->oSellPosition;
  }
  
  CMN_DEBUG("AFTER\n"
    "nFailedBuyOpen[%d]\n"
    "nFailedSellOpen[%d]\n"
    "nFailedBuyClose[%d]\n"
    "nFailedSellClose[%d]\n"
    "nPositionBuy[%d]\n"
    "nPositionSell[%d]\n"
    "nOrderBuy[%d]\n"
    "nOrderSell[%d]\n"
    "nFronzeBuy[%d]\n"
    "nFronzeSell[%d]\n",
    self->nFailedBuyOpen,
    self->nFailedSellOpen,
    self->nFailedBuyClose,
    self->nFailedSellClose,
    self->nPositionBuy,
    self->nPositionSell,
    self->nOrderBuy,
    self->nOrderSell,
    self->nFronzeBuy,
    self->nFronzeSell
    );


  strcpy(pPosition->T1, self->T1);
  strcpy(pPosition->T2, self->T2);
  pPosition->StageID = self->idx;
  pPosition->Direction = buy_sell;
  pPosition->RealPrice = 0.0f;
  pPosition->ExpPrice = 0.0f;
  pPosition->Volume = volume;

  return 0;
}

// 查询仓位
int trader_strategy_query_position(trader_strategy* self, char buy_sell, trade_position* p_position)
{
  trade_position* pPosition;

  if(TRADER_POSITION_BUY == buy_sell){
    pPosition = &self->oBuyPosition;
  }else{
    pPosition = &self->oSellPosition;
  }

  memcpy(p_position, pPosition, sizeof(trade_position));

  return 0;
}


// 更新仓位
int trader_strategy_update_position(trader_strategy* self, char buy_sell, int volume, double t1_price, double t2_price)
{
  trade_position* pPosition;
  if(TRADER_POSITION_BUY == buy_sell){
    pPosition = &self->oBuyPosition;
    pPosition->ExpPrice = self->DTOpen;
  }else{
    pPosition = &self->oSellPosition;
    pPosition->ExpPrice = self->KTOpen;
  }
  
  double real_price = t1_price * self->T1Weight - t2_price * self->T2Weight;

  if(volume > 0){
    pPosition->RealPrice = (pPosition->RealPrice * pPosition->Volume + real_price * volume) / (pPosition->Volume + volume);
  }
  
  pPosition->Volume += volume;
  return 0;
}

// 开盘自动策略
int trader_strategy_tick_auto(trader_strategy* self, int ms)
{
  if(AUTO_STRATEGY_TYPE_CLOSE == self->AutoType){
    return 0;
  }

  if(AUTO_STRATEGY_TYPE_1 == self->AutoType){
    if(!AUTO_STRATEGY_IS_TYPE_1(ms)){
      return 0;
    }
  }

  if(AUTO_STRATEGY_TYPE_2 == self->AutoType){
    if(AUTO_STRATEGY_IS_TYPE_1(ms)){
      return 0;
    }
  }

  // 待成交队列判断
  if(!TAILQ_EMPTY(&self->listNotFinishedOrder)){
    return 0;
  }

  // 成交队列判断
  if((self->nPositionBuy > 0) || (self->nPositionSell > 0)){
    return 0;
  } 

  trader_strategy_auto_sell(self);

  trader_strategy_auto_buy(self);

  return 0;
}


int trader_strategy_tick_not_finished(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");

  trader_strategy_order* pStrategyOrder;
  trader_order* pOrder;
  char cOrderType;
  int bCancel = 0;
  
  // 待成交队列处理
  // 读取未成交队列
  TAILQ_FOREACH(pStrategyOrder, &self->listNotFinishedOrder,  next){
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, 
      pStrategyOrder->ref);
    pOrder = pStrategyOrder->order;
    // 如果是T1，T1等待判断
    if((TRADER_POSITION_TYPE_OPEN_T1 == cOrderType)
    ||(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType)){
      bCancel = trader_strategy_judge_t1_wait(self, pOrder);
    }else if ((TRADER_POSITION_TYPE_OPEN_T2 == cOrderType)
    ||(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType)){
    // 如果是T2，T2等待判断
      bCancel = trader_strategy_judge_t2_wait(self, pOrder);
    }else {
      continue;
    }
    
    // 超过等待条件，撤单
    if(bCancel){
      trader_strategy_cancel_order(self, pStrategyOrder->ref);
      pStrategyOrder->needCancel = 1;
    }
  }

  return 0;
}

int trader_strategy_tick_finished(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");
  int nSize = 0;  
  int bClose = 0;

  if(!self->used){
    CMN_DEBUG("not used!\n");
    return 0;
  }
  
  // 报单判断
  if((self->nFronzeBuy > 0) || (self->nFronzeSell > 0)){
    CMN_DEBUG("nFronzeBuy[%d]nOrderSell[%d]!\n", self->nFronzeBuy, self->nFronzeSell);
    return 0;
  }

  // 平多单
  // 持仓判断
  if(self->nPositionBuy > 0){
    // 限仓判断
    if(self->nPositionBuy > (self->hold * self->T2Ratio)){
      bClose =  trader_strategy_judge_buy_close(self);
      if(bClose){
        // 新建平仓计划
        trader_strategy_insert_t1_close(self, TRADER_POSITION_LONG);
      }
    }
  }
  
  // 平空单
  // 持仓判断
  if(self->nPositionSell > 0){
    // 限仓判断
    if(self->nPositionSell > (self->hold * self->T2Ratio)){
      bClose =  trader_strategy_judge_sell_close(self);
      if(bClose){
        // 新建平仓计划
        trader_strategy_insert_t1_close(self, TRADER_POSITION_SHORT);
      }
    }
  }

  return 0;
}

int trader_strategy_tick_open(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");

  if(!self->used){
    CMN_DEBUG("not used!\n");
    return 0;
  }

  int nSize = 0;
  // 总持仓判断

  // MP 判断
  if((self->nOrderBuy > 0) || (self->nOrderSell > 0)){
    CMN_DEBUG("nOrderBuy[%d]nOrderSell[%d]!\n", self->nOrderBuy, self->nOrderSell);
    return 0;
  }

  // 单策略委托判断
  nSize = self->nPositionBuy 
    + self->nPositionSell;
  
  if(nSize >= self->MP * self->T2Ratio){
    CMN_DEBUG("nSize[%d]>=MP[%d]*T2Ratio[%d]!\n", nSize, self->MP, self->T2Ratio);
    return 0;
  }

  int nBuy = 0;
  int nSell = 0;
  double lBuyDiff;
  double lSellDiff;
  char cLongShort;
  trader_tick* pT1MduserInf = &self->oT1Tick;
  trader_tick* pT2MduserInf = &self->oT2Tick;
  
  // 开仓判断
  // 操作方向判断
  do {
    //买开判断
    nBuy = trader_strategy_judge_buy_open(self);
    if(nBuy){
      //买开
      cLongShort = TRADER_POSITION_LONG;
      break;
    }
    
    //卖开判断
    nSell = trader_strategy_judge_sell_open(self);
    if(nSell){
      // 卖开
      cLongShort = TRADER_POSITION_SHORT;
      break;
    }
    
    CMN_DEBUG("cannot open\n");
    return 0;
  }while(0);

  // 新建开仓计划  
  // 下单T1
  trader_strategy_insert_t1_open(self, cLongShort);

  return 0;
}

int trader_strategy_order_update(trader_strategy* self, trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder;
  // 获取订单
  pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, order_data->UserOrderLocalID);
  if(!pOrder){
    CMN_WARN("[%s]not in order list\n", order_data->UserOrderLocalID);
    return -1;
  }
  
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", order_data->UserOrderLocalID, pOrder);
  char origOrderStatus = pOrder->OrderStatus;
  trader_strategy_order* pIter = NULL;
  
  // 更新订单数据
  pOrder->OrderStatus = order_data->OrderStatus;
  pOrder->VolumeTraded = order_data->VolumeTraded;
  strcpy(pOrder->ExchangeID, order_data->ExchangeID);
  strcpy(pOrder->OrderSysID, order_data->OrderSysID);
  
  // 判断是否需要更新订单时间
  if(TRADER_ORDER_OS_UNKNOW != pOrder->OrderStatus){
    if((!pOrder->UpdateTime.tv_sec)
    && (!pOrder->UpdateTime.tv_usec)){
      pOrder->HedgeFlag = order_data->HedgeFlag;
      strcpy(pOrder->InsertTime, order_data->InsertTime);
      gettimeofday(&pOrder->UpdateTime, NULL);
      trader_strategy_print_order_time(self, pOrder);
    }
  }

  // 判断是否需要撤单
  do{
    // 判断进入队列
    if(TRADER_ORDER_OS_NOTRADEQUEUEING == origOrderStatus){
      break;
    }

    if(TRADER_ORDER_OS_NOTRADEQUEUEING != pOrder->OrderStatus){
      break;
    }
    
    TAILQ_FOREACH(pIter, &self->listNotFinishedOrder,  next){
      if(!strncmp(pIter->ref, pOrder->UserOrderLocalID, sizeof(pIter->ref))){
        if(pIter->needCancel){
          trader_strategy_cancel_order(self, pOrder->UserOrderLocalID);
        }
        break;
      }
    }
  }while(0);
  
  
  return 0;
}

int trader_strategy_order_t1_open_cancel(trader_strategy* self, trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  
  trader_strategy_plan* pStrategyPlan = NULL;
  // 获取下单计划
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, order_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //没找到报单
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }
  
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  // 更新可下单笔数
  if(TRADER_POSITION_LONG == pStrategyPlan->cLongShort){
    // 做多撤单
    self->nOrderBuy -= nReset;
    CMN_DEBUG("self->nOrderBuy=[%d]\n", self->nOrderBuy);
  }else{
    // 做空撤单
    self->nOrderSell -= nReset;
    CMN_DEBUG("self->nOrderSell=[%d]\n", self->nOrderSell);
  }
  
  return 0;
}

int trader_strategy_order_t1_close_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  trader_strategy_plan* pStrategyPlan = NULL;
  // 获取下单计划
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, order_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //没找到报单
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // 更新可下单笔数
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  if(TRADER_POSITION_LONG == pStrategyPlan->cLongShort){
    // 做多冻结
    self->nFronzeBuy -= nReset;
    CMN_DEBUG("self->nFronzeBuy=[%d]\n", self->nFronzeBuy);
  }else{
    // 做空冻结
    self->nFronzeSell -= nReset;
    CMN_DEBUG("self->nFronzeSell=[%d]\n", self->nFronzeSell);
  }

  return 0;
}

int trader_strategy_order_t2_open_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // 从T1队列中移除
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  
  if(pStrategyTrade->T2CancelNumber > self->pEngine->t2CancelLimit){
    CMN_WARN("UserOrderLocalID=[%s]T2CancelNumber[%d]>t2CancelLimit[%d]\n"
      , order_data->UserOrderLocalID
      , pStrategyTrade->T2CancelNumber
      , self->pEngine->t2CancelLimit);
    return -2;
  }
  
  CMN_DEBUG("UserOrderLocalID=[%s]T2CancelNumber[%d]<=t2CancelLimit[%d]\n"
    , order_data->UserOrderLocalID
    , pStrategyTrade->T2CancelNumber
    , self->pEngine->t2CancelLimit);

  // 重新计算T2价格
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  double fT2Price = trader_strategy_t2_price_opponent(self, order_data->Direction);
  pStrategyTrade->T2Price = fT2Price;
  pStrategyTrade->TradeVolume = nReset;
  pStrategyTrade->T2CancelNumber++;
  
  // 下单T2
  trader_strategy_insert_t2_open(self, pStrategyTrade);
  
  return 0;
}

int trader_strategy_order_t2_close_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // 从T1队列中移除
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  if(pStrategyTrade->T2CancelNumber > self->pEngine->t2CancelLimit){
    CMN_WARN("UserOrderLocalID=[%s]T2CancelNumber[%d]>t2CancelLimit[%d]\n"
      , order_data->UserOrderLocalID
      , pStrategyTrade->T2CancelNumber
      , self->pEngine->t2CancelLimit);
    return -2;
  }
  
  CMN_DEBUG("UserOrderLocalID=[%s]T2CancelNumber[%d]<=t2CancelLimit[%d]\n"
    , order_data->UserOrderLocalID
    , pStrategyTrade->T2CancelNumber
    , self->pEngine->t2CancelLimit);

  // 重新计算T2价格
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  double fT2Price = trader_strategy_t2_price_opponent(self, order_data->Direction);
  pStrategyTrade->T2Price = fT2Price;
  pStrategyTrade->TradeVolume = nReset;
  pStrategyTrade->T2CancelNumber++;
  
  // 下单T2
  trader_strategy_t2_close_imp(self, pStrategyTrade);
  
  return 0;
}

int trader_strategy_order_t2_open_failed(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }
  
  // 从T1队列中移除
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // 重新计算T2价格
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  CMN_ERROR("t2_open_failed[%d]!\n", nReset);
  self->used = 0;
  
  // 更新可下单笔数
  if(TRADER_POSITION_LONG == pStrategyTrade->cLongShort){
    self->nOrderBuy -= nReset;
    self->nFailedBuyOpen += nReset;
    CMN_DEBUG("self->nOrderBuy=[%d]\n", self->nOrderBuy);
    CMN_DEBUG("self->nFailedBuyOpen=[%d]\n", self->nFailedBuyOpen);
  }else{
    self->nOrderSell -= nReset;
    self->nFailedSellOpen += nReset;
    CMN_DEBUG("self->nOrderSell=[%d]\n", self->nOrderSell);
    CMN_DEBUG("self->nFailedSellOpen=[%d]\n", self->nFailedSellOpen);
  }
  
  return 0;
}

int trader_strategy_order_t2_close_failed(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // 从T1队列中移除
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // 重新计算T2价格
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  
  CMN_ERROR("t2_close_failed[%d]!\n", nReset);
  self->used = 0;

  // 更新可下单笔数
  if(TRADER_POSITION_LONG == pStrategyTrade->cLongShort){
    self->nPositionBuy -= nReset;
    self->nFronzeBuy -= nReset;
    self->nFailedBuyClose += nReset;
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
    CMN_DEBUG("self->nFronzeBuy=[%d]!\n", self->nFronzeBuy);
    CMN_DEBUG("self->nFailedBuyClose=[%d]!\n", self->nFailedBuyClose);
  }else{
    self->nPositionSell -= nReset;
    self->nFronzeSell -= nReset;
    self->nFailedSellClose += nReset;
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
    CMN_DEBUG("self->nFronzeSell=[%d]!\n", self->nFronzeSell);
    CMN_DEBUG("self->nFailedSellClose=[%d]!\n", self->nFailedSellClose);
  }
  
  return 0;
}


int trader_strategy_trade_t1_open(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder = NULL;
  char cBuySell;
  double lPrice;
  int nRet;
  int nT2Vol;
  trader_strategy_plan* pStrategyPlan = NULL;
  
  // 获取下单计划
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, trade_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //没找到报单
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  if(pStrategyPlan->nT1TradedVol >= pStrategyPlan->nPlanVol){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -2;
  }
  
  pStrategyPlan->nT1TradedVol += trade_data->TradeVolume;

  if(pStrategyPlan->nT1TradedVol == pStrategyPlan->nPlanVol){
    trader_strategy_release_order(self, trade_data->UserOrderLocalID);
  }
  

  // 获取T2方向
  cBuySell = trader_strategy_t2_direction(self, pStrategyPlan->cLongShort, TRADER_POSITION_OPEN);

  // 获取T2价格
  lPrice = trader_strategy_t2_price_opponent(self, cBuySell);

  // 计算T2手数
  nT2Vol = trade_data->TradeVolume * self->T2Ratio;

  nRet = trader_strategy_check_t2_price(self, lPrice);
  if(0 != nRet){
    CMN_ERROR("报单价格超过限制\n");
    return -1;
  }
  
  // 下单T2
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
  pStrategyTrade->cLongShort = pStrategyPlan->cLongShort;
  pStrategyTrade->T2Direction = cBuySell;
  pStrategyTrade->T2Offset = TRADER_POSITION_OPEN;
  pStrategyTrade->TradeVolume = nT2Vol;
  pStrategyTrade->T1Price = trade_data->TradePrice;
  pStrategyTrade->T2Price = lPrice;
  pStrategyTrade->T1TradeVolume = trade_data->TradeVolume;
  pStrategyTrade->T2TradeVolume = 0;
  pStrategyTrade->T2CancelNumber = 0;
  
  trader_strategy_insert_t2_open(self, pStrategyTrade);

  return 0;
}

int trader_strategy_trade_t1_close(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder = NULL;
  trader_strategy_plan* pStrategyPlan = NULL;
  char cBuySell;
  double lPrice;
  int nRet;

  // 获取下单计划
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, trade_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //没找到报单
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  if(pStrategyPlan->nT1TradedVol >= pStrategyPlan->nPlanVol){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -2;
  }
  
  pStrategyPlan->nT1TradedVol += trade_data->TradeVolume;

  if(pStrategyPlan->nT1TradedVol == pStrategyPlan->nPlanVol){
    trader_strategy_release_order(self, trade_data->UserOrderLocalID);
  }

  // 获取T2方向
  cBuySell = trader_strategy_t2_direction(self, pStrategyPlan->cLongShort, TRADER_POSITION_CLOSE);
  
  // 获取T2价格
  lPrice = trader_strategy_t2_price_opponent(self, cBuySell);

  nRet = trader_strategy_check_t2_price(self, lPrice);
  if(0 != nRet){
    CMN_ERROR("报单价格超过限制\n");
    return -1;
  }
  
  // 下单T2
  trader_strategy_insert_t2_close(self, pStrategyPlan->cLongShort, trade_data->TradeVolume, trade_data->TradePrice, lPrice);

  return 0;
}

int trader_strategy_trade_t2_open(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  
  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, trade_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }
  
  // 更新成交队列
  int tradedVolume;
  tradedVolume = (pStrategyTrade->T2TradeVolume % self->T2Ratio + trade_data->TradeVolume) / self->T2Ratio;
  
  pStrategyTrade->TradeVolume -= trade_data->TradeVolume;
  pStrategyTrade->T2TradeVolume += trade_data->TradeVolume;


  // 更新持仓信息
  if(TRADER_POSITION_LONG == pStrategyTrade->cLongShort){
    self->nPositionBuy += trade_data->TradeVolume;
    self->nOrderBuy -= trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
    CMN_DEBUG("self->nOrderBuy=[%d]!\n", self->nOrderBuy);
    trader_strategy_update_position(self, TRADER_POSITION_BUY, 
      tradedVolume, pStrategyTrade->T1Price, trade_data->TradePrice); 
  }else{
    self->nPositionSell += trade_data->TradeVolume;
    self->nOrderSell -= trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
    CMN_DEBUG("self->nOrderSell=[%d]!\n", self->nOrderSell);
    trader_strategy_update_position(self, TRADER_POSITION_SELL, 
      tradedVolume, pStrategyTrade->T1Price, trade_data->TradePrice); 
  }

  if(0 == pStrategyTrade->TradeVolume){
    trader_strategy_release_order(self, trade_data->UserOrderLocalID);
    // 全部成交，清理
    self->mapHalfTraded->pMethod->xRemove(self->mapHalfTraded, trade_data->UserOrderLocalID);
  }

  

  return 0;

}

int trader_strategy_trade_t2_close(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");

  // 查询StrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, trade_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }
  
  // 更新T1成交队列
  int tradedVolume;
  tradedVolume = (pStrategyTrade->T2TradeVolume % self->T2Ratio + trade_data->TradeVolume) / self->T2Ratio;
  
  pStrategyTrade->TradeVolume -= trade_data->TradeVolume;
  pStrategyTrade->T2TradeVolume += trade_data->TradeVolume;

  // 更新持仓信息
  if(TRADER_POSITION_LONG == pStrategyTrade->cLongShort){
    self->nPositionBuy -= trade_data->TradeVolume;
    self->nFronzeBuy -= trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
    CMN_DEBUG("self->nFronzeBuy=[%d]!\n", self->nFronzeBuy);
    trader_strategy_update_position(self, TRADER_POSITION_BUY, 
      -tradedVolume, 0.0f, 0.0f); 
  }else{
    self->nPositionSell -= trade_data->TradeVolume;
    self->nFronzeSell -= trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
    CMN_DEBUG("self->nFronzeSell=[%d]!\n", self->nFronzeSell);
    trader_strategy_update_position(self, TRADER_POSITION_SELL, 
      -tradedVolume, 0.0f, 0.0f); 
  }
  
  if(0 == pStrategyTrade->TradeVolume){
    trader_strategy_release_order(self, trade_data->UserOrderLocalID);
    // 全部成交，清理
    self->mapHalfTraded->pMethod->xRemove(self->mapHalfTraded, trade_data->UserOrderLocalID);
  }
  return 0;
}

int trader_strategy_insert_t1_open(trader_strategy* self, char long_short)
{
  CMN_DEBUG("sid[%02d]T1[%s]T2[%s]Enter!\n", self->idx, self->T1, self->T2);

  // 打印当前行情
  trader_strategy_print_tick(self);

  char sLocalUserId[21];  
  // 策略方向
  char cLongShort = long_short;

  // 报单方向
  char cT1Direction = trader_strategy_t1_direction(self, cLongShort, TRADER_POSITION_OPEN);

  int nSize1 = 0;
  int nSize2 = 0;
  // 计算下单量
  nSize1 = self->Mult;
  nSize2 = (self->MP * self->T2Ratio - self->nPositionBuy - self->nPositionSell);
  nSize2 /= self->T2Ratio;

  if(nSize2 <= 0){
    return -1;
  }
 
  if(nSize1 > nSize2){
    nSize1 = nSize2;
  }

  double lfT1Price = trader_strategy_t1_price(self, cLongShort, TRADER_POSITION_OPEN);
  double lfT2Price = trader_strategy_t2_price(self, cLongShort, TRADER_POSITION_OPEN);

  int nRet = 0;
  nRet = trader_strategy_check_t1_price(self, lfT1Price);
  if(0 != nRet){
    CMN_ERROR("报单价格超过限制\n");
    return -2;
  }

  // 获取订单号
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_OPEN_T1, sLocalUserId);

  // 创建下单计划
  trader_strategy_plan* pStrategyPlan = (trader_strategy_plan*)malloc(sizeof(trader_strategy_plan));
  strcpy(pStrategyPlan->UserOrderLocalID, sLocalUserId);
  
  pStrategyPlan->cOpenClose = TRADER_POSITION_OPEN;
  pStrategyPlan->cLongShort = cLongShort;
  
  pStrategyPlan->nPlanVol = nSize1;
  pStrategyPlan->nT1TradedVol = 0;
  pStrategyPlan->fT1Price = lfT1Price;
  pStrategyPlan->fT2Price = lfT2Price;

  // 添加到计划队列
  self->mapStrategyPlan->pMethod->xPut(self->mapStrategyPlan, sLocalUserId, (void*)pStrategyPlan);

  // 更新委托量
  if(TRADER_POSITION_LONG == cLongShort){
    // 做多开仓
    self->nOrderBuy += nSize1;
    CMN_DEBUG("self->nOrderBuy=[%d]!\n", self->nOrderBuy);
  }else{
    // 做空开仓
    self->nOrderSell += nSize1;
    CMN_DEBUG("self->nOrderSell=[%d]!\n", self->nOrderSell);
  }
  
  // 下单
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T1,
   cT1Direction, TRADER_POSITION_OPEN, pStrategyPlan->fT1Price, pStrategyPlan->nPlanVol, sLocalUserId);

  // 创建订单
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  pOrder->ExchangeID[0] = '\0';
  pOrder->OrderSysID[0] = '\0';
  strcpy(pOrder->InstrumentID, self->T1);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = cT1Direction;
  pOrder->OffsetFlag = TRADER_POSITION_OPEN;
  pOrder->LimitPrice = pStrategyPlan->fT1Price;
  pOrder->VolumeOriginal = pStrategyPlan->nPlanVol;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = TRADER_ORDER_OS_BEGIN;
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // 加入到订单列表
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", sLocalUserId, pOrder);

  // 未成交队列
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  pStrategyOrder->needCancel = 0;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);

  // 300毫秒撤单
  if(trader_strategy_is_guzhi(self)){
    trader_strategy_timed_cancel(self, sLocalUserId);
  }
  
  return 0;
}

int trader_strategy_insert_t1_close(trader_strategy* self, char long_short)
{
  CMN_DEBUG("sid[%02d]T1[%s]T2[%s]Enter!\n", self->idx, self->T1, self->T2);
  // 打印当前行情
  trader_strategy_print_tick(self);

  char cLongShort = long_short;

  int nSize1 = 0;
  int nSize2 = 0;
  int nSize3 = 0;
  
  if(TRADER_POSITION_LONG == cLongShort){
    nSize1 = self->nPositionBuy - self->hold * self->T2Ratio;
  }else{
    nSize1 = self->nPositionSell - self->hold * self->T2Ratio;
  }

  nSize1 /= self->T2Ratio;

  if(nSize1 > self->Mult){
    nSize1 = self->Mult;
  }

  // 获取可用报单笔数
  nSize2 = 0;
  nSize3 = 0;
  
  char cT1Direction = trader_strategy_t1_direction(self, cLongShort, TRADER_POSITION_CLOSE);
  trader_strategy_limit* pTraderStrategyLimit = self->pEngine->pTraderStrategyLimit;
  nSize2 = pTraderStrategyLimit->pMethod->xGetAvailable(pTraderStrategyLimit, 
    self->T1, cT1Direction, &nSize3);

  CMN_INFO("isSHFE[%d]Plan[%d]Available[%d]YdPosition[%d]\n",
    self->IsSHFE,
    nSize1,
    nSize2,
    nSize3);

  if(nSize1 > nSize2){
    nSize1 = nSize2;
  }

  if(nSize2 == 0){
    CMN_ERROR("isSHFE[%d]\n"
      "Plan[%d]\n"
      "Available[%d]\n"
      "YdPosition[%d]\n",
      self->IsSHFE,
      nSize1,
      nSize2,
      nSize3);
    return -1;
  }
  

  // 下单
  if(self->IsSHFE){
    if(nSize1 <= nSize3){
      // 全部平昨
      trader_strategy_t1_close_imp(self, cLongShort, TRADER_POSITION_CLOSE, nSize1);
    }else{
      if(nSize3 > 0){
        // 部分平昨
        trader_strategy_t1_close_imp(self, cLongShort, TRADER_POSITION_CLOSE, nSize3);
      }
      // 剩余平今
      trader_strategy_t1_close_imp(self, cLongShort, TRADER_POSITION_CLOSE_TODAY, nSize1 - nSize3);
    }
  }else{
    trader_strategy_t1_close_imp(self, cLongShort, TRADER_POSITION_CLOSE, nSize1);
  }

  return 0;
}

int trader_strategy_insert_t2_open(trader_strategy* self, trader_strategy_trade* strategy_trade)
{
  CMN_DEBUG("sid[%02d]T1[%s]T2[%s]Enter!\n", self->idx, self->T1, self->T2);
  char sLocalUserId[21];

  // 获取订单号
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_OPEN_T2, sLocalUserId);

  // 下单
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T2,
    strategy_trade->T2Direction, TRADER_POSITION_OPEN, 
    strategy_trade->T2Price, strategy_trade->TradeVolume, sLocalUserId);

  // 加入T1队列
  self->mapHalfTraded->pMethod->xPut(self->mapHalfTraded, sLocalUserId, strategy_trade);

  // 创建订单
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  pOrder->ExchangeID[0] = '\0';
  pOrder->OrderSysID[0] = '\0';
  strcpy(pOrder->InstrumentID, self->T2);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = strategy_trade->T2Direction;
  pOrder->OffsetFlag = TRADER_POSITION_OPEN;
  pOrder->LimitPrice = strategy_trade->T2Price;
  pOrder->VolumeOriginal = strategy_trade->TradeVolume;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = TRADER_ORDER_OS_BEGIN;
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // 加入到订单列表
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", sLocalUserId, pOrder);

  // 未成交队列
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  pStrategyOrder->needCancel = 0;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300毫秒撤单
  if(trader_strategy_is_guzhi(self)){
    if(0 == self->T2Wait){
      trader_strategy_timed_cancel(self, sLocalUserId);
    }
  }

  return 0;
}

int trader_strategy_insert_t2_close(trader_strategy* self, char long_short, int vol, double t1price, double t2price)
{
  CMN_DEBUG("sid[%02d]T1[%s]T2[%s]Enter!\n", self->idx, self->T1, self->T2);
  int nSize1 = vol * self->T2Ratio;
  int nSize2 = 0;
  int nSize3 = 0;
  char cLongShort = long_short;
  char cT2Direction = trader_strategy_t2_direction(self, cLongShort, TRADER_POSITION_CLOSE);
  int nT1TradeVolume = vol;
  
  trader_strategy_trade* pStrategyTrade;

  trader_strategy_limit* pTraderStrategyLimit = self->pEngine->pTraderStrategyLimit;
  nSize2 = pTraderStrategyLimit->pMethod->xGetAvailable(pTraderStrategyLimit, 
    self->T2, cT2Direction, &nSize3);

  CMN_INFO("isSHFE[%d]Plan[%d]Available[%d]YdPosition[%d]\n",
    self->T2IsSHFE,
    nSize1,
    nSize2,
    nSize3);

  
  if(nSize1 > nSize2){
    CMN_ERROR("isSHFE[%d]\n"
      "Plan[%d]\n"
      "Available[%d]\n"
      "YdPosition[%d]\n"
      "TdPosition[%d]\n",
      self->T2IsSHFE,
      nSize1,
      nSize2,
      nSize3);
    return -1;
  }

  // 下单
  if(self->T2IsSHFE){
    if(nSize1 <= nSize3){
      // 全部平昨
      CMN_DEBUG("全部平昨\n");
      pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
      pStrategyTrade->cLongShort = cLongShort;
      pStrategyTrade->T2Direction = cT2Direction;
      pStrategyTrade->T2Offset = TRADER_POSITION_CLOSE;
      pStrategyTrade->TradeVolume = nSize1;
      pStrategyTrade->T1Price = t1price;
      pStrategyTrade->T2Price = t2price;
      pStrategyTrade->T1TradeVolume = nT1TradeVolume;
      pStrategyTrade->T2TradeVolume = 0;
      pStrategyTrade->T2CancelNumber = 0;
      trader_strategy_t2_close_imp(self, pStrategyTrade);
    }else{
      if(nSize3 > 0){
         // 部分平昨
        CMN_DEBUG("部分平昨\n");
        pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
        pStrategyTrade->cLongShort = cLongShort;
        pStrategyTrade->T2Direction = cT2Direction;
        pStrategyTrade->T2Offset = TRADER_POSITION_CLOSE;
        pStrategyTrade->TradeVolume = nSize3;
        pStrategyTrade->T1Price = t1price;
        pStrategyTrade->T2Price = t2price;
        pStrategyTrade->T1TradeVolume = nSize3 / self->T2Ratio;
        pStrategyTrade->T2TradeVolume = 0;
        pStrategyTrade->T2CancelNumber = 0;
        trader_strategy_t2_close_imp(self, pStrategyTrade);
      }
        
      // 全部平今
      CMN_DEBUG("剩余平今\n");
      pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
      pStrategyTrade->cLongShort = cLongShort;
      pStrategyTrade->T2Direction = cT2Direction;
      pStrategyTrade->T2Offset = TRADER_POSITION_CLOSE_TODAY;
      pStrategyTrade->TradeVolume = nSize1 - nSize3;
      pStrategyTrade->T1Price = t1price;
      pStrategyTrade->T2Price = t2price;
      pStrategyTrade->T1TradeVolume = nT1TradeVolume - nSize3 / self->T2Ratio;
      pStrategyTrade->T2TradeVolume = 0;
      pStrategyTrade->T2CancelNumber = 0;
      trader_strategy_t2_close_imp(self, pStrategyTrade);
    }
  }else{
    pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
    pStrategyTrade->cLongShort = cLongShort;
    pStrategyTrade->T2Direction = cT2Direction;
    pStrategyTrade->T2Offset = TRADER_POSITION_CLOSE;
    pStrategyTrade->TradeVolume = nSize1;
    pStrategyTrade->T1Price = t1price;
    pStrategyTrade->T2Price = t2price;
    pStrategyTrade->T1TradeVolume = nT1TradeVolume;
    pStrategyTrade->T2TradeVolume = 0;
    pStrategyTrade->T2CancelNumber = 0;
    trader_strategy_t2_close_imp(self, pStrategyTrade);
  }

  return 0;

}

int trader_strategy_t1_close_imp(trader_strategy* self, char long_short, char offset, int vol)
{
  char sLocalUserId[21];
  int nSize1 = vol;
  char cCloseType = offset;
  char cLongShort = long_short;

  
  double lfT1Price = trader_strategy_t1_price(self, cLongShort, TRADER_POSITION_CLOSE);
  double lfT2Price = trader_strategy_t2_price(self, cLongShort, TRADER_POSITION_CLOSE);

  
  int nRet = 0;
  nRet = trader_strategy_check_t1_price(self, lfT1Price);
  if(0 != nRet){
    CMN_ERROR("报单价格超过限制\n");
    return -1;
  }
  
  // 报单方向
  char cT1Direction = trader_strategy_t1_direction(self, cLongShort, TRADER_POSITION_CLOSE);
  
  // 获取订单号
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_CLOSE_T1, sLocalUserId);

  // 创建下单计划
  trader_strategy_plan* pStrategyPlan = (trader_strategy_plan*)malloc(sizeof(trader_strategy_plan));
  strcpy(pStrategyPlan->UserOrderLocalID, sLocalUserId);
  pStrategyPlan->cOpenClose = TRADER_POSITION_CLOSE;
  pStrategyPlan->cLongShort = cLongShort;
  
  if(TRADER_POSITION_LONG == cLongShort){
    self->nFronzeBuy += nSize1; 
    CMN_DEBUG("self->nFronzeBuy=[%d]!\n", self->nFronzeBuy);
  }else{
    self->nFronzeSell += nSize1; 
    CMN_DEBUG("self->nFronzeSell=[%d]!\n", self->nFronzeSell);
  }
  pStrategyPlan->nPlanVol = nSize1;
  pStrategyPlan->nT1TradedVol = 0;
  pStrategyPlan->fT1Price = lfT1Price;
  pStrategyPlan->fT2Price = lfT2Price;

  // 添加到计划队列
  self->mapStrategyPlan->pMethod->xPut(self->mapStrategyPlan, sLocalUserId, pStrategyPlan);
  
  // 下单
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T1,
    cT1Direction, cCloseType,  pStrategyPlan->fT1Price, 
    pStrategyPlan->nPlanVol, sLocalUserId);

  // 创建订单
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  pOrder->ExchangeID[0] = '\0';
  pOrder->OrderSysID[0] = '\0';
  strcpy(pOrder->InstrumentID, self->T1);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = cT1Direction;
  pOrder->OffsetFlag = cCloseType;
  pOrder->LimitPrice = pStrategyPlan->fT1Price;
  pOrder->VolumeOriginal = pStrategyPlan->nPlanVol;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = TRADER_ORDER_OS_BEGIN;
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  trader_strategy_limit* pTraderStrategyLimit = self->pEngine->pTraderStrategyLimit;
  pTraderStrategyLimit->pMethod->xOnOrder(pTraderStrategyLimit, pOrder, self->IsSHFE);

  // 加入到订单列表
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", sLocalUserId, pOrder);

  // 未成交队列
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  pStrategyOrder->needCancel = 0;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300毫秒撤单
  if(trader_strategy_is_guzhi(self)){
    trader_strategy_timed_cancel(self, sLocalUserId);
  }

  return 0;
}

int trader_strategy_t2_close_imp(trader_strategy* self, trader_strategy_trade* strategy_trade)
{
  CMN_DEBUG("Enter!\n");
  char sLocalUserId[21];

  // 获取订单号
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_CLOSE_T2, sLocalUserId);

  // 下单
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T2,
    strategy_trade->T2Direction, strategy_trade->T2Offset, 
    strategy_trade->T2Price, strategy_trade->TradeVolume, sLocalUserId);

  // 加入T1队列
  self->mapHalfTraded->pMethod->xPut(self->mapHalfTraded, sLocalUserId, strategy_trade);

  // 创建订单
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  pOrder->ExchangeID[0] = '\0';
  pOrder->OrderSysID[0] = '\0';
  strcpy(pOrder->InstrumentID, self->T2);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = strategy_trade->T2Direction;
  pOrder->OffsetFlag = strategy_trade->T2Offset;
  pOrder->LimitPrice = strategy_trade->T2Price;
  pOrder->VolumeOriginal = strategy_trade->TradeVolume;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = TRADER_ORDER_OS_BEGIN;
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));
  
  trader_strategy_limit* pTraderStrategyLimit = self->pEngine->pTraderStrategyLimit;
  pTraderStrategyLimit->pMethod->xOnOrder(pTraderStrategyLimit, pOrder, self->T2IsSHFE);
  
  // 加入到订单列表
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", sLocalUserId, pOrder);

  // 未成交队列
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  pStrategyOrder->needCancel = 0;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300毫秒撤单
  if(trader_strategy_is_guzhi(self)){
    if(0 == self->T2Wait){
      trader_strategy_timed_cancel(self, sLocalUserId);
    }
  }

  return 0;

}

int trader_strategy_cancel_order(trader_strategy* self, char* user_order_id)
{
  CMN_DEBUG("Enter!\n");
  char sCancelOrderId[21];
  char cOrderType;
  
  trader_order* pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, user_order_id);
  CMN_DEBUG("user_order_id[%s]pOrder[%x]!\n", user_order_id, pOrder);
  if(!pOrder){
    return -1;
  }
  
  // 打印当前行情
  trader_strategy_print_tick(self);
  
  CMN_INFO(
    "pOrder->ExchangeID=[%s]\n"
    "pOrder->OrderSysID=[%s]\n"
    "pOrder->InstrumentID=[%s]\n"
    "pOrder->UserOrderLocalID=[%s]\n"
    "pOrder->OrderStatus=[%c]\n",
    pOrder->ExchangeID,
    pOrder->OrderSysID,
    pOrder->InstrumentID,
    pOrder->UserOrderLocalID,
    pOrder->OrderStatus
  );

  if((TRADER_ORDER_OS_PARTTRADEDQUEUEING != pOrder->OrderStatus)
  &&(TRADER_ORDER_OS_NOTRADEQUEUEING != pOrder->OrderStatus)){
    CMN_ERROR("pOrder->OrderStatus=[%c]\n", pOrder->OrderStatus);
    return -2;
  }
  
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, user_order_id);
  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T2_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T2_ACTION, sCancelOrderId);
  }

  self->pEngine->pMethod->xCancelOrder(self->pEngine,
    pOrder->InstrumentID, sCancelOrderId, user_order_id,
    pOrder->ExchangeID, pOrder->OrderSysID);

  return 0;
  
}

int trader_strategy_timed_cancel(trader_strategy* self, char* user_order_id)
{
  CMN_DEBUG("Enter!\n");
  char sCancelOrderId[21];
  char cOrderType;
  
  trader_order* pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, user_order_id);
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, user_order_id);
  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T2_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T2_ACTION, sCancelOrderId);
  }

  self->pEngine->pMethod->xSetTimer(self->pEngine,
    pOrder->InstrumentID, sCancelOrderId, user_order_id,
    pOrder->ExchangeID, pOrder->OrderSysID);

  return 0;
}

int trader_strategy_release_order(trader_strategy* self, char* user_order_id)
{

  // 从订单队列删除
  trader_order* pOrder = (trader_order*)self->mapOrder->pMethod->xGet(self->mapOrder, user_order_id);
  if(pOrder){
    //保存报单数据
    self->pEngine->pMethod->xSaveOrder(self->pEngine, pOrder);
    // 释放空间
    free(pOrder);
    self->mapOrder->pMethod->xRemove(self->mapOrder, user_order_id);
  }

  // 从待成交队列中删除
  trader_strategy_order* pStrategyOrder = NULL;
  trader_strategy_order* pIter;

  TAILQ_FOREACH(pIter, &self->listNotFinishedOrder,  next){
    if(!strcmp(pIter->ref, user_order_id)){
      pStrategyOrder = pIter;
    }
  }

  if(!pStrategyOrder){
    // 没有找到
    CMN_WARN("[%s] listNotFinishedOrder not found!\n", user_order_id);
    return -1;
  }
  
  TAILQ_REMOVE(&self->listNotFinishedOrder, pStrategyOrder, next);

  free(pStrategyOrder);

  return 0;
}

int trader_strategy_print_tick(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  CMN_INFO("InstrumentID[%s]\n", t1->InstrumentID);
  CMN_INFO("BidPrice1[%lf]\n", t1->BidPrice1);
  CMN_INFO("BidVolume1[%d]\n", t1->BidVolume1);
  CMN_INFO("AskPrice1[%lf]\n", t1->AskPrice1);
  CMN_INFO("AskVolume1[%d]\n", t1->AskVolume1);

  CMN_INFO("InstrumentID[%s]\n", t2->InstrumentID);
  CMN_INFO("BidPrice1[%lf]\n", t2->BidPrice1);
  CMN_INFO("BidVolume1[%d]\n", t2->BidVolume1);
  CMN_INFO("AskPrice1[%lf]\n", t2->AskPrice1);
  CMN_INFO("AskVolume1[%d]\n", t2->AskVolume1);

  return 0;
}

int trader_strategy_check_closing(trader_strategy* self, trader_tick* tick_data)
{
  if(!self->used){
    return 0;
  }

  do{
    //中午收盘判断
    if((0 == memcmp(tick_data->UpdateTime, "11:29:57", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "11:29:58", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "11:29:59", 8))){
      CMN_INFO("中午收盘\n");
      self->used = 0;
      break;
    }

    // 下午收盘判断
    if(IS_CLOSING_15_00(self->NightClosingTime))
    if((0 == memcmp(tick_data->UpdateTime, "14:59:57", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "14:59:58", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "14:59:59", 8))){
      CMN_INFO("下午收盘\n");
      self->used = 0;
      break;
    }

    // 夜盘收盘判断
    if(CLOSING_TIME_NONE == self->NightClosingTime){
      break;
    }

    if(IS_CLOSING_23_00(self->NightClosingTime)){
      if((0 == memcmp(tick_data->UpdateTime, "22:59:57", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "22:59:58", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "22:59:59", 8))){
        CMN_INFO("23点收盘\n");
        self->used = 0;
        break;
      }
    }

    if(IS_CLOSING_23_30(self->NightClosingTime)){
      if((0 == memcmp(tick_data->UpdateTime, "23:29:57", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "23:29:58", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "23:29:59", 8))){
        CMN_INFO("23点30分收盘\n");
        self->used = 0;
        break;
      }
    }

    if(IS_CLOSING_01_00(self->NightClosingTime)){
      if((0 == memcmp(tick_data->UpdateTime, "00:59:57", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "00:59:58", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "00:59:59", 8))){
        CMN_INFO("1点收盘\n");
        self->used = 0;
        break;
      }
    }

    if(IS_CLOSING_02_00(self->NightClosingTime)){
      if((0 == memcmp(tick_data->UpdateTime, "01:59:57", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "01:59:58", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "01:59:59", 8))){
        CMN_INFO("2点收盘\n");
        self->used = 0;
        break;
      }
    }

    if(IS_CLOSING_02_30(self->NightClosingTime)){
      if((0 == memcmp(tick_data->UpdateTime, "02:29:57", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "02:29:58", 8))
      ||(0 == memcmp(tick_data->UpdateTime, "02:29:59", 8))){
        CMN_INFO("2点30分收盘\n");
        self->used = 0;
        break;
      }
    }
  }while(0);

  return 0;
}

int trader_strategy_tick_trigger(trader_strategy* self, trader_tick* tick_data)
{

  if(TRIGGER_TYPE_0 == self->TriggerType){
    self->TriggerType = 0;
    if((0 == strcmp(self->oT1Tick.UpdateTime, self->oT2Tick.UpdateTime))
    && (self->oT1Tick.UpdateMillisec == self->oT2Tick.UpdateMillisec)){  
      if(0 == strcmp(self->T1, tick_data->InstrumentID)){
        self->TriggerType = TRIGGER_TYPE_1;
      }else{
        self->TriggerType = TRIGGER_TYPE_2;
      }
      CMN_INFO("self->TriggerType=[%d]self->T1=[%s]self->T2=[%s]\n", self->TriggerType, self->T1, self->T2);
      return 1;
    }
  }
  
  if(TRIGGER_TYPE_1 == self->TriggerType){
    if(0 == strcmp(self->T1, tick_data->InstrumentID)){
      return 1;
    }
  }
  
  if(TRIGGER_TYPE_2 == self->TriggerType){
    if(0 == strcmp(self->T2, tick_data->InstrumentID)){
      return 1;
    }
  }
  
  if(TRIGGER_TYPE_4 == self->TriggerType){
    if((0 == strcmp(self->oT1Tick.UpdateTime, self->oT2Tick.UpdateTime))
    && (self->oT1Tick.UpdateMillisec == self->oT2Tick.UpdateMillisec)){  
      return 1;
    }
  }

  if(TRIGGER_TYPE_3 == self->TriggerType){
    if((0 == memcmp(tick_data->UpdateTime, "09:00:00", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "10:15:00", 8))
    ||(0 == memcmp(tick_data->UpdateTime, "13:30:00", 8))){
      CMN_INFO("盘中重新开盘\n");
      if((0 == strcmp(self->oT1Tick.UpdateTime, self->oT2Tick.UpdateTime))
      && (self->oT1Tick.UpdateMillisec == self->oT2Tick.UpdateMillisec)){  
        return 1;
      }
      return 0;
    }
    
    if((self->oT1Tick.AskVolume1 + self->oT1Tick.BidVolume1) > (self->oT2Tick.AskVolume1 + self->oT2Tick.BidVolume1)){
      // T1 为主力合约
      if(0 == strcmp(self->T1, tick_data->InstrumentID)){
        return 1;
      }
    }else{
      // T2 为主力合约
      if(0 == strcmp(self->T2, tick_data->InstrumentID)){
        return 1;
      }
    }
  }

  return 0;

}

int trader_strategy_print_order_time(trader_strategy* self, trader_order* order_data)
{
  struct timeval* begin = &order_data->CreateTime;
  struct timeval* end = &order_data->UpdateTime;
  long sec = end->tv_sec - begin->tv_sec;
  long usec = end->tv_usec - begin->tv_usec;
  if(usec < 0){
    sec--;
    usec += 1000000;
  }
  
  CMN_INFO("{ConsumingTime}ExchangeID=[%s]"
    "InstrumentID=[%s]"
    "UserOrderLocalID=[%s]"
    "Cost=[%ld.%06ld]\n"
    , order_data->ExchangeID
    , order_data->InstrumentID
    , order_data->UserOrderLocalID
    , sec
    , usec
  );

  return 0;
}

trader_strategy* trader_strategy_new()
{
  trader_strategy* self = (trader_strategy*)malloc(sizeof(trader_strategy));

  self->pMethod = trader_strategy_method_get();

  return self;
}

void trader_strategy_free(trader_strategy* self)
{
  // 订单队列
  if(self->mapOrder){
    cmn_util_map_free(self->mapOrder);
  }
  
  // 计划队列
  if(self->mapOrder){
    cmn_util_map_free(self->mapStrategyPlan);
  }

  // T1成交队列
  if(self->mapOrder){
    cmn_util_map_free(self->mapHalfTraded);
  }  
  
  if(self){
    free(self);
  }

}



