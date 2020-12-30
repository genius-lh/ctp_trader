
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#include "Femas20Test.h"

#define FEMAS20_TEST_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sQryAddress[128];
  char sAddress[128];
  char sAppID[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sPasswd[128];
  int i = 1;

  if(argc < 6){
    printf("input sQryAddress:\n");
    scanf("%s", sQryAddress);
    printf("input sAddress:\n");
    scanf("%s", sAddress);
    printf("input sAppID:\n");
    scanf("%s", sAppID);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
    printf("input sBrokerID:\n");
    scanf("%s", sBrokerID);
    printf("input sUserId:\n");
    scanf("%s", sUserId);
    printf("input sPasswd:\n");
    scanf("%s", sPasswd);
  }else{
    strncpy(sQryAddress, argv[i++], sizeof(sQryAddress));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAppID, argv[i++], sizeof(sAppID));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sPasswd, argv[i++], sizeof(sPasswd));
  }
  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi();

  CFemas20TestHandler* pTraderHandler = new CFemas20TestHandler();
  // 初始化变量
  pTraderHandler->m_AppID = sAppID;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_Passwd = sPasswd;
  pTraderHandler->m_Loop = 1;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);

  pTraderApi->SubscribePublicTopic(USTP_TERT_RESUME);
  
  // 交易
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(sAddress);
  pTraderApi->RegisterQryFront(sQryAddress);
  
  // 连接交易服务器
  pTraderApi->Init();

  sleep(3);
  
  pTraderHandler->Loop();

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}


CFemas20TestHandler::CFemas20TestHandler()
{

}
CFemas20TestHandler::~CFemas20TestHandler()
{

}
void CFemas20TestHandler::OnFrontConnected()
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcDSUserInfoField reqDSUserInfoField;
  
  memset(&reqDSUserInfoField, 0, sizeof(reqDSUserInfoField));
  strncpy(reqDSUserInfoField.AppID, m_AppID, sizeof(reqDSUserInfoField.AppID));
  strncpy(reqDSUserInfoField.AuthCode, m_AuthCode, sizeof(reqDSUserInfoField.AuthCode));
  reqDSUserInfoField.EncryptType = '1';

  pTraderApi->ReqDSUserCertification(&reqDSUserInfoField, m_RequestId++);


}
void CFemas20TestHandler::OnQryFrontConnected()
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);

} 
void CFemas20TestHandler::OnFrontDisconnected(int nReason)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);

}
void CFemas20TestHandler::OnQryFrontDisconnected(int nReason)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);

}

///错误应答
void CFemas20TestHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///风控前置系统用户登录应答
void CFemas20TestHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

void CFemas20TestHandler::OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}


///用户退出应答
void CFemas20TestHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///报单录入应答
void CFemas20TestHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///报单操作应答
void CFemas20TestHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}
 
///成交回报
void CFemas20TestHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  PrintTrade((void *)pTrade);
  return;
}

///报单回报
void CFemas20TestHandler::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  PrintOrder((void *)pOrder);
  return;
}

///报单录入错误回报
void CFemas20TestHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///报单操作错误回报
void CFemas20TestHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///合约交易状态通知
void CFemas20TestHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pInstrumentStatus){
    FEMAS20_TEST_LOG("pInstrumentStatus->InstrumentID=[%s]\n", pInstrumentStatus->InstrumentID);
  }
  return;
}

///报单查询应答
void CFemas20TestHandler::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pOrder){
    PrintOrder((void *)pOrder);
  }
  return ;
}

///成交单查询应答
void CFemas20TestHandler::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pTrade){
    PrintTrade((void *)pTrade);
  }
  return ;
}

///可用投资者账户查询应答
void CFemas20TestHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspUserInvestor){
    FEMAS20_TEST_LOG("pRspUserInvestor->InvestorID=[%s]\n", pRspUserInvestor->InvestorID);
    strncpy(m_InvestorID, pRspUserInvestor->InvestorID, sizeof(m_InvestorID));
  }
  return ;
}

///投资者资金账户查询应答
void CFemas20TestHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspInvestorAccount){


  }
  return ;
}

///合约查询应答
void CFemas20TestHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///交易所查询应答
void CFemas20TestHandler::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;

}

