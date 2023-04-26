#ifndef _FEMAS_TRADER_HANDLER_H_
#define _FEMAS_TRADER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct femas_trader_api_cb_def femas_trader_api_cb;

struct femas_trader_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspError)(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspUserLogout)(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspOrderInsert)(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspOrderAction)(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnTrade)(void* arg, CUstpFtdcTradeField *pTrade);
  void (*xOnRtnOrder)(void* arg, CUstpFtdcOrderField *pOrder);
  void (*xOnErrRtnOrderInsert)(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);
  void (*xOnErrRtnOrderAction)(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);
  void (*xOnRspQryUserInvestor)(void* arg, CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspQryInstrument)(void* arg, CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnInstrumentStatus)(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus);
	void (*xOnRspQryInvestorAccount)(void* arg, CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
	void (*xOnRspQryInvestorPosition)(void* arg, CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspDSUserCertification)(void* arg, CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
};

#ifdef __cplusplus
}
#endif

class CFemasTraderHandler : public CUstpFtdcTraderSpi
{
public:
  CFemasTraderHandler(femas_trader_api_cb* cb, void* arg);
  ~CFemasTraderHandler();
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

  ///错误应答
  void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///风控前置系统用户登录应答
  void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///用户退出应答
  void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

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

  ///可用投资者账户查询应答
  void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者资金账户查询应答
  void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///合约查询应答
  void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者持仓查询应答
  void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///穿透监管客户认证应答
  void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
  femas_trader_api_cb* m_TraderCb;
  void* m_Arg;

};

#endif

