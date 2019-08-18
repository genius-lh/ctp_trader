
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "ThostFtdcUserApiDataType.h"
//#include "ThostFtdcMdApi.h"

//#include "CtpMduserHandler.h"

#include "EWrapper.h"
#include "EClientSocket.h"
#include "EReader.h"

#include "IBMduserHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_ib.h"


static void trader_mduser_api_ib_start(trader_mduser_api* self);
static void trader_mduser_api_ib_stop(trader_mduser_api* self);
static void trader_mduser_api_ib_login(trader_mduser_api* self);
static void trader_mduser_api_ib_logout(trader_mduser_api* self);
static void trader_mduser_api_ib_subscribe(trader_mduser_api* self, char* instrument);

#ifdef __cplusplus
}
#endif

trader_mduser_api_method* trader_mduser_api_ib_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_ib_start,
    trader_mduser_api_ib_stop,
    trader_mduser_api_ib_login,
    trader_mduser_api_ib_logout,
    trader_mduser_api_ib_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_ib_start(trader_mduser_api* self)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)malloc(sizeof(trader_mduser_api_ib));
  CIBMduserHandler pHandler = new CIBMduserHandler((void*)pImp);
  EReaderOSSignal* pOSSignal = new EReaderOSSignal(2000);
  EClientSocket* pClientSocket = new EClientSocket(pHandler, pOSSignal);

  char* host = "127.0.0.1";
  int port = atoi(self->pAddress);
  int clientId = atoi(self->pBrokerID);
  

  do{
  //! [connect]
  bool bRes = pClientSocket->eConnect( host, port, clientId, false);
  //! [connect]

  if (!bRes) {
    printf( "Cannot connect to %s:%d clientId:%d\n", pClientSocket->host().c_str(), pClientSocket->port(), clientId);

  }
  printf( "Connected to %s:%d clientId:%d\n", pClientSocket->host().c_str(), pClientSocket->port(), clientId);
  //! [ereader]
  EReader* pReader = new EReader(pClientSocket, pOSSignal);
  pReader->start();
  //! [ereader]
  pImp->pSignal = (void*)pOSSignal;
  pImp->pClient = (void*)pClientSocket;
  pImp->pReader = (void*)pReader;

  pImp->tickId = 0;
  pImp->reqId = 0;
  
  self->pUserApi = (void*)pImp;

  }while(0);
  
  return ;
}

void trader_mduser_api_ib_stop(trader_mduser_api* self)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;

  EClientSocket* pClientSocket = (EClientSocket*)pImp->pClient;

	pClientSocket->eDisconnect();

	printf ( "Disconnected\n");
  return ;
}

void trader_mduser_api_ib_login(trader_mduser_api* self)
{

  return ;
}

void trader_mduser_api_ib_logout(trader_mduser_api* self)
{

  return ;
}

void trader_mduser_api_ib_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;

  EClientSocket* pClientSocket = (EClientSocket*)pImp->pClient;

	pClientSocket->reqMktData(pImp->tickId, CIBTraderHandler::getContractById(instrument), "", false, false, TagValueListSPtr());

  return ;
}

