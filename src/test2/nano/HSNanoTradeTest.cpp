#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "HSDataType.h"
#include "HSStruct.h"
#include "HSExtendTradeApi.h"

#include "HSNanoTradeTest.h"

#define NANO_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sUserId[128];
  char sOldPassword[128];
  char sFilename[128];
  int i = 1;

  if(argc < 7){
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
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  CHSExtendTradeApi* pTraderApi = (CHSExtendTradeApi*)NewTradeApi("log");
  CHSNanoTraderHandler* pTraderHandler = new CHSNanoTraderHandler(pTraderApi);

  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;

  FILE* fp;
  fp = fopen(sFilename, "w+");
  if(NULL != fp){
    fclose(fp);
  }

  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterSubModel(HS_TERT_RESUME);
  pTraderApi->RegisterFensServer(sAddress, sUserId);

  pTraderApi->SetFPGAParams(false, false);

  CHSInitConfigField initCfg;
  memset(&initCfg, 0, sizeof(initCfg));
  initCfg.APICheckVersion = API_STRUCT_CHECK_VERSION;
  pTraderApi->Init(&initCfg);

  pTraderHandler->Loop();

  pTraderApi->ReleaseApi();
  delete pTraderHandler;
  return 0;
}


CHSNanoTraderHandler::CHSNanoTraderHandler(CHSExtendTradeApi* api)
: m_Loop(0), m_Arg(api), m_RequestId(1)
{

}

CHSNanoTraderHandler::~CHSNanoTraderHandler()
{

}

int CHSNanoTraderHandler::_IsErrorRspInfo(CHSRspInfoField *pRspInfo)
{
  int ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret) {
    printf("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
  }
  return ret;
}

void CHSNanoTraderHandler::OnFrontConnected()
{
  NANO_LOG("%s\n", __FUNCTION__);

  CHSReqAuthenticateField req = {};
  strncpy(req.AccountID, m_UserId, sizeof(req.AccountID) - 1);
  strncpy(req.AppID, m_AppID, sizeof(req.AppID) - 1);
  strncpy(req.Password, m_OldPasswd, sizeof(req.Password) - 1);
  strncpy(req.AuthCode, m_AuthCode, sizeof(req.AuthCode) - 1);

  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;
  int nRet = pTraderApi->ReqAuthenticate(&req, m_RequestId++);
  if(nRet != 0){
    NANO_LOG("ReqAuthenticate Error: nRet=%d\n", nRet);
  }
  NANO_LOG("ReqAuthenticate Sent\n");
}

void CHSNanoTraderHandler::OnFrontDisconnected(int nResult)
{
  NANO_LOG("OnFrontDisconnected: nResult=%d\n", nResult);
}

void CHSNanoTraderHandler::OnRspAuthenticate(CHSRspAuthenticateField* pRspAuthenticate, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("OnRspAuthenticate: nRequestID=%d, bIsLast=%d\n", nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspAuthenticate && bIsLast == true){
    NANO_LOG("OnRspAuthenticate Error!\n");
  } else {
    if(pRspAuthenticate){
      NANO_LOG("pRspAuthenticate->AccountID=[%s]\n"
        "pRspAuthenticate->AppID=[%s]\n"
        "pRspAuthenticate->AuthCode=[%s]\n"
        , pRspAuthenticate->AccountID
        , pRspAuthenticate->AppID
        , pRspAuthenticate->AuthCode);
    }

    CHSReqUserLoginField req = {};
    strncpy(req.AccountID, m_UserId, sizeof(req.AccountID) - 1);
    strncpy(req.Password, m_OldPasswd, sizeof(req.Password) - 1);
    req.UserApplicationType = '7';

    CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;
    int nRet = pTraderApi->ReqUserLogin(&req, m_RequestId++);
    if(nRet != 0){
      NANO_LOG("ReqUserLogin Error: nRet=%d\n", nRet);
    }
    NANO_LOG("ReqUserLogin Sent\n");
  }
}

