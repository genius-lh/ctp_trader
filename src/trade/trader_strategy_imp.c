
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
#include "ctp_trader_api.h"

#include "trader_strategy_limit.h"

static int trader_strategy_double_to_int(double val);

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
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder = order_data;
  int t1wait = 0;
  int diffPrice;

  if(IS_STG_GUZHI(self->STG)){
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
  if(TRADER_POSITION_BUY == pOrder->Direction){
    diffPrice = trader_strategy_double_to_int((t1Price - pStrategyPlan->fT1Price) / self->PriceTick);
    if(diffPrice > t1wait){
      return 1;
    }
  }else{
    diffPrice = trader_strategy_double_to_int((pStrategyPlan->fT1Price - t1Price) / self->PriceTick);
    if(diffPrice > t1wait){
      return 1;
    }
  }
  
  //2.  判断T2是否发生变化
  diffPrice = trader_strategy_double_to_int((t2Price - pStrategyPlan->fT2Price) / self->PriceTick);
  if(0  == diffPrice){
    return 0;
  }
  
  return 1;
}

int trader_strategy_judge_t2_wait(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  int bCancel = 0;
  trader_order* pOrder = order_data;
  
  double planPrice;
  double currentPrice;

  if(0 == self->T2Wait){
    return 1;
  }
  
  planPrice = pOrder->LimitPrice;
  if(TRADER_POSITION_BUY == pOrder->Direction){
    currentPrice = self->oT2Tick.BidPrice1;
    currentPrice += self->PriceTick * self->T2Over;
    planPrice += self->PriceTick * self->T2Wait;
    if(planPrice < currentPrice){
      bCancel = 1;
    }
    
  }else{
    currentPrice = self->oT2Tick.AskPrice1;
    currentPrice -= self->PriceTick * self->T2Over;
    planPrice -= self->PriceTick * self->T2Wait;
    if(planPrice > currentPrice){
      bCancel = 1;
    }
  }

  return bCancel;

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

  nRet = trader_strategy_double_to_int((diff - th) / self->PriceTick);
  CMN_INFO("DIFF[%.1lf]DTOPen[%.1lf]nRet[%d]\n", diff, th, nRet);
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

  nRet = trader_strategy_double_to_int((diff - th) / self->PriceTick);
  CMN_INFO("DIFF[%.1lf]DTClose[%.1lf]nRet[%d]\n", diff, th, nRet);
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

  nRet = trader_strategy_double_to_int((diff - th) / self->PriceTick);
  CMN_INFO("DIFF[%.1lf]KTOpen[%.1lf]nRet[%d]\n", diff, th, nRet);
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

  nRet = trader_strategy_double_to_int((diff - th) / self->PriceTick);
  CMN_INFO("DIFF[%.1lf]KTClose[%.1lf]nRet[%d]\n", diff, th, nRet);
  if(nRet <= 0){
    return 1;
  }
  return 0 ;
}

double trader_strategy_buy_price_diff(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  
  int nRet = 0;
  double t1set = t1->BidPrice1;
  double t2set = t2->BidPrice1;

  if(IS_STG_MM(self->STG)){
    long t2l = 1;
    t2l += (long)(t2->BidPrice1 / self->PriceTick);
    t2l += (long)(t2->AskPrice1 / self->PriceTick);
    t2l = t2l / 2;

    t2set = self->PriceTick * t2l;
  }

  if(IS_STG_OPTION(self->STG)){
    return t1set + t2set;
  }
  return t1set - t2set;

}

double trader_strategy_sell_price_diff(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  int nRet = 0;
  double t1set = t1->AskPrice1;
  double t2set = t2->AskPrice1;

  if(IS_STG_MM(self->STG)){
    long t2l = -1;
    t2l += (long)(t2->BidPrice1 / self->PriceTick);
    t2l += (long)(t2->AskPrice1 / self->PriceTick);
    t2l = t2l / 2;

    t2set = self->PriceTick * t2l;
  }

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
  double t2set;
  double diff;
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
    diff = trader_strategy_buy_price_diff(self);
    if(IS_STG_OPTION(self->STG)){
      t2set = diff - t1->BidPrice1;
      t1set = setVal - t2set + self->PriceTick * self->T1Over;
    }else{
      t2set = t1->BidPrice1 - diff;
      t1set = t2set + setVal + self->PriceTick * self->T1Over;
    }

    if(t1set > t1->UpperLimitPrice){
      t1set = t1->AskPrice1 + self->PriceTick * self->T1Over;
      if(t1set > t1->UpperLimitPrice){
        t1set = t1->UpperLimitPrice;
      }
    } 

  }else{
    // 卖出
    diff = trader_strategy_sell_price_diff(self);
    if(IS_STG_OPTION(self->STG)){
      t2set = diff - t1->AskPrice1;
      t1set = setVal - t2set - self->PriceTick * self->T1Over;
    }else{
      t2set = t1->AskPrice1 - diff;
      t1set = t2set + setVal - self->PriceTick * self->T1Over;
    }
    
    if(t1set < t1->LowerLimitPrice){
      t1set = t1->BidPrice1 - self->PriceTick * self->T1Over;
      if(t1set < t1->LowerLimitPrice){
        t1set = t1->LowerLimitPrice;
      }
    } 

  }

  return t1set;
}

