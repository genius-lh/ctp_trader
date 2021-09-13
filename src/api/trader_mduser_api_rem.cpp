#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

#include "RemMduserHandler.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "trader_data.h"
#include "trader_mduser_api.h"

typedef struct trader_mduser_api_rem_def  trader_mduser_api_rem;
struct trader_mduser_api_rem_def {
  void* pMdApi;
  void* pHandler;
  int nRequestID;
};

extern trader_mduser_api_method* trader_mduser_api_rem_method_get();

static void trader_mduser_api_rem_start(trader_mduser_api* self);
static void trader_mduser_api_rem_stop(trader_mduser_api* self);
static void trader_mduser_api_rem_login(trader_mduser_api* self);
static void trader_mduser_api_rem_logout(trader_mduser_api* self);
static void trader_mduser_api_rem_subscribe(trader_mduser_api* self, char* instrument);

#ifdef __cplusplus
}
#endif

trader_mduser_api_method* trader_mduser_api_rem_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_rem_start,
    trader_mduser_api_rem_stop,
    trader_mduser_api_rem_login,
    trader_mduser_api_rem_logout,
    trader_mduser_api_rem_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_rem_start(trader_mduser_api* self)
{
    EESQuoteApi* pQuoteApi = CreateEESQuoteApi();
    CRemMduserHandler* pTraderHandler = new CRemMduserHandler(pQuoteApi, self);

    trader_mduser_api_rem* pImp = (trader_mduser_api_rem*)malloc(sizeof(trader_mduser_api_rem));
    pImp->pMdApi = (void*)pQuoteApi;
    pImp->pHandler = (void*)pTraderHandler;

    // 初始化变量
    EqsTcpInfo svrInfo;
    strncpy(svrInfo.m_eqsIp, remoteTradeIp, sizeof(svrInfo.m_eqsIp));
    svrInfo.m_eqsPort = atoi(remoteTradeTCPPort);

    // 连接交易服务器
    pQuoteApi->ConnServer(svrInfo, pTraderHandler);

    return ;
}

void trader_mduser_api_rem_stop(trader_mduser_api* self)
{
  trader_mduser_api_rem* pImp = (trader_mduser_api_rem*)self->pUserApi;
  EESQuoteApi* pUserApi = (EESQuoteApi*)pImp->pMdApi;
  CRemMduserHandler* pHandler = (CRemMduserHandler*)pImp->pHandler;

  pTraderApi->DisConnServer();

  DestroyEESQuoteApi(pUserApi);
  delete pHandler;
  
  return ;
}

void trader_mduser_api_rem_login(trader_mduser_api* self)
{
  trader_mduser_api_rem* pImp = (trader_mduser_api_rem*)self->pUserApi;
  EESQuoteApi* pUserApi = (EESQuoteApi*)pImp->pMdApi;

  EqsLoginParam loginParam;
  strncpy(loginParam.m_loginId, self->pUser, sizeof(loginParam.m_loginId));
  strncpy(loginParam.m_password, self->pPwd, sizeof(loginParam.m_password));

  pUserApi->LoginToEqs(loginParam);

  return;
}

void trader_mduser_api_rem_logout(trader_mduser_api* self)
{
    return;
}

void trader_mduser_api_rem_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_rem* pImp = (trader_mduser_api_rem*)self->pUserApi;
  EESQuoteApi* pUserApi = (EESQuoteApi*)pImp->pMdApi;

  pUserApi->RegisterSymbol(EQS_FUTURE, instrument);

  return ;

}
