
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

#include "trader_order_mapper.h"

static int trader_trader_api_ib_get_trading_day(trader_trader_api* self, char* tradingday);
static int trader_trader_api_ib_get_max_order_local_id(trader_trader_api* self, long* local_id);

static void trader_trader_api_ib_start(trader_trader_api* self);
static void trader_trader_api_ib_stop(trader_trader_api* self);
static void trader_trader_api_ib_login(trader_trader_api* self);
static void trader_trader_api_ib_logout(trader_trader_api* self);
  
static int trader_trader_api_ib_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol, char* exchange_id);
static int trader_trader_api_ib_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
 
static int trader_trader_api_ib_qry_instrument(trader_trader_api* self);
static int trader_trader_api_ib_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_ib_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_ib_qry_trading_account(trader_trader_api* self);

static ib_trader_api_cb* ib_trader_api_cb_get();

static void ib_trader_on_front_connected(void* arg);
static void ib_trader_on_front_disconnected(void* arg);
static void ib_trader_on_position(void* arg, const char* account, const char* contract, int position, double avg_cost);
static void ib_trader_on_order(void* arg, long orderId, const char* status, int filled, int remaining,
   double avgFillPrice, int permId, double lastFillPrice, int clientId, double mktCapPrice);
static void ib_trader_on_next_valid_id(void* arg, long orderId);
static void ib_trader_on_error(void* arg, int errorCode, const char* errorMsg);


#ifdef __cplusplus
}
#endif

ib_trader_api_cb* ib_trader_api_cb_get()
{
  static ib_trader_api_cb ib_trader_api_cb_st = {
    ib_trader_on_front_connected,
    ib_trader_on_front_disconnected,
    ib_trader_on_position,
    ib_trader_on_order,
    ib_trader_on_next_valid_id,
    ib_trader_on_error
  };

  return &ib_trader_api_cb_st;
}

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
  //TODO
  
  return 0;
}

int trader_trader_api_ib_get_max_order_local_id(trader_trader_api* self, long* local_id)
{
  *local_id = self->userLocalId;
  return 0;
}


void trader_trader_api_ib_start(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)malloc(sizeof(trader_trader_api_ib));
  self->pUserApi = (void*)pImp;
  CIBTraderApi* pUserApi = CIBTraderApi::CreateIBTraderApi(self->pWorkspace);
  CIBTraderHandler* pHandler = new CIBTraderHandler(ib_trader_api_cb_get(), (void*)self);

  trader_order_mapper* pTraderOrderMapper = trader_order_mapper_new();
  
  pImp->pTraderHandler = (void*)pHandler;
  pImp->pTraderApi = (void*)pUserApi;
  pImp->pTraderOrderMapper = (void*)pTraderOrderMapper;
  
  // 连接交易服务器
  pUserApi->RegisterFront(self->pAddress);
  pUserApi->RegisterSpi(pHandler);
  pUserApi->RegisterClientId(atoi(self->pBrokerID));
  
  pUserApi->Init();

  return;

}

void trader_trader_api_ib_stop(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  if(!pImp){
    printf ( "ERROR! pImp == NULL \n");
    return ;
  }

  CIBTraderApi* pUserApi = (CIBTraderApi*)pImp->pTraderApi;
  CIBTraderHandler* pHandler = (CIBTraderHandler*)pImp->pTraderHandler;

  pUserApi->Release();
  delete pHandler;
  
  trader_order_mapper* pTraderOrderMapper = (trader_order_mapper*)pImp->pTraderOrderMapper;
  trader_order_mapper_free(pTraderOrderMapper);

  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_trader_api_ib_login(trader_trader_api* self)
{
  int errNo = 0;
  char* errMsg = NULL;
  
  sleep(2);

  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  
  return ;
}

void trader_trader_api_ib_logout(trader_trader_api* self)
{
  return;
}

  
int trader_trader_api_ib_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol, char* exchange_id)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CIBTraderApi* pTraderApi = (CIBTraderApi*)pImp->pTraderApi;
  
  trader_order_mapper* pTraderOrderMapper = (trader_order_mapper*)pImp->pTraderOrderMapper;

  long userLocalOrderId = atol(local_id);
  long sysOrderId = self->userLocalId++;

  pTraderOrderMapper->pMethod->xInsertOrder(pTraderOrderMapper, inst, sysOrderId, userLocalOrderId, buy_sell, open_close, vol);

  CIBInputOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
  strncpy(inputOrderField.InstrumentID, inst, sizeof(inputOrderField.InstrumentID));
  inputOrderField.LimitPrice = price;
  inputOrderField.Direction = buy_sell;
  inputOrderField.UserOrderLocalID = sysOrderId;
  inputOrderField.Volume = vol;

  pTraderApi->ReqOrderInsert(&inputOrderField);

  return 0;
}

