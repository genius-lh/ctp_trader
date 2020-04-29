
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"

#include "CtpTest.h"

//using namespace ctp_sopt;

#define CTP_LOG(...) printf(__VA_ARGS__)

#define READ_STRING(_msg_, _val_) \
  do {\
    printf("%s:", (_msg_));\
    scanf("%s", (_val_));\
  }while(0)

#define READ_INT(_msg_, _val_) \
  do {\
    printf("%s:", (_msg_));\
    scanf("%d", &(_val_));\
  }while(0)

#define READ_DOUBLE(_msg_, _val_) \
  do {\
    printf("%s:", (_msg_));\
    scanf("%lf", &(_val_));\
  }while(0)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
  char sInvestorID[128];
  char sFilename[128];
  int i = 1;

  if(argc < 8){
    printf("input username:\n");
    scanf("%s", sUserId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sAddress:\n");
    scanf("%s", sAddress);
    printf("input sAppId:\n");
    scanf("%s", sAppId);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
    printf("input sBrokerID:\n");
    scanf("%s", sBrokerID);
    printf("input sInvestorID:\n");
    scanf("%s", sInvestorID);
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
    strncpy(sInvestorID, argv[i++], sizeof(sInvestorID));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi("./");
  CFemasTraderHandler* pTraderHandler = new CFemasTraderHandler();
  // ��ʼ������
  // ����
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);

  pTraderApi->SubscribePublicTopic(USTP_TERT_RESUME);
  pTraderApi->RegisterFront(sAddress);

  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_InvestorID = sInvestorID;
  pTraderHandler->m_OutputFile = sFilename;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;

  
  FILE* fp;
  fp = fopen(sFilename, "w+");
  if(NULL != fp){
    fclose(fp);
  }

  // ���ӽ��׷�����
  pTraderApi->Init();

  pTraderHandler->Loop();

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}


CFemasTraderHandler::CFemasTraderHandler()
{

}

CFemasTraderHandler::~CFemasTraderHandler()
{

}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CFemasTraderHandler::OnFrontConnected()
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(0 == m_Loop){
    return ;
  }
  DSUserCertification();
  return ;
}


///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CFemasTraderHandler::OnFrontDisconnected(int nReason)
{
  CTP_LOG("%s\n", __FUNCTION__);
}
  
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
//void OnHeartBeatWarning(int nTimeLapse){};

///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
///@param nTopicID ������루��˽���������������������ȣ�
///@param nSequenceNo �������
//void OnPackageStart(int nTopicID, int nSequenceNo){};

///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
///@param nTopicID ������루��˽���������������������ȣ�
///@param nSequenceNo �������
//void OnPackageEnd(int nTopicID, int nSequenceNo){};

///����Ӧ��
void CFemasTraderHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///���ǰ��ϵͳ�û���¼Ӧ��
void CFemasTraderHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspUserLogin){
    CTP_LOG(
      "pRspUserLogin->TradingDay=[%s]"
      "pRspUserLogin->BrokerID=[%s]"
      "pRspUserLogin->UserID=[%s]"
      "pRspUserLogin->LoginTime=[%s]"
      "pRspUserLogin->ExchangeTime=[%s]"
      "pRspUserLogin->MaxOrderLocalID=[%s]"
      "pRspUserLogin->TradingSystemName=[%s]"
      "pRspUserLogin->DataCenterID=[%d]"
      "pRspUserLogin->PrivateFlowSize=[%d]"
      "pRspUserLogin->UserFlowSize=[%d]"
      "pRspUserLogin->ActionDay=[%s]"
      "pRspUserLogin->FemasVersion=[%s]"
      "pRspUserLogin->FemasLifeCycle=[%d]"
      "\n"
      , pRspUserLogin->TradingDay
      , pRspUserLogin->BrokerID
      , pRspUserLogin->UserID
      , pRspUserLogin->LoginTime
      , pRspUserLogin->ExchangeTime
      , pRspUserLogin->MaxOrderLocalID
      , pRspUserLogin->TradingSystemName
      , pRspUserLogin->DataCenterID
      , pRspUserLogin->PrivateFlowSize
      , pRspUserLogin->UserFlowSize
      , pRspUserLogin->ActionDay
      , pRspUserLogin->FemasVersion
      , pRspUserLogin->FemasLifeCycle
    );

  }
  
}

///�û��˳�Ӧ��
void CFemasTraderHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  m_Loop = 0;
}

