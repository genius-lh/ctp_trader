#ifndef _TRADER_MDUSER_PROXY_UDP_H_
#define _TRADER_MDUSER_PROXY_UDP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "TraderMduserProxyUtil.h"

class TraderMduserProxyUdpHandler
{
public:
  TraderMduserProxyUdpHandler(TraderMduserProxyUtil* util);
  virtual ~TraderMduserProxyUdpHandler();

  static void* udp_thread(void* arg);

private:
  char m_RemoteIp[32];
  char m_RemotePort[8];
  char m_LocalIp[32];
  char m_DataType[8];

  int m_LoopFlag;
  pthread_t m_ThreadId;
  int m_CpuId;
  trader_mduser_api_ef_vi_ops m_Ops;

public:
  void loop();

  void work();

  void OnRecvMessage(char* data, int len);
  
private:
  void init();
  void start();
  void stop();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_UDP_H_

