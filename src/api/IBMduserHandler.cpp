#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", tickerId, (int)field, price, attribs.canAutoExecute, attribs.pastLimit, attribs.preOpen);
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

void CIBMduserHandler::tickSize( TickerId tickerId, TickType field, int size)
{
  printf( "Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", tickerId, (int)field, size);
  // BID_SIZE, BID, ASK, ASK_SIZE
  IBFutureContract* future = m_contractFactory->Search(tickerId);
  if(!future){
    return ;
  }

  if(BID_SIZE == field){
    m_cb->xOnTickBidSize(m_parent, future->instrument, size);
  }else if(ASK_SIZE == field){
    m_cb->xOnTickAskSize(m_parent, future->instrument, size);
  }else if(DELAYED_BID_SIZE == field){
    m_cb->xOnTickBidSize(m_parent, future->instrument, size);
  }else if(DELAYED_ASK_SIZE == field){
    m_cb->xOnTickAskSize(m_parent, future->instrument, size);
  }else{

  }
  return ;
}

void CIBMduserHandler::tickString(TickerId tickerId, TickType tickType, const std::string& value)
{
	printf( "Tick String. Ticker Id: %ld, Type: %d, Value: %s\n", tickerId, (int)tickType, value.c_str());
}

void CIBMduserHandler::error(int id, int errorCode, const std::string& errorString)
{
  printf( "Error. Id: %d, Code: %d, Msg: %s\n", id, errorCode, errorString.c_str());
}

void CIBMduserHandler::connectAck()
{
  printf( "connectAck\n");
  m_cb->xOnFrontConnected(m_parent);
}


void CIBMduserHandler::connectionClosed()
{
	printf( "connectionClosed\n");
  m_cb->xOnFrontDisconnected(m_parent);
}