void CHSNanoTraderHandler::OnRspUserLogin(CHSRspUserLoginField* pRspUserLogin, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspUserLogin && bIsLast == true){
    NANO_LOG("OnRspUserLogin Last!\n");
    return;
  }

  if(pRspUserLogin){
    NANO_LOG("pRspUserLogin->BranchID=[%d]\n"
      "pRspUserLogin->AccountID=[%s]\n"
      "pRspUserLogin->UserName=[%s]\n"
      "pRspUserLogin->TradingDay=[%d]\n"
      "pRspUserLogin->PreTradingDay=[%d]\n"
      "pRspUserLogin->BillConfirmFlag=[%c]\n"
      "pRspUserLogin->SessionID=[%d]\n"
      "pRspUserLogin->UserApplicationType=[%c]\n"
      "pRspUserLogin->UserApplicationInfo=[%s]\n"
      "pRspUserLogin->RiskLevel=[%s]\n"
      "pRspUserLogin->LastMacAddress=[%s]\n"
      "pRspUserLogin->LastIPAddress=[%s]\n"
      "pRspUserLogin->LastLoginTime=[%d]\n"
      "pRspUserLogin->CZCETime=[%d]\n"
      "pRspUserLogin->DCETime=[%d]\n"
      "pRspUserLogin->SHFETime=[%d]\n"
      "pRspUserLogin->CFFEXTime=[%d]\n"
      "pRspUserLogin->INETime=[%d]\n"
      "pRspUserLogin->GFEXTime=[%d]\n"
      "pRspUserLogin->MaxOrderRef=[%s]\n"
      "pRspUserLogin->UserID=[%s]\n"
      , pRspUserLogin->BranchID
      , pRspUserLogin->AccountID
      , pRspUserLogin->UserName
      , pRspUserLogin->TradingDay
      , pRspUserLogin->PreTradingDay
      , pRspUserLogin->BillConfirmFlag
      , pRspUserLogin->SessionID
      , pRspUserLogin->UserApplicationType
      , pRspUserLogin->UserApplicationInfo
      , pRspUserLogin->RiskLevel
      , pRspUserLogin->LastMacAddress
      , pRspUserLogin->LastIPAddress
      , pRspUserLogin->LastLoginTime
      , pRspUserLogin->CZCETime
      , pRspUserLogin->DCETime
      , pRspUserLogin->SHFETime
      , pRspUserLogin->CFFEXTime
      , pRspUserLogin->INETime
      , pRspUserLogin->GFEXTime
      , pRspUserLogin->MaxOrderRef
      , pRspUserLogin->UserID
    );
  }
}

void CHSNanoTraderHandler::OnRspErrorOrderInsert(CHSRspOrderInsertField *pRspOrderInsert, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspOrderInsert && bIsLast == true){
    NANO_LOG("OnRspErrorOrderInsert Last!\n");
    return;
  }

  if(pRspOrderInsert){
    NANO_LOG("pRspOrderInsert->InstrumentID=[%s]\n"
      "pRspOrderInsert->Direction=[%c]\n"
      "pRspOrderInsert->OffsetFlag=[%c]\n"
      "pRspOrderInsert->OrderPrice=[%lf]\n"
      "pRspOrderInsert->OrderVolume=[%lf]\n",
      pRspOrderInsert->InstrumentID,
      pRspOrderInsert->Direction,
      pRspOrderInsert->OffsetFlag,
      pRspOrderInsert->OrderPrice,
      pRspOrderInsert->OrderVolume);
  }
}

void CHSNanoTraderHandler::OnRspOrderAction(CHSRspOrderActionField *pRspOrderAction, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspOrderAction && bIsLast == true){
    NANO_LOG("OnRspOrderAction Last!\n");
    return;
  }

  if(pRspOrderAction){
    NANO_LOG("pRspOrderAction->OrderSysID=[%s]\n"
      "pRspOrderAction->ExchangeID=[%s]\n",
      pRspOrderAction->OrderSysID,
      pRspOrderAction->ExchangeID);
  }
}

