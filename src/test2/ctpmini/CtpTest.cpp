
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"

#include "CtpTest.h"

//using namespace ctp_sopt;

#define CTP_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
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
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./");
  CCtpSopTraderHandler* pTraderHandler = new CCtpSopTraderHandler();
  // 初始化变量
  // 交易
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
  pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
  pTraderApi->RegisterFront(sAddress);

  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_NewPasswd = sFilename;

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


CCtpSopTraderHandler::CCtpSopTraderHandler()
{

}

CCtpSopTraderHandler::~CCtpSopTraderHandler()
{

}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CCtpSopTraderHandler::OnFrontConnected()
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(0 == m_Loop){
    return ;
  }
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcReqAuthenticateField reqAuthenticateField;
  memset(&reqAuthenticateField, 0, sizeof(reqAuthenticateField));
  strcpy(reqAuthenticateField.BrokerID, m_BrokerID);
  strcpy(reqAuthenticateField.UserID, m_UserId);
  strcpy(reqAuthenticateField.AppID, m_AppID);
  strcpy(reqAuthenticateField.AuthCode, m_AuthCode);
  pTraderApi->ReqAuthenticate(&reqAuthenticateField, m_RequestId++);
  return ;
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CCtpSopTraderHandler::OnFrontDisconnected(int nReason)
{
  CTP_LOG("%s\n", __FUNCTION__);
}

///客户端认证响应
void CCtpSopTraderHandler::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_OldPasswd);

  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
  return ;
}

///登录请求响应
void CCtpSopTraderHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  SettlementInfoConfirm();

}

///登出请求响应
void CCtpSopTraderHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

///报单录入请求响应
void CCtpSopTraderHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///报单操作请求响应
void CCtpSopTraderHandler::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///投资者结算结果确认响应
void CCtpSopTraderHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///执行宣告录入请求响应
void CCtpSopTraderHandler::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///执行宣告操作请求响应
void CCtpSopTraderHandler::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///请求查询报单响应
void CCtpSopTraderHandler::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

  if(pOrder){
    CTP_LOG("pOrder->InstrumentID=[%s]\n"
      "pOrder->OrderRef=[%s]\n"
      "pOrder->UserID=[%s]\n"
      "pOrder->VolumeTotalOriginal=[%d]\n"
      "pOrder->OrderLocalID=[%s]\n"
      "pOrder->ExchangeID=[%s]\n"
      "pOrder->TradingDay=[%s]\n"
      "pOrder->SettlementID=[%d]\n"
      "pOrder->OrderSysID=[%s]\n"
      "pOrder->OrderSource=[%c]\n"
      "pOrder->OrderStatus=[%c]\n"
      "pOrder->OrderType=[%c]\n"
      "pOrder->VolumeTraded=[%d]\n"
      "pOrder->VolumeTotal=[%d]\n"
      "pOrder->LimitPrice=[%lf]\n"
      "pOrder->InsertDate=[%s]\n"
      "pOrder->InsertTime=[%s]\n"
      "pOrder->ActiveTime=[%s]\n"
      "pOrder->SuspendTime=[%s]\n"
      "pOrder->UpdateTime=[%s]\n"
      "pOrder->CancelTime=[%s]\n",
      pOrder->InstrumentID,
      pOrder->OrderRef,
      pOrder->UserID,
      pOrder->VolumeTotalOriginal,
      pOrder->OrderLocalID,
      pOrder->ExchangeID,
      pOrder->TradingDay,
      pOrder->SettlementID,
      pOrder->OrderSysID,
      pOrder->OrderSource,
      pOrder->OrderStatus,
      pOrder->OrderType,
      pOrder->VolumeTraded,
      pOrder->VolumeTotal,
      pOrder->LimitPrice,
      pOrder->InsertDate,
      pOrder->InsertTime,
      pOrder->ActiveTime,
      pOrder->SuspendTime,
      pOrder->UpdateTime,
      pOrder->CancelTime);



  }

}

