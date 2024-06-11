
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include <string>
#include <iostream>
#include "EesTraderDefine.h"
#include "EesTraderErr.h"
#include "EesTraderApi.h"

#include "EesQueryDefine.h"
#include "EesQueryErr.h"
#include "EesQueryApi.h"

#include "Rem5Test.h"

#define REM_TEST_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char remoteTradeIp[128];
  char remoteTradeTCPPort[128];
  char remoteTradeUDPPort[128];
  char remoteQueryIp[128];
  char remoteQueryTCPPort[128];
  char localTradeIp[128];
  char localTradeUDPPort[128];
  
  char sAppID[128];
  char sAuthCode[128];
  char sUserId[128];
  char sPasswd[128];
  int i = 1;

  if(argc < 12){
    printf("input remoteTradeIp:\n");
    scanf("%s", remoteTradeIp);
    printf("input remoteTradeTCPPort:\n");
    scanf("%s", remoteTradeTCPPort);
    printf("input remoteTradeUDPPort:\n");
    scanf("%s", remoteTradeUDPPort);
    printf("input remoteQueryIp:\n");
    scanf("%s", remoteQueryIp);
    printf("input remoteQueryTCPPort:\n");
    scanf("%s", remoteQueryTCPPort);
    printf("input localTradeIp:\n");
    scanf("%s", localTradeIp);
    printf("input localTradeUDPPort:\n");
    scanf("%s", localTradeUDPPort);
    printf("input sAppID:\n");
    scanf("%s", sAppID);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
    printf("input sUserId:\n");
    scanf("%s", sUserId);
    printf("input sPasswd:\n");
    scanf("%s", sPasswd);
  }else{
    strncpy(remoteTradeIp, argv[i++], sizeof(remoteTradeIp));
    strncpy(remoteTradeTCPPort, argv[i++], sizeof(remoteTradeTCPPort));
    strncpy(remoteTradeUDPPort, argv[i++], sizeof(remoteTradeUDPPort));
    strncpy(remoteQueryIp, argv[i++], sizeof(remoteQueryIp));
    strncpy(remoteQueryTCPPort, argv[i++], sizeof(remoteQueryTCPPort));
    strncpy(localTradeIp, argv[i++], sizeof(localTradeIp));
    strncpy(localTradeUDPPort, argv[i++], sizeof(localTradeUDPPort));
    strncpy(sAppID, argv[i++], sizeof(sAppID));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sPasswd, argv[i++], sizeof(sPasswd));
  }

  EESTraderApi* pTraderApi = CreateEESTraderApi();
  EESQueryApi* pQueryApi = CreateEESQueryApi();

  CRem5TestHandler* pTestHandler = new CRem5TestHandler();

  CRem5TraderHandler* pTraderHandler = new CRem5TraderHandler(pTestHandler);
  CRem5QueryHandler* pQueryHandler = new CRem5QueryHandler(pTestHandler);

  // 初始化变量
  pTestHandler->m_AppID = sAppID;
  pTestHandler->m_AuthCode = sAuthCode;
  pTestHandler->m_UserId = sUserId;
  pTestHandler->m_Passwd = sPasswd;
  pTestHandler->m_Loop = 1;

  pTestHandler->m_TraderApi = (void*)pTraderApi;
  pTestHandler->m_QueryApi = (void*)pQueryApi;
  
  pTestHandler->m_RequestId = 1;

  RESULT ret_err;
	EES_TradeSvrInfo svrInfo;
	strncpy(svrInfo.m_RemoteTradeIp, remoteTradeIp, sizeof(svrInfo.m_RemoteTradeIp));
	svrInfo.m_RemoteTradeTCPPort = atoi(remoteTradeTCPPort);
	svrInfo.m_RemoteTradeUDPPort = atoi(remoteTradeUDPPort);
	strncpy(svrInfo.m_LocalTradeIp, localTradeIp, sizeof(svrInfo.m_LocalTradeIp));
	svrInfo.m_LocalTradeUDPPort = atoi(localTradeUDPPort);
  
  ret_err = pTraderApi->ConnServer(svrInfo, pTraderHandler);
	if (ret_err != NO_ERROR)
	{
		REM_TEST_LOG("connect to REM server failed!\n");
		return false;
	}

  EES_QuerySvrInfo querySvrInfo;
	strncpy(querySvrInfo.m_RemoteQueryIp, remoteQueryIp, sizeof(querySvrInfo.m_RemoteQueryIp));
	querySvrInfo.m_RemoteQueryTCPPort = atoi(remoteQueryTCPPort);
  
  ret_err = pQueryApi->ConnQueryServer(querySvrInfo, pQueryHandler);
	if (ret_err != NO_ERROR)
	{
		REM_TEST_LOG("connect to REM server failed!\n");
		return false;
	}

  sleep(3);
  
  pTestHandler->Loop();

  pTraderApi->DisConnServer();
  pQueryApi->DisConnQueryServer();
  
  DestroyEESTraderApi(pTraderApi);
  DestroyEESQueryApi(pQueryApi);
  
  return 0;  
}

