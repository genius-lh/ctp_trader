
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

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_xspeed.h"

static void trader_mduser_api_xspeed_start(trader_mduser_api* self);
static void trader_mduser_api_xspeed_stop(trader_mduser_api* self);
static void trader_mduser_api_xspeed_stock_login(trader_mduser_api* self);
static void trader_mduser_api_xspeed_stock_logout(trader_mduser_api* self);
static void trader_mduser_api_xspeed_stock_subscribe(trader_mduser_api* self, char* instrument);
static void trader_mduser_api_xspeed_sop_login(trader_mduser_api* self);
static void trader_mduser_api_xspeed_sop_logout(trader_mduser_api* self);
static void trader_mduser_api_xspeed_sop_subscribe(trader_mduser_api* self, char* instrument);

static dfitc_sec_mduser_api_cb* xspeed_mduser_api_cb_get();

static void xspeed_mduser_on_front_connected(void* arg);
static void xspeed_mduser_on_front_disconnected(void* arg, int nReason);
static void xspeed_mduser_on_rtn_notice(void* arg, DFITCSECRspNoticeField *pNotice);
static void xspeed_mduser_on_rsp_error(void* arg, DFITCSECRspInfoField *pRspInfo);
static void xspeed_mduser_on_rsp_stockuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_stockuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_sopuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_sopuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_fasluserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_fasluserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_stocksubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_stockunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_sopsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_rsp_sopunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
static void xspeed_mduser_on_stockmarketdata(void* arg, DFITCStockDepthMarketDataField * pMarketDataField);
static void xspeed_mduser_on_sopmarketdata(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField);
static void xspeed_mduser_on_rsp_stockavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);
static void xspeed_mduser_on_rsp_sopavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);
static void xspeed_mduser_on_rsp_usermdpasswordupdate(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);

#ifdef __cplusplus
}
#endif

trader_mduser_api_method* trader_mduser_api_xspeed_stock_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st ={
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_xspeed_start,
    trader_mduser_api_xspeed_stop,
    trader_mduser_api_xspeed_stock_login,
    trader_mduser_api_xspeed_stock_logout,
    trader_mduser_api_xspeed_stock_subscribe
  };

  return &trader_mduser_api_method_st;
}

trader_mduser_api_method* trader_mduser_api_xspeed_sop_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st ={
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_xspeed_start,
    trader_mduser_api_xspeed_stop,
    trader_mduser_api_xspeed_sop_login,
    trader_mduser_api_xspeed_sop_logout,
    trader_mduser_api_xspeed_sop_subscribe
  };

  return &trader_mduser_api_method_st;
}


void trader_mduser_api_xspeed_start(trader_mduser_api* self)
{
  DFITCSECMdApi* pUserApi = DFITCSECMdApi::CreateDFITCMdApi(self->pWorkspace);
  CDfitcSecMduserHandler* pHandler = new CDfitcSecMduserHandler(xspeed_mduser_api_cb_get(), (void*)self);

  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)malloc(sizeof(trader_mduser_api_xspeed));
  pImp->pMdApi = (void*)pUserApi;
  pImp->pHandler = (void*)pHandler;
  pImp->nRequestID = 0;
  strncpy(pImp->instPrefix, "SH", sizeof(pImp->instPrefix));
  
  self->pUserApi = (void*)pImp;

  // connect
  pUserApi->Init(self->pAddress, pHandler);

  return ;
}

void trader_mduser_api_xspeed_stop(trader_mduser_api* self)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;
  CDfitcSecMduserHandler* pHandler = (CDfitcSecMduserHandler*)pImp->pHandler;

  pUserApi->Release();
  delete pHandler;
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_xspeed_stock_login(trader_mduser_api* self)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  // login
  DFITCSECReqUserLoginField reqUserLogin;
  memset(&reqUserLogin, 0, sizeof(reqUserLogin));
  reqUserLogin.requestID = pImp->nRequestID++;
  strcpy(reqUserLogin.accountID, self->pUser);
  strcpy(reqUserLogin.password,self->pPwd);  
  reqUserLogin.compressflag = DFITCSEC_COMPRESS_FALSE;
  
  pUserApi->ReqStockUserLogin(&reqUserLogin);
}

void trader_mduser_api_xspeed_stock_logout(trader_mduser_api* self)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  DFITCSECReqUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  userLogout.requestID = pImp->nRequestID++;
  strcpy(userLogout.accountID, self->pUser);
  
  pUserApi->ReqStockUserLogout(&userLogout);

}

void trader_mduser_api_xspeed_stock_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  char buff[31];
  char* contracts[1];
  snprintf(buff, sizeof(buff), "%s%s", pImp->instPrefix, instrument);
  contracts[0] = buff;

  pUserApi->SubscribeStockMarketData(contracts, 1, pImp->nRequestID++);
  return ;
}

void trader_mduser_api_xspeed_sop_login(trader_mduser_api* self)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
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

void trader_mduser_api_xspeed_sop_logout(trader_mduser_api* self)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  DFITCSECReqUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  userLogout.requestID = pImp->nRequestID++;
  strcpy(userLogout.accountID, self->pUser);
  
  pUserApi->ReqSOPUserLogout(&userLogout);

}

