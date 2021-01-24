
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "hiredis.h"

#include "glbProfile.h"

#include "cmn_log.h"

#include "trader_mduser_api.h"
#include "trader_mduser_cnn.h"
#include "trader_mduser_boardcast.h"

#include "trader_mduser_svr.h"

#define MDUSER_INSTRUMENTS "MDUSER.INSTRUMENTS"

static int trader_mduser_svr_init(trader_mduser_svr* self, int argc, char* argv[]);
static int trader_mduser_svr_run(trader_mduser_svr* self);
static int trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt);

static trader_mduser_svr_method* trader_mduser_svr_method_get();

//CALLBACK
static void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_mduser_svr_tick_cb(void* arg, trader_mduser_evt* evt);

static int trader_mduser_svr_init_cnn(trader_mduser_svr* self);
static int trader_mduser_svr_init_boardcast(trader_mduser_svr* self, char* ip, int port);
static int trader_mduser_svr_init_instruments(trader_mduser_svr* self);

static int trader_mduser_svr_instrument_sort(trader_mduser_svr* self);

static void* trader_mduser_svr_instrument_bsearch(trader_mduser_svr* self, char* inst);

trader_mduser_svr_method* trader_mduser_svr_method_get()
{
  static trader_mduser_svr_method st_trader_mduser_svr_method = {
    trader_mduser_svr_init,
    trader_mduser_svr_run,
    trader_mduser_svr_proc
  };

  return &st_trader_mduser_svr_method;
}

int trader_mduser_svr_init_cnn(trader_mduser_svr* self)
{
  trader_mduser_api_method* api_imp = NULL;
  int nRet = 0;
#ifdef LTS
  //LTS
#include "trader_mduser_api_lts.h"
  api_imp = trader_mduser_api_lts_method_get();
#endif

#ifdef CTP
  //CTP
#include "trader_mduser_api_ctp.h"
  api_imp = trader_mduser_api_ctp_method_get();
#endif

#ifdef FEMAS
  //FEMAS
#include "trader_mduser_api_femas.h"
  api_imp = trader_mduser_api_femas_method_get();
#endif

#ifdef XSPEED_STOCK
#include "trader_mduser_api_xspeed.h"
  api_imp = trader_mduser_api_xspeed_stock_method_get();
#endif

#ifdef XSPEED
#include "trader_mduser_api_xspeed.h"
  api_imp = trader_mduser_api_xspeed_sop_method_get();
#endif

#ifdef GF_CFFEX
extern trader_mduser_api_method* trader_mduser_api_gf_method_get();
  api_imp = trader_mduser_api_gf_method_get();
#endif

#ifdef GF_SHFE
extern trader_mduser_api_method* trader_mduser_api_gf_method_get();
  api_imp = trader_mduser_api_gf_method_get();
#endif

#ifdef GD_CFFEX
  extern trader_mduser_api_method* trader_mduser_api_gd_method_get();
    api_imp = trader_mduser_api_gd_method_get();
#endif

#ifdef GF_SOLARFLARE
    extern trader_mduser_api_method* trader_mduser_api_sf_method_get();
      api_imp = trader_mduser_api_sf_method_get();
#endif

  self->pCnnMain->pMethod->xInit(self->pCnnMain, self->pBase,
    self->mainBrokerId, self->mainUser, self->mainPasswd, self->mainAddr, self->mainWorkspace,
    trader_mduser_svr_tick_cb, self,
    self->instruments, self->instrumentNumber,
    api_imp);

  if(!self->pCnnBackup){
    return 0;
  }
#ifdef GF_CTP_BACKUP
    //CTP
extern trader_mduser_api_method* trader_mduser_api_ctp_method_get();
    api_imp = trader_mduser_api_ctp_method_get();
#endif
  
  self->pCnnBackup->pMethod->xInit(self->pCnnBackup, self->pBase,
    self->backupBrokerId, self->backupUser, self->backupPasswd, self->backupAddr, self->backupWorkspace,
    trader_mduser_svr_tick_cb, self,
    self->instruments, self->instrumentNumber,
    api_imp);

  return 0;
}

int trader_mduser_svr_init_boardcast(trader_mduser_svr* self, char* ip, int port)
{
  if(0 != port){
    self->pBoardcast->method->xInit(self->pBoardcast, self->pBase,
      ip, port);
  }else{
    self->pBoardcast->method->xInitUnix(self->pBoardcast, self->pBase,
      ip);
  }
  
  return 0;
}