CRem5TraderHandler::CRem5TraderHandler(void* handler)
:m_Handler(handler)
{

}

CRem5TraderHandler::~CRem5TraderHandler()
{

}

void CRem5TraderHandler::OnConnection(ERR_NO errNo, const char* pErrStr )
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5TraderHandler::OnDisConnection(ERR_NO errNo, const char* pErrStr )
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5TraderHandler::OnUserLogon(EES_LogonResponse* pLogon, MY_SOCKET m_sock)
{
  REM_TEST_LOG("pLogon->m_Result=[%d]\n"
    "pLogon->m_UserId=[%d]\n"
    "pLogon->m_TradingDate=[%d]\n"
    "pLogon->m_MaxToken=[%d]\n"
    "pLogon->m_OrderFCCount=[%d]\n"
    "pLogon->m_OrderFCInterval=[%d]\n"
    "pLogon->m_CancelFCCount=[%d]\n"
    "pLogon->m_CancelFCCount=[%d]\n"
    ,pLogon->m_Result
    ,pLogon->m_UserId
    ,pLogon->m_TradingDate
    ,pLogon->m_MaxToken
    ,pLogon->m_OrderFCCount
    ,pLogon->m_OrderFCInterval
    ,pLogon->m_CancelFCCount
    ,pLogon->m_CancelFCInterval
  );
}

void CRem5TraderHandler::OnOrderAccept(EES_OrderAcceptField* pAccept )
{
    REM_TEST_LOG(
      "pAccept->m_ClientOrderToken=[%d]\n"
      "pAccept->m_MarketOrderToken=[%lld]\n"
      "pAccept->m_OrderState=[%d]\n"
      "pAccept->m_UserID=[%d]\n"
      "pAccept->m_AcceptTime=[%lld]\n"
      "pAccept->m_Account=[%s]\n"
      "pAccept->m_Side=[%d]\n"
      "pAccept->m_Exchange=[%d]\n"
      "pAccept->m_Symbol=[%s]\n"
      "pAccept->m_SecType=[%d]\n"
      "pAccept->m_Price=[%lf]\n"
      "pAccept->m_Qty=[%d]\n"
  //    "pAccept->m_OptExecFlag=[%d]\n"
      "pAccept->m_Tif=[%d]\n"
      "pAccept->m_MinQty=[%d]\n"
      "pAccept->m_CustomField=[%lld]\n"
      "pAccept->m_MarketSessionId=[%d]\n"
      "pAccept->m_HedgeFlag=[%d]\n"
      , pAccept->m_ClientOrderToken
      , pAccept->m_MarketOrderToken
      , pAccept->m_OrderState
      , pAccept->m_UserID
      , pAccept->m_AcceptTime
      , pAccept->m_Account
      , pAccept->m_Side
      , pAccept->m_Exchange
      , pAccept->m_Symbol
      , pAccept->m_SecType
      , pAccept->m_Price
      , pAccept->m_Qty
  //    , pAccept->m_OptExecFlag
      , pAccept->m_Tif
      , pAccept->m_MinQty
      , pAccept->m_CustomField
      , pAccept->m_MarketSessionId
      , pAccept->m_HedgeFlag
    );
}

