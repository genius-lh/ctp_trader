
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmn_log.h"
#include "cmn_util.h"
#include "trader_strategy.h"
#include "trader_data.h"
#include "trader_msg_struct.h"
#include "trader_trader_api.h"

#include "trader_strategy_engine.h"

static int trader_strategy_engine_init(trader_strategy_engine* self);
static int trader_strategy_engine_update_strategy(trader_strategy_engine* self, struct trader_cmd_update_req_def* strategy_data);
static int trader_strategy_engine_update_tick(trader_strategy_engine* self, trader_tick* tick_data);
static int trader_strategy_engine_update_trade(trader_strategy_engine* self, trader_trade* trade_data);
static int trader_strategy_engine_update_order(trader_strategy_engine* self, trader_order* order_data);
static int trader_strategy_engine_update_status(trader_strategy_engine* self, instrument_status* status_data);
static int trader_strategy_engine_send_order(trader_strategy_engine* self, trader_strategy* strategy, char* contract, char direction, char offset, double price, int volume, char* user_local_id);
static int trader_strategy_engine_cancel_order(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id, char*exchange_id, char* order_sys_id);
static int trader_strategy_engine_set_timer(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id, char*exchange_id, char* order_sys_id);

static int trader_strategy_engine_trading_day_set(trader_strategy_engine* self, char* day);
static int trader_strategy_engine_local_user_id_set(trader_strategy_engine* self, long user_id);
static int trader_strategy_engine_gen_local_id(trader_strategy_engine* self, char type, char* local_id);
static char trader_strategy_engine_get_order_type(trader_strategy_engine* self, char* local_id);

static int trader_strategy_engine_update_amount(trader_strategy_engine* self, int vol);
static int trader_strategy_engine_get_rest(trader_strategy_engine* self);

static int trader_strategy_engine_close_all(trader_strategy_engine* self);
static int trader_strategy_engine_reset_position(trader_strategy_engine* self, int stage_id, char buy_sell, int volume);
static int trader_strategy_engine_query_position(trader_strategy_engine* self, int stage_id, char buy_sell, trade_position* p_position);

static int trader_strategy_engine_save_order(trader_strategy_engine* self, trader_order* pTraderOrder);
static int trader_strategy_engine_save_trade(trader_strategy_engine* self, trader_trade* pTraderTrade);

static int trader_strategy_engine_init_investor_position(trader_strategy_engine* self, investor_position* pInvestorPsition);

static trader_strategy_engine_method* trader_strategy_engine_method_get();

static void  trader_strategy_engine_timeout_cb(evutil_socket_t fd, short event, void *arg);

static void trader_strategy_engine_status_timer_init(trader_strategy_engine* self);

static void trader_strategy_engine_status_timer_tick(trader_strategy_engine* self, char* pUpdateTime, int pUpdateMillisec);

static void  trader_strategy_engine_status_timer_timeout_cb(evutil_socket_t fd, short event, void *arg);

static void trader_strategy_engine_status_timer_event(trader_strategy_engine* self);

trader_strategy_engine_method* trader_strategy_engine_method_get()
{
  static trader_strategy_engine_method trader_strategy_engine_method_st = {
    trader_strategy_engine_init,
    trader_strategy_engine_update_strategy,
    trader_strategy_engine_update_tick,
    trader_strategy_engine_update_trade,
    trader_strategy_engine_update_order,
    trader_strategy_engine_update_status,
    trader_strategy_engine_send_order,
    trader_strategy_engine_cancel_order,
    trader_strategy_engine_set_timer,
    trader_strategy_engine_trading_day_set,
    trader_strategy_engine_local_user_id_set,
    trader_strategy_engine_gen_local_id,
    trader_strategy_engine_get_order_type,
    trader_strategy_engine_update_amount,
    trader_strategy_engine_get_rest,
    trader_strategy_engine_close_all,
    trader_strategy_engine_reset_position,
    trader_strategy_engine_query_position,
    trader_strategy_engine_save_order,
    trader_strategy_engine_save_trade,
    trader_strategy_engine_init_investor_position
  };
  return &trader_strategy_engine_method_st;
}

