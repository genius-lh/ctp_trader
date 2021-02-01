
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcTraderApi.h"

#include "FemasAFTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_femas_af.h"
#include "cmn_log.h"

static int trader_trader_api_femas_af_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_femas_af_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_femas_af_start(trader_trader_api* self);
static void trader_trader_api_femas_af_stop(trader_trader_api* self);
static void trader_trader_api_femas_af_login(trader_trader_api* self);
static void trader_trader_api_femas_af_logout(trader_trader_api* self);
  
static int trader_trader_api_femas_af_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_femas_af_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_femas_af_qry_instrument(trader_trader_api* self);
static int trader_trader_api_femas_af_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_femas_af_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_femas_af_qry_trading_account(trader_trader_api* self);


static void femas_af_trader_on_front_connected(void* arg);
static void femas_af_trader_on_qry_front_connected(void* arg);
static void femas_af_trader_on_front_disconnected(void* arg, int nReason);
static void femas_af_trader_on_qry_front_disconnected(void* arg, int nReason);
static void femas_af_trader_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rsp_query_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);

static void femas_af_trader_on_rsp_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rsp_order_action(void* arg, CUstpFtdcOrderActionField *pInputOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rtn_order(void* arg, CUstpFtdcOrderField *pOrder);
static void femas_af_trader_on_rtn_trade(void* arg, CUstpFtdcTradeField *pTrade);
static void femas_af_trader_on_err_rtn_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);
static void femas_af_trader_on_err_rtn_order_action(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);
static void femas_af_query_on_rsp_qry_investor(void* arg, CUstpFtdcRspUserInvestorField *pInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_query_on_rsp_qry_instrument(void* arg, CUstpFtdcRspInstrumentField *pInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_query_on_rsp_qry_trading_account(void* arg, CUstpFtdcRspInvestorAccountField *pTradingAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_query_on_rsp_qry_investor_position(void* arg, CUstpFtdcRspInvestorPositionField *pInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void femas_af_trader_on_rtn_instrument_status(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus);
static void femas_af_trader_on_rsp_ds_user_certification(void* arg, CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static femas_af_trader_api_cb* femas_af_trader_api_cb_get();

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_femas_af_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_femas_af_get_trading_day,
    trader_trader_api_femas_af_get_max_order_local_id,
    trader_trader_api_femas_af_start,
    trader_trader_api_femas_af_stop,
    trader_trader_api_femas_af_login,
    trader_trader_api_femas_af_logout,
    trader_trader_api_femas_af_order_insert,
    trader_trader_api_femas_af_order_action,
    trader_trader_api_femas_af_qry_instrument,
    trader_trader_api_femas_af_qry_user_investor,
    trader_trader_api_femas_af_qry_investor_position,
    trader_trader_api_femas_af_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_femas_af_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  strcpy(tradingday, pTraderApi->GetTradingDay());
  return 0;
}

int trader_trader_api_femas_af_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  *local_id = self->userLocalId;
  return 0;
}


void trader_trader_api_femas_af_start(trader_trader_api* self)
{
  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi(self->pWorkspace);
  CFemasAFTraderHandler* pTraderHandler = new CFemasAFTraderHandler(femas_af_trader_api_cb_get(), (void*)self);
  
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)malloc(sizeof(trader_trader_api_femas_af));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;

  self->pUserApi = (void*)pImp;
  
  self->timeCondition = USTP_FTDC_TC_GFD;
  self->hedgeFlag = USTP_FTDC_CHF_Speculation;
  
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);

  pTraderApi->SubscribePublicTopic(USTP_TERT_RESUME);
  
  // ����
  pTraderApi->RegisterSpi(pTraderHandler);
  
#ifndef FEMAS20
  pTraderApi->RegisterFront(self->pAddress);
#else
  char* pSavePtr;
  char sAddress[256];
  char* pQueryFrontAddress;
  char* pTcpFrontAddress;

  do{
    strncpy(sAddress, self->pAddress, sizeof(sAddress));
    
    pQueryFrontAddress = strtok_r(sAddress, "|", &pSavePtr);
    CMN_ASSERT (pQueryFrontAddress);
        
    pTcpFrontAddress = strtok_r(NULL, "|", &pSavePtr);
    CMN_ASSERT (pTcpFrontAddress);
    pTraderApi->RegisterFront(pTcpFrontAddress);
    pTraderApi->RegisterQryFront(pQueryFrontAddress);

  }while(0);
#endif
  // ���ӽ��׷�����
  pTraderApi->Init();

  return;

}

void trader_trader_api_femas_af_stop(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  CFemasAFTraderHandler* pTraderHandler = (CFemasAFTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_femas_af_login(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  CUstpFtdcDSUserInfoField reqDSUserInfoField;
  
  memset(&reqDSUserInfoField, 0, sizeof(reqDSUserInfoField));
  strncpy(reqDSUserInfoField.AppID, self->pAppID, sizeof(reqDSUserInfoField.AppID));
  strncpy(reqDSUserInfoField.AuthCode, self->pAuthCode, sizeof(reqDSUserInfoField.AuthCode));
  reqDSUserInfoField.EncryptType = '1';

  CMN_DEBUG("reqDSUserInfoField.AppID[%s]\n", reqDSUserInfoField.AppID);
  CMN_DEBUG("reqDSUserInfoField.AuthCode[%s]\n", reqDSUserInfoField.AuthCode);

  pTraderApi->ReqDSUserCertification(&reqDSUserInfoField, pImp->nTraderRequestID++);
}

void trader_trader_api_femas_af_logout(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_femas_af_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///���͹�˾����
	strcpy(inputOrderField.BrokerID, self->pBrokerID);
	///����������
	if(('I' == inst[0]) || ('T' == inst[0])){
  	strcpy(inputOrderField.ExchangeID, "CFFEX");
  }else{
  	strcpy(inputOrderField.ExchangeID, "SHFE");
  }
	///Ͷ���ߴ���
	strcpy(inputOrderField.InvestorID, pImp->sInvestorID);
	///��Լ����
	strcpy(inputOrderField.InstrumentID, inst);
	///��������
	strcpy(inputOrderField.UserOrderLocalID, local_id);
	///�û�����
	strcpy(inputOrderField.UserID, self->pUser);
	///�����۸�����
	inputOrderField.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
	///��������
	inputOrderField.Direction = buy_sell;
	///��Ͽ�ƽ��־
	inputOrderField.OffsetFlag = open_close;
	///���Ͷ���ױ���־
	inputOrderField.HedgeFlag = self->hedgeFlag;
	///�۸�
	inputOrderField.LimitPrice = price;
	///����
	inputOrderField.Volume = vol;
	///��Ч������
	inputOrderField.TimeCondition = self->timeCondition;
	///�ɽ�������
	inputOrderField.VolumeCondition = USTP_FTDC_VC_AV;
	///��С�ɽ���
	inputOrderField.MinVolume = 1;
	///ǿƽԭ��
	inputOrderField.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;
	///�Զ������־
	inputOrderField.IsAutoSuspend = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  
}

int trader_trader_api_femas_af_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

	///���͹�˾����
	strcpy(inputOrderActionField.BrokerID, self->pBrokerID);
	///Ͷ���ߴ���
	strcpy(inputOrderActionField.InvestorID, pImp->sInvestorID);
	///������������
	strcpy(inputOrderActionField.UserOrderActionLocalID, local_id);
	///��������
	strcpy(inputOrderActionField.UserOrderLocalID, org_local_id);
	///ǰ�ñ��
	//inputOrderActionField.FrontID = front_id;
	///�Ự���
	//inputOrderActionField.SessionID = session_id;
	///����������
	strncpy(inputOrderActionField.ExchangeID, exchange_id, sizeof(inputOrderActionField.ExchangeID));
	///������־
	inputOrderActionField.ActionFlag = USTP_FTDC_AF_Delete;
	///�û�����
	strcpy(inputOrderActionField.UserID, self->pUser);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);

}

 
int trader_trader_api_femas_af_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///��Լ����
	//TUstpFtdcInstrumentIDType	InstrumentID;
	///����������
	//TUstpFtdcExchangeIDType	ExchangeID;
	strcpy(qryInstrumentField.ExchangeID, "");

  pTraderApi->ReqQryInstrument(&qryInstrumentField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_af_qry_user_investor(trader_trader_api* self)
{
  CMN_DEBUG("Enter!\n");
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryUserInvestorField qryInvestorField;

  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
	///���͹�˾����
	strcpy(qryInvestorField.BrokerID, self->pBrokerID);
	///�û�����
	strcpy(qryInvestorField.UserID, self->pUser);  
  
  pTraderApi->ReqQryUserInvestor(&qryInvestorField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_af_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryInvestorPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

	///���͹�˾����
	strcpy(qryInvestorPositionField.BrokerID, self->pBrokerID);
	///�û�����
	strcpy(qryInvestorPositionField.UserID, self->pUser);  
	///Ͷ���ߴ���
	strcpy(qryInvestorPositionField.InvestorID, pImp->sInvestorID);
	///��Լ����
	//TUstpFtdcInstrumentIDType	InstrumentID;

  pTraderApi->ReqQryInvestorPosition(&qryInvestorPositionField, pImp->nTraderRequestID++);

}

int trader_trader_api_femas_af_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  
  CUstpFtdcQryInvestorAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///���͹�˾����
	strcpy(qryTradingAccountField.BrokerID, self->pBrokerID);
	///�û�����
	strcpy(qryTradingAccountField.UserID, self->pUser);  
	///Ͷ���ߴ���
	strcpy(qryTradingAccountField.InvestorID, pImp->sInvestorID);

  pTraderApi->ReqQryInvestorAccount(&qryTradingAccountField, pImp->nTraderRequestID++);

}

void femas_af_trader_on_front_connected(void* arg)
{
  CMN_INFO("Enter\n");

  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_connected(self);
}

void femas_af_trader_on_qry_front_connected(void* arg)
{
  CMN_INFO("Enter\n");
}

void femas_af_trader_on_front_disconnected(void* arg, int nReason)
{
  CMN_INFO("Enter[%d]\n", nReason);
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_disconnected(self, nReason);
}


void femas_af_trader_on_qry_front_disconnected(void* arg, int nReason)
{
  CMN_INFO("Enter[%d]\n", nReason);
}

void femas_af_trader_on_rsp_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  if(pRspUserLogin){  
    CMN_INFO("pRspUserLogin->TradingDay[%s]\n"
      "pRspUserLogin->BrokerID[%s]\n"
      "pRspUserLogin->UserID[%s]\n"
      "pRspUserLogin->LoginTime[%s]\n"
      "pRspUserLogin->MaxOrderLocalID[%s]\n"
      "pRspUserLogin->TradingSystemName[%s]\n"
      "pRspUserLogin->DataCenterID[%d]\n"
      "pRspUserLogin->PrivateFlowSize[%d]\n"
      "pRspUserLogin->UserFlowSize[%d]\n",
      pRspUserLogin->TradingDay,
      pRspUserLogin->BrokerID,
      pRspUserLogin->UserID,
      pRspUserLogin->LoginTime,
      pRspUserLogin->MaxOrderLocalID,
      pRspUserLogin->TradingSystemName,
      pRspUserLogin->DataCenterID,
      pRspUserLogin->PrivateFlowSize,
      pRspUserLogin->UserFlowSize
      );

    // ��ȡ��󱨵���
    trader_trader_api_get_max_order_local_id(self, pRspUserLogin->MaxOrderLocalID);
  }

  if(errNo){
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  }
}

void femas_af_trader_on_rsp_query_user_login(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    CMN_INFO("pRspInfo->ErrorID[%d]"
      "pRspInfo->ErrorMsg[%s]\n"
      , pRspInfo->ErrorID
      , pRspInfo->ErrorMsg
      );
  }
  if(pRspUserLogin){  
    CMN_INFO("pRspUserLogin->TradingDay[%s]\n"
      "pRspUserLogin->BrokerID[%s]\n"
      "pRspUserLogin->UserID[%s]\n"
      "pRspUserLogin->LoginTime[%s]\n"
      "pRspUserLogin->MaxOrderLocalID[%s]\n"
      "pRspUserLogin->TradingSystemName[%s]\n"
      "pRspUserLogin->DataCenterID[%d]\n"
      "pRspUserLogin->PrivateFlowSize[%d]\n"
      "pRspUserLogin->UserFlowSize[%d]\n",
      pRspUserLogin->TradingDay,
      pRspUserLogin->BrokerID,
      pRspUserLogin->UserID,
      pRspUserLogin->LoginTime,
      pRspUserLogin->MaxOrderLocalID,
      pRspUserLogin->TradingSystemName,
      pRspUserLogin->DataCenterID,
      pRspUserLogin->PrivateFlowSize,
      pRspUserLogin->UserFlowSize
      );
  }
  if(!pRspInfo->ErrorID){    
    CUstpFtdcQryUserInvestorField qryInvestorField;
    
    memset(&qryInvestorField, 0, sizeof(qryInvestorField));
    ///���͹�˾����
    strcpy(qryInvestorField.BrokerID, self->pBrokerID);
    ///�û�����
    strcpy(qryInvestorField.UserID, self->pUser);  
    
    pTraderApi->ReqQryUserInvestor(&qryInvestorField, pImp->nTraderRequestID++);
  }

  return ;
}


