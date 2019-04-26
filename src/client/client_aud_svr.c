
#include "client_aud_svr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "cmn_log.h"
#include "cmn_cfg.h"

static int client_aud_svr_init(client_aud_svr* self, evutil_socket_t* socks, int num);
static int client_aud_svr_run(client_aud_svr* self);
static int client_aud_svr_accept(client_aud_svr* self, evutil_socket_t fd);
static int client_aud_svr_on_trader_event(client_aud_svr* self, client_aud_cnn* cnn);
static int client_aud_svr_on_client_event(client_aud_svr* self, client_aud_cnn* cnn);
static int client_aud_svr_exit(client_aud_svr* self);
static int client_aud_svr_gc(client_aud_svr* self);

static client_aud_svr_method* client_aud_svr_method_get();

// callback
static void client_aud_cnn_trade_read_cb(struct bufferevent *bev, void *arg);
static void client_aud_cnn_trade_write_cb(struct bufferevent *bev, void *arg);
static void client_aud_cnn_trade_evt_cb(struct bufferevent *bev, short event, void *arg);
static void client_aud_cnn_client_read_cb(struct bufferevent *bev, void *arg);
static void client_aud_cnn_client_write_cb(struct bufferevent *bev, void *arg);
static void client_aud_cnn_client_evt_cb(struct bufferevent *bev, short event, void *arg);
static void client_aud_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data);
static void client_aud_signal_cb(evutil_socket_t fd, short event, void *arg);
static void client_aud_gc_timeout_cb(evutil_socket_t fd, short event, void *arg);



client_aud_svr_method* client_aud_svr_method_get()
{
  static client_aud_svr_method st_client_aud_svr_method = {
    client_aud_svr_init,
    client_aud_svr_run,
    client_aud_svr_accept,
    client_aud_svr_on_trader_event,
    client_aud_svr_on_client_event,
    client_aud_svr_exit,
    client_aud_svr_gc
  };

  return &st_client_aud_svr_method;

}

int client_aud_svr_init(client_aud_svr* self, evutil_socket_t* socks, int num)
{
  CMN_DEBUG("ENTER!\n");
  
  struct sockaddr_in sin;
  int nRet;
  int i;
  struct bufferevent * traderBufferEvent;
  client_aud_cnn* pCnn;
  
  // 各种初始化
  self->pBase = event_base_new();
  CMN_ASSERT(self->pBase);
  
  //signal event init
  self->pSigIntEvt = evsignal_new(self->pBase, SIGINT, client_aud_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigIntEvt);
  
  self->pSigTermEvt = evsignal_new(self->pBase, SIGTERM, client_aud_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigTermEvt);

  
  self->pTmrGc = evtimer_new(self->pBase,  client_aud_gc_timeout_cb, (void*)self);
  CMN_ASSERT(self->pTmrGc);

  
	struct timeval tv = {
    30, 0
  };
  evtimer_add(self->pTmrGc, &tv);
  
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  //listen
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(self->sIp);
  sin.sin_port = htons(self->nPort);

  self->pListener = evconnlistener_new_bind(self->pBase, client_aud_listener_cb, (void *)self,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
    (struct sockaddr*)&sin,
    sizeof(sin));

  TAILQ_INIT(&self->hCnnFreeList);
  TAILQ_INIT(&self->hCnnUsedList);

  //self->pBufPool;
  self->pBufPool = client_aud_pool_new();

  // CNN
  for(i = 0; i < num; i++){
    
    pCnn = client_aud_cnn_new();
    pCnn->parent = self;
    pCnn->pool = self->pBufPool;
    TAILQ_INSERT_TAIL(&self->hCnnFreeList, pCnn, next);
    
    traderBufferEvent = bufferevent_socket_new(self->pBase, socks[i], 0);
    bufferevent_setcb(traderBufferEvent, client_aud_cnn_trade_read_cb, 
      client_aud_cnn_trade_write_cb, client_aud_cnn_trade_evt_cb, (void*)self);
    bufferevent_enable(traderBufferEvent, EV_READ|EV_PERSIST);
    

    self->pBufPool->pMethod->xAddTrader(self->pBufPool, traderBufferEvent);
    
  }

  return 0;

}