///投资者持仓查询应答
void CFemas20TestHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

///穿透监管客户认证应答
void CFemas20TestHandler::OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS20_TEST_LOG("%s\n", __FUNCTION__);
  
  if(pRspInfo){
    FEMAS20_TEST_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

void CFemas20TestHandler::Loop()
{
  int choose;
  while(m_Loop){
    choose = ShowMenu();
    switch(choose){
    case 1:
      Login();
      break;
    case 2:
      Logout();
      m_Loop = 0;
      sleep(1);
      break;
    case 3:
      OrderInsert();
      break;
    case 4:
      OrderAction();
      break;
    case 5:
      QryOrder();
      break;
    case 6:
      QryTrade();
      break;
    case 7:
      QryUserInvestor();
      break;
    case 8:
      QryInvestorAccount();
      break;
    case 9:
      QryInstrument();
      break;
    case 10:
      QryExchange();
      break;
    case 11:
      QryInvestorPosition();
      break;
    default:
      break;
    }
  }
}
int CFemas20TestHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "选择需要执行的操作\n"
        "1-Login\n"
        "2-Logout\n"
        "3-OrderInsert\n"
        "4-OrderAction\n"
        "5-QryOrder\n"
        "6-QryTrade\n"
        "7-QryUserInvestor\n"
        "8-QryInvestorAccount\n"
        "9-QryInstrument\n"
        "10-QryExchange\n"
        "11-QryInvestorPosition\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}
void CFemas20TestHandler::Login()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strncpy(reqUserLoginField.BrokerID, m_BrokerID, sizeof(reqUserLoginField.BrokerID));
  strncpy(reqUserLoginField.UserID, m_UserId, sizeof(reqUserLoginField.UserID));
  strncpy(reqUserLoginField.Password, m_Passwd, sizeof(reqUserLoginField.Password));
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
  return;
}
void CFemas20TestHandler::Logout()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strncpy(userLogoutField.BrokerID, m_BrokerID, sizeof(userLogoutField.BrokerID));
  strncpy(userLogoutField.UserID, m_UserId, sizeof(userLogoutField.UserID));
  pTraderApi->ReqUserLogout(&userLogoutField, m_RequestId++);
  return;
}
void CFemas20TestHandler::OrderInsert()
{
  char Temp[64];
  char InstrumentID[64];
  char ExchangeID[64];
  char UserOrderLocalID[64];
  char Direction;
  char OffsetFlag;
  char HedgeFlag;
  double LimitPrice;
  int Volume;
  char TimeCondition;

  
  printf("input InstrumentID:\n");
  scanf("%s", InstrumentID);
  printf("input ExchangeID(CFFEX):\n");
  scanf("%s", ExchangeID);
  printf("input UserOrderLocalID:\n");
  scanf("%s", UserOrderLocalID);
  printf("input Direction[0|1]:\n");
  scanf("%s", Temp);
  Direction = Temp[0];
  printf("input OffsetFlag[0|1]:\n");
  scanf("%s", Temp);
  OffsetFlag = Temp[0];
  printf("input HedgeFlag(1):\n");
  scanf("%s", Temp);
  HedgeFlag = Temp[0];
  printf("input LimitPrice:\n");
  scanf("%lf", &LimitPrice);
  printf("input Volume:\n");
  scanf("%d", &Volume);
  printf("input TimeCondition(3):\n");
  scanf("%s", Temp);
  TimeCondition = Temp[0];
  
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcInputOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///经纪公司编号
  strncpy(inputOrderField.BrokerID, m_BrokerID, sizeof(inputOrderField.BrokerID));
	///投资者编号
  strncpy(inputOrderField.InvestorID, m_InvestorID, sizeof(inputOrderField.InvestorID));
	///用户代码
  strncpy(inputOrderField.UserID, m_UserId, sizeof(inputOrderField.UserID));
	///交易所代码
  strncpy(inputOrderField.ExchangeID, ExchangeID, sizeof(inputOrderField.ExchangeID));
	///系统报单编号
	///指定通过此席位序号下单
	///合约代码/套利组合合约号
  strncpy(inputOrderField.InstrumentID, InstrumentID, sizeof(inputOrderField.InstrumentID));
	///用户本地报单号
  strncpy(inputOrderField.UserOrderLocalID, UserOrderLocalID, sizeof(inputOrderField.UserOrderLocalID));
	///报单类型
	inputOrderField.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	///买卖方向
	inputOrderField.Direction = Direction;
	///开平标志
	inputOrderField.OffsetFlag = OffsetFlag;
	///投机套保标志
	inputOrderField.HedgeFlag = HedgeFlag;
	///价格
	inputOrderField.LimitPrice = LimitPrice;
	///数量
	inputOrderField.Volume = Volume;
	///有效期类型
	inputOrderField.TimeCondition = TimeCondition;
	///GTD日期
	///成交量类型
	inputOrderField.VolumeCondition = USTP_FTDC_VC_AV;
	///最小成交量
	inputOrderField.MinVolume = 1;
	///止损价止赢价
	///强平原因
	inputOrderField.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	///自动挂起标志
	///业务单元
	///用户自定义域
	///本地业务标识
	///业务发生日期
	///策略类别
	///客户编码
  pTraderApi->ReqOrderInsert(&inputOrderField, m_RequestId++);
  return;
}
void CFemas20TestHandler::OrderAction()
{
  char ExchangeID[64];
  char UserOrderActionLocalID[64];
  char UserOrderLocalID[64];
  printf("input ExchangeID(CFFEX):\n");
  scanf("%s", ExchangeID);
  printf("input UserOrderActionLocalID:\n");
  scanf("%s", UserOrderActionLocalID);
  printf("input UserOrderLocalID:\n");
  scanf("%s", UserOrderLocalID);

  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcOrderActionField orderActionField;
  memset(&orderActionField, 0, sizeof(orderActionField));
	///经纪公司编号
  strncpy(orderActionField.BrokerID, m_BrokerID, sizeof(orderActionField.BrokerID));
	///用户代码
  strncpy(orderActionField.UserID, m_UserId, sizeof(orderActionField.UserID));
	///投资者编号
  strncpy(orderActionField.InvestorID, m_InvestorID, sizeof(orderActionField.InvestorID));
	///交易所代码
  strncpy(orderActionField.ExchangeID, ExchangeID, sizeof(orderActionField.ExchangeID));
	///系统报单编号
	///本次撤单操作的本地编号
  strncpy(orderActionField.UserOrderActionLocalID, UserOrderActionLocalID, sizeof(orderActionField.UserOrderActionLocalID));
	///被撤订单的本地报单编号
  strncpy(orderActionField.UserOrderLocalID, UserOrderLocalID, sizeof(orderActionField.UserOrderLocalID));
	///报单操作标志
	orderActionField.ActionFlag = USTP_FTDC_AF_Delete;
	///价格
	///数量变化
	///本地业务标识
	///客户编码

  pTraderApi->ReqOrderAction(&orderActionField, m_RequestId++);

}
void CFemas20TestHandler::QryOrder()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcQryOrderField qryOrderField;
  memset(&qryOrderField, 0, sizeof(qryOrderField));
  strncpy(qryOrderField.BrokerID, m_BrokerID, sizeof(qryOrderField.BrokerID));
  strncpy(qryOrderField.UserID, m_UserId, sizeof(qryOrderField.UserID));
  strncpy(qryOrderField.InvestorID, m_InvestorID, sizeof(qryOrderField.InvestorID));
  
  pTraderApi->ReqQryOrder(&qryOrderField, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryTrade()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcQryTradeField qryTradeField;
  memset(&qryTradeField, 0, sizeof(qryTradeField));
  strncpy(qryTradeField.BrokerID, m_BrokerID, sizeof(qryTradeField.BrokerID));
  strncpy(qryTradeField.UserID, m_UserId, sizeof(qryTradeField.UserID));
  strncpy(qryTradeField.InvestorID, m_InvestorID, sizeof(qryTradeField.InvestorID));
  
  pTraderApi->ReqQryTrade(&qryTradeField, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryUserInvestor()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryUserInvestorField qryInvestorField;
  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
  strncpy(qryInvestorField.BrokerID, m_BrokerID, sizeof(qryInvestorField.BrokerID));
  strncpy(qryInvestorField.UserID, m_UserId, sizeof(qryInvestorField.UserID));
  
  pTraderApi->ReqQryUserInvestor(&qryInvestorField, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryInvestorAccount()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  
  CUstpFtdcQryInvestorAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));
  
  strncpy(qryTradingAccountField.BrokerID, m_BrokerID, sizeof(qryTradingAccountField.BrokerID));
  strncpy(qryTradingAccountField.UserID, m_UserId, sizeof(qryTradingAccountField.UserID));
  strncpy(qryTradingAccountField.InvestorID, m_InvestorID, sizeof(qryTradingAccountField.InvestorID));
  
  pTraderApi->ReqQryInvestorAccount(&qryTradingAccountField, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryInstrument()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;

  CUstpFtdcQryInstrumentField qryInstrumentField;
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));

  pTraderApi->ReqQryInstrument(&qryInstrumentField, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryExchange()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcQryExchangeField qryExchange;
  memset(&qryExchange, 0, sizeof(CUstpFtdcQryExchangeField));

  pTraderApi->ReqQryExchange(&qryExchange, m_RequestId++);
  return;
}
void CFemas20TestHandler::QryInvestorPosition()
{
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcQryInvestorPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

  strncpy(qryInvestorPositionField.BrokerID, m_BrokerID, sizeof(qryInvestorPositionField.BrokerID));
  strncpy(qryInvestorPositionField.UserID, m_UserId, sizeof(qryInvestorPositionField.UserID));
  strncpy(qryInvestorPositionField.InvestorID, m_InvestorID, sizeof(qryInvestorPositionField.InvestorID));

  pTraderApi->ReqQryInvestorPosition(&qryInvestorPositionField, m_RequestId++);
  return;
}
void CFemas20TestHandler::PrintOrder(void* data)
{
  CUstpFtdcOrderField* pOrder = (CUstpFtdcOrderField*)data;
  
  FEMAS20_TEST_LOG("pOrder->OrderSysID[%s]"
    "pOrder->InstrumentID[%s]"
    "pOrder->UserOrderLocalID[%s]"
    "pOrder->Direction[%c]"
    "pOrder->OffsetFlag[%c]"
    "pOrder->LimitPrice[%lf]"
    "pOrder->Volume[%d]"
    "pOrder->OrderLocalID[%s]"
    "pOrder->OrderStatus[%c]"
    "pOrder->CancelTime[%s]"
    "pOrder->VolumeTraded[%d]"
    "pOrder->VolumeRemain[%d]"
    "\n"
    , pOrder->OrderSysID
    , pOrder->InstrumentID
    , pOrder->UserOrderLocalID
    , pOrder->Direction
    , pOrder->OffsetFlag
    , pOrder->LimitPrice
    , pOrder->Volume
    , pOrder->OrderLocalID
    , pOrder->OrderStatus
    , pOrder->CancelTime
    , pOrder->VolumeTraded
    , pOrder->VolumeRemain
    );

}
void CFemas20TestHandler::PrintTrade(void* data)
{
  CUstpFtdcTradeField *pTrade = (CUstpFtdcTradeField *)data;

  FEMAS20_TEST_LOG("pOrder->TradeID[%s]"
    "pOrder->OrderSysID[%s]"
    "pOrder->UserOrderLocalID[%s]"
    "pOrder->InstrumentID[%s]"
    "pOrder->Direction[%c]"
    "pOrder->OffsetFlag[%c]"
    "pOrder->TradePrice[%lf]"
    "pOrder->TradeVolume[%d]"
    "pOrder->TradeTime[%s]"
    "pOrder->UsedFee[%lf]"
    "pOrder->UsedMargin[%lf]"
    "\n"
    , pTrade->TradeID
    , pTrade->OrderSysID
    , pTrade->UserOrderLocalID
    , pTrade->InstrumentID
    , pTrade->Direction
    , pTrade->OffsetFlag
    , pTrade->TradePrice
    , pTrade->TradeVolume
    , pTrade->TradeTime
    , pTrade->UsedFee
    , pTrade->UsedMargin
    );
}