void femas_af_trader_on_rsp_user_logout(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pRspUserLogout){
    CMN_DEBUG("pRspUserLogout->BrokerID[%s]\n"
      "pRspUserLogout->UserID[%s]\n",
      pRspUserLogout->BrokerID,
      pRspUserLogout->UserID
      );
  }
  
  trader_trader_api_on_rsp_user_logout(self, errNo, errMsg);
}

void femas_af_trader_on_rsp_error(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);
}


void femas_af_trader_on_rsp_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  trader_trader_api_on_rsp_order_insert(self, errNo, errMsg);
  
}

void femas_af_trader_on_rsp_order_action(void* arg, CUstpFtdcOrderActionField *pInputOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  trader_trader_api_on_rsp_order_action(self, errNo, errMsg);
}

void femas_af_trader_on_rtn_order(void* arg, CUstpFtdcOrderField *pOrder)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
	///����������
  strcpy(traderOrder.ExchangeID, pOrder->ExchangeID);
	///ϵͳ�������
  strcpy(traderOrder.OrderSysID, pOrder->OrderSysID);
  // ��Լ����
  strcpy(traderOrder.InstrumentID, pOrder->InstrumentID);
  // ���ر������
  strcpy(traderOrder.UserOrderLocalID, pOrder->UserOrderLocalID);
  // ����
  traderOrder.Direction = pOrder->Direction;
  // ��ƽ
  traderOrder.OffsetFlag = pOrder->OffsetFlag;
  ///Ͷ���ױ���־
  traderOrder.HedgeFlag = pOrder->HedgeFlag;
  // �����۸�
  traderOrder.LimitPrice = pOrder->LimitPrice;
  // ��������
  traderOrder.VolumeOriginal = pOrder->Volume;
  // �ɽ�����
  traderOrder.VolumeTraded = pOrder->VolumeTraded;
  // ����״̬
  traderOrder.OrderStatus = pOrder->OrderStatus;
  ///����ʱ��
  strcpy(traderOrder.InsertTime, pOrder->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
  
}

