#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CXeleTraderApi.hpp"

#include "XeleTest.h"

#define XELE_LOG(...) printf(__VA_ARGS__)


int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAddress1[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sLoginId[128];
  char sUserId[128];
  char sOldPassword[128];
  int i = 1;

  if(argc < 9){
    printf("input sLoginId:\n");
    scanf("%s", sLoginId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sAppId:\n");
    scanf("%s", sAppId);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
    printf("input sBrokerID:\n");
    scanf("%s", sBrokerID);
    printf("input sUserId:\n");
    scanf("%s", sUserId);
    printf("input sAddress:\n");
    scanf("%s", sAddress);
    printf("input sAddress1:\n");
    scanf("%s", sAddress1);
  }else{
    strncpy(sLoginId, argv[i++], sizeof(sLoginId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAddress1, argv[i++], sizeof(sAddress1));
  }


  CXeleTraderApi* pTraderApi = CXeleTraderApi::CreateTraderApi();
  CXeleTraderHandler* pTraderHandler = new CXeleTraderHandler();
  
  // 初始化变量
  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_LoginId = sLoginId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_UserId = sUserId;
  
  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
	pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(sAddress, sAddress1);
  /*	 * 准备login的结构体	 */
  /*	 * 订阅相应的流	 */
  pTraderApi->SubscribePrivateTopic(XELE_TERT_RESUME);
  pTraderApi->SubscribePublicTopic(XELE_TERT_RESUME);
  /*	 * 开始登录, 使客户端程序开始与交易柜台建立连接	 */
  pTraderApi->Init();

  pTraderHandler->Loop();

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}


CXeleTraderHandler::CXeleTraderHandler()
{

}

CXeleTraderHandler::~CXeleTraderHandler()
{

}

void CXeleTraderHandler::Loop()
{
  int choose;
  m_Loop = 1;
  do{
    choose = ShowMenu();
    switch(choose){
    case 1:
      QryInstrument();
      break;
    case 2:
      OpenOrder();
      break;
    case 3:
      Withdraw();
      break;
    case 4:
      QryOrder();
      break;
    case 5:
      QryTrade();
      break;
    case 6:
      QryInvestorPosition();
      break;
    case 7:
      QryTradingAccount();
      break;
    case 8:
      ChangePassword();
      break;
    case 0:
      LogOut();
      sleep(1);
      break;
    default:
      break;
    }
  }while(m_Loop);
  return ;

}

int CXeleTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "选择需要执行的操作\n"
        "1-合约查询\n"
        "2-报单\n"
        "3-撤单\n"
        "4-订单查询\n"
        "5-成交查询\n"
        "6-持仓查询\n"
        "7-资金查询\n"
        "8-修改密码\n"
        "0-退出\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CXeleTraderHandler::LogOut()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcReqUserLogoutField req;
	memset(&req, 0, sizeof(req));
  strncpy(req.AccountID, m_LoginId, sizeof(req.AccountID));
  pTraderApi->ReqUserLogout(&req, m_RequestId++);
  m_Loop = 0;

}

void CXeleTraderHandler::OpenOrder()
{
#if 0
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFairInputOrderField req;
  memset(&req, 0, sizeof(req));
  char InstrumentID[32];
  char Direction[32];
  char OffsetFlag[32];
  double LimitPrice;
  
  printf("请输入合约:");
  scanf("%s", InstrumentID);
  printf("请输入买卖方向:");
  scanf("%s", Direction);
  printf("请输入开平标志:");
  scanf("%s", OffsetFlag);
  printf("请输入价格:");
  scanf("%lf", &LimitPrice);

	/* 会员代码 */
	snprintf(req.ParticipantID, sizeof(req.ParticipantID), "%s", m_BrokerID);
	/* 客户代码 */
	snprintf(req.ClientID, sizeof(req.ClientID), "%s", m_UserId);
	/* 合约代码 */
	strncpy(req.InstrumentID, InstrumentID, sizeof(req.InstrumentID));
	/* 报单价格条件 */
	req.OrderPriceType = XELE_FTDC_OPT_LimitPrice;
	/* 买卖方向 */
	req.Direction = Direction[0];
	/* 组合开平标志 */
	req.CombOffsetFlag[0] = OffsetFlag[0];
	/* 组合投机套保标志 */
	req.CombHedgeFlag[0] = XELE_FTDC_HF_Speculation;
	/* 价格 */
	req.LimitPrice = LimitPrice;
	/* 数量 */
	req.VolumeTotalOriginal = 1;
	/* 有效期类型 */
	req.TimeCondition = XELE_FTDC_TC_GFD;
	/* 成交量类型 */
	req.VolumeCondition = XELE_FTDC_VC_AV;
	/* 最小成交量 */
	req.MinVolume = 0;
	/* 触发条件 */
	req.ContingentCondition = XELE_FTDC_CC_Immediately;
	/* 止损价 */
	req.StopPrice = 0;
	/* 强平原因 */
	req.ForceCloseReason = XELE_FTDC_FCC_NotForceClose;
  /* 本地报单编号 */
	snprintf(req.OrderLocalID, sizeof(req.OrderLocalID), "%ld", m_RequestId);
  /* 自动挂起标志 */
	req.IsAutoSuspend = 0;

  pTraderApi->ReqOrderInsert(&req, m_RequestId++);
#endif
  return ;
}

void CXeleTraderHandler::Withdraw()
{
#if 0
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFairOrderActionField req;
  memset(&req, 0, sizeof(req));
  
  char OrderSysID[32];
  printf("请输入报单编号:");
  scanf("%s", OrderSysID);

  ///报单编号
	//snprintf(req.OrderSysID, sizeof(req.OrderSysID), "%s", OrderSysID);
	req.OrderSysNo = atoi(OrderSysID);
  ///报单操作标志
  req.ActionFlag = XELE_FTDC_AF_Delete;
  ///会员代码
	//snprintf(req.ParticipantID, sizeof(req.ParticipantID), "%s", m_BrokerID);
  ///客户代码
	//snprintf(req.ClientID, sizeof(req.ClientID), "%s", m_UserId);
  ///操作本地编号
	//snprintf(req.ActionLocalID, sizeof(req.ActionLocalID), "%ld", m_RequestId);
	req.ActionLocalNo = atoi(m_RequestId);

  pTraderApi->ReqOrderAction(&req, m_RequestId++);

#endif
  return ;
}

void CXeleTraderHandler::QryInstrument()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcQryInstrumentField req;
  memset(&req, 0, sizeof(CXeleFtdcQryInstrumentField));
  /* 客户代码 */
  //snprintf(req.AccountID, sizeof(req.AccountID), "%s", m_UserId);
  pTraderApi->ReqQryInstrument(&req, m_RequestId++);

}