void trader_mduser_api_xspeed_sop_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;

  char buff[31];
  char* contracts[1];
  snprintf(buff, sizeof(buff), "%s%s", pImp->instPrefix, instrument);
  contracts[0] = buff;

  pUserApi->SubscribeSOPMarketData(contracts, 1, pImp->nRequestID++);
  return ;
}


dfitc_sec_mduser_api_cb* xspeed_mduser_api_cb_get()
{
  static dfitc_sec_mduser_api_cb dfitc_sec_mduser_api_cb_st ={
    xspeed_mduser_on_front_connected,
    xspeed_mduser_on_front_disconnected,
    xspeed_mduser_on_rtn_notice,
    xspeed_mduser_on_rsp_error,
    xspeed_mduser_on_rsp_stockuserlogin,
    xspeed_mduser_on_rsp_stockuserlogout,
    xspeed_mduser_on_rsp_sopuserlogin,
    xspeed_mduser_on_rsp_sopuserlogout,
    xspeed_mduser_on_rsp_fasluserlogin,
    xspeed_mduser_on_rsp_fasluserlogout,
    xspeed_mduser_on_rsp_stocksubmarketdata,
    xspeed_mduser_on_rsp_stockunsubmarketdata,
    xspeed_mduser_on_rsp_sopsubmarketdata,
    xspeed_mduser_on_rsp_sopunsubmarketdata,
    xspeed_mduser_on_stockmarketdata,
    xspeed_mduser_on_sopmarketdata,
    xspeed_mduser_on_rsp_stockavailablequot,
    xspeed_mduser_on_rsp_sopavailablequot,
    xspeed_mduser_on_rsp_usermdpasswordupdate
  };

  return &dfitc_sec_mduser_api_cb_st;
}

void xspeed_mduser_on_front_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_on_front_connected(self);
}

void xspeed_mduser_on_front_disconnected(void* arg, int nReason)
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

void xspeed_mduser_on_rtn_notice(void* arg, DFITCSECRspNoticeField *pNotice)
{
}

void xspeed_mduser_on_rsp_error(void* arg, DFITCSECRspInfoField *pRspInfo)
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

void xspeed_mduser_on_rsp_stockuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_xspeed* pImp = (trader_mduser_api_xspeed*)self->pUserApi;
  DFITCSECMdApi* pUserApi = (DFITCSECMdApi*)pImp->pMdApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;

    
  }

  if(pRspUserLogin){
    
    errNo = pRspUserLogin->result;
    errMsg = pRspUserLogin->rtnMsg;

    DFITCReqQuotQryField quotQry;
    memset(&quotQry, 0, sizeof(quotQry));
    
    quotQry.requestID = pImp->nRequestID++;
    strcpy(quotQry.accountID, self->pUser);
    //pUserApi->ReqStockAvailableQuotQry(&quotQry);

  }
  
  trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);
}

void xspeed_mduser_on_rsp_stockuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
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

void xspeed_mduser_on_rsp_sopuserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
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

void xspeed_mduser_on_rsp_sopuserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
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

void xspeed_mduser_on_rsp_fasluserlogin(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo)
{
}

void xspeed_mduser_on_rsp_fasluserlogout(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo)
{
}

void xspeed_mduser_on_rsp_stocksubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
    
    printf("errorID=[%d]"
      "errorMsg=[%s]\n",
      pRspInfo->errorID,
      pRspInfo->errorMsg);
  }
  
  if(pSpecificInstrument){
    printf("securityID=[%s]"
      "exchangeID=[%s]\n",
      pSpecificInstrument->securityID,
      pSpecificInstrument->exchangeID);
  }

  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);

  return;
}

void xspeed_mduser_on_rsp_stockunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
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

void xspeed_mduser_on_rsp_sopsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
    
    printf("errorID=[%d]"
      "errorMsg=[%s]\n",
      pRspInfo->errorID,
      pRspInfo->errorMsg);
  }
  
  if(pSpecificInstrument){
    printf("securityID=[%s]"
      "exchangeID=[%s]\n",
      pSpecificInstrument->securityID,
      pSpecificInstrument->exchangeID);
  }

  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);

  return;

}

void xspeed_mduser_on_rsp_sopunsubmarketdata(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo)
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

void xspeed_mduser_on_stockmarketdata(void* arg, DFITCStockDepthMarketDataField * pMarketDataField)
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

void xspeed_mduser_on_sopmarketdata(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField)
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

void xspeed_mduser_on_rsp_stockavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag)
{
  if(!pAvailableQuotInfo){
    return ;
  }
  printf("securityID=[%s]"
    "securityName=[%s]\n",
    pAvailableQuotInfo->securityID,
    pAvailableQuotInfo->securityName);

}

void xspeed_mduser_on_rsp_sopavailablequot(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag)
{
  if(!pAvailableQuotInfo){
    return ;
  }
  printf("securityID=[%s]"
    "securityName=[%s]\n",
    pAvailableQuotInfo->securityID,
    pAvailableQuotInfo->securityName);

}

void xspeed_mduser_on_rsp_usermdpasswordupdate(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo)
{
}

