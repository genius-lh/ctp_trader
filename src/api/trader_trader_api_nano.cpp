#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HSExtendTradeApi.h"
#include "HSDataType.h"
#include "HSExtendStruct.h"

#include "NanoTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_nano.h"
#include "cmn_log.h"

static int trader_trader_api_nano_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_nano_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_nano_start(trader_trader_api* self);
static void trader_trader_api_nano_stop(trader_trader_api* self);
static void trader_trader_api_nano_login(trader_trader_api* self);
static void trader_trader_api_nano_logout(trader_trader_api* self);

static int trader_trader_api_nano_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_nano_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);

static int trader_trader_api_nano_qry_instrument(trader_trader_api* self);
static int trader_trader_api_nano_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_nano_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_nano_qry_trading_account(trader_trader_api* self);

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_nano_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_nano_get_trading_day,
    trader_trader_api_nano_get_max_order_local_id,
    trader_trader_api_nano_start,
    trader_trader_api_nano_stop,
    trader_trader_api_nano_login,
    trader_trader_api_nano_logout,
    trader_trader_api_nano_order_insert,
    trader_trader_api_nano_order_action,
    trader_trader_api_nano_qry_instrument,
    trader_trader_api_nano_qry_user_investor,
    trader_trader_api_nano_qry_investor_position,
    trader_trader_api_nano_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_nano_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  int nTradingDay = pTraderHandler->GetTradingDate();
  snprintf(tradingday, 9, "%d", nTradingDay);
  return 0;
}

int trader_trader_api_nano_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;
  *local_id = pTraderHandler->GetMaxOrderRef() / 10;
  return 0;
}

void trader_trader_api_nano_start(trader_trader_api* self)
{
  CHSExtendTradeApi* pTraderApi = (CHSExtendTradeApi*)NewTradeApi(self->pWorkspace, "etc/HsApiConfig.ini");
  CNanoTraderHandler* pTraderHandler = new CNanoTraderHandler(pTraderApi, (void*)self);

  trader_trader_api_nano* pImp = (trader_trader_api_nano*)malloc(sizeof(trader_trader_api_nano));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  self->pUserApi = (void*)pImp;

  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterSubModel(HS_TERT_RESUME);
  pTraderApi->RegisterFensServer(self->pAddress, self->pUser);

  CHSInitConfigField initCfg;
  memset(&initCfg, 0, sizeof(initCfg));
  initCfg.APICheckVersion = API_STRUCT_CHECK_VERSION;
  pTraderApi->Init(&initCfg);

  return;
}

void trader_trader_api_nano_stop(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return;
  }
  CHSExtendTradeApi* pTraderApi = (CHSExtendTradeApi*)pImp->pTraderApi;

  pTraderApi->ReleaseApi();

  free(pImp);
  self->pUserApi = (void*)NULL;

  return;
}

void trader_trader_api_nano_login(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->Login();
  return;
}

void trader_trader_api_nano_logout(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return;
  }
  trader_trader_api_on_rsp_user_logout(self, 0, "OK");
  return;
}

int trader_trader_api_nano_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->InsertOrder(inst, local_id, buy_sell, open_close, price, vol);

  return 0;
}

int trader_trader_api_nano_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->CancelOrder(inst, exchange_id, local_id, org_local_id, order_sys_id);

  return 0;
}

int trader_trader_api_nano_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->QryInstrument();

  return 0;
}

int trader_trader_api_nano_qry_user_investor(trader_trader_api* self)
{
  return 0;
}

int trader_trader_api_nano_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->QryPosition();

  return 0;
}

int trader_trader_api_nano_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_nano* pImp = (trader_trader_api_nano*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CNanoTraderHandler* pTraderHandler = (CNanoTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->QryTradingAccount();

  return 0;
}