void CXeleTraderHandler::QryOrder()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcQryOrderField qryOrder;
  memset(&qryOrder, 0, sizeof(CXeleFtdcQryOrderField));
  /* 客户代码 */
  snprintf(qryOrder.AccountID, sizeof(qryOrder.AccountID), "%s", m_LoginId);
  pTraderApi->ReqQryOrder(&qryOrder, m_RequestId++);

}

void CXeleTraderHandler::QryTrade()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcQryTradeField qryTrade;
  memset(&qryTrade, 0, sizeof(CXeleFtdcQryTradeField));
  /* 客户代码 */
  snprintf(qryTrade.AccountID, sizeof(qryTrade.AccountID), "%s", m_LoginId);
  /* 合约代码 */
  //strcpy(qryTrade.InstrumentID, "cu1705");
  /* 成交编号 */
  //strcpy(qryTrade.TradeID, "233");
  pTraderApi->ReqQryTrade(&qryTrade, m_RequestId++);

}

void CXeleTraderHandler::QryInvestorPosition()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcQryClientPositionField qryPosition;
  memset(&qryPosition, 0, sizeof(CXeleFtdcQryClientPositionField));
  /* 客户代码 */
  snprintf(qryPosition.AccountID, sizeof(qryPosition.AccountID), "%s", m_LoginId);
  pTraderApi->ReqQryClientPosition(&qryPosition, m_RequestId++);

}

void CXeleTraderHandler::QryTradingAccount()
{
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcQryClientAccountField qryAccount;
  memset(&qryAccount, 0, sizeof(CXeleFtdcQryClientAccountField));
  /* 客户代码 */
  snprintf(qryAccount.AccountID, sizeof(qryAccount.AccountID), "%s", m_LoginId);
  pTraderApi->ReqQryClientAccount(&qryAccount, m_RequestId++);

}

