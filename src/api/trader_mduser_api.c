
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/util.h>

#include "hiredis.h"
#include "trader_mduser_api.h"

trader_mduser_api* trader_mduser_api_new(evutil_socket_t fd, trader_mduser_api_method* method)
{
  trader_mduser_api* self = (trader_mduser_api*)malloc(sizeof(trader_mduser_api));
  memset(self, 0, sizeof(trader_mduser_api));

  self->fd = fd;
  self->pMethod = method;

  return self;
}

void trader_mduser_api_free(trader_mduser_api* self)
{
  if(self){
    free(self);
  }
}

void trader_mduser_api_set_user(trader_mduser_api* self, char* broker_id, char* user, char* pwd)
{
  self->pBrokerID = broker_id;
  self->pUser = user;
  self->pPwd = pwd;
}

void trader_mduser_api_set_front_addr(trader_mduser_api* self, char* addr)
{
  self->pAddress = addr;
}

void trader_mduser_api_set_workspace(trader_mduser_api* self, char* ws)
{
  self->pWorkspace = ws;
}

int trader_mduser_api_load_instruments(const char* configFile, const char* section, mduser_instrument** insts)
{
  char pRedisIp[14+1];
  int nRedisPort = 0;
  char pInstrumentKey[32];
  
  int nCount = 0;
  mduser_instrument* ppInstruments;
  
  redisContext* pRedisCtx;
  
  redisReply* reply;
  redisReply* r;
  
  int nRet = -1;
  int i;

  
  // 读取配置文件
  nRet = glbPflGetString(section, "REDIS_ADDR", configFile, pRedisIp);
  nRet = glbPflGetInt(section, "REDIS_PORT", configFile, &nRedisPort);
  nRet = glbPflGetString(section, "REDIS_INST_KEY", configFile, pInstrumentKey);

  struct timeval tv = {
    5, 0
  };
  
  pRedisCtx = redisConnectWithTimeout(pRedisIp, nRedisPort, tv);
  if(!pRedisCtx){
    return -1;
  }

  // 查询key列表
  
  reply = (redisReply*)redisCommand(pRedisCtx, "SMEMBERS %s", pInstrumentKey);
  
  do {
    if(REDIS_REPLY_ARRAY == reply->type){
      nRet = 0;
      nCount = reply->elements;
      ppInstruments = (mduser_instrument*)malloc(nCount * sizeof(mduser_instrument));
      for(i = 0; i < nCount; i++){
        r = reply->element[i];
        strncpy(ppInstruments[i], r->str, sizeof(mduser_instrument));
      }
      break;
    }
  }while(0);
  
  freeReplyObject(reply);
  
  redisFree(pRedisCtx);

  *insts = ppInstruments;

  return nCount;
}

void trader_mduser_api_on_front_connected(trader_mduser_api* self)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONFRONTCONNECTED;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_front_disconnected(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONFRONTDISCONNECTED;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_user_login(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRSPUSERLOGIN;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_user_logout(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRSPUSERLOGOUT;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_error(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRSPERROR;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strncpy(oEvent.ErrorMsg, err_msg, sizeof(oEvent.ErrorMsg)-1);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRSPSUBMARKETDATA;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_un_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRSPUNSUBMARKETDATA;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rtn_depth_market_data(trader_mduser_api* self, trader_tick* tick)
{
  trader_mduser_evt oEvent;
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  memcpy(&oEvent.Tick, tick, sizeof(trader_tick));

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

