#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "HSDataType.h"
#include "HSStruct.h"
#include "HSExtendTradeApi.h"
#include "HSExtendStruct.h"
#include "NanoTraderHandler.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

#include "cmn_log.h"

#ifdef __cplusplus
}
#endif

CNanoTraderHandler::CNanoTraderHandler(CHSExtendTradeApi* pApi, void* pArg)
  : m_Arg(pArg), m_TraderApi(pApi), m_TraderRequestID(0)
{

}

CNanoTraderHandler::~CNanoTraderHandler()
{
}

void CNanoTraderHandler::OnFrontConnected()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_connected(self);
}

void CNanoTraderHandler::OnFrontDisconnected(int nResult)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_disconnected(self, nResult);
}

void CNanoTraderHandler::OnRspAuthenticate(CHSRspAuthenticateField* pRspAuthenticate, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    trader_trader_api_on_rsp_user_login(self, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }
  if(!pRspAuthenticate && bIsLast == true )
  {
    return;
  }

  CHSReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strncpy(reqUserLoginField.AccountID, self->pUser, sizeof(reqUserLoginField.AccountID) - 1);
  strncpy(reqUserLoginField.Password, self->pPwd, sizeof(reqUserLoginField.Password) - 1);
  reqUserLoginField.UserApplicationType = '7';

  m_TraderApi->ReqUserLogin(&reqUserLoginField, m_TraderRequestID++);
}

void CNanoTraderHandler::OnRspUserLogin(CHSRspUserLoginField* pRspUserLogin, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  CMN_DEBUG("Enter\n");
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    trader_trader_api_on_rsp_user_login(self, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }

  if(!pRspUserLogin && bIsLast == true){
    return;
  }

  CMN_INFO("pRspUserLogin->BranchID=[%d]\n"
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

  m_TradingDate = pRspUserLogin->TradingDay;

  m_MaxOrderRef = atol(pRspUserLogin->MaxOrderRef);

  trader_trader_api_on_rsp_user_login(self, pRspInfo->ErrorID, NULL);
  return;
}

void CNanoTraderHandler::OnRspErrorOrderInsert(CHSRspOrderInsertField* pRspOrderInsert, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }


}

void CNanoTraderHandler::OnRspOrderAction(CHSRspOrderActionField* pRspOrderAction, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }

}

void CNanoTraderHandler::OnRspQryPosition(CHSRspQryPositionField* pRspQryPosition, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));

  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
    return;
  }

  if(!pRspQryPosition && bIsLast == true )
  {
    CMN_INFO("OnRspQryPosition Last!\n");
    trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, bIsLast);
    return;
  }

  strncpy(traderPosition.InstrumentID, pRspQryPosition->InstrumentID, sizeof(traderPosition.InstrumentID) - 1);
  if(HS_DC_Buy == pRspQryPosition->Direction){
    traderPosition.PosiDirection = TRADER_POSITION_LONG;
  }else{
    traderPosition.PosiDirection = TRADER_POSITION_SHORT;
  }
  //TODO
  traderPosition.IsSHFE = 0;
  traderPosition.PositionDate = '3';
  // 当前昨仓
  traderPosition.YdPosition = (int)pRspQryPosition->PositionVolume;
  // 当前今仓
  traderPosition.TodayPosition = (int)pRspQryPosition->TodayPositionVolume;
  // 冻结的今仓数量
  traderPosition.Position = (int)(pRspQryPosition->TodayPositionVolume - pRspQryPosition->TodayAvailablePositionVolume);
  // 冻结的昨仓数量
  traderPosition.LongFrozen = (int)pRspQryPosition->CloseFrozenVolume;

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
}

void CNanoTraderHandler::OnRspQryTradingAccount(CHSRspQryTradingAccountField* pRspQryTradingAccount, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
    return;
  }

  if(!pRspQryTradingAccount && bIsLast == true )
  {
    CMN_INFO("OnRspQryTradingAccount Last!\n");
    trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, 0, NULL, bIsLast);
    return;
  }

  strncpy(traderAccount.AccountID, pRspQryTradingAccount->AccountID, sizeof(traderAccount.AccountID) - 1);

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
  return;
}