///�û������޸�Ӧ��
void CFemasTraderHandler::OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///����¼��Ӧ��
void CFemasTraderHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///��������Ӧ��
void CFemasTraderHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///����¼��Ӧ��
//void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///���۲���Ӧ��
//void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///ѯ������Ӧ��
//void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///�ͻ��������Ӧ��
//void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///�û���������Ӧ��
//void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///�û�����ϯ�����Ӧ��
//void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///����������֪ͨ
//void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {};

///�ɽ��ر�
void CFemasTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintTrade(pTrade);
}

///�����ر�
void CFemasTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintOrder(pOrder);
}

///����¼�����ر�
void CFemasTraderHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///������������ر�
void CFemasTraderHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///��Լ����״̬֪ͨ
void CFemasTraderHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pInstrumentStatus){


  }
}

///�˻������ر�
//void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {};

///���ۻر�
//void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {};

///����¼�����ر�
//void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {};

///���۳�������ر�
//void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo) {};

///ѯ�ۻر�
//void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {};

///������Ϲ���֪ͨ
//void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg) {};

///�ͻ��������ȷ��
//void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction) {};

///�û���������
//void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit) {};

///������ѯӦ��
void CFemasTraderHandler::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintOrder(pOrder);
}

///�ɽ�����ѯӦ��
void CFemasTraderHandler::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintTrade(pTrade);
}

///����Ͷ�����˻���ѯӦ��
void CFemasTraderHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
	///���͹�˾���
	TUstpFtdcBrokerIDType	BrokerID;
	///�����û�����
	TUstpFtdcUserIDType	UserID;
	///Ͷ���߱��
	TUstpFtdcInvestorIDType	InvestorID;

  if(pRspUserInvestor){
    CTP_LOG(
      "pRspUserInvestor->BrokerID=[%s]"
      "pRspUserInvestor->UserID=[%s]"
      "pRspUserInvestor->InvestorID=[%s]"
      "\n"
      , pRspUserInvestor->BrokerID
      , pRspUserInvestor->UserID
      , pRspUserInvestor->InvestorID
    );
  }

}

///���ױ����ѯӦ��
//void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///Ͷ�����ʽ��˻���ѯӦ��
void CFemasTraderHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///��Լ��ѯӦ��
void CFemasTraderHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  PrintInstrument(pRspInstrument);
  return;
}

///��������ѯӦ��
void CFemasTraderHandler::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///Ͷ���ֲֲ߳�ѯӦ��
void CFemasTraderHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///��������Ӧ��
void CFemasTraderHandler::OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///�Ϲ������ѯӦ��
//void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///�����ѯӦ��
//void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///Ͷ�����������ʲ�ѯӦ��
//void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///Ͷ���߱�֤���ʲ�ѯӦ��
//void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///���ױ�����ϳֲֲ�ѯӦ��
//void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///���ױ��뵥�ȳֲֲ�ѯӦ��
//void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///��͸��ܿͻ���֤Ӧ��
void CFemasTraderHandler::OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  if(bIsLast && pDSUserCertRspData){
    UserLogin()
  }
  
  return ;
}

///��͸�����Ϣ�ɼ��м��ϴ���Ϣ��Ӧ
//virtual void OnRspDSProxySubmitInfo(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

void CFemasTraderHandler::Loop()
{
  int choose;
  m_Loop = 1;
  do{
    choose = ShowMenu();
    switch(choose){
    case 1:
      UserPasswordUpdate();
      break;
    case 2:
      OrderInsert();
      break;
    case 3:
      OrderAction();
      break;
    case 4:
      QryOrder();
      break;
    case 5:
      QryTrade();
      break;
    case 6:
      QryUserInvestor();
      break;
    case 7:
      QryInvestorAccount();
      break;
    case 8:
      QryInstrument();
      break;
    case 9:
      QryInvestorPosition();
      break;
    case 10:
      QryExchange();
      break;
    case 11:
      SubscribeTopic();
      break;
    case 0:
      UserLogout();
      sleep(1);
      break;
    default:
      break;
    }
  }while(m_Loop);
  return ;
}

int CFemasTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "ѡ����Ҫִ�еĲ���\n"
        "1 - UserPasswordUpdate();\n"
        "2 - OrderInsert();\n"
        "3 - OrderAction();\n"
        "4 - QryOrder();\n"
        "5 - QryTrade();\n"
        "6 - QryUserInvestor();\n"
        "7 - QryInvestorAccount();\n"
        "8 - QryInstrument();\n"
        "9 - QryInvestorPosition();\n"
        "10- QryExchange();\n"
        "11- SubscribeTopic();\n"
        "0 - UserLogout();\n"
        "**********************\n"
        "��ѡ��"
  );
  scanf("%d",&ret);

  return ret;

}

