

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hiredis.h"

#include "trader_data.h"
#include "trader_msg_struct.h"

#include "cmn_log.h"

#include "trader_strategy_position_svr.h"

#define STRATEGY_POSITION_INDEX(stage_id, direction) \
  (((stage_id) << 1) | ((direction) & 0x1)) & (64 - 1)


static void trader_strategy_position_svr_init(trader_strategy_position_svr* self, const char* account);
static void trader_strategy_position_svr_update(trader_strategy_position_svr* self, trade_position* p_position);
static int trader_strategy_position_svr_query(trader_strategy_position_svr* self, int stage_id, char direction, trade_position** pp_position);

static void trader_strategy_position_svr_set(trader_strategy_position_svr* self, trade_position* p_position);
static void trader_strategy_position_svr_get(trader_strategy_position_svr* self, trade_position* p_position);

static trader_strategy_position_svr_method* trader_strategy_position_svr_method_get();

trader_strategy_position_svr_method* trader_strategy_position_svr_method_get()
{
  static trader_strategy_position_svr_method trader_strategy_position_svr_method_st = {
    trader_strategy_position_svr_init,
    trader_strategy_position_svr_update,
    trader_strategy_position_svr_query
  };

  return &trader_strategy_position_svr_method_st;
}

void trader_strategy_position_svr_init(trader_strategy_position_svr* self, const char* account)
{
  self->account = account;
  int i;
  int idx;
  trade_position* position;
  for(i = 0; i < 32; i++){
    idx = STRATEGY_POSITION_INDEX(i, TRADER_POSITION_BUY);
    position = &self->position[idx];
    position->StageID = i;
    position->Direction = TRADER_POSITION_BUY;
    trader_strategy_position_svr_get(self, position);
    
    idx = STRATEGY_POSITION_INDEX(i, TRADER_POSITION_SELL);
    position = &self->position[idx];
    position->StageID = i;
    position->Direction = TRADER_POSITION_SELL;
    trader_strategy_position_svr_get(self, position);
  }
  

}

void trader_strategy_position_svr_update(trader_strategy_position_svr* self, trade_position* p_position)
{
  trade_position* position = p_position;
  trader_strategy_position_svr_set(self, position);
}

int trader_strategy_position_svr_query(trader_strategy_position_svr* self, int stage_id, char direction, trade_position** pp_position)
{
  int idx = STRATEGY_POSITION_INDEX(stage_id, direction);

  *pp_position = &self->position[idx];

  return idx;
}

void trader_strategy_position_svr_set(trader_strategy_position_svr* self, trade_position* p_position)
{
  char sCmd[64]; 
  redisReply *reply;
  trade_position* position = p_position;
  snprintf(sCmd, sizeof(sCmd), "SET SP_%s_%d_%c %d_%f_%f_%s_%s"
    , self->account, position->StageID, position->Direction
    , position->Volume
    , position->RealPrice
    , position->ExpPrice
    , position->T1
    , position->T2
  );

  CMN_DEBUG(sCmd);
  
  reply = redisCommand((redisContext*)self->redis, sCmd);
  
  if(REDIS_REPLY_ERROR == reply->type){
    CMN_ERROR("call redis error[%s]", reply->str);
  }
  freeReplyObject(reply);
  return;
}

void trader_strategy_position_svr_get(trader_strategy_position_svr* self, trade_position* p_position)
{
  char sCmd[64]; 
  char* pSavePtr;
  char sReply[256];
  char* pTemp;

  redisReply *reply;
  trade_position* position = p_position;

  snprintf(sCmd, sizeof(sCmd), "GET SP_%s_%d_%c"
    , self->account, position->StageID, position->Direction
    );
  CMN_DEBUG("redis-cli %s\n", sCmd);
  
  reply = (redisReply*)redisCommand((redisContext*)self->redis, sCmd);

  do{
    if(REDIS_REPLY_STRING != reply->type){
      position->T1[0] = '\0';
      position->T2[0] = '\0';
      position->Volume = 0;
      position->RealPrice = 0.0f;
      position->ExpPrice = 0.0f;
      break;
    }
    CMN_DEBUG("reply=[%s]\n", reply->str);

    strncpy(sReply, reply->str, sizeof(sReply));
    
    pTemp = strtok_r(sReply, "_", &pSavePtr);
    position->Volume = atoi(pTemp);
    
    pTemp = strtok_r(NULL, "_", &pSavePtr);
    position->RealPrice = atof(pTemp);
    
    pTemp = strtok_r(NULL, "_", &pSavePtr);
    position->ExpPrice = atof(pTemp);

    pTemp = strtok_r(NULL, "_", &pSavePtr);
    if(pTemp){
      strncpy(position->T1, pTemp, sizeof(position->T1));
    }
    
    pTemp = strtok_r(NULL, "_", &pSavePtr);
    if(pTemp){
      strncpy(position->T2, pTemp, sizeof(position->T2));
    }

    
  }while(0);
  freeReplyObject(reply);
  return;
}

trader_strategy_position_svr* trader_strategy_position_svr_new(void* redis)
{
  trader_strategy_position_svr* self = (trader_strategy_position_svr*)malloc(sizeof(trader_strategy_position_svr));

  int i;  
  for(i = 0; i < sizeof(self->position)/ sizeof(trade_position); i++){
    memset(&self->position[i], 0, sizeof(trade_position));
  }
  
  self->redis = redis;
  self->pMethod = trader_strategy_position_svr_method_get();
  
  return self;
}


void trader_strategy_position_svr_free(trader_strategy_position_svr* self)
{
  if(self){
    free(self);
  }
}