void CRem5TraderHandler::OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept)
{
  REM_TEST_LOG(
    "pAccept->m_Account=[%s]\n"
    "pAccept->m_MarketOrderToken=[%lld]\n"
    "pAccept->m_MarketOrderId=[%s]\n"
    "pAccept->m_MarketTime=[%lld]\n"
    "pAccept->m_UserID=[%d]\n"
    "pAccept->m_ClientOrderToken=[%d]\n"
    "pAccept->m_ExchangeStatus=[%d]\n"
    , pAccept->m_Account
    , pAccept->m_MarketOrderToken
    , pAccept->m_MarketOrderId
    , pAccept->m_MarketTime
    , pAccept->m_UserID
    , pAccept->m_ClientOrderToken
    , pAccept->m_ExchangeStatus
  );

}

void CRem5TraderHandler::OnOrderMarketReport(EES_OrderMarketReportField* pReport)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5TraderHandler::OnOrderMarketReject(EES_OrderMarketRejectField* pReject)
{
  REM_TEST_LOG(
    "pReject->m_Userid=[%d]\n"
    "pReject->m_Timestamp=[%lld]\n"
    "pReject->m_ClientOrderToken=[%d]\n"
    "pReject->m_RejectedMan=[%d]\n"
    "pReject->m_ReasonCode=[%d]\n"
    "pReject->m_GrammerResult=[%s]\n"
    "pReject->m_RiskResult=[%s]\n"
    "pReject->m_GrammerText=[%s]\n"
    "pReject->m_RiskText=[%s]\n"
    , pReject->m_ClientOrderToken
    , pReject->m_Timestamp
    , pReject->m_ClientOrderToken
    , pReject->m_RejectedMan
    , pReject->m_ReasonCode
    , pReject->m_GrammerResult
    , pReject->m_RiskResult
    , pReject->m_GrammerText
    , pReject->m_RiskText
  );
}

void CRem5TraderHandler::OnOrderReject(EES_OrderRejectField* pReject )
{
  REM_TEST_LOG(
    "pReject->m_Userid=[%d]\n"
    "pReject->m_Timestamp=[%lld]\n"
    "pReject->m_ClientOrderToken=[%d]\n"
    "pReject->m_RejectedMan=[%d]\n"
    "pReject->m_ReasonCode=[%d]\n"
    "pReject->m_GrammerResult=[%s]\n"
    "pReject->m_RiskResult=[%s]\n"
    "pReject->m_GrammerText=[%s]\n"
    "pReject->m_RiskText=[%s]\n"
    , pReject->m_ClientOrderToken
    , pReject->m_Timestamp
    , pReject->m_ClientOrderToken
    , pReject->m_RejectedMan
    , pReject->m_ReasonCode
    , pReject->m_GrammerResult
    , pReject->m_RiskResult
    , pReject->m_GrammerText
    , pReject->m_RiskText
  );
}

void CRem5TraderHandler::OnOrderExecution(EES_OrderExecutionField* pExec )
{
  REM_TEST_LOG(
    "pExec->m_Userid=[%d]\n"
    "pExec->m_Timestamp=[%lld]\n"
    "pExec->m_ClientOrderToken=[%d]\n"
    "pExec->m_MarketOrderToken=[%lld]\n"
    "pExec->m_Quantity=[%d]\n"
    "pExec->m_Price=[%lf]\n"
    "pExec->m_ExecutionID=[%lld]\n"
    "pExec->m_MarketExecID=[%s]\n"
    , pExec->m_Userid
    , pExec->m_Timestamp
    , pExec->m_ClientOrderToken
    , pExec->m_MarketOrderToken
    , pExec->m_Quantity
    , pExec->m_Price
    , pExec->m_ExecutionID
    , pExec->m_MarketExecID
  );
}

void CRem5TraderHandler::OnOrderActionAccept(EES_OrderActionAccept* pCxled )
{

}

void CRem5TraderHandler::OnOrderActonReject(EES_OrderActionRej* pReject )
{

}

CRem5QueryHandler::CRem5QueryHandler(void* handler)
:m_Handler(handler)
{

}

CRem5QueryHandler::~CRem5QueryHandler()
{

}

void CRem5QueryHandler::OnQueryConnection(ERR_NO errNo, const char* pErrStr )
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5QueryHandler::OnQueryDisConnection(ERR_NO errNo, const char* pErrStr )
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5QueryHandler::OnQueryUserLogon(EES_QueryLogonResponse* pLogon)
{
  REM_TEST_LOG("pLogon->m_Result=[%d]\n"
    "pLogon->m_UserId=[%d]\n"
    ,pLogon->m_Result
    ,pLogon->m_UserId
  );

}

