#ifndef _TRADER_MDUSER_PROXY_MD_RECEIVER_H_
#define _TRADER_MDUSER_PROXY_MD_RECEIVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "TraderMduserProxyUtil.h"

//#include "trader_mduser_api_ef_vi.h"

class TraderMduserProxyMdReceiverHandler
{
public:
  TraderMduserProxyMdReceiverHandler(TraderMduserProxyUtil* util);
  virtual ~TraderMduserProxyMdReceiverHandler();

private:
  char m_FilterName[32];
  int m_BusId;
  int m_CpuId;

  int m_LoopFlag;
  pthread_t m_ThreadId;
  //struct resources m_Res;
  //trader_mduser_api_ef_vi_ops m_Ops;

public:
  void loop();

  void work();

  int OnRecvMessage(char* data, int len);
  
private:
  void init();
  void start();
  void stop();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_MD_RECEIVER_H_