void femas_af_trader_on_rtn_trade(void* arg, CUstpFtdcTradeField *pTrade)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///��Լ����
  strcpy(traderTrade.InstrumentID, pTrade->InstrumentID);
  ///���ر������
  strcpy(traderTrade.UserOrderLocalID, pTrade->UserOrderLocalID);
  ///������
  strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///�ɽ�ʱ��
  strcpy(traderTrade.TradeTime, pTrade->TradeTime);
  ///��������
  traderTrade.Direction = pTrade->Direction;
  ///��ƽ��־
  traderTrade.OffsetFlag = pTrade->OffsetFlag;
  ///�ɽ��۸�
  traderTrade.TradePrice = pTrade->TradePrice;
  ///�ɽ�����
  traderTrade.TradeVolume = pTrade->TradeVolume;
  //�ɽ����
  strcpy(traderTrade.TradeID, pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void femas_af_trader_on_err_rtn_order_insert(void* arg, CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
    
    CMN_WARN("����ʧ��\n");
    CMN_WARN("pRspInfo->ErrorID[%d]\n"
      "pRspInfo->ErrorMsg[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg
      );
  }
  
  trader_order traderOrder;

  if(pInputOrder){
    CMN_INFO("pInputOrder->ExchangeID[%s]\n"
      "pInputOrder->OrderSysID[%s]\n"
      "pInputOrder->UserID[%s]\n"
      "pInputOrder->InstrumentID[%s]\n"
      "pInputOrder->UserOrderLocalID[%s]\n",
      pInputOrder->ExchangeID,
      pInputOrder->OrderSysID,
      pInputOrder->UserID,
      pInputOrder->InstrumentID,
      pInputOrder->UserOrderLocalID
      );
      
    memset(&traderOrder, 0, sizeof(traderOrder));
    ///����������
    strcpy(traderOrder.ExchangeID, pInputOrder->ExchangeID);
    ///ϵͳ�������
    strcpy(traderOrder.OrderSysID, pInputOrder->OrderSysID);
    // ��Լ����
    strcpy(traderOrder.InstrumentID, pInputOrder->InstrumentID);
    // ���ر������
    strcpy(traderOrder.UserOrderLocalID, pInputOrder->UserOrderLocalID);
    // ����
    traderOrder.Direction = pInputOrder->Direction;
    // ��ƽ
    traderOrder.OffsetFlag = pInputOrder->OffsetFlag;
    ///Ͷ���ױ���־
    traderOrder.HedgeFlag = pInputOrder->HedgeFlag;
    // �����۸�
    traderOrder.LimitPrice = pInputOrder->LimitPrice;
    // ��������
    traderOrder.VolumeOriginal = pInputOrder->Volume;
    // �ɽ�����
    traderOrder.VolumeTraded = 0;
    // ����״̬
    traderOrder.OrderStatus = TRADER_ORDER_OS_CANCELED;

    trader_trader_api_on_rtn_order(self, &traderOrder);
  }
  
  trader_trader_api_on_err_rtn_order_insert(self, errNo, errMsg);
}