void CHSNanoTraderHandler::OnRspQryOrder(CHSOrderField *pRspQryOrder, CHSRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspQryOrder && bIsLast == true){
    NANO_LOG("OnRspQryOrder Last!\n");
    return;
  }

  if(pRspQryOrder){
    NANO_LOG("pRspQryOrder->InstrumentID=[%s]\n"
      "pRspQryOrder->OrderRef=[%s]\n"
      "pRspQryOrder->OrderSysID=[%s]\n"
      "pRspQryOrder->OrderPrice=[%lf]\n"
      "pRspQryOrder->OrderVolume=[%lf]\n"
      "pRspQryOrder->OrderStatus=[%c]\n"
      "pRspQryOrder->TradeVolume=[%lf]\n"
      "pRspQryOrder->TradingDay=[%d]\n"
      "pRspQryOrder->InsertDate=[%d]\n"
      "pRspQryOrder->InsertTime=[%d]\n",
      pRspQryOrder->InstrumentID,
      pRspQryOrder->OrderRef,
      pRspQryOrder->OrderSysID,
      pRspQryOrder->OrderPrice,
      pRspQryOrder->OrderVolume,
      pRspQryOrder->OrderStatus,
      pRspQryOrder->TradeVolume,
      pRspQryOrder->TradingDay,
      pRspQryOrder->InsertDate,
      pRspQryOrder->InsertTime);
  }
}

void CHSNanoTraderHandler::OnRspQryTrade(CHSTradeField* pRspQryTrade, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspQryTrade && bIsLast == true){
    NANO_LOG("OnRspQryTrade Last!\n");
    return;
  }

  if(pRspQryTrade){
    NANO_LOG("pRspQryTrade->InstrumentID=[%s]\n"
      "pRspQryTrade->OrderSysID=[%s]\n"
      "pRspQryTrade->ExchangeID=[%s]\n"
      "pRspQryTrade->TradeID=[%s]\n"
      "pRspQryTrade->Direction=[%c]\n"
      "pRspQryTrade->OffsetFlag=[%c]\n"
      "pRspQryTrade->TradePrice=[%lf]\n"
      "pRspQryTrade->TradeVolume=[%lf]\n"
      "pRspQryTrade->TradingDay=[%d]\n"
      "pRspQryTrade->TradeTime=[%d]\n",
      pRspQryTrade->InstrumentID,
      pRspQryTrade->OrderSysID,
      pRspQryTrade->ExchangeID,
      pRspQryTrade->TradeID,
      pRspQryTrade->Direction,
      pRspQryTrade->OffsetFlag,
      pRspQryTrade->TradePrice,
      pRspQryTrade->TradeVolume,
      pRspQryTrade->TradingDay,
      pRspQryTrade->TradeTime);
  }
}

void CHSNanoTraderHandler::OnRspQryInstrument(CHSRspQryInstrumentField* pRspQryInstrument, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspQryInstrument && bIsLast == true){
    NANO_LOG("OnRspQryInstrument Last!\n");
    return;
  }

  if(pRspQryInstrument){
    NANO_LOG("pRspQryInstrument->InstrumentID=[%s]\n"
      "pRspQryInstrument->ExchangeID=[%s]\n"
      "pRspQryInstrument->InstrumentName=[%s]\n"
      "pRspQryInstrument->VolumeMultiple=[%lf]\n"
      "pRspQryInstrument->PriceTick=[%lf]\n",
      pRspQryInstrument->InstrumentID,
      pRspQryInstrument->ExchangeID,
      pRspQryInstrument->InstrumentName,
      pRspQryInstrument->VolumeMultiple,
      pRspQryInstrument->PriceTick);
  }
}

void CHSNanoTraderHandler::OnRspQryPosition(CHSRspQryPositionField* pRspQryPosition, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspQryPosition && bIsLast == true){
    NANO_LOG("OnRspQryPosition Last!\n");
    return;
  }

  if(pRspQryPosition){
    NANO_LOG("pRspQryPosition->InstrumentID=[%s]\n"
      "pRspQryPosition->Direction=[%c]\n"
      "pRspQryPosition->PositionVolume=[%lf]\n"
      "pRspQryPosition->YdPositionVolume=[%lf]\n"
      "pRspQryPosition->TodayPositionVolume=[%lf]\n",
      pRspQryPosition->InstrumentID,
      pRspQryPosition->Direction,
      pRspQryPosition->PositionVolume,
      pRspQryPosition->YdPositionVolume,
      pRspQryPosition->TodayPositionVolume);
  }
}

