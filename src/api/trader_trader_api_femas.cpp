
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcTraderApi.h"

#include "FemasTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_femas.h"

static void trader_trader_api_femas_start(trader_trader_api* self);
static void trader_trader_api_femas_stop(trader_trader_api* self);
static void trader_trader_api_femas_login(trader_trader_api* self);
static void trader_trader_api_femas_logout(trader_trader_api* self);
  
static int trader_trader_api_femas_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_femas_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id);
 
static int trader_trader_api_femas_qry_instrument(trader_trader_api* self);
static int trader_trader_api_femas_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_femas_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_femas_qry_trading_account(trader_trader_api* self);


static void femas_trader_on_front_connected(void* arg);
static void femas_trader_on_front_disconnected(void* arg, int nReason);
static void femas_trader_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_trader_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_trader_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);

static void femas_trader_on_rsp_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_trader_on_rsp_order_action(void* arg, CUstpFtdcOrderActionField *pInputOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_trader_on_rtn_order(void* arg, CUstpFtdcOrderField *pOrder);
static void femas_trader_on_rtn_trade(void* arg, CUstpFtdcTradeField *pTrade);
static void femas_trader_on_err_rtn_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);
static void femas_trader_on_err_rtn_order_action(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);
static void femas_query_on_rsp_qry_investor(void* arg, CUstpFtdcRspUserInvestorField *pInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_query_on_rsp_qry_instrument(void* arg, CUstpFtdcRspInstrumentField *pInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_query_on_rsp_qry_trading_account(void* arg, CUstpFtdcRspInvestorAccountField *pTradingAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_query_on_rsp_qry_investor_position(void* arg, CUstpFtdcRspInvestorPositionField *pInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_trader_on_rtn_instrument_status(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus);
static femas_trader_api_cb* femas_trader_api_cb_get();

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_femas_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_param,
    trader_trader_api_get_trading_day,
    trader_trader_api_get_max_order_local_id,
    trader_trader_api_femas_start,
    trader_trader_api_femas_stop,
    trader_trader_api_femas_login,
    trader_trader_api_femas_logout,
    trader_trader_api_femas_order_insert,
    trader_trader_api_femas_order_action,
    trader_trader_api_femas_qry_instrument,
    trader_trader_api_femas_qry_user_investor,
    trader_trader_api_femas_qry_investor_position,
    trader_trader_api_femas_qry_trading_account
  };

  return &trader_trader_api_method_st;
}


void trader_trader_api_femas_start(trader_trader_api* self)
{
  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi(self->pWorkspace);
  CFemasTraderHandler* pTraderHandler = new CFemasTraderHandler(femas_trader_api_cb_get(), (void*)self);
  
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)malloc(sizeof(trader_trader_api_femas));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;

  // 交易
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(self->pAddress);

  
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);

  pTraderApi->SubscribePublicTopic(USTP_TERT_RESUME);
  
  // 连接交易服务器
  pTraderApi->Init();

  return;

}

void trader_trader_api_femas_stop(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  CFemasTraderHandler* pTraderHandler = (CFemasTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_femas_login(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  CUstpFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, self->pBrokerID);
  strcpy(reqUserLoginField.UserID, self->pUser);
  strcpy(reqUserLoginField.Password, self->pPwd);
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, pImp->nTraderRequestID++);
}

void trader_trader_api_femas_logout(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_femas_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///经纪公司代码
	strcpy(inputOrderField.BrokerID, self->pBrokerID);
	///交易所代码
	strcpy(inputOrderField.ExchangeID, "CFFEX");
	///投资者代码
	strcpy(inputOrderField.InvestorID, pImp->sInvestorID);
	///合约代码
	strcpy(inputOrderField.InstrumentID, inst);
	///报单引用
	strcpy(inputOrderField.UserOrderLocalID, local_id);
	///用户代码
	strcpy(inputOrderField.UserID, self->pUser);
	///报单价格条件
	inputOrderField.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	///买卖方向
	inputOrderField.Direction = buy_sell;
	///组合开平标志
	inputOrderField.OffsetFlag = open_close;
	///组合投机套保标志
	inputOrderField.HedgeFlag = USTP_FTDC_CHF_Speculation;
	///价格
	inputOrderField.LimitPrice = price;
	///数量
	inputOrderField.Volume= vol;
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

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  
}

