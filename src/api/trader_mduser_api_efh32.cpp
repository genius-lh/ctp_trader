

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

struct efh3_2_fut_lev1
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_symbol[8];			///<合约
	char			m_update_time_h;		///<最后更新的时间hh
	char			m_update_time_m;		///<最后更新的时间mm
	char			m_update_time_s;		///<最后更新的时间ss
	unsigned short  m_millisecond;		    ///<最后更新的毫秒数         

	double			m_last_px;				///<最新价
	unsigned int	m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量
	double			m_bid_px;				///<买一价
	unsigned int	m_bid_share;			///<买一量
	double			m_ask_px;				///<卖一价
	unsigned int	m_ask_share;			///<卖一量
	char            m_reserve;  			///<保留字段             
};

struct efh3_2_opt_lev1
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_symbol[16];			///<合约
	char			m_update_time_h;		///<最后更新的时间hh
	char			m_update_time_m;		///<最后更新的时间mm
	char			m_update_time_s;		///<最后更新的时间ss
	unsigned short  m_millisecond;		    ///<最后更新的毫秒数        

	double			m_last_px;				///<最新价
	unsigned int	m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量
	double			m_bid_px;				///<买一价
	unsigned int	m_bid_share;			///<买一量
	double			m_ask_px;				///<卖一价
	unsigned int	m_ask_share;			///<卖一量
	char            m_reserve;  			///<保留字段             
};

struct efh3_2_fut_lev2
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_symbol[8];			///<合约
	char			m_update_time_h;		///<最后更新的时间hh
	char			m_update_time_m;		///<最后更新的时间mm
	char			m_update_time_s;		///<最后更新的时间ss
	unsigned short  m_millisecond;		    ///<最后更新的毫秒数        

	double			m_last_px;				///<最新价
	unsigned int	m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量

	double			m_bid1_px;				///<买一价
	unsigned int	m_bid1_share;			///<买一量
	double			m_bid2_px;				///<买二价
	unsigned int	m_bid2_share;			///<买二量
	double			m_bid3_px;				///<买三价
	unsigned int	m_bid3_share;			///<买三量
	double			m_bid4_px;				///<买四价
	unsigned int	m_bid4_share;			///<买四量
	double			m_bid5_px;				///<买五价
	unsigned int	m_bid5_share;			///<买五量

	double			m_ask1_px;				///<卖一价
	unsigned int	m_ask1_share;			///<卖一量
	double			m_ask2_px;				///<卖二价
	unsigned int	m_ask2_share;			///<卖二量
	double			m_ask3_px;				///<卖三价
	unsigned int	m_ask3_share;			///<卖三量
	double			m_ask4_px;				///<卖四价
	unsigned int	m_ask4_share;			///<卖四量
	double			m_ask5_px;				///<卖五价
	unsigned int	m_ask5_share;			///<卖五量

	char            m_reserve;  			///<保留字段             
};


struct efh3_2_opt_lev2
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_symbol[16];			///<合约
	char			m_update_time_h;		///<最后更新的时间hh
	char			m_update_time_m;		///<最后更新的时间mm
	char			m_update_time_s;		///<最后更新的时间ss
	unsigned short  m_millisecond;		    ///<最后更新的毫秒数        

	double			m_last_px;				///<最新价
	unsigned int	m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量

	double			m_bid1_px;				///<买一价
	unsigned int	m_bid1_share;			///<买一量
	double			m_bid2_px;				///<买二价
	unsigned int	m_bid2_share;			///<买二量
	double			m_bid3_px;				///<买三价
	unsigned int	m_bid3_share;			///<买三量
	double			m_bid4_px;				///<买四价
	unsigned int	m_bid4_share;			///<买四量
	double			m_bid5_px;				///<买五价
	unsigned int	m_bid5_share;			///<买五量

	double			m_ask1_px;				///<卖一价
	unsigned int	m_ask1_share;			///<卖一量
	double			m_ask2_px;				///<卖二价
	unsigned int	m_ask2_share;			///<卖二量
	double			m_ask3_px;				///<卖三价
	unsigned int	m_ask3_share;			///<卖三量
	double			m_ask4_px;				///<卖四价
	unsigned int	m_ask4_share;			///<卖四量
	double			m_ask5_px;				///<卖五价
	unsigned int	m_ask5_share;			///<卖五量

	char            m_reserve;  			///<保留字段             
};