int trader_strategy_engine_init(trader_strategy_engine* self)
{
  // 获取当前时间
  // 计算下一个时间节点
  trader_strategy_engine_status_timer_init(self);

  return 0;
}

int trader_strategy_engine_update_strategy(trader_strategy_engine* self, struct trader_cmd_update_req_def* strategy_data)
{
  struct trader_cmd_update_req_def *pUpdate = strategy_data;
  int i;
  int idx;
  trader_strategy* pStrategy;
  CMN_INFO("UID[%s]\n", self->pCtpTraderApi->pUser);
  
  for(i = 0; i < pUpdate->num; i++){
    idx = pUpdate->stage[i].StageId;
    pStrategy = self->trader_strategys[idx];

  	CMN_INFO("StageId[%d]\n", pUpdate->stage[i].StageId);
  	CMN_INFO("T1[%s]\n", pUpdate->stage[i].T1);
  	CMN_INFO("T2[%s]\n", pUpdate->stage[i].T2);
  	CMN_INFO("MP[%d]\n", pUpdate->stage[i].MP);
  	CMN_INFO("Hold[%d]\n", pUpdate->stage[i].Hold);
  	CMN_INFO("Mult[%d]\n", pUpdate->stage[i].Mult);
  	CMN_INFO("KTOpen[%lf]\n", pUpdate->stage[i].KTOpen);
  	CMN_INFO("KTClose[%lf]\n", pUpdate->stage[i].KTClose);
  	CMN_INFO("DTOpen[%lf]\n", pUpdate->stage[i].DTOpen);
  	CMN_INFO("DTClose[%lf]\n", pUpdate->stage[i].DTClose);
  	CMN_INFO("T1Over[%d]\n", pUpdate->stage[i].T1Over);
  	CMN_INFO("T1Wait[%d]\n", pUpdate->stage[i].T1Wait);
  	CMN_INFO("T2Over[%d]\n", pUpdate->stage[i].T2Over);
  	CMN_INFO("T2Wait[%d]\n", pUpdate->stage[i].T2Wait);
  	CMN_INFO("PermitVol[%d]\n", pUpdate->stage[i].PermitVol);
  	CMN_INFO("STG[%d]\n", pUpdate->stage[i].STG);
  	CMN_INFO("Used[%d]\n", pUpdate->stage[i].Used);
  	CMN_INFO("AutoType[%d]\n", pUpdate->stage[i].AutoType);
  	CMN_INFO("AutoKTOpen[%lf]\n", pUpdate->stage[i].AutoKTOpen);
  	CMN_INFO("AutoKTClose[%lf]\n", pUpdate->stage[i].AutoKTClose);
  	CMN_INFO("AutoDTOpen[%lf]\n", pUpdate->stage[i].AutoDTOpen);
  	CMN_INFO("AutoDTClose[%lf]\n", pUpdate->stage[i].AutoDTClose);
  	CMN_INFO("T1Weight[%lf]\n", pUpdate->stage[i].T1Weight);
  	CMN_INFO("T2Weight[%lf]\n", pUpdate->stage[i].T2Weight);
  	CMN_INFO("T2Ratio[%d]\n", pUpdate->stage[i].T2Ratio);
  	CMN_INFO("NightClosingTime[%d]\n", pUpdate->stage[i].NightClosingTime);
  	CMN_INFO("TriggerType[%d]\n", pUpdate->stage[i].TriggerType);
  	CMN_INFO("PriceTick[%lf]\n", pUpdate->stage[i].PriceTick);
  	CMN_INFO("IsSHFE[%d]\n", pUpdate->stage[i].IsSHFE);
  	CMN_INFO("T2PriceTick[%lf]\n", pUpdate->stage[i].T2PriceTick);
  	CMN_INFO("T2IsSHFE[%d]\n", pUpdate->stage[i].T2IsSHFE);
    
    pStrategy->idx = pUpdate->stage[i].StageId;
    strcpy(pStrategy->T1, pUpdate->stage[i].T1);
    strcpy(pStrategy->T2, pUpdate->stage[i].T2);
    pStrategy->MP = pUpdate->stage[i].MP;
    pStrategy->hold = pUpdate->stage[i].Hold;
    pStrategy->Mult = pUpdate->stage[i].Mult;
    pStrategy->KTOpen = pUpdate->stage[i].KTOpen;
    pStrategy->KTClose = pUpdate->stage[i].KTClose;
    pStrategy->DTOpen = pUpdate->stage[i].DTOpen;
    pStrategy->DTClose = pUpdate->stage[i].DTClose;
    pStrategy->T1Over = pUpdate->stage[i].T1Over;
    pStrategy->T1Wait = pUpdate->stage[i].T1Wait;
    pStrategy->T2Over = pUpdate->stage[i].T2Over;
    pStrategy->T2Wait = pUpdate->stage[i].T2Wait;
    pStrategy->PermitVol = pUpdate->stage[i].PermitVol;
    pStrategy->STG = pUpdate->stage[i].STG;
    pStrategy->used = pUpdate->stage[i].Used; 
    pStrategy->AutoType = pUpdate->stage[i].AutoType; 
    pStrategy->AutoKTOpen = pUpdate->stage[i].AutoKTOpen; 
    pStrategy->AutoKTClose = pUpdate->stage[i].AutoKTClose; 
    pStrategy->AutoDTOpen = pUpdate->stage[i].AutoDTOpen; 
    pStrategy->AutoDTClose = pUpdate->stage[i].AutoDTClose; 
    pStrategy->T1Weight = pUpdate->stage[i].T1Weight; 
    pStrategy->T2Weight = pUpdate->stage[i].T2Weight; 
    pStrategy->T2Ratio = pUpdate->stage[i].T2Ratio; 
    pStrategy->NightClosingTime = pUpdate->stage[i].NightClosingTime; 
    pStrategy->TriggerType = pUpdate->stage[i].TriggerType; 
    pStrategy->PriceTick = pUpdate->stage[i].PriceTick;
    pStrategy->IsSHFE = pUpdate->stage[i].IsSHFE;
    pStrategy->T2PriceTick = pUpdate->stage[i].T2PriceTick;
    pStrategy->T2IsSHFE = pUpdate->stage[i].T2IsSHFE;

    strcpy(pStrategy->oBuyPosition.T1, pStrategy->T1);
    strcpy(pStrategy->oBuyPosition.T2, pStrategy->T2);
    strcpy(pStrategy->oSellPosition.T1, pStrategy->T1);
    strcpy(pStrategy->oSellPosition.T2, pStrategy->T2);

  }
  
  return 0;
}


