

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "CtpTraderHandler.h"
#include "CtpMduserHandler.h"

#define CMN_DEBUG(...) do {printf("[%s]", __FUNCTION__);printf(__VA_ARGS__);}while(0)
#define CMN_WARN(...) printf(__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static ctp_trader_api_cb* ctp_api_trader_api_cb_get();
static ctp_mduser_api_cb* ctp_api_mduser_api_cb_get();


static void ctp_api_trader_on_front_connected(void* arg);
static void ctp_api_trader_on_front_disconnected(void* arg, int nReason);
static void ctp_api_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
static void ctp_api_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
static void ctp_api_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade);
static void ctp_api_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder);
static void ctp_api_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
static void ctp_api_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
static void ctp_api_trader_on_rsp_qry_user_investor(void* arg, CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus);
static void ctp_api_mduser_on_front_connected(void* arg);
static void ctp_api_mduser_on_front_disconnected(void* arg, int nReason);
static void ctp_api_mduser_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_mduser_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_mduser_on_rtn_depth_market_data(void* arg, CThostFtdcDepthMarketDataField *pMarketData);
static void ctp_api_mduser_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_mduser_on_rsp_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void ctp_api_mduser_on_rsp_un_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


#ifdef __cplusplus
}
#endif

static void StartTraderTest();

static char appId[11];
static char userId[13];
static char pwd[40];
static char orderRef[13];
static char mduserFrontAddress[100];
static char traderFrontAddress[100];
static int frontId;
static int sessionId;
static int requestId;

static void TraderOrderInsert(CThostFtdcTraderApi* api)
{
	CThostFtdcInputOrderField req;

  char sInstrumentID[31];
  char cBuySell[1];
  char cOpenClose[1];
  double dPrice;

  printf("input InstrumentID:");
  scanf("%s", sInstrumentID);

  printf("input cBuySell:");
  scanf("%s", cBuySell);

  printf("input cOpenClose:");
  scanf("%s", cOpenClose);

  printf("input dPrice:");
  scanf("%lf", &dPrice);

	memset(&req, 0, sizeof(req));	
	strcpy(req.BrokerID, appId);  //应用单元代码	
	strcpy(req.InvestorID, userId); //投资者代码	
	strcpy(req.InstrumentID, sInstrumentID); //合约代码	
	sprintf(req.OrderRef, "%07d", requestId);  //报单引用
	int vol = 1;
  
  req.LimitPrice = dPrice;	//价格
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
  req.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
  
  req.Direction = cBuySell[0];  //买卖方向	
	req.CombOffsetFlag[0] = cOpenClose[0]; //组合开平标志:开仓
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志	
	req.VolumeTotalOriginal = vol;	///数量		
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	req.MinVolume = 1;	//最小成交量:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	
  //TThostFtdcPriceType	StopPrice;  //止损价
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	req.UserForceClose = 0;   //用户强评标志:否

	int ret = api->ReqOrderInsert(&req, requestId);

  CMN_DEBUG("\n"
    "req.BrokerID=[%s]\n"
    "req.InvestorID=[%s]\n"
    "req.InstrumentID=[%s]\n"
    "req.OrderRef=[%s]\n"
    "req.UserID=[%s]\n"
    "req.OrderPriceType=[%c]\n"
    "req.Direction=[%c]\n"
    "req.CombOffsetFlag=[%c]\n"
    "req.CombHedgeFlag=[%c]\n"
    "req.LimitPrice=[%10.2lf]\n"
    "req.VolumeTotalOriginal=[%d]\n"
    "req.TimeCondition=[%c]\n"
    "req.VolumeCondition=[%c]\n"
    "req.MinVolume=[%d]\n"
    "req.ForceCloseReason=[%c]\n"
    "req.IsAutoSuspend=[%d]\n"
    "req.UserForceClose=[%d]\n",
    req.BrokerID,
    req.InvestorID,
    req.InstrumentID,
    req.OrderRef,
    req.UserID,
    req.OrderPriceType,
    req.Direction,
    req.CombOffsetFlag[0],
    req.CombHedgeFlag[0],
    req.LimitPrice,
    req.VolumeTotalOriginal,
    req.TimeCondition,
    req.VolumeCondition,
    req.MinVolume,
    req.ForceCloseReason,
    req.IsAutoSuspend,
    req.UserForceClose);

  requestId++;

}

