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

  ///错误应答
  void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///风控前置系统用户登录应答
  void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///用户退出应答
  void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户密码修改应答
	void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单录入应答
  void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单操作应答
  void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
   
  ///成交回报
  void OnRtnTrade(CUstpFtdcTradeField *pTrade);

  ///报单回报
  void OnRtnOrder(CUstpFtdcOrderField *pOrder);

  ///报单录入错误回报
  void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);

  ///报单操作错误回报
  void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);

  ///合约交易状态通知
  void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus);

  ///报单查询应答
  void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///成交单查询应答
  void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///可用投资者账户查询应答
  void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者资金账户查询应答
  void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///合约查询应答
  void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///交易所查询应答
  void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者持仓查询应答
  void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///穿透监管客户认证应答
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

