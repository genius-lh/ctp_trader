#ifndef _FEMAS_TRADER_HANDLER_H_
#define _FEMAS_TRADER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

class CFemasTraderChangePassword : public CUstpFtdcTraderSpi
{
public:
    CFemasTraderChangePassword();
    virtual ~CFemasTraderChangePassword();
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
    
    ///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
    ///@param nTopicID ������루��˽���������������������ȣ�
    ///@param nSequenceNo �������
    virtual void OnPackageStart(int nTopicID, int nSequenceNo){};
    
    ///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
    ///@param nTopicID ������루��˽���������������������ȣ�
    ///@param nSequenceNo �������
    virtual void OnPackageEnd(int nTopicID, int nSequenceNo){};
    
    
    ///����Ӧ��
    void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    ///���ǰ��ϵͳ�û���¼Ӧ��
    void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    ///�û��˳�Ӧ��
    void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    ///�û������޸�Ӧ��
    void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    ///����¼��Ӧ��
    virtual void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///��������Ӧ��
    virtual void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///����¼��Ӧ��
    virtual void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///���۲���Ӧ��
    virtual void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///ѯ������Ӧ��
    virtual void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///�ͻ��������Ӧ��
    virtual void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///�û���������Ӧ��
    virtual void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///�û�����ϯ�����Ӧ��
    virtual void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///����������֪ͨ
    virtual void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {};
    
    ///�ɽ��ر�
    virtual void OnRtnTrade(CUstpFtdcTradeField *pTrade) {};
    
    ///�����ر�
    virtual void OnRtnOrder(CUstpFtdcOrderField *pOrder) {};
    
    ///����¼�����ر�
    virtual void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) {};
    
    ///������������ر�
    virtual void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) {};
    
    ///��Լ����״̬֪ͨ
    virtual void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) {};
    
    ///�˻������ر�
    virtual void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {};
    
    ///���ۻر�
    virtual void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {};
    
    ///����¼�����ر�
    virtual void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {};
    
    ///���۳�������ر�
    virtual void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo) {};
    
    ///ѯ�ۻر�
    virtual void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {};
    
    ///������Ϲ���֪ͨ
    virtual void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg) {};
    
    ///�ͻ��������ȷ��
    virtual void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction) {};
    
    ///�û���������
    virtual void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit) {};
    
    ///������ѯӦ��
    virtual void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///�ɽ�����ѯӦ��
    virtual void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///����Ͷ�����˻���ѯӦ��
    void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///���ױ����ѯӦ��
    virtual void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///Ͷ�����ʽ��˻���ѯӦ��
    virtual void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///��Լ��ѯӦ��
    virtual void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///��������ѯӦ��
    virtual void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///Ͷ���ֲֲ߳�ѯӦ��
    virtual void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
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
    
    ///���ױ�����ϳֲֲ�ѯӦ��
    virtual void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///���ױ��뵥�ȳֲֲ�ѯӦ��
    virtual void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
    
    ///��͸��ܿͻ���֤Ӧ��
    void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    ///��͸�����Ϣ�ɼ��м��ϴ���Ϣ��Ӧ
    virtual void OnRspDSProxySubmitInfo(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_NewPasswd;
  int* m_Loop;

  void* m_Arg;
  int m_RequestId;
};

#endif

