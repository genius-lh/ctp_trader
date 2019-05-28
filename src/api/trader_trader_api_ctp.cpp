
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"

#include "CtpTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_ctp.h"
#include "cmn_log.h"

static int trader_trader_api_ctp_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_ctp_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_ctp_start(trader_trader_api* self);
static void trader_trader_api_ctp_stop(trader_trader_api* self);
static void trader_trader_api_ctp_login(trader_trader_api* self);
static void trader_trader_api_ctp_logout(trader_trader_api* self);
  
static int trader_trader_api_ctp_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_ctp_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_ctp_qry_instrument(trader_trader_api* self);
static int trader_trader_api_ctp_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_ctp_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_ctp_qry_trading_account(trader_trader_api* self);


static void ctp_trader_on_front_connected(void* arg);
static void ctp_trader_on_front_disconnected(void* arg, int nReason);
static void ctp_trader_on_rsp_authenticate(void* arg, CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

static void ctp_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder);
static void ctp_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade);
static void ctp_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
static void ctp_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
static void ctp_query_on_rsp_qry_investor(void* arg, CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_query_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_query_on_rsp_qry_trading_account(void* arg, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_query_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus);
static ctp_trader_api_cb* ctp_trader_api_cb_get();

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_ctp_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_ctp_get_trading_day,
    trader_trader_api_ctp_get_max_order_local_id,
    trader_trader_api_ctp_start,
    trader_trader_api_ctp_stop,
    trader_trader_api_ctp_login,
    trader_trader_api_ctp_logout,
    trader_trader_api_ctp_order_insert,
    trader_trader_api_ctp_order_action,
    trader_trader_api_ctp_qry_instrument,
    trader_trader_api_ctp_qry_user_investor,
    trader_trader_api_ctp_qry_investor_position,
    trader_trader_api_ctp_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_ctp_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  strcpy(tradingday, pTraderApi->GetTradingDay());
  return 0;
}

int trader_trader_api_ctp_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  *local_id = self->userLocalId;
  return 0;
}


void trader_trader_api_ctp_start(trader_trader_api* self)
{
  CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(self->pWorkspace);
  CCtpTraderHandler* pTraderHandler = new CCtpTraderHandler(ctp_trader_api_cb_get(), (void*)self);
  
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)malloc(sizeof(trader_trader_api_ctp));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;

  // 交易
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(self->pAddress);

  self->timeCondition = THOST_FTDC_TC_GFD;
  self->hedgeFlag = THOST_FTDC_HF_Speculation;
  
  // 连接交易服务器
  pTraderApi->Init();

  return;

}

