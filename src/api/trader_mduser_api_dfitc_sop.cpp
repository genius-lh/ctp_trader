
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#include "DFITCSECMduserHandler.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "trader_data_type.h"
#include "trader_data_struct.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_dfitc_sop.h"


static void trader_mduser_api_dfitc_sop_start(trader_mduser_api* self);
static void trader_mduser_api_dfitc_sop_stop(trader_mduser_api* self);
static void trader_mduser_api_dfitc_sop_login(trader_mduser_api* self);
static void trader_mduser_api_dfitc_sop_logout(trader_mduser_api* self);
static void trader_mduser_api_dfitc_sop_subscribe(trader_mduser_api* self, char* instrument);

static dfitc_sec_mduser_api_cb* dfitc_sec_mduser_api_cb_get();

static void dfitc_sop_mduser_on_front_connected(void* arg);
static void dfitc_sop_mduser_on_front_disconnected(void* arg, int nReason);
#ifdef DFITC_SEC_V128ON
static void dfitc_sop_mduser_on_rtn_notice(void* arg, DFITCSECRspNoticeField *pNotice);
#endif
static void dfitc_sop_mduser_on_rsp_error(void* arg, DFITCSECRspInfoField *pRspInfo);
static void dfitc_sop_mduser_on_rsp_stockuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_stockuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_sopuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_sopuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_fasluserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_fasluserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_stocksubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_stockunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_sopsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_rsp_sopunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void dfitc_sop_mduser_on_stockmarketdata(void* arg, DFITCStockDepthMarketDataField * pMarketDataField);
static void dfitc_sop_mduser_on_sopmarketdata(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField);
static void dfitc_sop_mduser_on_rsp_stockavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);
static void dfitc_sop_mduser_on_rsp_sopavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);
static void dfitc_sop_mduser_on_rsp_usermdpasswordupdate(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);

#ifdef __cplusplus
}
#endif

trader_mduser_api_method* trader_mduser_api_dfitc_sop_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st ={
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_dfitc_sop_start,
    trader_mduser_api_dfitc_sop_stop,
    trader_mduser_api_dfitc_sop_login,
    trader_mduser_api_dfitc_sop_logout,
    trader_mduser_api_dfitc_sop_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_dfitc_sop_start(trader_mduser_api* self)
{
  DFITCSECMdApi* pUserApi = DFITCSECMdApi::CreateDFITCMdApi(self->pWorkspace);
  CDfitcSecMduserHandler* pHandler = new CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb_get(), (void*)self);

  trader_mduser_api_dfitc_sop* pImp = (trader_mduser_api_dfitc_sop*)malloc(sizeof(trader_mduser_api_dfitc_sop));
  pImp->pMdApi = (void*)pUserApi;
  pImp->pHandler = (void*)pHandler;
  pImp->nRequestID = 0;
  
  self->pUserApi = (void*)pImp;

  // connect
  pUserApi->Init(self->pAddress, pHandler);

  return ;
}

