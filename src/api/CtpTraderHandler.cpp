
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"

#include "CtpTraderHandler.h"


CCtpTraderHandler::CCtpTraderHandler(ctp_trader_api_cb* cb, void* arg)
:m_TraderCb(cb), m_Arg(arg)
{

}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CCtpTraderHandler::OnFrontConnected()
{
	m_TraderCb->xOnFrontConnected(m_Arg);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
void CCtpTraderHandler::OnFrontDisconnected(int nReason)
{
	m_TraderCb->xOnFrontDisconnected(m_Arg, nReason);
}

///错误应答
void CCtpTraderHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_TraderCb->xOnRspError(m_Arg, pRspInfo, nRequestID, bIsLast);
}

///投资者结算结果确认响应
void CCtpTraderHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspSettlementInfoConfirm(m_Arg, pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);

}


///风控前置系统用户登录应答
void CCtpTraderHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_TraderCb->xOnRspUserLogin(m_Arg, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///用户退出应答
void CCtpTraderHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspUserLogout(m_Arg, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

///报单录入应答
void CCtpTraderHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderInsert(m_Arg, pInputOrder, pRspInfo, nRequestID, bIsLast);
}

///报单操作应答
void CCtpTraderHandler::OnRspOrderAction(CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderAction(m_Arg, pOrderAction, pRspInfo, nRequestID, bIsLast);
}

///成交回报
void CCtpTraderHandler::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_TraderCb->xOnRtnTrade(m_Arg, pTrade);
}

///报单回报
void CCtpTraderHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	m_TraderCb->xOnRtnOrder(m_Arg, pOrder);
}

///报单录入错误回报
void CCtpTraderHandler::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderInsert(m_Arg, pInputOrder, pRspInfo);
}

///报单操作错误回报
void CCtpTraderHandler::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderAction(m_Arg, pOrderAction, pRspInfo);
}

///可用投资者账户查询应答
void CCtpTraderHandler::OnRspQryInvestor(CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspQryUserInvestor(m_Arg, pRspUserInvestor, pRspInfo, nRequestID, bIsLast);
}

///请求查询投资者持仓响应
void CCtpTraderHandler::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorPosition(m_Arg, pInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

void CCtpTraderHandler::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryTradingAccount(m_Arg, pTradingAccount, pRspInfo, nRequestID, bIsLast);
}


///合约查询应答
void CCtpTraderHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInstrument(m_Arg, pRspInstrument, pRspInfo, nRequestID, bIsLast);
}

void CCtpTraderHandler::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
  m_TraderCb->xOnRtnInstrumentStatus(m_Arg, pInstrumentStatus);
}





