#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "hxapitype.h"
#include "hxapistructs.h"
#include "hxapi.h"

#include "HxtsTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_hxts.h"
#include "cmn_log.h"

static int trader_trader_api_hxts_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_hxts_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_hxts_start(trader_trader_api* self);
static void trader_trader_api_hxts_stop(trader_trader_api* self);
static void trader_trader_api_hxts_login(trader_trader_api* self);
static void trader_trader_api_hxts_logout(trader_trader_api* self);
  
static int trader_trader_api_hxts_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_hxts_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_hxts_qry_instrument(trader_trader_api* self);
static int trader_trader_api_hxts_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_hxts_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_hxts_qry_trading_account(trader_trader_api* self);

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_hxts_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_hxts_get_trading_day,
    trader_trader_api_hxts_get_max_order_local_id,
    trader_trader_api_hxts_start,
    trader_trader_api_hxts_stop,
    trader_trader_api_hxts_login,
    trader_trader_api_hxts_logout,
    trader_trader_api_hxts_order_insert,
    trader_trader_api_hxts_order_action,
    trader_trader_api_hxts_qry_instrument,
    trader_trader_api_hxts_qry_user_investor,
    trader_trader_api_hxts_qry_investor_position,
    trader_trader_api_hxts_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_hxts_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;

  strcpy(tradingday, pTraderHandler->GetTradingDate());
  return 0;
}

int trader_trader_api_hxts_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;
  *local_id = pTraderHandler->GetMaxOrderRef() / 10;
  return 0;
}


void trader_trader_api_hxts_start(trader_trader_api* self)
{
	CHxTraderApi *pTraderApi = CHxTraderApi::CreateHxTraderApi();
  CHxtsTraderHandler* pTraderHandler = new CHxtsTraderHandler(pTraderApi, (void*)self);
  
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)malloc(sizeof(trader_trader_api_hxts));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;

  //NOT USED
  self->timeCondition = '0';
  self->hedgeFlag = '0';

  char ethName[128];
  char remoteTradeIp[128];
  char remoteTradeUDPPort[128];
  char localTradeIp[128];
  char localTradeUDPPort[128];
  char* pSavePtr;
  char sAddress[256];
  char* pIter;

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pIter = strtok_r(sAddress, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(ethName, pIter, sizeof(ethName));
    
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteTradeIp, pIter, sizeof(remoteTradeIp));
        
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteTradeUDPPort, pIter, sizeof(remoteTradeUDPPort));

    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(localTradeIp, pIter, sizeof(localTradeIp));
    
    //pIter = strtok_r(NULL, "|", &pSavePtr);
    //CMN_ASSERT (pIter);
    strncpy(localTradeUDPPort, self->pUserParam, sizeof(localTradeUDPPort));

  }while(0);

  // 连接交易服务器
	pTraderApi->RegisterSpi(pTraderHandler);
	pTraderApi->SetRecvNodeCount(1024*100);

  pthread_t tid = pTraderApi->Init(ethName, remoteTradeIp, atoi(remoteTradeUDPPort), localTradeIp, atoi(localTradeUDPPort));

  pImp->workTreadId = tid;

  trader_trader_api_on_front_connected(self);

  return;

}

void trader_trader_api_hxts_stop(trader_trader_api* self)
{
  int ret;
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return;
  }
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;

  ret = pthread_cancel(pImp->workTreadId);
  if(ret){
    CMN_ERROR("pthread_cancel errno[%d][%s]!\n", errno, strerror(errno));
    exit(0);
  }
  ret = pthread_join(pImp->workTreadId, (void**)NULL);
  if(ret){
    CMN_ERROR("pthread_join errno[%d][%s]!\n", errno, strerror(errno));
    exit(0);
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

void trader_trader_api_hxts_login(trader_trader_api* self)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  CHxTraderApi* pTraderApi = (CHxTraderApi*)pImp->pTraderApi;
	req_login_field_s req_login;
	strncpy(req_login.szAccount, self->pUser, sizeof(req_login.szAccount));
	strncpy(req_login.szPwd, self->pPwd, sizeof(req_login.szPwd));
	strncpy(req_login.szAppid, self->pAppID, sizeof(req_login.szAppid));
	strncpy(req_login.szAuthenticateId, self->pAuthCode, sizeof(req_login.szAuthenticateId));

	pTraderApi->ReqLogin(&req_login);
  return ;
}

void trader_trader_api_hxts_logout(trader_trader_api* self)
{

  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  CHxTraderApi* pTraderApi = (CHxTraderApi*)pImp->pTraderApi;
	pTraderApi->ReqLogout();

  return;
}

  
int trader_trader_api_hxts_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->InsertOrder(inst, local_id, buy_sell, open_close, price, vol);

  return 0;
}

int trader_trader_api_hxts_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->CancelOrder(inst, exchange_id, local_id, org_local_id, order_sys_id);

  return 0;
}

 
int trader_trader_api_hxts_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }

  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryInstrument();

  return 0;
}

int trader_trader_api_hxts_qry_user_investor(trader_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  return 0;
}

int trader_trader_api_hxts_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryInvestorPosition();
  return 0;
}

int trader_trader_api_hxts_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_hxts* pImp = (trader_trader_api_hxts*)self->pUserApi;
  if(!pImp){
    CMN_ERROR("pImp is null\n");
    return -1;
  }
  CHxtsTraderHandler* pTraderHandler = (CHxtsTraderHandler*)pImp->pTraderHandler;
  pTraderHandler->QryTradingAccount();
  return 0;
}

