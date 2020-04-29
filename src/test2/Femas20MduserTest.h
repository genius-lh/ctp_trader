#ifndef _CTP_SOP_MDUSER_TEST_H_
#define _CTP_SOP_MDUSER_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

//using namespace ctp_sopt;

class CFemasMduserHandler : public CUstpFtdcMduserSpi
{
public:
  CFemasMduserHandler();

  virtual ~CFemasMduserHandler();
  
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

	///错误应答
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///风控前置系统用户登录应答
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户退出应答
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅主题应答
	void OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///主题查询应答
	void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData);

	///订阅合约的相关信息
	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///退订合约的相关信息
	void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


public:
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_Filename;
  int m_Loop;

  time_t m_LastTm;
  int m_Gap;
  int m_BufferSize;
  int m_Cursor;
  CUstpFtdcDepthMarketDataField* m_Buffer;
  
  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void LogOut();
  void SubMarketData();
  void UnSubMarketData();
  int Check();
  void Flush();

};


#endif //_CTP_SOP_MDUSER_TEST_H_