void CRem5QueryHandler::OnQueryRspChangePassword(EES_ChangePasswordResult nResult)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
}

void CRem5QueryHandler::OnQueryUserAccount(EES_AccountInfo * pAccoutnInfo, bool bFinish)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  if(!pAccoutnInfo){
    return;
  }

  if('\0' == pAccoutnInfo->m_Account[0]){
    return ;
  }
  
  strncpy(m_InvestorID, pAccoutnInfo->m_Account, sizeof(m_InvestorID));
	
  REM_TEST_LOG(
    "pAccoutnInfo->m_Account=[%s]\n"
    "pAccoutnInfo->m_Previlege=[%d]\n"
    "pAccoutnInfo->m_InitialBp=[%lf]\n"
    "pAccoutnInfo->m_AvailableBp=[%lf]\n"
    "pAccoutnInfo->m_Margin=[%lf]\n"
    "pAccoutnInfo->m_FrozenMargin=[%lf]\n"
    "pAccoutnInfo->m_CommissionFee=[%lf]\n"
    "pAccoutnInfo->m_FrozenCommission=[%lf]\n"
    ,pAccoutnInfo->m_Account
    ,pAccoutnInfo->m_Previlege
    ,pAccoutnInfo->m_InitialBp
    ,pAccoutnInfo->m_AvailableBp
    ,pAccoutnInfo->m_Margin
    ,pAccoutnInfo->m_FrozenMargin
    ,pAccoutnInfo->m_CommissionFee
    ,pAccoutnInfo->m_FrozenCommission
  );

}

void CRem5QueryHandler::OnQueryAccountPosition(const char* pAccount, EES_AccountPosition* pAccoutnPosition, bool bFinish)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);

  REM_TEST_LOG(
    "pAccoutnPosition->pAccount=[%s]\n"
    , pAccount);
  

  REM_TEST_LOG(
    "pAccoutnPosition->m_actId=[%s]\n"
    "pAccoutnPosition->m_Symbol=[%s]\n"
    "pAccoutnPosition->m_PosiDirection=[%d]\n"
    "pAccoutnPosition->m_InitOvnQty=[%d]\n"
    "pAccoutnPosition->m_OvnQty=[%d]\n"
    "pAccoutnPosition->m_FrozenOvnQty=[%d]\n"
    "pAccoutnPosition->m_TodayQty=[%d]\n"
    "pAccoutnPosition->m_FrozenTodayQty=[%d]\n"
    "pAccoutnPosition->m_OvnMargin=[%lf]\n"
    "pAccoutnPosition->m_TodayMargin=[%lf]\n"
    "pAccoutnPosition->m_PositionCost=[%lf]\n"
    "pAccoutnPosition->m_HedgeFlag=[%d]\n"
    ,pAccoutnPosition->m_actId
    ,pAccoutnPosition->m_Symbol
    ,pAccoutnPosition->m_PosiDirection
    ,pAccoutnPosition->m_InitOvnQty
    ,pAccoutnPosition->m_OvnQty
    ,pAccoutnPosition->m_FrozenOvnQty
    ,pAccoutnPosition->m_TodayQty
    ,pAccoutnPosition->m_FrozenTodayQty
    ,pAccoutnPosition->m_OvnMargin
    ,pAccoutnPosition->m_TodayMargin
    ,pAccoutnPosition->m_PositionCost
    ,pAccoutnPosition->m_HedgeFlag
  );

}

void CRem5QueryHandler::OnQueryAccountBP(const char* pAccount, EES_AccountBP* pAccoutnBP)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  REM_TEST_LOG(
    "pAccoutnPosition->pAccount=[%s]\n"
    , pAccount);
  
  REM_TEST_LOG(
    "pAccoutnPosition->m_account=[%s]\n"
    "pAccoutnPosition->m_InitialBp=[%lf]\n"
    "pAccoutnPosition->m_AvailableBp=[%lf]\n"
    "pAccoutnPosition->m_Margin=[%lf]\n"
    "pAccoutnPosition->m_FrozenMargin=[%lf]\n"
    "pAccoutnPosition->m_CommissionFee=[%lf]\n"
    "pAccoutnPosition->m_FrozenCommission=[%lf]\n"
    "pAccoutnPosition->m_OvnInitMargin=[%lf]\n"
    "pAccoutnPosition->m_TotalLiquidPL=[%lf]\n"
    "pAccoutnPosition->m_TotalMarketPL=[%lf]\n"
    ,pAccoutnPosition->m_account
    ,pAccoutnPosition->m_InitialBp
    ,pAccoutnPosition->m_AvailableBp
    ,pAccoutnPosition->m_Margin
    ,pAccoutnPosition->m_FrozenMargin
    ,pAccoutnPosition->m_CommissionFee
    ,pAccoutnPosition->m_FrozenCommission
    ,pAccoutnPosition->m_OvnInitMargin
    ,pAccoutnPosition->m_TotalLiquidPL
    ,pAccoutnPosition->m_TotalMarketPL
  );

}

