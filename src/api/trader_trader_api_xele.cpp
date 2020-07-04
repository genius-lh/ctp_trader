
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CXeleTraderApi.hpp"
#include "XeleTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_xele.h"
#include "cmn_log.h"

static int trader_trader_api_xele_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_xele_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_xele_start(trader_trader_api* self);
static void trader_trader_api_xele_stop(trader_trader_api* self);
static void trader_trader_api_xele_login(trader_trader_api* self);
static void trader_trader_api_xele_logout(trader_trader_api* self);
  
static int trader_trader_api_xele_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_xele_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_xele_qry_instrument(trader_trader_api* self);
static int trader_trader_api_xele_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_xele_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_xele_qry_trading_account(trader_trader_api* self);

static void trader_trader_api_xele_set_param(trader_trader_api* self, char* key, char* val);


#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_xele_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_xele_set_param,
    trader_trader_api_xele_get_trading_day,
    trader_trader_api_xele_get_max_order_local_id,
    trader_trader_api_xele_start,
    trader_trader_api_xele_stop,
    trader_trader_api_xele_login,
    trader_trader_api_xele_logout,
    trader_trader_api_xele_order_insert,
    trader_trader_api_xele_order_action,
    trader_trader_api_xele_qry_instrument,
    trader_trader_api_xele_qry_user_investor,
    trader_trader_api_xele_qry_investor_position,
    trader_trader_api_xele_qry_trading_account
  };

  return &trader_trader_api_method_st;

}

int trader_trader_api_xele_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;

  strcpy(tradingday, pImp->sTradingDay);
  return 0;
}

int trader_trader_api_xele_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  *local_id = self->userLocalId;
  return 0;
}


void trader_trader_api_xele_start(trader_trader_api* self)
{
  CXeleTraderApi* pTraderApi = CXeleTraderApi::CreateTraderApi();
  CXeleTraderHandler* pTraderHandler = new CXeleTraderHandler((void*)self);
  
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)malloc(sizeof(trader_trader_api_xele));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;
  
  self->timeCondition = XELE_FTDC_TC_GFD;
  self->hedgeFlag = XELE_FTDC_HF_Speculation;
  
	pTraderApi->RegisterSpi(pTraderHandler);
  //TODO
  pTraderApi->RegisterFront(self->pAddress, self->pAddress);
  /*	 * 准备login的结构体	 */
  /*	 * 订阅相应的流	 */
  pTraderApi->SubscribePrivateTopic(XELE_TERT_RESUME);
  pTraderApi->SubscribePublicTopic(XELE_TERT_RESUME);
  /*	 * 开始登录, 使客户端程序开始与交易柜台建立连接	 */
  pTraderApi->Init();
  
  return;

}

