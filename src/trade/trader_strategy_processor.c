#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/util.h>
#include <event2/event.h>

#include "cmn_log.h"
#include "cmn_util.h"
#include "dict.h"

#include "trader_strategy.h"
#include "trader_data.h"

#include "trader_strategy_processor.h"

#define TRADER_STRATEGY_PROCESSOR_SIZE 32

typedef struct trader_tick_entity_def trader_tick_entity;
typedef struct trader_spread_def trader_spread;

struct trader_tick_entity_def{
  trader_tick tick;
  char spreads[TRADER_STRATEGY_PROCESSOR_SIZE];
};

struct trader_spread_def{
	trader_tick* T1;
	trader_tick* T2;
  double T1Weight;
  double T2Weight;
  double AskSpread;
  double BidSpread;
};


static unsigned int tickHash(const void *key)
{
  return dictGenHashFunction((const unsigned char *)key,
                             strlen((const char*)key));
}

static void *tickKeyDup(void *privdata, const void *src)
{
  ((void) privdata);
  int l1 = strlen((const char*)src)+1;
  char* dup = (char*)malloc(l1 * sizeof(char));
  strncpy(dup, (const char*)src, l1);
  return dup;
}

static void *tickValDup(void *privdata, const void *src)
{
  ((void) privdata);
  void* dup = malloc(sizeof(trader_tick_entity));
  memset(dup, 0, sizeof(trader_tick_entity));
  return dup;
}


static int tickKeyCompare(void *privdata, const void *key1, const void *key2)
{
  int l1, l2;
  ((void) privdata);

  l1 = strlen((const char*)key1);
  l2 = strlen((const char*)key2);
  if (l1 != l2) return 0;
  return memcmp(key1,key2,l1) == 0;
}

static void tickKeyDestructor(void *privdata, void *key)
{
  ((void) privdata);
  free((char*)key);
}

static void tickValDestructor(void *privdata, void *val)
{
  ((void) privdata);
  free(val);
}

static dictType* tickDictTypeGet()
{
  static dictType tickDict = {
    tickHash,
    tickKeyDup,
    tickValDup,
    tickKeyCompare,
    tickKeyDestructor,
    tickValDestructor
  };
  return &tickDict;
}

static trader_tick_entity* trader_strategy_processor_entity_get(trader_strategy_processor* self, const char* instrument);
static int trader_strategy_processor_spread_init(trader_strategy_processor* self, trader_strategy* strategy_data); 
static int trader_strategy_processor_spread_caculate(trader_strategy_processor* self, int spread_idx); 
static int trader_strategy_processor_set_loop_timer(trader_strategy_processor* self, trader_tick* tick_data); 

static int trader_strategy_processor_init(trader_strategy_processor* self, void* base);
static int trader_strategy_processor_update_tick(trader_strategy_processor* self, trader_tick* tick_data);
static int trader_strategy_processor_update_strategy(trader_strategy_processor* self, void* strategy_data);
static int trader_strategy_processor_on_auction_timer(trader_strategy_processor* self);
static int trader_strategy_processor_on_loop_timer(trader_strategy_processor* self);
static trader_strategy_processor_method* trader_strategy_processor_method_get();

static void trader_strategy_processor_loop_timer_cb(evutil_socket_t fd, short event, void* arg);
static void trader_strategy_processor_auction_timer_cb(evutil_socket_t fd, short event, void* arg);

trader_strategy_processor_method* trader_strategy_processor_method_get()
{
  static trader_strategy_processor_method trader_strategy_processor_method_st = {
    trader_strategy_processor_init,
    trader_strategy_processor_update_tick,
    trader_strategy_processor_update_strategy,
    trader_strategy_processor_on_auction_timer,
    trader_strategy_processor_on_loop_timer
  };
  return &trader_strategy_processor_method_st;
}

int trader_strategy_processor_init(trader_strategy_processor* self, void* base)
{
  // 初始化Timer
  self->loopTimer = evtimer_new(base, trader_strategy_processor_loop_timer_cb, (void*)self);
  self->auctionTimer = evtimer_new(base, trader_strategy_processor_auction_timer_cb, (void*)self);

  // 初始化词典
  dictType* tickDictType = tickDictTypeGet();
  self->tickDict = (void*)dictCreate(tickDictType, NULL);

  // 初始化价差
  int i;
  trader_spread* spread;
  trader_spread* p;
  self->spreads = (void*)malloc(TRADER_STRATEGY_PROCESSOR_SIZE * sizeof(trader_spread));
  p = (trader_spread*)self->spreads;
  for(i = 0; i < TRADER_STRATEGY_PROCESSOR_SIZE; i++){
    spread = &p[i];
    memset(spread, 0, sizeof(trader_spread));
  }
  
  self->status = 0;

  return 0;
}

int trader_strategy_processor_update_tick(trader_strategy_processor* self, trader_tick* tick_data)
{
  trader_tick* tick = tick_data;
  CMN_INFO("InstrumentID[%s]ReceiveTime[%04ld.%06ld]\n"
    , tick->InstrumentID
    , tick->ReceiveTime.tv_sec % 3600
    , tick->ReceiveTime.tv_usec
  );

  // 定时循环任务
  trader_strategy_processor_set_loop_timer(self, tick);
  
  trader_tick_entity* entity = trader_strategy_processor_entity_get(self, tick_data->InstrumentID);
  CMN_ASSERT(entity);

  memcpy(&entity->tick, tick, sizeof(trader_tick));

  // caculate spread
  int i;
  for(i = 0; i < TRADER_STRATEGY_PROCESSOR_SIZE; i++){
    if(!entity->spreads[i]){
      continue;
    }

    trader_strategy_processor_spread_caculate(self, i);
  }

  return 0;
}