void femas_af_trader_on_err_rtn_order_action(void* arg, CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(pOrderAction){
    CMN_DEBUG("pOrderAction->ExchangeID[%s]\n"
      "pOrderAction->OrderSysID[%s]\n"
      "pOrderAction->InvestorID[%s]\n"
      "pOrderAction->UserID[%s]\n"
      "pOrderAction->UserOrderActionLocalID[%s]\n"
      "pOrderAction->UserOrderLocalID[%s]\n",
      pOrderAction->ExchangeID,
      pOrderAction->OrderSysID,
      pOrderAction->InvestorID,
      pOrderAction->UserID,
      pOrderAction->UserOrderActionLocalID,
      pOrderAction->UserOrderLocalID
      );
  }
  trader_trader_api_on_err_rtn_order_action(self, errNo, errMsg);
}

femas_af_trader_api_cb* femas_af_trader_api_cb_get()
{
  static femas_af_trader_api_cb femas_af_trader_api_cb_st = {
    femas_af_trader_on_front_connected,
    femas_af_trader_on_qry_front_connected,
    femas_af_trader_on_front_disconnected,
    femas_af_trader_on_qry_front_disconnected,
    femas_af_trader_on_rsp_error,
    femas_af_trader_on_rsp_user_login,
    femas_af_trader_on_rsp_query_user_login,
    femas_af_trader_on_rsp_user_logout,
    femas_af_trader_on_rsp_order_insert,
    femas_af_trader_on_rsp_order_action,
    femas_af_trader_on_rtn_trade,
    femas_af_trader_on_rtn_order,
    femas_af_trader_on_err_rtn_order_insert,
    femas_af_trader_on_err_rtn_order_action,
    femas_af_query_on_rsp_qry_investor,
    femas_af_query_on_rsp_qry_instrument,
    femas_af_trader_on_rtn_instrument_status,
    femas_af_query_on_rsp_qry_trading_account,
    femas_af_query_on_rsp_qry_investor_position,
    femas_af_trader_on_rsp_ds_user_certification
  };

  return &femas_af_trader_api_cb_st;
}