#pragma pack(pop)


#define XHEFH32_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "dict.h"

extern trader_mduser_api_method* trader_mduser_api_efh32_method_get();

static void trader_mduser_api_efh32_start(trader_mduser_api* self);
static void trader_mduser_api_efh32_stop(trader_mduser_api* self);
static void trader_mduser_api_efh32_login(trader_mduser_api* self);
static void trader_mduser_api_efh32_logout(trader_mduser_api* self);
static void trader_mduser_api_efh32_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_efh32_thread(void* arg);

static int trader_mduser_api_efh32_prase_url(const char* url, char* local_host, char* remote_host, int* port, char* remote_host2, int* port2);

static void on_receive_message(void* arg, const char* buff, unsigned int len);

static void on_receive_fut_lev1(void* arg, efh3_2_fut_lev1* data);
static void on_receive_fut_lev2(void* arg, efh3_2_fut_lev2* data);
static void on_receive_opt_lev1(void* arg, efh3_2_opt_lev1* data);
static void on_receive_opt_lev2(void* arg, efh3_2_opt_lev2* data);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE (8 * 1024 * 1024)
#define MSG_BUF_SIZE (64 * 1024)

typedef struct trader_mduser_api_efh32_def trader_mduser_api_efh32;
struct trader_mduser_api_efh32_def{
  char remote_ip[16];			///< 组播IP
  unsigned short remote_port;			///< 组播端口
  char remote_ip2[16];			///< 组播IP
  unsigned short remote_port2;			///< 组播端口
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

static int udp_sock_init(int* pfd, const char* remote_ip, int remote_port, const char* local_ip);

int udp_sock_init(int* pfd, const char* remote_ip, int remote_port, const char* local_ip)
{
  int m_sock;

	m_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == m_sock) 
	{
		return -1;
	}
	
	//socket可以重新使用一个本地地址
	int flag=1;
	if(setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) != 0)
	{
		return -2;
	}

	int options = fcntl(m_sock, F_GETFL);
	if(options < 0)
	{
		return -3;
	}
  
	options = options | O_NONBLOCK;
	int i_ret = fcntl(m_sock, F_SETFL, options);
	if(i_ret < 0)
	{
		return -4;
	}

	struct sockaddr_in local_addr;
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	local_addr.sin_port = htons(remote_port);	//multicast port
	if (bind(m_sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0)
	{
		return -5;
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(remote_ip);	//multicast group ip
	mreq.imr_interface.s_addr = inet_addr(local_ip);

	if (setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) != 0)
	{
		return -6;
	}

	int receive_buf_size  = RCV_BUF_SIZE;	
	if (setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&receive_buf_size, sizeof(receive_buf_size)) != 0)
	{
		return -7;
	}

  *pfd = m_sock;
  return 0;
}

static void trader_mduser_api_efh32_tick_dict_init(trader_mduser_api_efh32* self)
{
  dictType* tickDictType = tickDictTypeGet();
  self->tick_dick = dictCreate(tickDictType,NULL);
  pthread_mutex_init(&self->mutex, NULL);
  return;
}

