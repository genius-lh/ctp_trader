#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Decimal.h"

#include "EWrapper.h"
#include "IBMduserHandler.h"
#include "IBFutureContractFactory.h"

CIBMduserHandler::CIBMduserHandler(ib_mduser_api_cb* cb, void * parent):
m_cb(cb)
, m_parent(parent)
, m_contractFactory(IBFutureContractFactory::GetInstance())
{

}

void CIBMduserHandler::tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attribs)
{
  // BID_SIZE, BID, ASK, ASK_SIZE
  IBFutureContract* future = m_contractFactory->Search(tickerId);
  if(!future){
    return ;
  }

  if(BID == field){
    m_cb->xOnTickBidPrice(m_parent, future->instrument, price);
  }else if(ASK == field){
    m_cb->xOnTickAskPrice(m_parent, future->instrument, price);
  }else if (DELAYED_BID == field){
    m_cb->xOnTickBidPrice(m_parent, future->instrument, price);
  }else if(DELAYED_ASK == field){
    m_cb->xOnTickAskPrice(m_parent, future->instrument, price);
  }
  return ;
}

void CIBMduserHandler::tickSize( TickerId tickerId, TickType field, Decimal size)
{
  // BID_SIZE, BID, ASK, ASK_SIZE
  IBFutureContract* future = m_contractFactory->Search(tickerId);
  if(!future){
    return ;
  }

  if(BID_SIZE == field){
    m_cb->xOnTickBidSize(m_parent, future->instrument, (int)decimalToDouble(size));
  }else if(ASK_SIZE == field){
    m_cb->xOnTickAskSize(m_parent, future->instrument, (int)decimalToDouble(size));
  }else if(DELAYED_BID_SIZE == field){
    m_cb->xOnTickBidSize(m_parent, future->instrument, (int)decimalToDouble(size));
  }else if(DELAYED_ASK_SIZE == field){
    m_cb->xOnTickAskSize(m_parent, future->instrument, (int)decimalToDouble(size));
  }else{

  }
  return ;
}

void CIBMduserHandler::error(int id, int errorCode, const std::string& errorString)
{
  m_cb->xOnRspError(m_parent, errorCode, errorString.c_str());
}

void CIBMduserHandler::connectAck()
{
  m_cb->xOnFrontConnected(m_parent);
}


void CIBMduserHandler::connectionClosed()
{
  m_cb->xOnFrontDisconnected(m_parent);
}

void CIBMduserHandler::tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, Decimal bidSize, Decimal askSize, const TickAttribBidAsk& tickAttribBidAsk)
{
  return ;
}