void CRem5QueryHandler::OnQuerySymbol(EES_SymbolField* pSymbol, bool bFinish)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  
  REM_TEST_LOG(
    "pSymbol->m_SecType=[%c]\n"
    "pSymbol->m_symbol=[%s]\n"
    "pSymbol->m_symbolName=[%s]\n"
    "pSymbol->m_ExchangeID=[%d]\n"
    "pSymbol->m_ProdID=[%s]\n"
    "pSymbol->m_DeliveryYear=[%d]\n"
    "pSymbol->m_DeliveryMonth=[%d]\n"
    "pSymbol->m_MaxMarketOrderVolume=[%d]\n"
    "pSymbol->m_MinMarketOrderVolume=[%d]\n"
    "pSymbol->m_MaxLimitOrderVolume=[%d]\n"
    "pSymbol->m_MinLimitOrderVolume=[%d]\n"
    "pSymbol->m_VolumeMultiple=[%d]\n"
    "pSymbol->m_PriceTick=[%lf]\n"
    "pSymbol->m_CreateDate=[%d]\n"
    "pSymbol->m_OpenDate=[%d]\n"
    "pSymbol->m_ExpireDate=[%d]\n"
    "pSymbol->m_StartDelivDate=[%d]\n"
    "pSymbol->m_EndDelivDate=[%d]\n"
    "pSymbol->m_InstLifePhase=[%d]\n"
    "pSymbol->m_IsTrading=[%d]\n"
    "pSymbol->m_StrikePrice=[%lf]\n"
    "pSymbol->m_CallPut=[%d]\n"
    "pSymbol->m_UnderlyingSymbol=[%s]\n"
    , pSymbol->m_SecType
    , pSymbol->m_symbol
    , pSymbol->m_symbolName
    , pSymbol->m_ExchangeID
    , pSymbol->m_ProdID
    , pSymbol->m_DeliveryYear
    , pSymbol->m_DeliveryMonth
    , pSymbol->m_MaxMarketOrderVolume
    , pSymbol->m_MinMarketOrderVolume
    , pSymbol->m_MaxLimitOrderVolume
    , pSymbol->m_MinLimitOrderVolume
    , pSymbol->m_VolumeMultiple
    , pSymbol->m_PriceTick
    , pSymbol->m_CreateDate
    , pSymbol->m_OpenDate
    , pSymbol->m_ExpireDate
    , pSymbol->m_StartDelivDate
    , pSymbol->m_EndDelivDate
    , pSymbol->m_InstLifePhase
    , pSymbol->m_IsTrading
    , pSymbol->m_StrikePrice
    , pSymbol->m_CallPut
    , pSymbol->m_UnderlyingSymbol
  );

}

