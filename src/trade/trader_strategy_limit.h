#ifndef _TRADER_STRATEGY_LIMIT_H_
#define _TRADER_STRATEGY_LIMIT_H_

#include "trader_data.h"
#include "trader_msg_struct.h"
#include "queue.h"

typedef struct trader_strategy_limit_position_def trader_strategy_limit_position;

typedef struct trader_strategy_limit_def trader_strategy_limit;
typedef struct trader_strategy_limit_method_def trader_strategy_limit_method;

///���ճֲ�
#define TRADER_POSITION_DATE_TODAY '1'
///��ʷ�ֲ�
#define TRADER_POSITION_DATE_HISTORY '2'
///�ϲ��ֲ�
#define TRADER_POSITION_DATE_BOTH '3'



struct trader_strategy_limit_position_def {
  TAILQ_ENTRY(trader_strategy_limit_position_def) next;
  char InstrumentID[31]; // ��Լ
  char PosiDirection; // ���
  int IsSHFE; // �Ƿ�Ϊ������
  // ������ʹ��
  int Position; // �ֲ�
  int Fronze; // ƽ�ֶ���
  // ����ʹ��
  int YdPosition; // ���
  int YdFronze; // ƽ�ֶ���
};


struct trader_strategy_limit_def {

  TAILQ_HEAD(,trader_strategy_limit_position_def) hPositionHead;

  trader_strategy_limit_method* pMethod;
};

struct trader_strategy_limit_method_def {
  void (*xOnInit)(trader_strategy_limit* self, investor_position* p_position);
  void (*xOnTrade)(trader_strategy_limit* self, trader_trade* p_trade, int is_shfe);
  void (*xOnOrder)(trader_strategy_limit* self, trader_order* p_order, int is_shfe);
  int (*xGetAvailable)(trader_strategy_limit* self, char* instrument_id, char direction, int* yd_volumn);
};

extern trader_strategy_limit* trader_strategy_limit_new();
extern void trader_strategy_limit_free(trader_strategy_limit* self);


#endif // _TRADER_STRATEGY_LIMIT_H_

