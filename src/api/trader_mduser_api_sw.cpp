

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

#pragma pack(push, 1)

struct guava_udp_normal
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_quote_flag;			///<行情标志  0 无time sale,无lev1, 
											///           1 有time sale,无lev1, 
											///           2 无time sale,有lev1, 
											///           3 有time sale,有lev1
	char			m_symbol[8];			///<合约
	char			m_update_time[9];		///<最后更新时间(秒)
	int				m_millisecond;			///<最后更新时间(毫秒)

	double			m_last_px;				///<最新价
	int				m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量
	double			m_bid_px;				///<最新买价
	int				m_bid_share;			///<最新买量
	double			m_ask_px;				///<最新卖价
	int				m_ask_share;			///<最新卖量
};

#pragma pack(pop)


#define GFXELE_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "dict.h"

extern trader_mduser_api_method* trader_mduser_api_sw_method_get();

static void trader_mduser_api_sw_start(trader_mduser_api* self);
static void trader_mduser_api_sw_stop(trader_mduser_api* self);
static void trader_mduser_api_sw_login(trader_mduser_api* self);
static void trader_mduser_api_sw_logout(trader_mduser_api* self);
static void trader_mduser_api_sw_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_sw_thread(void* arg);

static int trader_mduser_api_sw_prase_url(const char* url, char* local_host, char* remote_host, int* port);
static void sw_x10_mduser_on_rtn_depth_market_data(void* arg, struct guava_udp_normal *pMarketData);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE (8 * 1024 * 1024)
#define MSG_BUF_SIZE (64 * 1024)

typedef struct trader_mduser_api_sw_def trader_mduser_api_sw;
struct trader_mduser_api_sw_def{
	char remote_ip[16];			///< 组播IP
	unsigned short remote_port;			///< 组播端口
	char local_ip[16];				///< 本地IP
	pthread_t thread_id;
  int loop_flag;
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

static void trader_mduser_api_sw_tick_dict_init(trader_mduser_api_sw* self)
{
  dictType* tickDictType = tickDictTypeGet();
  self->tick_dick = dictCreate(tickDictType,NULL);
  pthread_mutex_init(&self->mutex, NULL);
  return;
}

static int trader_mduser_api_sw_tick_dict_add(trader_mduser_api_sw* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = dictAdd(self->tick_dick, (void*)instrument, (void*)NULL);
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static int trader_mduser_api_sw_tick_dict_find(trader_mduser_api_sw* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = (NULL != dictFind(self->tick_dick, (void*)instrument));
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static void trader_mduser_api_sw_tick_dict_destory(trader_mduser_api_sw* self)
{
  dictRelease(self->tick_dick);
  return;
}



trader_mduser_api_method* trader_mduser_api_sw_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_sw_start,
    trader_mduser_api_sw_stop,
    trader_mduser_api_sw_login,
    trader_mduser_api_sw_logout,
    trader_mduser_api_sw_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_sw_start(trader_mduser_api* self)
{
  trader_mduser_api_sw* pImp = (trader_mduser_api_sw*)malloc(sizeof(trader_mduser_api_sw));
  self->pUserApi = (void*)pImp;
  int port;
  int ret = trader_mduser_api_sw_prase_url(self->pAddress, pImp->local_ip, pImp->remote_ip, &port);
  if(ret < 0){
    return;
  }

  pImp->remote_port = (unsigned short)port;

  trader_mduser_api_sw_tick_dict_init(pImp);
  
  trader_mduser_api_on_rsp_user_login(self, 0, "OK");

  sleep(1);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_sw_thread, (void*)self);

  return ;
}

void trader_mduser_api_sw_stop(trader_mduser_api* self)
{
  trader_mduser_api_sw* pImp = (trader_mduser_api_sw*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }

  trader_mduser_api_sw_tick_dict_destory(pImp);
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_sw_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_sw_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_sw_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_sw* pImp = (trader_mduser_api_sw*)self->pUserApi;
  trader_mduser_api_sw_tick_dict_add(pImp, instrument);
  
  return ;
}

void sw_x10_mduser_on_rtn_depth_market_data(void* arg, struct guava_udp_normal *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  trader_mduser_api_sw* pImp = (trader_mduser_api_sw*)self->pUserApi;
  int found = trader_mduser_api_sw_tick_dict_find(pImp, pMarketData->m_symbol);

  if(!found){
    return;
  }
  
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->m_symbol);
  strcpy(oTick.TradingDay, "20210101");
  strcpy(oTick.UpdateTime, pMarketData->m_update_time);
  oTick.UpdateMillisec = pMarketData->m_millisecond;
  oTick.BidPrice1 = pMarketData->m_bid_px;
  oTick.BidVolume1 = pMarketData->m_bid_share;
  oTick.AskPrice1 = pMarketData->m_ask_px;
  oTick.AskVolume1 = pMarketData->m_ask_share;
  oTick.UpperLimitPrice = pMarketData->m_bid_px * 0.9;
  oTick.LowerLimitPrice = pMarketData->m_ask_px * 1.1;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void* trader_mduser_api_sw_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_sw* pImp = (trader_mduser_api_sw*)self->pUserApi;
  int m_sock;

	char line[MSG_BUF_SIZE] = "";

	int n_rcved = -1;

	struct sockaddr_in muticast_addr;
	memset(&muticast_addr, 0, sizeof(muticast_addr));

  
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

  do{
		m_sock = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == m_sock) 
		{
			break;
		}
		
		//socket可以重新使用一个本地地址
		int flag=1;
		if(setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) != 0)
		{
			break;
		}