int trader_mduser_svr_init_instruments(trader_mduser_svr* self)
{
  int nRet = -1;
  int i;
  self->instruments = (trader_instrument_id_type*)NULL;
  self->ticks = (trader_tick*)NULL;
  self->instrumentNumber = 0;
  
  redisReply* reply = (redisReply*)redisCommand(self->pRedisCtx, "SMEMBERS %s", self->redisInstrumentKey);
  redisReply* r;
  do {
    if(REDIS_REPLY_ARRAY == reply->type){
      nRet = 0;
      self->instrumentNumber = reply->elements;
      self->instruments = (trader_instrument_id_type*)malloc(self->instrumentNumber * sizeof(trader_instrument_id_type));
      self->ticks = (trader_tick*)malloc(self->instrumentNumber * sizeof(trader_tick));
      for(i = 0; i < self->instrumentNumber; i++){
        r = reply->element[i];
        strncpy(self->instruments[i], r->str, sizeof(trader_instrument_id_type));
        memset(&self->ticks[i], 0, sizeof(trader_tick));
        strncpy(self->ticks[i].InstrumentID, r->str, sizeof(trader_instrument_id_type));
      }
      trader_mduser_svr_instrument_sort(self);
      break;
    }
  }while(0);
  freeReplyObject(reply);

  return nRet;
}

int trader_mduser_svr_instrument_sort(trader_mduser_svr* self)
{
  int i;
  int j;
  int c;

  trader_instrument_id_type tmp; 

  for(i = 0; i < self->instrumentNumber; i++){
    for(j = i; j < self->instrumentNumber; j++){
      c = strcmp(self->ticks[i].InstrumentID, self->ticks[j].InstrumentID);
      if(c > 0){
        strcpy(tmp, self->ticks[i].InstrumentID);
        strcpy(self->ticks[i].InstrumentID, self->ticks[j].InstrumentID);
        strcpy(self->ticks[j].InstrumentID, tmp);
      }
    }
  }
  return 0;
}

void* trader_mduser_svr_instrument_bsearch(trader_mduser_svr* self, char* key)
{
  int h;
  int l;
  int m;
  int c;
  h = self->instrumentNumber - 1;
  l = 0;
  m = self->instrumentNumber / 2;

  while (l <= h) {
    m = (l + h) / 2;

    c = strcmp(self->ticks[m].InstrumentID, key);
    if(0 == c){
      return (void*)&self->ticks[m];
    }else if(c > 0) {
      h = m - 1;
    }else{
      l = m + 1;
    }
  }
  return (void*)NULL;
}

int trader_mduser_svr_init(trader_mduser_svr* self, int argc, char* argv[])
{
  int nRet;
  char pRedisIp[14+1];
  int nRedisPort = 0;
  char pBoardcastAddr[14+1];
  int nBoardcastPort = 0;
  int nUseBackup = 1;

  // 读取参数
  nRet = glbPflGetString("RUN_CONFIG", "REDIS_ADDR", argv[1], pRedisIp);
  nRet = glbPflGetInt("RUN_CONFIG", "REDIS_PORT", argv[1], &nRedisPort);
  nRet = glbPflGetString("RUN_CONFIG", "REDIS_INST_KEY", argv[1], self->redisInstrumentKey);
  if(nRet < 0){
    strncpy(self->redisInstrumentKey, MDUSER_INSTRUMENTS, sizeof(self->redisInstrumentKey));
  }
  
  nRet = glbPflGetString("RUN_CONFIG", "BOARDCAST_ADDR", argv[1], pBoardcastAddr);
  nRet = glbPflGetInt("RUN_CONFIG", "BOARDCAST_PORT", argv[1], &nBoardcastPort);
  
  nRet = glbPflGetInt("RUN_CONFIG", "USE_BACKUP", argv[1], &nUseBackup);
  
  nRet = glbPflGetString("MDUSER_MAIN", "MDUSER_BROKER_ID", argv[1], self->mainBrokerId);
  nRet = glbPflGetString("MDUSER_MAIN", "MDUSER_USER", argv[1], self->mainUser);
  nRet = glbPflGetString("MDUSER_MAIN", "MDUSER_PASSWD", argv[1], self->mainPasswd);
  nRet = glbPflGetString("MDUSER_MAIN", "MDUSER_ADDR", argv[1], self->mainAddr);
  nRet = glbPflGetString("MDUSER_MAIN", "MDUSER_WORKSPACE", argv[1], self->mainWorkspace);
  
  nRet = glbPflGetString("MDUSER_BACKUP", "MDUSER_BROKER_ID", argv[1], self->backupBrokerId);
  nRet = glbPflGetString("MDUSER_BACKUP", "MDUSER_USER", argv[1], self->backupUser);
  nRet = glbPflGetString("MDUSER_BACKUP", "MDUSER_PASSWD", argv[1], self->backupPasswd);
  nRet = glbPflGetString("MDUSER_BACKUP", "MDUSER_ADDR", argv[1], self->backupAddr);
  nRet = glbPflGetString("MDUSER_BACKUP", "MDUSER_WORKSPACE", argv[1], self->backupWorkspace);

  // 各种初始化
  CMN_DEBUG("self->pBase\n");
  self->pBase = event_base_new();
  CMN_ASSERT(self->pBase);
  
  //signal event init
  CMN_DEBUG("self->pSigIntEvt\n");
  self->pSigIntEvt = evsignal_new(self->pBase, SIGINT, trader_mduser_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigIntEvt);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  self->pSigTermEvt = evsignal_new(self->pBase, SIGTERM, trader_mduser_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigTermEvt);
   
  CMN_DEBUG("self->pCnnMain\n");
  self->pCnnMain = trader_mduser_cnn_new();
  CMN_ASSERT(self->pCnnMain);
  
  CMN_DEBUG("self->pCnnBackup\n");
  self->pCnnBackup = (trader_mduser_cnn*)NULL;
  if(nUseBackup){
    self->pCnnBackup = trader_mduser_cnn_new();
    CMN_ASSERT(self->pCnnBackup);
  }

  CMN_DEBUG("self->pBoardcast\n");
  self->pBoardcast = trader_mduser_boardcast_new();
  CMN_ASSERT(self->pBoardcast);

  CMN_DEBUG("self->pRedisCtx\n");
  struct timeval tv = {
    5, 0
  };
  self->pRedisCtx = redisConnectWithTimeout(pRedisIp, nRedisPort, tv);
  CMN_ASSERT(self->pRedisCtx);

  CMN_ASSERT(0 == self->pRedisCtx->err);

  nRet = trader_mduser_svr_init_instruments(self);
  CMN_ASSERT(0 == nRet);

  nRet = trader_mduser_svr_init_cnn(self);
  CMN_ASSERT(0 == nRet);
  
  nRet = trader_mduser_svr_init_boardcast(self, pBoardcastAddr, nBoardcastPort);
  CMN_ASSERT(0 == nRet);
  
  return 0;
}