void CHSNanoTraderHandler::OnRspQryTradingAccount(CHSRspQryTradingAccountField* pRspQryTradingAccount, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  NANO_LOG("%s: nRequestID=%d, bIsLast=%d\n", __FUNCTION__, nRequestID, bIsLast);

  if (_IsErrorRspInfo(pRspInfo)){
    return;
  }

  if(!pRspQryTradingAccount && bIsLast == true){
    NANO_LOG("OnRspQryTradingAccount Last!\n");
    return;
  }

  if(pRspQryTradingAccount){
    NANO_LOG("pRspQryTradingAccount->AccountID=[%s]\n",
      pRspQryTradingAccount->AccountID);
  }
}

void CHSNanoTraderHandler::OnRtnOrder(CHSOrderField* pRtnOrder)
{
  NANO_LOG("%s\n", __FUNCTION__);

  if(pRtnOrder){
    NANO_LOG("pRtnOrder->InstrumentID=[%s]\n"
      "pRtnOrder->OrderRef=[%s]\n"
      "pRtnOrder->OrderSysID=[%s]\n"
      "pRtnOrder->OrderPrice=[%lf]\n"
      "pRtnOrder->OrderVolume=[%lf]\n"
      "pRtnOrder->OrderStatus=[%c]\n"
      "pRtnOrder->TradeVolume=[%lf]\n"
      "pRtnOrder->TradingDay=[%d]\n"
      "pRtnOrder->InsertDate=[%d]\n"
      "pRtnOrder->InsertTime=[%d]\n",
      pRtnOrder->InstrumentID,
      pRtnOrder->OrderRef,
      pRtnOrder->OrderSysID,
      pRtnOrder->OrderPrice,
      pRtnOrder->OrderVolume,
      pRtnOrder->OrderStatus,
      pRtnOrder->TradeVolume,
      pRtnOrder->TradingDay,
      pRtnOrder->InsertDate,
      pRtnOrder->InsertTime);
  }
}

void CHSNanoTraderHandler::OnRtnTrade(CHSTradeField* pRtnTrade)
{
  NANO_LOG("%s\n", __FUNCTION__);

  if(pRtnTrade){
    NANO_LOG("pRtnTrade->InstrumentID=[%s]\n"
      "pRtnTrade->OrderSysID=[%s]\n"
      "pRtnTrade->ExchangeID=[%s]\n"
      "pRtnTrade->TradeID=[%s]\n"
      "pRtnTrade->Direction=[%c]\n"
      "pRtnTrade->OffsetFlag=[%c]\n"
      "pRtnTrade->TradePrice=[%lf]\n"
      "pRtnTrade->TradeVolume=[%lf]\n"
      "pRtnTrade->TradingDay=[%d]\n"
      "pRtnTrade->TradeTime=[%d]\n",
      pRtnTrade->InstrumentID,
      pRtnTrade->OrderSysID,
      pRtnTrade->ExchangeID,
      pRtnTrade->TradeID,
      pRtnTrade->Direction,
      pRtnTrade->OffsetFlag,
      pRtnTrade->TradePrice,
      pRtnTrade->TradeVolume,
      pRtnTrade->TradingDay,
      pRtnTrade->TradeTime);
  }
}

void CHSNanoTraderHandler::OnErrRtnOrderAction(CHSOrderActionField* pRtnOrderAction)
{
  NANO_LOG("%s\n", __FUNCTION__);
  NANO_LOG("pRtnOrderAction=%p\n", pRtnOrderAction);

  if(pRtnOrderAction){
    NANO_LOG("pRtnOrderAction->OrderSysID=[%s]\n"
      "pRtnOrderAction->ExchangeID=[%s]\n"
      "pRtnOrderAction->InstrumentID=[%s]\n"
      "pRtnOrderAction->OrderRef=[%s]\n"
      "pRtnOrderAction->Direction=[%c]\n"
      "pRtnOrderAction->OffsetFlag=[%c]\n"
      "pRtnOrderAction->OrderPrice=[%lf]\n"
      "pRtnOrderAction->OrderVolume=[%lf]\n"
      "pRtnOrderAction->OrderStatus=[%c]\n",
      pRtnOrderAction->OrderSysID,
      pRtnOrderAction->ExchangeID,
      pRtnOrderAction->InstrumentID,
      pRtnOrderAction->OrderRef,
      pRtnOrderAction->Direction,
      pRtnOrderAction->OffsetFlag,
      pRtnOrderAction->OrderPrice,
      pRtnOrderAction->OrderVolume,
      pRtnOrderAction->OrderStatus);
  }
}