void femas_af_query_on_rsp_qry_investor(void* arg, CUstpFtdcRspUserInvestorField *pInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  int errNo = 0;
  char* errMsg = NULL;
  char* investor = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestor) {
    CMN_INFO("pInvestor->UserID[%s]pInvestor->InvestorID[%s]\n"
      ,pInvestor->UserID
      ,pInvestor->InvestorID);
    investor = pInvestor->InvestorID;
    strncpy(pImp->sInvestorID, pInvestor->InvestorID, sizeof(pImp->sInvestorID));
  }
  
  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  return ;
}

void femas_af_query_on_rsp_qry_instrument(void* arg, CUstpFtdcRspInstrumentField *pInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInstrument) {
    strcpy(traderInstrument.InstrumentID, pInstrument->InstrumentID);
    strcpy(traderInstrument.ExchangeID, pInstrument->ExchangeID);
    traderInstrument.VolumeMultiple = pInstrument->VolumeMultiple;
    traderInstrument.PriceTick = pInstrument->PriceTick;

    CMN_DEBUG(
      "pInstrument->ExchangeID[%s]\n"
      "pInstrument->ProductID[%s]\n"
      "pInstrument->ProductName[%s]\n"
      "pInstrument->InstrumentID[%s]\n"
      "pInstrument->InstrumentName[%s]\n"
      "pInstrument->DeliveryYear[%d]\n"
      "pInstrument->DeliveryMonth[%d]\n"
      "pInstrument->MaxLimitOrderVolume[%d]\n"
      "pInstrument->MinLimitOrderVolume[%d]\n"
      "pInstrument->MaxMarketOrderVolume[%d]\n"
      "pInstrument->MinMarketOrderVolume[%d]\n"
      "pInstrument->VolumeMultiple[%d]\n"
      "pInstrument->PriceTick[%lf]\n"
      "pInstrument->Currency[%c]\n"
      "pInstrument->LongPosLimit[%d]\n"
      "pInstrument->ShortPosLimit[%d]\n"
      "pInstrument->LowerLimitPrice[%lf]\n"
      "pInstrument->UpperLimitPrice[%lf]\n"
      "pInstrument->PreSettlementPrice[%lf]\n"
      "pInstrument->InstrumentStatus[%c]\n"
      "pInstrument->CreateDate[%s]\n"
      "pInstrument->OpenDate[%s]\n"
      "pInstrument->StartDelivDate[%s]\n"
      "pInstrument->EndDelivDate[%s]\n"
      "pInstrument->BasisPrice[%lf]\n"
      "pInstrument->IsTrading[%d]\n"
      "pInstrument->UnderlyingInstrID[%s]\n"
      "pInstrument->UnderlyingMultiple[%d]\n"
      "pInstrument->PositionType[%c]\n"
      "pInstrument->StrikePrice[%lf]\n"
      "pInstrument->OptionsType[%c]\n",
      pInstrument->ExchangeID,
      pInstrument->ProductID,
      pInstrument->ProductName,
      pInstrument->InstrumentID,
      pInstrument->InstrumentName,
      pInstrument->DeliveryYear,
      pInstrument->DeliveryMonth,
      pInstrument->MaxLimitOrderVolume,
      pInstrument->MinLimitOrderVolume,
      pInstrument->MaxMarketOrderVolume,
      pInstrument->MinMarketOrderVolume,
      pInstrument->VolumeMultiple,
      pInstrument->PriceTick,
      pInstrument->Currency,
      pInstrument->LongPosLimit,
      pInstrument->ShortPosLimit,
      pInstrument->LowerLimitPrice,
      pInstrument->UpperLimitPrice,
      pInstrument->PreSettlementPrice,
      pInstrument->InstrumentStatus,
      pInstrument->CreateDate,
      pInstrument->OpenDate,
      pInstrument->StartDelivDate,
      pInstrument->EndDelivDate,
      pInstrument->BasisPrice,
      pInstrument->IsTrading,
      pInstrument->UnderlyingInstrID,
      pInstrument->UnderlyingMultiple,
      pInstrument->PositionType,
      pInstrument->StrikePrice,
      pInstrument->OptionsType
    );
  }

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);

  
}

