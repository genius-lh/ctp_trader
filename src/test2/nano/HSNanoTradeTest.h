#ifndef _HS_NANO_TEST_H_
#define _HS_NANO_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HSDataType.h"
#include "HSExtendStruct.h"
#include "HSExtendTradeApi.h"

class CHSNanoTraderHandler : public CHSExtendTradeSpi
{
public:
  CHSNanoTraderHandler(CHSExtendTradeApi* api);

  virtual ~CHSNanoTraderHandler();

  void OnFrontConnected();

  void OnFrontDisconnected(int nResult);

  void OnRspAuthenticate(CHSRspAuthenticateField *pRspAuthenticate, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspUserLogin(CHSRspUserLoginField *pRspUserLogin, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspErrorOrderInsert(CHSRspOrderInsertField *pRspOrderInsert, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspOrderAction(CHSRspOrderActionField *pRspOrderAction, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspQryOrder(CHSOrderField *pRspQryOrder, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspQryTrade(CHSTradeField *pRspQryTrade, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspQryInstrument(CHSRspQryInstrumentField *pRspQryInstrument, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRtnOrder(CHSOrderField *pRtnOrder);

  void OnRtnTrade(CHSTradeField *pRtnTrade);

  void OnErrRtnOrderAction(CHSOrderActionField *pRtnOrder);

  void OnRspQryPosition(CHSRspQryPositionField *pRspQryPosition, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  void OnRspQryTradingAccount(CHSRspQryTradingAccountField *pRspQryTradingAccount, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  int _IsErrorRspInfo(CHSRspInfoField *pRspInfo);

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_UserId;
  char* m_OldPasswd;
  int m_Loop;

  CHSExtendTradeApi* m_Arg;
  long m_RequestId;

public:
  void Loop();
  int ShowMenu();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryInvestorAccount();
  void QryInvestorPosition();
  void QryInstrument();
};


#endif //_HS_NANO_TEST_H_