static void TraderOrderAction(CThostFtdcTraderApi* api)
{
	CThostFtdcInputOrderActionField req;

  char sExchangeID[9];
  char sOrderSysID[21];
  char sInstrumentID[31];
  char sOrderRef[13];

  printf("input InstrumentID:");
  scanf("%s", sInstrumentID);
  
  printf("input sExchangeID:");
  scanf("%s", sExchangeID);
  
  printf("input sOrderSysID:");
  scanf("%s", sOrderSysID);

  printf("input sOrderRef:");
  scanf("%s", sOrderRef);
  

  
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);   //经纪公司代码	
	strcpy(req.InvestorID, userId); //投资者代码
	//strcpy(req.InstrumentID, sInstrumentID); //报单引用	
	//strcpy(req.OrderRef, orderRef); //报单引用	
	//req.FrontID = frontId;           //前置编号	
	//req.SessionID = sessionId;       //会话编号
  strcpy(req.ExchangeID, sExchangeID);
  sprintf(req.OrderSysID, "%12s", sOrderSysID);
	req.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志 

	int ret = api->ReqOrderAction(&req, requestId);

  
  CMN_DEBUG("\n"
    "OrderAction.OrderRef=[%s]\n"
    "OrderAction.BrokerID=[%s]\n"
    "OrderAction.InvestorID=[%s]\n"
    "OrderAction.OrderActionRef=[%d]\n"
    "OrderAction.ExchangeID=[%s]\n"
    "OrderAction.OrderSysID=[%s]\n"
    "OrderAction.ActionFlag=[%c]\n", 
    sOrderRef,
    req.BrokerID,
    req.InvestorID,
    req.OrderActionRef,
    req.ExchangeID,
    req.OrderSysID,
    req.ActionFlag);
  
  requestId++;

}

static void TraderQryInstrument(CThostFtdcTraderApi* api)
{
	CThostFtdcQryInstrumentField req;

	memset(&req, 0, sizeof(req));
  //strcpy(req.ExchangeID,"SHFE");
	int ret = api->ReqQryInstrument(&req, requestId);
  requestId++;

}

static void TraderQryUserInvestor(CThostFtdcTraderApi* api)
{
	CThostFtdcQryInvestorField req;

	memset(&req, 0, sizeof(req));

	int ret = api->ReqQryInvestor(&req, requestId);
  requestId++;

}

static void TraderQryInvestorPosition(CThostFtdcTraderApi* api)
{
	CThostFtdcQryInvestorPositionField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);  //应用单元代码	
	strcpy(req.InvestorID, userId); //投资者代码	
	int ret = api->ReqQryInvestorPosition(&req, requestId);
  requestId++;

}


void StartTraderTest()
{
  CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
  CCtpTraderHandler* pHandler = new CCtpTraderHandler(ctp_api_trader_api_cb_get(), (void*)pUserApi);
  
  //pUserApi->RegisterFront("tcp://180.166.65.114:41205");
  pUserApi->RegisterFront(traderFrontAddress);
  pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
  pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);
  pUserApi->RegisterSpi(pHandler);
  pUserApi->Init();

  int loop = 1;
  int choice = 0;
  while(loop){
    printf("input:\n");
    scanf("%d",&choice);
    printf("choice[%d]\n", choice);
    switch(choice) {
    case 0:
      loop = 0;
      break;
    case 1:
      TraderOrderInsert(pUserApi);
      break;
    case 2:
      TraderOrderAction(pUserApi);
      break;
    case 3:
      TraderQryInstrument(pUserApi);
      break;
    case 4:
      TraderQryUserInvestor(pUserApi);
      break;
    case 5:
      TraderQryInvestorPosition(pUserApi);
      break;
        
    }
  }

  
  pUserApi->RegisterSpi(NULL);
  pUserApi->Release();
  pUserApi = NULL;

  delete pHandler;
  pHandler = NULL;

  return ;

}

static void StartMduserTest();

