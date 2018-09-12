
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_aud_pool.h"

#include "hiredis.h"

static int client_aud_pool_add_trader(client_aud_pool* self, struct bufferevent* buff_trader);
static int client_aud_pool_get_trader(client_aud_pool* self, struct bufferevent** buff_trader, char* user_id, char* passwd);
static int client_aud_pool_free_trader(client_aud_pool* self, char* user_id);

static client_aud_pool_method* client_aud_pool_method_get();

int client_aud_pool_add_trader(client_aud_pool* self, struct bufferevent* buff_trader)
{
  client_aud_trader* pTrader = (client_aud_trader*)malloc(sizeof(client_aud_trader));

  pTrader->nStatus = CLIENT_AUD_TRADER_STATUS_EMPTY;
  pTrader->BufTrader = buff_trader;


  TAILQ_INSERT_TAIL(&self->hCnnTraderList, pTrader, next);

  return 0;
}

int client_aud_pool_get_trader(client_aud_pool* self, struct bufferevent** buff_trader, char* user_id, char* passwd)
{
  int nRet = 0;
  client_aud_trader* pEmpty = NULL;
  client_aud_trader* pUsed = NULL;
  client_aud_trader* pIter;


  // 检查用户名是否可用  
  redisReply *reply;
  reply = redisCommand(self->redis, "SISMEMBER %s %s", "user", user_id);
  nRet = (int)reply->integer;
  freeReplyObject(reply);

  if(0 == nRet){
    //账号非法
    return -3;
  }

  TAILQ_FOREACH(pIter, &self->hCnnTraderList, next) {
    if(CLIENT_AUD_TRADER_STATUS_EMPTY == pIter->nStatus){
      pEmpty = pIter;
    }else {
      if(!strcmp(user_id, pIter->UserId)){
        pUsed = pIter;
      }
    }
  }

  if(!pUsed){
    if(!pEmpty){
      // 没找到空闲的tradersvr
      return -1; 
    }else{
      *buff_trader = pEmpty->BufTrader;
      pEmpty->nStatus = CLIENT_AUD_TRADER_STATUS_LOGINED;
      strncpy(pEmpty->UserId, user_id, sizeof(pEmpty->UserId));
      strncpy(pEmpty->Passwd, passwd, sizeof(pEmpty->Passwd));
      return 0;
    }
  }else{
    *buff_trader = pUsed->BufTrader;
    pUsed->nStatus = CLIENT_AUD_TRADER_STATUS_LOGINED;
    strncpy(pUsed->UserId, user_id, sizeof(pUsed->UserId));
    strncpy(pUsed->Passwd, passwd, sizeof(pUsed->Passwd));
    return 0;
  }

  return -3;
}

int client_aud_pool_free_trader(client_aud_pool* self, char* user_id)
{
  int nRet = 0;
  client_aud_trader* pIter;

  TAILQ_FOREACH(pIter, &self->hCnnTraderList, next) {
    if(!strcmp(user_id, pIter->UserId)){
      pIter->nStatus = CLIENT_AUD_TRADER_STATUS_LOGOUTED;
      break;
    }
  }

  return 0;
}


client_aud_pool_method* client_aud_pool_method_get()
{
  static client_aud_pool_method client_aud_pool_method_st = {
    client_aud_pool_add_trader,
    client_aud_pool_get_trader,
    client_aud_pool_free_trader
  };

  return &client_aud_pool_method_st;

}


client_aud_pool* client_aud_pool_new()
{
  client_aud_pool* self = (client_aud_pool*)malloc(sizeof(client_aud_pool));
  
  TAILQ_INIT(&self->hCnnTraderList);

  
  redisContext *c;
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  c = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
  if (c == NULL || c->err) {
    if (c) {
      redisFree(c);
    } else {
    
    }
    exit(-1);
  }

  self->redis = c;

  self->pMethod = client_aud_pool_method_get();

  return self;
}

void client_aud_pool_free(client_aud_pool* self)
{
  client_aud_trader* pIter;
  while((pIter = TAILQ_FIRST(&self->hCnnTraderList))!=NULL){
  	TAILQ_REMOVE(&self->hCnnTraderList, pIter, next);
    free(pIter);
  }
  if(self->redis){
    redisFree(self->redis);
  }

  if(self){
    free(self);
  }
}