void CFemasTraderHandler::UserLogin()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_OldPasswd);
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
}

void CFemasTraderHandler::UserLogout()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLogoutField reqUserLogoutField;
  
  memset(&reqUserLogoutField, 0, sizeof(reqUserLogoutField));
  strcpy(reqUserLogoutField.BrokerID, m_BrokerID);
  strcpy(reqUserLogoutField.UserID, m_UserId);
  
  pTraderApi->ReqUserLogout(&reqUserLogoutField, m_RequestId++);
}

void CFemasTraderHandler::UserPasswordUpdate()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;

  char sNewPasswd[41];
  printf("������������:");
  scanf("%s", sNewPasswd);

  CUstpFtdcUserPasswordUpdateField reqUserPasswordUpdateField;
  memset(&reqUserPasswordUpdateField, 0, sizeof(reqUserPasswordUpdateField));
  strcpy(reqUserPasswordUpdateField.BrokerID, m_BrokerID);
  strcpy(reqUserPasswordUpdateField.UserID, m_UserId);
  strcpy(reqUserPasswordUpdateField.OldPassword, m_OldPasswd);
  strcpy(reqUserPasswordUpdateField.NewPassword, sNewPasswd);

  pTraderApi->ReqUserPasswordUpdate(&reqUserPasswordUpdateField, m_RequestId++);

}

void CFemasTraderHandler::OrderInsert()
{
  char exchangeID[11];
  char instrumentID[31];
  char userOrderLocalID[21];
  char direction[2];
  char offsetFlag[2];
  double limitPrice;
  int volume;
  
  READ_STRING("exchangeID", exchangeID);
  READ_STRING("instrumentID", instrumentID);
  READ_STRING("userOrderLocalID", userOrderLocalID);
  READ_STRING("direction", direction);
  READ_STRING("offsetFlag", offsetFlag);
  READ_DOUBLE("limitPrice", limitPrice);
  READ_INT("volume", volume);
  
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcInputOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
  
	///���͹�˾����
	strncpy(inputOrderField.BrokerID, m_BrokerID, sizeof(inputOrderField.BrokerID));
	///����������
	strncpy(inputOrderField.ExchangeID, exchangeID, sizeof(inputOrderField.ExchangeID));
	///Ͷ���ߴ���
	strncpy(inputOrderField.InvestorID, m_InvestorID, sizeof(inputOrderField.InvestorID));
	///�û�����
	strncpy(inputOrderField.UserID, m_UserId, sizeof(inputOrderField.UserID));
	///��Լ����
	strncpy(inputOrderField.InstrumentID, instrumentID, sizeof(inputOrderField.InstrumentID));
	///��������
	strncpy(inputOrderField.UserOrderLocalID, userOrderLocalID, sizeof(inputOrderField.UserOrderLocalID));
	///�����۸�����
	inputOrderField.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	///��������
	inputOrderField.Direction = direction[0];
	///��Ͽ�ƽ��־
	inputOrderField.OffsetFlag = offsetFlag[0];
	///���Ͷ���ױ���־
	inputOrderField.HedgeFlag = USTP_FTDC_CHF_Speculation;
	///�۸�
	inputOrderField.LimitPrice = limitPrice;
	///����
	inputOrderField.Volume = volume;
	///��Ч������
	inputOrderField.TimeCondition = USTP_FTDC_TC_GFD;
	///�ɽ�������
	inputOrderField.VolumeCondition = USTP_FTDC_VC_AV;
	///��С�ɽ���
	inputOrderField.MinVolume = 1;
	///ǿƽԭ��
	inputOrderField.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	///�Զ������־
	inputOrderField.IsAutoSuspend = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, m_RequestId++);

}