///请求查询合约响应
void CCtpSopTraderHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  if(!pInstrument){
    return ;
  }

  FILE* fp;
  fp = fopen(m_NewPasswd, "a+");
  if(NULL == fp){
    return ;
  }

  fprintf(fp, "%s,%s,%s,%s,%s,%c,%d,%d,%d,%d,"
    "%d,%d,%d,%lf,%s,%s,%s,%s,%s,%c,"
    "%d,%c,%c,%lf,%lf,%c,%s,%lf,%c,%lf,"
    "%c\n",
    pInstrument->InstrumentID,
    pInstrument->ExchangeID,
    pInstrument->InstrumentName,
    pInstrument->ExchangeInstID,
    pInstrument->ProductID,
    pInstrument->ProductClass,
    pInstrument->DeliveryYear,
    pInstrument->DeliveryMonth,
    pInstrument->MaxMarketOrderVolume,
    pInstrument->MinMarketOrderVolume,
    
    pInstrument->MaxLimitOrderVolume,
    pInstrument->MinLimitOrderVolume,
    pInstrument->VolumeMultiple,
    pInstrument->PriceTick,
    pInstrument->CreateDate,
    pInstrument->OpenDate,
    pInstrument->ExpireDate,
    pInstrument->StartDelivDate,
    pInstrument->EndDelivDate,
    pInstrument->InstLifePhase,
    
    pInstrument->IsTrading,
    pInstrument->PositionType,
    pInstrument->PositionDateType,
    pInstrument->LongMarginRatio,
    pInstrument->ShortMarginRatio,
    
    pInstrument->MaxMarginSideAlgorithm,
    pInstrument->UnderlyingInstrID,
    pInstrument->StrikePrice,
    pInstrument->OptionsType,
    pInstrument->UnderlyingMultiple,
    pInstrument->CombinationType);
 
  fclose(fp);

  return;
}

///错误应答
void CCtpSopTraderHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///报单通知
void CCtpSopTraderHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
  CTP_LOG("%s\n", __FUNCTION__);

  if(pOrder){
    CTP_LOG("pOrder->InstrumentID=[%s]\n"
      "pOrder->OrderRef=[%s]\n"
      "pOrder->UserID=[%s]\n"
      "pOrder->VolumeTotalOriginal=[%d]\n"
      "pOrder->OrderLocalID=[%s]\n"
      "pOrder->ExchangeID=[%s]\n"
      "pOrder->TradingDay=[%s]\n"
      "pOrder->SettlementID=[%d]\n"
      "pOrder->OrderSysID=[%s]\n"
      "pOrder->OrderSource=[%c]\n"
      "pOrder->OrderStatus=[%c]\n"
      "pOrder->OrderType=[%c]\n"
      "pOrder->VolumeTraded=[%d]\n"
      "pOrder->VolumeTotal=[%d]\n"
      "pOrder->InsertDate=[%s]\n"
      "pOrder->InsertTime=[%s]\n"
      "pOrder->ActiveTime=[%s]\n"
      "pOrder->SuspendTime=[%s]\n"
      "pOrder->UpdateTime=[%s]\n"
      "pOrder->CancelTime=[%s]\n",
      pOrder->InstrumentID,
      pOrder->OrderRef,
      pOrder->UserID,
      pOrder->VolumeTotalOriginal,
      pOrder->OrderLocalID,
      pOrder->ExchangeID,
      pOrder->TradingDay,
      pOrder->SettlementID,
      pOrder->OrderSysID,
      pOrder->OrderSource,
      pOrder->OrderStatus,
      pOrder->OrderType,
      pOrder->VolumeTraded,
      pOrder->VolumeTotal,
      pOrder->InsertDate,
      pOrder->InsertTime,
      pOrder->ActiveTime,
      pOrder->SuspendTime,
      pOrder->UpdateTime,
      pOrder->CancelTime);
  }

}

