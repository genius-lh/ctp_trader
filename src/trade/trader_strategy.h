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


// ���ּƻ�
struct trader_strategy_plan_def {
  TAILQ_ENTRY(trader_strategy_plan_def) next;
  // T1������
  char UserOrderLocalID[21];
  char cLongShort;
  char cOpenClose;
  int nPlanVol;
  int nT1TradedVol;
  double fT1Price;
  double fT2Price;
};

// �ֲ����
struct trader_strategy_position_def {
  TAILQ_ENTRY(trader_strategy_position_def) next;
  // T2������
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
  // ��������
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
  // ҹ������ʱ��
  int NightClosingTime;
  
  int TriggerType;

  // T1 ����
  trader_tick oT1Tick;
  // T2 ����
  trader_tick oT2Tick;
  
  // ��������
  cmn_util_map* mapOrder;
  
  // �ƻ�����
  cmn_util_map* mapStrategyPlan;
  
  // ���ɽ�����
  TAILQ_HEAD(trader_strategy_order_def_list_not_finished, trader_strategy_order_def) listNotFinishedOrder;

  // T1�ɽ�����
  cmn_util_map* mapHalfTraded;

  // ��ǰ���Գֲ�
  // ����
  // ����ֲ�
  int nPositionBuy;
  // ����ί��
  int nOrderBuy;
  // ���ඳ��
  int nFronzeBuy;
  // ���൥��
  int nFailedBuyOpen;
  int nFailedBuyClose;
  trade_position oBuyPosition;

  // ����
  // ���ճֲ�
  int nPositionSell;
  // ����ί��
  int nOrderSell;
  // ���ն���
  int nFronzeSell;
  // ���յ���
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
  // ���ò�λ
  int (*xResetPosition)(trader_strategy* self, char buy_sell, int volume);
  // ��ѯ��λ
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

