#ifndef _TRADER_MDUSER_PROXY_CTP_H_
#define _TRADER_MDUSER_PROXY_CTP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"

#include "TraderMduserProxyUtil.h"

#ifdef CTP_SOPT
using namespace ctp_sopt;
#endif

class TraderMduserProxyCtpHandler : public CThostFtdcMdSpi
{
public:
  TraderMduserProxyCtpHandler(TraderMduserProxyUtil* util);

  virtual ~TraderMduserProxyCtpHandler();
  
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
  char m_BrokerID[64];
  char m_UserId[64];
  char m_Passwd[64];
  char m_Address[128];
  char m_Workspace[128];
  
  void* m_Arg;
  long m_RequestId;

public:
  void loop();
  
private:
  void init();
  void login();
  void subMarketData();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_CTP_H_