void CRem5QueryHandler::OnQueryTradeOrder(const char* pAccount, EES_QueryAccountOrder* pQueryOrder, bool bFinish)
{
    REM_TEST_LOG("%s\n", __FUNCTION__);
  
    REM_TEST_LOG(
      "pQueryOrder->m_Userid=[%d]\n"
      "pQueryOrder->m_Timestamp=[%lld]\n"
      "pQueryOrder->m_ClientOrderToken=[%d]\n"
      "pQueryOrder->m_SideType=[%d]\n"
      "pQueryOrder->m_Quantity=[%d]\n"
      "pQueryOrder->m_InstrumentType=[%d]\n"
      "pQueryOrder->m_symbol=[%s]\n"
      "pQueryOrder->m_Price=[%lf]\n"
      "pQueryOrder->m_account=[%s]\n"
      "pQueryOrder->m_ExchengeID=[%d]\n"
  //    "pQueryOrder->m_OptExecFlag=[%d]\n"
      "pQueryOrder->m_MarketOrderToken=[%lld]\n"
      "pQueryOrder->m_OrderStatus=[%d]\n"
      "pQueryOrder->m_CloseTime=[%lld]\n"
      "pQueryOrder->m_FilledQty=[%d]\n"
      "pQueryOrder->m_Tif=[%d]\n"
      "pQueryOrder->m_MinQty=[%d]\n"
      "pQueryOrder->m_CustomField=[%lld]\n"
      "pQueryOrder->m_MarketOrderId=[%s]\n"
      "pQueryOrder->m_HedgeFlag=[%d]\n"
      , pQueryOrder->m_Userid
      , pQueryOrder->m_Timestamp
      , pQueryOrder->m_ClientOrderToken
      , pQueryOrder->m_SideType
      , pQueryOrder->m_Quantity
      , pQueryOrder->m_InstrumentType
      , pQueryOrder->m_symbol
      , pQueryOrder->m_Price
      , pQueryOrder->m_account
      , pQueryOrder->m_ExchengeID
  //    , pQueryOrder->m_OptExecFlag
      , pQueryOrder->m_MarketOrderToken
      , pQueryOrder->m_OrderStatus
      , pQueryOrder->m_CloseTime
      , pQueryOrder->m_FilledQty
      , pQueryOrder->m_Tif
      , pQueryOrder->m_MinQty
      , pQueryOrder->m_CustomField
      , pQueryOrder->m_MarketOrderId
      , pQueryOrder->m_HedgeFlag
    );
}

void CRem5QueryHandler:: OnQueryTradeOrderExec(const char* pAccount, EES_QueryOrderExecution* pQueryOrderExec, bool bFinish)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  
  REM_TEST_LOG(
    "pQueryOrderExec->m_Userid=[%d]\n"
    "pQueryOrderExec->m_Timestamp=[%lld]\n"
    "pQueryOrderExec->m_ClientOrderToken=[%d]\n"
    "pQueryOrderExec->m_MarketOrderToken=[%lld]\n"
    "pQueryOrderExec->m_ExecutedQuantity=[%d]\n"
    "pQueryOrderExec->m_ExecutionPrice=[%lf]\n"
    "pQueryOrderExec->m_ExecutionID=[%lld]\n"
    "pQueryOrderExec->m_MarketExecID=[%s]\n"
    , pQueryOrderExec->m_Userid
    , pQueryOrderExec->m_Timestamp
    , pQueryOrderExec->m_ClientOrderToken
    , pQueryOrderExec->m_MarketOrderToken
    , pQueryOrderExec->m_ExecutedQuantity
    , pQueryOrderExec->m_ExecutionPrice
    , pQueryOrderExec->m_ExecutionID
    , pQueryOrderExec->m_MarketExecID
  );

}

CRem5TestHandler::CRem5TestHandler()
{

}

CRem5TestHandler::~CRem5TestHandler()
{

}

void CRem5TestHandler::Loop()
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
      QryInvestorPosition();
      break;
    case 11:
      ChangePassword();
      break;
    default:
      break;
    }
  }

}

int CRem5TestHandler::ShowMenu()
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
        "10-QryInvestorPosition\n"
        "11-ChangePassword\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CRem5TestHandler::Login()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESTraderApi* pTraderApi = (EESTraderApi*)m_TraderApi;
	pTraderApi->UserLogon(m_UserId, m_Passwd, m_AppID, m_AuthCode);
  return ;
}

void CRem5TestHandler::Logout()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  m_Loop = 0;
  return ;
}