double trader_strategy_t2_price(trader_strategy* self, char long_short, char open_close)
{
  trader_tick* t1 = &self->oT1Tick;
  double t2set;
  double diff;

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

    if(TRADER_POSITION_BUY == cBuySell){
      // 买
      // 多开 空平
      diff = trader_strategy_buy_price_diff(self);
      t2set = diff - t1->BidPrice1 + self->PriceTick * self->T2Over;
    }else{
      // 卖
      // 空开 多平
      diff = trader_strategy_sell_price_diff(self);
      t2set = diff - t1->AskPrice1 - self->PriceTick * self->T2Over;
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

    if(TRADER_POSITION_BUY == cBuySell){
      // 买
      // 空开 多平
      diff = trader_strategy_sell_price_diff(self);
      t2set = t1->AskPrice1 - diff + self->PriceTick * self->T2Over;
    }else{
      // 卖
      // 多开 空平
      diff = trader_strategy_buy_price_diff(self);
      t2set = t1->BidPrice1 - diff - self->PriceTick * self->T2Over;
    }
  }
  return t2set;
}

double trader_strategy_t2_price_opponent(trader_strategy* self, char buy_sell)
{
  char cBuySell = buy_sell;
  trader_tick* t2 = &self->oT2Tick;
  
  double t2set;

  if(TRADER_POSITION_SELL == cBuySell){
    t2set = t2->BidPrice1 - self->T2Over * self->PriceTick;
  }else{
    t2set = t2->AskPrice1 + self->T2Over * self->PriceTick;
  }
    
  return t2set;
}

int trader_strategy_check_t1_price(trader_strategy* self, double price)
{
  trader_tick* t1 = &self->oT1Tick;

  if(price <= t1->LowerLimitPrice){
    CMN_ERROR("price[%lf] <= LowerLimitPrice[%lf]\n", price, t1->LowerLimitPrice); 
    return -1;
  }

  if(price >= t1->UpperLimitPrice){
    CMN_ERROR("price[%lf] >= LowerLimitPrice[%lf]\n", price, t1->UpperLimitPrice); 
    return -1;
  }

  return 0;
}

int trader_strategy_check_t2_price(trader_strategy* self, double price)
{
  trader_tick* t2 = &self->oT2Tick;

  if(price < t2->LowerLimitPrice){
    CMN_ERROR("price[%lf] < LowerLimitPrice[%lf]\n", price, t2->LowerLimitPrice); 
    return -1;
  }

  if(price > t2->UpperLimitPrice){
    CMN_ERROR("price[%lf] > LowerLimitPrice[%lf]\n", price, t2->UpperLimitPrice); 
    return -1;
  }

  return 0;
}

int trader_strategy_auto_sell(trader_strategy* self)
{

  // 当前价差
  double diff = trader_strategy_sell_price_diff(self);

  // 空开价差
  self->KTOpen = diff + self->AutoKTOpen;

  // 空平价差
  self->KTClose = diff + self->AutoKTOpen - self->AutoKTClose;
  
  CMN_INFO("diff[%.3f]KTOpen[%.3f]KTClose[%.3f]\n", diff, self->KTOpen, self->KTClose);

  return 0;
}

int trader_strategy_auto_buy(trader_strategy* self)
{
  // 当前价差
  double diff = trader_strategy_buy_price_diff(self);
  
  // 多开价差
  self->DTOpen = diff - self->AutoDTOpen;
  
  // 多平价差
  self->DTClose = diff - self->AutoDTOpen + self->AutoDTClose;
  
  CMN_INFO("diff[%.3f]DTOpen[%.3f]DTClose[%.3f]\n", diff, self->DTOpen, self->DTClose);
  
  return 0;
}