int trader_mduser_svr_run(trader_mduser_svr* self)
{
  int nRet;
  
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(nRet >= 0);

  if(self->pCnnMain){
    self->pCnnMain->pMethod->xStart(self->pCnnMain);
  }
  
  if(self->pCnnBackup){
    self->pCnnBackup->pMethod->xStart(self->pCnnBackup);
  }
  
  nRet = event_base_dispatch(self->pBase);

  if(self->pCnnMain){
    self->pCnnMain->pMethod->xStop(self->pCnnMain);
  }
  
  if(self->pCnnBackup){
    self->pCnnBackup->pMethod->xStop(self->pCnnBackup);
  }
  
  self->pBoardcast->method->xExit(self->pBoardcast);

  redisFree(self->pRedisCtx);
  
  return 0;
}

int  trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt)
{
  trader_tick* current;
  trader_tick* tick;
  
  current = (trader_tick*)trader_mduser_svr_instrument_bsearch(self, evt->Tick.InstrumentID);
  if(NULL == current){
    return -1;
  }

  tick = &evt->Tick;
  if(0 != strcmp(current->UpdateTime, tick->UpdateTime)
  || (current->UpdateMillisec != tick->UpdateMillisec)){
    memcpy(current, tick, sizeof(trader_tick));
    self->pBoardcast->method->xBoardcase(self->pBoardcast, (char*)evt, sizeof(trader_mduser_evt));
  }
  return 0;
}

void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_svr* self = (trader_mduser_svr*)arg;
  struct event_base *base = self->pBase;
  struct timeval delay = { 1, 0 };

  event_del(self->pSigIntEvt);
  event_del(self->pSigTermEvt);

  event_base_loopexit(base, &delay);
  
}

void trader_mduser_svr_tick_cb(void* arg, trader_mduser_evt* evt)
{
  trader_mduser_svr* self = (trader_mduser_svr*)arg;
  int nRet = self->pMethod->xProc(self, evt);
}

trader_mduser_svr* trader_mduser_svr_new()
{
  trader_mduser_svr* self = (trader_mduser_svr*)malloc(sizeof(trader_mduser_svr));
  self->pMethod = trader_mduser_svr_method_get();
  return self;
}

void trader_mduser_svr_free(trader_mduser_svr* self)
{
  if(self) {
    if(self->instruments){
      free(self->instruments);
    }
    
    if(self->ticks){
      free(self->ticks);
    }
    
    if(self->pCnnMain){
      trader_mduser_cnn_free(self->pCnnMain);
    }
    
    if(self->pCnnBackup){
      trader_mduser_cnn_free(self->pCnnBackup);
    }

    if(self->pBoardcast){
      trader_mduser_boardcast_free(self->pBoardcast);
    }

    if(self->pSigTermEvt) {
      event_free(self->pSigTermEvt);
    }

    if(self->pSigIntEvt) {
      event_free(self->pSigIntEvt);
    }

    if(self->pBase) {
      event_base_free(self->pBase);
    }    
    
    free(self);
  }

}

int main(int argc, char* argv[])
{
  trader_mduser_svr* svc = trader_mduser_svr_new();

  svc->pMethod->xInit(svc, argc, argv);

  svc->pMethod->xRun(svc);

  trader_mduser_svr_free(svc);

  return 0;
}


