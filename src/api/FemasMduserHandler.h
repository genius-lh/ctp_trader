#ifndef _FEMAS_MDUSER_HANDLER_H_
#define _FEMAS_MDUSER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct femas_mduser_api_cb_def femas_mduser_api_cb;

struct femas_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUserLogout)(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnDepthMarketData)(void* arg, CUstpFtdcDepthMarketDataField *pMarketData);
  void (*xOnRspError)(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspSubMarketData)(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUnSubMarketData)(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
};
#ifdef __cplusplus
}
#endif

class CFemasMduserHandler : public CUstpFtdcMduserSpi
{
public:
    // ���캯������Ҫһ����Ч��ָ��CUstpFtdcMduserApiʵ����ָ��
    CFemasMduserHandler(femas_mduser_api_cb *pUserCb, void * parent);
    ~CFemasMduserHandler();
    // ���ͻ��������鷢��������������ͨ�����ӣ��ͻ�����Ҫ���е�¼
    void OnFrontConnected();

    // ���ͻ��������鷢��������ͨ�����ӶϿ�ʱ���÷���������
    void OnFrontDisconnected(int nReason);

	virtual void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    // ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
    void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��˳�Ӧ��
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// �������֪ͨ�����������������֪ͨ�ͻ���
    void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData);
  
	// ����û�����ĳ���֪ͨ
    void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ĺ�Լ�������Ϣ
	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�˶���Լ�������Ϣ
	void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
  femas_mduser_api_cb *m_pUserCb;
  void * m_parent;

};
#endif

