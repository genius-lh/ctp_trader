#ifndef _FEMAS20_TEST_H_
#define _FEMAS20_TEST_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

class CFemas20TestHandler : public CUstpFtdcTraderSpi
{
public:
  CFemas20TestHandler();
  virtual ~CFemas20TestHandler();
  void OnFrontConnected();
  void OnQryFrontConnected(); 
  void OnFrontDisconnected(int nReason);
  void OnQryFrontDisconnected(int nReason);

  ///����Ӧ��
  void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///���ǰ��ϵͳ�û���¼Ӧ��
  void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�û��˳�Ӧ��
  void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û������޸�Ӧ��
	void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����¼��Ӧ��
  void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������Ӧ��
  void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
   
  ///�ɽ��ر�
  void OnRtnTrade(CUstpFtdcTradeField *pTrade);

  ///�����ر�
  void OnRtnOrder(CUstpFtdcOrderField *pOrder);

  ///����¼�����ر�
  void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);

  ///������������ر�
  void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);

  ///��Լ����״̬֪ͨ
  void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus);

  ///������ѯӦ��
  void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�ɽ�����ѯӦ��
  void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����Ͷ�����˻���ѯӦ��
  void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ�����ʽ��˻���ѯӦ��
  void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��Լ��ѯӦ��
  void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������ѯӦ��
  void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ���ֲֲ߳�ѯӦ��
  void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��͸��ܿͻ���֤Ӧ��
  void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
  
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_Passwd;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;
  char m_InvestorID[16];
  
public:
  void Loop();
  int ShowMenu();
  void Login();
  void Logout();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryUserInvestor();
  void QryInvestorAccount();
  void QryInstrument();
  void QryExchange();
  void QryInvestorPosition();
  void PrintOrder(void* data);
  void PrintTrade(void* data);
  void PasswordUpdate();
};

#endif //_FEMAS20_TEST_H_

