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
  
  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void OnFrontDisconnected(int nReason);

	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
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

