#ifndef _TRADER_MDUSER_PROXY_REM_H_
#define _TRADER_MDUSER_PROXY_REM_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

#include "TraderMduserProxyUtil.h"

class TraderMduserProxyRemHandler : public EESQuoteEvent
{
public:
  TraderMduserProxyRemHandler(TraderMduserProxyUtil* util);

  virtual ~TraderMduserProxyRemHandler();
  
	/// \brief 当服务器连接成功，登录前调用, 如果是组播模式不会发生, 只需判断InitMulticast返回值即可
  void OnEqsConnected();

	/// \brief 当服务器曾经连接成功，被断开时调用，组播模式不会发生该事件
  void OnEqsDisconnected();

	/// \brief 当登录成功或者失败时调用，组播模式不会发生
	/// \param bSuccess 登陆是否成功标志  
	/// \param pReason  登陆失败原因  
  void OnLoginResponse(bool bSuccess, const char* pReason);

	/// \brief 收到行情时调用,具体格式根据instrument_type不同而不同
	/// \param chInstrumentType  EES行情类型
	/// \param pDepthQuoteData   EES统一行情指针  
  void OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData);

	/// \brief 注册symbol响应消息来时调用，组播模式不支持行情注册
	/// \param chInstrumentType  EES行情类型
	/// \param pSymbol           合约名称
	/// \param bSuccess          注册是否成功标志
  void OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);

	/// \brief  注销symbol响应消息来时调用，组播模式不支持行情注册
	/// \param chInstrumentType  EES行情类型
	/// \param pSymbol           合约名称
	/// \param bSuccess          注册是否成功标志
  void OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);

private:
  char m_BrokerID[64];
  char m_UserId[64];
  char m_Passwd[64];
  char m_AddressIp[32];
  char m_AddressPort[8];
  
  void* m_Arg;
  long m_RequestId;

  int m_ContractIdx;

public:
  void loop();
  
private:
  void init();
  void login();
  void subMarketData();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_REM_H_

