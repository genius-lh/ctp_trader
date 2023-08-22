

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "trader_mduser_api_ef_vi.h"

#include "trader_tick_dict.h"

#ifdef __cplusplus
extern "C" {
#endif


#define XHEFH32_LOG(...) printf(__VA_ARGS__)

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "dict.h"

extern trader_mduser_api_method* trader_mduser_api_udp_method_get();

static void trader_mduser_api_udp_start(trader_mduser_api* self);
static void trader_mduser_api_udp_stop(trader_mduser_api* self);
static void trader_mduser_api_udp_login(trader_mduser_api* self);
static void trader_mduser_api_udp_logout(trader_mduser_api* self);
static void trader_mduser_api_udp_subscribe(trader_mduser_api* self, char* instrument);
static void trader_mduser_api_udp_config(trader_mduser_api* self);

static void* trader_mduser_api_udp_thread(void* arg);

static void trader_mduser_api_udp_sock_recv(void* arg, int fd);

static void on_receive_message(void* arg, char* buff, unsigned int len);

#ifdef __cplusplus
}
#endif

///最大的接收缓冲区最
#define	RCV_BUF_SIZE (8 * 1024 * 1024)
#define MSG_BUF_SIZE (64 * 1024)

typedef struct trader_mduser_api_udp_def trader_mduser_api_udp;
struct trader_mduser_api_udp_def{
  int m_type;
  trader_mduser_api_ef_vi_ops m_ops;
  char local_ip[16];				///< 本地IP
  int addr_count;
  struct {
    char m_ip[16];
    unsigned short m_port;
  } m_addr[2];
  pthread_t thread_id;
  int loop_flag;
  trader_tick_dict* tick_dict;
};

static int udp_sock_init(int* pfd, const char* remote_ip, int remote_port, const char* local_ip);

static int udp_sock_recv(int fd, void* arg);

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

int udp_sock_recv(int fd, void* arg)
{
	struct sockaddr_in muticast_addr;
	memset(&muticast_addr, 0, sizeof(muticast_addr));
	char line[MSG_BUF_SIZE] = "";
	int n_rcved = -1;
  socklen_t len = sizeof(sockaddr_in);
  int loop = 1;

  do{
    n_rcved = recvfrom(fd, line, MSG_BUF_SIZE, 0, (struct sockaddr*)&muticast_addr, &len);
    if ( n_rcved < 0) 
    {
      break;
    } 
    else if (0 == n_rcved)
    {
      break;
    }         
    else
    {
      on_receive_message(arg, line, n_rcved);
    }

  }while(loop);

  return 0;
}

trader_mduser_api_method* trader_mduser_api_udp_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_udp_start,
    trader_mduser_api_udp_stop,
    trader_mduser_api_udp_login,
    trader_mduser_api_udp_logout,
    trader_mduser_api_udp_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_udp_start(trader_mduser_api* self)
{
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)malloc(sizeof(trader_mduser_api_udp));
  self->pUserApi = (void*)pImp;
  
  trader_mduser_api_udp_config(self);

  pImp->tick_dict = trader_tick_dict_new();
  
  trader_mduser_api_on_rsp_user_login(self, 0, "OK");

  sleep(1);

	pthread_create(&pImp->thread_id, NULL, trader_mduser_api_udp_thread, (void*)self);

  return ;
}

void trader_mduser_api_udp_stop(trader_mduser_api* self)
{
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }

  if(pImp->tick_dict){  
    trader_tick_dict_free(pImp->tick_dict);
    pImp->tick_dict = NULL;
  }
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_udp_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_udp_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_udp_config(trader_mduser_api* self)
{
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)self->pUserApi;
  char* pSavePtr;
  char sAddress[256];
  char* pTemp;
  
  int i = 0;

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pTemp = strtok_r(sAddress, "|", &pSavePtr);
    pImp->m_type = (unsigned short)atoi(pTemp);

    pTemp = strtok_r(NULL, "|", &pSavePtr);
    pImp->addr_count = (unsigned short)atoi(pTemp);
    
    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(pImp->local_ip, pTemp, sizeof(pImp->local_ip));
        
    i = 0;
    pTemp = strtok_r(NULL, "|", &pSavePtr);
    strncpy(pImp->m_addr[i].m_ip, pTemp, sizeof(pImp->m_addr[i].m_ip));

    pTemp = strtok_r(NULL, "|", &pSavePtr);
    pImp->m_addr[i].m_port = (unsigned short)atoi(pTemp);

    if(2 == pImp->addr_count){
      ++i;
      pTemp = strtok_r(NULL, "|", &pSavePtr);
      strncpy(pImp->m_addr[i].m_ip, pTemp, sizeof(pImp->m_addr[i].m_ip));
      
      pTemp = strtok_r(NULL, "|", &pSavePtr);
      pImp->m_addr[i].m_port = (unsigned short)atoi(pTemp);
    }

    trader_mduser_api_ef_vi_ops_init(&pImp->m_ops, pImp->m_type);
  }while(0);
  return ;
}

void trader_mduser_api_udp_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)self->pUserApi;
  trader_tick_dict* tickDict = pImp->tick_dict;

  tickDict->pMethod->xAdd(tickDict, instrument, (void*)NULL);
  
  return ;
}

void trader_mduser_api_udp_sock_recv(void* arg, int fd)
{
  int n_rcved = 0;
  char line[MSG_BUF_SIZE];
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int loop = 1;
  
	struct sockaddr_in muticast_addr;
  socklen_t len = sizeof(sockaddr_in);
	memset(&muticast_addr, 0, sizeof(muticast_addr));

  do{
    n_rcved = recvfrom(fd, line, MSG_BUF_SIZE, 0, (struct sockaddr*)&muticast_addr, &len);
    if ( n_rcved < 0) 
    {
      break;
    } 
    else if (0 == n_rcved)
    {
      break;
    }         
    else
    {
      on_receive_message(self, line, n_rcved);
    }
  }while(loop);
}


void on_receive_message(void* arg, char* buff, unsigned int len)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)self->pUserApi;
  trader_tick_dict* tickDict = pImp->tick_dict;
  trader_tick oTick;

  const char* InstrumentID = buff + pImp->m_ops.m_md_id_pos;
  void* save_ptr;
  int found = tickDict->pMethod->xFind(tickDict, InstrumentID, &save_ptr);
  if(!found){
    return;
  }
  pImp->m_ops.md_fill(&oTick, buff);
  
  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return ;
}

void* trader_mduser_api_udp_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_udp* pImp = (trader_mduser_api_udp*)self->pUserApi;
  int m_sock[2];
  int max_sock;
  int ret;
  int i;

	int n_rcved = -1;

	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

  do{
    for(i = 0; i < pImp->addr_count; i++){
      ret = udp_sock_init(&m_sock[i], pImp->m_addr[i].m_ip, pImp->m_addr[i].m_port, pImp->local_ip);
      if(ret < 0){
        break;
      }
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
      
      for(i = 0; i < sizeof(m_sock) / sizeof(int); i++){
        if(!FD_ISSET(m_sock[i], &readSet)){
          continue;
        }

        udp_sock_recv(m_sock[i], self);

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

