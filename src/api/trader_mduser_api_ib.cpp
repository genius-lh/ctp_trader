
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "IBMduserHandler.h"
#include "IBFutureContractFactory.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_ib.h"
#include "trader_mduser_shm.h"

static void trader_mduser_api_ib_start(trader_mduser_api* self);
static void trader_mduser_api_ib_stop(trader_mduser_api* self);
static void trader_mduser_api_ib_login(trader_mduser_api* self);
static void trader_mduser_api_ib_logout(trader_mduser_api* self);
static void trader_mduser_api_ib_subscribe(trader_mduser_api* self, char* instrument);

static void ib_mduser_on_tick_bid_price(void* arg, const char* instrument, double price);
static void ib_mduser_on_tick_ask_price(void* arg, const char* instrument, double price);
static void ib_mduser_on_tick_bid_size(void* arg, const char* instrument, int size);
static void ib_mduser_on_tick_ask_size(void* arg, const char* instrument, int size);
static void ib_mduser_on_connected(void* arg);
static void ib_mduser_on_disconnected(void* arg);

static void* trader_mduser_api_ib_tick_search(trader_mduser_api* self, const char* instrument);
static int trader_mduser_api_ib_tick_cmp(void* data1, void* data2);


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
  CIBMduserApi* pUserApi = CIBMduserApi::CreateIBMduserApi(self->pWorkspace);
  CIBMduserHandler* pHandler = new CIBMduserHandler(ib_mduser_api_cb_get(), (void*)self);

  trader_mduser_shm_header* pShmHeader = trader_mduser_shm_header_at();

  pImp->pHandler = (void*)pHandler;
  pImp->pMdApi = (void*)pUserApi;
  pImp->pShmHeader = (void*)pShmHeader;

  pImp->marketDataType = 3;
  
  // 连接交易服务器
  pUserApi->RegisterFront(self->pAddress);
  pUserApi->RegisterSpi(pHandler);
  pUserApi->RegisterClientId(atoi(self->pBrokerID));
  
  pUserApi->Init();
  
  return ;
}

void trader_mduser_api_ib_stop(trader_mduser_api* self)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;
  if(!pImp){
    printf ( "ERROR! pImp == NULL \n");
    return ;
  }

  CIBMduserApi* pUserApi = (CIBMduserApi*)pImp->pMdApi;
  CIBMduserHandler* pHandler = (CIBMduserHandler*)pImp->pHandler;

  pUserApi->Release();
  delete pHandler;

  trader_mduser_shm_header* pShmHeader = (trader_mduser_shm_header*)pImp->pShmHeader;
  trader_mduser_shm_header_dt(pShmHeader);
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_ib_login(trader_mduser_api* self)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;
  CIBMduserApi* pUserApi = (CIBMduserApi*)pImp->pMdApi;
  int errNo = 0;
  char* errMsg = NULL;
  
  sleep(2);

  pUserApi->ReqMarketDataType(pImp->marketDataType);

  trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);
  
  return ;
}

void trader_mduser_api_ib_logout(trader_mduser_api* self)
{

  return ;
}

void trader_mduser_api_ib_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;
  
  CIBMduserApi* pUserApi = (CIBMduserApi*)pImp->pMdApi;

  char* contracts[1];
  contracts[0] = instrument;

  pUserApi->SubMarketData(contracts, 1);

  return ;
}


ib_mduser_api_cb* ib_mduser_api_cb_get()
{
  static ib_mduser_api_cb ib_mduser_api_cb_st = {
    ib_mduser_on_tick_bid_price,
    ib_mduser_on_tick_ask_price,
    ib_mduser_on_tick_bid_size,
    ib_mduser_on_tick_ask_size,
    ib_mduser_on_connected,
    ib_mduser_on_disconnected
  };

  return &ib_mduser_api_cb_st;
}

void* trader_mduser_api_ib_tick_search(trader_mduser_api* self, const char* instrument)
{
  trader_tick tmp;
  trader_mduser_api_ib* pImp = (trader_mduser_api_ib*)self->pUserApi;
  trader_mduser_shm_header* pShmHeader = (trader_mduser_shm_header*)pImp->pShmHeader;

  strncpy(tmp.InstrumentID, instrument, sizeof(tmp.InstrumentID));
  return trader_mduser_shm_bsearch(pShmHeader, (void *)&tmp, trader_mduser_api_ib_tick_cmp);
}

int trader_mduser_api_ib_tick_cmp(void* data1, void* data2)
{
  return strncmp(((trader_tick*)data1)->InstrumentID, ((trader_tick*)data2)->InstrumentID, sizeof(((trader_tick*)NULL)->InstrumentID));
}

void ib_mduser_on_tick_bid_price(void* arg, const char* instrument, double price)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;

  trader_tick* pTick = (trader_tick*)trader_mduser_api_ib_tick_search(self, instrument);
  if(NULL == pTick){
    return ;
  }
  pTick->BidPrice1 = price;
  return ;
}

void ib_mduser_on_tick_ask_price(void* arg, const char* instrument, double price)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;

  trader_tick* pTick = (trader_tick*)trader_mduser_api_ib_tick_search(self, instrument);
  if(NULL == pTick){
    return ;
  }
  pTick->AskPrice1 = price;
  return ;
}

void ib_mduser_on_tick_bid_size(void* arg, const char* instrument, int size)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;

  trader_tick* pTick = (trader_tick*)trader_mduser_api_ib_tick_search(self, instrument);
  if(NULL == pTick){
    return ;
  }
  pTick->BidVolume1 = size;
  return ;
}

void ib_mduser_on_tick_ask_size(void* arg, const char* instrument, int size)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;

  trader_tick* pTick = (trader_tick*)trader_mduser_api_ib_tick_search(self, instrument);
  if(NULL == pTick){
    return ;
  }
  pTick->AskVolume1 = size;
  return ;
}

void ib_mduser_on_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;

  trader_mduser_api_on_front_connected(self);
}

void ib_mduser_on_disconnected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  
  trader_mduser_api_on_front_disconnected(self, errNo, errMsg);
}

void ib_future_contract_factory_init(const char* config_file, const char* section)
{
  mduser_instrument* ppInstruments;
  int nCount = 0;

  nCount = trader_mduser_api_load_instruments(config_file, section, &ppInstruments);
  if(nCount < 0){
    return ;
  }

  IBFutureContractFactory* factory = IBFutureContractFactory::GetInstance();
  factory->Init(nCount, (void*)ppInstruments);
  
  free(ppInstruments);

}


