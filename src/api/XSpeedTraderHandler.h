#ifndef _DFITC_SEC_TRADER_HANDLER_H_
#define _DFITC_SEC_TRADER_HANDLER_H_
#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xspeed_trader_api_cb_def xspeed_trader_api_cb;

//#define DFITC_SEC_V128ON

struct xspeed_trader_api_cb_def{
  /**
   * SEC-��������������Ӧ
   */
  void (*xOnFrontConnected)(void* arg);
  /**
   * SEC-�������Ӳ�������Ӧ
   */
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  /**
   * SEC-��Ϣ֪ͨ
   */
  void (*xOnRtnNotice)(void* arg, DFITCSECRspNoticeField *pNotice);
  /**
  * ERR-����Ӧ��
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ַ
  */
  void (*xOnRspError)(void* arg, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-��¼��Ӧ
  * @param pData:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ĵ�ַ,������¼����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ��������¼����ʧ��
  */
  void (*xOnRspStockUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�ǳ���Ӧ
  * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ĵ�ַ,�����ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ǳ�����ʧ��
  */
  void (*xOnRspStockUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-���������Ӧ
  * @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ĵ�ַ,���������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������������ʧ��
  */
  void (*xOnRspStockUserPasswordUpdate)(void* arg, DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�б�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�б�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�б�������ʧ��
  */
  void (*xOnRspStockEntrustOrder)(void* arg, DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�г�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�г�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�г�������ʧ��
  */
  void (*xOnRspStockWithdrawOrder)(void* arg, DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryEntrustOrder)(void* arg, DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-ʵʱ�ɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����ʵʱ�ɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ʵʱ�ɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryRealTimeTrade)(void* arg, DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ֱʳɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֱʳɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֱʳɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQrySerialTrade)(void* arg, DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryPosition)(void* arg, DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ʽ��˺Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ��˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ʽ��˺Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��˺Ų�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryCapitalAccountInfo)(void* arg, DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�����˺Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������˺Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������˺Ų�ѯ����ʧ��
  */
  void (*xOnRspStockQryAccountInfo)(void* arg, DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-�ɶ��Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ��Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ɶ��Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ɶ��Ų�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryShareholderInfo)(void* arg, DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-�ʽ������Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ������Ӧ��Ϣ�ṹ��ĵ�ַ,�������ʽ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��������ʧ��
  */
  void (*xOnRspStockTransferFunds)(void* arg, DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-����ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û�����ί����Ӧ��Ϣ�ṹ��ĵ�ַ,��������ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ί������ʧ��
  */
  void (*xOnRspStockEntrustBatchOrder)(void* arg, DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-����������Ӧ
  * @param pData:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ĵ�ַ,����������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������������������ʧ��
  */
  void (*xOnRspStockWithdrawBatchOrder)(void* arg, DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�����ί��������Ӧ
  * @param pData:ָ�����ǿ�,�����û������ί��������Ӧ��Ϣ�ṹ��ĵ�ַ,���������ί����������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ί����������ʧ��
  */
  void (*xOnRspStockCalcAbleEntrustQty)(void* arg, DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-�����깺ETF������Ӧ
  * @param pData:ָ�����ǿ�,�����û������깺ETF������Ӧ��Ϣ�ṹ��ĵ�ַ,���������깺ETF��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������깺ETF��������ʧ��
  */
  void (*xOnRspStockCalcAblePurchaseETFQty)(void* arg, DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�����ʽ���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryFreezeFundsDetail)(void* arg, DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryFreezeStockDetail)(void* arg, DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryTransferStockDetail)(void* arg, DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�����ʽ���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryTransferFundsDetail)(void* arg, DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-֤ȯ��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryStockInfo)(void* arg, DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-֤ȯ��̬��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ��̬��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��̬��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��̬��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspStockQryStockStaticInfo)(void* arg, DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-����ʱ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����ʱ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������ʱ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ʱ���ѯ����ʧ��
  */
  void (*xOnRspStockQryTradeTime)(void* arg, DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�лر���Ӧ
  * @param pData:����ί�лر��ṹ��ĵ�ַ
  */
  void (*xOnStockEntrustOrderRtn)(void* arg, DFITCStockEntrustOrderRtnField * pData);
  /**
  * STOCK-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void (*xOnStockTradeRtn)(void* arg, DFITCStockTradeRtnField * pData);
  /**
  * STOCK-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void (*xOnStockWithdrawOrderRtn)(void* arg, DFITCStockWithdrawOrderRtnField * pData);
  
  /**
  * SOP-��¼��Ӧ
  * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
  */
  void (*xOnRspSOPUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
   * SOP-�ǳ���Ӧ
   * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
  void (*xOnRspSOPUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�û����������Ӧ
  * @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ַ,�����û������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������û������������ʧ��
  */
  void (*xOnRspSOPUserPasswordUpdate)(void* arg, DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
  */
  void (*xOnRspSOPEntrustOrder)(void* arg, DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-��ϲ��ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û���Ӧ��Ϣ�ṹ��ַ,������ϲ��ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ϲ��ί������ʧ��
  */
  void (*xOnRspSOPGroupSplit)(void* arg, DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-��ѯ�ͻ���ϳֲ���ϸ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�ͻ���ϳֲ���ϸ��Ӧ�ṹ��ַ,������ѯ�ͻ���ϳֲ���ϸ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ѯ�ͻ���ϳֲ���ϸ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryGroupPosition)(void* arg, DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-֤ȯ����������Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ����������Ӧ��Ϣ�ṹ��ַ,����֤ȯ������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������֤ȯ������������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPLockOUnLockStock)(void* arg, DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
  */
  void (*xOnRspSOPWithdrawOrder)(void* arg, DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,����ί�в�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryEntrustOrder)(void* arg, DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-�ֱʳɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֱʳɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֱʳɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQrySerialTrade)(void* arg, DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-�ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryPosition)(void* arg, DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-�ͻ������ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ������ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryCollateralPosition)(void* arg, DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-�ͻ��ʽ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
  */
  void (*xOnRspSOPQryCapitalAccountInfo)(void* arg, DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ͻ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
  */
  void (*xOnRspSOPQryAccountInfo)(void* arg, DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ɶ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
  */
  void (*xOnRspSOPQryShareholderInfo)(void* arg, DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * SOP-��ί��������ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ί��������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ί��������ѯ����ʧ��
  */
  void (*xOnRspSOPCalcAbleEntrustQty)(void* arg, DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ͻ�������֤ȯ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ�������֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�������֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryAbleLockStock)(void* arg, DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-��Ȩ��Լ�����ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���Ȩ��Լ�����ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ��Լ�����ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ��Լ�����ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryContactInfo)(void* arg, DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-ִ��ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ִ��ί����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ִ��ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ִ��ί������ʧ��
  */
  void (*xOnRspSOPExectueOrder)(void* arg, DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ͻ���Ȩָ����Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���Ȩָ����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ȩָ����Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ȩָ����Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryExecAssiInfo)(void* arg, DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ����ʱ����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ����ʱ����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ����ʱ������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ����ʱ������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryTradeTime)(void* arg, DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ȡ���н�����������Ӧ
  * @param pData:ָ�����ǿ�,�����û���ȡ���н�����������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ���н�������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ���н�������������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryExchangeInfo)(void* arg, DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ�����Ѳ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�����Ѳ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����Ѳ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����Ѳ�������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryCommission)(void* arg, DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ��֤���ʲ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ��֤���ʲ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ��֤���ʲ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ��֤���ʲ�������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryDeposit)(void* arg, DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��Ȩ�����Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���Ȩ�����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ�����Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ�����Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspSOPQryContractObjectInfo)(void* arg, DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-ί�лر���Ӧ
  * @param pData:����ί�лر��ṹ��ĵ�ַ
  */
  void (*xOnSOPEntrustOrderRtn)(void* arg, DFITCSOPEntrustOrderRtnField * pData);
  /**
  * SOP-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void (*xOnSOPTradeRtn)(void* arg, DFITCSOPTradeRtnField * pData);
  /**
  * SOP-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void (*xOnSOPWithdrawOrderRtn)(void* arg, DFITCSOPWithdrawOrderRtnField * pData);

  /**
  * FASL-��¼��Ӧ
  * @param pData:ָ�����ǿ�,�����û�������ȯ��¼��Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��¼����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��¼����ʧ��
  */
  void (*xOnRspFASLUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ǳ���Ӧ
  * @param pData:ָ�����ǿ�,�����û�������ȯ�ǳ���Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ�ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ�ǳ�����ʧ��
  */
  void (*xOnRspFASLUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ���������Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���������Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���������Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���������Ϣ����ʧ��
  */
  void (*xOnRspFASLQryAbleFinInfo)(void* arg, DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ�����ȯ��Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ�����ȯ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�����ȯ��Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�����ȯ��Ϣ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryAbleSloInfo)(void* arg, DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�����ﻮת��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ﻮת��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ﻮת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ﻮת����ʧ��
  */
  void (*xOnRspFASLTransferCollateral)(void* arg, DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-ֱ�ӻ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ֱ�ӻ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ֱ�ӻ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ֱ�ӻ�������ʧ��
  */
  void (*xOnRspFASLDirectRepayment)(void* arg, DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ȯ��ת��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ȯ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȯ��ת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȯ��ת����ʧ��
  */
  void (*xOnRspFASLRepayStockTransfer)(void* arg, DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ý�����Ӧ
  * @param pData:ָ�����ǿ�,�����û����ý�����Ӧ��Ϣ�ṹ��ַ,�����ͻ����ý�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ý�������ʧ��
  */
  void (*xOnRspFASLEntrustCrdtOrder)(void* arg, DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-������ȯ������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������ȯ������Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��������ʧ��
  */
  void (*xOnRspFASLEntrustOrder)(void* arg, DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ÿ�ί��������ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ÿ�ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ÿ�ί��������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ÿ�ί��������ѯ����ʧ��
  */
  void (*xOnRspFASLCalcAbleEntrustCrdtQty)(void* arg, DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ѯ�����ʽ���Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�����ʽ���Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����ʽ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����ʽ�����ʧ��
  */
  void (*xOnRspFASLQryCrdtFunds)(void* arg, DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ú�Լ��Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ú�Լ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ��Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ��Ϣ����ʧ��
  */
  void (*xOnRspFASLQryCrdtContract)(void* arg, DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLat);
  /**
  * FASL-���ú�Լ�䶯��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ú�Լ�䶯��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ�䶯��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ�䶯��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryCrdtConChangeInfo)(void* arg, DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ��ת��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ��ת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ��ת����ʧ��
  */
  void (*xOnRspFASLTransferFunds)(void* arg, DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
  */
  void (*xOnRspFASLQryAccountInfo)(void* arg, DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ��ʽ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryCapitalAccountInfo)(void* arg, DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ɶ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryShareholderInfo)(void* arg, DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryPosition)(void* arg, DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ί�в�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryEntrustOrder)(void* arg, DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ֱʳɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֱʳɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֱʳɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQrySerialTrade)(void* arg, DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ʵʱ�ɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ʵʱ�ɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ʵʱ�ɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryRealTimeTrade)(void* arg, DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ𶳽���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ𶳽���ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ𶳽���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ𶳽���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryFreezeFundsDetail)(void* arg, DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-֤ȯ������ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�֤ȯ������ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�֤ȯ������ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryFreezeStockDetail)(void* arg, DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ������ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ������ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ������ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryTransferFundsDetail)(void* arg, DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
  */
  void (*xOnRspFASLWithdrawOrder)(void* arg, DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ǰϵͳʱ���ѯ������Ӧ
  * @param pData:ָ�����ǿ�,�����û�ϵͳʱ���ѯ��Ӧ��Ϣ�ṹ��ַ,����ϵͳʱ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ϵͳʱ���ѯ����ʧ��
  */
  void (*xOnRspFASLQrySystemTime)(void* arg, DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ת�뵣��֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ؿ�ת�뵣��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,������ת�뵣��֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ת�뵣��֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryTransferredContract)(void* arg, DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ͻ���ȡ�ʽ����������Ӧ
  * @param pData:ָ�����ǿ�,���ؿͻ���ȡ�ʽ������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ�ʽ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ�ʽ��������ʧ��
  */
  void (*xOnRspFASLDesirableFundsOut)(void* arg, DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-����֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ص���֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,��������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ����������֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryGuaranteedContract)(void* arg, DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-���֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ر��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ���������֤ȯ��ѯʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ)(void* arg, 0-��,1-��)
  */
  void (*xOnRspFASLQryUnderlyingContract)(void* arg, DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ί�лر���Ӧ
  * @param pData:����ί�лر��ṹ��ĵ�ַ
  */
  void (*xOnFASLEntrustOrderRtn)(void* arg, DFITCStockEntrustOrderRtnField *pData);
  /**
  * FASL-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void (*xOnFASLTradeRtn)(void* arg, DFITCStockTradeRtnField *pData);
  /**
  * FASL-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void (*xOnFASLWithdrawOrderRtn)(void* arg, DFITCStockWithdrawOrderRtnField *pData);
};
#ifdef __cplusplus
}
#endif


class CXSpeedTraderHandler : public DFITCSECTraderSpi
{
public:
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
  void OnRtnNotice(DFITCSECRspNoticeField *pNotice);
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
  void OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�ǳ���Ӧ
  * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ĵ�ַ,�����ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ǳ�����ʧ��
  */
  void OnRspStockUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-���������Ӧ
  * @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ĵ�ַ,���������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������������ʧ��
  */
  void OnRspStockUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�б�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�б�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�б�������ʧ��
  */
  void OnRspStockEntrustOrder(DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�г�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�г�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�г�������ʧ��
  */
  void OnRspStockWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryEntrustOrder(DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-ʵʱ�ɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����ʵʱ�ɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ʵʱ�ɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ֱʳɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֱʳɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֱʳɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQrySerialTrade(DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�ʽ��˺Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ��˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ʽ��˺Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��˺Ų�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�����˺Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������˺Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������˺Ų�ѯ����ʧ��
  */
  void OnRspStockQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-�ɶ��Ų�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ��Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ɶ��Ų�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ɶ��Ų�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-�ʽ������Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ������Ӧ��Ϣ�ṹ��ĵ�ַ,�������ʽ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��������ʧ��
  */
  void OnRspStockTransferFunds(DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-����ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û�����ί����Ӧ��Ϣ�ṹ��ĵ�ַ,��������ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ί������ʧ��
  */
  void OnRspStockEntrustBatchOrder(DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-����������Ӧ
  * @param pData:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ĵ�ַ,����������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������������������ʧ��
  */
  void OnRspStockWithdrawBatchOrder(DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�����ί��������Ӧ
  * @param pData:ָ�����ǿ�,�����û������ί��������Ӧ��Ϣ�ṹ��ĵ�ַ,���������ί����������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ί����������ʧ��
  */
  void OnRspStockCalcAbleEntrustQty(DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-�����깺ETF������Ӧ
  * @param pData:ָ�����ǿ�,�����û������깺ETF������Ӧ��Ϣ�ṹ��ĵ�ַ,���������깺ETF��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������깺ETF��������ʧ��
  */
  void OnRspStockCalcAblePurchaseETFQty(DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-�����ʽ���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-����֤ȯ��ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryTransferStockDetail(DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-�����ʽ���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-֤ȯ��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryStockInfo(DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-֤ȯ��̬��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ��̬��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��̬��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��̬��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspStockQryStockStaticInfo(DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-����ʱ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�����ʱ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������ʱ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ʱ���ѯ����ʧ��
  */
  void OnRspStockQryTradeTime(DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-ί�лر���Ӧ
  * @param pData:����ί�лر��ṹ��ĵ�ַ
  */
  void OnStockEntrustOrderRtn(DFITCStockEntrustOrderRtnField * pData);
  /**
  * STOCK-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void OnStockTradeRtn(DFITCStockTradeRtnField * pData);
  /**
  * STOCK-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void OnStockWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField * pData);
  
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
  void OnRspSOPUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
  */
  void OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-��ϲ��ί����Ӧ
  * @param pData:ָ�����ǿ�,�����û���Ӧ��Ϣ�ṹ��ַ,������ϲ��ί������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ϲ��ί������ʧ��
  */
  void OnRspSOPGroupSplit(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-��ѯ�ͻ���ϳֲ���ϸ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�ͻ���ϳֲ���ϸ��Ӧ�ṹ��ַ,������ѯ�ͻ���ϳֲ���ϸ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ѯ�ͻ���ϳֲ���ϸ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryGroupPosition(DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-֤ȯ����������Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ����������Ӧ��Ϣ�ṹ��ַ,����֤ȯ������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������֤ȯ������������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPLockOUnLockStock(DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
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
  void OnRspSOPQrySerialTrade(DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
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
  void OnRspSOPQryCollateralPosition(DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-�ͻ��ʽ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
  */
  void OnRspSOPQryCapitalAccountInfo(DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ͻ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
  */
  void OnRspSOPQryAccountInfo(DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ɶ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
  */
  void OnRspSOPQryShareholderInfo(DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * SOP-��ί��������ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ί��������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ί��������ѯ����ʧ��
  */
  void OnRspSOPCalcAbleEntrustQty(DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-�ͻ�������֤ȯ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ�������֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�������֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryAbleLockStock(DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
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
  void OnRspSOPQryExecAssiInfo(DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ����ʱ����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ����ʱ����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ����ʱ������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ����ʱ������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryTradeTime(DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ȡ���н�����������Ӧ
  * @param pData:ָ�����ǿ�,�����û���ȡ���н�����������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ���н�������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ���н�������������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryExchangeInfo(DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ�����Ѳ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�����Ѳ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����Ѳ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����Ѳ�������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryCommission(DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��ѯ��֤���ʲ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ��֤���ʲ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ��֤���ʲ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ��֤���ʲ�������ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryDeposit(DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-��Ȩ�����Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û���Ȩ�����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ�����Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ�����Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspSOPQryContractObjectInfo(DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
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
  void OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ǳ���Ӧ
  * @param pData:ָ�����ǿ�,�����û�������ȯ�ǳ���Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ�ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ�ǳ�����ʧ��
  */
  void OnRspFASLUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ���������Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���������Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���������Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���������Ϣ����ʧ��
  */
  void OnRspFASLQryAbleFinInfo(DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ�����ȯ��Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ�����ȯ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�����ȯ��Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�����ȯ��Ϣ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryAbleSloInfo(DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�����ﻮת��Ӧ
  * @param pData:ָ�����ǿ�,�����û������ﻮת��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ﻮת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ﻮת����ʧ��
  */
  void OnRspFASLTransferCollateral(DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-ֱ�ӻ�����Ӧ
  * @param pData:ָ�����ǿ�,�����û�ֱ�ӻ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ֱ�ӻ�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ֱ�ӻ�������ʧ��
  */
  void OnRspFASLDirectRepayment(DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ȯ��ת��Ӧ
  * @param pData:ָ�����ǿ�,�����û���ȯ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȯ��ת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȯ��ת����ʧ��
  */
  void OnRspFASLRepayStockTransfer(DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ý�����Ӧ
  * @param pData:ָ�����ǿ�,�����û����ý�����Ӧ��Ϣ�ṹ��ַ,�����ͻ����ý�������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ý�������ʧ��
  */
  void OnRspFASLEntrustCrdtOrder(DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-������ȯ������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������ȯ������Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��������ʧ��
  */
  void OnRspFASLEntrustOrder(DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ÿ�ί��������ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ÿ�ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ÿ�ί��������ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ÿ�ί��������ѯ����ʧ��
  */
  void OnRspFASLCalcAbleEntrustCrdtQty(DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ѯ�����ʽ���Ӧ
  * @param pData:ָ�����ǿ�,�����û���ѯ�����ʽ���Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����ʽ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����ʽ�����ʧ��
  */
  void OnRspFASLQryCrdtFunds(DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-���ú�Լ��Ϣ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ú�Լ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ��Ϣ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ��Ϣ����ʧ��
  */
  void OnRspFASLQryCrdtContract(DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLat);
  /**
  * FASL-���ú�Լ�䶯��Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û����ú�Լ�䶯��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ�䶯��Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ�䶯��Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryCrdtConChangeInfo(DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ��ת��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ��ת����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ��ת����ʧ��
  */
  void OnRspFASLTransferFunds(DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
  */
  void OnRspFASLQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-�ͻ��ʽ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ɶ���Ϣ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ֲֲ�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֲֲ�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֲֲ�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ί�в�ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ί�в�ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ί�в�ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryEntrustOrder(DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ֱʳɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֱʳɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֱʳɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQrySerialTrade(DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ʵʱ�ɽ���ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ʵʱ�ɽ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ʵʱ�ɽ���ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ𶳽���ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ𶳽���ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ𶳽���ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ𶳽���ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-֤ȯ������ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û�֤ȯ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�֤ȯ������ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�֤ȯ������ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ʽ������ϸ��ѯ��Ӧ
  * @param pData:ָ�����ǿ�,�����û��ʽ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ������ϸ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ������ϸ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-������Ӧ
  * @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
  */
  void OnRspFASLWithdrawOrder(DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ǰϵͳʱ���ѯ������Ӧ
  * @param pData:ָ�����ǿ�,�����û�ϵͳʱ���ѯ��Ӧ��Ϣ�ṹ��ַ,����ϵͳʱ���ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ϵͳʱ���ѯ����ʧ��
  */
  void OnRspFASLQrySystemTime(DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-��ת�뵣��֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ؿ�ת�뵣��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,������ת�뵣��֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ת�뵣��֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryTransferredContract(DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-�ͻ���ȡ�ʽ����������Ӧ
  * @param pData:ָ�����ǿ�,���ؿͻ���ȡ�ʽ������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ�ʽ��������ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ�ʽ��������ʧ��
  */
  void OnRspFASLDesirableFundsOut(DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-����֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ص���֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,��������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ����������֤ȯ��ѯ����ʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryGuaranteedContract(DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-���֤ȯ��ѯ������Ӧ
  * @param pData:ָ�����ǿ�,���ر��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�������֤ȯ��ѯ����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ���������֤ȯ��ѯʧ��
  * @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
  */
  void OnRspFASLQryUnderlyingContract(DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-ί�лر���Ӧ
  * @param pData:����ί�лر��ṹ��ĵ�ַ
  */
  void OnFASLEntrustOrderRtn(DFITCStockEntrustOrderRtnField *pData);
  /**
  * FASL-�ɽ��ر���Ӧ
  * @param pData:���سɽ��ر��ṹ��ĵ�ַ
  */
  void OnFASLTradeRtn(DFITCStockTradeRtnField *pData);
  /**
  * FASL-�����ر���Ӧ
  * @param pData:���س����ر��ṹ��ĵ�ַ
  */
  void OnFASLWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField *pData);
  
public:
  CXSpeedTraderHandler(xspeed_trader_api_cb *pUserCb, void * parent);
  ~CXSpeedTraderHandler();

private:
  xspeed_trader_api_cb* m_pUserCb;
  void* m_parent;
};

#endif //_DFITC_SEC_TRADER_HANDLER_H_

