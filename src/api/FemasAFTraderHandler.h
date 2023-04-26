#ifndef _FEMAS_AF_TRADER_HANDLER_H_
#define _FEMAS_AF_TRADER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#include <string>
#include <map>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct femas_af_trader_api_cb_def femas_af_trader_api_cb;

struct femas_af_trader_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnQryFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnQryFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspError)(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspQueryUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspUserLogout)(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspOrderInsert)(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspOrderAction)(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnTrade)(void* arg, CUstpFtdcTradeField *pTrade);
  void (*xOnRtnOrder)(void* arg, CUstpFtdcOrderField *pOrder);
  void (*xOnErrRtnOrderInsert)(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);
  void (*xOnErrRtnOrderAction)(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);
  void (*xOnRspQryUserInvestor)(void* arg, CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspQryInstrument)(void* arg, CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnInstrumentStatus)(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus);
	void (*xOnRspQryInvestorAccount)(void* arg, CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
	void (*xOnRspQryInvestorPosition)(void* arg, CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspDSUserCertification)(void* arg, CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
};

#ifdef __cplusplus
}
#endif

class CFemasAFTraderHandler : public CUstpFtdcTraderSpi
{
public:
  CFemasAFTraderHandler(femas_af_trader_api_cb* cb, void* arg);
  ~CFemasAFTraderHandler();
  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
  void OnFrontConnected();
  void OnQryFrontConnected(); 
  ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
  ///@param nReason ����ԭ��
  ///        0x1001 �����ʧ��
  ///        0x1002 ����дʧ��
  ///        0x2001 ����������ʱ
  ///        0x2002 ��������ʧ��
  ///        0x2003 �յ�������
  void OnFrontDisconnected(int nReason);
  void OnQryFrontDisconnected(int nReason);

  ///����Ӧ��
  void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///���ǰ��ϵͳ�û���¼Ӧ��
  void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�û��˳�Ӧ��
  void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����¼��Ӧ��
  void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��������Ӧ��
  void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�ɽ��ر�
  void OnRtnTrade(CUstpFtdcTradeField *pTrade);

  ///�����ر�
  void OnRtnOrder(CUstpFtdcOrderField *pOrder);

  ///����¼�����ر�
  void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);

  ///������������ر�
  void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);

  ///��Լ����״̬֪ͨ
  void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus);

  ///����Ͷ�����˻���ѯӦ��
  void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ�����ʽ��˻���ѯӦ��
  void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///��Լ��ѯӦ��
  void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///Ͷ���ֲֲ߳�ѯӦ��
  void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��͸��ܿͻ���֤Ӧ��
  void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///��ѯǰ��ϵͳ�û���¼Ӧ��
  void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
  const char* GetExchangeID(const char* instrument_id);

private:
  femas_af_trader_api_cb* m_TraderCb;
  void* m_Arg;

  std::map<std::string, std::string> mapSymbol;

};

#endif

