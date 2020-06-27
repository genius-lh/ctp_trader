#ifndef _XELE_TRADER_HANDLER_H_
#define _XELE_TRADER_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CXeleTraderApi.hpp"

class CXeleTraderHandler : public CXeleTraderSpi
{
public:
    CXeleTraderHandler(void* arg);
    
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
    
    void PrintTrade(CXeleFtdcTradeField *pTrade);
    void PrintOrder(CXeleFtdcOrderField *pOrder);

private:
  void* m_Arg;

};

#endif

