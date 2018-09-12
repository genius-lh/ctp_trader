
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "cmn_util.h"

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "trader_mduser_cnn.h"

static trader_mduser_cnn_method* trader_mduser_cnn_method_get();

static int trader_mduser_cnn_init(trader_mduser_cnn* self,  struct event_base* base,
                                      char* broker_id, char* user, char* pwd, char* addr, char* ws,
                                      trader_mduser_cnn_tick_callback tick_callback, void* user_data,
                                      trader_instrument_id_type* insts, int inst_num,
                                      trader_mduser_api_method* api_imp);

static int trader_mduser_cnn_start(trader_mduser_cnn* self);
static int trader_mduser_cnn_proc(trader_mduser_cnn* self, trader_mduser_evt* evt);
static int trader_mduser_cnn_stop(trader_mduser_cnn* self);

static void trader_mduser_cnn_read_cb(struct bufferevent *bev, void *arg);
static void trader_mduser_cnn_evt_cb(struct bufferevent *bev, short event, void *arg);


trader_mduser_cnn_method* trader_mduser_cnn_method_get()
{
  static trader_mduser_cnn_method trader_mduser_cnn_method_st = {
    trader_mduser_cnn_init,
    trader_mduser_cnn_start,
    trader_mduser_cnn_proc,
    trader_mduser_cnn_stop
  };

  return &trader_mduser_cnn_method_st;
}


int trader_mduser_cnn_init(trader_mduser_cnn* self,  struct event_base* base,
                               char* broker_id, char* user, char* pwd, char* addr, char* ws,
                               trader_mduser_cnn_tick_callback tick_callback, void* user_data,
                               trader_instrument_id_type* insts, int inst_num,
                               trader_mduser_api_method* api_imp)
{
  self->pBase = base;
  
  strncpy(self->sBrokerId, broker_id, sizeof(self->sBrokerId));
  strncpy(self->sUser, user, sizeof(self->sUser));
  strncpy(self->sPasswd, pwd, sizeof(self->sPasswd));
  strncpy(self->sAddr, addr, sizeof(self->sAddr));
  strncpy(self->sWorkspace, ws, sizeof(self->sWorkspace));

  self->tickCallback = tick_callback;
  self->userData = user_data;

  self->instruments = insts;
  self->instrumentNumber = inst_num;

  self->pApiImp = api_imp;

  return 0;
}

int trader_mduser_cnn_start(trader_mduser_cnn* self)
{
  int nRet;
  evutil_socket_t pair[2];
  
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);
  
  self->pBufMduser = bufferevent_socket_new(self->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(self->pBufMduser, trader_mduser_cnn_read_cb, 
    NULL, trader_mduser_cnn_evt_cb, (void*)self);
  bufferevent_enable(self->pBufMduser, EV_READ|EV_PERSIST);

  self->pMduserApi = trader_mduser_api_new(pair[1], self->pApiImp);
  self->pMduserApi->pMethod->xSetUser(self->pMduserApi, self->sBrokerId, self->sUser, self->sPasswd);
  self->pMduserApi->pMethod->xSetFrontAddr(self->pMduserApi, self->sAddr);
  self->pMduserApi->pMethod->xSetWorkspace(self->pMduserApi, self->sWorkspace);
  
  self->pMduserApi->pMethod->xStart(self->pMduserApi);

  self->nPair = pair[1];

  return 0;
}

int trader_mduser_cnn_proc(trader_mduser_cnn* self, trader_mduser_evt* evt)
{
  trader_mduser_evt* pEvt = evt;

  int nType = pEvt->Type;

  int i;

  if(ONRTNDEPTHMARKETDATA == nType){
    if(self->tickCallback){
      (self->tickCallback)(self->userData, pEvt);
    }
    return 0;
  }

  if(ONFRONTCONNECTED == nType){
    self->pMduserApi->pMethod->xLogin(self->pMduserApi);
  }else if(ONRSPUSERLOGIN == nType){
    if(0 == pEvt->ErrorCd){
      for(i = 0; i < self->instrumentNumber; i++){
        self->pMduserApi->pMethod->xSubscribe(self->pMduserApi, self->instruments[i]);
      }
    }else{
      //TODO
      event_base_loopexit(self->pBase, NULL);
    }
  }
  
  return 0;
}

int trader_mduser_cnn_stop(trader_mduser_cnn* self)
{
  self->pMduserApi->pMethod->xStop(self->pMduserApi);
  
  trader_mduser_api_free(self->pMduserApi);

  bufferevent_free(self->pBufMduser);
  
  close(self->nPair);

  return 0;
}


void trader_mduser_cnn_read_cb(struct bufferevent *bev, void *arg)
{
  trader_mduser_cnn* self = (trader_mduser_cnn*)arg;
  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_mduser_evt oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(self->pBufMduser, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    self->pMethod->xProc(self, &oMsg);
  }while(loop);
  
}

void trader_mduser_cnn_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_mduser_cnn* self = (trader_mduser_cnn*)arg;
}


trader_mduser_cnn* trader_mduser_cnn_new()
{
  trader_mduser_cnn* self = (trader_mduser_cnn*)malloc(sizeof(trader_mduser_cnn));

  self->pMethod = trader_mduser_cnn_method_get();
    
  return self;

}

void trader_mduser_cnn_free(trader_mduser_cnn* self)
{
  if(self){
    free(self);
  }
}