int trader_strategy_engine_update_tick(trader_strategy_engine* self, trader_tick* tick_data)
{
  int i = 0;
  trader_strategy* pStrategy;
  
  trader_tick oTick;
  
  strcpy(oTick.InstrumentID, tick_data->InstrumentID);
  strcpy(oTick.TradingDay, tick_data->TradingDay);
  strcpy(oTick.UpdateTime, tick_data->UpdateTime);
  oTick.UpdateMillisec = tick_data->UpdateMillisec;
  oTick.BidPrice1 = tick_data->BidPrice1;
  oTick.BidVolume1 = tick_data->BidVolume1;
  oTick.AskPrice1 = tick_data->AskPrice1;
  oTick.AskVolume1 = tick_data->AskVolume1;
  oTick.UpperLimitPrice = tick_data->UpperLimitPrice;
  oTick.LowerLimitPrice = tick_data->LowerLimitPrice;

  //CMN_DEBUG("推送Tick数据\n");
  //CMN_INFO("tick[%s]UpdateTime[%s]UpdateMillisec[%d]\n", tick_data->InstrumentID, tick_data->UpdateTime, tick_data->UpdateMillisec);

  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pStrategy = self->trader_strategys[i];
    pStrategy->pMethod->xOnTick(pStrategy, &oTick);
  }

  trader_strategy_engine_status_timer_tick(self, oTick.UpdateTime, oTick.UpdateMillisec);

  return 0;
}