///成交通知
void CCtpSopTraderHandler::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pTrade){

    CTP_LOG("pTrade->InstrumentID=[%s]\n"
      "pTrade->OrderRef=[%s]\n"
      "pTrade->UserID=[%s]\n"
      "pTrade->ExchangeID=[%s]\n"
      "pTrade->TradeID=[%s]\n"
      "pTrade->Direction=[%c]\n"
      "pTrade->OrderSysID=[%s]\n"
      "pTrade->ExchangeInstID=[%s]\n"
      "pTrade->OffsetFlag=[%c]\n"
      "pTrade->HedgeFlag=[%c]\n"
      "pTrade->Price=[%lf]\n"
      "pTrade->Volume=[%d]\n"
      "pTrade->TradeDate=[%s]\n"
      "pTrade->TradeTime=[%s]\n"
      "pTrade->OrderLocalID=[%s]\n"
      "pTrade->SequenceNo=[%d]\n"
      "pTrade->TradingDay=[%s]\n",
      pTrade->InstrumentID,
      pTrade->OrderRef,
      pTrade->UserID,
      pTrade->ExchangeID,
      pTrade->TradeID,
      pTrade->Direction,
      pTrade->OrderSysID,
      pTrade->ExchangeInstID,
      pTrade->OffsetFlag,
      pTrade->HedgeFlag,
      pTrade->Price,
      pTrade->Volume,
      pTrade->TradeDate,
      pTrade->TradeTime,
      pTrade->OrderLocalID,
      pTrade->SequenceNo,
      pTrade->TradingDay);
  }

}

///报单录入错误回报
void CCtpSopTraderHandler::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
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
void CCtpSopTraderHandler::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///执行宣告通知
void CCtpSopTraderHandler::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pExecOrder){
    CTP_LOG("pExecOrder->InstrumentID=[%s]\n"
      "pExecOrder->ExecOrderRef=[%s]\n"
      "pExecOrder->UserID=[%s]\n"
      "pExecOrder->Volume=[%d]\n"
      "pExecOrder->OffsetFlag=[%c]\n"
      "pExecOrder->HedgeFlag=[%c]\n"
      "pExecOrder->ActionType=[%c]\n"
      "pExecOrder->PosiDirection=[%c]\n"
      "pExecOrder->CloseFlag=[%c]\n"
      "pExecOrder->ExecOrderLocalID=[%s]\n"
      "pExecOrder->ExchangeID=[%s]\n"
      "pExecOrder->ExchangeInstID=[%s]\n"
      "pExecOrder->OrderSubmitStatus=[%c]\n"
      "pExecOrder->TradingDay=[%s]\n"
      "pExecOrder->SettlementID=[%d]\n"
      "pExecOrder->ExecOrderSysID=[%s]\n"
      "pExecOrder->InsertDate=[%s]\n"
      "pExecOrder->InsertTime=[%s]\n"
      "pExecOrder->CancelTime=[%s]\n"
      "pExecOrder->ExecResult=[%c]\n"
      "pExecOrder->SequenceNo=[%d]\n",
      pExecOrder->InstrumentID,
      pExecOrder->ExecOrderRef,
      pExecOrder->UserID,
      pExecOrder->Volume,
      pExecOrder->OffsetFlag,
      pExecOrder->HedgeFlag,
      pExecOrder->ActionType,
      pExecOrder->PosiDirection,
      pExecOrder->CloseFlag,
      pExecOrder->ExecOrderLocalID,
      pExecOrder->ExchangeID,
      pExecOrder->ExchangeInstID,
      pExecOrder->OrderSubmitStatus,
      pExecOrder->TradingDay,
      pExecOrder->SettlementID,
      pExecOrder->ExecOrderSysID,
      pExecOrder->InsertDate,
      pExecOrder->InsertTime,
      pExecOrder->CancelTime,
      pExecOrder->ExecResult,
      pExecOrder->SequenceNo);
  }

}

///执行宣告录入错误回报
void CCtpSopTraderHandler::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///执行宣告操作错误回报
void CCtpSopTraderHandler::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