void trader_mduser_api_dfitc_sop_stop(trader_mduser_api* self)
{
  trader_mduser_api_dfitc_sop* pImp = (trader_mduser_api_dfitc_sop*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;
  CDfitcSecMduserHandler* pHandler = (CDfitcSecMduserHandler*)pImp->pHandler;

  pUserApi->Release();
  delete pHandler;
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_dfitc_sop_login(trader_mduser_api* self)
{
  trader_mduser_api_dfitc_sop* pImp = (trader_mduser_api_dfitc_sop*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  // login
  DFITCSECReqUserLoginField reqUserLogin;
  memset(&reqUserLogin, 0, sizeof(reqUserLogin));
  reqUserLogin.requestID = pImp->nRequestID++;
  strcpy(reqUserLogin.accountID, self->pUser);
  strcpy(reqUserLogin.password,self->pPwd);  
  reqUserLogin.compressflag = DFITCSEC_COMPRESS_FALSE;
  
  pUserApi->ReqSOPUserLogin(&reqUserLogin);
}

void trader_mduser_api_dfitc_sop_logout(trader_mduser_api* self)
{
  trader_mduser_api_dfitc_sop* pImp = (trader_mduser_api_dfitc_sop*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  DFITCSECReqUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  userLogout.requestID = pImp->nRequestID++;
  strcpy(userLogout.accountID, self->pUser);
  
  pUserApi->ReqSOPUserLogout(&userLogout);

}

void trader_mduser_api_dfitc_sop_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_dfitc_sop* pImp = (trader_mduser_api_dfitc_sop*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  char* contracts[1];
  contracts[0] = instrument;

  pUserApi->SubscribeSOPMarketData(contracts, 1, pImp->nRequestID++);
  return ;
}

dfitc_sec_mduser_api_cb* dfitc_sec_mduser_api_cb_get()
{
  static dfitc_sec_mduser_api_cb dfitc_sec_mduser_api_cb_st ={
    dfitc_sop_mduser_on_front_connected,
    dfitc_sop_mduser_on_front_disconnected,
#ifdef DFITC_SEC_V128ON
    dfitc_sop_mduser_on_rtn_notice,
#endif
    dfitc_sop_mduser_on_rsp_error,
    dfitc_sop_mduser_on_rsp_stockuserlogin,
    dfitc_sop_mduser_on_rsp_stockuserlogout,
    dfitc_sop_mduser_on_rsp_sopuserlogin,
    dfitc_sop_mduser_on_rsp_sopuserlogout,
    dfitc_sop_mduser_on_rsp_fasluserlogin,
    dfitc_sop_mduser_on_rsp_fasluserlogout,
    dfitc_sop_mduser_on_rsp_stocksubmarketdata,
    dfitc_sop_mduser_on_rsp_stockunsubmarketdata,
    dfitc_sop_mduser_on_rsp_sopsubmarketdata,
    dfitc_sop_mduser_on_rsp_sopunsubmarketdata,
    dfitc_sop_mduser_on_stockmarketdata,
    dfitc_sop_mduser_on_sopmarketdata,
    dfitc_sop_mduser_on_rsp_stockavailablequot,
    dfitc_sop_mduser_on_rsp_sopavailablequot,
    dfitc_sop_mduser_on_rsp_usermdpasswordupdate
  };

  return &dfitc_sec_mduser_api_cb_st;
}

void dfitc_sop_mduser_on_front_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_on_front_connected(self);
}

void dfitc_sop_mduser_on_front_disconnected(void* arg, int nReason)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int errNo = nReason;
  char errMsg[81];

  switch(nReason)
  {
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

#ifdef DFITC_SEC_V128ON
void dfitc_sop_mduser_on_rtn_notice(void* arg, DFITCSECRspNoticeField *pNotice)
{
}
#endif

void dfitc_sop_mduser_on_rsp_error(void* arg, DFITCSECRspInfoField *pRspInfo)
{  
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_mduser_api_on_rsp_error(self, errNo, errMsg);
}

void dfitc_sop_mduser_on_rsp_stockuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_stockuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_sopuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);

}

void dfitc_sop_mduser_on_rsp_sopuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_mduser_api_on_rsp_user_logout(self, errNo, errMsg);
}

void dfitc_sop_mduser_on_rsp_fasluserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_fasluserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_stocksubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_stockunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
}

void dfitc_sop_mduser_on_rsp_sopsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }

  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);

  return;

}

void dfitc_sop_mduser_on_rsp_sopunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_mduser_api_on_rsp_un_sub_market_data(self, errNo, errMsg);
  return ;
}

void dfitc_sop_mduser_on_stockmarketdata(void* arg, DFITCStockDepthMarketDataField * pMarketDataField)
{
}

void dfitc_sop_mduser_on_sopmarketdata(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketDataField->staticDataField.securityID);

  sprintf(oTick.TradingDay, "%8d", pMarketDataField->staticDataField.tradingDay);
  memcpy(oTick.UpdateTime, &(pMarketDataField->sharedDataField.updateTime[0]), 8);
  oTick.UpdateMillisec = atoi(&pMarketDataField->sharedDataField.updateTime[9]);

  oTick.BidPrice1 = pMarketDataField->sharedDataField.bidPrice1;
  oTick.BidVolume1= pMarketDataField->sharedDataField.bidQty1;
  oTick.AskPrice1 = pMarketDataField->sharedDataField.askPrice1;
  oTick.AskVolume1= pMarketDataField->sharedDataField.askQty1;
  
  oTick.UpperLimitPrice = pMarketDataField->staticDataField.upperLimitPrice;
  oTick.LowerLimitPrice = pMarketDataField->staticDataField.lowerLimitPrice;
  
  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;

}

void dfitc_sop_mduser_on_rsp_stockavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag)
{
}

void dfitc_sop_mduser_on_rsp_sopavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag)
{
}

void dfitc_sop_mduser_on_rsp_usermdpasswordupdate(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo)
{
}