void CNanoTraderHandler::OnRspQryInstrument(CHSRspQryInstrumentField* pRspQryInstrument, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
    return;
  }

  if(!pRspQryInstrument && bIsLast == true )
  {
    CMN_INFO("OnRspQryInstrument Last!\n");
    trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, bIsLast);
    return;
  }

  std::string strKey(pRspQryInstrument->InstrumentID);
  std::string strVal(pRspQryInstrument->ExchangeID);
  mapHSExchangeID[strKey] = strVal;

  strncpy(traderInstrument.InstrumentID, pRspQryInstrument->InstrumentID, sizeof(traderInstrument.InstrumentID) - 1);
  strncpy(traderInstrument.ExchangeID, pRspQryInstrument->ExchangeID, sizeof(traderInstrument.ExchangeID) - 1);
  traderInstrument.VolumeMultiple = (int)pRspQryInstrument->VolumeMultiple;
  traderInstrument.PriceTick = pRspQryInstrument->PriceTick;
  traderInstrument.UnitMargin = 0.0;

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, pRspInfo->ErrorID, pRspInfo->ErrorMsg, bIsLast);
  return;
}

void CNanoTraderHandler::OnRspQryPositionDetail(CHSRspQryPositionDetailField* pRspQryPositionDetail, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }

  if(!pRspQryPositionDetail && bIsLast == true )
  {
    CMN_INFO("OnRspQryPositionDetail Last!\n");
    return;
  }

  return;
}

void CNanoTraderHandler::OnRspQryDepthMarketData(CHSDepthMarketDataField* pRspQryDepthMarketData, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }
  
  if(!pRspQryDepthMarketData && bIsLast == true )
  {
    CMN_INFO("OnRspQryDepthMarketData Last!\n");
    return;
  }

  CMN_INFO("pRspQryDepthMarketData->InstrumentID=[%s]\n"
    "pRspQryDepthMarketData->ExchangeID=[%s]\n"
    "pRspQryDepthMarketData->UpdateTime=[%06d.%03d]\n"
    "pRspQryDepthMarketData->UpperLimitPrice=[%lf]\n"
    "pRspQryDepthMarketData->LowerLimitPrice=[%lf]\n"
    , pRspQryDepthMarketData->InstrumentID
    , pRspQryDepthMarketData->ExchangeID
    , pRspQryDepthMarketData->UpdateTime / 1000
    , pRspQryDepthMarketData->UpdateTime % 1000
    , pRspQryDepthMarketData->UpperLimitPrice
    , pRspQryDepthMarketData->LowerLimitPrice
  );
  return;
}

