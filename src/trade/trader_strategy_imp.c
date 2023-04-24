
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

static int trader_strategy_double_to_int(double val);
static double trader_strategy_t1_buy_price(trader_strategy* self, double diff);
static double trader_strategy_t2_buy_price(trader_strategy* self);
static double trader_strategy_t1_sell_price(trader_strategy* self, double diff);
static double trader_strategy_t2_sell_price(trader_strategy* self);

int trader_strategy_double_to_int(double val)
{
  if(val < 0){
    val -= 0.5f;
  }else{
    val += 0.5f;
  }

  return (int)val;
}


int trader_strategy_judge_t1_wait(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("sid[%02d]Enter!\n", self->idx);
  trader_order* pOrder = order_data;
  int t1wait = 0;
  int diffPrice;

  if(trader_strategy_is_guzhi(self)){
    CMN_DEBUG("股指策略直接撤单\n");
    return 1;
  }

  if(0 == self->used){
    CMN_DEBUG("策略关闭直接撤单\n");
    return 1;
  }

  
  trader_strategy_plan* pStrategyPlan = NULL;
  // 获取下单计划
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, order_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //没找到报单
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return 0;
  }
  
  // 商品策略
  t1wait = self->T1Wait;
  if(t1wait <= 0){
    return 1;
  }
  
  double t1Price = trader_strategy_t1_price(self, pStrategyPlan->cLongShort, pStrategyPlan->cOpenClose);
  double t2Price = trader_strategy_t2_price(self, pStrategyPlan->cLongShort, pStrategyPlan->cOpenClose);

  CMN_DEBUG("t1wait=[%d]\n"
    "priceTick=[%lf]\n"
    "t1Price=[%lf]\n"
    "t2Price=[%lf]\n"
    "t1PricePlan=[%lf]\n"
    "t2PricePlan=[%lf]\n",
    t1wait,
    self->PriceTick,
    t1Price,
    t2Price,
    pStrategyPlan->fT1Price,
    pStrategyPlan->fT2Price);
  
  //1.  判断T1是否发生变化
  diffPrice = trader_strategy_double_to_int((t1Price - pStrategyPlan->fT1Price) / self->PriceTick);
  if(diffPrice < 0){
    diffPrice = -diffPrice;
  }
  
  if(diffPrice > t1wait){
    return 1;
  }
  
  //2.  判断T2是否发生变化
  diffPrice = trader_strategy_double_to_int((t2Price - pStrategyPlan->fT2Price) / self->T2PriceTick);
  if(0 == diffPrice){
    return 0;
  }
  
  return 1;
}

int trader_strategy_judge_t2_wait(trader_strategy* self,  trader_order* order_data)
{
  if(0 == self->T2Wait){
    return 1;
  }
  
  CMN_DEBUG("sid[%02d]Enter!\n", self->idx);
  return 1;
}

int trader_strategy_judge_buy_open(trader_strategy* self)
{
  trader_tick* t2 = &self->oT2Tick;
  double diff;
  double th;
  int nRet = 0;
  
  if(IS_STG_SHANGPIN(self->STG)){
    if(t2->BidVolume1 < self->PermitVol){
      CMN_DEBUG("t2->BidVolume1[%d] < stage->PermitVol[%d]\n", t2->BidVolume1, self->PermitVol);
      return nRet;
    }   
  }

  if(IS_STG_OPTION(self->STG)){
    if(t2->AskVolume1 < self->PermitVol){
      CMN_DEBUG("t2->AskVolume1[%d] < stage->PermitVol[%d]\n", t2->AskVolume1, self->PermitVol);
      return nRet;
    }   
  }

  diff = trader_strategy_buy_price_diff(self);
  th = self->DTOpen;

  nRet = trader_strategy_double_to_int((diff - th) / self->T1Weight / self->PriceTick);
  CMN_INFO("SID[%02d]T1[%s]T2[%s]DIFF[%.1lf]DTOpen[%.1lf]nRet[%d]\n", 
    self->idx, self->T1, self->T2, diff, th, nRet);
  if(nRet <= 0){
    return 1;
  }
  return 0 ;
}

