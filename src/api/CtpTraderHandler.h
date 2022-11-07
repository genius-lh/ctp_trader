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
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  void OnFrontConnected();
  
  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  void OnFrontDisconnected(int nReason);
    
  ///客户端认证响应
  void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登录请求响应
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登出请求响应
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单录入请求响应
  void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单操作请求响应
  void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者结算结果确认响应
  void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询投资者持仓响应
  void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询资金账户响应
  void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询投资者响应
  void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询合约响应
  void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///错误应答
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单通知
  void OnRtnOrder(CThostFtdcOrderField *pOrder);

  ///成交通知
  void OnRtnTrade(CThostFtdcTradeField *pTrade);

  ///报单录入错误回报
  void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

  ///报单操作错误回报
  void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

  ///合约交易状态通知
  void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

  ///交易所公告通知
  virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {};

private:
  ctp_trader_api_cb* m_TraderCb;
  void* m_Arg;

};


#endif //_CTP_TRADER_HANDLER_H_