void CHSNanoTraderHandler::OrderInsert()
{
  NANO_LOG("1-1-%s\n", "预埋单");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqOrderInsertField data;
  memset(&data, 0, sizeof(data));

  char sInstrumentID[21] = {0};
  char direction;
  char offsetFlag;
  char exchangeID[5] = {0};
  double orderPrice;
  int orderVolume;

  printf("请输入合约代码:");
  scanf("%s", sInstrumentID);
  printf("请输入交易所代码(F1-CZCE/F2-DCE/F3-SHFE/F4-CFFEX/F5-INE/F6-GFEX):");
  scanf("%s", exchangeID);
  printf("请输入方向(B/S):");
  scanf(" %c", &direction);
  printf("请输入开平标志(O/K/L/C):");
  scanf(" %c", &offsetFlag);
  printf("请输入价格:");
  scanf("%lf", &orderPrice);
  printf("请输入数量:");
  scanf("%d", &orderVolume);

  strncpy(data.ExchangeID, exchangeID, sizeof(data.ExchangeID) - 1);
  strncpy(data.InstrumentID, sInstrumentID, sizeof(data.InstrumentID) - 1);
  snprintf(data.OrderRef, sizeof(data.OrderRef), "%ld", m_RequestId);

  if(direction == 'B' || direction == 'b'){
    data.Direction = HS_DC_Buy;
  }else{
    data.Direction = HS_DC_Sell;
  }

  if(offsetFlag == 'O' || offsetFlag == 'o'){
    data.OffsetFlag = HS_OF_Open;
  }else if(offsetFlag == 'K' || offsetFlag == 'k'){
    data.OffsetFlag = HS_OF_Close;
  }else if(offsetFlag == 'L' || offsetFlag == 'l'){
    data.OffsetFlag = HS_OF_CloseToday;
  }else{
    data.OffsetFlag = HS_OF_Close;
  }

  data.HedgeType = HS_HT_Speculation;
  data.OrderCommand = HS_CT_Limit;
  data.OrderPrice = orderPrice;
  data.OrderVolume = orderVolume;
  data.MinVolume = 1;
  data.SeatIndex = 0;

  NANO_LOG("data.ExchangeID=[%s]\n"
    "data.InstrumentID=[%s]\n"
    "data.OrderRef=[%s]\n"
    "data.Direction=[%c]\n"
    "data.OffsetFlag=[%c]\n"
    "data.OrderPrice=[%lf]\n"
    "data.OrderVolume=[%lf]\n"
    "data.OrderCommand=[%d]\n",
    data.ExchangeID,
    data.InstrumentID,
    data.OrderRef,
    data.Direction,
    data.OffsetFlag,
    data.OrderPrice,
    data.OrderVolume,
    data.OrderCommand);

  int nRet = pTraderApi->ReqOrderInsert(&data, m_RequestId++);
  NANO_LOG("ReqOrderInsert[%d]\n", nRet);
}

void CHSNanoTraderHandler::OrderAction()
{
  NANO_LOG("1-2-%s\n", "撤单");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqOrderActionField data;
  memset(&data, 0, sizeof(data));

  char orderSys[17] = {0};
  char exchange[5] = {0};

  printf("请输入被撤单号(OrderSysID):");
  scanf("%s", orderSys);
  printf("请输入交易所代码(F1-CZCE/F2-DCE/F3-SHFE/F4-CFFEX/F5-INE/F6-GFEX):");
  scanf("%s", exchange);

  strncpy(data.OrderSysID, orderSys, sizeof(data.OrderSysID) - 1);
  strncpy(data.ExchangeID, exchange, sizeof(data.ExchangeID) - 1);
  strncpy(data.OrderRef, "0", sizeof(data.OrderRef) - 1);
  strncpy(data.OrderActionRef, "0", sizeof(data.OrderActionRef) - 1);

  NANO_LOG("data.OrderSysID=[%s]\n"
    "data.ExchangeID=[%s]\n"
    "data.OrderRef=[%s]\n"
    "data.OrderActionRef=[%s]\n",
    data.OrderSysID,
    data.ExchangeID,
    data.OrderRef,
    data.OrderActionRef);

  int nRet = pTraderApi->ReqOrderAction(&data, m_RequestId++);
  NANO_LOG("ReqOrderAction[%d]\n", nRet);
}

