

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"

extern trader_mduser_api_method* trader_mduser_api_gf_method_get();

static void trader_mduser_api_gf_start(trader_mduser_api* self);
static void trader_mduser_api_gf_stop(trader_mduser_api* self);
static void trader_mduser_api_gf_login(trader_mduser_api* self);
static void trader_mduser_api_gf_logout(trader_mduser_api* self);
static void trader_mduser_api_gf_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_gf_thread(void* arg);

static int trader_mduser_api_gf_prase_url(const char* url, char* local_host, char* remote_host, int* port);
static void gf_mduser_on_rtn_depth_market_data(void* arg, CUstpFtdcDepthMarketDataField *pMarketData);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE						65535

typedef struct trader_mduser_api_gf_def trader_mduser_api_gf;
struct trader_mduser_api_gf_def{
	char remote_ip[16];			///< 组播IP
	unsigned short remote_port;			///< 组播端口
	char local_ip[16];				///< 本地IP
	pthread_t thread_id;
  int loop_flag;
};


trader_mduser_api_method* trader_mduser_api_gf_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_gf_start,
    trader_mduser_api_gf_stop,
    trader_mduser_api_gf_login,
    trader_mduser_api_gf_logout,
    trader_mduser_api_gf_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_gf_start(trader_mduser_api* self)
{
  trader_mduser_api_gf* pImp = (trader_mduser_api_gf*)malloc(sizeof(trader_mduser_api_gf));
  self->pUserApi = (void*)pImp;
  int port;
  int ret = trader_mduser_api_gf_prase_url(self->pAddress, pImp->local_ip, pImp->remote_ip, &port);
  if(ret < 0){
    return;
  }

  pImp->remote_port = (unsigned short)port;

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_gf_thread, (void*)self);

  return ;
}

void trader_mduser_api_gf_stop(trader_mduser_api* self)
{
  trader_mduser_api_gf* pImp = (trader_mduser_api_gf*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_gf_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_gf_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_gf_subscribe(trader_mduser_api* self, char* instrument)
{
  return ;
}

void gf_mduser_on_rtn_depth_market_data(void* arg, CUstpFtdcDepthMarketDataField *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->InstrumentID);
  strcpy(oTick.TradingDay, pMarketData->TradingDay);
  strcpy(oTick.UpdateTime, pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1= pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1= pMarketData->AskVolume1;
  oTick.UpperLimitPrice= pMarketData->UpperLimitPrice;
  oTick.LowerLimitPrice= pMarketData->LowerLimitPrice;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void* trader_mduser_api_gf_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_gf* pImp = (trader_mduser_api_gf*)self->pUserApi;
  int m_sock;

	char line[RCV_BUF_SIZE] = "";

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
    	n_rcved = recvfrom(m_sock, line, RCV_BUF_SIZE, 0, (struct sockaddr*)&muticast_addr, &len);
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
    		gf_mduser_on_rtn_depth_market_data(self, (CUstpFtdcDepthMarketDataField*)line);				
    	}
    }
  }while(0);
  if(m_sock > 0){
    close(m_sock);
  }

  return (void*)NULL;
}

int trader_mduser_api_gf_prase_url(const char* url, char* local_host, char* remote_host, int* port)
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



