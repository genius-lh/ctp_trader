#ifndef _XELE_TEST_H_
#define _XELE_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CXeleTraderApi.hpp"
#include <unistd.h>

class CXeleTraderHandler : public CXeleTraderSpi
{
public:
  CXeleTraderHandler();

  virtual ~CXeleTraderHandler();

  void OnFrontConnected();

  void OnFrontDisconnected(int nReason);

  void OnRspError(CXeleFtdcRspInfoField *pRspInfo,
                        int nRequestID,
                        bool bIsLast);

  void OnRspUserLogin(CXeleFtdcRspUserLoginField *pRspUserLogin,
                            CXeleFtdcRspInfoField *pRspInfo,
                            int nRequestID,
                            bool bIsLast);

  void OnRspUserLogout(CXeleFtdcRspUserLogoutField *pRspUserLogout,
                             CXeleFtdcRspInfoField *pRspInfo,
                             int nRequestID,
                             bool bIsLast);

  void OnRspOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                              CXeleFtdcRspInfoField *pRspInfo,
                              int nRequestID,
                              bool bIsLast);

  void OnRspOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                              CXeleFtdcRspInfoField *pRspInfo,
                              int nRequestID,
                              bool bIsLast);

  void OnRspQryClientPosition(CXeleFtdcRspClientPositionField *pRspClientPosition,
                                    CXeleFtdcRspInfoField *pRspInfo,
                                    int nRequestID,
                                    bool bIsLast);


  void OnRspQryInstrument(CXeleFtdcRspInstrumentField *pRspInstrument,
                                CXeleFtdcRspInfoField *pRspInfo,
                                int nRequestID,
                                bool bIsLast);

  void OnRspQryClientAccount(CXeleFtdcRspClientAccountField *pClientAccount,
                                   CXeleFtdcRspInfoField *pRspInfo,
                                   int nRequestID,
                                   bool bIsLast);

  void OnRtnTrade(CXeleFtdcTradeField *pTrade);

  void OnRtnOrder(CXeleFtdcOrderField *pOrder);

  void OnRtnInsInstrument(CXeleFtdcInstrumentField *pInstrument);

  void OnErrRtnOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                                 CXeleFtdcRspInfoField *pRspInfo);

  void OnErrRtnOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                                 CXeleFtdcRspInfoField *pRspInfo);

  void OnRspQryOrder(CXeleFtdcOrderField* pOrderField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

  void OnRspQryTrade(CXeleFtdcTradeField* pTradeField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

public:
  void Loop();
  int ShowMenu();
  void LogOut();
  void OpenOrder();
  void Withdraw();

  void QryInstrument();
  void QryOrder();
  void QryTrade();
  void QryInvestorPosition();
  void QryTradingAccount();

  void PrintTrade(CXeleFtdcTradeField *pTrade);
  void PrintOrder(CXeleFtdcOrderField *pOrder);

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_LoginId;
  char* m_OldPasswd;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;
};

#endif //_XELE_TEST_H_