void CNanoTraderHandler::OnRspQrySeatID(CHSRspQrySeatIDField* pRspQrySeatID, CHSRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if((pRspInfo) && (pRspInfo->ErrorID != 0)){
    CMN_ERROR("ErrorID:%d, ErrorMsg:%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    return;
  }
  
  if(!pRspQrySeatID && bIsLast == true )
  {
    CMN_INFO("OnRspQrySeatID Last!\n");
    return;
  }

  CMN_INFO("pRspQrySeatID->SeatID=[%s]\n"
    "pRspQrySeatID->SeatIndex=[%d]\n"
    , pRspQrySeatID->SeatID
    , pRspQrySeatID->SeatIndex
  );
  return;
}

void CNanoTraderHandler::OnRtnTrade(CHSTradeField* pRtnTrade)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strncpy(traderTrade.InstrumentID, pRtnTrade->InstrumentID, sizeof(traderTrade.InstrumentID) - 1);
  ///本地报单编号
  FromHSOrderRef(traderTrade.UserOrderLocalID, pRtnTrade->OrderRef, sizeof(traderTrade.UserOrderLocalID));
  ///交易日
  snprintf(traderTrade.TradingDay, sizeof(traderTrade.TradingDay), "%d", pRtnTrade->TradingDay);
  ///成交时间
  FromHSTime(traderTrade.TradeTime, sizeof(traderTrade.TradeTime), pRtnTrade->TradeTime);
  ///买卖方向
  traderTrade.Direction = FromHSDirection(pRtnTrade->Direction);
  ///开平标志
  traderTrade.OffsetFlag = FromHSOffsetFlag(pRtnTrade->OffsetFlag);
  ///成交价格
  traderTrade.TradePrice = pRtnTrade->TradePrice;
  ///成交数量
  traderTrade.TradeVolume = (int)pRtnTrade->TradeVolume;
  //成交编号
  strncpy(traderTrade.TradeID, pRtnTrade->TradeID, sizeof(traderTrade.TradeID) - 1);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void CNanoTraderHandler::OnRtnOrder(CHSOrderField* pRtnOrder)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));

	///交易所代码
  strncpy(traderOrder.ExchangeID, pRtnOrder->ExchangeID, sizeof(traderOrder.ExchangeID) - 1);
	///系统报单编号
  strncpy(traderOrder.OrderSysID, pRtnOrder->OrderSysID, sizeof(traderOrder.OrderSysID) - 1);
  // 合约代码
  strncpy(traderOrder.InstrumentID, pRtnOrder->InstrumentID, sizeof(traderOrder.InstrumentID) - 1);
  // 本地报单编号
  FromHSOrderRef(traderOrder.UserOrderLocalID, pRtnOrder->OrderRef, sizeof(traderOrder.UserOrderLocalID) - 1);
  // 买卖
  traderOrder.Direction = FromHSDirection(pRtnOrder->Direction);
  // 开平
  traderOrder.OffsetFlag = FromHSOffsetFlag(pRtnOrder->OffsetFlag);
  ///投机套保标志
  traderOrder.HedgeFlag = pRtnOrder->HedgeType;
  // 报单价格
  traderOrder.LimitPrice = pRtnOrder->OrderPrice;
  // 报单手数
  traderOrder.VolumeOriginal = (int)pRtnOrder->OrderVolume;
  // 成交手数
  traderOrder.VolumeTraded = (int)pRtnOrder->TradeVolume;
  // 订单状态
  traderOrder.OrderStatus = FromHSOrderStatus(pRtnOrder->OrderStatus);
  ///插入时间
  FromHSTime(traderOrder.InsertTime, sizeof(traderOrder.InsertTime), pRtnOrder->InsertTime / 1000);

  trader_trader_api_on_rtn_order(self, &traderOrder);
}

void CNanoTraderHandler::OnErrRtnOrderAction(CHSOrderActionField* pRtnOrderAction)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
	///交易所代码
  strncpy(traderOrder.ExchangeID, pRtnOrderAction->ExchangeID, sizeof(traderOrder.ExchangeID) - 1);
	///系统报单编号
  strncpy(traderOrder.OrderSysID, pRtnOrderAction->OrderSysID, sizeof(traderOrder.OrderSysID) - 1);
  // 合约代码
  strncpy(traderOrder.InstrumentID, pRtnOrderAction->InstrumentID, sizeof(traderOrder.InstrumentID) - 1);
  // 本地报单编号
  FromHSOrderRef(traderOrder.UserOrderLocalID, pRtnOrderAction->OrderRef, sizeof(traderOrder.UserOrderLocalID) - 1);
  traderOrder.Direction = FromHSDirection(pRtnOrderAction->Direction);
  traderOrder.OffsetFlag = FromHSOffsetFlag(pRtnOrderAction->OffsetFlag);
  traderOrder.HedgeFlag = pRtnOrderAction->HedgeType;
  traderOrder.LimitPrice = pRtnOrderAction->OrderPrice;
  traderOrder.VolumeOriginal = (int)pRtnOrderAction->OrderVolume;
  traderOrder.OrderStatus = FromHSOrderStatus(pRtnOrderAction->OrderStatus);

  trader_trader_api_on_rtn_order(self, &traderOrder);
}

