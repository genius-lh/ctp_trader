
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

static void ib_mduser_on_tick_bid_price(void* arg, long tickerId, double price);
static void ib_mduser_on_tick_ask_price(void* arg, long tickerId, double price);
static void ib_mduser_on_tick_bid_size(void* arg, long tickerId, int size);
static void ib_mduser_on_tick_ask_size(void* arg, long tickerId, int size);

static ib_mduser_api_cb* ib_mduser_api_cb_get();


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
  self->pUserApi = (void*)pImp;
  CIBMduserHandler* pHandler = new CIBMduserHandler(ib_mduser_api_cb_get(), (void*)self);
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
    pImp->pHandler = (void*)pHandler;
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
  if(!pImp){
    printf ( "ERROR! pImp == NULL \n");
    return ;
  }

  EClientSocket* pClientSocket = (EClientSocket*)pImp->pClient;

	pClientSocket->eDisconnect();

	printf ( "Disconnected\n");

   
  if(pImp->pReader){
    delete pImp->pReader;
  }
  if(pImp->pClient){
    delete pImp->pClient;
  }
  if(pImp->pSignal){
    delete pImp->pSignal;
  }
  if(pImp->pHandler){
    delete pImp->pHandler;
  }

  free(pImp);
  
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


ib_mduser_api_cb* ib_mduser_api_cb_get()
{
  static ib_mduser_api_cb ib_mduser_api_cb_st = {
    ib_mduser_on_tick_bid_price,
    ib_mduser_on_tick_ask_price,
    ib_mduser_on_tick_bid_size,
    ib_mduser_on_tick_ask_size
  };

  return &ib_mduser_api_cb_st;
}

ContractInfo::ContractInfo()
{

}


void ContractInfo::setContractInfo(long tickerid, char* symbol, char* secType, char* currency, char* exchange, void* tickerdata)
{
  this.tickerId = tickerid;
  this.contract.symbol = symbol;
  this.contract.secType = secType;
  this.contract.currency = currency;
  this.contract.exchange = exchange;
  this.tickerData = tickerdata;
}

Contract ContractInfo::getContract()
{
  return this.contract;
}

TickerId ContractInfo::getTickerId()
{
  return this.tickerId;
}

void* ContractInfo::getTickerData()
{
  return this.tickerData;
}


static ContractMap* ContractMap::getInstance()
{
  if(!instance){
    instance = new ContractMap();
    instance.load();
  }

  return instance;
}


bool ContractMap::getContractInfo(char* instrument, ContractInfo& contractInfo)
{
  bool ret = false;
  int i = 0;
  for(i = 0; i < this.number; i++){
    Contract contract = this.contracts[i].getContract();
    if(0 == strcmp(instrument, contract.symbol.c_str())){
      contractInfo = this.contracts[i];
      return true;
    }
  }
  
  return ret;
}

void* ContractMap::getTickerData(TickerId tickerId)
{
  return this.contracts[tickerId - TICKER_ID_BASE].getTickerData();
}

void ContractMap::addContractInfo(int index, char* symbol, char* secType, char* currency, char* exchange, void* tickerdata)
{
  ContractInfo contractInfo = this.contracts[index];
  contractInfo.setContractInfo(TICKER_ID_BASE + index, symbol, secType, currency, exchange, tickerdata);
  this.number++;

}

ContractMap::ContractMap(int default_size = 8)
{
  this.number = 0;
  this.size = default_size;
  this.contracts = new ContractInfo[this.size];
}

ContractMap::~ContractMap()
{
  delete [] contracts;
}

void ContractMap::load(const char* contract_cfg)
{


}