int trader_strategy_engine_update_trade(trader_strategy_engine* self, trader_trade* trade_data)
{
  trader_strategy* pStrategy;
  char* pLocalId = trade_data->UserOrderLocalID;

  
  pStrategy = (trader_strategy*)self->orderStrategyMap->pMethod->xGet(self->orderStrategyMap, pLocalId);
  if(!pStrategy){
    CMN_ERROR("trader_strategy not found pLocalId=[%s]\n", pLocalId);
    return -1;
  }

  int isSHFE = pStrategy->IsSHFE;
  if(!strcmp(pStrategy->T2, trade_data->InstrumentID)){
    isSHFE = pStrategy->T2IsSHFE;
  }
  
  // 更新总限仓
  self->pTraderStrategyLimit->pMethod->xOnTrade(self->pTraderStrategyLimit,
    trade_data, isSHFE);

  trader_trade oTrade;
  trader_trade* pTrade = &oTrade;
  strcpy(pTrade->InstrumentID, trade_data->InstrumentID);
  strcpy(pTrade->UserOrderLocalID, trade_data->UserOrderLocalID);
  strcpy(pTrade->TradingDay, trade_data->TradingDay);
  strcpy(pTrade->TradeTime, trade_data->TradeTime);
  pTrade->Direction = trade_data->Direction;
  pTrade->OffsetFlag = trade_data->OffsetFlag;
  pTrade->TradePrice = trade_data->TradePrice;
  pTrade->TradeVolume = trade_data->TradeVolume;
  strcpy(pTrade->TradeID, trade_data->TradeID);
  
  CMN_DEBUG("推送Trade数据\n");
  pStrategy->pMethod->xOnTrade(pStrategy, pTrade);

  return 0;

}


int trader_strategy_engine_update_order(trader_strategy_engine* self, trader_order* order_data)
{
  trader_strategy* pStrategy;
  char* pLocalId = order_data->UserOrderLocalID;
  
  pStrategy = (trader_strategy*)self->orderStrategyMap->pMethod->xGet(self->orderStrategyMap, pLocalId);
  if(!pStrategy){
    CMN_ERROR("trader_strategy not found pLocalId=[%s]\n", pLocalId);
    return -1;
  }

  int isSHFE = pStrategy->IsSHFE;
  if(!strcmp(pStrategy->T2, order_data->InstrumentID)){
    isSHFE = pStrategy->T2IsSHFE;
  }

  // 更新总限仓
  self->pTraderStrategyLimit->pMethod->xOnOrder(self->pTraderStrategyLimit,
    order_data, isSHFE);

  trader_order oOrder;
  trader_order* pOrder = &oOrder;  
  strcpy(pOrder->ExchangeID, order_data->ExchangeID);
  strcpy(pOrder->OrderSysID, order_data->OrderSysID);
  strcpy(pOrder->InstrumentID, order_data->InstrumentID);
  strcpy(pOrder->UserOrderLocalID, order_data->UserOrderLocalID);
  pOrder->Direction = order_data->Direction;
  pOrder->OffsetFlag = order_data->OffsetFlag;
  pOrder->HedgeFlag = order_data->HedgeFlag;
  pOrder->LimitPrice = order_data->LimitPrice;
  pOrder->VolumeOriginal = order_data->VolumeOriginal;
  pOrder->VolumeTraded = order_data->VolumeTraded;
  pOrder->OrderStatus = order_data->OrderStatus;
  strcpy(pOrder->InsertTime, order_data->InsertTime);

  CMN_DEBUG("推送pOrder数据\n");
  pStrategy->pMethod->xOnOrder(pStrategy, pOrder);
}