void CNanoTraderHandler::InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  CHSReqOrderInsertField reqOrderField;
  memset(&reqOrderField, 0, sizeof(reqOrderField));

  const char* pHSExchangeID = GetHSExchangeID(inst);
  if(pHSExchangeID){
    strncpy(reqOrderField.ExchangeID, pHSExchangeID, sizeof(reqOrderField.ExchangeID) - 1);
  }
  strncpy(reqOrderField.InstrumentID, inst, sizeof(reqOrderField.InstrumentID) - 1);

  strncpy(reqOrderField.OrderRef, local_id, sizeof(reqOrderField.OrderRef) - 1);

  switch(buy_sell){
    case TRADER_POSITION_BUY: reqOrderField.Direction = HS_DC_Buy; break;
    case TRADER_POSITION_SELL: reqOrderField.Direction = HS_DC_Sell; break;
    default: reqOrderField.Direction = HS_DC_Buy; break;
  }

  switch(open_close){
    case TRADER_POSITION_OPEN: reqOrderField.OffsetFlag = HS_OF_Open; break;
    case TRADER_POSITION_CLOSE: reqOrderField.OffsetFlag = HS_OF_Close; break;
    case TRADER_POSITION_CLOSE_TODAY: reqOrderField.OffsetFlag = HS_OF_CloseToday; break;
    case TRADER_POSITION_CLOSE_YESTERDAY: reqOrderField.OffsetFlag = HS_OF_Close; break;
    default: reqOrderField.OffsetFlag = HS_OF_Open; break;
  }

  reqOrderField.HedgeType = HS_HT_Speculation;
  reqOrderField.OrderPrice = price;
  reqOrderField.OrderVolume = vol;
  reqOrderField.OrderCommand = HS_CT_Limit;
  reqOrderField.MinVolume = 1;
  reqOrderField.SeatIndex = 0;

  m_TraderApi->ReqOrderInsert(&reqOrderField, m_TraderRequestID++);
}

void CNanoTraderHandler::CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id)
{
  CHSReqOrderActionField reqActionField;
  memset(&reqActionField, 0, sizeof(reqActionField));

  strncpy(reqActionField.ExchangeID, exchange_id, sizeof(reqActionField.ExchangeID) - 1);
  strncpy(reqActionField.OrderSysID, order_sys_id, sizeof(reqActionField.OrderSysID) - 1);

  m_TraderApi->ReqOrderAction(&reqActionField, m_TraderRequestID++);
}

void CNanoTraderHandler::Login()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  CHSReqAuthenticateField reqAuthenticateField;
  memset(&reqAuthenticateField, 0, sizeof(reqAuthenticateField));

  strncpy(reqAuthenticateField.AccountID, self->pUser, sizeof(reqAuthenticateField.AccountID) - 1);
  strncpy(reqAuthenticateField.AppID, self->pAppID, sizeof(reqAuthenticateField.AppID) - 1);
  strncpy(reqAuthenticateField.Password, self->pPwd, sizeof(reqAuthenticateField.Password) - 1);
  strncpy(reqAuthenticateField.AuthCode, self->pAuthCode,sizeof(reqAuthenticateField.AuthCode) - 1);

  m_TraderApi->ReqAuthenticate(&reqAuthenticateField, m_TraderRequestID++);
}

