

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "xele_md_data_if.h"

#define GDXELE_LOG(...) printf(__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

#include "queue.h"

#include "trader_data.h"
#include "trader_mduser_api.h"

extern trader_mduser_api_method* trader_mduser_api_gd_method_get();

static void trader_mduser_api_gd_start(trader_mduser_api* self);
static void trader_mduser_api_gd_stop(trader_mduser_api* self);
static void trader_mduser_api_gd_login(trader_mduser_api* self);
static void trader_mduser_api_gd_logout(trader_mduser_api* self);
static void trader_mduser_api_gd_subscribe(trader_mduser_api* self, char* instrument);

static void* trader_mduser_api_gd_thread(void* arg);
static void* trader_mduser_api_gd_tcp_thread(void* arg);

static int trader_mduser_api_gd_prase_url(const char* url, char* local_host, char* remote_host, int* port);
static void gd_mduser_on_rtn_depth_market_data(void* arg, CXeleCffexLevelOneMarketData *pMarketData);

#ifdef __cplusplus
}
#endif

typedef struct xele_md_static_inf_def xele_md_static_inf;
struct xele_md_static_inf_def{
  TAILQ_ENTRY(xele_md_static_inf_def) next;
  struct CXeleCffexMarketDataStaticField data;
};

///最大的接收缓冲区最
#define	RCV_BUF_SIZE						65535
#define	MSG_MAX_LEN						65535

typedef struct trader_mduser_api_gd_def trader_mduser_api_gd;
struct trader_mduser_api_gd_def{
	char remote_ip[16];			///< 组播IP
	unsigned short remote_port;			///< 组播端口
	char local_ip[16];				///< 本地IP
	pthread_t thread_id;
  int loop_flag;
  pthread_mutex_t thread_mutex;
	pthread_t thread_tcp_id;

  TAILQ_HEAD(xele_md_static_inf_list, xele_md_static_inf_def) md_static_list;
  int md_static_num;
  CXeleCffexMarketDataStaticField* md_static_field_list;
};

static int trader_mduser_api_static_inf_init(void* arg);

static int trader_mduser_api_static_inf_load(void* self, const char* ip, int port);

static int trader_mduser_api_static_inf_add(void* self, CXeleCffexMarketDataStaticField* pMarketDataStaticField);

static int trader_mduser_api_static_inf_sort(void* self);

static int trader_mduser_api_static_inf_search(void* self, CXeleCffexMarketDataStaticField* pMarketDataStaticField);

static int trader_mduser_api_static_inf_compare(CXeleCffexMarketDataStaticField* p, CXeleCffexMarketDataStaticField* q);

static int trader_mduser_api_static_inf_test();

trader_mduser_api_method* trader_mduser_api_gd_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_gd_start,
    trader_mduser_api_gd_stop,
    trader_mduser_api_gd_login,
    trader_mduser_api_gd_logout,
    trader_mduser_api_gd_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_gd_start(trader_mduser_api* self)
{
  //trader_mduser_api_static_inf_test();

  trader_mduser_api_gd* pImp = (trader_mduser_api_gd*)malloc(sizeof(trader_mduser_api_gd));
  self->pUserApi = (void*)pImp;
  int port;
  int ret = trader_mduser_api_gd_prase_url(self->pAddress, pImp->local_ip, pImp->remote_ip, &port);
  if(ret < 0){
    return;
  }

  pImp->remote_port = (unsigned short)port;

  trader_mduser_api_static_inf_init((void*)pImp);

	ret = pthread_create(&pImp->thread_id, NULL, trader_mduser_api_gd_thread, (void*)self);
  
	ret = pthread_create(&pImp->thread_tcp_id, NULL, trader_mduser_api_gd_tcp_thread, (void*)self);

  return ;
}

