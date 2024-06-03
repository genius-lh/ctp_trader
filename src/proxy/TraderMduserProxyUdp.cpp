
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "cmn_util.h"

#include "trader_mduser_api_ef_vi.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyUdp.h"

#include "TraderMduserProxyStruct.h"

#define	RCV_BUF_SIZE (8 * 1024 * 1024)
#define MSG_BUF_SIZE (64 * 1024)

static int udp_sock_init(int* pfd, const char* remote_ip, int remote_port, const char* local_ip);

static int udp_sock_recv(int fd, void* arg);

static int udp_sock_close(int fd);

static void* trader_mduser_proxy_udp_thread(void* arg);


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
  TraderMduserProxyUdpHandler* self = (TraderMduserProxyUdpHandler*)arg;

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
      self->OnRecvMessage(line, n_rcved);
    }

  }while(loop);

  return 0;
}

int udp_sock_close(int fd)
{
  close(fd);
  return 0;
}


static void* trader_mduser_proxy_udp_thread(void* arg)
{
  TraderMduserProxyUdpHandler* self = (TraderMduserProxyUdpHandler*)arg;
  self->work();
  return (void*)NULL;
}



int main(int argc, char* argv[])
{
  int ret;
  // 初始化util
  TraderMduserProxyUtil* pMduserUtil = new TraderMduserProxyUtil(argv[1]);
  ret = pMduserUtil->init();
  if(ret < 0){
    // 初始化异常
    exit(-1);
  }

  // 初始化handler
  TraderMduserProxyUdpHandler* pMduserHandler = new TraderMduserProxyUdpHandler(pMduserUtil);

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}


TraderMduserProxyUdpHandler::TraderMduserProxyUdpHandler(TraderMduserProxyUtil* util)
  :m_CpuId(-1)
  , pProxyUtil(util)
{
  init();
}

TraderMduserProxyUdpHandler::~TraderMduserProxyUdpHandler()
{

}

void TraderMduserProxyUdpHandler::loop()
{

  // 连接交易服务器
  start();

  pProxyUtil->loop();

  stop();

  return ;  
}

void TraderMduserProxyUdpHandler::work()
{
  int sock;
  int max_sock;
  int ret;

	int n_rcved = -1;

	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

  //创建线路
  ret = udp_sock_init(&sock, m_RemoteIp, atoi(m_RemotePort), m_LocalIp);
  if(ret < 0){
    return;
  }

  max_sock = sock + 1;

  // 绑核
	cmn_util_bind_cpu(m_CpuId);

  // 主循环
  while (m_LoopFlag)
  {
    FD_ZERO( &readSet);
    FD_ZERO( &writeSet);
    FD_ZERO( &errorSet);

    FD_SET(sock, &readSet);
    FD_SET(sock, &errorSet);
    
    tval.tv_usec = 100 * 1000; //100 ms
    tval.tv_sec = 0;

    n_rcved = select(max_sock, &readSet, &writeSet, &errorSet, &tval);

    if(n_rcved < 1) { // timeout
      continue;
    }
    
    if(!FD_ISSET(sock, &readSet)){
      continue;
    }

    udp_sock_recv(sock, this);

  }
  
  // 退出处理
  if(sock > 0){
    udp_sock_close(sock);
  }
  return;
}

void TraderMduserProxyUdpHandler::init()
{
  char tmp[8];
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_UDP", "MDUSER_REMOTE_IP", m_RemoteIp, sizeof(m_RemoteIp));
  pProxyUtil->getCfgString("MDUSER_UDP", "MDUSER_REMOTE_PORT", m_RemotePort, sizeof(m_RemotePort));
  pProxyUtil->getCfgString("MDUSER_UDP", "MDUSER_LOCAL_IP", m_LocalIp, sizeof(m_LocalIp));
  pProxyUtil->getCfgString("MDUSER_UDP", "MDUSER_DATA_TYPE", m_DataType, sizeof(m_DataType));
  pProxyUtil->getCfgString("MDUSER_UDP", "CPU_ID", tmp, sizeof(tmp));
  m_CpuId = atoi(tmp);
  m_ThreadId = 0;

  trader_mduser_api_ef_vi_ops_init(&m_Ops, atoi(m_DataType));

  return;
}

void TraderMduserProxyUdpHandler::start()
{

  m_LoopFlag = 1;
	pthread_create(&m_ThreadId, NULL, trader_mduser_proxy_udp_thread, (void*)this);

  return ;  
}


void TraderMduserProxyUdpHandler::stop()
{
  void* ret;
  m_LoopFlag = 0;
  
  if(m_ThreadId){
    pthread_join(m_ThreadId, &ret);
  }
  return ;  
}

///深度行情通知
int TraderMduserProxyUdpHandler::OnRecvMessage(char* data, int size)
{
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;

  const char* InstrumentID = data + m_Ops.m_md_id_pos;

  int md_size = m_Ops.m_md_size;
  if(md_size > size){    
    return md_size;
  }
  
  int found = pProxyUtil->findContractById(InstrumentID);
  if(!found){
    return md_size;
  }

  // 转发行情
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  m_Ops.md_fill(pTick, data);
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  return md_size;
}