int trader_strategy_engine_update_status(trader_strategy_engine* self, instrument_status* status_data)
{
  int i = 0;
  trader_strategy* pStrategy;
  
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pStrategy = self->trader_strategys[i];
    pStrategy->pMethod->xOnStatus(pStrategy, status_data);
  }

  return 0;
}


int trader_strategy_engine_send_order(trader_strategy_engine* self, trader_strategy* strategy, char* contract, char direction, char offset, double price, int volume, char* user_local_id)
{
  CMN_INFO("报单数据\n"
    "\tInstrument[%s]\n"
    "\tDirection[%c]\n"
    "\tOffset[%c]\n"
    "\tPrice[%12.4lf]\n"
    "\tVolume[%d]\n"
    "\tUserLocalId[%s]\n",
    contract,
    direction,
    offset,
    price,
    volume,
    user_local_id
  );
  
  // 下单
  self->pCtpTraderApi->pMethod->xOrderInsert(self->pCtpTraderApi, 
    contract, user_local_id, direction, offset, price, volume);
  
  self->orderStrategyMap->pMethod->xPut(self->orderStrategyMap, user_local_id, (void*)strategy);

  return 0;
}

int trader_strategy_engine_cancel_order(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id, char* exchange_id, char* order_sys_id)
{
  // 撤单
  self->pCtpTraderApi->pMethod->xOrderAction(self->pCtpTraderApi, 
    contract, user_local_id, org_user_local_id, exchange_id, order_sys_id);

  CMN_DEBUG("OUT\n");

  return 0;

}

int trader_strategy_engine_set_timer(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id, char*exchange_id, char* order_sys_id)
{
  trader_strategy_engine_action_param* param = (trader_strategy_engine_action_param*)malloc(sizeof(trader_strategy_engine_action_param));
  param->parent = self;
  
  param->contract = (char*)malloc(sizeof(char)*(strlen(contract) + 1));
  strcpy(param->contract, contract);
  
  param->user_local_id = (char*)malloc(sizeof(char)*(strlen(user_local_id) + 1));
  strcpy(param->user_local_id, user_local_id);
  
  param->org_user_local_id = (char*)malloc(sizeof(char)*(strlen(org_user_local_id) + 1));
  strcpy(param->org_user_local_id, org_user_local_id);

  param->exchange_id = (char*)malloc(sizeof(char)*(strlen(exchange_id) + 1));
  strcpy(param->exchange_id, exchange_id);

  param->order_sys_id = (char*)malloc(sizeof(char)*(strlen(order_sys_id) + 1));
  strcpy(param->order_sys_id, order_sys_id);

  param->evTimer = evtimer_new(self->pBase, trader_strategy_engine_timeout_cb, (void*)param);

  
  static struct timeval t1_timeout = {
    0, 300*1000
  };
  evtimer_add(param->evTimer, &t1_timeout);
  
  return 0;
}

int trader_strategy_engine_trading_day_set(trader_strategy_engine* self, char* day)
{
  strncpy(self->TradingDay, day, sizeof(self->TradingDay));
  return 0;
}

int trader_strategy_engine_local_user_id_set(trader_strategy_engine* self, long user_id)
{
  long nSeq = user_id;

  if(nSeq > self->nSequence){
    self->nSequence = nSeq;
  }
  self->nSequence++;

  return 0;
}

int trader_strategy_engine_gen_local_id(trader_strategy_engine* self, char type, char* local_id)
{
  sprintf(local_id, "%07d%c", self->nSequence, type);
  self->nSequence++;
  return 0;
}

char trader_strategy_engine_get_order_type(trader_strategy_engine* self, char* local_id)
{
  return local_id[7];
}

int trader_strategy_engine_update_amount(trader_strategy_engine* self, int vol)
{
  self->nPositionSize += vol;
  return 0;
}