void trader_mduser_api_gd_stop(trader_mduser_api* self)
{
  trader_mduser_api_gd* pImp = (trader_mduser_api_gd*)self->pUserApi;
  pImp->loop_flag = 0;
  
  void* ret;
  if(pImp->thread_id){
    pthread_join(pImp->thread_id, &ret);
  }
  
  if(pImp->thread_tcp_id){
    pthread_join(pImp->thread_tcp_id, &ret);
  }

  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_gd_login(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_gd_logout(trader_mduser_api* self)
{
  return ;
}

void trader_mduser_api_gd_subscribe(trader_mduser_api* self, char* instrument)
{
  return ;
}

void gd_mduser_on_rtn_depth_market_data(void* arg, CXeleCffexLevelOneMarketData *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->InstrumentID);
  strcpy(oTick.UpdateTime, pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice;
  oTick.BidVolume1= pMarketData->BidVolume;
  oTick.AskPrice1 = pMarketData->AskPrice;
  oTick.AskVolume1= pMarketData->AskVolume;
  oTick.LastPrice = pMarketData->LastPrice;

  int ret;
  CXeleCffexMarketDataStaticField marketDataStaticField;
  strncpy(marketDataStaticField.InstrumentID, pMarketData->InstrumentID, sizeof(marketDataStaticField.InstrumentID));

  ret = trader_mduser_api_static_inf_search((void*)self->pUserApi, &marketDataStaticField);
  if(!ret){
    oTick.UpperLimitPrice= marketDataStaticField.UpperLimitPrice;
    oTick.LowerLimitPrice= marketDataStaticField.LowerLimitPrice;
  }else{
    oTick.UpperLimitPrice= oTick.AskPrice1 * 1.1;
    oTick.LowerLimitPrice= oTick.BidPrice1 * 0.9;
  }
  
  //strcpy(oTick.TradingDay, marketDataStaticField.TradingDay);//

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}
void* trader_mduser_api_gd_tcp_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_gd* pImp = (trader_mduser_api_gd*)self->pUserApi;
  const char* tcp_ip = "172.37.88.12";
  int tcp_port = 10007;
  int ret = trader_mduser_api_static_inf_load((void*)pImp, tcp_ip, tcp_port);
  if(ret < 0){
    GDXELE_LOG("trader_mduser_api_static_inf_load ret=[%d]\n", ret);
    exit(-1);
  }

  return (void*)NULL;
}

void* trader_mduser_api_gd_thread(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_gd* pImp = (trader_mduser_api_gd*)self->pUserApi;
  int m_sock;

	char line[RCV_BUF_SIZE] = "";

	int n_rcved = -1;

	struct sockaddr_in muticast_addr;
	memset(&muticast_addr, 0, sizeof(muticast_addr));

  
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

  do{
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    GDXELE_LOG("trader_mduser_api_gd_thread bind\n");
		struct sockaddr_in local_addr;
		memset(&local_addr, 0, sizeof(local_addr));
		local_addr.sin_family = AF_INET;
		//local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton(pImp->remote_ip, &local_addr.sin_addr);
		local_addr.sin_port = htons(pImp->remote_port);	//multicast port
		if (bind(m_sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0)
		{
			break;
		}

    GDXELE_LOG("trader_mduser_api_gd_thread IP_ADD_MEMBERSHIP\n");
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

    int ret = 0;

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
    	  if(!ret){
          GDXELE_LOG("gd_mduser_on_rtn_depth_market_data\n");
          ret = 1;
        }
    		gd_mduser_on_rtn_depth_market_data(self, (CXeleCffexLevelOneMarketData*)(line + sizeof(CXeleCffexMdMessageHead)));				
    	}
    }
  }while(0);
  if(m_sock > 0){
    close(m_sock);
  }

  return (void*)NULL;
}

int trader_mduser_api_gd_prase_url(const char* url, char* local_host, char* remote_host, int* port)
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

int trader_mduser_api_static_inf_init(void* arg)
{
  GDXELE_LOG("trader_mduser_api_static_inf_init enter\n");

  trader_mduser_api_gd* self = (trader_mduser_api_gd*)arg;
  TAILQ_INIT(&self->md_static_list);
  self->md_static_num = 0;
  pthread_mutex_init(&self->thread_mutex, NULL);
  
  self->loop_flag = 1;

  return 0;
}


int trader_mduser_api_static_inf_load(void* arg, const char* ip, int port)
{
  GDXELE_LOG("trader_mduser_api_static_inf_load enter\n");

  trader_mduser_api_gd* self = (trader_mduser_api_gd*)arg;

  // 建链
  int ret = 0;
  int fd = 0;
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    return -1;
  }

  int on = 1;
  ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (ret == -1) {
    return -2;
  }

  GDXELE_LOG("trader_mduser_api_static_inf_load connect\n");
  struct sockaddr_in tcp_server_addr;
  memset(&tcp_server_addr, 0, sizeof(tcp_server_addr));
  tcp_server_addr.sin_family = AF_INET;
  inet_aton(ip, &tcp_server_addr.sin_addr);
  tcp_server_addr.sin_port = htons(port);
  do {
    ret = connect(fd, (struct sockaddr *) &tcp_server_addr, sizeof(tcp_server_addr));
    if (ret == -1) {
      GDXELE_LOG("trader_mduser_api_static_inf_load connect failed\n");
      usleep(500 * 1000);
    }
  }while((ret < 0) && self->loop_flag);

  pthread_mutex_lock(&self->thread_mutex);

  char buf[MSG_MAX_LEN];
  while (true) {
    GDXELE_LOG("trader_mduser_api_static_inf_load recv\n");
    ret = recv(fd, buf, sizeof(CXeleCffexMdMessageHead) + sizeof(CXeleCffexMarketDataStaticField), MSG_WAITALL);
    if (ret == 0) {
      break;
    } else if (ret < 0) {
      break;
    }
    trader_mduser_api_static_inf_add(arg, (CXeleCffexMarketDataStaticField *) (buf + sizeof(CXeleCffexMdMessageHead)));
  }
  close(fd);
  trader_mduser_api_static_inf_sort(arg);
  pthread_mutex_unlock(&self->thread_mutex);

  return 0;
}

