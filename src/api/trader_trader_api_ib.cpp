
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "IBTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_ib.h"
#include "cmn_log.h"

static int trader_trader_api_ib_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_ib_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_ib_start(trader_trader_api* self);
static void trader_trader_api_ib_stop(trader_trader_api* self);
static void trader_trader_api_ib_login(trader_trader_api* self);
static void trader_trader_api_ib_logout(trader_trader_api* self);
  
static int trader_trader_api_ib_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_ib_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_ib_qry_instrument(trader_trader_api* self);
static int trader_trader_api_ib_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_ib_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_ib_qry_trading_account(trader_trader_api* self);

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_ib_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_app_id,
    trader_trader_api_set_auth_code,
    trader_trader_api_set_param,
    trader_trader_api_ib_get_trading_day,
    trader_trader_api_ib_get_max_order_local_id,
    trader_trader_api_ib_start,
    trader_trader_api_ib_stop,
    trader_trader_api_ib_login,
    trader_trader_api_ib_logout,
    trader_trader_api_ib_order_insert,
    trader_trader_api_ib_order_action,
    trader_trader_api_ib_qry_instrument,
    trader_trader_api_ib_qry_user_investor,
    trader_trader_api_ib_qry_investor_position,
    trader_trader_api_ib_qry_trading_account
  };

  return &trader_trader_api_method_st;
}

int trader_trader_api_ib_get_trading_day(trader_trader_api* self, char* tradingday)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  strcpy(tradingday, pTraderApi->GetTradingDay());
  return 0;
}

int trader_trader_api_ib_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  *local_id = self->userLocalId;
  return 0;
}


void trader_trader_api_ib_start(trader_trader_api* self)
{
  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi(self->pWorkspace);
  CFemasTraderHandler* pTraderHandler = new CFemasTraderHandler(ib_trader_api_cb_get(), (void*)self);
  
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)malloc(sizeof(trader_trader_api_ib));
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
  pTraderApi->RegisterFront(self->pAddress);
  
  // ���ӽ��׷�����
  pTraderApi->Init();

  return;

}

void trader_trader_api_ib_stop(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;
  CFemasTraderHandler* pTraderHandler = (CFemasTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_ib_login(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
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

void trader_trader_api_ib_logout(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  return;
}

  
int trader_trader_api_ib_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///���͹�˾����
	strcpy(inputOrderField.BrokerID, self->pBrokerID);
	///����������
	strcpy(inputOrderField.ExchangeID, "CFFEX");
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

int trader_trader_api_ib_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
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
	strcpy(inputOrderActionField.ExchangeID, "CFFEX");
	///������־
	inputOrderActionField.ActionFlag = USTP_FTDC_AF_Delete;
	///�û�����
	strcpy(inputOrderActionField.UserID, self->pUser);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);

}

 
int trader_trader_api_ib_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///��Լ����
	//TUstpFtdcInstrumentIDType	InstrumentID;
	///����������
	//TUstpFtdcExchangeIDType	ExchangeID;
	strcpy(qryInstrumentField.ExchangeID, "CFFEX");

  pTraderApi->ReqQryInstrument(&qryInstrumentField, pImp->nTraderRequestID++);

}

int trader_trader_api_ib_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)pImp->pTraderApi;

  CUstpFtdcQryUserInvestorField qryInvestorField;

  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
	///���͹�˾����
	strcpy(qryInvestorField.BrokerID, self->pBrokerID);
	///�û�����
	strcpy(qryInvestorField.UserID, self->pUser);  
  
  pTraderApi->ReqQryUserInvestor(&qryInvestorField, pImp->nTraderRequestID++);

}

int trader_trader_api_ib_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
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

int trader_trader_api_ib_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
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