void femas_af_query_on_rsp_qry_trading_account(void* arg, CUstpFtdcRspInvestorAccountField *pTradingAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pTradingAccount) {
    strcpy(traderAccount.AccountID, pTradingAccount->AccountID);
    CMN_INFO(
      "pTradingAccount->BrokerID[%s]\n"
      "pTradingAccount->InvestorID[%s]\n"
      "pTradingAccount->AccountID[%s]\n"
      "pTradingAccount->PreBalance[%lf]\n"
      "pTradingAccount->Deposit[%lf]\n"
      "pTradingAccount->Withdraw[%lf]\n"
      "pTradingAccount->FrozenMargin[%lf]\n"
      "pTradingAccount->FrozenFee[%lf]\n"
      "pTradingAccount->FrozenPremium[%lf]\n"
      "pTradingAccount->Fee[%lf]\n"
      "pTradingAccount->CloseProfit[%lf]\n"
      "pTradingAccount->PositionProfit[%lf]\n"
      "pTradingAccount->Available[%lf]\n"
      "pTradingAccount->LongFrozenMargin[%lf]\n"
      "pTradingAccount->ShortFrozenMargin[%lf]\n"
      "pTradingAccount->LongMargin[%lf]\n"
      "pTradingAccount->ShortMargin[%lf]\n"
      "pTradingAccount->ReleaseMargin[%lf]\n"
      "pTradingAccount->DynamicRights[%lf]\n"
      "pTradingAccount->TodayInOut[%lf]\n"
      "pTradingAccount->Margin[%lf]\n"
      "pTradingAccount->Premium[%lf]\n"
      "pTradingAccount->Risk[%lf]\n",
      pTradingAccount->BrokerID,
      pTradingAccount->InvestorID,
      pTradingAccount->AccountID,
      pTradingAccount->PreBalance,
      pTradingAccount->Deposit,
      pTradingAccount->Withdraw,
      pTradingAccount->FrozenMargin,
      pTradingAccount->FrozenFee,
      pTradingAccount->FrozenPremium,
      pTradingAccount->Fee,
      pTradingAccount->CloseProfit,
      pTradingAccount->PositionProfit,
      pTradingAccount->Available,
      pTradingAccount->LongFrozenMargin,
      pTradingAccount->ShortFrozenMargin,
      pTradingAccount->LongMargin,
      pTradingAccount->ShortMargin,
      pTradingAccount->ReleaseMargin,
      pTradingAccount->DynamicRights,
      pTradingAccount->TodayInOut,
      pTradingAccount->Margin,
      pTradingAccount->Premium,
      pTradingAccount->Risk
    );
  }

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, errNo, errMsg, bIsLast);
}

