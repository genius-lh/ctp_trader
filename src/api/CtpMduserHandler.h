#ifndef _CTP_MDUSER_HANDLER_H_
#define _CTP_MDUSER_HANDLER_H_

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ctp_mduser_api_cb_def ctp_mduser_api_cb;

struct ctp_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUserLogout)(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRtnDepthMarketData)(void* arg, CThostFtdcDepthMarketDataField *pMarketData);
  void (*xOnRspError)(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspSubMarketData)(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUnSubMarketData)(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};
#ifdef __cplusplus
}
#endif

class CCtpMduserHandler : public CThostFtdcMdSpi
{
  public:
    
  CCtpMduserHandler(ctp_mduser_api_cb *pUserCb, void * parent);

public:
  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
  void OnFrontConnected();
  
  ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
  ///@param nReason ����ԭ��
  ///        0x1001 �����ʧ��
  ///        0x1002 ����дʧ��
  ///        0x2001 ����������ʱ
  ///        0x2002 ��������ʧ��
  ///        0x2003 �յ�������
  void OnFrontDisconnected(int nReason);
    
  ///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
  ///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
  virtual void OnHeartBeatWarning(int nTimeLapse){};
  

  ///��¼������Ӧ
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�ǳ�������Ӧ
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����Ӧ��
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������Ӧ��
  void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///ȡ����������Ӧ��
  void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����ѯ��Ӧ��
  virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

  ///ȡ������ѯ��Ӧ��
  virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

  ///�������֪ͨ
  void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

  ///ѯ��֪ͨ
  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

  private:
    void* m_parent;
    ctp_mduser_api_cb* m_pUserCb;
    
};

#endif //_CTP_MDUSER_HANDLER_H_
