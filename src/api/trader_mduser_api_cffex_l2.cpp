

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GFXELE_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "efvi-common.h"

typedef struct __attribute__((__packed__)) 
{
    char          ETH_IP_UDP[42];
    char          PackageHeader[20];
    char          Tag2439[4];
    char          InstrumentID[31];
    char          UpdateTime[9];
    char          UpdateMillisec[4];
    char          TAG2432[4];
    char          OpenPrice[8];
    char          HighestPrice[8];
    char          LowestPrice[8];
    char          ClosePrice[8];
    char          UpperLimitPrice[8];
    char          LowerLimitPrice[8];
    char          SettlementPrice[8];
    char          CurrDelta[8];
    char          Tag2433[4];
    char          Val2433[0x1c];
    char          Tag2434[4];
    char          Val2434[0x18];
    char          Tag2435[4];
    char          Val2435[0x18];
    char          Tag2436[4];
    char          Val2436[0x18];
    char          Tag2437[4];
    char          Val2437[0x18];
    char          Tag2438[4];
    char          Val2438[0x18];
}cffex_l2_t;

typedef struct __attribute__((__packed__)) 
{
  char          BidPrice1[8];
  char          BidVolume1[4];
  char          AskPrice1[8];
  char          AskVolume1[4];
}cffex_l2_data_t;



extern int init_connector(const char *interface,const char *filter);
extern void receive_frame(recv_buf_t *buf);


extern trader_mduser_api_method* trader_mduser_api_cffex_l2_method_get();

static void trader_mduser_api_cffex_l2_start(trader_mduser_api* self);
static void trader_mduser_api_cffex_l2_stop(trader_mduser_api* self);
static void trader_mduser_api_cffex_l2_login(trader_mduser_api* self);
static void trader_mduser_api_cffex_l2_logout(trader_mduser_api* self);
static void trader_mduser_api_cffex_l2_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_cffex_l2_thread(void* arg);

static void cffex_l2_mduser_on_rtn_depth_market_data(void* arg, cffex_l2_t* pMarketData);

static int trader_mduser_api_cffex_l2_dump(void* arg);

static int conv_int(const char* value);
static double conv_double(const char* value);
static int unmask(char* value, int len);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE						65535

typedef struct trader_mduser_api_cffex_l2_def trader_mduser_api_cffex_l2;
struct trader_mduser_api_cffex_l2_def{
	char interface[64];			///< 组播IP
	char filter[64];				///< 本地IP
	pthread_t thread_id;
  int loop_flag;
  char store_file[64];
  int store_cnt;
  cffex_l2_t store[1024];
};


trader_mduser_api_method* trader_mduser_api_cffex_l2_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_cffex_l2_start,
    trader_mduser_api_cffex_l2_stop,
    trader_mduser_api_cffex_l2_login,
    trader_mduser_api_cffex_l2_logout,
    trader_mduser_api_cffex_l2_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_cffex_l2_start(trader_mduser_api* self)
{
  trader_mduser_api_cffex_l2* pImp = (trader_mduser_api_cffex_l2*)malloc(sizeof(trader_mduser_api_cffex_l2));
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

  }while(0);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_cffex_l2_thread, (void*)self);

  return ;
}