static int trader_mduser_api_efh32_tick_dict_add(trader_mduser_api_efh32* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = dictAdd(self->tick_dick, (void*)instrument, (void*)NULL);
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static int trader_mduser_api_efh32_tick_dict_find(trader_mduser_api_efh32* self, const char* instrument)
{
  int ret;
  pthread_mutex_lock(&self->mutex);
  ret = (NULL != dictFind(self->tick_dick, (void*)instrument));
  pthread_mutex_unlock(&self->mutex);
  return ret;
}

static void trader_mduser_api_efh32_tick_dict_destory(trader_mduser_api_efh32* self)
{
  dictRelease(self->tick_dick);
  return;
}



trader_mduser_api_method* trader_mduser_api_efh32_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_efh32_start,
    trader_mduser_api_efh32_stop,
    trader_mduser_api_efh32_login,
    trader_mduser_api_efh32_logout,
    trader_mduser_api_efh32_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_efh32_start(trader_mduser_api* self)
{
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)malloc(sizeof(trader_mduser_api_efh32));
  self->pUserApi = (void*)pImp;
  int port;
  int port2;
  int ret = trader_mduser_api_efh32_prase_url(self->pAddress, pImp->local_ip,
    pImp->remote_ip, &port, pImp->remote_ip2, &port2);
  if(ret < 0){
    return;
  }

  XHEFH32_LOG("local_ip[%s]\n"
    "remote_ip[%s]\n"
    "remote_port[%d]\n"
    "remote_ip2[%s]\n"
    "remote_port2[%d]\n"
    , pImp->local_ip
    , pImp->remote_ip
    , port
    , pImp->remote_ip2
    , port2
  );

  pImp->remote_port = (unsigned short)port;
  pImp->remote_port2 = (unsigned short)port2;

  trader_mduser_api_efh32_tick_dict_init(pImp);
  
  trader_mduser_api_on_rsp_user_login(self, 0, "OK");

  sleep(1);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_efh32_thread, (void*)self);

  return ;
}

void trader_mduser_api_efh32_stop(trader_mduser_api* self)
{
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }

  trader_mduser_api_efh32_tick_dict_destory(pImp);
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_efh32_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_efh32_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_efh32_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  trader_mduser_api_efh32_tick_dict_add(pImp, instrument);
  
  return ;
}

void on_receive_message(void* arg, const char* buff, unsigned int len)
{
	if ( 0 == (len % sizeof(efh3_2_fut_lev1)))
	{
		/// add by zhou.hu review  2019/12/25  接收的到是上期所的期货lev1
		unsigned int proc_len = 0;
		while(proc_len < len)
		{
			efh3_2_fut_lev1* ptr_data = (efh3_2_fut_lev1*)(buff + proc_len);
			on_receive_fut_lev1(arg, ptr_data);
			proc_len +=  sizeof(efh3_2_fut_lev1);
		}

		return;
	}


	if ( 0 == (len % sizeof(efh3_2_fut_lev2)))
	{
		/// add by zhou.hu review  2019/12/25  接收的到是上期所的期货lev1
		unsigned int proc_len = 0;
		while(proc_len < len)
		{
			efh3_2_fut_lev2* ptr_data = (efh3_2_fut_lev2*)(buff + proc_len);
			on_receive_fut_lev2(arg, ptr_data);
			proc_len +=  sizeof(efh3_2_fut_lev2);
		}

		return;
	}

	if ( 0 == (len % sizeof(efh3_2_opt_lev1)))
	{
		/// add by zhou.hu review  2019/12/25  接收的到是上期所的期货lev1
		unsigned int proc_len = 0;
		while(proc_len < len)
		{
			efh3_2_opt_lev1* ptr_data = (efh3_2_opt_lev1*)(buff + proc_len);
			on_receive_opt_lev1(arg, ptr_data);
			proc_len +=  sizeof(efh3_2_opt_lev1);
		}

		return;
	}


	if ( 0 == (len % sizeof(efh3_2_opt_lev2)))
	{
		/// add by zhou.hu review  2019/12/25  接收的到是上期所的期货lev1
		unsigned int proc_len = 0;
		while(proc_len < len)
		{
			efh3_2_opt_lev2* ptr_data = (efh3_2_opt_lev2*)(buff + proc_len);
			on_receive_opt_lev2(arg, ptr_data);
			proc_len +=  sizeof(efh3_2_opt_lev2);
		}

		return;
	}
	
}

