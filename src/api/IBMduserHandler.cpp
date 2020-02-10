#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EWrapper.h"
#include "IBMduserHandler.h"

CIBMduserHandler::CIBMduserHandler(ib_mduser_api_cb* cb, void * parent):
m_cb(cb)
, m_parent(parent)
{

}

void CIBMduserHandler::tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attribs)
{
	printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", tickerId, (int)field, price, attribs.canAutoExecute, attribs.pastLimit, attribs.preOpen);
}

void CIBMduserHandler::tickSize( TickerId tickerId, TickType field, int size)
{
  printf( "Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", tickerId, (int)field, size);
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
}


void CIBMduserHandler::connectionClosed()
{
	printf( "connectionClosed\n");
}