void CXeleTraderHandler::ChangePassword()
{
  char NewPassword[41];
  printf("请输入新密码:");
  scanf("%s", NewPassword);

  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;
  CXeleFtdcUserPasswordUpdateField req;
  memset(&req, 0, sizeof(req));
  
  snprintf(req.AccountID, sizeof(req.AccountID), "%s", m_LoginId);
  snprintf(req.ParticipantID, sizeof(req.ParticipantID), "%s", m_BrokerID);
  snprintf(req.OldPassword, sizeof(req.OldPassword), "%s", m_OldPasswd);
  snprintf(req.NewPassword, sizeof(req.NewPassword), "%s", NewPassword);
  
  pTraderApi->ReqUserPasswordUpdate(&req, m_RequestId++);
}


void CXeleTraderHandler::PrintTrade(CXeleFtdcTradeField *pTrade)
{
  if(!pTrade){
    return ;
  }
  
  XELE_LOG(
    "pTrade->TradingDay=[%s]"
    "pTrade->SettlementGroupID=[%s]"
    "pTrade->SettlementID=[%d]"
    "pTrade->TradeID=[%s]"
    "pTrade->Direction=[%c]"
    "pTrade->OrderSystemNo=[%d]"
    "pTrade->ParticipantID=[%s]"
    "pTrade->ClientID=[%s]"
    "pTrade->TradingRole=[%c]"
    "pTrade->AccountID=[%s]"
    "pTrade->InstrumentID=[%s]"
    "pTrade->OffsetFlag=[%c]"
    "pTrade->HedgeFlag=[%c]"
    "pTrade->Price=[%lf]"
    "pTrade->Volume=[%d]"
    "pTrade->TradeTime=[%s]"
    "pTrade->TradeType=[%c]"
    "pTrade->PriceSource=[%c]"
    "pTrade->UserID=[%s]"
    "pTrade->OrderLocalNo=[%d]"
    "pTrade->ExchangeOrderSysID=[%s]"
    "\n"
    ,pTrade->TradingDay
    ,pTrade->SettlementGroupID
    ,pTrade->SettlementID
    ,pTrade->TradeID
    ,pTrade->Direction
    ,pTrade->OrderSystemNo
    ,pTrade->ParticipantID
    ,pTrade->ClientID
    ,pTrade->TradingRole
    ,pTrade->AccountID
    ,pTrade->InstrumentID
    ,pTrade->OffsetFlag
    ,pTrade->HedgeFlag
    ,pTrade->Price
    ,pTrade->Volume
    ,pTrade->TradeTime
    ,pTrade->TradeType
    ,pTrade->PriceSource
    ,pTrade->UserID
    ,pTrade->OrderLocalNo
    ,pTrade->ExchangeOrderSysID
  );


}

void CXeleTraderHandler::PrintOrder(CXeleFtdcOrderField *pOrder)
{
  if(!pOrder){
    return ;
  }
  
  XELE_LOG(
    "pOrder->TradingDay=[%s]"
    "pOrder->SettlementGroupID=[%s]"
    "pOrder->SettlementID=[%d]"
    "pOrder->OrderSystemNo=[%d]"
    "pOrder->ParticipantID=[%s]"
    "pOrder->ClientID=[%s]"
    "pOrder->UserID=[%s]"
    "pOrder->InstrumentID=[%s]"
    "pOrder->OrderPriceType=[%c]"
    "pOrder->Direction=[%c]"
    "pOrder->CombOffsetFlag=[%s]"
    "pOrder->CombHedgeFlag=[%s]"
    "pOrder->LimitPrice=[%lf]"
    "pOrder->VolumeTotalOriginal=[%d]"
    "pOrder->TimeCondition=[%c]"
    "pOrder->GTDDate=[%s]"
    "pOrder->VolumeCondition=[%c]"
    "pOrder->MinVolume=[%d]"
    "pOrder->ContingentCondition=[%c]"
    "pOrder->StopPrice=[%lf]"
    "pOrder->ForceCloseReason=[%c]"
    "pOrder->OrderLocalNo=[%d]"
    "pOrder->IsAutoSuspend=[%d]"
    "pOrder->OrderSource=[%c]"
    "pOrder->OrderStatus=[%c]"
    "pOrder->OrderType=[%c]"
    "pOrder->VolumeTraded=[%d]"
    "pOrder->VolumeTotal=[%d]"
    "pOrder->InsertDate=[%s]"
    "pOrder->InsertTime=[%s]"
    "pOrder->ActiveTime=[%s]"
    "pOrder->SuspendTime=[%s]"
    "pOrder->UpdateTime=[%s]"
    "pOrder->CancelTime=[%s]"
    "pOrder->ActiveUserID=[%s]"
    "pOrder->Priority=[%d]"
    "pOrder->TimeSortID=[%d]"
    "pOrder->ExchangeOrderSysID=[%s]"
    "\n"
    ,pOrder->TradingDay
    ,pOrder->SettlementGroupID
    ,pOrder->SettlementID
    ,pOrder->OrderSystemNo
    ,pOrder->ParticipantID
    ,pOrder->ClientID
    ,pOrder->UserID
    ,pOrder->InstrumentID
    ,pOrder->OrderPriceType
    ,pOrder->Direction
    ,pOrder->CombOffsetFlag
    ,pOrder->CombHedgeFlag
    ,pOrder->LimitPrice
    ,pOrder->VolumeTotalOriginal
    ,pOrder->TimeCondition
    ,pOrder->GTDDate
    ,pOrder->VolumeCondition
    ,pOrder->MinVolume
    ,pOrder->ContingentCondition
    ,pOrder->StopPrice
    ,pOrder->ForceCloseReason
    ,pOrder->OrderLocalNo
    ,pOrder->IsAutoSuspend
    ,pOrder->OrderSource
    ,pOrder->OrderStatus
    ,pOrder->OrderType
    ,pOrder->VolumeTraded
    ,pOrder->VolumeTotal
    ,pOrder->InsertDate
    ,pOrder->InsertTime
    ,pOrder->ActiveTime
    ,pOrder->SuspendTime
    ,pOrder->UpdateTime
    ,pOrder->CancelTime
    ,pOrder->ActiveUserID
    ,pOrder->Priority
    ,pOrder->TimeSortID
    ,pOrder->ExchangeOrderSysID
  );

}


