
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <map>
using namespace std;

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#include "FemasAFTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "cmn_log.h"

#ifdef __cplusplus
}
#endif

CFemasAFTraderHandler::CFemasAFTraderHandler(femas_af_trader_api_cb* cb, void* arg)
:m_TraderCb(cb), m_Arg(arg)
{

}

CFemasAFTraderHandler::~CFemasAFTraderHandler()
{

}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CFemasAFTraderHandler::OnFrontConnected()
{
	m_TraderCb->xOnFrontConnected(m_Arg);
}

void CFemasAFTraderHandler::OnQryFrontConnected()
{
	m_TraderCb->xOnQryFrontConnected(m_Arg);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
void CFemasAFTraderHandler::OnFrontDisconnected(int nReason)
{
	m_TraderCb->xOnFrontDisconnected(m_Arg, nReason);
}

void CFemasAFTraderHandler::OnQryFrontDisconnected(int nReason)
{
	m_TraderCb->xOnQryFrontDisconnected(m_Arg, nReason);
}

///错误应答
void CFemasAFTraderHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_TraderCb->xOnRspError(m_Arg, pRspInfo, nRequestID, bIsLast);
}

void CFemasAFTraderHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  m_TraderCb->xOnRspUserLogin(m_Arg, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

void CFemasAFTraderHandler::OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspQueryUserLogin(m_Arg, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///用户退出应答
void CFemasAFTraderHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspUserLogout(m_Arg, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

///报单录入应答
void CFemasAFTraderHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderInsert(m_Arg, pInputOrder, pRspInfo, nRequestID, bIsLast);
}

///报单操作应答
void CFemasAFTraderHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderAction(m_Arg, pOrderAction, pRspInfo, nRequestID, bIsLast);
}

///成交回报
void CFemasAFTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
	m_TraderCb->xOnRtnTrade(m_Arg, pTrade);
}

///报单回报
void CFemasAFTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
	m_TraderCb->xOnRtnOrder(m_Arg, pOrder);
}

///报单录入错误回报
void CFemasAFTraderHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderInsert(m_Arg, pInputOrder, pRspInfo);
}

///报单操作错误回报
void CFemasAFTraderHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderAction(m_Arg, pOrderAction, pRspInfo);
}

///可用投资者账户查询应答
void CFemasAFTraderHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspQryUserInvestor(m_Arg, pRspUserInvestor, pRspInfo, nRequestID, bIsLast);
}

///合约查询应答
void CFemasAFTraderHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(pRspInstrument){
    string symbol = pRspInstrument->InstrumentID;
    string exchangeID = pRspInstrument->ExchangeID;
    mapSymbol.insert(map<string, string>::value_type(symbol, exchangeID));
  }

  m_TraderCb->xOnRspQryInstrument(m_Arg, pRspInstrument, pRspInfo, nRequestID, bIsLast);
}

///合约交易状态通知
void CFemasAFTraderHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) 
{
  m_TraderCb->xOnRtnInstrumentStatus(m_Arg, pInstrumentStatus);
}

///投资者资金账户查询应答
void CFemasAFTraderHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorAccount(m_Arg, pRspInvestorAccount, pRspInfo, nRequestID, bIsLast);
}


///投资者持仓查询应答
void CFemasAFTraderHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorPosition(m_Arg, pRspInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

///穿透监管客户认证应答
void CFemasAFTraderHandler::OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspDSUserCertification(m_Arg, pDSUserCertRspData, pRspInfo, nRequestID, bIsLast);
}

const char* CFemasAFTraderHandler::GetExchangeID(const char* instrument_id)
{
  string symbol = instrument_id;
  map<string, string>::iterator iter = mapSymbol.find(symbol);

  if(iter == mapSymbol.end()){
    CMN_ERROR("find symbol failed(%s)\n", instrument_id);
    return "SHFE";
  }

  return iter->second.c_str();
}