int trader_strategy_engine_get_rest(trader_strategy_engine* self)
{
  return self->nPositionSize;
}

int trader_strategy_engine_close_all(trader_strategy_engine* self)
{
  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = self->trader_strategys[i];
    pTraderStrategy->used = 0;
  }
  return 0;
}

int trader_strategy_engine_reset_position(trader_strategy_engine* self, int stage_id, char buy_sell, int volume)
{
  int nRet = 0;
  trader_strategy* pTraderStrategy;
  pTraderStrategy = self->trader_strategys[stage_id];

  nRet = pTraderStrategy->pMethod->xResetPosition(pTraderStrategy, buy_sell, volume);
  
  return nRet;
}

int trader_strategy_engine_query_position(trader_strategy_engine* self, int stage_id, char buy_sell, trade_position* p_position)
{
  //CMN_DEBUG("Enter!\n");
  int nRet = 0;
  trader_strategy* pTraderStrategy;
  pTraderStrategy = self->trader_strategys[stage_id];

  nRet = pTraderStrategy->pMethod->xQueryPosition(pTraderStrategy, buy_sell, p_position);
  
  return nRet;
}

int trader_strategy_engine_save_order(trader_strategy_engine* self, trader_order* pTraderOrder)
{
  CMN_DEBUG("Enter!\n");
  struct trader_db_order_inf_def oOrderInf;
  int nRet;
  int nSqlCode = 0;

  strncpy(oOrderInf.UserOrderLocalID, pTraderOrder->UserOrderLocalID, sizeof(oOrderInf.UserOrderLocalID));
  strncpy(oOrderInf.InstrumentID, pTraderOrder->InstrumentID, sizeof(oOrderInf.InstrumentID));
  snprintf(oOrderInf.Direction, sizeof(oOrderInf.Direction), "%c", pTraderOrder->Direction);
  snprintf(oOrderInf.OffsetFlag, sizeof(oOrderInf.OffsetFlag), "%c", pTraderOrder->OffsetFlag);
  snprintf(oOrderInf.HedgeFlag, sizeof(oOrderInf.HedgeFlag), "%c", pTraderOrder->HedgeFlag);
  snprintf(oOrderInf.OrderStatus, sizeof(oOrderInf.OrderStatus), "%c", pTraderOrder->OrderStatus);
  oOrderInf.LimitPrice = pTraderOrder->LimitPrice;
  oOrderInf.Volume = pTraderOrder->VolumeOriginal;
  strncpy(oOrderInf.InsertTime, pTraderOrder->InsertTime, sizeof(oOrderInf.InsertTime));
  cmn_util_timeval_conv(oOrderInf.LocalCreateTime, &pTraderOrder->CreateTime);
  cmn_util_timeval_conv(oOrderInf.LocalUpdateTime, &pTraderOrder->UpdateTime);

  nRet = self->pTraderDb->pMethod->xOrderInsert(self->pTraderDb, &oOrderInf, &nSqlCode);

  return nRet;
}

int trader_strategy_engine_save_trade(trader_strategy_engine* self, trader_trade* pTraderTrade)
{
  CMN_DEBUG("Enter!\n");
  struct trader_db_trade_inf_def oTradeInf;
  int nRet;
  int nSqlCode = 0;

  strcpy(oTradeInf.InstrumentID, pTraderTrade->InstrumentID);
  strcpy(oTradeInf.TradingDay, pTraderTrade->TradingDay);
  strcpy(oTradeInf.TradeID, pTraderTrade->TradeID);
  strcpy(oTradeInf.UserOrderLocalID, pTraderTrade->UserOrderLocalID);
  oTradeInf.TradePrice = pTraderTrade->TradePrice;
  oTradeInf.TradeVolume = pTraderTrade->TradeVolume;
  strcpy(oTradeInf.TradeTime, pTraderTrade->TradeTime);
  cmn_util_timeval_conv(oTradeInf.LocalUpdateTime, &pTraderTrade->UpdateTime);

  nRet = self->pTraderDb->pMethod->xTradeInsert(self->pTraderDb, &oTradeInf, &nSqlCode);

  return nRet;
}

