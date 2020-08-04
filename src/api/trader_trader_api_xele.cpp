
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "CXeleTraderApi.hpp"
#include "XeleTraderOrderApi.h"
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
    trader_trader_api_set_param,
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
  CXeleTraderApi* pQueryApi = CXeleTraderApi::CreateTraderApi();
  CXeleTraderHandler* pQueryHandler = new CXeleTraderHandler((void*)self);

  CXeleTraderOrderApi* pTraderApi = CXeleTraderOrderApi::CreateTraderApi();
  CXeleTraderOrderHandler* pTraderHandler = new CXeleTraderOrderHandler((void*)self);
  
  char* pSavePtr;
  char sAddress[256];
  char sFrontAddress[64];
  char* pQueryFrontAddress;
  char* pUdpFrontAddress;
  char* pTcpFrontAddress;

  trader_trader_api_xele* pImp = (trader_trader_api_xele*)malloc(sizeof(trader_trader_api_xele));
  pImp->pQueryApi = (void*)pQueryApi;
  pImp->pQueryHandler = (void*)pQueryHandler;
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  // 获取clientId和localPort
  strncpy(sAddress, self->pUserParam, sizeof(sAddress));
  pQueryFrontAddress = strtok_r(sAddress, "|", &pSavePtr);
  strncpy(pImp->sClientID, pQueryFrontAddress, sizeof(pImp->sClientID));
  
  pQueryFrontAddress = strtok_r(NULL, "|", &pSavePtr);
  strncpy(pImp->sLocalPort, pQueryFrontAddress, sizeof(pImp->sLocalPort));

  self->pUserApi = (void*)pImp;
  
  self->timeCondition = XELE_FTDC_TC_GFD;
  self->hedgeFlag = XELE_FTDC_HF_Speculation;

  
  snprintf(sFrontAddress, sizeof(sFrontAddress), "xxx://0.0.0.0:%s", pImp->sLocalPort);

  strncpy(sAddress, self->pAddress, sizeof(sAddress));
  
  pQueryFrontAddress = strtok_r(sAddress, "|", &pSavePtr);
  CMN_ASSERT (pQueryFrontAddress);
  
  pUdpFrontAddress = strtok_r(NULL, "|", &pSavePtr);
  CMN_ASSERT (pUdpFrontAddress);
  
  pTcpFrontAddress = strtok_r(NULL, "|", &pSavePtr);
  CMN_ASSERT (pTcpFrontAddress);
  
	pQueryApi->RegisterSpi(pQueryHandler);
  pQueryApi->RegisterFront(sFrontAddress, pQueryFrontAddress);
  
  /*	 * 准备login的结构体	 */
  /*	 * 订阅相应的流	 */
  pQueryApi->SubscribePrivateTopic(XELE_TERT_RESUME);
  pQueryApi->SubscribePublicTopic(XELE_TERT_RESUME);
  pQueryApi->SubscribeUserTopic(XELE_TERT_RESUME);

  pQueryApi->SetCustomClientSide();

  pTraderApi->RegisterFront(pUdpFrontAddress, pTcpFrontAddress);
  pTraderApi->RegisterSpi(pTraderHandler);

  /*	 * 开始登录, 使客户端程序开始与交易柜台建立连接	 */
  pQueryApi->Init(false);
  
  signal(SIGSEGV, SIG_DFL);
  
  return;

}

void trader_trader_api_xele_stop(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pQueryApi = (CXeleTraderApi*)pImp->pQueryApi;
  CXeleTraderHandler* pQueryHandler = (CXeleTraderHandler*)pImp->pQueryHandler;
  
  CXeleTraderOrderApi* pTraderApi = (CXeleTraderOrderApi*)pImp->pTraderApi;
  CXeleTraderOrderHandler* pTraderHandler = (CXeleTraderOrderHandler*)pImp->pTraderHandler;
  
  //pQueryApi->RegisterSpi(NULL);
  pQueryApi->Release();
  delete pQueryHandler;

  
  pTraderApi->Release();
  delete pTraderApi;
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_xele_login(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pQueryApi;
  
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
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pQueryApi;

  CXeleFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.AccountID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_xele_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderOrderApi* pTraderApi = (CXeleTraderOrderApi*)pImp->pTraderApi;

  CXeleFairInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));

  char insertType = CXeleTraderOrderApi::ConvertInsertType(XELE_FTDC_TC_GFD, open_close, buy_sell, XELE_FTDC_VC_AV);
  if(insertType < 1){
    return -1;
  }
  ///客户编号
  ///客户令牌
  ///本地报单编号
  inputOrderField.OrderLocalNo = atoi(local_id);
  ///报单价格
  inputOrderField.LimitPrice = price;
  ///合约代码
  strncpy(inputOrderField.InstrumentID, inst, sizeof(inputOrderField.InstrumentID));
  ///数量
  inputOrderField.VolumeTotalOriginal = vol;
  ///输入报单类型
  inputOrderField.InsertType = insertType;
  ///最小成交数量
  inputOrderField.MinVolume = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  return 0;
}

int trader_trader_api_xele_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderOrderApi* pTraderApi = (CXeleTraderOrderApi*)pImp->pTraderApi;

  CXeleFairOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

  ///本地报单操作编号
	inputOrderActionField.ActionLocalNo = atoi(local_id);
  ///被撤单柜台编码
	inputOrderActionField.OrderSysNo = atol(order_sys_id);

  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);
  
  return 0;
}

 
int trader_trader_api_xele_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pQueryApi;

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
  
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  
  return 0;
}

int trader_trader_api_xele_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pQueryApi;

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
  CXeleTraderApi* pTraderApi = (CXeleTraderApi*)pImp->pQueryApi;
  
  CXeleFtdcQryClientAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///投资者代码
  snprintf(qryTradingAccountField.AccountID, sizeof(qryTradingAccountField.AccountID), "%s", self->pUser);

  pTraderApi->ReqQryClientAccount(&qryTradingAccountField, pImp->nTraderRequestID++);
  return 0;
}


