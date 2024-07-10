#ifndef _TRADER_MDUSER_PROXY_CTP_H_
#define _TRADER_MDUSER_PROXY_CTP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "M2Api.h"

#include "TraderMduserProxyUtil.h"

class TraderMduserProxyM2Handler : public CM2Spi
{
public:
  TraderMduserProxyM2Handler(TraderMduserProxyUtil* util);

  virtual ~TraderMduserProxyM2Handler();
  
	///深度行情通知
	void OnRtnDepthMarketData(CMarketDataField *pDepthMarketData);

private:
  char m_Aim[10];
  char m_IfName[14];
  int m_CpuId;

  int m_LoopFlag;
  pthread_t m_ThreadId;

public:
  void loop();
  void work();
  
private:
  void init();
  void start();
  void stop();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_CTP_H_

