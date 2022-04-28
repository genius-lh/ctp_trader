

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <limits.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GFXELE_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "efvi-common.h"
#include "cffex_md.h"

extern int init_connector(const char *interface,const char *filter);
extern void receive_frame(recv_buf_t *buf);


extern trader_mduser_api_method* trader_mduser_api_sf_method_get();

static void trader_mduser_api_sf_start(trader_mduser_api* self);
static void trader_mduser_api_sf_stop(trader_mduser_api* self);
static void trader_mduser_api_sf_login(trader_mduser_api* self);
static void trader_mduser_api_sf_logout(trader_mduser_api* self);
static void trader_mduser_api_sf_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_sf_thread(void* arg);

static void sf_mduser_on_rtn_depth_market_data(void* arg, cffex_md_t *pMarketData);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE						65535

typedef struct trader_mduser_api_sf_def trader_mduser_api_sf;
struct trader_mduser_api_sf_def{
	char interface[64];			///< 组播IP
	char filter[64];				///< 本地IP
	pthread_t thread_id;
  int loop_flag;
  
  int commitCount;
  int linePos;
  cffex_md_t* tickBuffer;
  char csvFile[256];
  char tradingDay[9];
};

static void trader_mduser_api_sf_record_init(trader_mduser_api_sf* self);
static void trader_mduser_api_sf_record_on_tick(trader_mduser_api_sf* self, cffex_md_t *pMarketData);
static void trader_mduser_api_sf_record_flush(trader_mduser_api_sf* self);
static void trader_mduser_api_sf_record_free(trader_mduser_api_sf* self);

trader_mduser_api_method* trader_mduser_api_sf_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_sf_start,
    trader_mduser_api_sf_stop,
    trader_mduser_api_sf_login,
    trader_mduser_api_sf_logout,
    trader_mduser_api_sf_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_sf_start(trader_mduser_api* self)
{
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)malloc(sizeof(trader_mduser_api_sf));
  self->pUserApi = (void*)pImp;
  int ret;
  
  char* pSavePtr;
  char sAddress[256];
  char* pQueryFrontAddress;
  char* pTcpFrontAddress;

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pQueryFrontAddress = strtok_r(sAddress, "|", &pSavePtr);
    //CMN_ASSERT (pQueryFrontAddress);
        
    pTcpFrontAddress = strtok_r(NULL, "|", &pSavePtr);
    //CMN_ASSERT (pTcpFrontAddress);
    strncpy(pImp->interface, pQueryFrontAddress, sizeof(pImp->interface));
    strncpy(pImp->filter, pTcpFrontAddress, sizeof(pImp->filter));

    pImp->loop_flag = 1;
    
    trader_mduser_api_sf_record_init(pImp);

  }while(0);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_sf_thread, (void*)self);

  return ;
}

void trader_mduser_api_sf_stop(trader_mduser_api* self)
{
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }

  trader_mduser_api_sf_record_free(pImp);

  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_sf_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_sf_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_sf_subscribe(trader_mduser_api* self, char* instrument)
{
  return ;
}

void sf_mduser_on_rtn_depth_market_data(void* arg, cffex_md_t *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)self->pUserApi;

  if(0 == memcmp(pMarketData->InstrumentID, "IO", 2)){
    return ;
  }

  
  trader_mduser_api_sf_record_on_tick(pImp, pMarketData);
  
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  struct timeval tm;
  
  gettimeofday(&tm, NULL);

  strcpy(oTick.InstrumentID, (char*)pMarketData->InstrumentID);
  snprintf(oTick.TradingDay, sizeof(oTick.TradingDay), "%02d%06d", tm.tv_sec % 60, tm.tv_usec);
  strcpy(oTick.UpdateTime, (char*)pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1 = pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1 = pMarketData->AskVolume1;
  oTick.UpperLimitPrice = pMarketData->UpperLimitPrice;
  oTick.LowerLimitPrice = pMarketData->LowerLimitPrice;
  oTick.LastPrice = pMarketData->LastPrice;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}


