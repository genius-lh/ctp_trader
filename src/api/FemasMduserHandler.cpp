
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

#include "FemasMduserHandler.h"

extern char *INI_FILE_NAME;

CFemasMduserHandler::CFemasMduserHandler(femas_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{

}

CFemasMduserHandler::~CFemasMduserHandler() 
{

}

// ���ͻ��������鷢��������������ͨ�����ӣ��ͻ�����Ҫ���е�¼
void CFemasMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// ���ͻ��������鷢��������ͨ�����ӶϿ�ʱ���÷���������
void CFemasMduserHandler::OnFrontDisconnected(int nReason) 
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
void CFemasMduserHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///�û��˳�Ӧ��
void CFemasMduserHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

// �������֪ͨ�����������������֪ͨ�ͻ���
void CFemasMduserHandler::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketData);
}

// ����û�����ĳ���֪ͨ
void CFemasMduserHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CFemasMduserHandler::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CFemasMduserHandler::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

