
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "hiredis.h"

#include "cmn_log.h"

#include "trader_mduser_api.h"
#include "trader_mduser_cnn.h"
#include "trader_mduser_boardcast.h"

#include "trader_mduser_svr.h"

#define MDUSER_BOARDCAST_IP "MDUSER.BOARDCAST.IP"
#define MDUSER_BOARDCAST_PORT "MDUSER.BOARDCAST.PORT"

#define MDUSER_CNN_MAIN_BROKER_ID "MDUSER.CNN.MAIN.BROKER.ID"
#define MDUSER_CNN_MAIN_USER "MDUSER.CNN.MAIN.USER"
#define MDUSER_CNN_MAIN_PASSWD "MDUSER.CNN.MAIN.PASSWD"
#define MDUSER_CNN_MAIN_ADDR "MDUSER.CNN.MAIN.ADDR"

#define MDUSER_CNN_BACKUP_BROKER_ID "MDUSER.CNN.BACKUP.BROKER.ID"
#define MDUSER_CNN_BACKUP_USER "MDUSER.CNN.BACKUP.USER"
#define MDUSER_CNN_BACKUP_PASSWD "MDUSER.CNN.BACKUP.PASSWD"
#define MDUSER_CNN_BACKUP_ADDR "MDUSER.CNN.BACKUP.ADDR"

#define MDUSER_INSTRUMENTS "MDUSER.INSTRUMENTS"


static int trader_mduser_svr_init(trader_mduser_svr* self);
static int trader_mduser_svr_run(trader_mduser_svr* self);
static int trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt);
static int trader_mduser_svr_set_addr(trader_mduser_svr* self, char* addr);

static trader_mduser_svr_method* trader_mduser_svr_method_get();

//CALLBACK
static void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_mduser_svr_tick_cb(void* arg, trader_mduser_evt* evt);

static int trader_mduser_svr_init_cnn(trader_mduser_svr* self);
static int trader_mduser_svr_init_boardcast(trader_mduser_svr* self);
static int trader_mduser_svr_init_instruments(trader_mduser_svr* self);


static int trader_mduser_svr_redis_get(trader_mduser_svr* self, const char* key, char* val, int size);

static int trader_mduser_svr_instrument_sort(trader_mduser_svr* self);

static void* trader_mduser_svr_instrument_bsearch(trader_mduser_svr* self, char* inst);

trader_mduser_svr_method* trader_mduser_svr_method_get()
{
  static trader_mduser_svr_method st_trader_mduser_svr_method = {
    trader_mduser_svr_init,
    trader_mduser_svr_run,
    trader_mduser_svr_proc,
    trader_mduser_svr_set_addr
  };

  return &st_trader_mduser_svr_method;
}

int trader_mduser_svr_init_cnn(trader_mduser_svr* self)
{
  char brokerid[16];
  char user[16];
  char passwd[16];
  char addr[64];
  char workspace[64];
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
#include "trader_mduser_api_femas_method_get.h"
  api_imp = trader_mduser_api_femas_method_get();
#endif
  
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_MAIN_BROKER_ID, brokerid, sizeof(brokerid));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_MAIN_USER, user, sizeof(user));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_MAIN_PASSWD, passwd, sizeof(passwd));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_MAIN_ADDR, addr, sizeof(addr));
  self->pCnnMain->pMethod->xInit(self->pCnnMain, self->pBase,
    brokerid, user, passwd, addr, "./main/",
    trader_mduser_svr_tick_cb, self,
    self->instruments, self->instrumentNumber,
    api_imp);

  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_BACKUP_BROKER_ID, brokerid, sizeof(brokerid));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_BACKUP_USER, user, sizeof(user));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_BACKUP_PASSWD, passwd, sizeof(passwd));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_CNN_BACKUP_ADDR, addr, sizeof(addr));
  self->pCnnBackup->pMethod->xInit(self->pCnnBackup, self->pBase,
    brokerid, user, passwd, addr, "./backup/",
    trader_mduser_svr_tick_cb, self,
    self->instruments, self->instrumentNumber,
    api_imp);

  return 0;
}

