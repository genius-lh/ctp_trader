
#include "ctp_trader_api.h"
#include "CtpTraderHandler.h"
#include "ThostFtdcTraderApi.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/util.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_msg_struct.h"
#include "cmn_util.h"
#include "cmn_log.h"

static ctp_trader_api_cb* ctp_trader_api_cb_get();
static ctp_trader_api_method* ctp_trader_api_method_get();

static void ctp_trader_on_front_connected(void* arg);
static void ctp_trader_on_front_disconnected(void* arg, int nReason);
static void ctp_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
static void ctp_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
static void ctp_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade);
static void ctp_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder);
static void ctp_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
static void ctp_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
static void ctp_trader_on_rsp_qry_user_investor(void* arg, CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_qry_trading_account(void* arg, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus);

static int ctp_trader_api_init(ctp_trader_api* self);
static int ctp_trader_api_start(ctp_trader_api* self, char* user_id, char* pwd);
static int ctp_trader_api_stop(ctp_trader_api* self);
static int ctp_trader_api_order_insert(ctp_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int ctp_trader_api_order_action(ctp_trader_api* self, char* inst, char* local_id, char* org_local_id,  char* exchange_id, char* order_sys_id);
static int ctp_trader_api_query_instrument(ctp_trader_api* self);
static int ctp_trader_api_query_user_investor(ctp_trader_api* self);
static int ctp_trader_api_get_tradingday(ctp_trader_api* self, char* tradingday);
static int ctp_trader_api_get_maxuserlocalid(ctp_trader_api* self, char* local_id);

static int ctp_trader_api_mng(ctp_trader_api* self, int error_cd, char* error_msg);
static int ctp_trader_api_qry_instrument(ctp_trader_api* self, char* instrument, int last, int err_cd);
static int ctp_trader_api_qry_settlement_info_confirm(ctp_trader_api* self);
static int ctp_trader_api_qry_investor_position(ctp_trader_api* self);

#ifdef __cplusplus
}
#endif

ctp_trader_api_cb* ctp_trader_api_cb_get()
{
  static ctp_trader_api_cb ctp_trader_api_cb_st = {
    ctp_trader_on_front_connected,
    ctp_trader_on_front_disconnected,
    ctp_trader_on_rsp_user_login,
    ctp_trader_on_rsp_user_logout,
    ctp_trader_on_rsp_error,
    ctp_trader_on_rsp_order_insert,
    ctp_trader_on_rsp_order_action,
    ctp_trader_on_rtn_order,
    ctp_trader_on_rtn_trade,
    ctp_trader_on_err_rtn_order_insert,
    ctp_trader_on_err_rtn_order_action,
    ctp_trader_on_rsp_qry_user_investor,
    ctp_trader_on_rsp_qry_instrument,
    ctp_trader_on_rsp_settlement_info_confirm,
    ctp_trader_on_rsp_qry_trading_account,
    ctp_trader_on_rsp_qry_investor_position,
    ctp_trader_on_rtn_instrument_status
  };

  return &ctp_trader_api_cb_st;

}

ctp_trader_api_method* ctp_trader_api_method_get()
{
  static ctp_trader_api_method ctp_trader_api_method_st = {
    ctp_trader_api_start,
    ctp_trader_api_stop,
    ctp_trader_api_order_insert,
    ctp_trader_api_order_action,
    ctp_trader_api_query_instrument,
    ctp_trader_api_query_user_investor,
    ctp_trader_api_get_tradingday,
    ctp_trader_api_get_maxuserlocalid,
    ctp_trader_api_qry_settlement_info_confirm,
    ctp_trader_api_qry_investor_position
  };

  return &ctp_trader_api_method_st;
}



void ctp_trader_on_front_connected(void* arg)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)self->pUserApi;
  
  CThostFtdcReqUserLoginField reqUserLogin;
  memset(&reqUserLogin,0,sizeof(CThostFtdcReqUserLoginField));
  strcpy(reqUserLogin.BrokerID, self->BrokenId);
  strcpy(reqUserLogin.UserID, self->UserId);
  strcpy(reqUserLogin.Password, self->Passwd);
  strcpy(reqUserLogin.UserProductInfo,"Trader Login");
  pUserApi->ReqUserLogin(&reqUserLogin, self->nLocalId++);
  
  return;
}