void CRem5TestHandler::OrderInsert()
{
  char Temp[64];
  char InstrumentID[64];
  int ExchangeID;
  char Direction;
  char OffsetFlag;
  double LimitPrice;
  int Volume;
  int Times;
  int i;

  
  printf("input InstrumentID:\n");
  scanf("%s", InstrumentID);
  printf("input ExchangeID(CFFEX=102):\n");
  scanf("%s", Temp);
  ExchangeID = atoi(Temp);
  printf("input Direction[0|1]:\n");
  scanf("%s", Temp);
  Direction = Temp[0];
  printf("input OffsetFlag[0|1]:\n");
  scanf("%s", Temp);
  OffsetFlag = Temp[0];
  printf("input LimitPrice:\n");
  scanf("%lf", &LimitPrice);
  printf("input Volume:\n");
  scanf("%d", &Volume);
  printf("input Times:\n");
  scanf("%d", &Times);

  EES_SideType SideType;
  if('0' == OffsetFlag){
    if('0' == Direction){
      SideType = EES_SideType_open_long;
    }else{
      SideType = EES_SideType_open_short;
    }
  }else{
    if('0' == Direction){
      SideType = EES_SideType_close_long;
    }else{
      SideType = EES_SideType_close_short;
    }
  }
  

  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESTraderApi* pTraderApi = (EESTraderApi*)m_TraderApi;
	EES_ClientToken order_token = 0;

	EES_EnterOrderField temp;

  for(i = 0; i < Times; i++){
  	pTraderApi->GetMaxToken(&order_token);
    memset(&temp, 0, sizeof(EES_EnterOrderField));
    temp.m_Tif = EES_OrderTif_Day;
    temp.m_HedgeFlag = EES_HedgeFlag_Speculation;
    strcpy(temp.m_Account, m_InvestorID);
    strcpy(temp.m_Symbol, InstrumentID);
    temp.m_Side = SideType;
    //temp.m_Side = EES_SideType_open_short;
    temp.m_Exchange = (unsigned char)ExchangeID;
    temp.m_SecType = EES_SecType_fut;
    temp.m_Price = LimitPrice;
    temp.m_Qty = Volume;
  	temp.m_ClientOrderToken = order_token + 1;

  	RESULT ret = pTraderApi->EnterOrder(&temp);
  	if (ret != NO_ERROR)
  	{
  		REM_TEST_LOG("send order failed(%d)\n", ret);
  		return;
  	}
  }
}

void CRem5TestHandler::OrderAction()
{
  int Quantity;
  EES_MarketToken MarketOrderID;
  char Temp[64];
  printf("input Quantity:\n");
  scanf("%s", Temp);
  Quantity = atoi(Temp);
  
  printf("input MarketOrderID:\n");
  scanf("%s", Temp);
  MarketOrderID = atol(Temp);

  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESTraderApi* pTraderApi = (EESTraderApi*)m_TraderApi;
	EES_CancelOrder  temp;
	memset(&temp, 0, sizeof(EES_CancelOrder));

	strcpy(temp.m_Account, m_InvestorID);
	temp.m_Quantity = Quantity;
	temp.m_MarketOrderToken = MarketOrderID;

	RESULT ret = pTraderApi->CancelOrder(&temp);
	if (ret != NO_ERROR)
	{
		printf("send cancel failed(%d)\n", ret);
		return;
	}


}

void CRem5TestHandler::QryOrder()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QueryAccountOrder(m_InvestorID);

}

void CRem5TestHandler::QryTrade()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QueryAccountOrderExecution(m_InvestorID);

}

void CRem5TestHandler::QryUserInvestor()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QueryUserAccount();
}

void CRem5TestHandler::QryInvestorAccount()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QueryAccountBP(m_InvestorID);

}

void CRem5TestHandler::QryInstrument()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QuerySymbolList();

}

void CRem5TestHandler::QryInvestorPosition()
{
  REM_TEST_LOG("%s\n", __FUNCTION__);
  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  pQueryApi->QueryAccountPosition(m_InvestorID);

}

void CRem5TestHandler::PrintOrder(void* data)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);

}

void CRem5TestHandler::PrintTrade(void* data)
{
  REM_TEST_LOG("%s\n", __FUNCTION__);

}

void CRem5TestHandler::ChangePassword()
{
  using namespace std;

  REM_TEST_LOG("%s\n", __FUNCTION__);
  string oldPwd, newPwd;
  cout<<"input oldPwd"<<endl;
  cin>>oldPwd;
  cout<<"input newPwd"<<endl;
  cin>>newPwd;

  EESQueryApi* pQueryApi = (EESQueryApi*)m_QueryApi;
  REM_TEST_LOG("ChangePassword(%s, %s)\n", oldPwd.c_str(), newPwd.c_str());
  RESULT ret = pQueryApi->QueryChangePassword(oldPwd.c_str(), newPwd.c_str());
  REM_TEST_LOG("ChangePassword[%d]\n", ret);

}

