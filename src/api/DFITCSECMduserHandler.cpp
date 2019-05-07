
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#include "DFITCSECMduserHandler.h"

CDfitcSecMduserHandler::CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CDfitcSecMduserHandler::~CDfitcSecMduserHandler() 
{
}

/** 
* ��������������Ӧ
*/
void CDfitcSecMduserHandler::OnFrontConnected(){
m_pUserCb->xOnFrontConnected(m_parent);
}

/**
* �������Ӳ�������Ӧ
*/
void CDfitcSecMduserHandler::OnFrontDisconnected(int nReason){
m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

/**
* SEC-��Ϣ֪ͨ
*/
void CDfitcSecMduserHandler::OnRtnNotice(DFITCSECRspNoticeField *pNotice) {
m_pUserCb->xOnRtnNotice(m_parent, pNotice);
}

/**
* ERR-����Ӧ��
* @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
*/
void CDfitcSecMduserHandler::OnRspError(DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspError(m_parent, pRspInfo);
}

/**
* STOCK-��¼��Ӧ
* @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
* @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
*/
void CDfitcSecMduserHandler::OnRspStockUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* STOCK-�ǳ���Ӧ
* @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
*/
void CDfitcSecMduserHandler::OnRspStockUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* SOP-��¼��Ӧ
* @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
*/
void CDfitcSecMduserHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* SOP-�ǳ���Ӧ
* @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
*/
void CDfitcSecMduserHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* FASL-��¼��Ӧ
* @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
*/
void CDfitcSecMduserHandler::OnRspFASLUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspFASLUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* FASL-�ǳ���Ӧ
* @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
*/
void CDfitcSecMduserHandler::OnRspFASLUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspFASLUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* STOCK-���鶩����Ӧ
* @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
*/
void CDfitcSecMduserHandler::OnRspStockSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* STOCK-ȡ������������Ӧ
* @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
*/
void CDfitcSecMduserHandler::OnRspStockUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUnSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* SOP-���鶩����Ӧ
* @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
*/
void CDfitcSecMduserHandler::OnRspSOPSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* SOP-ȡ������������Ӧ
* @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
*/
void CDfitcSecMduserHandler::OnRspSOPUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUnSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* STOCK-����������Ӧ
* @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
*/
void CDfitcSecMduserHandler::OnStockMarketData(DFITCStockDepthMarketDataField * pMarketDataField) {
m_pUserCb->xOnStockMarketData(m_parent, pMarketDataField);
}

/**
* SOP-����������Ӧ
* @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
*/
void CDfitcSecMduserHandler::OnSOPMarketData(DFITCSOPDepthMarketDataField * pMarketDataField) {
m_pUserCb->xOnSOPMarketData(m_parent, pMarketDataField);
}

/**
* STOCK-����������Ӧ
* @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
* @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
*/
void CDfitcSecMduserHandler::OnRspStockAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, bool flag) {
m_pUserCb->xOnRspStockAvailableQuot(m_parent, pAvailableQuotInfo, pRspInfo, flag);
}

/**
* SOP-����������Ӧ
* @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
* @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
*/
void CDfitcSecMduserHandler::OnRspSopAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, bool flag) {
m_pUserCb->xOnRspSopAvailableQuot(m_parent, pAvailableQuotInfo, pRspInfo, flag);
}

/**
* �������������Ӧ
* @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
*/
void CDfitcSecMduserHandler::OnRspUserMDPasswordUpdate(DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo){
m_pUserCb->xOnRspUserMDPasswordUpdate(m_parent, pMDPasswordUpdate, pRspInfo);
}

