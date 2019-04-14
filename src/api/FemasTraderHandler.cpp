
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#include "FemasTraderHandler.h"

CFemasTraderHandler::CFemasTraderHandler(femas_trader_api_cb* cb, void* arg)
:m_TraderCb(cb), m_Arg(arg)
{

}

CFemasTraderHandler::~CFemasTraderHandler()
{

}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CFemasTraderHandler::OnFrontConnected()
{
	m_TraderCb->xOnFrontConnected(m_Arg);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
void CFemasTraderHandler::OnFrontDisconnected(int nReason)
{
	m_TraderCb->xOnFrontDisconnected(m_Arg, nReason);
}

///错误应答
void CFemasTraderHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_TraderCb->xOnRspError(m_Arg, pRspInfo, nRequestID, bIsLast);
}

///风控前置系统用户登录应答
void CFemasTraderHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_TraderCb->xOnRspUserLogin(m_Arg, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///用户退出应答
void CFemasTraderHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspUserLogout(m_Arg, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

///报单录入应答
void CFemasTraderHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderInsert(m_Arg, pInputOrder, pRspInfo, nRequestID, bIsLast);
}

///报单操作应答
void CFemasTraderHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderAction(m_Arg, pOrderAction, pRspInfo, nRequestID, bIsLast);
}

///成交回报
void CFemasTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
	m_TraderCb->xOnRtnTrade(m_Arg, pTrade);
}

///报单回报
void CFemasTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
	m_TraderCb->xOnRtnOrder(m_Arg, pOrder);
}

///报单录入错误回报
void CFemasTraderHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderInsert(m_Arg, pInputOrder, pRspInfo);
}

///报单操作错误回报
void CFemasTraderHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderAction(m_Arg, pOrderAction, pRspInfo);
}

///可用投资者账户查询应答
void CFemasTraderHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspQryUserInvestor(m_Arg, pRspUserInvestor, pRspInfo, nRequestID, bIsLast);
}

///合约查询应答
void CFemasTraderHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInstrument(m_Arg, pRspInstrument, pRspInfo, nRequestID, bIsLast);
}

///合约交易状态通知
void CFemasTraderHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) 
{
  m_TraderCb->xOnRtnInstrumentStatus(m_Arg, pInstrumentStatus);
}

///投资者资金账户查询应答
void CFemasTraderHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorAccount(m_Arg, pRspInvestorAccount, pRspInfo, nRequestID, bIsLast);
}


///投资者持仓查询应答
void CFemasTraderHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorPosition(m_Arg, pRspInvestorPosition, pRspInfo, nRequestID, bIsLast);
}