static void MduserSubscribe( CThostFtdcMdApi* api)
{
  CThostFtdcMdApi* pUserApi = api;
  char* instrument[] = {"rb1601"};
  api->SubscribeMarketData(instrument,1);
}

static void MduserUnsubscribe( CThostFtdcMdApi* api)
{
  CThostFtdcMdApi* pUserApi = api;
  char* instrument[] = {"rb1601"};
  api->UnSubscribeMarketData(instrument,1);
}


void StartMduserTest()
{
  void* self;
  CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
  CCtpMduserHandler* pHandler = new CCtpMduserHandler(ctp_api_mduser_api_cb_get(), (void*)pUserApi);
  
  // connect
  pUserApi->RegisterSpi(pHandler);
  //pUserApi->SubscribeMarketDataTopic(self->TopicId, USTP_TERT_QUICK);
  pUserApi->RegisterFront(mduserFrontAddress);
  pUserApi->Init();

  int loop = 1;
  int choice = 0;
  while(loop){
    printf("input:\n");
    scanf("%d",&choice);
    printf("choice[%d]\n", choice);
    switch(choice) {
    case 0:
      loop = 0;
      break;
    case 1:
      MduserSubscribe(pUserApi);
      break;
    case 2:
      MduserUnsubscribe(pUserApi);
      break;
    }
  }

  pUserApi->RegisterSpi(NULL);

  pUserApi->Release();

  pUserApi = NULL;

  delete pHandler;

  pHandler = NULL;
  

  return ;

}


ctp_mduser_api_cb* ctp_api_mduser_api_cb_get()
{
	static ctp_mduser_api_cb ctp_mduser_api_cb_st = {
		ctp_api_mduser_on_front_connected,
		ctp_api_mduser_on_front_disconnected,
		ctp_api_mduser_on_rsp_user_login,
		ctp_api_mduser_on_rsp_user_logout,
		ctp_api_mduser_on_rtn_depth_market_data,
		ctp_api_mduser_on_rsp_error,
		ctp_api_mduser_on_rsp_sub_market_data,
		ctp_api_mduser_on_rsp_un_sub_market_data
	};
	return &ctp_mduser_api_cb_st;
}

ctp_trader_api_cb* ctp_api_trader_api_cb_get()
{
  static ctp_trader_api_cb ctp_trader_api_cb_st = {
    ctp_api_trader_on_front_connected,
    ctp_api_trader_on_front_disconnected,
    ctp_api_trader_on_rsp_error,
    ctp_api_trader_on_rsp_user_login,
    ctp_api_trader_on_rsp_user_logout,
    ctp_api_trader_on_rsp_order_insert,
    ctp_api_trader_on_rsp_order_action,
    ctp_api_trader_on_rtn_trade,
    ctp_api_trader_on_rtn_order,
    ctp_api_trader_on_err_rtn_order_insert,
    ctp_api_trader_on_err_rtn_order_action,
    ctp_api_trader_on_rsp_qry_user_investor,
    ctp_api_trader_on_rsp_qry_instrument,
    ctp_api_trader_on_rsp_settlement_info_confirm,
    ctp_api_trader_on_rsp_qry_investor_position,
    ctp_api_trader_on_rtn_instrument_status
  };

  return &ctp_trader_api_cb_st;

}


int main(int argc, char* argv[])
{
  strcpy(appId, "8060");
  //strcpy(userId, "99015469");
  //strcpy(pwd, "090537");
  //strcpy(mduserFrontAddress, "tcp://180.166.65.114:41213");
  //strcpy(traderFrontAddress, "tcp://180.166.65.114:41205");

  
  strcpy(userId, "9030000501");
  strcpy(pwd, "282038");
  strcpy(mduserFrontAddress, "tcp://180.166.65.119:41213");
  strcpy(traderFrontAddress, "tcp://180.166.65.120:41205");
  //strcpy(appId, "9999");
  //strcpy(userId, "033098");
  //strcpy(pwd, "282038");
  //strcpy(mduserFrontAddress, "tcp://180.168.146.187:10010");
  //strcpy(traderFrontAddress, "tcp://180.168.146.187:10000");
  
  requestId = 0;
  

  //StartMduserTest();
  StartTraderTest();
  

  exit(0);
}


