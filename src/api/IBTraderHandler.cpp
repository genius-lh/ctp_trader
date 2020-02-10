
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include "CommonDefs.h"
#include "Contract.h"
#include "Order.h"
#include "OrderState.h"

#include "IBTraderHandler.h"

CIBTraderHandler::CIBTraderHandler(ib_trader_api_cb* cb, void * parent):
m_cb(cb)
, m_parent(parent)
{

}

void CIBTraderHandler::error(int id, int errorCode, const std::string& errorString)
{
  printf( "Error. Id: %d, Code: %d, Msg: %s\n", id, errorCode, errorString.c_str());
}

void CIBTraderHandler::connectAck()
{
  printf( "connectAck\n");
}


void CIBTraderHandler::connectionClosed()
{
	printf( "connectionClosed\n");
}

void CIBTraderHandler::orderStatus(OrderId orderId, const std::string& status, double filled,
    double remaining, double avgFillPrice, int permId, int parentId,
    double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice)
{
  printf("OrderStatus. Id: %ld, Status: %s, Filled: %g, Remaining: %g, AvgFillPrice: %g, PermId: %d, LastFillPrice: %g, ClientId: %d, WhyHeld: %s, MktCapPrice: %g\n", orderId, status.c_str(), filled, remaining, avgFillPrice, permId, lastFillPrice, clientId, whyHeld.c_str(), mktCapPrice);
}

void CIBTraderHandler::openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState) 
{
  printf( "OpenOrder. PermId: %ld, ClientId: %ld, OrderId: %ld, Account: %s, Symbol: %s, SecType: %s, Exchange: %s:, Action: %s, OrderType:%s, TotalQty: %g, CashQty: %g, "
  "LmtPrice: %g, AuxPrice: %g, Status: %s\n", 
    order.permId, order.clientId, orderId, order.account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.exchange.c_str(), 
    order.action.c_str(), order.orderType.c_str(), order.totalQuantity, order.cashQty == UNSET_DOUBLE ? 0 : order.cashQty, order.lmtPrice, order.auxPrice, orderState.status.c_str());
}

void CIBTraderHandler::openOrderEnd()
{
  printf( "OpenOrderEnd\n");
}

void CIBTraderHandler::completedOrder(const Contract& contract, const Order& order, const OrderState& orderState)
{
  printf( "CompletedOrder. PermId: %ld, ParentPermId: %lld, Account: %s, Symbol: %s, SecType: %s, Exchange: %s:, Action: %s, OrderType: %s, TotalQty: %g, CashQty: %g, FilledQty: %g, "
    "LmtPrice: %g, AuxPrice: %g, Status: %s, CompletedTime: %s, CompletedStatus: %s\n", 
    order.permId, order.parentPermId == UNSET_LONG ? 0 : order.parentPermId, order.account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.exchange.c_str(), 
    order.action.c_str(), order.orderType.c_str(), order.totalQuantity, order.cashQty == UNSET_DOUBLE ? 0 : order.cashQty, order.filledQuantity, 
    order.lmtPrice, order.auxPrice, orderState.status.c_str(), orderState.completedTime.c_str(), orderState.completedStatus.c_str());
}

void CIBTraderHandler::completedOrdersEnd() 
{
  printf( "CompletedOrdersEnd\n");
}

void CIBTraderHandler::nextValidId( OrderId orderId)
{
  printf("Next Valid Id: %ld\n", orderId);
}


