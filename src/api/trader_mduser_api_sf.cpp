

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <limits.h>
#include <float.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GFXELE_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "efvi-common.h"
#include "dict.h"

#pragma pack(1)
typedef struct MarketDepthUpdate 
{
  int8_t          ETH_IP_UDP[40];
  unsigned char 	Flag;					//协议标志
  char 			TypeID;					//协议版本
  unsigned short  Length;					//包长度
  int 			PacketNo;				//全0
  unsigned int	ChangeNo;				//增量编号
  short			InstrumentNo;			//合约编码
  char			InstrumentID[10];		//合约
  unsigned int	UpdateTime;				//最后更新时间(秒)
  unsigned short	UpdateMillisec;			//最后更新时间(毫秒)
  int				Volume;
  int				OpenInterest;
  int				BidVolume1;
  int				BidVolume2;
  int				BidVolume3;
  int				BidVolume4;
  int				BidVolume5;
  int				AskVolume1;
  int				AskVolume2;
  int				AskVolume3;
  int				AskVolume4;
  int				AskVolume5;
  double			LastPrice;
  double			BidPrice1;
  double			BidPrice2;
  double			BidPrice3;
  double			BidPrice4;
  double			BidPrice5;
  double			AskPrice1;
  double			AskPrice2;
  double			AskPrice3;
  double			AskPrice4;
  double			AskPrice5;
  double			Turnover;
}cffex_md_t;
#pragma pack()

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

  pthread_mutex_t mutex;
  dict* tick_dick;
};

static unsigned int tickHash(const void *key) {
    return dictGenHashFunction((const unsigned char *)key,
                               strlen((const char*)key));
}

static void *tickKeyDup(void *privdata, const void *src) {
    ((void) privdata);
    int l1 = strlen((const char*)src)+1;
    char* dup = (char*)malloc(l1 * sizeof(char));
    strncpy(dup, (const char*)src, l1);
    return dup;
}

static int tickKeyCompare(void *privdata, const void *key1, const void *key2) {
    int l1, l2;
    ((void) privdata);

    l1 = strlen((const char*)key1);
    l2 = strlen((const char*)key2);
    if (l1 != l2) return 0;
    return memcmp(key1,key2,l1) == 0;
}

static void tickKeyDestructor(void *privdata, void *key) {
    ((void) privdata);
    free((char*)key);
}

static dictType* tickDictTypeGet() {
  static dictType tickDict = {
    tickHash,
    tickKeyDup,
    NULL,
    tickKeyCompare,
    tickKeyDestructor,
    NULL
  };
  return &tickDict;
}
static void trader_mduser_api_sf_tick_dict_init(trader_mduser_api_sf* self)
{
  dictType* tickDictType = tickDictTypeGet();
  self->tick_dick = dictCreate(tickDictType,NULL);
  pthread_mutex_init(&self->mutex, NULL);
  return;
}

static int trader_mduser_api_sf_tick_dict_add(trader_mduser_api_sf* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = dictAdd(self->tick_dick, (void*)instrument, (void*)NULL);
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static int trader_mduser_api_sf_tick_dict_find(trader_mduser_api_sf* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = (NULL != dictFind(self->tick_dick, (void*)instrument));
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static void trader_mduser_api_sf_tick_dict_destory(trader_mduser_api_sf* self)
{
  dictRelease(self->tick_dick);
  return;
}

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

    trader_mduser_api_sf_tick_dict_init(pImp);
    
    trader_mduser_api_on_rsp_user_login(self, 0, "OK");
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

  trader_mduser_api_sf_tick_dict_destory(pImp);

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
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)self->pUserApi;
  trader_mduser_api_sf_tick_dict_add(pImp, instrument);
  return ;
}

void sf_mduser_on_rtn_depth_market_data(void* arg, cffex_md_t *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_sf* pImp = (trader_mduser_api_sf*)self->pUserApi;

  int found = trader_mduser_api_sf_tick_dict_find(pImp, pMarketData->InstrumentID);

  if(!found){
    return;
  }
  
  //trader_mduser_api_sf_record_on_tick(pImp, pMarketData);
  
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  struct tm now;
  time_t current = (time_t)pMarketData->UpdateTime;
  localtime_r(&current, &now);    

  strcpy(oTick.InstrumentID, (char*)pMarketData->InstrumentID);
  snprintf(oTick.TradingDay, sizeof(oTick.TradingDay), "%04d%02d%02d", now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(oTick.UpdateTime, sizeof(oTick.UpdateTime), "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1 = pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1 = pMarketData->AskVolume1;
  oTick.UpperLimitPrice = pMarketData->AskPrice5;
  oTick.LowerLimitPrice = pMarketData->BidPrice5;
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
    snprintf(line, sizeof(line), "%s,%s,%d,%d,%f,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d\n"
      , tick->InstrumentID
      , self->tradingDay
      , tick->UpdateTime
      , tick->UpdateMillisec
      , tick->LastPrice
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