int trader_strategy_judge_buy_close(trader_strategy* self)
{
  double diff;
  double th;
  int nRet = 0;
  
  diff = trader_strategy_sell_price_diff(self);
  th = self->DTClose;

  nRet = trader_strategy_double_to_int((diff - th) / self->T1Weight / self->PriceTick);
  CMN_INFO("SID[%02d]T1[%s]T2[%s]DIFF[%.1lf]DTClose[%.1lf]nRet[%d]\n", 
    self->idx, self->T1, self->T2, diff, th, nRet);
  if(nRet >= 0){
    return 1;
  }
  return 0 ;

}

int trader_strategy_judge_sell_open(trader_strategy* self)
{
  trader_tick* t2 = &self->oT2Tick;
  double diff;
  double th;
  int nRet = 0;

  if(IS_STG_SHANGPIN(self->STG)){
    if(t2->AskVolume1 < self->PermitVol){
      CMN_DEBUG("t2->AskVolume1[%d] < stage->PermitVol[%d]\n", t2->AskVolume1, self->PermitVol);
      return nRet;
    }   
  }

  if(IS_STG_OPTION(self->STG)){
    if(t2->BidVolume1 < self->PermitVol){
      CMN_DEBUG("t2->BidVolume1[%d] < stage->PermitVol[%d]\n", t2->BidVolume1, self->PermitVol);
      return nRet;
    }   
  }


  diff = trader_strategy_sell_price_diff(self);
  th = self->KTOpen;

  nRet = trader_strategy_double_to_int((diff - th) / self->T1Weight / self->PriceTick);
  CMN_INFO("SID[%02d]T1[%s]T2[%s]DIFF[%.1lf]KTOPen[%.1lf]nRet[%d]\n", 
    self->idx, self->T1, self->T2, diff, th, nRet);
  if(nRet >= 0){
    return 1;
  }
  return 0 ;
}

int trader_strategy_judge_sell_close(trader_strategy* self)
{
  double diff;
  double th;
  int nRet = 0;
  
  diff = trader_strategy_buy_price_diff(self);
  th = self->KTClose;

  nRet = trader_strategy_double_to_int((diff - th) / self->T1Weight / self->PriceTick);
  CMN_INFO("SID[%02d]T1[%s]T2[%s]DIFF[%.1lf]KTClose[%.1lf]nRet[%d]\n", 
    self->idx, self->T1, self->T2, diff, th, nRet);
  if(nRet <= 0){
    return 1;
  }
  return 0 ;
}

double trader_strategy_buy_price_diff(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  
  double t1set = t1->BidPrice1;
  double t2set = trader_strategy_t2_sell_price(self);

  //配比
  t1set *= self->T1Weight;
  t2set *= self->T2Weight;

  CMN_INFO("T1SET[%.4lf]T2SET[%.4lf]\n", t1set, t2set);

  if(IS_STG_OPTION(self->STG)){
    return t1set + t2set;
  }
  return t1set - t2set;

}

double trader_strategy_sell_price_diff(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;

  double t1set = t1->AskPrice1;
  double t2set = trader_strategy_t2_buy_price(self);

  // 配比
  t1set *= self->T1Weight;
  t2set *= self->T2Weight;
  
  CMN_INFO("T1SET[%.4lf]T2SET[%.4lf]\n", t1set, t2set);

  if(IS_STG_OPTION(self->STG)){
    return t1set + t2set;
  }
  return t1set - t2set;
}