void ctp_trader_on_front_disconnected(void* arg, int nReason)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
}


void ctp_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
}


void ctp_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;
  
  if(0 != pRspInfo->ErrorID){
    // 通知客户端登录失败
    ctp_trader_api_mng(self, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }

  // 获取OrderId
  CMN_DEBUG("pRspUserLogin->MaxOrderLocalID[%s]\n", pRspUserLogin->MaxOrderRef);
  strcpy(self->MaxOrderLocalID, pRspUserLogin->MaxOrderRef);
  
  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(CThostFtdcSettlementInfoConfirmField));

  strcpy(req.BrokerID, self->BrokenId);
  strcpy(req.InvestorID, self->UserId);
  strcpy(req.ConfirmDate, pTrader->GetTradingDay());
  strcpy(req.ConfirmTime, "09:00:00");
  
  pTrader->ReqSettlementInfoConfirm(&req, self->nLocalId++);
  
}


void ctp_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  if(0 != pRspInfo->ErrorID){
    // 通知客户端登出失败
    //TODO

    return ;
  }
  self->bInit = 0;

  return;
}


void ctp_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;

  if(pRspInfo != NULL && 0 != pRspInfo->ErrorID){
    // 通知客户端报单失败
    CMN_WARN("通知客户端报单失败\n");
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    struct trader_msg_trader_struct_def oMsg;
    oMsg.type = TRADER_RECV_TRADER_ON_ORDER;
    oMsg.ErrorCd = 0;
    strcpy(oMsg.oOrder.UserOrderLocalID, pInputOrder->OrderRef);
    strcpy(oMsg.oOrder.InstrumentID, pInputOrder->InstrumentID);
    oMsg.oOrder.Direction = pInputOrder->Direction;
    oMsg.oOrder.OffsetFlag = pInputOrder->CombOffsetFlag[0];
    oMsg.oOrder.LimitPrice = pInputOrder->LimitPrice;
    oMsg.oOrder.HedgeFlag = pInputOrder->CombHedgeFlag[0];
    oMsg.oOrder.VolumeOriginal = pInputOrder->VolumeTotalOriginal;
    oMsg.oOrder.VolumeTraded = 0;
    oMsg.oOrder.OrderStatus = '6';
    strcpy(oMsg.oOrder.InsertTime, "-");
    
    cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));
    return ;
  }

  return;
  
}


void ctp_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;

  if(pRspInfo != NULL && 0 != pRspInfo->ErrorID){
    // 通知客户端撤单提交失败
    CMN_WARN("通知客户端撤单失败\n");
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }
}


void ctp_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;

  // 通知客户端成交
  struct trader_msg_trader_struct_def oMsg;
  oMsg.type = TRADER_RECV_TRADER_ON_TRADE;
  oMsg.ErrorCd = 0;
  strcpy(oMsg.oTrade.InstrumentID, pTrade->InstrumentID);
  strcpy(oMsg.oTrade.UserOrderLocalID, pTrade->OrderRef);
  oMsg.oTrade.Direction = pTrade->Direction;
  oMsg.oTrade.OffsetFlag = pTrade->OffsetFlag;
  oMsg.oTrade.TradePrice = pTrade->Price;
  oMsg.oTrade.TradeVolume = pTrade->Volume;
  strcpy(oMsg.oTrade.TradingDay, pTrade->TradingDay);
  strcpy(oMsg.oTrade.TradeTime, pTrade->TradeTime);
  strcpy(oMsg.oTrade.TradeID,  pTrade->TradeID);

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));
}