void trader_mduser_api_cffex_l2_stop(trader_mduser_api* self)
{
  trader_mduser_api_cffex_l2* pImp = (trader_mduser_api_cffex_l2*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_cffex_l2_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_cffex_l2_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_cffex_l2_subscribe(trader_mduser_api* self, char* instrument)
{
  return ;
}

void cffex_l2_mduser_on_rtn_depth_market_data(void* arg, cffex_l2_t *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_cffex_l2* pImp = (trader_mduser_api_cffex_l2*)self->pUserApi;

  if(0 == memcmp(pMarketData->InstrumentID, "IO", 2)){
    return ;
  }
  
  // 记录数据
  memcpy(&pImp->store[pImp->store_cnt++], pMarketData, sizeof(cffex_l2_t));
  if(pImp->store_cnt == (sizeof(pImp->store)/sizeof(cffex_l2_t))){
    GFXELE_LOG("store buffer is full\n");
    trader_mduser_api_cffex_l2_dump(arg);
  }

  //unmask(pMarketData->Val2434, 0x18);
  cffex_l2_data_t* price = (cffex_l2_data_t*)pMarketData->Val2434;
  
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  struct timeval tm;
  gettimeofday(&tm, NULL);

  strncpy(oTick.InstrumentID, (char*)pMarketData->InstrumentID, sizeof(oTick.InstrumentID));
  snprintf(oTick.TradingDay, sizeof(oTick.TradingDay), "%02d%06d", tm.tv_sec % 60, tm.tv_usec);
  strncpy(oTick.UpdateTime, (char*)pMarketData->UpdateTime, sizeof(oTick.UpdateTime));
  oTick.UpdateMillisec = conv_int(pMarketData->UpdateMillisec);
  oTick.BidPrice1 = conv_double(price->BidPrice1);
  oTick.BidVolume1 = conv_int(price->BidVolume1);
  oTick.AskPrice1 = conv_double(price->AskPrice1);
  oTick.AskVolume1 = conv_int(price->AskVolume1);
  oTick.UpperLimitPrice = conv_double(pMarketData->UpperLimitPrice);
  oTick.LowerLimitPrice = conv_double(pMarketData->LowerLimitPrice);

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}


void* trader_mduser_api_cffex_l2_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_cffex_l2* pImp = (trader_mduser_api_cffex_l2*)self->pUserApi;
  recv_buf_t msg_buf;
  cffex_l2_t *md;
  int i = 0;
  struct timeval tm;

  do{
    GFXELE_LOG("interface[%s]filter[%s]\n",pImp->interface, pImp->filter);
    /* set filter to 'sniff:promisc to receive all pkts.'
     * root permission is needed.*/
    if (init_connector(pImp->interface, pImp->filter) != 0 )
    {
      break;
    }

    do{
      gettimeofday(&tm, NULL);
      snprintf(pImp->store_file, sizeof(pImp->store_file), "dump%ld", tm.tv_sec / (24 * 60 * 60));
      pImp->store_cnt = 0;
      GFXELE_LOG("dump file[%s]\n", pImp->store_file);
    }while(0);


    while(pImp->loop_flag)
    {
      receive_frame(&msg_buf);
      if(!msg_buf.count){
        trader_mduser_api_cffex_l2_dump(arg);
        usleep(1000);
        continue;
      }
      for (i = 0; i < msg_buf.count; i++)
      {
        //GFXELE_LOG("received %ld bytes data\n", msg_buf.iov[i].iov_len);
        if (msg_buf.iov[i].iov_len == sizeof(cffex_l2_t))
        {
          md = (cffex_l2_t *)msg_buf.iov[i].iov_base;
          cffex_l2_mduser_on_rtn_depth_market_data(arg, md);
        }
      }
    }
  }while(0);

  return (void*)NULL;
}

int trader_mduser_api_cffex_l2_dump(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_cffex_l2* pImp = (trader_mduser_api_cffex_l2*)self->pUserApi;
  if(!pImp->store_cnt){
    return 0;
  }
  do{
    FILE* fp = fopen(pImp->store_file, "a");
    if(!fp){
      break;
    }

    fwrite(pImp->store, sizeof(cffex_l2_t), pImp->store_cnt, fp);
    
    fclose(fp);

    pImp->store_cnt = 0;
  }while(0);
  return 0;
}


int conv_int(const char* value)
{
  int data = 0;
  data = (*value++ & 0xFF) << 24;
  data |= (*value++ & 0xFF) << 16;
  data |= (*value++ & 0xFF) << 8;
  data |= (*value++ & 0xFF);
  return data;
}

double conv_double(const char* value)
{
  double data;

  char* p = (char*)&data;
  int i = 0;
  for(i = 0; i < sizeof(data); i++){
          p[i] = value[sizeof(data) - 1 - i];
  }
  return data;
}

int unmask(char* value, int len)
{
  int i = 0;
  int mask = len + 7;
  while(i < len){
    value[i] ^= (mask && 0xFF);
    i += 2;
    mask -= 2;
  }
  return 0;
}