void CXeleTraderHandler::OnFrontConnected()
{
  XELE_LOG("%s\n", __FUNCTION__);
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)m_Arg;

	CXeleFtdcReqUserLoginField login_info;
  CXeleFtdcAuthenticationInfoField auth_info;
  int nRet = 0;

	memset(&login_info, 0, sizeof(login_info));
  snprintf(login_info.AccountID, sizeof(login_info.AccountID), "%s", m_LoginId);
  snprintf(login_info.Password, sizeof(login_info.Password), "%s", m_OldPasswd);
  memset(&auth_info, 0, sizeof(auth_info));
  snprintf(auth_info.AuthCode, sizeof(auth_info.AuthCode), "%s", m_AuthCode);
  snprintf(auth_info.AppID, sizeof(auth_info.AppID), "%s", m_AppID);
	pTraderApi->RegisterAuthentication(&auth_info);
  nRet = pTraderApi->ReqUserLogin(&login_info, m_RequestId++);
  XELE_LOG("ReqUserLogin ret=[%d]\n", nRet);
  

}

void CXeleTraderHandler::OnFrontDisconnected(int nReason)
{
  XELE_LOG("%s[0x%x][0x%x]\n", __FUNCTION__, ((nReason >> 16) & 0xFFFF), (nReason & 0xFFFF));

}

void CXeleTraderHandler::OnRspError(CXeleFtdcRspInfoField *pRspInfo,
                      int nRequestID,
                      bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnRspUserLogin(CXeleFtdcRspUserLoginField *pRspUserLogin,
                          CXeleFtdcRspInfoField *pRspInfo,
                          int nRequestID,
                          bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    if(!pRspInfo->ErrorID){
      m_ClientIndex = pRspUserLogin->ClientIndex[0];
      m_Token = pRspUserLogin->Token[0];
    }
  }
}

void CXeleTraderHandler::OnRspUserLogout(CXeleFtdcRspUserLogoutField *pRspUserLogout,
                           CXeleFtdcRspInfoField *pRspInfo,
                           int nRequestID,
                           bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnRspOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                            CXeleFtdcRspInfoField *pRspInfo,
                            int nRequestID,
                            bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnRspOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                            CXeleFtdcRspInfoField *pRspInfo,
                            int nRequestID,
                            bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnRspQryClientPosition(CXeleFtdcRspClientPositionField *pRspClientPosition,
                                  CXeleFtdcRspInfoField *pRspInfo,
                                  int nRequestID,
                                  bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspClientPosition) {
    XELE_LOG( 
      "pRspClientPosition->TradingDay=[%s]"
      "pRspClientPosition->SettlementGroupID=[%s]"
      "pRspClientPosition->SettlementID=[%d]"
      "pRspClientPosition->HedgeFlag=[%c]"
      "pRspClientPosition->LongYdPosition=[%d]"
      "pRspClientPosition->LongPosition=[%d]"
      "pRspClientPosition->ShortYdPosition=[%d]"
      "pRspClientPosition->ShortPosition=[%d]"
      "pRspClientPosition->InstrumentID=[%s]"
      "pRspClientPosition->AccountID=[%s]"
      "\n"
      ,pRspClientPosition->TradingDay
      ,pRspClientPosition->SettlementGroupID
      ,pRspClientPosition->SettlementID
      ,pRspClientPosition->HedgeFlag
      ,pRspClientPosition->LongYdPosition
      ,pRspClientPosition->LongPosition
      ,pRspClientPosition->ShortYdPosition
      ,pRspClientPosition->ShortPosition
      ,pRspClientPosition->InstrumentID
      ,pRspClientPosition->AccountID
    );
  }

}