void ctp_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;

  struct trader_msg_trader_struct_def oMsg;
  oMsg.type = TRADER_RECV_TRADER_ON_ORDER;
  oMsg.ErrorCd = 0;
  strcpy(oMsg.oOrder.ExchangeID, pOrder->ExchangeID);
  strcpy(oMsg.oOrder.OrderSysID, pOrder->OrderSysID);
  strcpy(oMsg.oOrder.UserOrderLocalID, pOrder->OrderRef);
  strcpy(oMsg.oOrder.InstrumentID, pOrder->InstrumentID);
  oMsg.oOrder.Direction = pOrder->Direction;
  oMsg.oOrder.OffsetFlag = pOrder->CombOffsetFlag[0];
  oMsg.oOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  oMsg.oOrder.LimitPrice = pOrder->LimitPrice;
  oMsg.oOrder.VolumeOriginal = pOrder->VolumeTotalOriginal;
  oMsg.oOrder.VolumeTraded = pOrder->VolumeTraded;
  oMsg.oOrder.OrderStatus = pOrder->OrderStatus;
  strcpy(oMsg.oOrder.InsertTime, pOrder->InsertTime);
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));

  return ;
  
}


void ctp_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  
  // 通知客户端报单失败
  CMN_WARN("通知客户端报单失败\n");
  
}


void ctp_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  // 通知客户端撤单失败
  CMN_WARN("通知客户端撤单失败\n");

}


void ctp_trader_on_rsp_qry_user_investor(void* arg, CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;

  struct trader_msg_trader_struct_def oMsg;
  oMsg.type = TRADER_RECV_TRADER_ON_QRY_USER_INVESTOR;
  if(pRspInfo){
    oMsg.ErrorCd = -10000 - pRspInfo->ErrorID;
    strcpy(oMsg.ErrorMsg, pRspInfo->ErrorMsg);
  }else{
    oMsg.ErrorCd = 0;
    strcpy(oMsg.ErrorMsg, "");
    strcpy(self->InvestorID, pRspUserInvestor->InvestorID);
  }
  
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));
  
  return ;
}

void ctp_trader_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  char* inst = NULL;
  int errCd = 0;
  do{
  	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)	{
      errCd = -104;
      break;
    }
    if (pRspInstrument==NULL) {
      errCd = -104;
      break;
    }
    CMN_INFO("pRspInstrument->InstrumentID=[%s]\n", pRspInstrument->InstrumentID);
    CMN_INFO("pRspInstrument->ExchangeID=[%s]\n", pRspInstrument->ExchangeID);
    CMN_INFO("pRspInstrument->PriceTick=[%lf]\n", pRspInstrument->PriceTick);
    struct trader_msg_trader_struct_def oMsg;
    oMsg.type = TRADER_RECV_TRADER_ON_QRY_INSTRUMENT;
    oMsg.ErrorCd = -10000 - errCd;
    oMsg.bIsLast = bIsLast;
    if(!errCd){
      oMsg.ErrorCd = 0;
      strcpy(oMsg.InstrumentID, pRspInstrument->InstrumentID);
      strcpy(oMsg.ExchangeID, pRspInstrument->ExchangeID);
      oMsg.PriceTick = pRspInstrument->PriceTick;
    }
    cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));

    
  }while(0);
  
  self->bInit = 1;
}

void ctp_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  ctp_trader_api* self = (ctp_trader_api*)arg;
  ctp_trader_api_mng(self, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
  if(0 != pRspInfo->ErrorID){
    // 通知客户端登录失败
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }
  
  self->bConnected = 1;

}

void ctp_trader_on_rsp_qry_trading_account(void* arg, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return ;
}


