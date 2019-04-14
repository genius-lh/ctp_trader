

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcMduserApi.h"

#include "FemasMduserHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_femas.h"


static void trader_mduser_api_femas_start(trader_mduser_api* self);
static void trader_mduser_api_femas_stop(trader_mduser_api* self);
static void trader_mduser_api_femas_login(trader_mduser_api* self);
static void trader_mduser_api_femas_logout(trader_mduser_api* self);
static void trader_mduser_api_femas_subscribe(trader_mduser_api* self, char* instrument);

static femas_mduser_api_cb* femas_mduser_api_cb_get();

static void femas_mduser_on_front_connected(void* arg);
static void femas_mduser_on_front_disconnected(void* arg, int nReason);
static void femas_mduser_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_mduser_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_mduser_on_rtn_depth_market_data(void* arg, CUstpFtdcDepthMarketDataField *pMarketData);
static void femas_mduser_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_mduser_on_rsp_sub_market_data(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_mduser_on_rsp_un_sub_market_data(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);



#ifdef __cplusplus
}
#endif

femas_mduser_api_cb* femas_mduser_api_cb_get()
{
  static femas_mduser_api_cb femas_mduser_api_cb_st = {
    femas_mduser_on_front_connected,
    femas_mduser_on_front_disconnected,
    femas_mduser_on_rsp_user_login,
    femas_mduser_on_rsp_user_logout,
    femas_mduser_on_rtn_depth_market_data,
    femas_mduser_on_rsp_error,
    femas_mduser_on_rsp_sub_market_data,
    femas_mduser_on_rsp_un_sub_market_data
  };

  return &femas_mduser_api_cb_st;

}


trader_mduser_api_method* trader_mduser_api_femas_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_femas_start,
    trader_mduser_api_femas_stop,
    trader_mduser_api_femas_login,
    trader_mduser_api_femas_logout,
    trader_mduser_api_femas_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_femas_start(trader_mduser_api* self)
{
  CUstpFtdcMduserApi* pUserApi = CUstpFtdcMduserApi::CreateFtdcMduserApi();
  CFemasMduserHandler* pHandler = new CFemasMduserHandler(femas_mduser_api_cb_get(), (void*)self);

  trader_mduser_api_femas* pImp = (trader_mduser_api_femas*)malloc(sizeof(trader_mduser_api_femas));
  pImp->pMdApi = (void*)pUserApi;
  pImp->pHandler = (void*)pHandler;
  pImp->nRequestID = 0;
  
  self->pUserApi = (void*)pImp;

  // connect
  pUserApi->RegisterSpi(pHandler);
  
  pUserApi->SubscribeMarketDataTopic (100, USTP_TERT_RESUME);
  //pUserApi->SubscribeMarketDataTopic (110, USTP_TERT_RESTART);
  
  //pUserApi->SubscribeMarketDataTopic(self->TopicId, USTP_TERT_QUICK);
  pUserApi->RegisterFront(self->pAddress);
  pUserApi->Init();

  return ;

}

void trader_mduser_api_femas_stop(trader_mduser_api* self)
{
  trader_mduser_api_femas* pImp = (trader_mduser_api_femas*)self->pUserApi;
  CUstpFtdcMduserApi* pUserApi = (CUstpFtdcMduserApi*)pImp->pMdApi;
  CFemasMduserHandler* pHandler = (CFemasMduserHandler*)pImp->pHandler;

  pUserApi->RegisterSpi(NULL);
  pUserApi->Release();
  delete pHandler;
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_femas_login(trader_mduser_api* self)
{
  trader_mduser_api_femas* pImp = (trader_mduser_api_femas*)self->pUserApi;
  CUstpFtdcMduserApi* pUserApi = (CUstpFtdcMduserApi*)pImp->pMdApi;

  // login
  CUstpFtdcReqUserLoginField reqUserLogin;
  memset(&reqUserLogin, 0, sizeof(reqUserLogin));
  strcpy(reqUserLogin.TradingDay, pUserApi->GetTradingDay());
  strcpy(reqUserLogin.BrokerID, self->pBrokerID);
  strcpy(reqUserLogin.UserID, self->pUser);
  strcpy(reqUserLogin.Password,self->pPwd);
  strcpy(reqUserLogin.UserProductInfo, "Login");
  pUserApi->ReqUserLogin(&reqUserLogin, pImp->nRequestID++);
}

void trader_mduser_api_femas_logout(trader_mduser_api* self)
{
  trader_mduser_api_femas* pImp = (trader_mduser_api_femas*)self->pUserApi;
  CUstpFtdcMduserApi* pUserApi = (CUstpFtdcMduserApi*)pImp->pMdApi;

  CUstpFtdcReqUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  strcpy(userLogout.BrokerID, self->pBrokerID);
  strcpy(userLogout.UserID, self->pUser);
  
  pUserApi->ReqUserLogout(&userLogout, pImp->nRequestID++);

}

void trader_mduser_api_femas_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_femas* pImp = (trader_mduser_api_femas*)self->pUserApi;
  CUstpFtdcMduserApi* pUserApi = (CUstpFtdcMduserApi*)pImp->pMdApi;

  char* contracts[1];
  contracts[0] = instrument;

  pUserApi->SubMarketData(contracts, 1);
  return ;

}

void femas_mduser_on_front_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_on_front_connected(self);
  
}

void femas_mduser_on_front_disconnected(void* arg, int nReason)
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

void femas_mduser_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
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

void femas_mduser_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
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

void femas_mduser_on_rtn_depth_market_data(void* arg, CUstpFtdcDepthMarketDataField *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->InstrumentID);
  strcpy(oTick.TradingDay, pMarketData->TradingDay);
  strcpy(oTick.UpdateTime, pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1= pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1= pMarketData->AskVolume1;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void femas_mduser_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
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

void femas_mduser_on_rsp_sub_market_data(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
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

void femas_mduser_on_rsp_un_sub_market_data(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
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