int trader_mduser_api_static_inf_add(void* arg, CXeleCffexMarketDataStaticField* pMarketDataStaticField)
{
  trader_mduser_api_gd* self = (trader_mduser_api_gd*)arg;

  xele_md_static_inf* p = (xele_md_static_inf*)malloc(sizeof(xele_md_static_inf));

  memcpy(&p->data, pMarketDataStaticField, sizeof(CXeleCffexMarketDataStaticField));

  GDXELE_LOG("p->data.InstrumentID=[%s]\n", p->data.InstrumentID);

  TAILQ_INSERT_TAIL(&self->md_static_list, p, next);

  self->md_static_num++;

  return 0;
}

int trader_mduser_api_static_inf_sort(void* arg)
{
  trader_mduser_api_gd* self = (trader_mduser_api_gd*)arg;
  self->md_static_field_list = (CXeleCffexMarketDataStaticField*)malloc(sizeof(CXeleCffexMarketDataStaticField) * self->md_static_num);
  xele_md_static_inf* p;
  CXeleCffexMarketDataStaticField* q;
  CXeleCffexMarketDataStaticField tmp;

  int i = 0;
  int j = 0;
  int ret;
  TAILQ_FOREACH(p, &self->md_static_list, next){
    if(0 == i){
      memcpy(&self->md_static_field_list[i], &p->data, sizeof(CXeleCffexMarketDataStaticField));
    }else{
      j = i - 1;
      while(j > -1){
        ret = trader_mduser_api_static_inf_compare(&p->data, &self->md_static_field_list[j]);
        if(ret < 0){
          memcpy(&self->md_static_field_list[j+1], &self->md_static_field_list[j], sizeof(CXeleCffexMarketDataStaticField));
          j--;
        }else{
          break;
        }
      }
      memcpy(&self->md_static_field_list[j+1], &p->data, sizeof(CXeleCffexMarketDataStaticField));      
    }
    i++;
  }
  return 0;
}

int trader_mduser_api_static_inf_search(void* arg, CXeleCffexMarketDataStaticField* pMarketDataStaticField)
{
  trader_mduser_api_gd* self = (trader_mduser_api_gd*)arg;
  
  pthread_mutex_lock(&self->thread_mutex);

	int low = 0;
	int high = self->md_static_num - 1;
	int mid;
  int ret;
  
	while(low <= high){
		mid = (low+high) / 2;
    ret = trader_mduser_api_static_inf_compare(pMarketDataStaticField, &self->md_static_field_list[mid]);
		if(0 == ret){
      pMarketDataStaticField->UpperLimitPrice = self->md_static_field_list[mid].UpperLimitPrice;
      pMarketDataStaticField->LowerLimitPrice = self->md_static_field_list[mid].LowerLimitPrice;
      pthread_mutex_unlock(&self->thread_mutex);
			return 0;    
		}else if(ret > 0){
			low = mid + 1;
		}else{
			high = mid - 1;
    }
	}
  
  pthread_mutex_unlock(&self->thread_mutex);
	return -1; 

}

int trader_mduser_api_static_inf_compare(CXeleCffexMarketDataStaticField* p, CXeleCffexMarketDataStaticField* q)
{
  return strncmp(p->InstrumentID, q->InstrumentID, sizeof(p->InstrumentID));
}

int trader_mduser_api_static_inf_test()
{
  trader_mduser_api_gd* self = (trader_mduser_api_gd*)malloc(sizeof(trader_mduser_api_gd));
  
  TAILQ_INIT(&self->md_static_list);
  self->md_static_num = 0;

  CXeleCffexMarketDataStaticField field;
  memset(&field, 0, sizeof(field));

  strncpy(field.InstrumentID, "IC2010", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IF2011", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IH2103", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IF2009", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IC2011", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IH2011", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IH2009", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IH2012", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IF2103", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IC2009", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);
  
  strncpy(field.InstrumentID, "IF2012", sizeof(field.InstrumentID));
  trader_mduser_api_static_inf_add((void*)self, &field);

  trader_mduser_api_static_inf_sort((void*)self);

  int i = 0;
  for(i = 0; i < self->md_static_num; i++){
    GDXELE_LOG("[%d][%s]\n", i, self->md_static_field_list[i].InstrumentID);
  }

  int ret = 0;
  ret = trader_mduser_api_static_inf_search((void*)self, &field);
  GDXELE_LOG("trader_mduser_api_static_inf_search ret=[%d]\n", ret);

  exit(-1);
}