void CCtpSopTraderHandler::Loop()
{
  int choose;
  m_Loop = 1;
  do{
    choose = ShowMenu();
    switch(choose){
    case 1:
      QueryContract();
      break;
    case 2:
      CoveredOpen();
      break;
    case 3:
      CoveredClose();
      break;
    case 4:
      CallBuyOpen();
      break;
    case 5:
      CallSellClose();
      break;
    case 6:
      CallSellOpen();
      break;
    case 7:
      CallBuyClose();
      break;
    case 8:
      PutBuyOpen();
      break;
    case 9:
      PutSellClose();
      break;
    case 10:
      PutSellOpen();
      break;
    case 11:
      PutBuyClose();
      break;
    case 12:
      CallExecuteOrder();
      break;
    case 13:
      PutExecuteOrder();
      break;
    case 14:
      Query();
      break;
    case 15:
      Withdraw();
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

int CCtpSopTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "选择需要执行的操作\n"
        "1-合约查询\n"
        "2-备兑开仓\n"
        "3-备兑平仓\n"
        "4-认购买入开仓\n"
        "5-认购卖出平仓\n"
        "6-认购卖出开仓\n"
        "7-认购买入平仓\n"
        "8-认沽买入开仓\n"
        "9-认沽卖出平仓\n"
        "10-认沽卖出开仓\n"
        "11-认沽买入平仓\n"
        "12-认购行权\n"
        "13-认沽行权\n"
        "14-订单查询\n"
        "15-撤单\n"
        "0-退出\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CCtpSopTraderHandler::QueryContract()
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  struct CThostFtdcQryInstrumentField data3;
  memset(&data3, 0, sizeof(data3));

  pTraderApi->ReqQryInstrument(&data3, m_RequestId++);

  return ;
}

void CCtpSopTraderHandler::SettlementInfoConfirm()
{
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;

  CThostFtdcSettlementInfoConfirmField data3;
  memset(&data3, 0, sizeof(CThostFtdcSettlementInfoConfirmField));

	///经纪公司代码
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者代码
	strncpy(data3.InvestorID, m_UserId, sizeof(data3.InvestorID));
  strncpy(data3.ConfirmDate, pTraderApi->GetTradingDay(), sizeof(data3.ConfirmDate));
  strncpy(data3.ConfirmTime, "09:30:00", sizeof(data3.ConfirmTime));
  
  pTraderApi->ReqSettlementInfoConfirm(&data3, m_RequestId++);

}


void CCtpSopTraderHandler::InsertOrder(char* inst, char* exchange_id, char buy_sell, char open_close, double price, int vol, char covered)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  
  struct CThostFtdcInputOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
  
	///经纪公司代码
	strncpy(inputOrderField.BrokerID, m_BrokerID, sizeof(inputOrderField.BrokerID));
	///投资者代码
	strncpy(inputOrderField.InvestorID, m_UserId, sizeof(inputOrderField.InvestorID));
	///合约代码
	strncpy(inputOrderField.InstrumentID, inst, sizeof(inputOrderField.InstrumentID));
	///报单引用
	snprintf(inputOrderField.OrderRef, sizeof(inputOrderField.OrderRef), "%ld", m_RequestId);
	///用户代码
	strncpy(inputOrderField.UserID, m_UserId, sizeof(inputOrderField.UserID));
	///报单价格条件
	inputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向
	inputOrderField.Direction = (char)buy_sell;
	///组合开平标志
	inputOrderField.CombOffsetFlag[0] = (char)open_close;
	///组合投机套保标志
	inputOrderField.CombHedgeFlag[0] = (char)covered;
	///价格
	inputOrderField.LimitPrice = price;
	///数量
	inputOrderField.VolumeTotalOriginal = vol;
	///有效期类型
	inputOrderField.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//TThostFtdcDateType	GTDDate;
	///成交量类型
	inputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量
	inputOrderField.MinVolume = 1;
	///触发条件
	inputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//TThostFtdcPriceType	StopPrice;
	///强平原因
	inputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志
	//TThostFtdcBoolType	IsAutoSuspend;
	///业务单元
	//TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//TThostFtdcRequestIDType	RequestID;
	///用户强评标志
	//TThostFtdcBoolType	UserForceClose;
	///互换单标志
	//TThostFtdcBoolType	IsSwapOrder;
	///交易所代码
	strncpy(inputOrderField.ExchangeID, exchange_id, sizeof(inputOrderField.ExchangeID));
	///投资单元代码
	//TThostFtdcInvestUnitIDType	InvestUnitID;
	///资金账号
	//TThostFtdcAccountIDType	AccountID;
	///币种代码
	//TThostFtdcCurrencyIDType	CurrencyID;
	///交易编码
	//TThostFtdcClientIDType	ClientID;
	///IP地址
	//TThostFtdcIPAddressType	IPAddress;
	///Mac地址
	//TThostFtdcMacAddressType	MacAddress;

  pTraderApi->ReqOrderInsert(&inputOrderField, m_RequestId++);

  return ;
}

void CCtpSopTraderHandler::ExecuteOrder(char* inst, char* exchange_id, int vol)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  struct CThostFtdcInputExecOrderField data3;
  memset(&data3, 0, sizeof(data3));
  
	///经纪公司代码
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者代码
	strncpy(data3.InvestorID, m_UserId, sizeof(data3.InvestorID));
	///合约代码
	strncpy(data3.InstrumentID, inst, sizeof(data3.InstrumentID));
	///执行宣告引用
	snprintf(data3.ExecOrderRef, sizeof(data3.ExecOrderRef), "%ld", m_RequestId);
	///用户代码
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));
	///数量
	data3.Volume = vol;
	///请求编号
	//TThostFtdcRequestIDType	RequestID;
	///业务单元
	//TThostFtdcBusinessUnitType	BusinessUnit;
	///开平标志
	data3.OffsetFlag = THOST_FTDC_OF_Close;
	///投机套保标志
	data3.HedgeFlag = THOST_FTDC_HF_Speculation;
	///执行类型
	data3.ActionType = THOST_FTDC_ACTP_Exec;
	///保留头寸申请的持仓方向
	data3.PosiDirection = THOST_FTDC_PD_Long;
	///期权行权后是否保留期货头寸的标记,该字段已废弃
	//TThostFtdcExecOrderPositionFlagType	ReservePositionFlag;
	///期权行权后生成的头寸是否自动平仓
	data3.CloseFlag = THOST_FTDC_EOCF_NotToClose;
	///交易所代码
	//TThostFtdcExchangeIDType	ExchangeID;
	strncpy(data3.ExchangeID, exchange_id, sizeof(data3.ExchangeID));
	///投资单元代码
	//TThostFtdcInvestUnitIDType	InvestUnitID;
	///资金账号
	//TThostFtdcAccountIDType	AccountID;
	///币种代码
	//TThostFtdcCurrencyIDType	CurrencyID;
	///交易编码
	//TThostFtdcClientIDType	ClientID;
	///IP地址
	//TThostFtdcIPAddressType	IPAddress;
	///Mac地址
	//TThostFtdcMacAddressType	MacAddress;  
  pTraderApi->ReqExecOrderInsert(&data3, m_RequestId++);

  return ;
}

