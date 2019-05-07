/**
* ��Ȩ����(C)2012-2016, �����ɴ���Ϣ�������޹�˾
* �ļ����ƣ�DFITCSECTraderApi.h
* �ļ�˵��������ӿ���������ݽӿ�
* ��ǰ�汾��1.6.73
* ���ߣ�XSpeed֤ȯ��Ŀ��
* �������ڣ�2016��09��
*/
#ifndef DFITCSECTRADERAPI_H_
#define DFITCSECTRADERAPI_H_

#include "DFITCSECApiStruct.h"

#ifdef WIN32
#ifndef AFX_EXT_CLASS
#define AFX_EXT_CLASS       _declspec(dllexport)
#endif

#ifdef DFITCSECAPI_EXPORTS
#define DFITCSEC_TRADER_API __declspec(dllexport)
#else 
#define DFITCSEC_TRADER_API __declspec(dllimport)
#endif
#else 
#define DFITCSEC_TRADER_API 
#endif

class DFITCSECTraderSpi
{
public:
    /**
     * SEC-��������������Ӧ
     */
    virtual void OnFrontConnected(){};
    /**
     * SEC-�������Ӳ�������Ӧ
     */
    virtual void OnFrontDisconnected(int nReason) {};
    /**
     * SEC-��Ϣ֪ͨ
     */
    virtual void OnRtnNotice(DFITCSECRspNoticeField *pNotice) {};
    /**
    * ERR-����Ӧ��
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ַ
    */
    virtual void OnRspError(DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-��¼��Ӧ
    * @param pData:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ĵ�ַ,������¼����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ��������¼����ʧ��
    */
    virtual void OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-�ǳ���Ӧ
    * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ĵ�ַ,�����ǳ�����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ǳ�����ʧ��
    */
    virtual void OnRspStockUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-���������Ӧ
    * @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ĵ�ַ,���������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������������ʧ��
    */
    virtual void OnRspStockUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-ί�б�����Ӧ
    * @param pData:ָ�����ǿ�,�����û�ί�б�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�б�������ʧ��
    */
    virtual void OnRspStockEntrustOrder(DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-ί�г�����Ӧ
    * @param pData:ָ�����ǿ�,�����û�ί�г�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�г�������ʧ��
    */
    virtual void OnRspStockWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-ί�в�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,������ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�в�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryEntrustOrder(DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-ʵʱ�ɽ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����ʵʱ�ɽ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ʵʱ�ɽ���ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-�ֱʳɽ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֱʳɽ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֱʳɽ���ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQrySerialTrade(DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-�ֲֲ�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֲֲ�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֲֲ�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-�ʽ��˺Ų�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ʽ��˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ʽ��˺Ų�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��˺Ų�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-�����˺Ų�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û������˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������˺Ų�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������˺Ų�ѯ����ʧ��
    */
    virtual void OnRspStockQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {}; 
    /**
    * STOCK-�ɶ��Ų�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ɶ��Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ɶ��Ų�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ɶ��Ų�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {}; 
    /**
    * STOCK-�ʽ������Ӧ
    * @param pData:ָ�����ǿ�,�����û��ʽ������Ӧ��Ϣ�ṹ��ĵ�ַ,�������ʽ��������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��������ʧ��
    */
    virtual void OnRspStockTransferFunds(DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-����ί����Ӧ
    * @param pData:ָ�����ǿ�,�����û�����ί����Ӧ��Ϣ�ṹ��ĵ�ַ,��������ί������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ί������ʧ��
    */
    virtual void OnRspStockEntrustBatchOrder(DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-����������Ӧ
    * @param pData:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ĵ�ַ,����������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������������������ʧ��
    */
    virtual void OnRspStockWithdrawBatchOrder(DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-�����ί��������Ӧ
    * @param pData:ָ�����ǿ�,�����û������ί��������Ӧ��Ϣ�ṹ��ĵ�ַ,���������ί����������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ί����������ʧ��
    */
    virtual void OnRspStockCalcAbleEntrustQty(DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {}; 
    /**
    * STOCK-�����깺ETF������Ӧ
    * @param pData:ָ�����ǿ�,�����û������깺ETF������Ӧ��Ϣ�ṹ��ĵ�ַ,���������깺ETF��������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������깺ETF��������ʧ��
    */
    virtual void OnRspStockCalcAblePurchaseETFQty(DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-�����ʽ���ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {}; 
    /**
    * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryTransferStockDetail(DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-�����ʽ���ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-֤ȯ��Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�֤ȯ��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryStockInfo(DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-֤ȯ��̬��Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�֤ȯ��̬��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��̬��Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��̬��Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspStockQryStockStaticInfo(DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * STOCK-����ʱ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�����ʱ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������ʱ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ʱ���ѯ����ʧ��
    */
    virtual void OnRspStockQryTradeTime(DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * STOCK-ί�лر���Ӧ
    * @param pData:����ί�лر��ṹ��ĵ�ַ
    */
    virtual void OnStockEntrustOrderRtn(DFITCStockEntrustOrderRtnField * pData){};
    /**
    * STOCK-�ɽ��ر���Ӧ
    * @param pData:���سɽ��ر��ṹ��ĵ�ַ
    */
    virtual void OnStockTradeRtn(DFITCStockTradeRtnField * pData){};
    /**
    * STOCK-�����ر���Ӧ
    * @param pData:���س����ر��ṹ��ĵ�ַ
    */
    virtual void OnStockWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField * pData){};
    
    /**
    * SOP-��¼��Ӧ
    * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
    */
    virtual void OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
     * SOP-�ǳ���Ӧ
     * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
     * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
     */
    virtual void OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-�û����������Ӧ
    * @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ַ,�����û������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������û������������ʧ��
    */
    virtual void OnRspSOPUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-������Ӧ
    * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
    */
    virtual void OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-��ϲ��ί����Ӧ
    * @param pData:ָ�����ǿ�,�����û���Ӧ��Ϣ�ṹ��ַ,������ϲ��ί������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ϲ��ί������ʧ��
    */
    virtual void OnRspSOPGroupSplit(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-��ѯ�ͻ���ϳֲ���ϸ��Ӧ
    * @param pData:ָ�����ǿ�,�����û���ѯ�ͻ���ϳֲ���ϸ��Ӧ�ṹ��ַ,������ѯ�ͻ���ϳֲ���ϸ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ѯ�ͻ���ϳֲ���ϸ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryGroupPosition(DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-֤ȯ����������Ӧ
    * @param pData:ָ�����ǿ�,�����û�֤ȯ����������Ӧ��Ϣ�ṹ��ַ,����֤ȯ������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������֤ȯ������������ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPLockOUnLockStock(DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {}; 
    /**
    * SOP-������Ӧ
    * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
    */
    virtual void OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-ί�в�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,����ί�в�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ί�в�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-�ֱʳɽ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֱʳɽ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֱʳɽ���ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQrySerialTrade(DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-�ֲֲ�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֲֲ�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֲֲ�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-�ͻ������ֲֲ�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ������ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ֲֲ�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ֲֲ�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryCollateralPosition(DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-�ͻ��ʽ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
    */
    virtual void OnRspSOPQryCapitalAccountInfo(DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-�ͻ���Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
    */
    virtual void OnRspSOPQryAccountInfo(DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-�ɶ���Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
    */
    virtual void OnRspSOPQryShareholderInfo(DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo) {}; 
    /**
    * SOP-��ί��������ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û���ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ί��������ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ί��������ѯ����ʧ��
    */
    virtual void OnRspSOPCalcAbleEntrustQty(DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-�ͻ�������֤ȯ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ�������֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�������֤ȯ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�������֤ȯ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryAbleLockStock(DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {}; 
    /**
    * SOP-��Ȩ��Լ�����ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û���Ȩ��Լ�����ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ��Լ�����ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ��Լ�����ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {}; 
    /**
    * SOP-ִ��ί����Ӧ
    * @param pData:ָ�����ǿ�,�����û�ִ��ί����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ִ��ί������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ִ��ί������ʧ��
    */
    virtual void OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * SOP-�ͻ���Ȩָ����Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ���Ȩָ����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ȩָ����Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ȩָ����Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryExecAssiInfo(DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-��ѯ����ʱ����Ӧ
    * @param pData:ָ�����ǿ�,�����û���ѯ����ʱ����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ����ʱ������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ����ʱ������ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryTradeTime(DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-��ȡ���н�����������Ӧ
    * @param pData:ָ�����ǿ�,�����û���ȡ���н�����������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ���н�������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ���н�������������ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryExchangeInfo(DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-��ѯ�����Ѳ�����Ӧ
    * @param pData:ָ�����ǿ�,�����û���ѯ�����Ѳ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����Ѳ�������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����Ѳ�������ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryCommission(DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-��ѯ��֤���ʲ�����Ӧ
    * @param pData:ָ�����ǿ�,�����û���ѯ��֤���ʲ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ��֤���ʲ�������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ��֤���ʲ�������ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryDeposit(DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-��Ȩ�����Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û���Ȩ�����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ�����Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ�����Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspSOPQryContractObjectInfo(DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * SOP-ί�лر���Ӧ
    * @param pData:����ί�лر��ṹ��ĵ�ַ
    */
    virtual void OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData){};
    /**
    * SOP-�ɽ��ر���Ӧ
    * @param pData:���سɽ��ر��ṹ��ĵ�ַ
    */
    virtual void OnSOPTradeRtn(DFITCSOPTradeRtnField * pData){};
    /**
    * SOP-�����ر���Ӧ
    * @param pData:���س����ر��ṹ��ĵ�ַ
    */
    virtual void OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData){};

    /**
    * FASL-��¼��Ӧ
    * @param pData:ָ�����ǿ�,�����û�������ȯ��¼��Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��¼����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��¼����ʧ��
    */
    virtual void OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-�ǳ���Ӧ
    * @param pData:ָ�����ǿ�,�����û�������ȯ�ǳ���Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ�ǳ�����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ�ǳ�����ʧ��
    */
    virtual void OnRspFASLUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-�ͻ���������Ϣ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ���������Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���������Ϣ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���������Ϣ����ʧ��
    */
    virtual void OnRspFASLQryAbleFinInfo(DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-�ͻ�����ȯ��Ϣ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ�����ȯ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�����ȯ��Ϣ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�����ȯ��Ϣ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryAbleSloInfo(DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�����ﻮת��Ӧ
    * @param pData:ָ�����ǿ�,�����û������ﻮת��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ﻮת����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ﻮת����ʧ��
    */
    virtual void OnRspFASLTransferCollateral(DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-ֱ�ӻ�����Ӧ
    * @param pData:ָ�����ǿ�,�����û�ֱ�ӻ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ֱ�ӻ�������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ֱ�ӻ�������ʧ��
    */
    virtual void OnRspFASLDirectRepayment(DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-��ȯ��ת��Ӧ
    * @param pData:ָ�����ǿ�,�����û���ȯ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȯ��ת����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȯ��ת����ʧ��
    */
    virtual void OnRspFASLRepayStockTransfer(DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-���ý�����Ӧ
    * @param pData:ָ�����ǿ�,�����û����ý�����Ӧ��Ϣ�ṹ��ַ,�����ͻ����ý�������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ý�������ʧ��
    */
    virtual void OnRspFASLEntrustCrdtOrder(DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-������ȯ������Ӧ
    * @param pData:ָ�����ǿ�,�����û�������ȯ������Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��������ʧ��
    */
    virtual void OnRspFASLEntrustOrder(DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-���ÿ�ί��������ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û����ÿ�ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ÿ�ί��������ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ÿ�ί��������ѯ����ʧ��
    */
    virtual void OnRspFASLCalcAbleEntrustCrdtQty(DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-��ѯ�����ʽ���Ӧ
    * @param pData:ָ�����ǿ�,�����û���ѯ�����ʽ���Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����ʽ�����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����ʽ�����ʧ��
    */
    virtual void OnRspFASLQryCrdtFunds(DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-���ú�Լ��Ϣ��Ӧ
    * @param pData:ָ�����ǿ�,�����û����ú�Լ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ��Ϣ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ��Ϣ����ʧ��
    */
    virtual void OnRspFASLQryCrdtContract(DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLat) {};
    /**
    * FASL-���ú�Լ�䶯��Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û����ú�Լ�䶯��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ�䶯��Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ�䶯��Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryCrdtConChangeInfo(DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ʽ��ת��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ʽ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ��ת����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ��ת����ʧ��
    */
    virtual void OnRspFASLTransferFunds(DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-�ͻ���Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
    */
    virtual void OnRspFASLQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-�ͻ��ʽ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ɶ���Ϣ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ֲֲ�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֲֲ�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֲֲ�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-ί�в�ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ί�в�ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ί�в�ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryEntrustOrder(DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ֱʳɽ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֱʳɽ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֱʳɽ���ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQrySerialTrade(DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-ʵʱ�ɽ���ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ʵʱ�ɽ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ʵʱ�ɽ���ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ʽ𶳽���ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ʽ𶳽���ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ𶳽���ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ𶳽���ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-֤ȯ������ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û�֤ȯ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�֤ȯ������ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�֤ȯ������ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ʽ������ϸ��ѯ��Ӧ
    * @param pData:ָ�����ǿ�,�����û��ʽ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ������ϸ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ������ϸ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-������Ӧ
    * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
    */
    virtual void OnRspFASLWithdrawOrder(DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-��ǰϵͳʱ���ѯ������Ӧ
    * @param pData:ָ�����ǿ�,�����û�ϵͳʱ���ѯ��Ӧ��Ϣ�ṹ��ַ,����ϵͳʱ���ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ϵͳʱ���ѯ����ʧ��
    */
    virtual void OnRspFASLQrySystemTime(DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-��ת�뵣��֤ȯ��ѯ������Ӧ
    * @param pData:ָ�����ǿ�,���ؿ�ת�뵣��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,������ת�뵣��֤ȯ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ת�뵣��֤ȯ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryTransferredContract(DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-�ͻ���ȡ�ʽ����������Ӧ
    * @param pData:ָ�����ǿ�,���ؿͻ���ȡ�ʽ������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ�ʽ��������ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ�ʽ��������ʧ��
    */
    virtual void OnRspFASLDesirableFundsOut(DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo) {};
    /**
    * FASL-����֤ȯ��ѯ������Ӧ
    * @param pData:ָ�����ǿ�,���ص���֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,��������֤ȯ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ����������֤ȯ��ѯ����ʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryGuaranteedContract(DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-���֤ȯ��ѯ������Ӧ
    * @param pData:ָ�����ǿ�,���ر��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�������֤ȯ��ѯ����ɹ�
    * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ���������֤ȯ��ѯʧ��
    * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
    */
    virtual void OnRspFASLQryUnderlyingContract(DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {};
    /**
    * FASL-ί�лر���Ӧ
    * @param pData:����ί�лر��ṹ��ĵ�ַ
    */
    virtual void OnFASLEntrustOrderRtn(DFITCStockEntrustOrderRtnField *pData){};
    /**
    * FASL-�ɽ��ر���Ӧ
    * @param pData:���سɽ��ر��ṹ��ĵ�ַ
    */
    virtual void OnFASLTradeRtn(DFITCStockTradeRtnField *pData){};
    /**
    * FASL-�����ر���Ӧ
    * @param pData:���س����ر��ṹ��ĵ�ַ
    */
    virtual void OnFASLWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField *pData){};
};



class DFITCSEC_TRADER_API DFITCSECTraderApi
{
public:
     /**
      * ����DFITCSECTraderApi�ӿڶ���
      * @ pszLogAddr log���ڵ�·�������pszLogAddressΪNULL��������log��
      * @ pszPriFlowDir ˽������¼���ڵ�·�������pszPriFlowDirΪNULL����Ĭ�Ͻ�˽������¼�ڵ�ǰĿ¼�¡�
      */
     static DFITCSECTraderApi *CreateDFITCSECTraderApi(const char* pszLogAddr = "", const char* pszPriFlowDir = "");
     /**
      * �ͷ�DFITCSECTraderApi�ӿڶ���
      */
     virtual void Release() = 0;
     /**
      * ��ʼ�� 
      * @param pszFrontAddress:ǰ�û���ַ
      *                  �����ַ�ĸ�ʽΪ:"protocol://ipaddress:port",��"tcp://127.0.0.1:10920"
      *                  ����protocol��ֵ����Ϊtcp��
      *                  ipaddress��ʾ����ǰ�õ�IP,port��ʾ����ǰ�õĶ˿�
      * @param pSpi:ָ��ص���������ָ��
      * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
      */
     virtual int Init(const char *pszFrontAddress, DFITCSECTraderSpi *pSpi) = 0;
     /**
      * ����˽����
      * @param nResumeType: ˽�����ش���ʽ
      *         TERT_RESTART:�ӱ������տ�ʼ�ش�
      *         TERT_RESUME:���ϴ��յ�������
      *         TERT_QUICK:ֻ���͵�¼��˽����������
      * @remark �÷���Ҫ��UserLogin����ǰ���á����������򲻻��յ�˽���������ݡ�
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int SubscribePrivateTopic(RESUME_TYPE nResumeType) = 0; 
     /**
      * STOCK-��¼����
      * @param p:ָ���û���¼����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockUserLogin(DFITCSECReqUserLoginField *p) = 0;    
     /**
      * STOCK-�ǳ�����
      * @param p:ָ���û��ǳ�����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockUserLogout(DFITCSECReqUserLogoutField *p) = 0;
     /**
      * STOCK-�����������
      * @param p:ָ���û������������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockUserPasswordUpdate(DFITCSECReqPasswordUpdateField *p) = 0;
     /**
      * STOCK-ί������
      * @param p:ָ���û�ί������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockEntrustOrder(DFITCStockReqEntrustOrderField *p) = 0;
     /**
      * STOCK-��������
      * @param p:ָ���û���������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockWithdrawOrder(DFITCSECReqWithdrawOrderField *p) = 0;
     /**
      * STOCK-ί�в�ѯ����
      * @param p:ָ���û�ί�в�ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryEntrustOrder(DFITCStockReqQryEntrustOrderField *p) = 0;
     /**
      * STOCK-ʵʱ�ɽ���ѯ����
      * @param p:ָ���û�ʵʱ�ɽ���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryRealTimeTrade(DFITCStockReqQryRealTimeTradeField *p) = 0;
     /**
      * STOCK-�ֱʳɽ���ѯ����
      * @param p:ָ���û��ֱʳɽ���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQrySerialTrade(DFITCStockReqQrySerialTradeField *p) = 0;
     /**
      * STOCK-�ֲֲ�ѯ����
      * @param p:ָ���û��ֲֲ�ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryPosition(DFITCStockReqQryPositionField *p) = 0;
     /**
      * STOCK-�ʽ��˻���ѯ����
      * @param p:ָ���û��ʽ��˻���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryCapitalAccountInfo(DFITCStockReqQryCapitalAccountField *p) = 0;
     /**
      * STOCK-�˻���ѯ����
      * @param p:ָ���û��˻���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryAccountInfo(DFITCStockReqQryAccountField *p) = 0;
     /**
      * STOCK-�ɶ���ѯ����
      * @param p:ָ���û��ɶ���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryShareholderInfo(DFITCStockReqQryShareholderField *p) = 0;
     /**
      * STOCK-�����ʽ�����
      * @param p:ָ���û������ʽ�����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockTransferFunds(DFITCStockReqTransferFundsField *p) = 0;
     /**
      * STOCK-����ί������
      * @param p:ָ���û�����ί������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockEntrustBatchOrder(DFITCStockReqEntrustBatchOrderField *p) = 0;
     /**
      * STOCK-������������
      * @param p:ָ���û�������������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockWithdrawBatchOrder(DFITCStockReqWithdrawBatchOrderField *p) = 0;
     /**
      * STOCK-�����ί����������
      * @param p:ָ���û������ί����������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockCalcAbleEntrustQty(DFITCStockReqCalcAbleEntrustQtyField *p) = 0;
     /**
      * STOCK-������깺ETF����������
      * @param p:ָ���û�������깺ETF����������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockCalcAblePurchaseETFQty(DFITCStockReqCalcAblePurchaseETFQtyField *p) = 0;
     /**
      * STOCK-�����ʽ���ϸ��ѯ����
      * @param p:ָ���û������ʽ���ϸ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryFreezeFundsDetail(DFITCStockReqQryFreezeFundsDetailField *p) = 0;
     /**
      * STOCK-����֤ȯ��ϸ��ѯ
      * @param p:ָ���û�����֤ȯ��ϸ��ѯ�ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryFreezeStockDetail(DFITCStockReqQryFreezeStockDetailField *p) = 0;
     /**
      * STOCK-�����ʽ���ϸ��ѯ����
      * @param p:ָ���û������ʽ���ϸ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryTransferFundsDetail(DFITCStockReqQryTransferFundsDetailField *p) = 0;
     /**
      * STOCK-����֤ȯ��ϸ��ѯ����
      * @param p:ָ���û�����֤ȯ��ϸ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryTransferStockDetail(DFITCStockReqQryTransferStockDetailField *p) = 0; 
     /**
      * STOCK-��Ʊ��ѯ����
      * @param p:ָ���û���Ʊ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryStockInfo(DFITCStockReqQryStockField *p) = 0;
     /**
      * STOCK-��Ʊ��̬��Ϣ��ѯ����
      * @param p:ָ���û���Ʊ��̬��Ϣ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryStockStaticInfo(DFITCStockReqQryStockStaticField *p) = 0;
     /**
      * STOCK-����ʱ���ѯ����
      * @param p:ָ���û�����ʱ���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqStockQryTradeTime(DFITCStockReqQryTradeTimeField *p) = 0;
     /**
      * SOP-��¼����
      * @param p:ָ���û���¼����ṹ�ĵ�ַ
      * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
      */
     virtual int ReqSOPUserLogin(DFITCSECReqUserLoginField *p) = 0;
     /**
      * SOP-�ǳ�����
      * @param p:ָ���û��ǳ�����ṹ�ĵ�ַ
      * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
      */
     virtual int ReqSOPUserLogout(DFITCSECReqUserLogoutField *p) = 0;
     /**
      * SOP-���������������
      * @param p:ָ���û������������ṹ�ĵ�ַ
      * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
      */
     virtual int ReqSOPUserPasswordUpdate(DFITCSECReqPasswordUpdateField *p) = 0;
    /**
     * SOP-��������
     * @param p:ָ���û���������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
     */
    virtual int ReqSOPEntrustOrder(DFITCSOPReqEntrustOrderField *p) = 0;    
    /**
     * SOP-�ֲ���ϲ��ί������
     * @param p:ָ���û��������ֲ���ϲ��ί������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
     */
    virtual int ReqSOPGroupSplit(DFITCSOPReqGroupSplitField *p) = 0;
    /**
     * SOP-��ѯ�ͻ���ϳֲ���ϸ����
     * @param p:ָ���û���ѯ�ͻ���ϳֲ���ϸ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
     */
    virtual int ReqSOPQryGroupPosition(DFITCSOPReqQryGroupPositionField *p) = 0;
    /**
     * SOP-֤ȯ������������
     * @param p:ָ���û�֤ȯ������������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPLockOUnLockStock(DFITCSOPReqLockOUnLockStockField *p) = 0;
    /**
     * SOP-��������
     * @param p:ָ���û���������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPWithdrawOrder(DFITCSECReqWithdrawOrderField *p) = 0;
    /**
     * SOP-����ί�в�ѯ����
     * @param p:ָ���û�����ί�в�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryEntrustOrder(DFITCSOPReqQryEntrustOrderField *p) = 0;
    /**
     * SOP-�ֱʳɽ���ѯ����
     * @param p:ָ���û��ֱʳɽ���ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQrySerialTrade(DFITCSOPReqQrySerialTradeField *p) = 0;
    /**
     * SOP-�ֲֲ�ѯ����
     * @param p:ָ���û��ֲֲ�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryPosition(DFITCSOPReqQryPositionField *p) = 0;
    /**
     * SOP-������ֲֲ�ѯ����
     * @param p:ָ���û�������ֲֲ�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryCollateralPosition(DFITCSOPReqQryCollateralPositionField *p) = 0;
    /**
     * SOP-�ʽ���Ϣ��ѯ����
     * @param p:ָ���û��ʽ���Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryCapitalAccountInfo(DFITCSOPReqQryCapitalAccountField *p) = 0;
    /**
     * SOP-�ͻ���Ϣ��ѯ����
     * @param p:ָ���û��ͻ���Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryAccountInfo(DFITCSOPReqQryAccountField *p) = 0;
    /**
     * SOP-�ɶ���Ϣ��ѯ����
     * @param p:ָ���û��ɶ���Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryShareholderInfo(DFITCSOPReqQryShareholderField *p) = 0;
    /**
     * SOP-��ί��������������
     * @param p:ָ���û���ί��������������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPCalcAbleEntrustQty(DFITCSOPReqCalcAbleEntrustQtyField *p) = 0;
    /**
     * SOP-������֤ȯ������ѯ����
     * @param p:ָ���û�������֤ȯ������ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryAbleLockStock(DFITCSOPReqQryAbleLockStockField *p) = 0;
    /**
     * SOP-��Ȩ��Լ�����ѯ����
     * @param p:ָ���û���Ȩ��Լ�����ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryContactInfo(DFITCSOPReqQryContactField *p) = 0;
    /**
     * SOP-��Ȩί������
     * @param p:ָ���û���Ȩί������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPExectueOrder(DFITCSOPReqExectueOrderField *p) = 0;
    /**
     * SOP-��Ȩָ����Ϣ��ѯ����
     * @param p:ָ���û���Ȩָ����Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryExecAssiInfo(DFITCSOPReqQryExecAssiInfoField *p) = 0;
    /**
     * SOP-����ʱ���ѯ����
     * @param p:ָ���û�����ʱ���ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryTradeTime(DFITCSOPReqQryTradeTimeField *p) = 0;
    /**
     * SOP-��������Ϣ��ѯ����
     * @param p:ָ���û���������Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryExchangeInfo(DFITCSOPReqQryExchangeInfoField *p) = 0;
    /**
     * SOP-�����Ѳ�ѯ����
     * @param p:ָ���û������Ѳ�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryCommission(DFITCSOPReqQryCommissionField *p) = 0;
    /**
     * SOP-��֤���ѯ����
     * @param p:ָ���û���֤���ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryDeposit(DFITCSOPReqQryDepositField *p) = 0;
    /**
     * SOP-�����Ϣ��ѯ����
     * @param p:ָ���û������Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqSOPQryContractObjectInfo(DFITCSOPReqQryContractObjectField *p) = 0;
    /**
     * FASL-��¼����
     * @param p:ָ���û���¼����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLUserLogin(DFITCSECReqUserLoginField *p) = 0;
    /**
     * FASL-�ǳ�����
     * @param p:ָ���û��ǳ�����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLUserLogout(DFITCSECReqUserLogoutField *p) = 0;
    /**
     * FASL-�ͻ���������Ϣ����
     * @param p:ָ���û��ͻ���������Ϣ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryAbleFinInfo(DFITCFASLReqAbleFinInfoField *p) = 0;
    /**
     * FASL-�ͻ�����ȯ��Ϣ����
     * @param p:ָ���û��ͻ�����ȯ��Ϣ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryAbleSloInfo(DFITCFASLReqAbleSloInfoField *p) = 0;
    /**
     * FASL-�����ﻮת����
     * @param p:ָ���û������ﻮת����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLTransferCollateral(DFITCFASLReqTransferCollateralField *p) = 0;
    /**
     * FASL-ֱ�ӻ�������
     * @param p:ָ���û�ֱ�ӻ�������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLDirectRepayment(DFITCFASLReqDirectRepaymentField *p) = 0;
    /**
     * FASL-��ȯ��ת����
     * @param p:ָ���û���ȯ��ת����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLRepayStockTransfer(DFITCFASLReqRepayStockTransferField *p) = 0;
    /**
     * FASL-���ý�������
     * @param p:ָ���û����ý�������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLEntrustCrdtOrder(DFITCFASLReqEntrustCrdtOrderField *p) = 0;
    /**
     * FASL-������ȯ��������
     * @param p:ָ���û�������ȯ��������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLEntrsuctOrder(DFITCFASLReqEntrustOrderField *p) = 0;
    /**
     * FASL-��������
     * @param p:ָ���û���������ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLWithdrawOrder(DFITCFASLReqWithdrawOrderField *p) = 0;
    /**
     * FASL-���ÿ�ί��������ѯ����
     * @param p:ָ���û����ÿ�ί��������ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLCalcAbleEntrustCrdtQty(DFITCFASLReqCalcAbleEntrustCrdtQtyField *p) = 0;
    /**
     * FASL-��ѯ�����ʽ�����
     * @param p:ָ���û���ѯ�����ʽ�����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryCrdtFunds(DFITCFASLReqQryCrdtFundsField *p) = 0;
    /**
     * FASL-���ú�Լ��Ϣ����
     * @param p:ָ���û����ú�Լ��Ϣ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryCrdtContract(DFITCFASLReqQryCrdtContractField *p) = 0;
    /**
     * FASL-���ú�Լ�䶯��Ϣ��ѯ����
     * @param p:ָ���û����ú�Լ�䶯��Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryCrdtConChangeInfo(DFITCFASLReqQryCrdtConChangeInfoField *p) = 0;
    /**
     * FASL-�ʽ��ת����
     * @param p:ָ���û��ʽ��ת����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLTransferFunds(DFITCStockReqTransferFundsField *p) = 0;
    /**
     * FASL-�ͻ���Ϣ��ѯ����
     * @param p:ָ���û��ͻ���Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryAccountInfo(DFITCStockReqQryAccountField *p) = 0;
    /**
     * FASL-�ͻ��ʽ��ѯ����
     * @param p:ָ���û��ͻ��ʽ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryCapitalAccountInfo(DFITCStockReqQryCapitalAccountField *p) = 0;
    /**
     * FASL-�ɶ���Ϣ��ѯ����
     * @param p:ָ���û��ɶ���Ϣ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryShareholderInfo(DFITCStockReqQryShareholderField *p) = 0;
    /**
     * FASL-�ֲֲ�ѯ����
     * @param p:ָ���û��ֲֲ�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryPosition(DFITCStockReqQryPositionField *p) = 0;
    /**
     * FASL-ί�в�ѯ����
     * @param p:ָ���û�ί�в�ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryEntrustOrder(DFITCStockReqQryEntrustOrderField *p) = 0;
    /**
     * FASL-�ֱʳɽ���ѯ����
     * @param p:ָ���û��ֱʳɽ���ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQrySerialTrade(DFITCStockReqQrySerialTradeField *p) = 0;
    /**
     * FASL-ʵʱ�ɽ���ѯ����
     * @param p:ָ���û�ʵʱ�ɽ���ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryRealTimeTrade(DFITCStockReqQryRealTimeTradeField *p) = 0;
    /**
     * FASL-�ʽ𶳽���ϸ��ѯ����
     * @param p:ָ���û��ʽ𶳽���ϸ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryFreezeFundsDetail(DFITCStockReqQryFreezeFundsDetailField *p) = 0;
    /**
     * FASL-֤ȯ������ϸ��ѯ����
     * @param p:ָ���û�֤ȯ������ϸ��ѯ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryFreezeStockDetail(DFITCStockReqQryFreezeStockDetailField *p) = 0;
    /**
     * FASL-��ѯ�ʽ������ϸ����
     * @param p:ָ���û���ѯ�ʽ������ϸ����ṹ�ĵ�ַ
     * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml  
     */
    virtual int ReqFASLQryTransferFundsDetail(DFITCStockReqQryTransferFundsDetailField *p) = 0;
    /**
      * FASL-��ǰϵͳʱ���ѯ����
      * @param p:ָ���û�����ʱ���ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqFASLQrySystemTime(DFITCFASLReqQryTradeTimeField *p) = 0;
    /**
      * FASL-��ת�뵣��֤ȯ��ѯ����
      * @param p:ָ���ת�뵣��֤ȯ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqFASLQryTransferredContract(DFITCFASLReqQryTransferredContractField *p) = 0;
    /**
      * FASL-�ͻ���ȡ�ʽ��������
      * @param p:ָ��ͻ���ȡ�ʽ��������ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqFASLDesirableFundsOut(DFITCFASLReqDesirableFundsOutField *p) = 0;
    /**
      * FASL-����֤ȯ��ѯ����
      * @param p:ָ�򵣱�֤ȯ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqFASLQryGuaranteedContract(DFITCFASLReqQryGuaranteedContractField *p) = 0;
    /**
      * FASL-���֤ȯ��ѯ����
      * @param p:ָ����֤ȯ��ѯ����ṹ��ĵ�ַ
      * @return : 0 ��ʾ�����ͳɹ����� 0 ��ʾ������ʧ�ܣ����������ο�error.xml
      */
     virtual int ReqFASLQryUnderlyingContract(DFITCFASLReqQryUnderlyingContractField *p) = 0;
public:
    DFITCSECTraderApi();
    virtual ~DFITCSECTraderApi();
};

#endif