int trader_strategy_processor_update_strategy(trader_strategy_processor* self, void* strategy_data)
{
  trader_strategy* strategy = (trader_strategy*)strategy_data;
  
  trader_strategy_processor_spread_init(self, strategy);

  trader_strategy_processor_spread_caculate(self, strategy->idx);

  return 0;
}

int trader_strategy_processor_on_auction_timer(trader_strategy_processor* self)
{
  return 0;
}

int trader_strategy_processor_on_loop_timer(trader_strategy_processor* self)
{
  self->status = 0;
  evtimer_del(self->loopTimer);
  CMN_INFO("self->currentTime[%s]\n", self->currentTime);
  // TODO
  int i;
  for(i = 0; i < TRADER_STRATEGY_PROCESSOR_SIZE; i++){
    trader_strategy_processor_spread_caculate(self, i);
  }
  return 0;
}

trader_strategy_processor* trader_strategy_processor_new()
{
  trader_strategy_processor* self = (trader_strategy_processor*)malloc(sizeof(trader_strategy_processor));


  self->method = trader_strategy_processor_method_get();

  return self;

}

void trader_strategy_processor_free(trader_strategy_processor* self)
{
  if(self->loopTimer){
    event_free(self->loopTimer);
  }
  
  if(self->auctionTimer){
    event_free(self->auctionTimer);
  }
  
  if(self->tickDict){
    dictRelease(self->tickDict);
  }
  
  if(self->spreads){
    free(self->spreads);
  }
  
  if(self){
    free(self);
  }
  return;
}

void trader_strategy_processor_loop_timer_cb(evutil_socket_t fd, short event, void* arg)
{
  trader_strategy_processor* self = (trader_strategy_processor*)arg;
  self->method->xOnLoopTimer(self);
  return;
}

void trader_strategy_processor_auction_timer_cb(evutil_socket_t fd, short event, void* arg)
{
  trader_strategy_processor* self = (trader_strategy_processor*)arg;
  self->method->xOnAuctionTimer(self);
  return;
}

trader_tick_entity* trader_strategy_processor_entity_get(trader_strategy_processor* self, const char* instrument)
{
  dictEntry* entry = dictFind((dict*)self->tickDict, (const void*)instrument);
  if(NULL != entry){
    return (trader_tick_entity*)entry->val;
  }
  
  int ret = dictAdd((dict*)self->tickDict, (void*)instrument, (void*)NULL);
  if(DICT_ERR == ret){
    // SHOUlD NOT REACH HERE
    CMN_ERROR("DICT_ERR == ret\n");
    return (trader_tick_entity*)NULL;
  }
  
  entry = dictFind((dict*)self->tickDict, (const void*)instrument);
  if(NULL == entry){
    // SHOUlD NOT REACH HERE
    CMN_ERROR("NULL == entry\n");
    return (trader_tick_entity*)NULL;
  }
  
  return (trader_tick_entity*)entry->val;
}

int trader_strategy_processor_spread_init(trader_strategy_processor* self, trader_strategy* strategy_data)
{
  CMN_DEBUG("Enter!\n");
  trader_strategy* strategy = strategy_data;
  trader_spread* p = (trader_spread*)self->spreads;
  trader_spread* spread = &p[strategy->idx];
  trader_tick_entity* entity;
  
  // 初始化T1
  entity = trader_strategy_processor_entity_get(self, strategy->T1);
  CMN_ASSERT(entity);
  spread->T1 = &entity->tick;
  entity->spreads[strategy->idx] = '1';

  // 初始化T2
  entity = trader_strategy_processor_entity_get(self, strategy->T2);
  CMN_ASSERT(entity);
  spread->T2 = &entity->tick;
  entity->spreads[strategy->idx] = '1';

  spread->T1Weight = strategy->T1Weight;
  spread->T2Weight = strategy->T2Weight;

  return 0;
}

int trader_strategy_processor_spread_caculate(trader_strategy_processor* self, int spread_idx)
{
  CMN_DEBUG("Enter!\n");
  trader_spread* p = (trader_spread*)self->spreads;
  trader_spread* spread = &p[spread_idx];
  trader_tick* t1 = spread->T1;
  trader_tick* t2 = spread->T2;
  

  // 策略未初始化
  if(!t1){
    return 0;
  }

  if(!t2){
    return 0;
  }

  //行情未初始化
  if('\0' == t1->InstrumentID[0]){
    return 0;
  }

  if('\0' == t2->InstrumentID[0]){
    return 0;
  }

  spread->AskSpread = t1->AskPrice1 * spread->T1Weight - t2->AskPrice1 * spread->T2Weight;
  spread->BidSpread = t1->BidPrice1 * spread->T1Weight - t2->BidPrice1 * spread->T2Weight;

  CMN_INFO("t1->InstrumentID[%s]\n"
    "t2->InstrumentID[%s]\n"
    "spread->T1Weight[%lf]\n"
    "spread->T2Weight[%lf]\n"
    "spread->AskSpread[%lf]\n"
    "spread->BidSpread[%lf]\n"
    , t1->InstrumentID
    , t2->InstrumentID
    , spread->T1Weight
    , spread->T2Weight
    , spread->AskSpread
    , spread->BidSpread
  );
  
  return 1;
}

int trader_strategy_processor_set_loop_timer(trader_strategy_processor* self, trader_tick* tick_data)
{
  // 判断窗口期
  if(self->status){
    return 0;
  }

  self->status = 1;
  strncpy(self->currentTime, tick_data->UpdateTime, sizeof(self->currentTime));

  // 等待30微
  static struct timeval timeout = {
    0, 15*1000
  };
  evtimer_add(self->loopTimer, &timeout);

  return 0;
}




