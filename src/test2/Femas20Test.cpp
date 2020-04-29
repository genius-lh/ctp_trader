
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
  // 初始化变量
  // 交易
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

  // 连接交易服务器
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

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CFemasTraderHandler::OnFrontConnected()
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(0 == m_Loop){
    return ;
  }
  DSUserCertification();
  return ;
}


///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CFemasTraderHandler::OnFrontDisconnected(int nReason)
{
  CTP_LOG("%s\n", __FUNCTION__);
}
  
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
//void OnHeartBeatWarning(int nTimeLapse){};

///报文回调开始通知。当API收到一个报文后，首先调用本方法，然后是各数据域的回调，最后是报文回调结束通知。
///@param nTopicID 主题代码（如私有流、公共流、行情流等）
///@param nSequenceNo 报文序号
//void OnPackageStart(int nTopicID, int nSequenceNo){};

///报文回调结束通知。当API收到一个报文后，首先调用报文回调开始通知，然后是各数据域的回调，最后调用本方法。
///@param nTopicID 主题代码（如私有流、公共流、行情流等）
///@param nSequenceNo 报文序号
//void OnPackageEnd(int nTopicID, int nSequenceNo){};

///错误应答
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

///风控前置系统用户登录应答
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

///用户退出应答
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

///用户密码修改应答
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

///报单录入应答
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

///报单操作应答
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

///报价录入应答
//void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///报价操作应答
//void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///询价请求应答
//void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///客户申请组合应答
//void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///用户请求出入金应答
//void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///用户主次席出入金应答
//void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///数据流回退通知
//void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {};

///成交回报
void CFemasTraderHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintTrade(pTrade);
}

///报单回报
void CFemasTraderHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintOrder(pOrder);
}

///报单录入错误回报
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

///报单操作错误回报
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

///合约交易状态通知
void CFemasTraderHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pInstrumentStatus){


  }
}

///账户出入金回报
//void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {};

///报价回报
//void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {};

///报价录入错误回报
//void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {};

///报价撤单错误回报
//void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo) {};

///询价回报
//void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {};

///增加组合规则通知
//void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg) {};

///客户申请组合确认
//void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction) {};

///用户请求出入金
//void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit) {};

///报单查询应答
void CFemasTraderHandler::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintOrder(pOrder);
}

///成交单查询应答
void CFemasTraderHandler::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  PrintTrade(pTrade);
}

///可用投资者账户查询应答
void CFemasTraderHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
	///经纪公司编号
	TUstpFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TUstpFtdcUserIDType	UserID;
	///投资者编号
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

///交易编码查询应答
//void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///投资者资金账户查询应答
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

///合约查询应答
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

///交易所查询应答
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

///投资者持仓查询应答
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

///订阅主题应答
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

///合规参数查询应答
//void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///主题查询应答
//void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///投资者手续费率查询应答
//void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///投资者保证金率查询应答
//void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///交易编码组合持仓查询应答
//void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///交易编码单腿持仓查询应答
//void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///穿透监管客户认证应答
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