void CNanoTraderHandler::EfivInsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
    CNakedProtocolReqOrderInsertField orderInsertReq;
    memset(&orderInsertReq, 0, sizeof(orderInsertReq));

    orderInsertReq.Ver = FUTU_EFVI_REQUET::PACK_VER;
    orderInsertReq.FuncType = FUTU_EFVI_REQUET::EnumFuncType::FT_Order;
    orderInsertReq.OrderVolume = vol;
    orderInsertReq.OrderPrice = price;
    orderInsertReq.OrderRef = atol(local_id);
    strncpy(orderInsertReq.InstrumentID, inst, sizeof(orderInsertReq.InstrumentID) - 1);

    switch(buy_sell){
        case TRADER_POSITION_BUY: orderInsertReq.IsSell = 0; break;
        case TRADER_POSITION_SELL: orderInsertReq.IsSell = 1; break;
        default: orderInsertReq.IsSell = 0; break;
    }

    switch(open_close){
        case TRADER_POSITION_OPEN: orderInsertReq.OffsetFlag = FUTU_EFVI_REQUET::EnumOffsetFlag::OF_Open; break;
        case TRADER_POSITION_CLOSE: orderInsertReq.OffsetFlag = FUTU_EFVI_REQUET::EnumOffsetFlag::OF_Close; break;
        case TRADER_POSITION_CLOSE_TODAY: orderInsertReq.OffsetFlag = FUTU_EFVI_REQUET::EnumOffsetFlag::OF_CloseToday; break;
        case TRADER_POSITION_CLOSE_YESTERDAY: orderInsertReq.OffsetFlag = FUTU_EFVI_REQUET::EnumOffsetFlag::OF_Close; break;
        default: orderInsertReq.OffsetFlag = FUTU_EFVI_REQUET::EnumOffsetFlag::OF_Open; break;
    }

    orderInsertReq.IsMarket = 0;
    orderInsertReq.IsFak = false;
    orderInsertReq.IsFok = false;
    orderInsertReq.IsGIS = false;
    orderInsertReq.SeatMode = false;
    orderInsertReq.SeatIndex = 0;

    const char* pHSExchangeID = GetHSExchangeID(inst);
    if(pHSExchangeID){
        if(strcmp(pHSExchangeID, HS_EI_CZCE) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Czce;
        }
        else if(strcmp(pHSExchangeID, HS_EI_DCE) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Dce;
        }
        else if(strcmp(pHSExchangeID, HS_EI_SHFE) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Shfe;
        }
        else if(strcmp(pHSExchangeID, HS_EI_CFFEX) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
        }
        else if(strcmp(pHSExchangeID, HS_EI_INE) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Ine;
        }
        else if(strcmp(pHSExchangeID, HS_EI_GFEX) == 0){
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Gfe;
        }
        else {
            orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
        }
    }
    else {
        orderInsertReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
    }

    orderInsertReq.Token = 0;

    m_TraderApi->ReqNakedProtocolOrderInsert(&orderInsertReq);
}

void CNanoTraderHandler::EfivCancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id)
{
    CNakedProtocolReqOrderActionField orderActionReq;
    memset(&orderActionReq, 0, sizeof(orderActionReq));

    orderActionReq.Ver = FUTU_EFVI_REQUET::PACK_VER;
    orderActionReq.FuncType = FUTU_EFVI_REQUET::EnumFuncType::FT_CancelOrder;
    strncpy(orderActionReq.OrderSysID, order_sys_id, sizeof(orderActionReq.OrderSysID) - 1);
    orderActionReq.SeatIndex = 0;
    orderActionReq.ActionMode = 0;
    orderActionReq.Token = 0;
    memset(orderActionReq.Reserve, 0, sizeof(orderActionReq.Reserve));

    const char* pHSExchangeID = GetHSExchangeID(inst);
    if(pHSExchangeID){
        if(strcmp(pHSExchangeID, HS_EI_CZCE) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Czce;
        }
        else if(strcmp(pHSExchangeID, HS_EI_DCE) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Dce;
        }
        else if(strcmp(pHSExchangeID, HS_EI_SHFE) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Shfe;
        }
        else if(strcmp(pHSExchangeID, HS_EI_CFFEX) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
        }
        else if(strcmp(pHSExchangeID, HS_EI_INE) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Ine;
        }
        else if(strcmp(pHSExchangeID, HS_EI_GFEX) == 0){
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Gfe;
        }
        else {
            orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
        }
    }
    else {
        orderActionReq.ExchangeID = FUTU_EFVI_REQUET::EnumExchType::ET_Cffex;
    }

    m_TraderApi->ReqNakedProtocolOrderAction(&orderActionReq);
}

