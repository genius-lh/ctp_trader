#include <iostream>

#include "CommonDefs.h"
#include "Order.h"
#include "Contract.h"
#include "EClientSocket.h"

#include "IBTraderApi.h"
#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

CIBTraderApi * CIBTraderApi::CreateIBTraderApi(const char *pszFlowPath)
{
  CIBTraderApi* self = new CIBTraderApi();
  return self;
}

///报单录入请求
int CIBTraderApi::ReqOrderInsert(CIBInputOrderField *pInputOrder)
{
	Contract contract;
	Order order;
  
  char* instrumentID = pInputOrder->InstrumentID;
  IBFutureContract* future;
  future = GetFutureContract(instrumentID);
  if(NULL == future){
    return -1;
  }
  
	//contract.symbol = future->symbol;
	contract.secType = future->secType;
	contract.exchange = future->exchange;
	contract.currency = future->currency;
	contract.localSymbol = future->localSymbol;

  
	order.action = ConvOrderAction(pInputOrder->Direction);
	order.totalQuantity = doubleToDecimal((double)pInputOrder->Volume);
  if(pInputOrder->LimitPrice > 0){
  	order.orderType = "LMT";
  	order.lmtPrice = pInputOrder->LimitPrice;
  }else{
  	order.orderType = "MKT";
  }

  // TODO
  OrderId orderId = pInputOrder->UserOrderLocalID;

  Api()->placeOrder(orderId, contract, order);

  return 0;

}

///报单操作请求
int CIBTraderApi::ReqOrderAction(CIBOrderActionField *pOrderAction)
{
  OrderId orderId = pOrderAction->UserOrderActionLocalID;

	Api()->cancelOrder(orderId, "");

  return 0;

}

void CIBTraderApi::ReqOrderId()
{
	Api()->reqIds(-1);

}

void CIBTraderApi::ReqPositions()
{
  Api()->reqPositions();
}

const char* CIBTraderApi::ConvOrderAction(char direction)
{
  return ('0' == direction) ? "BUY" : "SELL";
}

