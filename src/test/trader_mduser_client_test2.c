
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <float.h>

#include <sys/time.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "glbProfile.h"

#include "trader_mduser_client.h"

#include "stg_mysql_sys.h"
#include "stg_strategy_struct.h"

typedef struct trader_mduser_client_test_api_def trader_mduser_client_test_api;
struct trader_mduser_client_test_api_def{
  void* test;
  trader_mduser_client* pApi;
  char cache[sizeof(trader_mduser_evt)];
  int cacheLen;
};

typedef struct trader_mduser_client_test_def trader_mduser_client_test;
struct trader_mduser_client_test_def {

  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  trader_mduser_client_test_api oTestApi;
  int commitCount;
  int linePos;
  trader_tick* tickBuffer;

  char csvFile[256];
};


static void trader_mduser_client_test_connect_callback(void* user_data);
static void trader_mduser_client_test_disconnect_callback(void* user_data);
static void trader_mduser_client_test_recv_callback(void* user_data, void* data, int len);

static void trader_mduser_client_test_on_tick(trader_mduser_client_test* self, trader_tick* tick);

static void trader_mduser_client_test_flush(trader_mduser_client_test* self);

static void client_test_signal_cb(evutil_socket_t fd, short event, void *arg);

void client_test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_client_test* test = (trader_mduser_client_test*)arg;
  struct timeval tv = {
    2, 0
  };
  event_base_loopexit(test->pBase, &tv);
}

void trader_mduser_client_test_connect_callback(void* user_data)
{
  printf("connected!\n");
}

void trader_mduser_client_test_disconnect_callback(void* user_data)
{
  printf("disconnected!\n");
  sleep(1);
  trader_mduser_client_test_api* pTestApi = (trader_mduser_client_test_api*)user_data;
  trader_mduser_client* pApi = pTestApi->pApi;
  pApi->method->xConnect(pApi);
}

void trader_mduser_client_test_recv_callback(void* user_data, void* data, int len)
{
  trader_mduser_client_test_api* pTestApi = (trader_mduser_client_test_api*)user_data;
  trader_mduser_client_test* test = (trader_mduser_client_test*)pTestApi->test;
  
  int bDone = 0;
  int nLen;
  int nPos = 0;
  int nPos2 = 0;
  int nRest = len;
  char pBuff[sizeof(trader_mduser_evt)];
  trader_mduser_evt* pEvt = (trader_mduser_evt*)pBuff;
  trader_tick* tick_data = &pEvt->Tick;
  char* pData = (char*)data;
  
  if((pTestApi->cacheLen + len) < sizeof(trader_mduser_evt)){
    memcpy(&pTestApi->cache[pTestApi->cacheLen], pData, len);
    pTestApi->cacheLen += len;
    return ;
  }

  if(pTestApi->cacheLen > 0){
    nPos = sizeof(trader_mduser_evt) - pTestApi->cacheLen;
    memcpy(pBuff, pTestApi->cache, pTestApi->cacheLen);
    memcpy(&pBuff[pTestApi->cacheLen], pData, nPos);
    pTestApi->cacheLen = 0;
    
    pEvt = (trader_mduser_evt*)pBuff;
    tick_data = &pEvt->Tick;
    trader_mduser_client_test_on_tick(test, tick_data);  
  }

  while(nPos < len){
    if((nPos + sizeof(trader_mduser_evt)) > len){
      pTestApi->cacheLen = len - nPos;
      memcpy(&pTestApi->cache[0], &pData[nPos], pTestApi->cacheLen);
      break;
    }

    pEvt = (trader_mduser_evt*)&pData[nPos];
    tick_data = &pEvt->Tick;
    trader_mduser_client_test_on_tick(test, tick_data);

    nPos += sizeof(trader_mduser_evt);
  }
  return ;
}

void trader_mduser_client_test_on_tick(trader_mduser_client_test* self, trader_tick* tick)
{
  trader_tick* buffer = &self->tickBuffer[self->linePos];
  memcpy(buffer, tick, sizeof(trader_tick));
  self->linePos++;

  if(self->linePos == self->commitCount){
    trader_mduser_client_test_flush(self);
  }
}


void trader_mduser_client_test_flush(trader_mduser_client_test* self)
{
  int len;
  if(!self->linePos){
    return ;
  }

  int sqlCode = 0;
  int nRet;
  strategy_market_data_tick_t dbTick;

  trader_tick* tick;
  int i = 0;
  int pos = 0;
  for(i = 0; i < self->linePos; i++){
    tick = &self->tickBuffer[i];
    if((DBL_MAX == tick->BidPrice1)
    ||(DBL_MAX == tick->AskPrice1)){
      continue;
    }
    strncpy(dbTick.instrumentid, tick->InstrumentID, sizeof(dbTick.instrumentid)); 
    strncpy(dbTick.exchangeid, "", sizeof(dbTick.instrumentid)); 
    strncpy(dbTick.tradingday, tick->TradingDay, sizeof(dbTick.instrumentid)); 
    strncpy(dbTick.updatetime, tick->UpdateTime, sizeof(dbTick.instrumentid)); 
    dbTick.updatemillisec = tick->UpdateMillisec;
    dbTick.bidprice1 = tick->BidPrice1;
    dbTick.askprice1 = tick->AskPrice1;
    dbTick.bidvolume1 = tick->BidVolume1;
    dbTick.askvolume1 = tick->AskVolume1;
    nRet = strategy_market_data_tick_insert1(&dbTick, &sqlCode);
    
    if(nRet < 0){
      printf("strategy_market_data_tick_insert1 failed!nRet=[%d]sqlCode=[%d]\n", nRet, sqlCode);
      stg_mysql_rollback(&sqlCode);
      break;
    }
  }
  self->linePos = 0;
  stg_mysql_commit(&sqlCode);

  return ;

}



