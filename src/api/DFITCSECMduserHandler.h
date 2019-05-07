#ifndef _DFITC_SEC_MDUSER_HANDLER_H_
#define _DFITC_SEC_MDUSER_HANDLER_H_
#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dfitc_sec_mduser_api_cb_def dfitc_sec_mduser_api_cb;

struct dfitc_sec_mduser_api_cb_def{
  /** 
   * ��������������Ӧ
   */
 void (*xOnFrontConnected)(void* arg);

  /**
   * �������Ӳ�������Ӧ
   */
 void (*xOnFrontDisconnected)(void* arg, int nReason);

  /**
   * SEC-��Ϣ֪ͨ
   */
 void (*xOnRtnNotice)(void* arg, DFITCSECRspNoticeField *pNotice);

  /**
   * ERR-����Ӧ��
   * @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
   */
 void (*xOnRspError)(void* arg, DFITCSECRspInfoField *pRspInfo);

  /**
   * STOCK-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void (*xOnRspStockUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void (*xOnRspStockUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void (*xOnRspSOPUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void (*xOnRspSOPUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void (*xOnRspFASLUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void (*xOnRspFASLUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
 
  /**
   * STOCK-���鶩����Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
   */
 void (*xOnRspStockSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-ȡ������������Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   */
 void (*xOnRspStockUnSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
  
  /**
   * SOP-���鶩����Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
   */
 void (*xOnRspSOPSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-ȡ������������Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   */
 void (*xOnRspSOPUnSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-����������Ӧ
   * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
   */
 void (*xOnStockMarketData)(void* arg, DFITCStockDepthMarketDataField * pMarketDataField);

  /**
   * SOP-����������Ӧ
   * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
   */
 void (*xOnSOPMarketData)(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField);

  /**
   * STOCK-����������Ӧ
   * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
   */
 void (*xOnRspStockAvailableQuot)(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);

  /**
   * SOP-����������Ӧ
   * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
   */
 void (*xOnRspSopAvailableQuot)(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);

  /**
   * �������������Ӧ
   * @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
   */
 void (*xOnRspUserMDPasswordUpdate)(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);
};
#ifdef __cplusplus
}
#endif

class CDfitcSecMduserHandler : public DFITCSECMdSpi
{
public:
  /** 
   * ��������������Ӧ
   */
 void OnFrontConnected();

  /**
   * �������Ӳ�������Ӧ
   */
 void OnFrontDisconnected(int nReason);

  /**
   * SEC-��Ϣ֪ͨ
   */
 void OnRtnNotice(DFITCSECRspNoticeField *pNotice);

  /**
   * ERR-����Ӧ��
   * @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
   */
 void OnRspError(DFITCSECRspInfoField *pRspInfo);

  /**
   * STOCK-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void OnRspStockUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void OnRspStockUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void OnRspSOPUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void OnRspSOPUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-��¼��Ӧ
   * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
   */
 void OnRspFASLUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-�ǳ���Ӧ
   * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
   */
 void OnRspFASLUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
 
  /**
   * STOCK-���鶩����Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
   */
 void OnRspStockSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-ȡ������������Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   */
 void OnRspStockUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
  
  /**
   * SOP-���鶩����Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
   */
 void OnRspSOPSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-ȡ������������Ӧ
   * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   */
 void OnRspSOPUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-����������Ӧ
   * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
   */
 void OnStockMarketData(DFITCStockDepthMarketDataField * pMarketDataField);

  /**
   * SOP-����������Ӧ
   * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
   */
 void OnSOPMarketData(DFITCSOPDepthMarketDataField * pMarketDataField);

  /**
   * STOCK-����������Ӧ
   * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
   */
 void OnRspStockAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag);

  /**
   * SOP-����������Ӧ
   * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
   * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
   */
 void OnRspSopAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag);

  /**
   * �������������Ӧ
   * @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
   * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
   */
 void OnRspUserMDPasswordUpdate(DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);
  
public:
  CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb *pUserCb, void * parent);
  ~CDfitcSecMduserHandler();

private:
  dfitc_sec_mduser_api_cb *m_pUserCb;
  void * m_parent;
};

#endif // _DFITC_SEC_MDUSER_HANDLER_H_