void CCtpSopTraderHandler::WithdrawOrder(char* inst, char* exchange_id, long sys_order_id)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcInputOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));
  
  //strcpy(req.OrderRef, orderRef); //报单引用	
  //req.FrontID = frontId;           //前置编号	
  //req.SessionID = sessionId;       //会话编号

	///经纪公司代码
	strncpy(inputOrderActionField.BrokerID, m_BrokerID, sizeof(inputOrderActionField.BrokerID));
	///投资者代码
	strncpy(inputOrderActionField.InvestorID, m_UserId, sizeof(inputOrderActionField.InvestorID));
	///报单操作引用
	inputOrderActionField.OrderActionRef = m_RequestId;
	///报单引用
  snprintf(inputOrderActionField.OrderSysID, sizeof(inputOrderActionField.OrderSysID), "%12ld", sys_order_id);
	///前置编号
	//inputOrderActionField.FrontID = front_id;
	///会话编号
	//inputOrderActionField.SessionID = session_id;
	///交易所代码
	strncpy(inputOrderActionField.ExchangeID, exchange_id, sizeof(inputOrderActionField.ExchangeID));
	///操作标志
	inputOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;
	///用户代码
	strncpy(inputOrderActionField.UserID, m_UserId, sizeof(inputOrderActionField.UserID));
	///合约代码
	strcpy(inputOrderActionField.InstrumentID, inst);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, m_RequestId++);

  return ;
}

