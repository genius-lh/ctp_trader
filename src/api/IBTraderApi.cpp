
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

///����¼������
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
	order.orderType = "LMT";
	order.totalQuantity = (double)pInputOrder->Volume;
	order.lmtPrice = pInputOrder->LimitPrice;

  OrderId orderId = pInputOrder->UserOrderLocalID;

  Api()->placeOrder(orderId, contract, order);

  return 0;

}

///������������
int CIBTraderApi::ReqOrderAction(CIBOrderActionField *pOrderAction)
{
  OrderId orderId = pOrderAction->UserOrderActionLocalID;

	Api()->cancelOrder(orderId);

  return 0;

}

void CIBTraderApi::ReqOrderId()
{
	Api()->reqIds(-1);

}

const char* CIBTraderApi::ConvOrderAction(char direction)
{
  return ('0' == direction) ? "BUY" : "SELL";
}