void ctp_api_trader_on_front_connected(void* arg)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  
  CThostFtdcReqUserLoginField reqUserLogin;
  memset(&reqUserLogin,0,sizeof(CThostFtdcReqUserLoginField));
  strcpy(reqUserLogin.BrokerID, appId);
  strcpy(reqUserLogin.UserID, userId);
  strcpy(reqUserLogin.Password, pwd);
  strcpy(reqUserLogin.UserProductInfo,"Trader Login");
  pUserApi->ReqUserLogin(&reqUserLogin, requestId++);
  
  return;
}


void ctp_api_trader_on_front_disconnected(void* arg, int nReason)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
}


void ctp_api_trader_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
}


void ctp_api_trader_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  
  if(0 != pRspInfo->ErrorID){
    // 通知客户端登录失败
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }

  // 获取OrderId
  CMN_DEBUG(
    "FrontID[%d]\n"
    "SessionID[%d]\n"
    "MaxOrderLocalID[%s]\n",
    pRspUserLogin->FrontID,
    pRspUserLogin->SessionID,
    pRspUserLogin->MaxOrderRef);
  
  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(CThostFtdcSettlementInfoConfirmField));

  strcpy(req.BrokerID, appId);
  strcpy(req.InvestorID, userId);
  strcpy(req.ConfirmDate, pUserApi->GetTradingDay());
  strcpy(req.ConfirmTime, "09:00:00");
  
  pUserApi->ReqSettlementInfoConfirm(&req, requestId++);

  

}


void ctp_api_trader_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  if(0 != pRspInfo->ErrorID){
    // 通知客户端登出失败
    //TODO

    return ;
  }

  return;
}


void ctp_api_trader_on_rsp_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;

  if(pRspInfo != NULL && 0 != pRspInfo->ErrorID){
    // 通知客户端报单失败
    CMN_WARN("通知客户端报单失败\n");
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }

  return;
  
}


void ctp_api_trader_on_rsp_order_action(void* arg, CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;

  if(pRspInfo != NULL && 0 != pRspInfo->ErrorID){
    // 通知客户端撤单提交失败
    CMN_WARN("通知客户端撤单失败\n");
    CMN_WARN("pRspInfo->ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_WARN("pRspInfo->ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
    return ;
  }

  
}


void ctp_api_trader_on_rtn_trade(void* arg, CThostFtdcTradeField *pTrade)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;

  // 通知客户端成交
}


void ctp_api_trader_on_rtn_order(void* arg, CThostFtdcOrderField *pOrder)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;

  CMN_DEBUG("FrontID=%d\n"
    "SessionID=%d\n"
    "OrderRef=%s\n"
    "ExchangeID=%s\n"
    "OrderSysID=%s\n"
    "OrderStatus=%c\n",
    pOrder->FrontID,
    pOrder->SessionID,
    pOrder->OrderRef,
    pOrder->ExchangeID,
    pOrder->OrderSysID,
    pOrder->OrderStatus
  );

  frontId = pOrder->FrontID;
  sessionId = pOrder->SessionID;
  strcpy(orderRef, pOrder->OrderRef);

  return ;
  
}


void ctp_api_trader_on_err_rtn_order_insert(void* arg, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  
  // 通知客户端报单失败
  CMN_WARN("通知客户端报单失败\n");
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
}


void ctp_api_trader_on_err_rtn_order_action(void* arg, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  // 通知客户端撤单失败
  CMN_WARN("通知客户端撤单失败\n");
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
}


void ctp_api_trader_on_rsp_qry_user_investor(void* arg, CThostFtdcInvestorField *pRspUserInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;

  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }

  CMN_DEBUG(
    "InstrumentID[%s]\n",
    pRspUserInvestor->InvestorID
    );
  
  return ;
}

void ctp_api_trader_on_rsp_qry_instrument(void* arg, CThostFtdcInstrumentField *pRspInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  if(pRspInfo){
    //CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    //CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }

  if(pRspInstrument) {
    FILE* fp = fopen("instrument.csv", "a+");
    if(fp){
      fprintf(fp, "PriceTick:%30s,%lf\n", pRspInstrument->InstrumentID, pRspInstrument->PriceTick);
      fclose(fp);
    }
  }
  
}

void ctp_api_trader_on_rsp_settlement_info_confirm(void* arg, CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }

}


