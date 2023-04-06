

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

#include "efvi_receive_depend.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "trader_tick_dict.h"

extern trader_mduser_api_method* trader_mduser_api_ef_vi_method_get();

static void trader_mduser_api_ef_vi_start(trader_mduser_api* self);
static void trader_mduser_api_ef_vi_stop(trader_mduser_api* self);
static void trader_mduser_api_ef_vi_login(trader_mduser_api* self);
static void trader_mduser_api_ef_vi_logout(trader_mduser_api* self);
static void trader_mduser_api_ef_vi_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_ef_vi_thread(void* arg);

static int trader_mduser_api_ef_vi_read(void* arg, const char* data, int size);

#ifdef __cplusplus
}
#endif

typedef struct trader_mduser_api_ef_vi_def trader_mduser_api_ef_vi;
struct trader_mduser_api_ef_vi_def{
  struct resources* m_res;
  int m_res_count;
  int loop_flag;
	pthread_t thread_id;
  trader_tick_dict* tick_dict;
};

trader_mduser_api_method* trader_mduser_api_ef_vi_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_ef_vi_start,
    trader_mduser_api_ef_vi_stop,
    trader_mduser_api_ef_vi_login,
    trader_mduser_api_ef_vi_logout,
    trader_mduser_api_ef_vi_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_ef_vi_start(trader_mduser_api* self)
{
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)malloc(sizeof(trader_mduser_api_ef_vi));
  self->pUserApi = (void*)pImp;
  int ret;
  
  char* pSavePtr;
  char sAddress[256];
  char* pTemp;
  char sEthName[32];
  char sIp[16];
  char sPort1[8];
  char sPort2[8];

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pTemp = strtok_r(sAddress, "|", &pSavePtr);
    //CMN_ASSERT (pTemp);
    strncpy(sEthName, pTemp, sizeof(sEthName));
        
    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(sIp, pTemp, sizeof(sIp));

    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(sPort1, pTemp, sizeof(sPort1));

    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(sPort2, pTemp, sizeof(sPort2));

    pImp->loop_flag = 1;
    pImp->tick_dict = trader_tick_dict_new();

    pImp->m_res_count = 2;
    pImp->m_res = (struct resources*)calloc(2, sizeof(struct resources));
    if (!pImp->m_res)
    {
      exit(1);
    }
    
    init_resources(&pImp->m_res[0], sEthName, sIp, (unsigned short)atoi(sPort1));
    pImp->m_res[0].read = trader_mduser_api_ef_vi_read;
    pImp->m_res[0].arg = (void*)self;
    
    init_resources(&pImp->m_res[1], sEthName, sIp, (unsigned short)atoi(sPort2));
    pImp->m_res[1].read = trader_mduser_api_ef_vi_read;
    pImp->m_res[1].arg = (void*)self;

    trader_mduser_api_on_rsp_user_login(self, 0, "OK");
  }while(0);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_ef_vi_thread, (void*)self);

  return ;
}

void trader_mduser_api_ef_vi_stop(trader_mduser_api* self)
{
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }

  trader_tick_dict_free(pImp->tick_dict);

  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_ef_vi_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_ef_vi_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_ef_vi_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;
  trader_tick_dict* tickDict = pImp->tick_dict;

  tickDict->pMethod->xAdd(tickDict, instrument, (void*)NULL);
  
  return ;
}

void* trader_mduser_api_ef_vi_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;
  int i = 0;

  /* set filter to 'sniff:promisc to receive all pkts.'
   * root permission is needed.*/
  int ev = 0;
  while(pImp->loop_flag)
  {
    ev = 0;
    for(i = 0; i < pImp->m_res_count; i++){
      ev += poll_resources(&pImp->m_res[i]);
    }
    if(!ev){
      usleep(1000);
    }
  }

  return (void*)NULL;
}

// TODO
#pragma pack(push, 1)
struct dzqh_fut_md
{
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
};

#pragma pack(pop)


int trader_mduser_api_ef_vi_read(void* arg, const char* data, int size)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;
  trader_tick_dict* tickDict = pImp->tick_dict;
  void* save_ptr;

  if(sizeof(struct dzqh_fut_md) > size){
    return size;
  }

  struct dzqh_fut_md* pMarketData = (struct dzqh_fut_md*)data;

  int found = tickDict->pMethod->xFind(tickDict, pMarketData->InstrumentID, &save_ptr);

  if(!found){
    return size;
  }
  
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

  return sizeof(struct dzqh_fut_md);
}