void trader_trader_api_xele_stop(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;
  CXeleTraderHandler* pTraderHandler = (CXeleTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_xele_login(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;
  
	CXeleFtdcReqUserLoginField loginInfo;
  CXeleFtdcAuthenticationInfoField authInfo;

	memset(&loginInfo, 0, sizeof(loginInfo));
  snprintf(loginInfo.AccountID, sizeof(loginInfo.AccountID), "%s", self->pUser);
  snprintf(loginInfo.Password, sizeof(loginInfo.Password), "%s", self->pPwd);
  memset(&authInfo, 0, sizeof(authInfo));
  snprintf(authInfo.AuthCode, sizeof(authInfo.AuthCode), "%s", self->pAuthCode);
  snprintf(authInfo.AppID, sizeof(authInfo.AppID), "%s", self->pAppID);
  
	pTraderApi->RegisterAuthentication(&authInfo);
  pTraderApi->ReqUserLogin(&loginInfo, pImp->nTraderRequestID++);
}

void trader_trader_api_xele_logout(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;

  CXeleFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.AccountID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_xele_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;

  CXeleFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
		/* 会员代码 */
	snprintf(inputOrderField.ParticipantID, sizeof(inputOrderField.ParticipantID), "%s", self->pBrokerID);
	/* 客户代码 */
	snprintf(inputOrderField.ClientID, sizeof(inputOrderField.ClientID), "%s", pImp->sClientID);
	/* 合约代码 */
	strncpy(inputOrderField.InstrumentID, inst, sizeof(inputOrderField.InstrumentID));
	/* 报单价格条件 */
	inputOrderField.OrderPriceType = XELE_FTDC_OPT_LimitPrice;
	/* 买卖方向 */
	inputOrderField.Direction = buy_sell;
	/* 组合开平标志 */
	inputOrderField.CombOffsetFlag[0] = open_close;
	/* 组合投机套保标志 */
	inputOrderField.CombHedgeFlag[0] = self->hedgeFlag;
	/* 价格 */
	inputOrderField.LimitPrice = price;
	/* 数量 */
	inputOrderField.VolumeTotalOriginal = vol;
	/* 有效期类型 */
	inputOrderField.TimeCondition = self->timeCondition;
	/* 成交量类型 */
	inputOrderField.VolumeCondition = XELE_FTDC_VC_AV;
	/* 最小成交量 */
	inputOrderField.MinVolume = 0;
	/* 触发条件 */
	inputOrderField.ContingentCondition = XELE_FTDC_CC_Immediately;
	/* 止损价 */
	inputOrderField.StopPrice = 0;
	/* 强平原因 */
	inputOrderField.ForceCloseReason = XELE_FTDC_FCC_NotForceClose;
  /* 本地报单编号 */
	snprintf(inputOrderField.OrderLocalID, sizeof(inputOrderField.OrderLocalID), "%s", local_id);
  /* 自动挂起标志 */
	inputOrderField.IsAutoSuspend = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  return 0;
}

int trader_trader_api_xele_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;

  CXeleFtdcOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

  ///报单编号
	snprintf(inputOrderActionField.OrderSysID, sizeof(inputOrderActionField.OrderSysID), "%s", order_sys_id);
  ///报单操作标志
  inputOrderActionField.ActionFlag = XELE_FTDC_AF_Delete;
  ///会员代码
	snprintf(inputOrderActionField.ParticipantID, sizeof(inputOrderActionField.ParticipantID), "%s", self->pBrokerID);
  ///客户代码
	snprintf(inputOrderActionField.ClientID, sizeof(inputOrderActionField.ClientID), "%s", pImp->sClientID);
  ///操作本地编号
	snprintf(inputOrderActionField.ActionLocalID, sizeof(inputOrderActionField.ActionLocalID), "%s", local_id);
	
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);
  return 0;
}

 
int trader_trader_api_xele_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;

  CXeleFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///合约代码
	//TUstpFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	//TUstpFtdcExchangeIDType	ExchangeID;
	//strcpy(qryInstrumentField.ExchangeID, "");

  pTraderApi->ReqQryInstrument(&qryInstrumentField, pImp->nTraderRequestID++);
  return 0;

}

int trader_trader_api_xele_qry_user_investor(trader_trader_api* self)
{
  // empty
  return 0;
}

int trader_trader_api_xele_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;

  CXeleFtdcQryClientPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));
	///投资者代码
  snprintf(qryInvestorPositionField.AccountID, sizeof(qryInvestorPositionField.AccountID), "%s", self->pUser);

  pTraderApi->ReqQryClientPosition(&qryInvestorPositionField, pImp->nTraderRequestID++);
  return 0;
}

int trader_trader_api_xele_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pTraderApi;
  
  CXeleFtdcQryClientAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///投资者代码
  snprintf(qryTradingAccountField.AccountID, sizeof(qryTradingAccountField.AccountID), "%s", self->pUser);

  pTraderApi->ReqQryClientAccount(&qryTradingAccountField, pImp->nTraderRequestID++);
  return 0;
}

void trader_trader_api_xele_set_param(trader_trader_api* self, char* key, char* val)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  if(!strcmp("XELE_CLIENTID", key)){
    strncpy(pImp->sClientID, val, sizeof(pImp->sClientID));
    return ;
  }

  trader_trader_api_set_param(self, key, val);
}


