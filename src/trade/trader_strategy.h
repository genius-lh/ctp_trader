#ifndef _TRADER_STRATEGY_H_
#define _TRADER_STRATEGY_H_

#include "queue.h"
#include "cmn_util.h"

#include "trader_data.h"
#include "trader_msg_struct.h"
#include "trader_strategy_engine.h"

#define AUTO_STRATEGY_TYPE_CLOSE 0
#define AUTO_STRATEGY_TYPE_1 1
#define AUTO_STRATEGY_TYPE_2 2

#define AUTO_STRATEGY_IS_TYPE_1(__msec) ((__msec) < 500)

#define CLOSING_TIME_NONE 0
#define CLOSING_TIME_23_00 1
#define CLOSING_TIME_23_30 2
#define CLOSING_TIME_01_00 3
#define CLOSING_TIME_02_00 4
#define CLOSING_TIME_02_30 5

typedef struct trader_strategy_order_def trader_strategy_order;
typedef struct trader_strategy_trade_def trader_strategy_trade;
typedef struct trader_strategy_plan_def trader_strategy_plan;
typedef struct trader_strategy_position_def trader_strategy_position;

struct trader_strategy_order_def {
  TAILQ_ENTRY(trader_strategy_order_def) next;
  char ref[21];
  trader_order* order;
  
};

struct trader_strategy_trade_def{
  TAILQ_ENTRY(trader_strategy_trade_def) next;
  char cLongShort;
  char T2Direction;
  char T2Offset;
  int TradeVolume;
  double T1Price;
  double T2Price;
  int T1TradeVolume;
  int T2TradeVolume;
};


// 开仓计划
struct trader_strategy_plan_def {
  TAILQ_ENTRY(trader_strategy_plan_def) next;
  // T1报单号
  char UserOrderLocalID[21];
  char cLongShort;
  char cOpenClose;
  int nPlanVol;
  int nT1TradedVol;
  double fT1Price;
  double fT2Price;
};

// 持仓情况
struct trader_strategy_position_def {
  TAILQ_ENTRY(trader_strategy_position_def) next;
  // T2报单号
  char UserOrderLocalID[21];
  char cOpenClose;
  char cT1Direction;
  char cT2Direction;
  int nTradeVol;
  double fT1Price;
  double fT2Price;
};


typedef struct trader_strategy_def trader_strategy;
typedef struct trader_strategy_method_def trader_strategy_method;

struct trader_strategy_def{
  // 策略详情
  int idx;
  char T1[TraderInstrumentIDTypeLen];
  char T2[TraderInstrumentIDTypeLen];
  int MP;
  int hold;
  int Mult;
  double KTOpen;
  double KTClose;
  double DTOpen;
  double DTClose;
  int T1Over;
  int T1Wait;
  int T2Over;
  int T2Wait;
  int PermitVol;
  int STG;
  int used;

  double PriceTick;
  int IsSHFE;
  double T2PriceTick;
  int T2IsSHFE;
  
  int AutoType;
  double AutoKTOpen;
  double AutoKTClose;
  double AutoDTOpen;
  double AutoDTClose;

  double T1Weight;
  double T2Weight;
  int T2Ratio;
  // 夜盘收盘时间
  int NightClosingTime;
  
  int TriggerType;

  // T1 行情
  trader_tick oT1Tick;
  // T2 行情
  trader_tick oT2Tick;
  
  // 订单队列
  cmn_util_map* mapOrder;
  
  // 计划队列
  cmn_util_map* mapStrategyPlan;
  
  // 待成交队列
  TAILQ_HEAD(trader_strategy_order_def_list_not_finished, trader_strategy_order_def) listNotFinishedOrder;

  // T1成交队列
  cmn_util_map* mapHalfTraded;

  // 当前策略持仓
  // 做多
  // 做多持仓
  int nPositionBuy;
  // 做多委托
  int nOrderBuy;
  // 做多冻结
  int nFronzeBuy;
  // 做多单腿
  int nFailedBuyOpen;
  int nFailedBuyClose;
  trade_position oBuyPosition;

  // 做空
  // 做空持仓
  int nPositionSell;
  // 做空委托
  int nOrderSell;
  // 做空冻结
  int nFronzeSell;
  // 做空单腿
  int nFailedSellOpen;
  int nFailedSellClose;
  trade_position oSellPosition;

  
  struct trader_strategy_engine_def* pEngine;
  trader_strategy_method* pMethod;
};


struct trader_strategy_method_def{
  int (*xInit)(trader_strategy* self);
  int (*xOnTick)(trader_strategy* self, trader_tick* tick_data);
  int (*xOnOrder)(trader_strategy* self, trader_order* order_data);
  int (*xOnTrade)(trader_strategy* self, trader_trade* trade_data);
  int (*xOnStatus)(trader_strategy* self, instrument_status* status_data);
  int (*xOnTimerStatus)(trader_strategy* self);
  // 重置仓位
  int (*xResetPosition)(trader_strategy* self, char buy_sell, int volume);
  // 查询仓位
  int (*xQueryPosition)(trader_strategy* self, char buy_sell, trade_position* p_position);
};

extern trader_strategy* trader_strategy_new();
extern void trader_strategy_free(trader_strategy* self);

extern int trader_strategy_judge_t1_wait(trader_strategy* self,  trader_order* order_data);
extern int trader_strategy_judge_t2_wait(trader_strategy* self,  trader_order* order_data);

extern int trader_strategy_judge_buy_open(trader_strategy* self);
extern int trader_strategy_judge_buy_close(trader_strategy* self);
extern int trader_strategy_judge_sell_open(trader_strategy* self);
extern int trader_strategy_judge_sell_close(trader_strategy* self);

extern double trader_strategy_buy_price_diff(trader_strategy* self);
extern double trader_strategy_sell_price_diff(trader_strategy* self);

extern char trader_strategy_t1_direction(trader_strategy* self, char long_short, char open_close);
extern char trader_strategy_t2_direction(trader_strategy* self, char long_short, char open_close);

extern double trader_strategy_t1_price(trader_strategy* self, char long_short, char open_close);
extern double trader_strategy_t2_price(trader_strategy* self, char long_short, char open_close);

extern double trader_strategy_t2_price_opponent(trader_strategy* self, char buy_sell);

extern int trader_strategy_check_t1_price(trader_strategy* self, double price);
extern int trader_strategy_check_t2_price(trader_strategy* self, double price);

extern int trader_strategy_auto_sell(trader_strategy* self);
extern int trader_strategy_auto_buy(trader_strategy* self);



#endif //_TRADER_STRATEGY_H_

