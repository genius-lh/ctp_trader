
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/event.h>
#include <event2/http.h>

#include <time.h>
#include <sys/time.h>

#include "glbProfile.h"

#include "trader_md_data_svr.h"

static int trader_md_data_svr_init_param(trader_md_data_svr* self, int argc, char* argv[]);
static int trader_md_data_svr_run(trader_md_data_svr* self);
static int trader_md_data_svr_on_tick_timer(trader_md_data_svr* self);
static int trader_md_data_svr_req_tick_data(trader_md_data_svr* self, const char* t1, const char* t2,
  double m1, double m2, double tp, double* bid, double* ask);
static trader_md_data_svr_method* trader_md_data_svr_method_get();

static void trader_md_data_svr_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_md_data_svr_http_cb(struct evhttp_request *req, void *arg);
static void trader_md_data_svr_timer_cb(evutil_socket_t fd, short event, void *arg);

static void trader_md_data_svr_tick_update(trader_md_data_svr* self);
static int trader_md_data_svr_tick_index(trader_md_data_svr* self, const char* contract);
static int trader_md_data_svr_get_tick_diff(trader_md_data_svr* self, int frame, int t1, int t2,
  double w1, double w2, double tick_price, double* bid, double* ask);
static trader_tick* trader_md_data_svr_get_tick(trader_md_data_svr* self, int frame, int idx);
static double trader_md_data_svr_bid_diff(trader_tick* t1, trader_tick* t2, double w1, double w2, double tick_price);
static double trader_md_data_svr_ask_diff(trader_tick* t1, trader_tick* t2, double w1, double w2, double tick_price);

trader_md_data_svr_method* trader_md_data_svr_method_get()
{
  static trader_md_data_svr_method stTraderMdDataSvrMethods = {
    trader_md_data_svr_init_param,
    trader_md_data_svr_run,
    trader_md_data_svr_on_tick_timer,
    trader_md_data_svr_req_tick_data
  };

  return &stTraderMdDataSvrMethods;
}

int trader_md_data_svr_init_param(trader_md_data_svr* self, int argc, char* argv[])
{
  self->nMdSize = 0;
  self->nMdHeadPos = 0;
  self->nMdTailPos = 0;
  self->nContractSize = 0;
  self->nHttpPort = 0;
  self->nInterval = 0;

  self->pShmKeyFile = NULL;
  self->pServiceName = NULL;
  self->pDataCache = NULL;
  //TODO

}

int trader_md_data_svr_run(trader_md_data_svr* self)
{
  int nRet = 0; 
  trader_mduser_shm_header* pShmHdr;

  // 各种初始化
  CMN_DEBUG("self->pBase\n");
  self->pBase = event_base_new();
  CMN_ASSERT(self->pBase);

  CMN_DEBUG("self->pHttp\n");
  self->pHttp = evhttp_new(self->base);
  CMN_ASSERT(self->pHttp);

  //signal event init
  CMN_DEBUG("self->pSigIntEvt\n");
  self->pSigIntEvt = evsignal_new(self->pBase, SIGINT, trader_md_data_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigIntEvt);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  self->pSigTermEvt = evsignal_new(self->pBase, SIGTERM, trader_md_data_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigTermEvt);
  
  CMN_DEBUG("self->pEvTimer\n");
  self->pEvTimer = evtimer_new(self->base, trader_md_data_svr_timer_cb, (void *)self);
  CMN_ASSERT(self->pEvTimer);
  
  trader_mduser_shm_key_file(self->pShmKeyFile);

  CMN_DEBUG("self->pShmHdr\n");
  pShmHdr = trader_mduser_shm_header_at();
  self->pShmHdr = (void*)pShmHdr;
  CMN_ASSERT(self->pShmHdr);

  self->pDataCache = (char*)calloc(self->nMdSize, pShmHdr->nFieldSize * pShmHdr->nFieldSize);

  // 绑定http 端口
  nRet = evhttp_bind_socket(self->pHttp, "0.0.0.0", self->nHttpPort);
  CMN_ASSERT(!nRet);

  // 绑定http  接口
  nRet = evhttp_set_cb(self->pHttp, self->pServiceName, trader_md_data_svr_http_cb, (void*)self);
  CMN_ASSERT(!nRet);

  // signal 事件
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(!nRet);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(!nRet);

  // 启动timer
  struct timeval to = {
    self->nInterval / 1000,
    (self->nInterval % 1000) * 1000
  };
  nRet = evtimer_add(self->pEvTimer, &to);
  CMN_ASSERT(!nRet);

  nRet = event_base_dispatch(self->pBase);
  CMN_ASSERT(!nRet);

  return 0;
}

int trader_md_data_svr_on_tick_timer(trader_md_data_svr* self)
{
  // 移动头尾指针
  self->nMdTailPos = (self->nMdTailPos + 1) % self->nMdSize;

  if(self->nMdTailPos <= self->nMdHeadPos){
    self->nMdHeadPos = (self->nMdHeadPos + 1) % self->nMdSize;
  }

  // 更新当前页
  trader_md_data_svr_tick_update(self);

  // 启动timer
  struct timeval to = {
    self->nInterval / 1000,
    (self->nInterval % 1000) * 1000
  };
  evtimer_add(self->pEvTimer, &to);
  return 0;
}