char trader_strategy_t1_direction(trader_strategy* self, char long_short, char open_close)
{
  char cDirection;
  if(TRADER_POSITION_LONG == long_short){
    // 做多
    if(TRADER_POSITION_OPEN == open_close){
      cDirection = TRADER_POSITION_BUY;
    }else{
      cDirection = TRADER_POSITION_SELL;
    }
  }else{
    // 做空
    if(TRADER_POSITION_OPEN == open_close){
      cDirection = TRADER_POSITION_SELL;
    }else{
      cDirection = TRADER_POSITION_BUY;
    }
  }

  return cDirection;
}

char trader_strategy_t2_direction(trader_strategy* self, char long_short, char open_close)
{
  char cDirection;
  
  if(IS_STG_OPTION(self->STG)){
    cDirection = trader_strategy_t1_direction(self, long_short, open_close);
    return cDirection;
  }
  
  if(TRADER_POSITION_LONG == long_short){
    // 做多
    if(TRADER_POSITION_OPEN == open_close){
      cDirection = TRADER_POSITION_SELL;
    }else{
      cDirection = TRADER_POSITION_BUY;
    }
  }else{
    // 做空
    if(TRADER_POSITION_OPEN == open_close){
      cDirection = TRADER_POSITION_BUY;
    }else{
      cDirection = TRADER_POSITION_SELL;
    }
  }

  return cDirection;
}

double trader_strategy_t1_price(trader_strategy* self, char long_short, char open_close)
{
  trader_tick* t1 = &self->oT1Tick;
  double t1set;
  double setVal;
  char cBuySell = TRADER_POSITION_BUY;
  
  if(TRADER_POSITION_LONG == long_short){
    if(TRADER_POSITION_OPEN == open_close){
      cBuySell = TRADER_POSITION_BUY;
      setVal = self->DTOpen;
    }else{
      cBuySell = TRADER_POSITION_SELL;
      setVal = self->DTClose;
    }
  }else{
    if(TRADER_POSITION_OPEN == open_close){
      cBuySell = TRADER_POSITION_SELL;
      setVal = self->KTOpen;
    }else{
      cBuySell = TRADER_POSITION_BUY;
      setVal = self->KTClose;
    }
  }

  if(TRADER_POSITION_BUY == cBuySell) {
    // 买入
    t1set = trader_strategy_t1_buy_price(self, setVal);

    // 对手价
    if(t1set > t1->AskPrice1){
      t1set = t1->AskPrice1;
    }

    // 超价
    t1set += self->PriceTick * self->T1Over;

    // 涨停判断
    if(t1->UpperLimitPrice > 0){
      if(t1set > t1->UpperLimitPrice){
        t1set = t1->UpperLimitPrice;
      }
    }

  }else{
    // 卖出
    t1set = trader_strategy_t1_sell_price(self, setVal);

    // 对手价
    if(t1set < t1->BidPrice1){
      t1set = t1->BidPrice1;
    }

    // 超价
    t1set -= self->PriceTick * self->T1Over;

    // 跌停判断
    if(t1->LowerLimitPrice > 0){
      if(t1set < t1->LowerLimitPrice){
        t1set = t1->LowerLimitPrice;
      }
    }
  }

  return t1set;
}