void CXeleTraderHandler::OnRspQryInstrument(CXeleFtdcRspInstrumentField *pRspInstrument,
                              CXeleFtdcRspInfoField *pRspInfo,
                              int nRequestID,
                              bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspInstrument){
    XELE_LOG(
      "pRspInstrument->SettlementGroupID=[%s]"
      "pRspInstrument->ProductID=[%s]"
      "pRspInstrument->ProductGroupID=[%s]"
      "pRspInstrument->UnderlyingInstrID=[%s]"
      "pRspInstrument->ProductClass=[%c]"
      "pRspInstrument->PositionType=[%c]"
      "pRspInstrument->StrikePrice=[%lf]"
      "pRspInstrument->OptionsType=[%c]"
      "pRspInstrument->VolumeMultiple=[%d]"
      "pRspInstrument->UnderlyingMultiple=[%lf]"
      "pRspInstrument->InstrumentID=[%s]"
      "pRspInstrument->InstrumentName=[%s]"
      "pRspInstrument->DeliveryYear=[%d]"
      "pRspInstrument->DeliveryMonth=[%d]"
      "pRspInstrument->AdvanceMonth=[%s]"
      "pRspInstrument->IsTrading=[%d]"
      "pRspInstrument->CreateDate=[%s]"
      "pRspInstrument->OpenDate=[%s]"
      "pRspInstrument->ExpireDate=[%s]"
      "pRspInstrument->StartDelivDate=[%s]"
      "pRspInstrument->EndDelivDate=[%s]"
      "pRspInstrument->BasisPrice=[%lf]"
      "pRspInstrument->MaxMarketOrderVolume=[%d]"
      "pRspInstrument->MinMarketOrderVolume=[%d]"
      "pRspInstrument->MaxLimitOrderVolume=[%d]"
      "pRspInstrument->MinLimitOrderVolume=[%d]"
      "pRspInstrument->PriceTick=[%lf]"
      "pRspInstrument->AllowDelivPersonOpen=[%d]"
      "\n"
      ,pRspInstrument->SettlementGroupID
      ,pRspInstrument->ProductID
      ,pRspInstrument->ProductGroupID
      ,pRspInstrument->UnderlyingInstrID
      ,pRspInstrument->ProductClass
      ,pRspInstrument->PositionType
      ,pRspInstrument->StrikePrice
      ,pRspInstrument->OptionsType
      ,pRspInstrument->VolumeMultiple
      ,pRspInstrument->UnderlyingMultiple
      ,pRspInstrument->InstrumentID
      ,pRspInstrument->InstrumentName
      ,pRspInstrument->DeliveryYear
      ,pRspInstrument->DeliveryMonth
      ,pRspInstrument->AdvanceMonth
      ,pRspInstrument->IsTrading
      ,pRspInstrument->CreateDate
      ,pRspInstrument->OpenDate
      ,pRspInstrument->ExpireDate
      ,pRspInstrument->StartDelivDate
      ,pRspInstrument->EndDelivDate
      ,pRspInstrument->BasisPrice
      ,pRspInstrument->MaxMarketOrderVolume
      ,pRspInstrument->MinMarketOrderVolume
      ,pRspInstrument->MaxLimitOrderVolume
      ,pRspInstrument->MinLimitOrderVolume
      ,pRspInstrument->PriceTick
      ,pRspInstrument->AllowDelivPersonOpen
    );

  }

}