int client_aud_svr_run(client_aud_svr* self)
{
  int nRet = event_base_dispatch(self->pBase);

  return nRet;

}

int client_aud_svr_accept(client_aud_svr* self, evutil_socket_t fd)
{
  struct bufferevent * clientBufferEvent;
  client_aud_cnn* pCnn;

  if(TAILQ_EMPTY(&self->hCnnFreeList)){
    CMN_ERROR("no free cnn\n");
    evutil_closesocket(fd);
    return -1;
  }

  pCnn = TAILQ_FIRST(&self->hCnnFreeList);
  TAILQ_REMOVE(&self->hCnnFreeList, pCnn, next);
  TAILQ_INSERT_TAIL(&self->hCnnUsedList, pCnn, next);

  clientBufferEvent = bufferevent_socket_new(self->pBase, fd, BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(clientBufferEvent, client_aud_cnn_client_read_cb, 
    client_aud_cnn_client_write_cb, client_aud_cnn_client_evt_cb, (void*)pCnn);
  bufferevent_enable(clientBufferEvent, EV_READ|EV_WRITE);

  pCnn->BufClient = clientBufferEvent;

  pCnn->BufTrader = NULL;
  
  pCnn->nFree = 0;

  return 0;

}

int client_aud_svr_on_trader_event(client_aud_svr* self, client_aud_cnn* cnn)
{
  CMN_FATAL("Something wrong!\n");
}

int client_aud_svr_on_client_event(client_aud_svr* self, client_aud_cnn* cnn)
{
  CMN_DEBUG("client disconnected!\n");
  client_aud_cnn* pCnn = cnn;
  
  bufferevent_free(pCnn->BufClient);
  pCnn->BufClient = 0;
  TAILQ_REMOVE(&self->hCnnUsedList, pCnn, next);
  TAILQ_INSERT_TAIL(&self->hCnnFreeList, pCnn, next);

  return 0;
}

int client_aud_svr_exit(client_aud_svr* self)
{
  
  client_aud_cnn* pCnn;

  while((pCnn = TAILQ_FIRST(&self->hCnnUsedList))!=NULL){
  	TAILQ_REMOVE(&self->hCnnUsedList, pCnn, next);
    bufferevent_free(pCnn->BufClient);
    client_aud_cnn_free(pCnn);
  }

  while((pCnn = TAILQ_FIRST(&self->hCnnFreeList))!=NULL){
  	TAILQ_REMOVE(&self->hCnnFreeList, pCnn, next);
    client_aud_cnn_free(pCnn);
  }

  if(self->pBufPool){
    client_aud_pool_free(self->pBufPool);
  }
  
  evconnlistener_free(self->pListener);
  event_free(self->pSigIntEvt);
  event_free(self->pSigTermEvt);
  event_free(self->pTmrGc);
  event_base_free(self->pBase);
  
  return 0;
}

int client_aud_svr_gc(client_aud_svr* self)
{
  client_aud_cnn* p = (client_aud_cnn*)NULL;
  client_aud_cnn* q = (client_aud_cnn*)NULL;
  
  p = TAILQ_FIRST(&self->hCnnUsedList);
  while(p != TAILQ_END(&self->hCnnUsedList)) {
    q = TAILQ_NEXT(p, next);
    if(p->nFree) {
      self->pMethod->xOnClientEvent(self, p);
    }
    p = q;
  }

  
  return 0;
}


// Callback
void client_aud_cnn_trade_read_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_svr* self = (client_aud_svr*)arg;
  client_aud_cnn* pCnn = (client_aud_cnn*)NULL;
  client_aud_cnn* pIter = (client_aud_cnn*)NULL;
  long lCmp = 0;

  TAILQ_FOREACH(pIter, &self->hCnnUsedList, next){
    if(pIter->BufTrader == bev){
      if(NULL == pCnn){
        pCnn = pIter;
      }else {
        lCmp = pIter->tvLast.tv_sec - pCnn->tvLast.tv_sec;
        lCmp *= 100000;
        lCmp += pIter->tvLast.tv_usec - pCnn->tvLast.tv_usec;
        if(lCmp > 0){
          pCnn->nFree = 1;
          pCnn = pIter;
        }else{
          pIter->nFree = 1;
        }
      }
    }
  }

  if(pCnn){  
    pCnn->pMethod->xOnTraderRecv(pCnn);
  }
}