void ctp_trader_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  ctp_trader_api* self = (ctp_trader_api*)arg;
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return;
  }
  if(pInvestorPosition){
    CMN_DEBUG(
      "pInvestorPosition->InstrumentID[%s]\n"
      "pInvestorPosition->BrokerID[%s]\n"
      "pInvestorPosition->InvestorID[%s]\n"
      "pInvestorPosition->PosiDirection[%c]\n"
      "pInvestorPosition->HedgeFlag[%c]\n"
      "pInvestorPosition->PositionDate[%c]\n"
      "pInvestorPosition->YdPosition[%d]\n"
      "pInvestorPosition->Position[%d]\n"
      "pInvestorPosition->LongFrozen[%d]\n"
      "pInvestorPosition->ShortFrozen[%d]\n"
      "pInvestorPosition->LongFrozenAmount[%lf]\n"
      "pInvestorPosition->ShortFrozenAmount[%lf]\n"
      "pInvestorPosition->OpenVolume[%d]\n"
      "pInvestorPosition->CloseVolume[%d]\n"
      "pInvestorPosition->OpenAmount[%lf]\n"
      "pInvestorPosition->CloseAmount[%lf]\n"
      "pInvestorPosition->PositionCost[%lf]\n"
      "pInvestorPosition->PreMargin[%lf]\n"
      "pInvestorPosition->UseMargin[%lf]\n"
      "pInvestorPosition->FrozenMargin[%lf]\n"
      "pInvestorPosition->FrozenCash[%lf]\n"
      "pInvestorPosition->FrozenCommission[%lf]\n"
      "pInvestorPosition->CashIn[%lf]\n"
      "pInvestorPosition->Commission[%lf]\n"
      "pInvestorPosition->CloseProfit[%lf]\n"
      "pInvestorPosition->PositionProfit[%lf]\n"
      "pInvestorPosition->PreSettlementPrice[%lf]\n"
      "pInvestorPosition->SettlementPrice[%lf]\n"
      "pInvestorPosition->TradingDay[%s]\n"
      "pInvestorPosition->SettlementID[%d]\n"
      "pInvestorPosition->OpenCost[%lf]\n"
      "pInvestorPosition->ExchangeMargin[%lf]\n"
      "pInvestorPosition->CombPosition[%d]\n"
      "pInvestorPosition->CombLongFrozen[%d]\n"
      "pInvestorPosition->CombShortFrozen[%d]\n"
      "pInvestorPosition->CloseProfitByDate[%lf]\n"
      "pInvestorPosition->CloseProfitByTrade[%lf]\n"
      "pInvestorPosition->TodayPosition[%d]\n"
      "pInvestorPosition->MarginRateByMoney[%lf]\n"
      "pInvestorPosition->MarginRateByVolume[%lf]\n"
      "pInvestorPosition->StrikeFrozen[%d]\n"
      "pInvestorPosition->StrikeFrozenAmount[%lf]\n"
      "pInvestorPosition->AbandonFrozen[%d]\n",
      pInvestorPosition->InstrumentID,
      pInvestorPosition->BrokerID,
      pInvestorPosition->InvestorID,
      pInvestorPosition->PosiDirection,
      pInvestorPosition->HedgeFlag,
      pInvestorPosition->PositionDate,
      pInvestorPosition->YdPosition,
      pInvestorPosition->Position,
      pInvestorPosition->LongFrozen,
      pInvestorPosition->ShortFrozen,
      pInvestorPosition->LongFrozenAmount,
      pInvestorPosition->ShortFrozenAmount,
      pInvestorPosition->OpenVolume,
      pInvestorPosition->CloseVolume,
      pInvestorPosition->OpenAmount,
      pInvestorPosition->CloseAmount,
      pInvestorPosition->PositionCost,
      pInvestorPosition->PreMargin,
      pInvestorPosition->UseMargin,
      pInvestorPosition->FrozenMargin,
      pInvestorPosition->FrozenCash,
      pInvestorPosition->FrozenCommission,
      pInvestorPosition->CashIn,
      pInvestorPosition->Commission,
      pInvestorPosition->CloseProfit,
      pInvestorPosition->PositionProfit,
      pInvestorPosition->PreSettlementPrice,
      pInvestorPosition->SettlementPrice,
      pInvestorPosition->TradingDay,
      pInvestorPosition->SettlementID,
      pInvestorPosition->OpenCost,
      pInvestorPosition->ExchangeMargin,
      pInvestorPosition->CombPosition,
      pInvestorPosition->CombLongFrozen,
      pInvestorPosition->CombShortFrozen,
      pInvestorPosition->CloseProfitByDate,
      pInvestorPosition->CloseProfitByTrade,
      pInvestorPosition->TodayPosition,
      pInvestorPosition->MarginRateByMoney,
      pInvestorPosition->MarginRateByVolume,
      pInvestorPosition->StrikeFrozen,
      pInvestorPosition->StrikeFrozenAmount,
      pInvestorPosition->AbandonFrozen
    );
    
    struct trader_msg_trader_struct_def oMsg;
    oMsg.type = TRADER_RECV_TRADER_ON_QRY_INVESTOR_POSITION;
    oMsg.ErrorCd = 0;
    oMsg.bIsLast = bIsLast;
    
    strcpy(oMsg.oPosition.InstrumentID, pInvestorPosition->InstrumentID);

    oMsg.oPosition.PositionDate = pInvestorPosition->PositionDate;
    oMsg.oPosition.PosiDirection = pInvestorPosition->PosiDirection;
    oMsg.oPosition.YdPosition = pInvestorPosition->YdPosition;
    oMsg.oPosition.TodayPosition = pInvestorPosition->TodayPosition;
    oMsg.oPosition.Position = pInvestorPosition->Position;
    oMsg.oPosition.LongFrozen = pInvestorPosition->LongFrozen + pInvestorPosition->ShortFrozen;
    
    cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));

  }
}

