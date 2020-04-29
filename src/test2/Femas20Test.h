#ifndef _XSPEED_TEST_H_
#define _XSPEED_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

class CFemasTraderHandler : public CUstpFtdcTraderSpi
{
public:
  CFemasTraderHandler();

  virtual ~CFemasTraderHandler();
  
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
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	//void OnHeartBeatWarning(int nTimeLapse);
	
	///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	//void OnPackageStart(int nTopicID, int nSequenceNo);
	
	///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	//void OnPackageEnd(int nTopicID, int nSequenceNo);

	///����Ӧ��
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ǰ��ϵͳ�û���¼Ӧ��
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��˳�Ӧ��
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û������޸�Ӧ��
	void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��Ӧ��
	void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��Ӧ��
	//void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���۲���Ӧ��
	//void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ѯ������Ӧ��
	//void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ͻ��������Ӧ��
	//void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û���������Ӧ��
	//void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û�����ϯ�����Ӧ��
	//void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����������֪ͨ
	//void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel);

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

	///�˻������ر�
	//void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes);

	///���ۻر�
	//void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote);

	///����¼�����ر�
	//void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo);

	///���۳�������ر�
	//void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo);

	///ѯ�ۻر�
	//void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote);

	///������Ϲ���֪ͨ
	//void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg);

	///�ͻ��������ȷ��
	//void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction);

	///�û���������
	//void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit);

	///������ѯӦ��
	void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ɽ�����ѯӦ��
	void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ͷ�����˻���ѯӦ��
	void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ױ����ѯӦ��
	//void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ�����ʽ��˻���ѯӦ��
	void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��Լ��ѯӦ��
	void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������ѯӦ��
	void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���ֲֲ߳�ѯӦ��
	void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�Ϲ������ѯӦ��
	//void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯӦ��
	//void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ�����������ʲ�ѯӦ��
	//void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߱�֤���ʲ�ѯӦ��
	//void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ױ�����ϳֲֲ�ѯӦ��
	//void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ױ��뵥�ȳֲֲ�ѯӦ��
	//void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��͸��ܿͻ���֤Ӧ��
	void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��͸�����Ϣ�ɼ��м��ϴ���Ϣ��Ӧ
	//virtual void OnRspDSProxySubmitInfo(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_InvestorID;
  char* m_OutputFile;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void UserLogin();
  void UserLogout();
  void UserPasswordUpdate();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryUserInvestor();
  void QryInvestorAccount();
  void QryInstrument();
  void DSUserCertification();
  void QryExchange();
  void QryInvestorPosition();
  void SubscribeTopic();
  
	void PrintTrade(CUstpFtdcTradeField *pTrade);
	void PrintOrder(CUstpFtdcOrderField *pOrder);
  void PrintInstrument(CUstpFtdcRspInstrumentField *pRspInstrument);

};


#endif //_XSPEED_TEST_H_

