#ifndef _XSPEED_TEST_H_
#define _XSPEED_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

class CXSpeedTraderHandler : public DFITCSECTraderSpi
{
public:
  CXSpeedTraderHandler();
  virtual ~CXSpeedTraderHandler();
  
  /**
   * SEC-��������������Ӧ
   */
  void OnFrontConnected();
  /**
   * SEC-�������Ӳ�������Ӧ
   */
  void OnFrontDisconnected(int nReason);
  /**
   * SEC-��Ϣ֪ͨ
   */
  virtual void OnRtnNotice(DFITCSECRspNoticeField *pNotice) {};
  /**
  * ERR-����Ӧ��
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ַ
  */
  void OnRspError(DFITCSECRspInfoField *pRspInfo);
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
  void OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
   * SOP-�ǳ���Ӧ
   * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
  void OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
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
  void OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�����̱�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�����ί����Ӧ��Ϣ�ṹ��ַ,���������̱�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������̱�������ʧ��
  */
  virtual void OnRspSOPQuoteEntrustOrder(DFITCSOPRspQuoteEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {};
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
  void OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,����ί�в�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
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
  void OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
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
  void OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-ִ��ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ִ��ί����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ִ��ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ִ��ί������ʧ��
  */
  void OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo);
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
  void OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData);
  /**
  * SOP-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void OnSOPTradeRtn(DFITCSOPTradeRtnField * pData);
  /**
  * SOP-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData);

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

public:
  char* m_UserId;
  char* m_OldPasswd;
  char* m_NewPasswd;
  int* m_Loop;

  void* m_Arg;
  int m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void QueryContract();
  void InsertOrder(char* inst, int buy_sell, int open_close, double price, int vol, int covered);
  void ExecuteOrder(char* inst, int direction, int open_close, int vol);
  void WithdrawOrder(int sys_order_id);
  void QueryOrder(int sys_order_id);
  
  void CoveredOpen();
  void CoveredClose();
  void CallBuyOpen();
  void CallSellClose();
  void CallSellOpen();
  void CallBuyClose();
  void PutBuyOpen();
  void PutSellClose();
  void PutSellOpen();
  void PutBuyClose();
  void CallExecuteOrder();
  void PutExecuteOrder();
  void LogOut();
  void Query();
  void Withdraw();
  void QueryPosition();

};


#endif //_XSPEED_TEST_H_