void femas_af_query_on_rsp_qry_investor_position(void* arg, CUstpFtdcRspInvestorPositionField *pInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestorPosition) {
    CMN_DEBUG(
      "pInvestorPosition->InvestorID[%s]\n"
      "pInvestorPosition->BrokerID[%s]\n"
      "pInvestorPosition->ExchangeID[%s]\n"
      "pInvestorPosition->ClientID[%s]\n"
      "pInvestorPosition->InstrumentID[%s]\n"
      "pInvestorPosition->Direction[%c]\n"
      "pInvestorPosition->HedgeFlag[%c]\n"
      "pInvestorPosition->UsedMargin[%lf]\n"
      "pInvestorPosition->Position[%d]\n"
      "pInvestorPosition->PositionCost[%lf]\n"
      "pInvestorPosition->YdPosition[%d]\n"
      "pInvestorPosition->YdPositionCost[%lf]\n"
      "pInvestorPosition->FrozenMargin[%lf]\n"
      "pInvestorPosition->FrozenPosition[%d]\n"
      "pInvestorPosition->FrozenClosing[%d]\n"
      "pInvestorPosition->FrozenPremium[%lf]\n"
      "pInvestorPosition->LastTradeID[%s]\n"
      "pInvestorPosition->LastOrderLocalID[%s]\n"
      "pInvestorPosition->Currency[%s]\n",
      pInvestorPosition->InvestorID,
      pInvestorPosition->BrokerID,
      pInvestorPosition->ExchangeID,
      pInvestorPosition->ClientID,
      pInvestorPosition->InstrumentID,
      pInvestorPosition->Direction,
      pInvestorPosition->HedgeFlag,
      pInvestorPosition->UsedMargin,
      pInvestorPosition->Position,
      pInvestorPosition->PositionCost,
      pInvestorPosition->YdPosition,
      pInvestorPosition->YdPositionCost,
      pInvestorPosition->FrozenMargin,
      pInvestorPosition->FrozenPosition,
      pInvestorPosition->FrozenClosing,
      pInvestorPosition->FrozenPremium,
      pInvestorPosition->LastTradeID,
      pInvestorPosition->LastOrderLocalID,
      pInvestorPosition->Currency
    );
    
    strcpy(traderPosition.InstrumentID, pInvestorPosition->InstrumentID);
    traderPosition.PositionDate = '1'; //
    traderPosition.PosiDirection = TRADER_POSITION_LONG;
    if(USTP_FTDC_D_Sell == pInvestorPosition->Direction){
      traderPosition.PosiDirection = TRADER_POSITION_SHORT;
    }
    traderPosition.YdPosition = pInvestorPosition->YdPosition;
    traderPosition.TodayPosition = pInvestorPosition->Position - pInvestorPosition->YdPosition;
    traderPosition.Position = pInvestorPosition->Position;
    traderPosition.LongFrozen = pInvestorPosition->FrozenPosition;
  }

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);
  return ;
}