void ctp_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
  CMN_INFO("\n"
    "pInstrumentStatus->InstrumentID=[%s]\n"
    "pInstrumentStatus->InstrumentStatus=[%c]\n"
    "pInstrumentStatus->TradingSegmentSN=[%d]\n"
    "pInstrumentStatus->EnterTime=[%s]\n"
    "pInstrumentStatus->EnterReason=[%c]\n",
    pInstrumentStatus->InstrumentID,
    pInstrumentStatus->InstrumentStatus,
    pInstrumentStatus->TradingSegmentSN,
    pInstrumentStatus->EnterTime,
    pInstrumentStatus->EnterReason
  );
}



int ctp_trader_api_init(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");

  return 0;
}

int ctp_trader_api_start(ctp_trader_api* self, char* user_id, char* pwd)
{
  CMN_DEBUG("Enter!\n");
  strcpy(self->UserId, user_id);
  strcpy(self->Passwd, pwd);
  if(!self->bInit){
    CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    CCtpTraderHandler* pHandler = new CCtpTraderHandler(ctp_trader_api_cb_get(), self);
    self->pUserApi = (void*)pUserApi;
    self->pHandler = (void*)pHandler;

    pUserApi->RegisterFront(self->FrontAddress);
    pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
    pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);
    pUserApi->RegisterSpi(pHandler);
    pUserApi->Init();
    self->bInit = 1;
  }else{
    if(!self->bConnected){
      CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)self->pUserApi;
      CThostFtdcReqUserLoginField reqUserLogin;
      memset(&reqUserLogin,0,sizeof(CThostFtdcReqUserLoginField));
      strcpy(reqUserLogin.BrokerID, self->BrokenId);
      strcpy(reqUserLogin.UserID, self->UserId);
      strcpy(reqUserLogin.Password, self->Passwd);
      strcpy(reqUserLogin.UserProductInfo,"Trader Login");
      pUserApi->ReqUserLogin(&reqUserLogin, self->nLocalId++);
    }else{
      ctp_trader_api_mng(self, 0, "");
    }
  }

  return 0;
}

int ctp_trader_api_stop(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  
  return 0;
}


