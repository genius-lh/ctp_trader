#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ydApi.h"
#include "ydError.h"
#include "YdTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_yd.h"
#include "cmn_log.h"

static int trader_trader_api_yd_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_yd_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_yd_start(trader_trader_api* self);
static void trader_trader_api_yd_stop(trader_trader_api* self);
static void trader_trader_api_yd_login(trader_trader_api* self);
static void trader_trader_api_yd_logout(trader_trader_api* self);
  
static int trader_trader_api_yd_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_yd_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_yd_qry_instrument(trader_trader_api* self);
static int trader_trader_api_yd_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_yd_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_yd_qry_trading_account(trader_trader_api* self);

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_yd_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_yd_get_trading_day,
    trader_trader_api_yd_get_max_order_local_id,
    trader_trader_api_yd_start,
    trader_trader_api_yd_stop,
    trader_trader_api_yd_login,
    trader_trader_api_yd_logout,
    trader_trader_api_yd_order_insert,
    trader_trader_api_yd_order_action,
    trader_trader_api_yd_qry_instrument,
    trader_trader_api_yd_qry_user_investor,
    trader_trader_api_yd_qry_investor_position,
    trader_trader_api_yd_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_yd_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;

  sprintf(tradingday, "%d", pTraderHandler->GetTradingDate());
  return 0;
}

int trader_trader_api_yd_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;
  *local_id = pTraderHandler->GetMaxOrderRef() / 10;
  return 0;
}


void trader_trader_api_yd_start(trader_trader_api* self)
{
  char *configFilename = self->pAddress;
  YDExtendedApi* pTraderApi = makeYDExtendedApi(configFilename);
  CYdTraderHandler* pTraderHandler = new CYdTraderHandler(pTraderApi, (void*)self);
  
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)malloc(sizeof(trader_trader_api_yd));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;

  //NOT USED
  self->timeCondition = '0';
  self->hedgeFlag = '0';

  // 连接交易服务器
  bool retErr = pTraderApi->start(pTraderHandler);
  if(retErr != true){
    CMN_ERROR("ConnServer[%d]\n", retErr);
  }

  return;

}

void trader_trader_api_yd_stop(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return;
  }

  YDApi* pTraderApi = (YDApi*)pImp->pTraderApi;
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;

  if(pTraderApi){
    pTraderApi->startDestroy();
    pTraderHandler->Join();
    delete pTraderApi;
  }

  if(pTraderHandler){
    delete pTraderHandler;
    pTraderHandler = NULL;
  }
  
  if(pImp){
    free(pImp);
    pImp = NULL;
  }
  
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_yd_login(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  YDApi* pTraderApi = (YDApi*)pImp->pTraderApi;
  
	int ret = pTraderApi->login(self->pUser, self->pPwd, NULL, NULL);
  
  if (!ret){
    CMN_ERROR("can not login\n");
  }
  return ;
}

void trader_trader_api_yd_logout(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  YDApi* pTraderApi = (YDApi*)pImp->pTraderApi;
  pTraderApi->disconnect();

  trader_trader_api_on_rsp_user_logout(self, 0, "OK");

  return;
}

  
int trader_trader_api_yd_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->InsertOrder(inst, local_id, buy_sell, open_close, price, vol);

  return 0;
}

int trader_trader_api_yd_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->CancelOrder(inst, exchange_id, local_id, org_local_id, order_sys_id);

  return 0;
}

 
int trader_trader_api_yd_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }

  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryInstrument();

  return 0;
}

int trader_trader_api_yd_qry_user_investor(trader_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  return 0;
}

int trader_trader_api_yd_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryInvestorPosition();
  return 0;
}

int trader_trader_api_yd_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_yd* pImp = (trader_trader_api_yd*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CYdTraderHandler* pTraderHandler = (CYdTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryTradingAccount();
  return 0;
}

