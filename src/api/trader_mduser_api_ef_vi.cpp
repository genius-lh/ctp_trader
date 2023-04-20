

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <limits.h>
#include <float.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
static void* trader_mduser_api_ef_vi_msg_thread(void* arg);

static int trader_mduser_api_ef_vi_read(void* arg, const char* data, int size);

typedef struct ef_vi_msg_def{
  long   msg_tp;            /*消息类型*/
  union{
    char  msg_data[512];     /*消息内容*/
    char* msg_ptr;
  };
}ef_vi_msg_t;

static int ef_vi_msg_init(void* arg);
static int ef_vi_msg_send(void* arg, const char* data, int len);
static int ef_vi_msg_stop(void* arg);
static int ef_vi_msg_recv(void* arg, ef_vi_msg_t* ipc_msg);
static void ef_vi_msg_destroy(void* arg);

struct ef_vi_thread {
  int m_loop_flag;
  int m_cpu_id;
	pthread_t m_thread_id;
  struct resources m_res;
};

static int ef_vi_thread_init(struct ef_vi_thread* self, int cpu_id, 
  const char* eth_name, const char* ip,  unsigned short port,
  int (*read)(void* , const char* , int ), void* arg);
static int ef_vi_thread_start(struct ef_vi_thread* self);
static void ef_vi_thread_stop(struct ef_vi_thread* self);
static void ef_vi_thread_destroy(struct ef_vi_thread* self);


#ifdef __cplusplus
}
#endif

typedef struct trader_mduser_api_ef_vi_def trader_mduser_api_ef_vi;
struct trader_mduser_api_ef_vi_def{
  int m_msg_loop;
  int m_msg_id;
	pthread_t m_msg_thread_id;
  int m_thread_count;
  struct ef_vi_thread* m_thread;
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
  unsigned short usPort[2];
  
  int i = 0;


  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pTemp = strtok_r(sAddress, "|", &pSavePtr);
    //CMN_ASSERT (pTemp);
    strncpy(sEthName, pTemp, sizeof(sEthName));
        
    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(sIp, pTemp, sizeof(sIp));

    i = 0;
    pTemp = strtok_r(NULL, "|", &pSavePtr);
    usPort[i++] = (unsigned short)atoi(pTemp);

    pTemp = strtok_r(NULL, "|", &pSavePtr);
    usPort[i++] = (unsigned short)atoi(pTemp);

    // 初始化消息队列
    ret = ef_vi_msg_init((void*)pImp);
    if(ret < 0){
      exit(0);
    }
    
    // 启动消息队列接收线程
    ret = pthread_create(&pImp->m_msg_thread_id, NULL, trader_mduser_api_ef_vi_msg_thread, (void*)self);

    pImp->tick_dict = trader_tick_dict_new();

    pImp->m_thread_count = 2;
    pImp->m_thread = (struct ef_vi_thread*)calloc(pImp->m_thread_count, sizeof(struct ef_vi_thread));
    if (!pImp->m_thread)
    {
      exit(1);
    }

    struct ef_vi_thread* pThread;

    for(i = 0; i < pImp->m_thread_count; i++){
      pThread = &pImp->m_thread[i];
      ef_vi_thread_init(pThread, i+1, sEthName, sIp, usPort[i], trader_mduser_api_ef_vi_read, (void*)self);
    }

    trader_mduser_api_on_rsp_user_login(self, 0, "OK");

    for(i = 0; i < pImp->m_thread_count; i++){
      ef_vi_thread_start(&pImp->m_thread[i]);
    }
  }while(0);

  return ;
}