int ctp_trader_api_order_insert(ctp_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;
  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));	
  strcpy(req.BrokerID, self->BrokenId);  //应用单元代码	
  strcpy(req.InvestorID, self->InvestorID); //投资者代码	
  strcpy(req.InstrumentID, inst); //合约代码	
  strcpy(req.OrderRef, local_id);  //报单引用  
  strcpy(req.UserID, self->UserId); //投资者代码	
  
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
  req.Direction = buy_sell;  //买卖方向	
  req.CombOffsetFlag[0] = open_close; //组合开平标志:开仓
  req.CombHedgeFlag[0] = self->HedgeFlag;	  //组合投机套保标志	
  req.LimitPrice = price;	//价格
  req.VolumeTotalOriginal = vol;	///数量		
  req.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效

  req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
  req.MinVolume = 1;	//最小成交量:1	
  req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即

  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
  req.IsAutoSuspend = 0;  //自动挂起标志:否	
  req.UserForceClose = 0;   //用户强评标志:否

  pTrader->ReqOrderInsert(&req, self->nLocalId++);


  CMN_INFO("\n"
    "req.BrokerID=[%s]\n"
    "req.InvestorID=[%s]\n"
    "req.InstrumentID=[%s]\n"
    "req.OrderRef=[%s]\n"
    "req.UserID=[%s]\n"
    "req.OrderPriceType=[%c]\n"
    "req.Direction=[%c]\n"
    "req.CombOffsetFlag=[%c]\n"
    "req.CombHedgeFlag=[%c]\n"
    "req.LimitPrice=[%10.2lf]\n"
    "req.VolumeTotalOriginal=[%d]\n"
    "req.TimeCondition=[%c]\n"
    "req.VolumeCondition=[%c]\n"
    "req.MinVolume=[%d]\n"
    "req.ForceCloseReason=[%c]\n"
    "req.IsAutoSuspend=[%d]\n"
    "req.UserForceClose=[%d]\n",
    req.BrokerID,
    req.InvestorID,
    req.InstrumentID,
    req.OrderRef,
    req.UserID,
    req.OrderPriceType,
    req.Direction,
    req.CombOffsetFlag[0],
    req.CombHedgeFlag[0],
    req.LimitPrice,
    req.VolumeTotalOriginal,
    req.TimeCondition,
    req.VolumeCondition,
    req.MinVolume,
    req.ForceCloseReason,
    req.IsAutoSuspend,
    req.UserForceClose);
  
  return 0;

}


int ctp_trader_api_order_action(ctp_trader_api* self, char* inst, char* local_id, char* org_local_id,
  char* exchange_id, char* order_sys_id)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;

  CThostFtdcInputOrderActionField OrderAction;
  memset(&OrderAction,0,sizeof(CThostFtdcInputOrderActionField));

  strcpy(OrderAction.BrokerID,  self->BrokenId);   //经纪公司代码	
  strcpy(OrderAction.InvestorID, self->InvestorID); //投资者代码
  OrderAction.OrderActionRef = atoi(local_id);
  //strcpy(req.OrderRef, orderRef); //报单引用	
  //req.FrontID = frontId;           //前置编号	
  //req.SessionID = sessionId;       //会话编号
  strcpy(OrderAction.ExchangeID, exchange_id);
  strcpy(OrderAction.OrderSysID, order_sys_id);
  OrderAction.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志 
  
  pTrader->ReqOrderAction(&OrderAction, self->nLocalId++);

  
  CMN_INFO("\n"
    "OrderAction.OrderRef=[%s]\n"
    "OrderAction.BrokerID=[%s]\n"
    "OrderAction.InvestorID=[%s]\n"
    "OrderAction.OrderActionRef=[%d]\n"
    "OrderAction.ExchangeID=[%s]\n"
    "OrderAction.OrderSysID=[%s]\n"
    "OrderAction.ActionFlag=[%c]\n", 
    org_local_id,
    OrderAction.BrokerID,
    OrderAction.InvestorID,
    OrderAction.OrderActionRef,
    OrderAction.ExchangeID,
    OrderAction.OrderSysID,
    OrderAction.ActionFlag);

  
  return 0;

}

