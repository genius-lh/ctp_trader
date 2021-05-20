
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

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CFemasAFTraderHandler::OnFrontConnected()
{
	m_TraderCb->xOnFrontConnected(m_Arg);
}

void CFemasAFTraderHandler::OnQryFrontConnected()
{
	m_TraderCb->xOnQryFrontConnected(m_Arg);
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
void CFemasAFTraderHandler::OnFrontDisconnected(int nReason)
{
	m_TraderCb->xOnFrontDisconnected(m_Arg, nReason);
}

void CFemasAFTraderHandler::OnQryFrontDisconnected(int nReason)
{
	m_TraderCb->xOnQryFrontDisconnected(m_Arg, nReason);
}

///����Ӧ��
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

///�û��˳�Ӧ��
void CFemasAFTraderHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspUserLogout(m_Arg, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

///����¼��Ӧ��
void CFemasAFTraderHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderInsert(m_Arg, pInputOrder, pRspInfo, nRequestID, bIsLast);
}

///��������Ӧ��
void CFemasAFTraderHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspOrderAction(m_Arg, pOrderAction, pRspInfo, nRequestID, bIsLast);
}

///�ɽ��ر�
void CFemasAFTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
	m_TraderCb->xOnRtnTrade(m_Arg, pTrade);
}

///�����ر�
void CFemasAFTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
	m_TraderCb->xOnRtnOrder(m_Arg, pOrder);
}

///����¼�����ر�
void CFemasAFTraderHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderInsert(m_Arg, pInputOrder, pRspInfo);
}

///������������ر�
void CFemasAFTraderHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
	m_TraderCb->xOnErrRtnOrderAction(m_Arg, pOrderAction, pRspInfo);
}

///����Ͷ�����˻���ѯӦ��
void CFemasAFTraderHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_TraderCb->xOnRspQryUserInvestor(m_Arg, pRspUserInvestor, pRspInfo, nRequestID, bIsLast);
}

///��Լ��ѯӦ��
void CFemasAFTraderHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(pRspInstrument){
    string symbol = pRspInstrument->InstrumentID;
    string exchangeID = pRspInstrument->ExchangeID;
    mapSymbol.insert(map<string, string>::value_type(symbol, exchangeID));
  }

  m_TraderCb->xOnRspQryInstrument(m_Arg, pRspInstrument, pRspInfo, nRequestID, bIsLast);
}

///��Լ����״̬֪ͨ
void CFemasAFTraderHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) 
{
  m_TraderCb->xOnRtnInstrumentStatus(m_Arg, pInstrumentStatus);
}

///Ͷ�����ʽ��˻���ѯӦ��
void CFemasAFTraderHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorAccount(m_Arg, pRspInvestorAccount, pRspInfo, nRequestID, bIsLast);
}


///Ͷ���ֲֲ߳�ѯӦ��
void CFemasAFTraderHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  m_TraderCb->xOnRspQryInvestorPosition(m_Arg, pRspInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

///��͸��ܿͻ���֤Ӧ��
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