int trader_md_data_svr_req_tick_data(trader_md_data_svr* self, const char* t1, const char* t2, 
  double m1, double m2, double tp, double* bid, double* ask)
{
  int nT1Idx = 0;
  int nT2Idx = 0;
  int nFrame = 0;
  int nSize = 0;
  int nEnd = self->nMdSize;
  int i = 0;
  
  // 获取队列长度
  if(self->nMdHeadPos == self->nMdTailPos){
    return 0;
  }
  
  // 获取t1 t2索引
  // 获取第1帧数据，定位t1 t2
  nT1Idx = trader_md_data_svr_tick_index(self, t1);
  nT2Idx = trader_md_data_svr_tick_index(self, t2);

  if(self->nMdHeadPos < self->nMdTailPos){
    nEnd = self->nMdTailPos + 1;
    nSize = nEnd - self->nMdHeadPos;
  }else{
    nSize = (self->nMdSize - self->nMdHeadPos) + (self->nMdTailPos + 1);
  }

  do{
    for(i = self->nMdHeadPos; i < nEnd; i++){
      trader_md_data_svr_get_tick_diff(self, i, nT1Idx, nT2Idx, m1, m2, tp, &bid[nFrame], &ask[nFrame]);
      nFrame++;
    }

    if(self->nMdHeadPos < self->nMdTailPos){
      break;
    }
    
    nEnd = self->nMdTailPos + 1;
    
    for(i = 0; i < nEnd; i++){
      trader_md_data_svr_get_tick_diff(self, i, nT1Idx, nT2Idx, m1, m2, tp, &bid[nFrame], &ask[nFrame]);
      nFrame++;
    }
    
  }while(0);

  return nSize;
}

void trader_md_data_svr_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_svr* self = (trader_mduser_svr*)arg;
  struct event_base *base = self->pBase;
  struct timeval delay = { 1, 0 };

  event_del(self->pSigIntEvt);
  event_del(self->pSigTermEvt);
  event_del(self->pEvTimer);

  event_base_loopexit(base, &delay);
  return;
}

void trader_md_data_svr_http_cb(struct evhttp_request *req, void *arg)
{
  trader_md_data_svr* self = (trader_md_data_svr*)arg;
  //TODO

}

void trader_md_data_svr_timer_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_md_data_svr* self = (trader_md_data_svr*)arg;
  self->pMethod->xOnTickTimer(self);
  return;
}

void trader_md_data_svr_tick_update(trader_md_data_svr* self)
{
  trader_mduser_shm_header* pShmHdr = (trader_mduser_shm_header*)self->pShmHdr;

  int nFrameSize = pShmHdr->nFieldNum * pShmHdr->nFieldSize;

  void* pData = (void*)&self->pDataCache[self->nMdTailPos * nFrameSize];

  memcpy(pData, pShmHdr->pData, nFrameSize);

  return ;
}

int trader_md_data_svr_tick_index(trader_md_data_svr* self, const char* instrument)
{
  trader_tick* pData = (trader_tick*)&self->pDataCache[0];
  trader_mduser_shm_header* pShmHeader = (trader_mduser_shm_header*)self->pShmHdr;
  
  int h = pShmHeader->nFieldNum - 1;
  int l = 0;
  int m = pShmHeader->nFieldNum / 2;
  int c = 0;

  trader_tick* p;

  while (l <= h) {
    m = (l + h) / 2;
    p = &pData[m];
    c = strncmp(p->InstrumentID, instrument, sizeof(p->InstrumentID));
    if(0 == c){
      return (void*)p;
    }else if(c > 0) {
      h = m - 1;
    }else{
      l = m + 1;
    }
  }
  return -1;
}

int trader_md_data_svr_get_tick_diff(trader_md_data_svr* self, int frame, int t1, int t2,
  double w1, double w2, double tick_price, double* bid, double* ask)
{
  //TODO

}

trader_tick* trader_md_data_svr_get_tick(trader_md_data_svr* self, int frame, int idx)
{
  trader_mduser_shm_header* pShmHeader = (trader_mduser_shm_header*)self->pShmHdr;
  trader_tick* pData = (trader_tick*)&self->pDataCache[frame * pShmHeader->nFieldSize * pShmHeader->nFieldNum];

  return &pData[idx];
}

double trader_md_data_svr_bid_diff(trader_tick* t1, trader_tick* t2, double w1, double w2, double tick_price)
{
  //TODO
}

double trader_md_data_svr_ask_diff(trader_tick* t1, trader_tick* t2, double w1, double w2, double tick_price)
{
  //TODO

}

trader_md_data_svr* trader_md_data_svr_new()
{
  trader_md_data_svr* self = (trader_md_data_svr*)malloc(sizeof(trader_md_data_svr));
  self->pMethod = trader_md_data_svr_method_get();
  return self;
}

void trader_md_data_svr_free(trader_md_data_svr* self)
{
  if(!self){
    return ;
  }

  if(self->pHttp){
    evhttp_free(self->pHttp);
  }

  if(self->pSigIntEvt){
    event_free(self->pSigIntEvt);
  }

  if(self->pSigTermEvt){
    event_free(self->pSigTermEvt);
  }

  if(self->pEvTimer){
    event_free(self->pEvTimer);
  }

  if(self->pBase) {
    event_base_free(self->pBase);
  }


  if(self->pShmHdr){
    trader_mduser_shm_header_dt(self->pShmHdr);
  }

  free(self);

  return ;
}

int main(int argc, char* argv[])
{
  //TODO

}

