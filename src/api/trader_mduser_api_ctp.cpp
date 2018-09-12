
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcMdApi.h"

#include "CtpMduserHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_ctp.h"


static void trader_mduser_api_ctp_start(trader_mduser_api* self);
static void trader_mduser_api_ctp_stop(trader_mduser_api* self);
static void trader_mduser_api_ctp_login(trader_mduser_api* self);
static void trader_mduser_api_ctp_logout(trader_mduser_api* self);
static void trader_mduser_api_ctp_subscribe(trader_mduser_api* self, char* instrument);

static ctp_mduser_api_cb* ctp_mduser_api_cb_get();

static void ctp_mduser_on_front_connected(void* arg);
static void ctp_mduser_on_front_disconnected(void* arg, int nReason);
static void ctp_mduser_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_mduser_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_mduser_on_rtn_depth_market_data(void* arg, CThostFtdcDepthMarketDataField *pMarketData);
static void ctp_mduser_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_mduser_on_rsp_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_mduser_on_rsp_un_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

#ifdef __cplusplus
}
#endif

ctp_mduser_api_cb* ctp_mduser_api_cb_get()
{
  static ctp_mduser_api_cb ctp_mduser_api_cb_st = {
    ctp_mduser_on_front_connected,
    ctp_mduser_on_front_disconnected,
    ctp_mduser_on_rsp_user_login,
    ctp_mduser_on_rsp_user_logout,
    ctp_mduser_on_rtn_depth_market_data,
    ctp_mduser_on_rsp_error,
    ctp_mduser_on_rsp_sub_market_data,
    ctp_mduser_on_rsp_un_sub_market_data
  };

  return &ctp_mduser_api_cb_st;

}


trader_mduser_api_method* trader_mduser_api_ctp_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_ctp_start,
    trader_mduser_api_ctp_stop,
    trader_mduser_api_ctp_login,
    trader_mduser_api_ctp_logout,
    trader_mduser_api_ctp_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_ctp_start(trader_mduser_api* self)
{
  CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(self->pWorkspace, true, false);
  CCtpMduserHandler* pHandler = new CCtpMduserHandler(ctp_mduser_api_cb_get(), (void*)self);

  trader_mduser_api_ctp* pImp = (trader_mduser_api_ctp*)malloc(sizeof(trader_mduser_api_ctp));
  pImp->pMdApi = (void*)pUserApi;
  pImp->pHandler = (void*)pHandler;
  pImp->nRequestID = 0;
  
  self->pUserApi = (void*)pImp;

  // connect
  pUserApi->RegisterSpi(pHandler);
  //pUserApi->SubscribeMarketDataTopic(self->TopicId, USTP_TERT_QUICK);
  pUserApi->RegisterFront(self->pAddress);
  pUserApi->Init();

  return ;

}

void trader_mduser_api_ctp_stop(trader_mduser_api* self)
{
  trader_mduser_api_ctp* pImp = (trader_mduser_api_ctp*)self->pUserApi;
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)pImp->pMdApi;
  CCtpMduserHandler* pHandler = (CCtpMduserHandler*)pImp->pHandler;

  pUserApi->RegisterSpi(NULL);
  pUserApi->Release();
  delete pHandler;
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_ctp_login(trader_mduser_api* self)
{
  trader_mduser_api_ctp* pImp = (trader_mduser_api_ctp*)self->pUserApi;
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)pImp->pMdApi;

  // login
  CThostFtdcReqUserLoginField reqUserLogin;
  memset(&reqUserLogin, 0, sizeof(reqUserLogin));
  //strcpy(reqUserLogin.TradingDay, pUserApi->GetTradingDay());
  strcpy(reqUserLogin.BrokerID, self->pBrokerID);
  strcpy(reqUserLogin.UserID, self->pUser);
  strcpy(reqUserLogin.Password,self->pPwd);
  strcpy(reqUserLogin.UserProductInfo, "Login");
  pUserApi->ReqUserLogin(&reqUserLogin, pImp->nRequestID++);
}

void trader_mduser_api_ctp_logout(trader_mduser_api* self)
{
  trader_mduser_api_ctp* pImp = (trader_mduser_api_ctp*)self->pUserApi;
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)pImp->pMdApi;

  CThostFtdcUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  strcpy(userLogout.BrokerID, self->pBrokerID);
  strcpy(userLogout.UserID, self->pUser);
  
  pUserApi->ReqUserLogout(&userLogout, pImp->nRequestID++);

}

void trader_mduser_api_ctp_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_ctp* pImp = (trader_mduser_api_ctp*)self->pUserApi;
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)pImp->pMdApi;

  char* contracts[1];
  contracts[0] = instrument;

  pUserApi->SubscribeMarketData(contracts, 1);
  return ;

}

void ctp_mduser_on_front_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_on_front_connected(self);
  
}

void ctp_mduser_on_front_disconnected(void* arg, int nReason)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int errNo = nReason;
  char errMsg[81];

  switch(nReason) {
  case 0x1001:
    strcpy(errMsg, "0x1001 ÍøÂç¶ÁÊ§°Ü");
    break;
  case 0x1002:
    strcpy(errMsg, "0x1002 ÍøÂçÐ´Ê§°Ü");
    break;
  case 0x2001:
    strcpy(errMsg, "0x2001 ½ÓÊÕÐÄÌø³¬Ê±");
    break;
  case 0x2002:
    strcpy(errMsg, "0x2002 ·¢ËÍÐÄÌøÊ§°Ü");
    break;
  case 0x2003:
    strcpy(errMsg, "0x2003 ÊÕµ½´íÎó±¨ÎÄ");
    break;
  }
  
  trader_mduser_api_on_front_disconnected(self, errNo, errMsg);

  return ;
}

void ctp_mduser_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);
  
}

void ctp_mduser_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_user_logout(self, errNo, errMsg);
  
}

void ctp_mduser_on_rtn_depth_market_data(void* arg, CThostFtdcDepthMarketDataField *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->InstrumentID);
  strcpy(oTick.TradingDay, pMarketData->TradingDay);
  strcpy(oTick.UpdateTime, pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1 = pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1 = pMarketData->AskVolume1;
  oTick.UpperLimitPrice = pMarketData->UpperLimitPrice;
  oTick.LowerLimitPrice = pMarketData->LowerLimitPrice;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void ctp_mduser_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_error(self, errNo, errMsg);

}

void ctp_mduser_on_rsp_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);

}

void ctp_mduser_on_rsp_un_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_un_sub_market_data(self, errNo, errMsg);

}