void CFemasTraderHandler::OrderAction()
{
  char exchangeID[11];
  char orderSysID[31];
  char userOrderLocalID[21];
  
  READ_STRING("exchangeID", exchangeID);
  READ_STRING("orderSysID", orderSysID);
  READ_STRING("userOrderLocalID", userOrderLocalID);

  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcOrderActionField data3;
  
  memset(&data3, 0, sizeof(data3));
	///����������
	strncpy(data3.ExchangeID, exchangeID, sizeof(data3.ExchangeID));
	///ϵͳ�������
	strncpy(data3.OrderSysID, orderSysID, sizeof(data3.InvestorID));
	///���͹�˾���
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///Ͷ���߱��
	strncpy(data3.InvestorID, m_InvestorID, sizeof(data3.InvestorID));
	///�û�����
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));
	///���γ��������ı��ر��
	strncpy(data3.UserOrderActionLocalID, userOrderLocalID, sizeof(data3.UserOrderActionLocalID));
	///���������ı��ر������
	//strncpy(data3.UserOrderLocalID, userOrderLocalID, sizeof(data3.UserOrderLocalID));
	///����������־
	data3.ActionFlag = USTP_FTDC_AF_Delete;
	///�۸�
	///�����仯
	///����ҵ���ʶ
	///�ͻ�����

  pTraderApi->ReqOrderAction(&data3, m_RequestId++);
}

void CFemasTraderHandler::QryOrder()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcQryOrderField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.BrokerID, m_BrokerID);
  strcpy(data3.UserID, m_UserId);
  strcpy(data3.InvestorID, m_InvestorID);
  
  pTraderApi->ReqQryOrder(&data3, m_RequestId++);

}

void CFemasTraderHandler::QryTrade()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryTradeField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.BrokerID, m_BrokerID);
  strcpy(data3.UserID, m_UserId);
  strcpy(data3.InvestorID, m_InvestorID);
  
  pTraderApi->ReqQryTrade(&data3, m_RequestId++);

}

void CFemasTraderHandler::QryUserInvestor()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryUserInvestorField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.BrokerID, m_BrokerID);
  strcpy(data3.UserID, m_UserId);
  
  pTraderApi->ReqQryUserInvestor(&data3, m_RequestId++);

}

void CFemasTraderHandler::QryInvestorAccount()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryInvestorAccountField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.BrokerID, m_BrokerID);
  strcpy(data3.UserID, m_UserId);
  strcpy(data3.InvestorID, m_InvestorID);
  
  pTraderApi->ReqQryInvestorAccount(&data3, m_RequestId++);

}

void CFemasTraderHandler::QryInstrument()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryInstrumentField data3;
  memset(&data3, 0, sizeof(data3));
  
  pTraderApi->ReqQryInstrument(&data3, m_RequestId++);

}

void CFemasTraderHandler::DSUserCertification()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcDSUserInfoField reqDSUserInfoField;
  
  memset(&reqDSUserInfoField, 0, sizeof(reqDSUserInfoField));
  strncpy(reqDSUserInfoField.AppID, m_AppID, sizeof(reqDSUserInfoField.AppID));
  strncpy(reqDSUserInfoField.AuthCode, m_AuthCode, sizeof(reqDSUserInfoField.AuthCode));
  reqDSUserInfoField.EncryptType = '1';
  pTraderApi->ReqDSUserCertification(&reqDSUserInfoField, m_RequestId++);
}

void CFemasTraderHandler::QryExchange()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryExchangeField data3;
  memset(&data3, 0, sizeof(data3));
  
  pTraderApi->ReqQryExchange(&data3, m_RequestId++);
}

void CFemasTraderHandler::QryInvestorPosition()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryInvestorPositionField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.BrokerID, m_BrokerID);
  strcpy(data3.UserID, m_UserId);
  strcpy(data3.InvestorID, m_InvestorID);
  
  pTraderApi->ReqQryInvestorPosition(&data3, m_RequestId++);

}
void CFemasTraderHandler::SubscribeTopic()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcDisseminationField data3;
  memset(&data3, 0, sizeof(data3));
  
  pTraderApi->ReqSubscribeTopic(&data3, m_RequestId++);
}

