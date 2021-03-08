#ifndef _XSPEED_TEST_H_
#define _XSPEED_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
using namespace ctp_sopt;

class CCtpSopTraderHandler : public CThostFtdcTraderSpi
{
public:
  CCtpSopTraderHandler();

  virtual ~CCtpSopTraderHandler();
  
  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
  void OnFrontConnected();

  ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ���������
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
  
  ///����¼��������Ӧ
  void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������������Ӧ
  void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ���߽�����ȷ����Ӧ
  void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///ִ������¼��������Ӧ
  void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///ִ���������������Ӧ
  void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������¼��������Ӧ
	void OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ������Ӧ
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ɽ���Ӧ
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ���������Ӧ
	void OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�����ѯ��Լ��Ӧ
  void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����Ӧ��
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����֪ͨ
  void OnRtnOrder(CThostFtdcOrderField *pOrder);

  ///�ɽ�֪ͨ
  void OnRtnTrade(CThostFtdcTradeField *pTrade);

  ///����¼�����ر�
  void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

  ///������������ر�
  void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

  ///ִ������֪ͨ
  void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

  ///ִ������¼�����ر�
  void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);

  ///ִ�������������ر�
  void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);
  
	///�������֪ͨ
	void OnRtnCombAction(CThostFtdcCombActionField *pCombAction);

	///�������¼�����ر�
	void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo);

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_NewPasswd;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void QueryContract();
  void SettlementInfoConfirm();
  void InsertOrder(char* inst, char* exchange_id, char buy_sell, char open_close, double price, int vol, char covered);
  void ExecuteOrder(char* inst, char* exchange_id, int vol);
  void WithdrawOrder(char* inst, char* exchange_id, char* sys_order_id);
  void QueryOrder(char* inst, char* exchange_id, char* sys_order_id);
  
  void CoveredOpen();
  void CoveredClose();
  void BuyOpen();
  void SellClose();
  void SellOpen();
  void BuyClose();
  void ExecuteOrder();
  void LogOut();
  void Withdraw();
  void CombAction();

  void QryOrder();
  void QryTrade();
  void QryInvestorPosition();
  void QryTradingAccount();
  void QryCombAction();

};


#endif //_XSPEED_TEST_H_