int trader_trader_api_femas_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

	///经纪公司代码
	strcpy(inputOrderActionField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(inputOrderActionField.InvestorID, pImp->sInvestorID);
	///报单操作引用
	strcpy(inputOrderActionField.UserOrderActionLocalID, local_id);
	///报单引用
	strcpy(inputOrderActionField.UserOrderLocalID, org_local_id);
	///前置编号
	//inputOrderActionField.FrontID = front_id;
	///会话编号
	//inputOrderActionField.SessionID = session_id;
	///交易所代码
	strcpy(inputOrderActionField.ExchangeID, "CFFEX");
	///操作标志
	inputOrderActionField.ActionFlag = USTP_FTDC_AF_Delete;
	///用户代码
	strcpy(inputOrderActionField.UserID, self->pUser);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);

}

 
int trader_trader_api_femas_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///合约代码
	//TUstpFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	//TUstpFtdcExchangeIDType	ExchangeID;
	strcpy(qryInstrumentField.ExchangeID, "CFFEX");

  pTraderApi->ReqQryInstrument(&qryInstrumentField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryUserInvestorField qryInvestorField;

  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
	///经纪公司代码
	strcpy(qryInvestorField.BrokerID, self->pBrokerID);
	///用户代码
	strcpy(qryInvestorField.UserID, self->pUser);  
  
  pTraderApi->ReqQryUserInvestor(&qryInvestorField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryInvestorPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

	///经纪公司代码
	strcpy(qryInvestorPositionField.BrokerID, self->pBrokerID);
	///用户代码
	strcpy(qryInvestorPositionField.UserID, self->pUser);  
	///投资者代码
	strcpy(qryInvestorPositionField.InvestorID, pImp->sInvestorID);
	///合约代码
	//TUstpFtdcInstrumentIDType	InstrumentID;

  pTraderApi->ReqQryInvestorPosition(&qryInvestorPositionField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  
  CUstpFtdcQryInvestorAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///经纪公司代码
	strcpy(qryTradingAccountField.BrokerID, self->pBrokerID);
	///用户代码
	strcpy(qryTradingAccountField.UserID, self->pUser);  
	///投资者代码
	strcpy(qryTradingAccountField.InvestorID, pImp->sInvestorID);

  pTraderApi->ReqQryInvestorAccount(&qryTradingAccountField, pImp->nTraderRequestID++);

}

void femas_trader_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_connected(self);

}

void femas_trader_on_front_disconnected(void* arg, int nReason)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_disconnected(self, nReason);
}

void femas_trader_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
    
  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
}

void femas_trader_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_user_logout(self, errNo, errMsg);
}

void femas_trader_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);
}


void femas_trader_on_rsp_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_order_insert(self, errNo, errMsg);
  
}

void femas_trader_on_rsp_order_action(void* arg, CUstpFtdcOrderActionField *pInputOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_order_action(self, errNo, errMsg);
}

void femas_trader_on_rtn_order(void* arg, CUstpFtdcOrderField *pOrder)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  
  // 合约代码
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pOrder->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pOrder->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pOrder->Direction;
  // 开平
  traderOrder.OffsetFlag = pOrder->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pOrder->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pOrder->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pOrder->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pOrder->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
  
}

void femas_trader_on_rtn_trade(void* arg, CUstpFtdcTradeField *pTrade)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strcpy(traderTrade.InstrumentID, pTrade->InstrumentID);
  ///本地报单编号
  strcpy(traderTrade.UserOrderLocalID, pTrade->UserOrderLocalID);
  ///交易日
  strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///成交时间
  strcpy(traderTrade.TradeTime, pTrade->TradeTime);
  ///成交价格
  traderTrade.TradePrice = pTrade->TradePrice;
  ///成交数量
  traderTrade.TradeVolume = pTrade->TradeVolume;
  //成交编号
  strcpy(traderTrade.TradeID, pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void femas_trader_on_err_rtn_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_err_rtn_order_insert(self, errNo, errMsg);
}

void femas_trader_on_err_rtn_order_action(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_err_rtn_order_action(self, errNo, errMsg);
}

femas_trader_api_cb* femas_trader_api_cb_get()
{
  static femas_trader_api_cb femas_trader_api_cb_st = {
    femas_trader_on_front_connected,
    femas_trader_on_front_disconnected,
    femas_trader_on_rsp_error,
    femas_trader_on_rsp_user_login,
    femas_trader_on_rsp_user_logout,
    femas_trader_on_rsp_order_insert,
    femas_trader_on_rsp_order_action,
    femas_trader_on_rtn_trade,
    femas_trader_on_rtn_order,
    femas_trader_on_err_rtn_order_insert,
    femas_trader_on_err_rtn_order_action,
    femas_query_on_rsp_qry_investor,
    femas_query_on_rsp_qry_instrument,
    femas_trader_on_rtn_instrument_status,
    femas_query_on_rsp_qry_trading_account,
    femas_query_on_rsp_qry_investor_position
  };

  return &femas_trader_api_cb_st;
}


void femas_query_on_rsp_qry_investor(void* arg, CUstpFtdcRspUserInvestorField *pInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas* pImp = (trader_trader_api_femas*)self->pUserApi;
  int errNo = 0;
  char* errMsg = NULL;
  char* investor = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestor) {
    investor = pInvestor->InvestorID;
    strncpy(pImp->sInvestorID, pInvestor->InvestorID, sizeof(pImp->sInvestorID));
  }
  
  trader_trader_api_on_rsp_qry_user_investor(self, investor, errNo, errMsg);
}

void femas_query_on_rsp_qry_instrument(void* arg, CUstpFtdcRspInstrumentField *pInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInstrument) {
    strcpy(traderInstrument.InstrumentID, pInstrument->InstrumentID);
    //TODO
    traderInstrument.PriceTick = pInstrument->PriceTick;
    traderInstrument.VolumeMultiple = pInstrument->VolumeMultiple;
    
  }

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);

  
}

void femas_query_on_rsp_qry_trading_account(void* arg, CUstpFtdcRspInvestorAccountField *pTradingAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pTradingAccount) {
    strcpy(traderAccount.AccountID, pTradingAccount->AccountID);
    //TODO
  }

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, errNo, errMsg, bIsLast);
}

void femas_query_on_rsp_qry_investor_position(void* arg, CUstpFtdcRspInvestorPositionField *pInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestorPosition) {
    strcpy(traderPosition.InstrumentID, pInvestorPosition->InstrumentID);
    //TODO
  }

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);
}

void femas_trader_on_rtn_instrument_status(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus)
{
  //TODO
/*
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
  */
}