void CXeleTraderHandler::OnRspQryClientAccount(CXeleFtdcRspClientAccountField *pClientAccount,
                                 CXeleFtdcRspInfoField *pRspInfo,
                                 int nRequestID,
                                 bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pClientAccount){
    XELE_LOG(
      "pClientAccount->TradingDay=[%s]"
      "pClientAccount->SettlementGroupID=[%s]"
      "pClientAccount->SettlementID=[%d]"
      "pClientAccount->PreBalance=[%lf]"
      "pClientAccount->CurrMargin=[%lf]"
      "pClientAccount->CloseProfit=[%lf]"
      "pClientAccount->Premium=[%lf]"
      "pClientAccount->Deposit=[%lf]"
      "pClientAccount->Withdraw=[%lf]"
      "pClientAccount->Balance=[%lf]"
      "pClientAccount->Available=[%lf]"
      "pClientAccount->AccountID=[%s]"
      "pClientAccount->FrozenMargin=[%lf]"
      "pClientAccount->FrozenPremium=[%lf]"
      "pClientAccount->BaseReserve=[%lf]"
      "pClientAccount->floatProfitAndLoss=[%lf]"
      "\n"
      ,pClientAccount->TradingDay
      ,pClientAccount->SettlementGroupID
      ,pClientAccount->SettlementID
      ,pClientAccount->PreBalance
      ,pClientAccount->CurrMargin
      ,pClientAccount->CloseProfit
      ,pClientAccount->Premium
      ,pClientAccount->Deposit
      ,pClientAccount->Withdraw
      ,pClientAccount->Balance
      ,pClientAccount->Available
      ,pClientAccount->AccountID
      ,pClientAccount->FrozenMargin
      ,pClientAccount->FrozenPremium
      ,pClientAccount->BaseReserve
      ,pClientAccount->floatProfitAndLoss
    );

  }

}

void CXeleTraderHandler::OnRtnTrade(CXeleFtdcTradeField *pTrade)
{
  XELE_LOG("%s\n", __FUNCTION__);
  PrintTrade(pTrade);
}

void CXeleTraderHandler::OnRtnOrder(CXeleFtdcOrderField *pOrder)
{
  XELE_LOG("%s\n", __FUNCTION__);
  PrintOrder(pOrder);
}

void CXeleTraderHandler::OnRtnInsInstrument(CXeleFtdcInstrumentField *pInstrument)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pInstrument){
    XELE_LOG(
      "pInstrument->SettlementGroupID=[%s]"
      "pInstrument->ProductID=[%s]"
      "pInstrument->ProductGroupID=[%s]"
      "pInstrument->UnderlyingInstrID=[%s]"
      "pInstrument->ProductClass=[%c]"
      "pInstrument->PositionType=[%c]"
      "pInstrument->StrikePrice=[%lf]"
      "pInstrument->OptionsType=[%c]"
      "pInstrument->VolumeMultiple=[%d]"
      "pInstrument->UnderlyingMultiple=[%lf]"
      "pInstrument->InstrumentID=[%s]"
      "pInstrument->InstrumentName=[%s]"
      "pInstrument->DeliveryYear=[%d]"
      "pInstrument->DeliveryMonth=[%d]"
      "pInstrument->AdvanceMonth=[%s]"
      "pInstrument->IsTrading=[%d]"
      "\n"
      ,pInstrument->SettlementGroupID
      ,pInstrument->ProductID
      ,pInstrument->ProductGroupID
      ,pInstrument->UnderlyingInstrID
      ,pInstrument->ProductClass
      ,pInstrument->PositionType
      ,pInstrument->StrikePrice
      ,pInstrument->OptionsType
      ,pInstrument->VolumeMultiple
      ,pInstrument->UnderlyingMultiple
      ,pInstrument->InstrumentID
      ,pInstrument->InstrumentName
      ,pInstrument->DeliveryYear
      ,pInstrument->DeliveryMonth
      ,pInstrument->AdvanceMonth
      ,pInstrument->IsTrading
    );
  }

}

void CXeleTraderHandler::OnErrRtnOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                               CXeleFtdcRspInfoField *pRspInfo)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnErrRtnOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                               CXeleFtdcRspInfoField *pRspInfo)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CXeleTraderHandler::OnRspQryOrder(CXeleFtdcOrderField* pOrderField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  PrintOrder(pOrderField);
}

void CXeleTraderHandler::OnRspQryTrade(CXeleFtdcTradeField* pTradeField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  PrintTrade(pTradeField);
}


void CXeleTraderHandler::OnRspUserPasswordUpdate(CXeleFtdcUserPasswordUpdateField *pUserPasswordUpdate,
                                       CXeleFtdcRspInfoField *pRspInfo,
                                       int nRequestID,
                                       bool bIsLast)
{
  XELE_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XELE_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}