int trader_trader_api_ib_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CIBTraderApi* pTraderApi = (CIBTraderApi*)pImp->pTraderApi;

  CIBOrderActionField inputOrderActionField;
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));
  
  long userLocalOrderId = atol(org_local_id);
  long sysOrderId = userLocalOrderId / 10;

	///报单引用
	inputOrderActionField.UserOrderActionLocalID = sysOrderId;
  
  pTraderApi->ReqOrderAction(&inputOrderActionField);

  return 0;
}

 
int trader_trader_api_ib_qry_instrument(trader_trader_api* self)
{
  return 0;
}

int trader_trader_api_ib_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CIBTraderApi* pTraderApi = (CIBTraderApi*)pImp->pTraderApi;

  //TODO

  return 0;
}

int trader_trader_api_ib_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CIBTraderApi* pTraderApi = (CIBTraderApi*)pImp->pTraderApi;

  pTraderApi->ReqPositions();

  return 0;
}

int trader_trader_api_ib_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  CIBTraderApi* pTraderApi = (CIBTraderApi*)pImp->pTraderApi;

  //TODO

  return 0;
}

void ib_trader_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;

  trader_trader_api_on_front_connected(self);

}

void ib_trader_on_front_disconnected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;

  trader_trader_api_on_front_disconnected(self, errNo);
}

void ib_trader_on_position(void* arg, const char* account, const char* contract, int position, double avg_cost)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  int errNo = 0;
  char* errMsg = NULL;
  
  strcpy(traderPosition.InstrumentID, contract);
  traderPosition.PositionDate = '1';
  traderPosition.PosiDirection = TRADER_POSITION_LONG;
  traderPosition.YdPosition = position;
  if(position < 0){
    traderPosition.PosiDirection = TRADER_POSITION_SHORT;
    traderPosition.YdPosition = -position;
  }
  traderPosition.IsSHFE = 0;
  
  traderPosition.TodayPosition = 0;
  traderPosition.Position = traderPosition.YdPosition;
  traderPosition.LongFrozen = 0;
  
  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, 0);

  return ;
}

