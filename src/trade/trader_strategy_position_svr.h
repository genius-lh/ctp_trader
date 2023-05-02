#ifndef _TRADER_STRATEGY_POSITION_SVR_H_
#define _TRADER_STRATEGY_POSITION_SVR_H_

#include "trader_data.h"
#include "trader_msg_struct.h"

typedef struct trader_strategy_position_svr_def trader_strategy_position_svr;
typedef struct trader_strategy_position_svr_method_def trader_strategy_position_svr_method;

struct trader_strategy_position_svr_def {
  void* redis;
  const char* account;
  trade_position position[64];
  trader_strategy_position_svr_method* pMethod;
};

struct trader_strategy_position_svr_method_def {
  void (*xInit)(trader_strategy_position_svr* self, const char* account);
  void (*xUpdate)(trader_strategy_position_svr* self, trade_position* p_position);
  int (*xGetPosition)(trader_strategy_position_svr* self, int stage_id, char direction, trade_position** pp_position);
};

extern trader_strategy_position_svr* trader_strategy_position_svr_new(void* redis);
extern void trader_strategy_position_svr_free(trader_strategy_position_svr* self);


#endif // _TRADER_STRATEGY_POSITION_SVR_H_