double trader_strategy_t2_price(trader_strategy* self, char long_short, char open_close)
{
  trader_tick* t2 = &self->oT2Tick;
  double t2set;

  char cBuySell = TRADER_POSITION_BUY;
  if(IS_STG_OPTION(self->STG)){
    if(TRADER_POSITION_LONG == long_short){
      if(TRADER_POSITION_OPEN == open_close) {
        cBuySell = TRADER_POSITION_BUY;
      }else{
        cBuySell = TRADER_POSITION_SELL;
      }
    }else{
      if(TRADER_POSITION_OPEN == open_close) {
        cBuySell = TRADER_POSITION_SELL;
      }else{
        cBuySell = TRADER_POSITION_BUY;
      }
    }
  }else{
    if(TRADER_POSITION_LONG == long_short){
      if(TRADER_POSITION_OPEN == open_close) {
        cBuySell = TRADER_POSITION_SELL;
      }else{
        cBuySell = TRADER_POSITION_BUY;
      }
    }else{
      if(TRADER_POSITION_OPEN == open_close) {
        cBuySell = TRADER_POSITION_BUY;
      }else{
        cBuySell = TRADER_POSITION_SELL;
      }
    }
  }
  if(TRADER_POSITION_BUY == cBuySell){
    // 买
    // 多开 空平
    t2set = trader_strategy_t2_buy_price(self);
    t2set += self->PriceTick * self->T2Over;

    // 涨停判断
    if(t2->UpperLimitPrice > 0){
      if(t2set > t2->UpperLimitPrice){
        t2set = t2->UpperLimitPrice;
      }
    } 
  }else{
    // 卖
    // 空开 多平
    t2set = trader_strategy_t2_sell_price(self);
    t2set -= self->PriceTick * self->T2Over;

    // 跌停判断
    if(t2->LowerLimitPrice > 0){
      if(t2set < t2->LowerLimitPrice){
        t2set = t2->LowerLimitPrice;
      }
    }
  }
  return t2set;
}

double trader_strategy_t2_price_opponent(trader_strategy* self, char buy_sell)
{
  char cBuySell = buy_sell;
  trader_tick* t2 = &self->oT2Tick;
  
  double t2set;

  if(TRADER_POSITION_BUY == cBuySell){
    t2set = t2->AskPrice1 + self->T2Over * self->PriceTick;
    // 涨停判断
    if(t2->UpperLimitPrice > 0){
      if(t2set > t2->UpperLimitPrice){
        t2set = t2->UpperLimitPrice;
      }
    }

  }else{
    t2set = t2->BidPrice1 - self->T2Over * self->PriceTick;
    // 跌停判断
    if(t2->LowerLimitPrice > 0){
      if(t2set < t2->LowerLimitPrice){
        t2set = t2->LowerLimitPrice;
      }
    }
  }
    
  return t2set;
}

int trader_strategy_check_t1_price(trader_strategy* self, double price)
{
  trader_tick* t1 = &self->oT1Tick;

  if(0 == price){
    CMN_ERROR("price[%lf]\n", price); 
    return -1;
  }

  if(t1->LowerLimitPrice > 0){
    if(price <= t1->LowerLimitPrice){
      CMN_ERROR("price[%lf] <= LowerLimitPrice[%lf]\n", price, t1->LowerLimitPrice); 
      return -1;
    }
  }

  if(t1->UpperLimitPrice > 0){
    if(price >= t1->UpperLimitPrice){
      CMN_ERROR("price[%lf] >= UpperLimitPrice[%lf]\n", price, t1->UpperLimitPrice); 
      return -1;
    }
  }

  return 0;
}

int trader_strategy_check_t2_price(trader_strategy* self, double price)
{
  trader_tick* t2 = &self->oT2Tick;
  
  if(0 == price){
    CMN_ERROR("price[%lf]\n", price); 
    return -1;
  }

  if(t2->LowerLimitPrice > 0){
    if(price < t2->LowerLimitPrice){
      CMN_ERROR("price[%lf] < LowerLimitPrice[%lf]\n", price, t2->LowerLimitPrice); 
      return -1;
    }
  }

  if(t2->UpperLimitPrice > 0){
    if(price > t2->UpperLimitPrice){
      CMN_ERROR("price[%lf] > UpperLimitPrice[%lf]\n", price, t2->UpperLimitPrice); 
      return -1;
    }
  }
  
  return 0;
}

int trader_strategy_auto_sell(trader_strategy* self)
{

  // 当前价差
  double diff = trader_strategy_sell_price_diff(self);

  // 空开价差
  self->KTOpen = diff + self->AutoKTOpen * self->PriceTick * self->T1Weight;

  // 空平价差
  self->KTClose = diff + ((self->AutoKTOpen - self->AutoKTClose) * self->PriceTick * self->T1Weight);
  
  CMN_INFO("SID[%02d]T1[%s]T2[%s]diff[%.3f]KTOpen[%.3f]KTClose[%.3f]\n", 
    self->idx, self->T1, self->T2, diff, self->KTOpen, self->KTClose);

  return 0;
}