void ib_trader_on_order(void* arg, long orderId, const char* status, int filled, int remaining,
   double avgFillPrice, int permId, double lastFillPrice, int clientId, double mktCapPrice)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;  
  trader_order_mapper* pTraderOrderMapper = (trader_order_mapper*)pImp->pTraderOrderMapper;
  trader_order_view* orderView;
  int nRet = 0;
  trader_trade traderTrade;
  trader_order traderOrder;

  nRet = pTraderOrderMapper->pMethod->xFindOrder(pTraderOrderMapper, orderId, &orderView);
  if(nRet < 0){
    // not found!
    return ;
  }

  if(filled > orderView->filled){
    memset(&traderTrade, 0, sizeof(traderTrade));
    ///合约代码
    strncpy(traderTrade.InstrumentID, orderView->contract, sizeof(traderTrade.InstrumentID));
    ///本地报单编号
    snprintf(traderTrade.UserOrderLocalID, sizeof(traderTrade.UserOrderLocalID), "%08ld", orderView->localUserOrderId);
    ///交易日
    //strcpy(traderTrade.TradingDay, pTrade->TradingDay);
    strcpy(traderTrade.TradingDay, "");
    ///成交时间
    //strcpy(traderTrade.TradeTime, pTrade->TradeTime);
    strcpy(traderTrade.TradeTime, "");
    ///买卖方向
    traderTrade.Direction = orderView->direction;
    ///开平标志
    traderTrade.OffsetFlag = orderView->offsetFlag;
    ///成交价格
    traderTrade.TradePrice = lastFillPrice;
    ///成交数量
    traderTrade.TradeVolume = (filled - orderView->filled);
    //成交编号
    snprintf(traderTrade.TradeID, sizeof(traderTrade.TradeID), "%d", permId);

    trader_trader_api_on_rtn_trade(self, &traderTrade);
  }

  do{
    memset(&traderOrder, 0, sizeof(traderOrder));
    ///交易所代码
    strcpy(traderOrder.ExchangeID, "IB");
    ///系统报单编号
    snprintf(traderOrder.OrderSysID, sizeof(traderOrder.OrderSysID), "%ld", orderId);
    // 合约代码
    strncpy(traderOrder.InstrumentID, orderView->contract, sizeof(traderOrder.InstrumentID));
    // 本地报单编号
    snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%08ld", orderView->localUserOrderId);
    // 买卖
    traderOrder.Direction = orderView->direction;
    // 开平
    traderOrder.OffsetFlag = orderView->offsetFlag;
    ///投机套保标志
    traderOrder.HedgeFlag = '0';
    // 报单价格
    traderOrder.LimitPrice = avgFillPrice;
    // 报单手数
    traderOrder.VolumeOriginal = orderView->total;
    // 成交手数
    traderOrder.VolumeTraded = filled;
    // 订单状态
    if(0 == strcmp("ApiPending", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_UNKNOW;
    }else if(0 == strcmp("PendingSubmit", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_UNKNOW;
    }else if(0 == strcmp("PendingCancel", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_UNKNOW;
    }else if(0 == strcmp("PreSubmitted", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_BEGIN;
    }else if(0 == strcmp("Submitted", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_NOTRADEQUEUEING;
    }else if(0 == strcmp("ApiCancelled", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_NOTRADENOTQUEUEING;
    }else if(0 == strcmp("Cancelled", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_CANCELED;
    }else if(0 == strcmp("Filled", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_ALLTRADED;
    }else if(0 == strcmp("Inactive", status)){
      traderOrder.OrderStatus = TRADER_ORDER_OS_FAILED;
    }else{
      traderOrder.OrderStatus = TRADER_ORDER_OS_UNKNOW;
    }
    ///插入时间
    strcpy(traderOrder.InsertTime, "");
    //strcpy(traderOrder.InsertTime, pOrder->InsertTime);

    trader_trader_api_on_rtn_order(self, &traderOrder);
  }while(0);
  
  return ;
}

void ib_trader_on_next_valid_id(void* arg, long orderId)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_ib* pImp = (trader_trader_api_ib*)self->pUserApi;
  
  self->userLocalId = orderId;
  
  trader_order_mapper* pTraderOrderMapper = (trader_order_mapper*)pImp->pTraderOrderMapper;

  pTraderOrderMapper->pMethod->xInit(pTraderOrderMapper, orderId, TRADER_ORDER_DEFAULT_SIZE);

  return ;
}

void ib_trader_on_error(void* arg, int errorCode, const char* errorMsg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = errorCode;
  const char* errMsg = errorMsg;
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);

}

void ib_future_contract_factory_init(const char* config_file, const char* section)
{
  mduser_instrument* ppInstruments;
  int nCount = 0;
  nCount = trader_trader_api_load_instruments(config_file, section, &ppInstruments);
  if(nCount < 0){
    return ;
  }

  IBFutureContractFactory* factory = IBFutureContractFactory::GetInstance();
  factory->Init(nCount, (void*)ppInstruments);

  free(ppInstruments);
}

void ib_future_contract_factory_fini()
{
  IBFutureContractFactory::Release();
}