int ctp_trader_api_query_instrument(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;
  // 查询合约列表
  CThostFtdcQryInstrumentField QryInstrument;
  memset(&QryInstrument,0,sizeof(CThostFtdcQryInstrumentField));
  //strcpy(QryInstrument.ExchangeID,"SHFE");
  pTrader->ReqQryInstrument(&QryInstrument,self->nLocalId++);

  return 0;
}

int ctp_trader_api_query_user_investor(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;

  // 查询投资者编号
  CThostFtdcQryInvestorField QryUserInvestor;
  memset(&QryUserInvestor, 0, sizeof(QryUserInvestor));
  pTrader->ReqQryInvestor(&QryUserInvestor, self->nLocalId++);

  return 0;
}

int ctp_trader_api_get_tradingday(ctp_trader_api* self, char* tradingday)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)self->pUserApi;
  strcpy(tradingday, pUserApi->GetTradingDay());

  return 0;
}


int ctp_trader_api_get_maxuserlocalid(ctp_trader_api* self, char* local_id)
{
  CMN_DEBUG("Enter!\n");
  strcpy(local_id, self->MaxOrderLocalID);

  return 0;
}


int ctp_trader_api_mng(ctp_trader_api* self, int error_cd, char* error_msg)
{
  CMN_DEBUG("Enter!\n");
  struct trader_msg_trader_struct_def oMsg;
  oMsg.type = TRADER_RECV_TRADER_ON_LOGIN;
  if(error_cd){
    oMsg.ErrorCd = -10000 - error_cd;
    strcpy(oMsg.ErrorMsg, error_msg);
  }else{
    oMsg.ErrorCd = 0;
    strcpy(oMsg.ErrorMsg, "");
  }
  
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));

  return 0;
}

int ctp_trader_api_qry_instrument(ctp_trader_api* self, char* instrument, int last, int err_cd)
{
  CMN_DEBUG("Enter!\n");
  struct trader_msg_trader_struct_def oMsg;
  oMsg.type = TRADER_RECV_TRADER_ON_QRY_INSTRUMENT;
  oMsg.ErrorCd = -10000 - err_cd;
  oMsg.bIsLast = last;
  if(!err_cd){
    oMsg.ErrorCd = 0;
    strcpy(oMsg.InstrumentID, instrument);
  }
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oMsg, sizeof(oMsg));

  return 0;

}

int ctp_trader_api_qry_settlement_info_confirm(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;
  
  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(CThostFtdcSettlementInfoConfirmField));

  strcpy(req.BrokerID, self->BrokenId);
  strcpy(req.InvestorID, self->UserId);
  strcpy(req.ConfirmDate, pTrader->GetTradingDay());
  strcpy(req.ConfirmTime, "09:00:00");
  
  pTrader->ReqSettlementInfoConfirm(&req, self->nLocalId++);
  
  return 0;
}

int ctp_trader_api_qry_investor_position(ctp_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pTrader = (CThostFtdcTraderApi*)self->pUserApi;

  CThostFtdcQryInvestorPositionField req;
  memset(&req, 0, sizeof(CThostFtdcQryInvestorPositionField));

  strcpy(req.BrokerID, self->BrokenId);
  strcpy(req.InvestorID, self->UserId);

  pTrader->ReqQryInvestorPosition(&req, self->nLocalId++);
  return 0;
}


ctp_trader_api* ctp_trader_api_new(evutil_socket_t fd)
{
  ctp_trader_api* self = (ctp_trader_api*)malloc(sizeof(ctp_trader_api));
  self->fd = fd;

  self->pUserApi = (void*)NULL;
  self->pHandler = (void*)NULL;

  self->bConnected = 0;
  self->bInit = 0;

  self->nLocalId = 0;
  
  self->pMethod = ctp_trader_api_method_get();

  return self;

}

void ctp_trader_api_free(ctp_trader_api* self)
{
  if(self->bInit){
    CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)self->pUserApi ;
    CCtpTraderHandler* pHandler = (CCtpTraderHandler*)self->pHandler;

    pUserApi->Release(); 
    delete pHandler;
    self->bInit = 0;
  }

  if(self){
    free(self);
  }
  
}