int trader_strategy_auto_buy(trader_strategy* self)
{
  // 当前价差
  double diff = trader_strategy_buy_price_diff(self);
  
  // 多开价差
  self->DTOpen = diff - self->AutoDTOpen * self->PriceTick * self->T1Weight;
  
  // 多平价差
  self->DTClose = diff - ((self->AutoDTOpen - self->AutoDTClose) * self->PriceTick * self->T1Weight);
  
  CMN_INFO("SID[%02d]T1[%s]T2[%s]diff[%.3f]DTOpen[%.3f]DTClose[%.3f]\n", 
    self->idx, self->T1, self->T2, diff, self->DTOpen, self->DTClose);
  
  return 0;
}

double trader_strategy_t1_buy_price(trader_strategy* self, double diff)
{
  double t2set = trader_strategy_t2_sell_price(self);
  double t1set;
  if(IS_STG_OPTION(self->STG)){
    t1set = (diff - t2set * self->T2Weight) / self->T1Weight;
  }else{
    t1set = (diff + t2set * self->T2Weight) / self->T1Weight;
  }
  
  CMN_INFO("t1set[%.4lf]diff[%.4lf]t2set[%.4lf]T2Weight[%.4lf]T1Weight[%.4lf]\n", 
    t1set, diff, t2set, self->T2Weight, self->T1Weight);
  
  return t1set;
}

double trader_strategy_t2_buy_price(trader_strategy* self)
{
  trader_tick* t2 = &self->oT2Tick;

  double t2set = t2->AskPrice1;

  if(IS_STG_MM(self->STG)){
    long t2l = -1;
    t2l += (long)(t2->BidPrice1 / self->T2PriceTick);
    t2l += (long)(t2->AskPrice1 / self->T2PriceTick);
    t2l = t2l / 2;

    t2set = self->T2PriceTick * t2l;
  }

  return t2set;
}

double trader_strategy_t1_sell_price(trader_strategy* self, double diff)
{
  double t2set = trader_strategy_t2_buy_price(self);
  double t1set; 
  if(IS_STG_OPTION(self->STG)){
    t1set = (diff - t2set * self->T2Weight) / self->T1Weight;
  }else{
    t1set = (diff + t2set * self->T2Weight) / self->T1Weight;
  }
  
  CMN_INFO("t1set[%.4lf]diff[%.4lf]t2set[%.4lf]T2Weight[%.4lf]T1Weight[%.4lf]\n", 
    t1set, diff, t2set, self->T2Weight, self->T1Weight);

  return t1set;
}

double trader_strategy_t2_sell_price(trader_strategy* self)
{
  trader_tick* t2 = &self->oT2Tick;
  
  double t2set = t2->BidPrice1;

  if(IS_STG_MM(self->STG)){
    long t2l = 1;
    t2l += (long)(t2->BidPrice1 / self->T2PriceTick);
    t2l += (long)(t2->AskPrice1 / self->T2PriceTick);
    t2l = t2l / 2;

    t2set = self->T2PriceTick * t2l;
  }

  return t2set;
}

int trader_strategy_is_guzhi(trader_strategy* self)
{
  char* currentTick = self->oT2Tick.UpdateTime;
  
  if(!IS_STG_GUZHI(self->STG)){
    return 0;
  }

  if((strncmp(self->oT2Tick.UpdateTime, "09:30:00", sizeof(self->oT2Tick.UpdateTime)) < 0)||
    (strncmp(self->oT2Tick.UpdateTime, "09:30:05", sizeof(self->oT2Tick.UpdateTime)) > 0)){
    return 0;
  }

  return 1;
}