void on_receive_fut_lev1(void* arg, efh3_2_fut_lev1* pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  int found = trader_mduser_api_efh32_tick_dict_find(pImp, pMarketData->m_symbol);

  if(!found){
    return;
  }
  
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->m_symbol);
  strcpy(oTick.TradingDay, "20210101");
  snprintf(oTick.UpdateTime, sizeof(oTick.UpdateTime), "%02d%02d%02d",
    (int)pMarketData->m_update_time_h,
    (int)pMarketData->m_update_time_m,
    (int)pMarketData->m_update_time_s
  );
  oTick.UpdateMillisec = pMarketData->m_millisecond;
  oTick.BidPrice1 = pMarketData->m_bid_px;
  oTick.BidVolume1 = pMarketData->m_bid_share;
  oTick.AskPrice1 = pMarketData->m_ask_px;
  oTick.AskVolume1 = pMarketData->m_ask_share;
  oTick.UpperLimitPrice = pMarketData->m_ask_px * 1.1;
  oTick.LowerLimitPrice = pMarketData->m_bid_px * 0.9;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void on_receive_fut_lev2(void* arg, efh3_2_fut_lev2* pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  int found = trader_mduser_api_efh32_tick_dict_find(pImp, pMarketData->m_symbol);

  if(!found){
    return;
  }
  
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->m_symbol);
  strcpy(oTick.TradingDay, "20210101");
  snprintf(oTick.UpdateTime, sizeof(oTick.UpdateTime), "%02d%02d%02d",
    (int)pMarketData->m_update_time_h,
    (int)pMarketData->m_update_time_m,
    (int)pMarketData->m_update_time_s
  );
  oTick.UpdateMillisec = pMarketData->m_millisecond;
  oTick.BidPrice1 = pMarketData->m_bid1_px;
  oTick.BidVolume1 = pMarketData->m_bid1_share;
  oTick.AskPrice1 = pMarketData->m_ask1_px;
  oTick.AskVolume1 = pMarketData->m_ask1_share;
  oTick.UpperLimitPrice = pMarketData->m_ask1_px * 1.1;
  oTick.LowerLimitPrice = pMarketData->m_bid1_px * 0.9;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void on_receive_opt_lev1(void* arg, efh3_2_opt_lev1* pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  int found = trader_mduser_api_efh32_tick_dict_find(pImp, pMarketData->m_symbol);

  if(!found){
    return;
  }
  
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->m_symbol);
  strcpy(oTick.TradingDay, "20210101");
  snprintf(oTick.UpdateTime, sizeof(oTick.UpdateTime), "%02d%02d%02d",
    (int)pMarketData->m_update_time_h,
    (int)pMarketData->m_update_time_m,
    (int)pMarketData->m_update_time_s
  );
  oTick.UpdateMillisec = pMarketData->m_millisecond;
  oTick.BidPrice1 = pMarketData->m_bid_px;
  oTick.BidVolume1 = pMarketData->m_bid_share;
  oTick.AskPrice1 = pMarketData->m_ask_px;
  oTick.AskVolume1 = pMarketData->m_ask_share;
  oTick.UpperLimitPrice = pMarketData->m_ask_px * 1.1;
  oTick.LowerLimitPrice = pMarketData->m_bid_px * 0.9;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void on_receive_opt_lev2(void* arg, efh3_2_opt_lev2* pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  int found = trader_mduser_api_efh32_tick_dict_find(pImp, pMarketData->m_symbol);

  if(!found){
    return;
  }
  
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->m_symbol);
  strcpy(oTick.TradingDay, "20210101");
  snprintf(oTick.UpdateTime, sizeof(oTick.UpdateTime), "%02d%02d%02d",
    (int)pMarketData->m_update_time_h,
    (int)pMarketData->m_update_time_m,
    (int)pMarketData->m_update_time_s
  );
  oTick.UpdateMillisec = pMarketData->m_millisecond;
  oTick.BidPrice1 = pMarketData->m_bid1_px;
  oTick.BidVolume1 = pMarketData->m_bid1_share;
  oTick.AskPrice1 = pMarketData->m_ask1_px;
  oTick.AskVolume1 = pMarketData->m_ask1_share;
  oTick.UpperLimitPrice = pMarketData->m_ask1_px * 1.1;
  oTick.LowerLimitPrice = pMarketData->m_bid1_px * 0.9;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void* trader_mduser_api_efh32_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_efh32* pImp = (trader_mduser_api_efh32*)self->pUserApi;
  int m_sock[2];
  int max_sock;
  int ret;
  int i;

	char line[MSG_BUF_SIZE] = "";

	int n_rcved = -1;

	struct sockaddr_in muticast_addr;
	memset(&muticast_addr, 0, sizeof(muticast_addr));

  
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

  do{
    i = 0;
    ret = udp_sock_init(&m_sock[i], pImp->remote_ip, pImp->remote_port, pImp->local_ip);
    if(ret < 0){
      break;
    }
    i++;

    ret = udp_sock_init(&m_sock[i], pImp->remote_ip2, pImp->remote_port2, pImp->local_ip);
    if(ret < 0){
      break;
    }

    if(m_sock[0] > m_sock[1]){
      max_sock = m_sock[0] + 1;
    }else{
      max_sock = m_sock[1] + 1;
    }

    pImp->loop_flag = 1;
    while (pImp->loop_flag)
    {
    	FD_ZERO( &readSet);
    	FD_ZERO( &writeSet);
    	FD_ZERO( &errorSet);

      for(i = 0; i < sizeof(m_sock) / sizeof(int); i++){
      	FD_SET(m_sock[i], &readSet);
      	FD_SET(m_sock[i], &errorSet);
      }
      tval.tv_usec = 100 * 1000; //100 ms
      tval.tv_sec = 0;

      n_rcved = select(max_sock, &readSet, &writeSet, &errorSet, &tval);
      
    	if(n_rcved < 1) { // timeout
        continue;
      }
      
    	socklen_t len = sizeof(sockaddr_in);
      for(i = 0; i < sizeof(m_sock) / sizeof(int); i++){
        if(!FD_ISSET(m_sock[i], &readSet){
          continue;
        }
        n_rcved = recvfrom(m_sock[i], line, MSG_BUF_SIZE, 0, (struct sockaddr*)&muticast_addr, &len);
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
          on_receive_message(self, line, n_rcved);
        }
      }

    }
  }while(0);
  
  for(i = 0; i < sizeof(m_sock) / sizeof(int); i++){
    if(m_sock[i] > 0){
      close(m_sock[i]);
    }
  }
  return (void*)NULL;
}

int trader_mduser_api_efh32_prase_url(const char* url, char* local_host, char* remote_host, int* port, char* remote_host2, int* port2)
{
  char* p;
  char* q;
  char tmp[6];

  // 定位://
  p = url;
  q = (char*)strstr(p, "|");
  if(NULL == q){
    return -1;
  }
  memcpy(local_host, p, q - p);
  local_host[q-p] = '\0';
  q++;

  
  p = q;
  q = (char*)strstr(p, ":");
  if(NULL == q){
    return -2;
  }
  memcpy(remote_host, p, q - p);
  remote_host[q-p] = '\0';
  q++;

  p = q;
  q = (char*)strstr(p, "|");
  if(NULL == q){
    return -3;
  }
  memcpy(tmp, p, q - p);
  *port = atoi(tmp);
  q++;

  p = q;
  q = (char*)strstr(p, ":");
  if(NULL == q){
    return -4;
  }
  memcpy(remote_host2, p, q - p);
  remote_host2[q-p] = '\0';
  q++;

  p = q;
  *port2 = atoi(p);
  
  return 0;
}