void CHSNanoTraderHandler::QryOrder()
{
  NANO_LOG("3-%s\n", "报单查询");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqQryOrderField data;
  memset(&data, 0, sizeof(data));

  int nRet = pTraderApi->ReqQryOrder(&data, m_RequestId++);
  NANO_LOG("ReqQryOrder[%d]\n", nRet);
}

void CHSNanoTraderHandler::QryTrade()
{
  NANO_LOG("4-%s\n", "成交查询");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqQryTradeField data;
  memset(&data, 0, sizeof(data));

  int nRet = pTraderApi->ReqQryTrade(&data, m_RequestId++);
  NANO_LOG("ReqQryTrade[%d]\n", nRet);
}

void CHSNanoTraderHandler::QryInvestorAccount()
{
  NANO_LOG("5-%s\n", "资金账户查询");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqQryTradingAccountField data;
  memset(&data, 0, sizeof(data));

  int nRet = pTraderApi->ReqQryTradingAccount(&data, m_RequestId++);
  NANO_LOG("ReqQryTradingAccount[%d]\n", nRet);
}

void CHSNanoTraderHandler::QryInvestorPosition()
{
  NANO_LOG("6-%s\n", "持仓查询");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqQryPositionField data;
  memset(&data, 0, sizeof(data));

  char exchangeID[5] = {0};
  char instrumentID[21] = {0};

  printf("请输入交易所代码(留空查全部):");
  scanf("%s", exchangeID);
  printf("请输入合约代码(留空查全部):");
  scanf("%s", instrumentID);

  if(strlen(exchangeID) > 0) strncpy(data.ExchangeID, exchangeID, sizeof(data.ExchangeID) - 1);
  if(strlen(instrumentID) > 0) strncpy(data.InstrumentID, instrumentID, sizeof(data.InstrumentID) - 1);

  int nRet = pTraderApi->ReqQryPosition(&data, m_RequestId++);
  NANO_LOG("ReqQryPosition[%d]\n", nRet);
}

void CHSNanoTraderHandler::QryInstrument()
{
  NANO_LOG("7-%s\n", "持仓查询");
  CHSTradeApi* pTraderApi = (CHSTradeApi*)m_Arg;

  CHSReqQryInstrumentField data;
  memset(&data, 0, sizeof(data));
  int nRet = pTraderApi->ReqQryInstrument(&data, m_RequestId++);
  NANO_LOG("ReqQryInstrument[%d]\n", nRet);

}


void CHSNanoTraderHandler::Loop()
{
  int choose;
  m_Loop = 1;
  do{
    choose = ShowMenu();
    switch(choose){
    case 1:
      OrderInsert();
      break;
    case 2:
      OrderAction();
      break;
    case 3:
      QryOrder();
      break;
    case 4:
      QryTrade();
      break;
    case 5:
      QryInvestorAccount();
      break;
    case 6:
      QryInvestorPosition();
      break;
    case 7:
      QryInstrument();
      break;
    case 9:
      m_Loop = 0;
      sleep(1);
      break;
    default:
      break;
    }
  }while(m_Loop);
  return ;
}

int CHSNanoTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
    "请选择需要执行的操作\n"
    "1-OrderInsert\n"
    "2-OrderAction\n"
    "3-QryOrder\n"
    "4-QryTrade\n"
    "5-QryInvestorAccount\n"
    "6-QryInvestorPosition\n"
    "7-QryInstrument\n"
    "9-Exit\n"
    "**********************\n"
    "请选择:");
  scanf("%d",&ret);

  return ret;
}
