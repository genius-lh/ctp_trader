#ifndef _CTP_TRADER_HANDLER_H_
#define _CTP_TRADER_HANDLER_H_

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ctp_trader_api_cb_def ctp_trader_api_cb;

struct ctp_trader_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspAuthenticate)(void* arg, CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUserLogin)(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUserLogout)(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspError)(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspOrderInsert)(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspOrderAction)(void* arg, CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRtnOrder)(void* arg, CThostFtdcOrderField *pOrder);
  void (*xOnRtnTrade)(void* arg, CThostFtdcTradeField *pTrade);
  void (*xOnErrRtnOrderInsert)(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
  void (*xOnErrRtnOrderAction)(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
  void (*xOnRspQryUserInvestor)(void* arg, CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspQryInstrument)(void* arg, CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspSettlementInfoConfirm)(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspQryTradingAccount)(void* arg, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspQryInvestorPosition)(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRtnInstrumentStatus)(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus);
};

#ifdef __cplusplus
}
#endif

class CCtpTraderHandler : public  CThostFtdcTraderSpi
{
public:
  CCtpTraderHandler(ctp_trader_api_cb* cb, void* arg);
  
public:
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

  ///����¼��������Ӧ
  void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������������Ӧ
  void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ���߽�����ȷ����Ӧ
  void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�����ѯͶ���ֲ߳���Ӧ
  void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�����ѯ�ʽ��˻���Ӧ
  void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�����ѯͶ������Ӧ
  void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

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

  ///��Լ����״̬֪ͨ
  void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

  ///����������֪ͨ
  virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {};

private:
  ctp_trader_api_cb* m_TraderCb;
  void* m_Arg;

};


#endif //_CTP_TRADER_HANDLER_H_