void CFemasTraderHandler::PrintTrade(CUstpFtdcTradeField *pTrade)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pTrade){
    CTP_LOG(
      "pTrade->BrokerID=[%s]"
      "pTrade->ExchangeID=[%s]"
      "pTrade->TradingDay=[%s]"
      "pTrade->ParticipantID=[%s]"
      "pTrade->SeatID=[%s]"
      "pTrade->InvestorID=[%s]"
      "pTrade->ClientID=[%s]"
      "pTrade->UserID=[%s]"
      "pTrade->OrderUserID=[%s]"
      "pTrade->TradeID=[%s]"
      "pTrade->OrderSysID=[%s]"
      "pTrade->UserOrderLocalID=[%s]"
      "pTrade->InstrumentID=[%s]"
      "pTrade->Direction=[%c]"
      "pTrade->OffsetFlag=[%c]"
      "pTrade->HedgeFlag=[%c]"
      "pTrade->TradePrice=[%lf]"
      "pTrade->TradeVolume=[%d]"
      "pTrade->TradeTime=[%s]"
      "pTrade->ClearingPartID=[%s]"
      "pTrade->UsedFee=[%lf]"
      "pTrade->UsedMargin=[%lf]"
      "pTrade->Premium=[%lf]"
      "pTrade->Position=[%d]"
      "pTrade->PositionCost=[%lf]"
      "pTrade->Available=[%lf]"
      "pTrade->Margin=[%lf]"
      "pTrade->FrozenMargin=[%lf]"
      "pTrade->BusinessLocalID=[%d]"
      "pTrade->ActionDay=[%s]"
      "pTrade->ArbiType=[%c]"
      "pTrade->ArbiInstrumentID=[%s]"
  	  "\n"
      , pTrade->BrokerID
      , pTrade->ExchangeID
      , pTrade->TradingDay
      , pTrade->ParticipantID
      , pTrade->SeatID
      , pTrade->InvestorID
      , pTrade->ClientID
      , pTrade->UserID
      , pTrade->OrderUserID
      , pTrade->UserOrderLocalID
      , pTrade->InstrumentID
      , pTrade->Direction
      , pTrade->OffsetFlag
      , pTrade->HedgeFlag
      , pTrade->TradePrice
      , pTrade->TradeVolume
      , pTrade->TradeTime
      , pTrade->ClearingPartID
      , pTrade->UsedFee
      , pTrade->UsedMargin
      , pTrade->Premium
      , pTrade->Position
      , pTrade->PositionCost
      , pTrade->Available
      , pTrade->Margin
      , pTrade->FrozenMargin
      , pTrade->BusinessLocalID
      , pTrade->ActionDay
      , pTrade->ArbiType
      , pTrade->ArbiInstrumentID
      );
  }
}

///�����ر�
void CFemasTraderHandler::PrintOrder(CUstpFtdcOrderField *pOrder)
{

  if(pOrder){
    CTP_LOG(
      "pOrder->BrokerID=[%s]"
      "pOrder->ExchangeID=[%s]"
      "pOrder->OrderSysID=[%s]"
      "pOrder->InvestorID=[%s]"
      "pOrder->UserID=[%s]"
      "pOrder->SeatNo=[%d]"
      "pOrder->InstrumentID=[%s]"
      "pOrder->UserOrderLocalID=[%s]"
      "pOrder->OrderPriceType=[%c]"
      "pOrder->Direction=[%c]"
      "pOrder->OffsetFlag=[%c]"
      "pOrder->HedgeFlag=[%c]"
      "pOrder->LimitPrice=[%lf]"
      "pOrder->Volume=[%d]"
      "pOrder->TimeCondition=[%c]"
      "pOrder->GTDDate=[%s]"
      "pOrder->VolumeCondition=[%c]"
      "pOrder->MinVolume=[%d]"
      "pOrder->StopPrice=[%lf]"
      "pOrder->ForceCloseReason=[%c]"
      "pOrder->IsAutoSuspend=[%d]"
      "pOrder->BusinessUnit=[%s]"
      "pOrder->UserCustom=[%s]"
      "pOrder->BusinessLocalID=[%d]"
      "pOrder->ActionDay=[%s]"
      "pOrder->ArbiType=[%d]"
      "pOrder->TradingDay=[%s]"
      "pOrder->ParticipantID=[%s]"
      "pOrder->OrderUserID=[%s]"
      "pOrder->ClientID=[%s]"
      "pOrder->SeatID=[%s]"
      "pOrder->InsertTime=[%s]"
      "pOrder->OrderLocalID=[%s]"
      "pOrder->OrderSource=[%c]"
      "pOrder->OrderStatus=[%c]"
      "pOrder->CancelTime=[%s]"
      "pOrder->CancelUserID=[%s]"
      "pOrder->VolumeTraded=[%d]"
      "pOrder->VolumeRemain=[%d]"
      "pOrder->OrderType=[%c]"
      "pOrder->DeliveryFlag=[%c]"
      "\n"
      , pOrder->BrokerID
      , pOrder->ExchangeID
      , pOrder->OrderSysID
      , pOrder->InvestorID
      , pOrder->UserID
      , pOrder->SeatNo
      , pOrder->InstrumentID
      , pOrder->UserOrderLocalID
      , pOrder->OrderPriceType
      , pOrder->Direction
      , pOrder->OffsetFlag
      , pOrder->HedgeFlag
      , pOrder->LimitPrice
      , pOrder->Volume
      , pOrder->TimeCondition
      , pOrder->GTDDate
      , pOrder->VolumeCondition
      , pOrder->MinVolume
      , pOrder->StopPrice
      , pOrder->ForceCloseReason
      , pOrder->IsAutoSuspend
      , pOrder->BusinessUnit
      , pOrder->UserCustom
      , pOrder->BusinessLocalID
      , pOrder->ActionDay
      , pOrder->ArbiType
      , pOrder->TradingDay
      , pOrder->ParticipantID
      , pOrder->OrderUserID
      , pOrder->ClientID
      , pOrder->SeatID
      , pOrder->InsertTime
      , pOrder->OrderLocalID
      , pOrder->OrderSource
      , pOrder->OrderStatus
      , pOrder->CancelTime
      , pOrder->CancelUserID
      , pOrder->VolumeTraded
      , pOrder->VolumeRemain
      , pOrder->OrderType
      , pOrder->DeliveryFlag
    );
  }
}