void trader_mduser_api_ef_vi_stop(trader_mduser_api* self)
{
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;
  int i;
  void* ret;
  
  for(i = 0; i < pImp->m_thread_count; i++){
    ef_vi_thread_stop(&pImp->m_thread[i]);
  }

  for(i = 0; i < pImp->m_thread_count; i++){
    ef_vi_thread_destroy(&pImp->m_thread[i]);
  }

  pImp->m_msg_loop = 0;
  ef_vi_msg_stop((void *)pImp);
  if(pImp->m_msg_thread_id){
    pthread_join(pImp->m_msg_thread_id, &ret);
  }
  ef_vi_msg_destroy((void *)pImp);

  if(pImp->m_thread){
    free(pImp->m_thread);
    pImp->m_thread = NULL;
  }

  if(pImp->tick_dict){  
    trader_tick_dict_free(pImp->tick_dict);
    pImp->tick_dict = NULL;
  }
  
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

int ef_vi_thread_init(struct ef_vi_thread* self, int cpu_id, 
  const char* eth_name, const char* ip,  unsigned short port,
  int (*read)(void* , const char* , int ), void* arg)
{
  self->m_cpu_id = cpu_id;
  init_resources(&self->m_res, eth_name, ip, port);
  self->m_res.read = read;
  self->m_res.arg = arg;

  return 0;
}

int ef_vi_thread_start(struct ef_vi_thread* self)
{
  self->m_loop_flag = 1;
	int ret = pthread_create(&self->m_thread_id, NULL, trader_mduser_api_ef_vi_thread, (void*)self);
  return ret;
}

void ef_vi_thread_stop(struct ef_vi_thread* self)
{
  self->m_loop_flag = 0;
}

void ef_vi_thread_destroy(struct ef_vi_thread* self)
{
  if(!self){
    return;
  }

  void* ret;
  if(self->m_thread_id){
    pthread_join(self->m_thread_id, &ret);
  }

  free_resources(&self->m_res);

  do{
    
    time_t tt = time(NULL);
    struct tm now;
    localtime_r(&tt, &now);
    
    FILE* fp = fopen("ef_vi_count.txt", "a");
    if(!fp){
      break;
    }

    fprintf(fp, "c%d,%04d%02d%02d,%ld,%ld,%ld\n"
      , self->m_cpu_id, now.tm_year+1900, now.tm_mon+1, now.tm_mday
      , self->m_res.discard_count + self->m_res.normal_count
      , self->m_res.discard_count
      , self->m_res.multi_count
    );
    
    fclose(fp);
  }while(0);

  return;
}


void* trader_mduser_api_ef_vi_thread(void* arg)
{
  struct ef_vi_thread* pThread = (struct ef_vi_thread*)arg;

	bind_cpu( pThread->m_cpu_id, pthread_self() );

  /* set filter to 'sniff:promisc to receive all pkts.'
   * root permission is needed.*/
  int ev = 0;
  while(pThread->m_loop_flag)
  {
    ev = poll_resources(&pThread->m_res);
    if(!ev){
      wait_resources(&pThread->m_res);
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

  if(sizeof(struct dzqh_fut_md) > size){
    //TODO
    
    return size;
  }

  struct dzqh_fut_md* pMarketData = (struct dzqh_fut_md*)data;

  void* save_ptr;
  int found = tickDict->pMethod->xFind(tickDict, pMarketData->InstrumentID, &save_ptr);
  if(!found){
    return sizeof(struct dzqh_fut_md);
  }

  int ret = ef_vi_msg_send((void *) pImp, data, sizeof(struct dzqh_fut_md));
  if(ret < 0){
    printf("ef_vi_msg_send[%d][%d]", ret, errno);
  }

  return sizeof(struct dzqh_fut_md);
  
}

int ef_vi_msg_init(void* arg)
{
  trader_mduser_api_ef_vi* self = (trader_mduser_api_ef_vi*)arg;
  self->m_msg_id = msgget(IPC_PRIVATE, IPC_CREAT|0600);
  if(-1 == self->m_msg_id){
    return -1;
  }
  return 0;
}

int ef_vi_msg_send(void* arg, const char* data, int len)
{
  trader_mduser_api_ef_vi* self = (trader_mduser_api_ef_vi*)arg;
  int ret;
  ef_vi_msg_t ipc_msg;

  int data_len = sizeof(ipc_msg.msg_data);
  if(data_len < len){
    return -1;
  }

  ipc_msg.msg_tp = 1;
  memcpy(ipc_msg.msg_data, data, len);
  ret = msgsnd(self->m_msg_id, &ipc_msg, len, IPC_NOWAIT);

  return ret;
}

int ef_vi_msg_stop(void* arg)
{
  trader_mduser_api_ef_vi* self = (trader_mduser_api_ef_vi*)arg;
  int ret;
  ef_vi_msg_t ipc_msg;

  ipc_msg.msg_tp = 2;
  ret = msgsnd(self->m_msg_id, &ipc_msg, 4, IPC_NOWAIT);

  return ret;
}


int ef_vi_msg_recv(void* arg, ef_vi_msg_t* ipc_msg)
{
  trader_mduser_api_ef_vi* self = (trader_mduser_api_ef_vi*)arg;
  int ret;
  ret = msgrcv(self->m_msg_id, ipc_msg, sizeof(ef_vi_msg_t) - sizeof(long), 0, MSG_NOERROR);
  if(ret <= 0){
    // 获取队列失败
    return -1;
  }
  
  return ret;

}
void ef_vi_msg_destroy(void* arg)
{
  trader_mduser_api_ef_vi* self = (trader_mduser_api_ef_vi*)arg;
  int ret;
  ret = msgctl(self->m_msg_id, IPC_RMID, NULL);
  if(ret < 0){
    printf("msgctl[%d][%d]", ret, errno);
  }
  return;
}

void* trader_mduser_api_ef_vi_msg_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_ef_vi* pImp = (trader_mduser_api_ef_vi*)self->pUserApi;

  ef_vi_msg_t ipc_msg;
  int ret;

  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  struct dzqh_fut_md* pMarketData = (struct dzqh_fut_md*)ipc_msg.msg_data;

  pImp->m_msg_loop = 1;

  while(pImp->m_msg_loop){
    ret = ef_vi_msg_recv((void*)pImp, &ipc_msg);
    if(2 == ipc_msg.msg_tp){
      break;
    }
    if(ret < 0){
      usleep(100);
      continue;
    }
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
    oTick.Reserved = (long)pMarketData->ChangeNo;
    
    trader_mduser_api_on_rtn_depth_market_data(self, &oTick);
  }

  return 0;
}