void CNanoTraderHandler::QryInstrument()
{
  CHSReqQryInstrumentField qryInstrument;
  memset(&qryInstrument, 0, sizeof(qryInstrument));

  m_TraderApi->ReqQryInstrument(&qryInstrument, m_TraderRequestID++);
  return;
}

void CNanoTraderHandler::QryPosition()
{
  CHSReqQryPositionField qryPosition;
  memset(&qryPosition, 0, sizeof(qryPosition));

  m_TraderApi->ReqQryPosition(&qryPosition, m_TraderRequestID++);
  return;
}

void CNanoTraderHandler::QryTradingAccount()
{
  CHSReqQryTradingAccountField qryAccount;
  memset(&qryAccount, 0, sizeof(qryAccount));

  m_TraderApi->ReqQryTradingAccount(&qryAccount, m_TraderRequestID++);
  return;
}

long CNanoTraderHandler::GetMaxOrderRef()
{
  return m_MaxOrderRef;
}

int CNanoTraderHandler::GetTradingDate()
{
  return m_TradingDate;
}

const char* CNanoTraderHandler::GetHSExchangeID(const char* inst)
{
  std::string strKey(inst);
  auto it = mapHSExchangeID.find(strKey);
  if(it != mapHSExchangeID.end()){
    return it->second.c_str();
  }
  return nullptr;
}

inline char CNanoTraderHandler::FromHSOffsetFlag(HSOffsetFlag offset)
{
  if(HS_OF_Open == offset){
    return TRADER_POSITION_OPEN;
  }

  if(HS_OF_CloseToday == offset){
    return TRADER_POSITION_CLOSE_TODAY;
  }

  return TRADER_POSITION_CLOSE;
}

inline char CNanoTraderHandler::FromHSDirection(HSDirection direction)
{
  if(HS_DC_Buy == direction){
    return TRADER_POSITION_BUY;
  }
  return TRADER_POSITION_SELL;
}

inline char CNanoTraderHandler::FromHSOrderStatus(HSOrderStatus order_status)
{
  if(HS_OS_Reported == order_status){
    return TRADER_ORDER_OS_NOTRADEQUEUEING;
  }

  if(HS_OS_Traded == order_status){
    return TRADER_ORDER_OS_ALLTRADED;
  }

  if(HS_OS_Canceled == order_status){
    return TRADER_ORDER_OS_CANCELED;
  }

  if(HS_OS_PartsTraded == order_status){
    return TRADER_ORDER_OS_PARTTRADEDQUEUEING;
  }

  if(HS_OS_PartsTradedToBeCancel == order_status){
    return TRADER_ORDER_OS_PARTTRADEDNOTQUEUEING;
  }

  if(HS_OS_ReportedToBeCancel == order_status){
    return TRADER_ORDER_OS_NOTRADENOTQUEUEING;
  }

  if(HS_OS_CanceledWithPartsTraded == order_status){
    return TRADER_ORDER_OS_CANCELED;
  }
  
  CMN_ERROR("order_status:%c\n", order_status);
  return TRADER_ORDER_OS_ACCEPTEDNOREPLY;
}

inline int CNanoTraderHandler::FromHSOrderRef(char* local_id, HSRef order_ref, int size)
{
  long orderRef = atol(order_ref);
  
  snprintf(local_id, size, "%08ld", orderRef);
  
  return 0;
}

inline int CNanoTraderHandler::FromHSTime(char* trade_time, int size, HSTime hhmmss)
{
  int hh = hhmmss / 10000;
  int mm = (hhmmss / 100) % 100;
  int ss = hhmmss % 100;
  
  snprintf(trade_time, size, "%02d:%02d:%02d", hh, mm, ss);
  
  return 0;
}