int main(int argc, char* argv[])
{
  trader_mduser_client_test trader_mduser_client_test_st;
  trader_mduser_client_test* test = &trader_mduser_client_test_st;

  char* pCfgFile = argv[1];
  char boardcastAddr[14+1];
  int boardcastPort;
  char boardcastAddr2[14+1];
  int boardcastPort2;
  char filenamePreifx[200+1];
  int bUsed = 0;
  // 获取连库参数
  char* dbName;
  char* dbUser;
  char* dbPwd;
  char* dbHost;
  int dbPort;
  int sqlCode = 0;
  int nRet;
  char tmp[512];
  
  nRet = glbPflGetString("MDUSER", "DB_NAME", pCfgFile, tmp);
  if(nRet < 0){
    return -1;
  }
  dbName = strdup(tmp);
  
  nRet = glbPflGetString("MDUSER", "DB_USER", pCfgFile, tmp);
  if(nRet < 0){
    return -1;
  }
  dbUser = strdup(tmp);
  
  nRet = glbPflGetString("MDUSER", "DB_PWD", pCfgFile, tmp);
  if(nRet < 0){
    return -1;
  }
  dbPwd = strdup(tmp);
  
  nRet = glbPflGetString("MDUSER", "DB_HOST", pCfgFile, tmp);
  if(nRet < 0){
    return -1;
  }
  dbHost = strdup(tmp);
  
  nRet = glbPflGetString("MDUSER", "DB_PORT", pCfgFile, tmp);
  if(nRet < 0){
    return -1;
  }
  dbPort = atoi(tmp);
  
  nRet = glbPflGetString("MDUSER", "BOARDCAST_ADDR", pCfgFile, boardcastAddr);
  if(nRet < 0){
    return -1;
  }
  
  nRet = glbPflGetInt("MDUSER", "BOARDCAST_PORT", pCfgFile, &boardcastPort);
  if(nRet < 0){
    return -1;
  }
  
  nRet = glbPflGetInt("MDUSER", "COMMIT_COUNT", pCfgFile, &test->commitCount);
  if(nRet < 0){
    return -1;
  }
  
  nRet = glbPflGetString("MDUSER", "CSV_FILENAME", pCfgFile, filenamePreifx);
  if(nRet < 0){
    return -1;
  }
  
  nRet = stg_mysql_connect(dbName, dbUser, dbPwd, dbHost, dbPort, &sqlCode);
  if(nRet < 0){
    printf("stg_mysql_connect failed!nRet=[%d]sqlCode=[%d]\n", nRet, sqlCode);
    return -1;
  }

  test->pBase = event_base_new();

  test->pSigIntEvt = evsignal_new(test->pBase, SIGINT, client_test_signal_cb, (void *)test);
  test->pSigTermEvt = evsignal_new(test->pBase, SIGTERM, client_test_signal_cb, (void *)test);
  
  test->tickBuffer = (trader_tick*)malloc((test->commitCount + 1) * sizeof(trader_tick));
  test->linePos = 0;

  trader_mduser_client_test_api* pTestApi;

  // 1 
  pTestApi = &test->oTestApi;
  pTestApi->pApi = trader_mduser_client_new();
  pTestApi->cacheLen = 0;
  pTestApi->test = test;

  pTestApi->pApi->method->xInit(pTestApi->pApi, test->pBase, boardcastAddr, boardcastPort,
    trader_mduser_client_test_connect_callback,
    trader_mduser_client_test_disconnect_callback,
    trader_mduser_client_test_recv_callback,
    pTestApi
    );

  pTestApi = &test->oTestApi;
  pTestApi->pApi->method->xConnect(pTestApi->pApi);
    
  nRet = event_add(test->pSigIntEvt, NULL);
  
  nRet = event_add(test->pSigTermEvt, NULL);

  event_base_dispatch(test->pBase);

  pTestApi = &test->oTestApi;
  pTestApi->pApi->method->xExit(pTestApi->pApi);
  trader_mduser_client_free(pTestApi->pApi);

  event_free(test->pSigTermEvt);
  event_free(test->pSigIntEvt);

  trader_mduser_client_test_flush(test);
  
  free((void*)test->tickBuffer);

  event_base_free(test->pBase);
  
  // 断开数据库
  stg_mysql_disconnect(&sqlCode);

  free(dbName);
  free(dbUser);
  free(dbPwd);
  free(dbHost);

  return 0;
}


