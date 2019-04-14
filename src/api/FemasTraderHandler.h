#ifndef _FEMAS_TRADER_HANDLER_H_
#define _FEMAS_TRADER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct femas_trader_api_cb_def femas_trader_api_cb;

struct femas_trader_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspError)(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
  void (*xOnRspUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast) ;
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
};

#ifdef __cplusplus
}
#endif

class CFemasTraderHandler : public CUstpFtdcTraderSpi
{
public:
    CFemasTraderHandler(femas_trader_api_cb* cb, void* arg);
    ~CFemasTraderHandler();
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	
	///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
	virtual void OnPackageStart(int nTopicID, int nSequenceNo){};
	
	///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
	virtual void OnPackageEnd(int nTopicID, int nSequenceNo){};

	///����Ӧ��
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///���ǰ��ϵͳ�û���¼Ӧ��
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�û��˳�Ӧ��
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û������޸�Ӧ��
	virtual void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����¼��Ӧ��
	void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��Ӧ��
	//virtual void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���۲���Ӧ��
	//virtual void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ѯ������Ӧ��
	//virtual void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����������֪ͨ
	virtual void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {};

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

	///�˻������ر�
	virtual void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {};

	///���ۻر�
	//virtual void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {};

	///����¼�����ر�
	//virtual void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {};

	///ѯ�ۻر�
	//virtual void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {};

	///������ѯӦ��
	virtual void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ɽ�����ѯӦ��
	virtual void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ͷ�����˻���ѯӦ��
	void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ױ����ѯӦ��
	virtual void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ�����ʽ��˻���ѯӦ��
	void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��Լ��ѯӦ��
	void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������ѯӦ��
	virtual void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���ֲֲ߳�ѯӦ��
	void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�Ϲ������ѯӦ��
	virtual void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯӦ��
	virtual void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ�����������ʲ�ѯӦ��
	virtual void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߱�֤���ʲ�ѯӦ��
	virtual void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

private:
  femas_trader_api_cb* m_TraderCb;
  void* m_Arg;

};

#endif