void femas_af_trader_on_rtn_instrument_status(void* arg, CUstpFtdcInstrumentStatusField *pInstrumentStatus)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  if(pInstrumentStatus){
    if(0 == memcmp(pInstrumentStatus->InstrumentID, "IO", 2)){
      return ;
    }
    CMN_DEBUG(
      "pInstrumentStatus->ExchangeID[%s]\n"
      "pInstrumentStatus->InstrumentID[%s]\n"
      "pInstrumentStatus->InstrumentStatus[%c]\n",
      pInstrumentStatus->ExchangeID,
      pInstrumentStatus->InstrumentID,
      pInstrumentStatus->InstrumentStatus
      );
    instrument_status oInstrumentStatus;
    strncpy(oInstrumentStatus.InstrumentID, pInstrumentStatus->InstrumentID, sizeof(oInstrumentStatus.InstrumentID));
    oInstrumentStatus.InstrumentStatus = pInstrumentStatus->InstrumentStatus;
    
    trader_trader_api_on_rtn_instrument_status(self, &oInstrumentStatus);
  }

  return ;
}

void femas_af_trader_on_rsp_ds_user_certification(void* arg, CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_femas_af* pImp = (trader_trader_api_femas_af*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(pDSUserCertRspData) {
    CMN_DEBUG("pDSUserCertRspData->AppID[%s]\n",
      pDSUserCertRspData->AppID);
  }

  if(errNo){
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
    return ;
  }
  
  CUstpFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, self->pBrokerID);
  strcpy(reqUserLoginField.UserID, self->pUser);
  strcpy(reqUserLoginField.Password, self->pPwd);
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, pImp->nTraderRequestID++);

}

