
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"

#include "CtpMduserHandler.h"

CCtpMduserHandler::CCtpMduserHandler(ctp_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{

}

// ���ͻ��������鷢��������������ͨ�����ӣ��ͻ�����Ҫ���е�¼
void CCtpMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// ���ͻ��������鷢��������ͨ�����ӶϿ�ʱ���÷���������
void CCtpMduserHandler::OnFrontDisconnected(int nReason) 
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
void CCtpMduserHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///�û��˳�Ӧ��
void CCtpMduserHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

// �������֪ͨ�����������������֪ͨ�ͻ���
void CCtpMduserHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pMarketData) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketData);
}

// ����û�����ĳ���֪ͨ
void CCtpMduserHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CCtpMduserHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CCtpMduserHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}