void* trader_mduser_api_sf_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)self->pUserApi;
  recv_buf_t msg_buf;
  cffex_md_t *md;
  int i = 0;

  do{
    GFXELE_LOG("interface[%s]filter[%s]\n",pImp->interface, pImp->filter);
    /* set filter to 'sniff:promisc to receive all pkts.'
     * root permission is needed.*/
    if (init_connector(pImp->interface, pImp->filter) != 0 )
    {
      break;
    }


    while(pImp->loop_flag)
    {
      receive_frame(&msg_buf);
      if(!msg_buf.count){
        usleep(1000);
        continue;
      }
      for (i = 0; i < msg_buf.count; i++)
      {
        //GFXELE_LOG("received %ld bytes data\n", msg_buf.iov[i].iov_len);
        if (msg_buf.iov[i].iov_len == sizeof(cffex_md_t))
        {
          md = (cffex_md_t *)msg_buf.iov[i].iov_base;
          sf_mduser_on_rtn_depth_market_data(arg, md);
        }
      }
    }
  }while(0);

  return (void*)NULL;
}

void trader_mduser_api_sf_record_init(trader_mduser_api_sf* self)
{
  self->commitCount = 2000;
  self->linePos = 0;
  self->tickBuffer = (cffex_md_t *)malloc(self->commitCount * sizeof(cffex_md_t));
  
  time_t tt = time(NULL);
  struct tm now;

  tt += 4 * 60 * 60;
  localtime_r(&tt, &now);

  snprintf(self->csvFile, sizeof(self->csvFile),  "mduser%04d%02d%02d.csv", 
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(self->tradingDay, sizeof(self->tradingDay),  "%04d%02d%02d", 
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  
  return;
}

void trader_mduser_api_sf_record_on_tick(trader_mduser_api_sf* self, cffex_md_t *pMarketData)
{
  cffex_md_t* buffer = &self->tickBuffer[self->linePos];
  memcpy(buffer, pMarketData, sizeof(cffex_md_t));
  self->linePos++;

  if(self->linePos == self->commitCount){
    trader_mduser_api_sf_record_flush(self);
  }
  return;
}

void trader_mduser_api_sf_record_flush(trader_mduser_api_sf* self)
{
  char line[1000];
  int len;
  if(!self->linePos){
    return ;
  }

  char* buffer = (char*)malloc(self->commitCount * 1000 * sizeof(char));
  cffex_md_t* tick;
  int i = 0;
  int pos = 0;
  for(i = 0; i < self->linePos; i++){
    tick = &self->tickBuffer[i];
    if((DBL_MAX == tick->BidPrice1)
    ||(DBL_MAX == tick->AskPrice1)){
      continue;
    }
    snprintf(line, sizeof(line), "%s,%s,%s,%d,%f,%f,%f,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d\n"
      , tick->InstrumentID
      , self->tradingDay
      , tick->UpdateTime
      , tick->UpdateMillisec
      , tick->LastPrice
      , tick->UpperLimitPrice
      , tick->LowerLimitPrice
      , tick->BidPrice1
      , tick->BidVolume1
      , tick->AskPrice1
      , tick->AskVolume1
      , tick->BidPrice2
      , tick->BidVolume2
      , tick->AskPrice2
      , tick->AskVolume2
      , tick->BidPrice3
      , tick->BidVolume3
      , tick->AskPrice3
      , tick->AskVolume3
      , tick->BidPrice4
      , tick->BidVolume4
      , tick->AskPrice4
      , tick->AskVolume4
      , tick->BidPrice5
      , tick->BidVolume5
      , tick->AskPrice5
      , tick->AskVolume5
    );

    len = strlen(line);
    memcpy(&buffer[pos], line, len);
    pos += len;
  }
  self->linePos = 0;

  do{
    FILE* fp = fopen(self->csvFile, "a");
    if(!fp){
      break;
    }

    fwrite(buffer, pos, 1, fp);
    
    fclose(fp);
  }while(0);
  
  free(buffer);

  return ;

}


void trader_mduser_api_sf_record_free(trader_mduser_api_sf* self)
{
  trader_mduser_api_sf_record_flush(self);

  if(self->tickBuffer){
    free(self->tickBuffer);
    self->tickBuffer = NULL;
  }
}



