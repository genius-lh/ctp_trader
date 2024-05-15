#ifndef _TRADER_MDUSER_PROXY_EFVI_H_
#define _TRADER_MDUSER_PROXY_EFVI_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "TraderMduserProxyUtil.h"

#include "trader_mduser_api_ef_vi.h"

class TraderMduserProxyEfviHandler
{
public:
  TraderMduserProxyEfviHandler(TraderMduserProxyUtil* util);
  virtual ~TraderMduserProxyEfviHandler();

private:
  char m_RemoteIp[32];
  char m_RemotePort[8];
  char m_EthName[32];
  char m_DataType[8];
  int m_CpuId;

  int m_LoopFlag;
  pthread_t m_ThreadId;
  struct resources m_Res;
  trader_mduser_api_ef_vi_ops m_Ops;

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


#endif //_TRADER_MDUSER_PROXY_EFVI_H_

