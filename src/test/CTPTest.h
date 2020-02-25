#ifndef _CTP_TEST_H_
#define _CTP_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"

class CCtpTraderHandler : public CThostFtdcTraderSpi
{
public:
  CCtpTraderHandler();
  virtual ~CCtpTraderHandler();
  
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
  
	///�ͻ�����֤��Ӧ
	void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����Ӧ��
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_NewPasswd;
  int* m_Loop;

  void* m_Arg;
  int m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void QueryContract();  
  void LogOut();
};


#endif //_CTP_TEST_H_