void ctp_api_trader_on_rsp_qry_investor_position(void* arg, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CThostFtdcTraderApi* pUserApi = (CThostFtdcTraderApi*)arg;
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
  if(pInvestorPosition){
    CMN_DEBUG(
      "pInvestorPosition->InstrumentID[%s]\n"
      "pInvestorPosition->PosiDirection[%c]\n"
      "pInvestorPosition->YdPosition[%d]\n"
      "pInvestorPosition->Position[%d]\n"
      "pInvestorPosition->OpenVolume[%d]\n"
      "pInvestorPosition->CloseVolume[%d]\n"
      "pInvestorPosition->TodayPosition[%d]\n",
      pInvestorPosition->InstrumentID,
      pInvestorPosition->PosiDirection,
      pInvestorPosition->YdPosition,
      pInvestorPosition->Position,
      pInvestorPosition->OpenVolume,
      pInvestorPosition->CloseVolume,
      pInvestorPosition->TodayPosition
    );

  }
}

void ctp_api_trader_on_rtn_instrument_status(void* arg, CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
  CMN_DEBUG("\n"
    "pInstrumentStatus->InstrumentID=[%s]\n"
    "pInstrumentStatus->InstrumentStatus=[%c]\n"
    "pInstrumentStatus->TradingSegmentSN=[%d]\n"
    "pInstrumentStatus->EnterTime=[%s]\n"
    "pInstrumentStatus->EnterReason=[%c]\n",
    pInstrumentStatus->InstrumentID,
    pInstrumentStatus->InstrumentStatus,
    pInstrumentStatus->TradingSegmentSN,
    pInstrumentStatus->EnterTime,
    pInstrumentStatus->EnterReason
  );
}



void ctp_api_mduser_on_front_connected(void* arg)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;

  // login
  CThostFtdcReqUserLoginField reqUserLogin;
  strcpy(reqUserLogin.TradingDay, pUserApi->GetTradingDay());
  strcpy(reqUserLogin.BrokerID, appId);
  strcpy(reqUserLogin.UserID, userId);
  strcpy(reqUserLogin.Password, pwd);
  strcpy(reqUserLogin.UserProductInfo, "Login");
  pUserApi->ReqUserLogin(&reqUserLogin, 0);

}

void ctp_api_mduser_on_front_disconnected(void* arg, int nReason)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;
}

void ctp_api_mduser_on_rsp_user_login(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
  
}

void ctp_api_mduser_on_rsp_user_logout(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;
  // 退出
}

void ctp_api_mduser_on_rtn_depth_market_data(void* arg, CThostFtdcDepthMarketDataField *pMarketData)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;

  // 通知客户端行情更新
  CMN_DEBUG("TradingDay=%s\n", pMarketData->TradingDay);
  CMN_DEBUG("UpdateTime=%s\n", pMarketData->UpdateTime);
  CMN_DEBUG("UpdateMillisec=%d\n", pMarketData->UpdateMillisec);
  CMN_DEBUG("InstrumentID=%s\n", pMarketData->InstrumentID);
  CMN_DEBUG("ExchangeID=%s\n", pMarketData->ExchangeID);
  CMN_DEBUG("BidPrice1=%lf\n", pMarketData->BidPrice1);
  CMN_DEBUG("BidVolume1=%d\n", pMarketData->BidVolume1);
  CMN_DEBUG("AskPrice1=%lf\n", pMarketData->AskPrice1);
  CMN_DEBUG("AskVolume1=%d\n", pMarketData->AskVolume1);
  
  return;
}

void ctp_api_mduser_on_rsp_error(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;

}

void ctp_api_mduser_on_rsp_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
  if(bIsLast){
    // 通知客户端订阅成功
  }


}

void ctp_api_mduser_on_rsp_un_sub_market_data(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("Enter!\n");
  if(pRspInfo){
    CMN_DEBUG("ErrorID[%d]\n", pRspInfo->ErrorID);
    CMN_DEBUG("ErrorMsg[%s]\n", pRspInfo->ErrorMsg);
  }
  CThostFtdcMdApi* pUserApi = (CThostFtdcMdApi*)arg;

}



