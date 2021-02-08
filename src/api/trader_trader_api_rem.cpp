
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "EesTraderApi.h"
#include "RemTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_rem.h"
#include "cmn_log.h"

static int trader_trader_api_rem_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_rem_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_rem_start(trader_trader_api* self);
static void trader_trader_api_rem_stop(trader_trader_api* self);
static void trader_trader_api_rem_login(trader_trader_api* self);
static void trader_trader_api_rem_logout(trader_trader_api* self);
  
static int trader_trader_api_rem_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_rem_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_rem_qry_instrument(trader_trader_api* self);
static int trader_trader_api_rem_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_rem_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_rem_qry_trading_account(trader_trader_api* self);

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_rem_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_rem_get_trading_day,
    trader_trader_api_rem_get_max_order_local_id,
    trader_trader_api_rem_start,
    trader_trader_api_rem_stop,
    trader_trader_api_rem_login,
    trader_trader_api_rem_logout,
    trader_trader_api_rem_order_insert,
    trader_trader_api_rem_order_action,
    trader_trader_api_rem_qry_instrument,
    trader_trader_api_rem_qry_user_investor,
    trader_trader_api_rem_qry_investor_position,
    trader_trader_api_rem_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_rem_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  sprintf(tradingday, "%d", pTraderHandler->GetTradingDate());
  return 0;
}

int trader_trader_api_rem_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;
  *local_id = pTraderHandler->GetMaxToken();
  return 0;
}


void trader_trader_api_rem_start(trader_trader_api* self)
{

  EESTraderApi* pTraderApi = CreateEESTraderApi();
  CRemTraderHandler* pTraderHandler = new CRemTraderHandler(pTraderApi, (void*)self);
  
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)malloc(sizeof(trader_trader_api_rem));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;

  //NOT USED
  self->timeCondition = '0';
  self->hedgeFlag = '0';

  char remoteTradeIp[128];
  char remoteTradeTCPPort[128];
  char remoteTradeUDPPort[128];
  char remoteQueryIp[128];
  char remoteQueryTCPPort[128];
  char localTradeIp[128];
  char localTradeUDPPort[128];
  char* pSavePtr;
  char sAddress[256];
  char* pIter;

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pIter = strtok_r(sAddress, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteTradeIp, pIter, sizeof(remoteTradeIp));
        
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteTradeTCPPort, pIter, sizeof(remoteTradeTCPPort));
    
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteTradeUDPPort, pIter, sizeof(remoteTradeUDPPort));
    
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteQueryIp, pIter, sizeof(remoteQueryIp));
    
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(remoteQueryTCPPort, pIter, sizeof(remoteQueryTCPPort));
    
    pIter = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pIter);
    strncpy(localTradeIp, pIter, sizeof(localTradeIp));
    
    //pIter = strtok_r(NULL, "|", &pSavePtr);
    //CMN_ASSERT (pIter);
    strncpy(localTradeUDPPort, self->pUserParam, sizeof(localTradeUDPPort));

  }while(0);

  // 连接交易服务器
  RESULT retErr;
	EES_TradeSvrInfo svrInfo;
	strncpy(svrInfo.m_remoteTradeIp, remoteTradeIp, sizeof(svrInfo.m_remoteTradeIp));
	svrInfo.m_remoteTradeTCPPort = atoi(remoteTradeTCPPort);
	svrInfo.m_remoteTradeUDPPort = atoi(remoteTradeUDPPort);
	strncpy(svrInfo.m_remoteQueryIp, remoteQueryIp, sizeof(svrInfo.m_remoteQueryIp));
	svrInfo.m_remoteQueryTCPPort = atoi(remoteQueryTCPPort);
	strncpy(svrInfo.m_LocalTradeIp, localTradeIp, sizeof(svrInfo.m_LocalTradeIp));
	svrInfo.m_LocalTradeUDPPort = atoi(localTradeUDPPort);
  retErr = pTraderApi->ConnServer(svrInfo, pTraderHandler);
  if(retErr != NO_ERROR){
    CMN_ERROR("ConnServer[%d]\n", retErr);
  }

  return;

}

void trader_trader_api_rem_stop(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  EESTraderApi* pTraderApi = (EESTraderApi*)pImp->pTraderApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  if(pTraderApi){
    pTraderApi->DisConnServer();
    DestroyEESTraderApi(pTraderApi);
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

void trader_trader_api_rem_login(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  EESTraderApi* pTraderApi = (EESTraderApi*)pImp->pTraderApi;

  RESULT retErr = pTraderApi->UserLogon(self->pUser, self->pPwd, self->pAppID, self->pAuthCode);
  if(retErr != NO_ERROR){
    CMN_ERROR("UserLogon[%d]\n", retErr);
  }
  return ;
}

void trader_trader_api_rem_logout(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;

  trader_trader_api_on_rsp_user_logout(self, 0, "OK");

  return;
}

  
int trader_trader_api_rem_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->InsertOrder(inst, local_id, buy_sell, open_close, price, vol);

  return 0;
}

int trader_trader_api_rem_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  pTraderHandler->CancelOrder(inst, local_id, org_local_id, exchange_id, order_sys_id);

  return 0;
}

 
int trader_trader_api_rem_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  EESTraderApi* pTraderApi = (EESTraderApi*)pImp->pTraderApi;

  RESULT retErr = pTraderApi->QuerySymbolList();
  if(retErr != NO_ERROR){
    CMN_ERROR("QuerySymbolList[%d]\n", retErr);
  }
  return 0;
}

int trader_trader_api_rem_qry_user_investor(trader_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  //TODO
  return 0;
}

int trader_trader_api_rem_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  EESTraderApi* pTraderApi = (EESTraderApi*)pImp->pTraderApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  RESULT retErr = pTraderApi->QueryAccountPosition(pTraderHandler->GetAccountId(), pImp->nTraderRequestID++);
  if(retErr != NO_ERROR){
    CMN_ERROR("QuerySymbolList[%d]\n", retErr);
  }
  return 0;
}

int trader_trader_api_rem_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_rem* pImp = (trader_trader_api_rem*)self->pUserApi;
  EESTraderApi* pTraderApi = (EESTraderApi*)pImp->pTraderApi;
  CRemTraderHandler* pTraderHandler = (CRemTraderHandler*)pImp->pTraderHandler;

  RESULT retErr = pTraderApi->QueryAccountBP(pTraderHandler->GetAccountId(), pImp->nTraderRequestID++);
  if(retErr != NO_ERROR){
    CMN_ERROR("QuerySymbolList[%d]\n", retErr);
  }
  return 0;
}