		int options = fcntl(m_sock, F_GETFL);
		if(options < 0)
		{
			break;
		}
		options = options | O_NONBLOCK;
		int i_ret = fcntl(m_sock, F_SETFL, options);
		if(i_ret < 0)
		{
			break;
		}

		struct sockaddr_in local_addr;
		memset(&local_addr, 0, sizeof(local_addr));
		local_addr.sin_family = AF_INET;
		local_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
		local_addr.sin_port = htons(pImp->remote_port);	//multicast port
		if (bind(m_sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0)
		{
			break;
		}

		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(pImp->remote_ip);	//multicast group ip
		mreq.imr_interface.s_addr = inet_addr(pImp->local_ip);

		if (setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) != 0)
		{
			break;
		}

		int receive_buf_size  = RCV_BUF_SIZE;	
		if (setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&receive_buf_size, sizeof(receive_buf_size)) != 0)
		{
			break;
		}

    pImp->loop_flag = 1;
    flag = 0;
    while (pImp->loop_flag)
    {
    	FD_ZERO( &readSet);
    	FD_ZERO( &writeSet);
    	FD_ZERO( &errorSet);
      
    	FD_SET(m_sock, &readSet);
    	FD_SET(m_sock, &errorSet);
      
      tval.tv_usec = 100 * 1000; //100 ms
      tval.tv_sec = 0;

      n_rcved = select(m_sock + 1, &readSet, &writeSet, &errorSet, &tval);
      
    	if(n_rcved < 1) { // timeout
        continue;
      }
      
    	socklen_t len = sizeof(sockaddr_in);
    	n_rcved = recvfrom(m_sock, line, MSG_BUF_SIZE, 0, (struct sockaddr*)&muticast_addr, &len);
    	if ( n_rcved < 0) 
    	{
    		continue;
    	} 
    	else if (0 == n_rcved)
    	{
    		continue;
    	}					
    	else
    	{
        //sw_mduser_on_rtn_depth_market_data(self, (CUstpFtdcDepthMarketDataField*)line);
        if(!flag){
          flag = 1;
          GFXELE_LOG("sw_ctp_mduser_on_rtn_depth_market_data\n");
        }
        //sw_ctp_mduser_on_rtn_depth_market_data(self, (CThostFtdcDepthMarketDataField*)line);
        if(n_rcved > sizeof(struct guava_udp_normal)){
          GFXELE_LOG("n_rcved[%d] > DATA_WITH_HEAD\n", n_rcved);
        }
        sw_x10_mduser_on_rtn_depth_market_data(self, (struct guava_udp_normal*)line);
    	}
    }
  }while(0);
  if(m_sock > 0){
    close(m_sock);
  }

  return (void*)NULL;
}

int trader_mduser_api_sw_prase_url(const char* url, char* local_host, char* remote_host, int* port)
{
  char* p;
  char* q;
  // 定位://
  p = (char*)strstr(url, "://");
  if(NULL == p){
    return -1;
  }
  // 移动3个字符
  p += 3;
  
  // 定位:
  q = (char*)strstr(p, "@");
  if(NULL == q){
    return -2;
  }

  // 获取host
  memcpy(local_host, p, q - p);
  local_host[q-p] = '\0';
  q++;
  p = q;
  
  // 定位:
  q = (char*)strstr(p, ":");
  if(NULL == q){
    return -3;
  }
  // 获取host
  memcpy(remote_host, p, q - p);
  remote_host[q-p] = '\0';

  //移动1个字符
  q++;

  // 获取port
  *port = atoi(q);
  return 0;
}