///穿透监管信息采集中继上传信息响应
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
        "选择需要执行的操作\n"
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
        "请选择："
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
  printf("请输入新密码:");
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
  
	///经纪公司代码
	strncpy(inputOrderField.BrokerID, m_BrokerID, sizeof(inputOrderField.BrokerID));
	///交易所代码
	strncpy(inputOrderField.ExchangeID, exchangeID, sizeof(inputOrderField.ExchangeID));
	///投资者代码
	strncpy(inputOrderField.InvestorID, m_InvestorID, sizeof(inputOrderField.InvestorID));
	///用户代码
	strncpy(inputOrderField.UserID, m_UserId, sizeof(inputOrderField.UserID));
	///合约代码
	strncpy(inputOrderField.InstrumentID, instrumentID, sizeof(inputOrderField.InstrumentID));
	///报单引用
	strncpy(inputOrderField.UserOrderLocalID, userOrderLocalID, sizeof(inputOrderField.UserOrderLocalID));
	///报单价格条件
	inputOrderField.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	///买卖方向
	inputOrderField.Direction = direction[0];
	///组合开平标志
	inputOrderField.OffsetFlag = offsetFlag[0];
	///组合投机套保标志
	inputOrderField.HedgeFlag = USTP_FTDC_CHF_Speculation;
	///价格
	inputOrderField.LimitPrice = limitPrice;
	///数量
	inputOrderField.Volume = volume;
	///有效期类型
	inputOrderField.TimeCondition = USTP_FTDC_TC_GFD;
	///成交量类型
	inputOrderField.VolumeCondition = USTP_FTDC_VC_AV;
	///最小成交量
	inputOrderField.MinVolume = 1;
	///强平原因
	inputOrderField.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	///自动挂起标志
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
	///交易所代码
	strncpy(data3.ExchangeID, exchangeID, sizeof(data3.ExchangeID));
	///系统报单编号
	strncpy(data3.OrderSysID, orderSysID, sizeof(data3.InvestorID));
	///经纪公司编号
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者编号
	strncpy(data3.InvestorID, m_InvestorID, sizeof(data3.InvestorID));
	///用户代码
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));
	///本次撤单操作的本地编号
	strncpy(data3.UserOrderActionLocalID, userOrderLocalID, sizeof(data3.UserOrderActionLocalID));
	///被撤订单的本地报单编号
	//strncpy(data3.UserOrderLocalID, userOrderLocalID, sizeof(data3.UserOrderLocalID));
	///报单操作标志
	data3.ActionFlag = USTP_FTDC_AF_Delete;
	///价格
	///数量变化
	///本地业务标识
	///客户编码

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

///报单回报
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
    ///交易所代码
    pInstrument->ExchangeID,
    ///品种代码
    pInstrument->ProductID,
    ///品种名称
    pInstrument->ProductName,
    ///合约代码
    pInstrument->InstrumentID,
    ///合约名称
    pInstrument->InstrumentName,
    
    ///交割年份
    pInstrument->DeliveryYear,
    ///交割月
    pInstrument->DeliveryMonth,
    ///限价单最大下单量
    pInstrument->MaxLimitOrderVolume,
    ///限价单最小下单量
    pInstrument->MinLimitOrderVolume,
    ///市价单最大下单量
    pInstrument->MaxMarketOrderVolume,
    
    ///市价单最小下单量
    pInstrument->MinMarketOrderVolume,
    ///数量乘数
    pInstrument->VolumeMultiple,
    ///报价单位
    pInstrument->PriceTick,
    ///币种
    pInstrument->Currency,
    ///多头限仓
    pInstrument->LongPosLimit,
    
    ///空头限仓
    pInstrument->ShortPosLimit,
    ///跌停板价
    pInstrument->LowerLimitPrice,
    ///涨停板价
    pInstrument->UpperLimitPrice,
    ///昨结算
    pInstrument->PreSettlementPrice,
    ///合约交易状态
    pInstrument->InstrumentStatus,
    
    ///创建日
    pInstrument->CreateDate,
    ///上市日
    pInstrument->OpenDate,
    ///到期日
    pInstrument->ExpireDate,
    ///开始交割日
    pInstrument->StartDelivDate,
    ///最后交割日
    pInstrument->EndDelivDate,
    
    ///挂牌基准价
    pInstrument->BasisPrice,
    ///当前是否交易
    pInstrument->IsTrading,
    ///基础商品代码
    pInstrument->UnderlyingInstrID,
    ///基础商品乘数
    pInstrument->UnderlyingMultiple,
    ///持仓类型
    pInstrument->PositionType,
    
    ///执行价
    pInstrument->StrikePrice,
    ///期权类型
    pInstrument->OptionsType,
    ///币种代码
    pInstrument->CurrencyID,
    ///策略类别
    pInstrument->ArbiType,
    ///第一腿合约代码
    pInstrument->InstrumentID_1,
    
    ///第一腿买卖方向
    pInstrument->Direction_1,
    ///第一腿数量比例
    pInstrument->Ratio_1,
    ///第二腿合约代码
    pInstrument->InstrumentID_2,
    ///第二腿买卖方向
    pInstrument->Direction_2,
    ///第二腿数量比例
    pInstrument->Ratio_2
    );
 
  fclose(fp);
  return ;
}




