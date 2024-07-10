#ifndef _TRADER_STRATEGY_PROCESSOR_H_
#define _TRADER_STRATEGY_PROCESSOR_H_

#include "trader_data.h"
#include "trader_strategy.h"

typedef struct trader_strategy_processor_def trader_strategy_processor;
typedef struct trader_strategy_processor_method_def trader_strategy_processor_method;

struct trader_strategy_processor_def{
  void* loopTimer;
  void* auctionTimer;
  int status;
  char currentTime[9];

  void* tickDict;
  void* spreads;

  trader_strategy_processor_method* method;
};

struct trader_strategy_processor_method_def{
  int (*xInit)(trader_strategy_processor* self, void* base);
  int (*xUpdateTick)(trader_strategy_processor* self, trader_tick* tick_data);
  int (*xUpdateStrategy)(trader_strategy_processor* self, void* strategy_data);
  int (*xOnAuctionTimer)(trader_strategy_processor* self);
  int (*xOnLoopTimer)(trader_strategy_processor* self);
};

extern trader_strategy_processor* trader_strategy_processor_new();
extern void trader_strategy_processor_free(trader_strategy_processor* self);

#endif //_TRADER_STRATEGY_PROCESSOR_H_