void CCtpSopTraderHandler::QueryOrder(char* inst, char* exchange_id, long sys_order_id)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  struct CThostFtdcQryOrderField data3;
  memset(&data3, 0, sizeof(data3));
	///经纪公司代码
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者代码
	strncpy(data3.InvestorID, m_UserId, sizeof(data3.InvestorID));
	///合约代码
	//strncpy(data3.InstrumentID, inst, sizeof(data3.InstrumentID));
	///交易所代码
	strncpy(data3.ExchangeID, exchange_id, sizeof(data3.ExchangeID));
	///报单编号
  //snprintf(data3.OrderSysID, sizeof(data3.OrderSysID), "%ld", sys_order_id);
	///开始时间
	//TThostFtdcTimeType	InsertTimeStart;
	///结束时间
	//TThostFtdcTimeType	InsertTimeEnd;
	///投资单元代码
	//TThostFtdcInvestUnitIDType	InvestUnitID;

  pTraderApi->ReqQryOrder(&data3, m_RequestId++);

  return ;
}


void CCtpSopTraderHandler::CoveredOpen()
{
  CTP_LOG("2-备兑开仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Sell, THOST_FTDC_OFEN_Open, price, 1, THOST_FTDC_HF_Speculation);
  
}

void CCtpSopTraderHandler::CoveredClose()
{
  CTP_LOG("3-备兑平仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Buy, THOST_FTDC_OFEN_CloseToday, price, 1, THOST_FTDC_HF_Speculation);

}

void CCtpSopTraderHandler::CallBuyOpen()
{
  CTP_LOG("4-认购买入开仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Buy, THOST_FTDC_OFEN_Open, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::CallSellClose()
{
  CTP_LOG("5-认购卖出平仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Sell, THOST_FTDC_OFEN_Close, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::CallSellOpen()
{
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Sell, THOST_FTDC_OFEN_Open, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::CallBuyClose()
{
  CTP_LOG("7-认购买入平仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Buy, THOST_FTDC_OFEN_Close, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::PutBuyOpen()
{
  CTP_LOG("8-认沽买入开仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Buy, THOST_FTDC_OFEN_Open, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::PutSellClose()
{
  CTP_LOG("9-认沽卖出平仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Sell, THOST_FTDC_OFEN_Close, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::PutSellOpen()
{
  CTP_LOG("10-认沽卖出开仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Sell, THOST_FTDC_OFEN_Open, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::PutBuyClose()
{
  CTP_LOG("11-认沽买入平仓\n");
  char instrument[31];
  char exchangeid[9];
  double price;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入价格:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, exchangeid, THOST_FTDC_D_Buy, THOST_FTDC_OFEN_Close, price, 1, THOST_FTDC_HF_Speculation);
}

void CCtpSopTraderHandler::CallExecuteOrder()
{
  CTP_LOG("12-认购行权\n");
  char instrument[31];
  char exchangeid[9];
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  
  ExecuteOrder(instrument, exchangeid, 1);
}

void CCtpSopTraderHandler::PutExecuteOrder()
{
  CTP_LOG("13-认沽行权\n");
  char instrument[31];
  char exchangeid[9];
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  
  ExecuteOrder(instrument, exchangeid, 1);

}

void CCtpSopTraderHandler::LogOut()
{
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  struct CThostFtdcUserLogoutField data3;
  memset(&data3, 0, sizeof(data3));
	///经纪公司代码
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者代码
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));

  pTraderApi->ReqUserLogout(&data3, m_RequestId++);

  return ;
}

void CCtpSopTraderHandler::Query()
{
  CTP_LOG("14-订单查询\n");
  char instrument[31];
  char exchangeid[9];
  long sysOrderId;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入系统报单号:");
  scanf("%ld", &sysOrderId);
  QueryOrder(instrument, exchangeid, sysOrderId);
}
void CCtpSopTraderHandler::Withdraw()
{
  CTP_LOG("15-撤单\n");
  char instrument[31];
  char exchangeid[9];
  long sysOrderId;
  printf("请输入合约:");
  scanf("%s", instrument);
  printf("请输入交易所代码:");
  scanf("%s", exchangeid);
  printf("请输入系统报单号:");
  scanf("%ld", &sysOrderId);
  WithdrawOrder(instrument, exchangeid, sysOrderId);  
}