int trader_mduser_svr_init_boardcast(trader_mduser_svr* self)
{
  char ip[32];
  char port[6];
  int nRet = 0;
  if(NULL == self->pAddr){
    nRet = trader_mduser_svr_redis_get(self, MDUSER_BOARDCAST_IP, ip, sizeof(ip));
    nRet = trader_mduser_svr_redis_get(self, MDUSER_BOARDCAST_PORT, port, sizeof(port));
  }else{
    char* q = strstr(self->pAddr, ":");
    if(NULL == q){
      strcpy(ip, "localhost");
      strncpy(port, self->pAddr, sizeof(port));
    }else{
      memcpy(ip, self->pAddr, q - self->pAddr);
      ip[q - self->pAddr] = '\0';
      strncpy(port, q + 1, sizeof(port));
    }
  }

  self->pBoardcast->method->xInit(self->pBoardcast, self->pBase,
    ip, atoi(port));
  
  return 0;
}

int trader_mduser_svr_init_instruments(trader_mduser_svr* self)
{
  int nRet = -1;
  int i;
  redisReply* reply = (redisReply*)redisCommand(self->pRedisCtx, "SMEMBERS %s", MDUSER_INSTRUMENTS);
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


int trader_mduser_svr_redis_get(trader_mduser_svr* self, const char* key, char* val, int size)
{
  int nRet = -1;
  redisReply* reply = (redisReply*)redisCommand(self->pRedisCtx, "GET %s", key);
  do {
    if(REDIS_REPLY_STRING == reply->type){
      nRet = 0;
      strncpy(val, reply->str, size);
      break;
    }
  }while(0);
  freeReplyObject(reply);

  return nRet;
}

int trader_mduser_svr_init(trader_mduser_svr* self)
{
  int nRet;
  evutil_socket_t pair[2];

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
  self->pCnnBackup= trader_mduser_cnn_new();
  CMN_ASSERT(self->pCnnBackup);

  CMN_DEBUG("self->pBoardcast\n");
  self->pBoardcast = trader_mduser_boardcast_new();
  CMN_ASSERT(self->pBoardcast);


  CMN_DEBUG("self->pRedisCtx\n");
  struct timeval tv = {
    5, 0
  };
  self->pRedisCtx = redisConnectWithTimeout("localhost", 6379, tv);
  CMN_ASSERT(self->pRedisCtx);

  CMN_ASSERT(0 == self->pRedisCtx->err);

  nRet = trader_mduser_svr_init_instruments(self);
  CMN_ASSERT(0 == nRet);

  nRet = trader_mduser_svr_init_cnn(self);
  CMN_ASSERT(0 == nRet);
  
  nRet = trader_mduser_svr_init_boardcast(self);
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

  self->pCnnMain->pMethod->xStart(self->pCnnMain);
  self->pCnnBackup->pMethod->xStart(self->pCnnBackup);
  
  nRet = event_base_dispatch(self->pBase);

  self->pCnnMain->pMethod->xStop(self->pCnnMain);
  self->pCnnBackup->pMethod->xStop(self->pCnnBackup);
  
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

int trader_mduser_svr_set_addr(trader_mduser_svr* self, char* addr)
{

  if(!addr){
    self->pAddr = (char*)NULL;
    return 0;
  }
  
  int len = strlen(addr);
  
  self->pAddr = (char*)malloc((len + 1) * sizeof(char));

  strncpy(self->pAddr, addr, len + 1);
  
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

    if(self->pAddr){
      free(self->pAddr);
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

  char* addr = (char*)NULL;
  if(argc >= 2){
    addr = argv[1];
  }
  
  svc->pMethod->xSetAddr(svc, addr);

  svc->pMethod->xInit(svc);

  svc->pMethod->xRun(svc);

  trader_mduser_svr_free(svc);

  return 0;
}