void trader_trader_api_ctp_stop(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;
  CCtpTraderHandler* pTraderHandler = (CCtpTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_ctp_login(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;
  CThostFtdcReqAuthenticateField reqAuthenticateField;
  memset(&reqAuthenticateField, 0, sizeof(reqAuthenticateField));
  strcpy(reqAuthenticateField.BrokerID, self->pBrokerID);
  strcpy(reqAuthenticateField.UserID, self->pUser);
  strcpy(reqAuthenticateField.AppID, self->pAppID);
  strcpy(reqAuthenticateField.AuthCode, self->pAuthCode);
  pTraderApi->ReqAuthenticate(&reqAuthenticateField, pImp->nTraderRequestID++);
  
}

void trader_trader_api_ctp_logout(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_ctp_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///经纪公司代码
	strcpy(inputOrderField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(inputOrderField.InvestorID, self->pUser);
	///合约代码
	strcpy(inputOrderField.InstrumentID, inst);
	///报单引用
	strcpy(inputOrderField.OrderRef, local_id);
	///用户代码
	strcpy(inputOrderField.UserID, self->pUser);
	///报单价格条件
	inputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向
	inputOrderField.Direction = buy_sell;
	///组合开平标志
	inputOrderField.CombOffsetFlag[0] = open_close;
	///组合投机套保标志
	inputOrderField.CombHedgeFlag[0] = self->hedgeFlag;
	///价格
	inputOrderField.LimitPrice = price;
	///数量
	inputOrderField.VolumeTotalOriginal = vol;
	///有效期类型
	inputOrderField.TimeCondition = self->timeCondition;
	///成交量类型
	inputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量
	inputOrderField.MinVolume = 1;
	///触发条件
	inputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
	///强平原因
	inputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志
	inputOrderField.IsAutoSuspend = 0;
	///用户强评标志
	inputOrderField.UserForceClose = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  
}

int trader_trader_api_ctp_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcInputOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));
  
  //strcpy(req.OrderRef, orderRef); //报单引用	
  //req.FrontID = frontId;           //前置编号	
  //req.SessionID = sessionId;       //会话编号

	///经纪公司代码
	strcpy(inputOrderActionField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(inputOrderActionField.InvestorID, self->pUser);
	///报单操作引用
	inputOrderActionField.OrderActionRef = atoi(local_id);
	///报单引用
  strcpy(inputOrderActionField.OrderSysID, order_sys_id);
	///前置编号
	//inputOrderActionField.FrontID = front_id;
	///会话编号
	//inputOrderActionField.SessionID = session_id;
	///交易所代码
	strcpy(inputOrderActionField.ExchangeID, exchange_id);
	///操作标志
	inputOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;
	///用户代码
	strcpy(inputOrderActionField.UserID, self->pUser);
	///合约代码
	strcpy(inputOrderActionField.InstrumentID, inst);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);

}

 
int trader_trader_api_ctp_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///合约代码
	//TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	//TThostFtdcExchangeIDType	ExchangeID;
	//strcpy(qryInstrumentField.ExchangeID, "SSE");

  pTraderApi->ReqQryInstrument(&qryInstrumentField, pImp->nTraderRequestID++);

}

int trader_trader_api_ctp_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcQryInvestorField qryInvestorField;

  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
	///经纪公司代码
	strcpy(qryInvestorField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryInvestorField.InvestorID, self->pUser);  
  
  pTraderApi->ReqQryInvestor(&qryInvestorField, pImp->nTraderRequestID++);

}

int trader_trader_api_ctp_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;

  CThostFtdcQryInvestorPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

	///经纪公司代码
	strcpy(qryInvestorPositionField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryInvestorPositionField.InvestorID, self->pUser);  
	///合约代码
	//TThostFtdcInstrumentIDType	InstrumentID;

  pTraderApi->ReqQryInvestorPosition(&qryInvestorPositionField, pImp->nTraderRequestID++);

}

int trader_trader_api_ctp_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;
  
  CThostFtdcQryTradingAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///经纪公司代码
	strcpy(qryTradingAccountField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryTradingAccountField.InvestorID, self->pUser);  

  pTraderApi->ReqQryTradingAccount(&qryTradingAccountField, pImp->nTraderRequestID++);

}

void ctp_trader_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_connected(self);

}

void ctp_trader_on_front_disconnected(void* arg, int nReason)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_disconnected(self, nReason);
}

void ctp_trader_on_rsp_authenticate(void* arg, CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(errNo){
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
    return ;
  }

  CThostFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, self->pBrokerID);
  strcpy(reqUserLoginField.UserID, self->pUser);
  strcpy(reqUserLoginField.Password, self->pPwd);

  pTraderApi->ReqUserLogin(&reqUserLoginField, pImp->nTraderRequestID++);


}

void ctp_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_ctp* pImp = (trader_trader_api_ctp*)self->pUserApi;
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(errNo){
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
    return ;
  }
  
  if(pRspUserLogin){
    // 获取最大报单号
    trader_trader_api_get_max_order_local_id(self, pRspUserLogin->MaxOrderRef);
  }

  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
    
  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(CThostFtdcSettlementInfoConfirmField));

  strcpy(req.BrokerID, self->pBrokerID);
  strcpy(req.InvestorID, self->pUser);
  strcpy(req.ConfirmDate, pTraderApi->GetTradingDay());
  strcpy(req.ConfirmTime, "09:00:00");
  
  pTraderApi->ReqSettlementInfoConfirm(&req, pImp->nTraderRequestID++);
  
}

