
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "evmqueue.h"

#include "hiredis.h"

#include "glbProfile.h"

#include "cmn_log.h"

#include "trader_tick_dict.h"

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

static void trader_mduser_svr_mqueue_recv_cb(const char* data, int len, void* privdata);


static int trader_mduser_svr_init_cnn(trader_mduser_svr* self);
static int trader_mduser_svr_init_boardcast(trader_mduser_svr* self, char* ip, int port);
static int trader_mduser_svr_init_instruments(trader_mduser_svr* self);

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

#ifdef FEMAS_AF
  //FEMAS
#include "trader_mduser_api_femas_af.h"
  api_imp = trader_mduser_api_femas_af_method_get();
#endif

#ifdef REM
extern trader_mduser_api_method* trader_mduser_api_rem_method_get();
  api_imp = trader_mduser_api_rem_method_get();
#endif

#ifdef XSPEED_STOCK
#include "trader_mduser_api_xspeed.h"
  api_imp = trader_mduser_api_xspeed_stock_method_get();
#endif

#ifdef XSPEED
#include "trader_mduser_api_xspeed.h"
  api_imp = trader_mduser_api_xspeed_sop_method_get();
#endif

#ifdef LTS
  //LTS
#include "trader_mduser_api_lts.h"
  api_imp = trader_mduser_api_lts_method_get();
#endif

#ifdef GD_UDP_CTP
  extern trader_mduser_api_method* trader_mduser_api_gd_method_get();
    api_imp = trader_mduser_api_gd_method_get();
#endif

#ifdef GF_SHFE
extern trader_mduser_api_method* trader_mduser_api_gf_method_get();
  api_imp = trader_mduser_api_gf_method_get();
#endif

#ifdef GF_SOLARFLARE
    extern trader_mduser_api_method* trader_mduser_api_sf_method_get();
      api_imp = trader_mduser_api_sf_method_get();
#endif

#ifdef XHGZ_SF_REM
extern trader_mduser_api_method* trader_mduser_api_cffex_l2_method_get();
  api_imp = trader_mduser_api_cffex_l2_method_get();
#endif

#ifdef XHSP_EFH32_CTPMINI
extern trader_mduser_api_method* trader_mduser_api_efh32_method_get();
  api_imp = trader_mduser_api_efh32_method_get();
#endif

#ifdef SW_EFH32_REM
extern trader_mduser_api_method* trader_mduser_api_sw_method_get();
  api_imp = trader_mduser_api_sw_method_get();
#endif

#ifdef SW_EFH32_FEMAS302
extern trader_mduser_api_method* trader_mduser_api_sw_method_get();
  api_imp = trader_mduser_api_sw_method_get();
#endif

#ifdef SW_EFH32_CTP66
extern trader_mduser_api_method* trader_mduser_api_sw_method_get();
  api_imp = trader_mduser_api_sw_method_get();
#endif

#ifdef EFVI
extern trader_mduser_api_method* trader_mduser_api_ef_vi_method_get();
  api_imp = trader_mduser_api_ef_vi_method_get();
#endif

#ifdef UDP
extern trader_mduser_api_method* trader_mduser_api_udp_method_get();
  api_imp = trader_mduser_api_udp_method_get();
#endif

  self->pCnnMain->pMethod->xInit(self->pCnnMain, self->pBase,
    self->mainBrokerId, self->mainUser, self->mainPasswd, self->mainAddr, self->mainWorkspace,
    trader_mduser_svr_tick_cb, self,
    self->instruments, self->instrumentNumber,
    api_imp);

  if(!self->pCnnBackup){
    return 0;
  }

#ifdef CTP_BACKUP
extern trader_mduser_api_method* trader_mduser_api_ctp_method_get();
    api_imp = trader_mduser_api_ctp_method_get();
#endif

#ifdef REM_BACKUP
extern trader_mduser_api_method* trader_mduser_api_rem_method_get();
  api_imp = trader_mduser_api_rem_method_get();
#endif

#ifdef FEMAS_AF_BACKUP
extern trader_mduser_api_method* trader_mduser_api_femas_af_method_get();
  api_imp = trader_mduser_api_femas_af_method_get();
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
  trader_tick* tick = (trader_tick*)NULL;
  self->ticks = (trader_tick*)NULL;
  self->tickDict = (trader_tick_dict*)NULL;
  self->instrumentNumber = 0;
  self->instruments = (trader_instrument_id_type*)NULL;
  
  redisReply* reply = (redisReply*)redisCommand(self->pRedisCtx, "SMEMBERS %s", self->redisInstrumentKey);
  redisReply* r;
  do {
    if(REDIS_REPLY_ARRAY == reply->type){

      
      nRet = 0;
      self->instrumentNumber = reply->elements;
      self->shmHdr = trader_mduser_shm_header_init(self->redisInstrumentKey, sizeof(trader_tick), (self->instrumentNumber / 8 + 1) * 8);
      self->ticks = trader_mduser_shm_header_calloc(self->shmHdr, self->instrumentNumber);
      self->tickDict = trader_tick_dict_new();
      self->instruments = (trader_instrument_id_type*)malloc(self->instrumentNumber * sizeof(trader_instrument_id_type));
      for(i = 0; i < self->instrumentNumber; i++){
        r = reply->element[i];
        tick = &self->ticks[i];
        memset(tick, 0, sizeof(trader_tick));
        strncpy(tick->InstrumentID, r->str, sizeof(tick->InstrumentID));
        self->tickDict->pMethod->xAdd(self->tickDict, tick->InstrumentID, (void*)tick);
        strncpy(self->instruments[i], r->str, sizeof(trader_instrument_id_type));
      }
      break;
    }
  }while(0);
  freeReplyObject(reply);

  return nRet;
}


void* trader_mduser_svr_instrument_bsearch(trader_mduser_svr* self, char* key)
{
  void* save_ptr;
  int found = self->tickDict->pMethod->xFind(self->tickDict, key, &save_ptr);
  if(!found){
    return (void*)NULL;
  }
  
  return save_ptr;
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
  
  nRet = glbPflGetString("RUN_CONFIG", "MQUEUE_NAME", argv[1], self->mqueueName);
  if(nRet < 0){
    self->mqueueName[0] = '\0';
  }

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

  self->mqueueContext = NULL;
  if(self->mqueueName[0]){
    mqueue_async_context* c = mqueue_async_context_open(self->mqueueName, 2000, 512);
    CMN_ASSERT(c);
    self->mqueueContext = c;
    
    mqueue_async_context_libevent_attach(c, self->pBase);
    
    mqueue_async_context_set_recv_callback(c, trader_mduser_svr_mqueue_recv_cb, self);

  }
  
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

  if(self->mqueueContext){
    mqueue_async_context_close(self->mqueueContext);
    
    mqueue_async_context_free(self->mqueueContext);
    
    mqueue_context_unlink(self->mqueueName);
  }
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
  || (current->AskPrice1 != tick->AskPrice1)
  || (current->BidPrice1 != tick->BidPrice1)){
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

void trader_mduser_svr_mqueue_recv_cb(const char* data, int len, void* privdata)
{
  trader_mduser_svr* self = (trader_mduser_svr*)privdata;
  trader_mduser_evt* evt = (trader_mduser_evt*)data;
  self->pMethod->xProc(self, evt);
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
    
    if(self->shmHdr){
      trader_mduser_shm_header_dt(self->shmHdr);
    }
    
    if(self->tickDict){
      trader_tick_dict_free(self->tickDict);
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