void client_aud_cnn_trade_write_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_svr* self = (client_aud_svr*)arg;
  client_aud_cnn* pCnn = (client_aud_cnn*)NULL;

  TAILQ_FOREACH(pCnn, &self->hCnnUsedList, next){
    if(pCnn->BufTrader == bev){
      pCnn->pMethod->xOnTraderSend(pCnn);
      break;
    }
  }

}

void client_aud_cnn_trade_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_svr* self = (client_aud_svr*)arg;
  client_aud_cnn* pCnn = (client_aud_cnn*)NULL;

  TAILQ_FOREACH(pCnn, &self->hCnnUsedList, next){
    if(pCnn->BufTrader == bev){
      self->pMethod->xOnTraderEvent(self, pCnn);
      break;
    }
  }
}

void client_aud_cnn_client_read_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_cnn* pCnn = (client_aud_cnn*)arg;
  client_aud_svr* self = (client_aud_svr*)pCnn->parent;
  int nRet = 0;
  nRet = pCnn->pMethod->xOnClientRecv(pCnn);
  if(nRet < 0){
    CMN_DEBUG("xOnClientRecv[%d]!\n");
    self->pMethod->xOnClientEvent(self, pCnn);
  }else{
    gettimeofday(&(pCnn->tvLast), NULL);
    pCnn->nFree = 0;
  }

}

void client_aud_cnn_client_write_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_cnn* pCnn = (client_aud_cnn*)arg;
  pCnn->pMethod->xOnClientSend(pCnn);

}

void client_aud_cnn_client_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_cnn* pCnn = (client_aud_cnn*)arg;

  pCnn->pMethod->xOnClose(pCnn);
  
  client_aud_svr* self = (client_aud_svr*)pCnn->parent;

  self->pMethod->xOnClientEvent(self, pCnn);

}

void client_aud_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
  CMN_DEBUG("Enter!\n");
  client_aud_svr* self = (client_aud_svr*)user_data;

  self->pMethod->xAccept(self, fd);

}

void client_aud_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  client_aud_svr* self = (client_aud_svr*)arg;
  struct event_base *base = self->pBase;
  struct timeval delay = { 1, 0 };

  event_del(self->pSigIntEvt);
  event_del(self->pSigTermEvt);
  event_del(self->pTmrGc);

  event_base_loopexit(base, &delay);

}

void client_aud_gc_timeout_cb(evutil_socket_t fd, short event, void *arg)
{
  CMN_DEBUG("Enter!\n");
  client_aud_svr* self = (client_aud_svr*)arg;
  
  self->pMethod->xGC(self);

  
	struct timeval tv = {
    30, 0
  };
  evtimer_add(self->pTmrGc, &tv);
}


client_aud_svr* client_aud_svr_new()
{
  client_aud_svr* self = (client_aud_svr*)malloc(sizeof(client_aud_svr));

  self->pMethod = client_aud_svr_method_get();

  return self;
}

void client_aud_svr_free(client_aud_svr* self)
{
  if(self){
    free(self);
  }
}

int aud_main(char* cfg_file, evutil_socket_t* socks, int num)
{
  CMN_LOG_INI(cfg_file, "traderAudSvr");
  cmn_util_change_proc("traderAudSvr");
  
  CMN_DEBUG("traderAudSvr\n");

  int nRet = 0;
  
  client_aud_svr* self = client_aud_svr_new();
  CMN_ASSERT(self);
  
  // 变量初始化
  cmn_cfg* pCfg = cmn_cfg_new(cfg_file);
  nRet = pCfg->pMethod->xInit(pCfg);
  CMN_ASSERT(!nRet);

  strncpy(self->sIp, pCfg->pMethod->xGetListenIP(pCfg), sizeof(self->sIp));
  self->nPort = pCfg->pMethod->xGetListenPort(pCfg);

  cmn_cfg_free(pCfg);
  
  // 服务初始化
  nRet = self->pMethod->xInit(self, socks, num);
  CMN_ASSERT(!nRet);
  
  self->pMethod->xRun(self);

  // 退出处理
  self->pMethod->xExit(self);
  
  client_aud_svr_free(self);

  CMN_LOG_FINI();
  return 0;


}