void CFemasTraderHandler::PrintInstrument(CUstpFtdcRspInstrumentField *pInstrument)
{
  FILE* fp;
  
  if(!pInstrument){
    return ;
  }

  fp = fopen(m_OutputFile, "a+");
  if(NULL == fp){
    return ;
  }

  fprintf(fp, "%s,%s,%s,%s,%s"
    ",%d,%d,%d,%d,%d"
    ",%d,%d,%lf,%c,%d"
    ",%d,%lf,%lf,%lf,%c"
    ",%s,%s,%s,%s,%s"
    ",%lf,%d,%c,%lf,%c"
    ",%lf,%c,%c,%c,%s"
    ",%c,%lf,%s,%c,%lf"
     "\n",
    ///����������
    pInstrument->ExchangeID,
    ///Ʒ�ִ���
    pInstrument->ProductID,
    ///Ʒ������
    pInstrument->ProductName,
    ///��Լ����
    pInstrument->InstrumentID,
    ///��Լ����
    pInstrument->InstrumentName,
    
    ///�������
    pInstrument->DeliveryYear,
    ///������
    pInstrument->DeliveryMonth,
    ///�޼۵�����µ���
    pInstrument->MaxLimitOrderVolume,
    ///�޼۵���С�µ���
    pInstrument->MinLimitOrderVolume,
    ///�м۵�����µ���
    pInstrument->MaxMarketOrderVolume,
    
    ///�м۵���С�µ���
    pInstrument->MinMarketOrderVolume,
    ///��������
    pInstrument->VolumeMultiple,
    ///���۵�λ
    pInstrument->PriceTick,
    ///����
    pInstrument->Currency,
    ///��ͷ�޲�
    pInstrument->LongPosLimit,
    
    ///��ͷ�޲�
    pInstrument->ShortPosLimit,
    ///��ͣ���
    pInstrument->LowerLimitPrice,
    ///��ͣ���
    pInstrument->UpperLimitPrice,
    ///�����
    pInstrument->PreSettlementPrice,
    ///��Լ����״̬
    pInstrument->InstrumentStatus,
    
    ///������
    pInstrument->CreateDate,
    ///������
    pInstrument->OpenDate,
    ///������
    pInstrument->ExpireDate,
    ///��ʼ������
    pInstrument->StartDelivDate,
    ///��󽻸���
    pInstrument->EndDelivDate,
    
    ///���ƻ�׼��
    pInstrument->BasisPrice,
    ///��ǰ�Ƿ���
    pInstrument->IsTrading,
    ///������Ʒ����
    pInstrument->UnderlyingInstrID,
    ///������Ʒ����
    pInstrument->UnderlyingMultiple,
    ///�ֲ�����
    pInstrument->PositionType,
    
    ///ִ�м�
    pInstrument->StrikePrice,
    ///��Ȩ����
    pInstrument->OptionsType,
    ///���ִ���
    pInstrument->CurrencyID,
    ///�������
    pInstrument->ArbiType,
    ///��һ�Ⱥ�Լ����
    pInstrument->InstrumentID_1,
    
    ///��һ����������
    pInstrument->Direction_1,
    ///��һ����������
    pInstrument->Ratio_1,
    ///�ڶ��Ⱥ�Լ����
    pInstrument->InstrumentID_2,
    ///�ڶ�����������
    pInstrument->Direction_2,
    ///�ڶ�����������
    pInstrument->Ratio_2
    );
 
  fclose(fp);
  return ;
}