void ctp_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

void ctp_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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


void ctp_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

void ctp_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

void ctp_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  
  strcpy(traderOrder.ExchangeID, pOrder->ExchangeID);
  strcpy(traderOrder.OrderSysID, pOrder->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pOrder->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pOrder->OrderRef);
  // 买卖
  traderOrder.Direction = pOrder->Direction;
  // 开平
  traderOrder.OffsetFlag = pOrder->CombOffsetFlag[0];
  ///投机套保标志
  traderOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  // 报单价格
  traderOrder.LimitPrice = pOrder->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->VolumeTotalOriginal;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pOrder->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pOrder->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
  
}

void ctp_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strcpy(traderTrade.InstrumentID, pTrade->InstrumentID);
  ///本地报单编号
  strcpy(traderTrade.UserOrderLocalID, pTrade->OrderRef);
  ///交易日
  strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///成交时间
  strcpy(traderTrade.TradeTime, pTrade->TradeTime);
  ///买卖方向
  traderTrade.Direction = pTrade->Direction;
  ///开平标志
  traderTrade.OffsetFlag = pTrade->OffsetFlag;
  ///成交价格
  traderTrade.TradePrice = pTrade->Price;
  ///成交数量
  traderTrade.TradeVolume = pTrade->Volume;
  //成交编号
  strcpy(traderTrade.TradeID, pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void ctp_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
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

void ctp_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
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


ctp_trader_api_cb* ctp_trader_api_cb_get()
{
  static ctp_trader_api_cb ctp_trader_api_cb_st = {
    ctp_trader_on_front_connected,
    ctp_trader_on_front_disconnected,
    ctp_trader_on_rsp_authenticate,
    ctp_trader_on_rsp_user_login,
    ctp_trader_on_rsp_user_logout,
    ctp_trader_on_rsp_error,
    ctp_trader_on_rsp_order_insert,
    ctp_trader_on_rsp_order_action,
    ctp_trader_on_rtn_order,
    ctp_trader_on_rtn_trade,
    ctp_trader_on_err_rtn_order_insert,
    ctp_trader_on_err_rtn_order_action,
    ctp_query_on_rsp_qry_investor,
    ctp_query_on_rsp_qry_instrument,
    ctp_trader_on_rsp_settlement_info_confirm,
    ctp_query_on_rsp_qry_trading_account,
    ctp_query_on_rsp_qry_investor_position,
    ctp_trader_on_rtn_instrument_status
  };

  return &ctp_trader_api_cb_st;
}

void ctp_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;

}


void ctp_query_on_rsp_qry_investor(void* arg, CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  char* investor = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestor) {
    investor = pInvestor->InvestorID;
  }
  
  trader_trader_api_on_rsp_qry_user_investor(self, investor, errNo, errMsg);
}

void ctp_query_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
    strcpy(traderInstrument.ExchangeID, pInstrument->ExchangeID);
    traderInstrument.PriceTick = pInstrument->PriceTick;
  }

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);

  
}

void ctp_query_on_rsp_qry_trading_account(void* arg, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

void ctp_query_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
    
    strcpy(traderPosition.InstrumentID, pInvestorPosition->InstrumentID);
    traderPosition.PositionDate = pInvestorPosition->PositionDate;
    traderPosition.PosiDirection = pInvestorPosition->PosiDirection;
    traderPosition.YdPosition = pInvestorPosition->YdPosition;
    traderPosition.TodayPosition = pInvestorPosition->TodayPosition;
    traderPosition.Position = pInvestorPosition->Position;
    traderPosition.LongFrozen = pInvestorPosition->LongFrozen + pInvestorPosition->ShortFrozen;

  }

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);
}

void ctp_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus)
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