int trader_strategy_engine_init_investor_position(trader_strategy_engine* self, investor_position* pInvestorPosition)
{
  // 更新总限仓
  self->pTraderStrategyLimit->pMethod->xOnInit(self->pTraderStrategyLimit,
    pInvestorPosition);
    
  return 0;
}


void trader_strategy_engine_timeout_cb(evutil_socket_t fd, short event, void* arg)
{
  trader_strategy_engine_action_param* param = (trader_strategy_engine_action_param*)arg;
  trader_strategy_engine* self = param->parent;
  self->pMethod->xCancelOrder(self, 
    param->contract, 
    param->user_local_id, 
    param->org_user_local_id,
    param->exchange_id,
    param->order_sys_id
    );

  free(param->contract);
  free(param->user_local_id);
  free(param->org_user_local_id);
  free(param->exchange_id);
  free(param->order_sys_id);
  evtimer_del(param->evTimer);
  event_free(param->evTimer);

  free(param);

  return;
}

trader_strategy_engine* trader_strategy_engine_new()
{
  trader_strategy_engine* self = (trader_strategy_engine*)malloc(sizeof(trader_strategy_engine));

  self->orderStrategyMap = cmn_util_map_new();
  self->nSequence = 1;
  
  //trader_strategys
  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = trader_strategy_new();
    pTraderStrategy->pMethod->xInit(pTraderStrategy);
    pTraderStrategy->pEngine = self;
    pTraderStrategy->idx = i;
    pTraderStrategy->TriggerType = 0;
    self->trader_strategys[i] = pTraderStrategy;
  }

  self->pTraderStrategyLimit = trader_strategy_limit_new();

  self->pMethod = trader_strategy_engine_method_get();

  return self;

}

void trader_strategy_engine_free(trader_strategy_engine* self)
{  
  if(self->orderStrategyMap){
    cmn_util_map_free(self->orderStrategyMap);
  }

  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = self->trader_strategys[i];
    trader_strategy_free(pTraderStrategy);
  }
  
  if(self){
    free(self);
  }
  
}

void trader_strategy_engine_status_timer_init(trader_strategy_engine* self)
{
  CMN_INFO("Enter!\n");
  self->statusFlag = 1;
  self->tickTimerEvent = evtimer_new(self->pBase, trader_strategy_engine_status_timer_timeout_cb, (void*)self);
  strncpy(self->currentTime, "09:29:00", sizeof(self->currentTime));
  return;
}

void trader_strategy_engine_status_timer_tick(trader_strategy_engine* self, char* pUpdateTime, int pUpdateMillisec)
{
  if(!self->statusFlag){
    return;
  }

  if(0 != strncmp(self->currentTime, pUpdateTime, sizeof(self->currentTime))){
    return;
  }

  CMN_INFO("Enter!\n");
  self->statusFlag = 0;
  int UpdateMillisec = 1000 - pUpdateMillisec;

  if(0 == pUpdateMillisec){
    UpdateMillisec = 995;
  }

  struct timeval t1_timeout = {
    59, UpdateMillisec*1000
  };

  evtimer_add(self->tickTimerEvent, &t1_timeout);

  return ;
}

void  trader_strategy_engine_status_timer_timeout_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_strategy_engine* self = (trader_strategy_engine*)arg;
  evtimer_del(self->tickTimerEvent);
  event_free(self->tickTimerEvent);
  trader_strategy_engine_status_timer_event(self);
  return;
}

void trader_strategy_engine_status_timer_event(trader_strategy_engine* self)
{
  CMN_INFO("Enter!\n");
  int i;
  trader_strategy* pStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pStrategy = self->trader_strategys[i];
    pStrategy->pMethod->xOnTimerStatus(pStrategy);  
  }

  return;
}

