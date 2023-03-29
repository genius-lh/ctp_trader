
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include "CommonDefs.h"
#include "Contract.h"
#include "Order.h"
#include "OrderState.h"
#include "Utils.h"

#include "IBTraderHandler.h"

CIBTraderHandler::CIBTraderHandler(ib_trader_api_cb* cb, void * parent):
m_cb(cb)
, m_parent(parent)
, m_contractFactory(IBFutureContractFactory::GetInstance())
{

}

void CIBTraderHandler::error(int id, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson)
{
  m_cb->xOnError(m_parent, errorCode, errorString.c_str());
}

void CIBTraderHandler::connectAck()
{
  m_cb->xOnFrontConnected(m_parent);
}


void CIBTraderHandler::connectionClosed()
{
  m_cb->xOnFrontDisconnected(m_parent);
}

void CIBTraderHandler::orderStatus(OrderId orderId, const std::string& status, Decimal filled,
    Decimal remaining, double avgFillPrice, int permId, int parentId,
    double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice)
{
  printf("OrderStatus. Id: %ld, Status: %s, Filled: %s, Remaining: %s, AvgFillPrice: %s, PermId: %s, LastFillPrice: %s, ClientId: %s, WhyHeld: %s, MktCapPrice: %s\n", 
      orderId, status.c_str(), decimalStringToDisplay(filled).c_str(), decimalStringToDisplay(remaining).c_str(), Utils::doubleMaxString(avgFillPrice).c_str(), Utils::intMaxString(permId).c_str(), 
      Utils::doubleMaxString(lastFillPrice).c_str(), Utils::intMaxString(clientId).c_str(), whyHeld.c_str(), Utils::doubleMaxString(mktCapPrice).c_str());
  m_cb->xOnOrder(m_parent, orderId, status.c_str(), filled, remaining, avgFillPrice, permId, lastFillPrice, clientId, mktCapPrice);
}

void CIBTraderHandler::openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState) 
{
  printf( "OpenOrder. PermId: %s, ClientId: %s, OrderId: %s, Account: %s, Symbol: %s, SecType: %s, Exchange: %s:, Action: %s, OrderType:%s, TotalQty: %s, CashQty: %s, "
      "LmtPrice: %s, AuxPrice: %s, Status: %s, MinTradeQty: %s, MinCompeteSize: %s, CompeteAgainstBestOffset: %s, MidOffsetAtWhole: %s, MidOffsetAtHalf: %s\n", 
      Utils::intMaxString(order.permId).c_str(), Utils::longMaxString(order.clientId).c_str(), Utils::longMaxString(orderId).c_str(), order.account.c_str(), contract.symbol.c_str(), 
      contract.secType.c_str(), contract.exchange.c_str(), order.action.c_str(), order.orderType.c_str(), decimalStringToDisplay(order.totalQuantity).c_str(), 
      Utils::doubleMaxString(order.cashQty).c_str(), Utils::doubleMaxString(order.lmtPrice).c_str(), Utils::doubleMaxString(order.auxPrice).c_str(), orderState.status.c_str(),
      Utils::intMaxString(order.minTradeQty).c_str(), Utils::intMaxString(order.minCompeteSize).c_str(), 
      order.competeAgainstBestOffset == COMPETE_AGAINST_BEST_OFFSET_UP_TO_MID ? "UpToMid" : Utils::doubleMaxString(order.competeAgainstBestOffset).c_str(),
      Utils::doubleMaxString(order.midOffsetAtWhole).c_str(), Utils::doubleMaxString(order.midOffsetAtHalf).c_str());
}

void CIBTraderHandler::openOrderEnd()
{
  printf( "OpenOrderEnd\n");
}

void CIBTraderHandler::nextValidId( OrderId orderId)
{
  m_cb->xOnNextValidId(m_parent, orderId);
}

void CIBTraderHandler::position( const std::string& account, const Contract& contract, Decimal position, double avgCost)
{
  m_cb->xOnPosition(m_parent, account.c_str(), contract.localSymbol.c_str(), position, avgCost);
}

void CIBTraderHandler::positionEnd() {
    printf( "PositionEnd\n");
}



