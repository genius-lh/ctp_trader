#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EWrapper.h"
#include "IBMduserHandler.h"

CIBMduserHandler::CIBMduserHandler(ib_mduser_api_cb* cb, void* parent)
:m_cb(cb), m_parent(parent)
{

}

CIBMduserHandler::~CIBMduserHandler()
{

}

//! [tickprice]
void CIBMduserHandler::tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attribs) {
	printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", 
    tickerId, (int)field, price, attribs.canAutoExecute, attribs.pastLimit, attribs.preOpen);
  if(BID == field){
    m_cb->xOnTickBidPrice(tickerId, price);
  }else if(ASK == field){
    m_cb->xOnTickAskPrice(tickerId, price);
  }else{
    // EMPTY
  }
  
}
//! [tickprice]

//! [ticksize]
void CIBMduserHandler::tickSize( TickerId tickerId, TickType field, int size) {
	printf( "Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", 
    tickerId, (int)field, size);
  if(BID_SIZE == field){
    m_cb->xOnTickBidSize(tickerId, size);
  }else if(ASK_SIZE == field){
    m_cb->xOnTickAskSize(tickerId, size);
  }else{
    // EMPTY
  }
}
//! [ticksize]





