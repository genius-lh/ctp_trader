
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trader_data.h"
#include "IBFutureContractFactory.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "cmn_log.h"
static int ib_future_contract_make(char* instrument, long tick_id, IBFutureContract* future);
#ifdef __cplusplus
}
#endif


const long TICK_ID_BASE = 1001;

int ib_future_contract_make(char* instrument, long tick_id, IBFutureContract* future)
{
  char* savePtr;
  char* symbol;
  char* localSymbol;
  char contract[64];

  strncpy(contract, instrument, sizeof(future->instrument));
  
  symbol = strtok_r(contract, ".", &savePtr);
  if (NULL == symbol) {
   return -1;
  }

  localSymbol = strtok_r(NULL, ".", &savePtr);
  if (NULL == localSymbol) {
   return -2;
  }

  strncpy(future->instrument, instrument, sizeof(future->instrument));
  future->tickId = tick_id;
  strncpy(future->symbol, symbol, sizeof(future->symbol));
  if(0 == strcmp(symbol, "USD")){
    strncpy(future->secType, "CASH", sizeof(future->secType));
    strncpy(future->currency, localSymbol, sizeof(future->currency));
    strncpy(future->exchange, "IDEALPRO", sizeof(future->exchange));
    strncpy(future->localSymbol, instrument, sizeof(future->localSymbol));
  }else{
    strncpy(future->localSymbol, localSymbol, sizeof(future->localSymbol));
    strncpy(future->secType, "FUT", sizeof(future->secType));
    strncpy(future->currency, "USD", sizeof(future->currency));
    if(0 == memcmp(future->symbol, "COIL", 4)){
      strncpy(future->exchange, "QBALGOIEU", sizeof(future->exchange));
    }else if (0 == memcmp(future->symbol, "ZM", 2)){
      strncpy(future->exchange, "ECBOT", sizeof(future->exchange));
    }else{
      strncpy(future->exchange, "NYMEX", sizeof(future->exchange));
    }
  }
  return 0;
}


IBFutureContractFactory* IBFutureContractFactory::GetInstance()
{
  static IBFutureContractFactory* instance = (IBFutureContractFactory*)NULL;
  if(NULL == instance){
    instance = new IBFutureContractFactory();
  }
  
  return instance;
}

void IBFutureContractFactory::Release()
{
  IBFutureContractFactory* instance = IBFutureContractFactory::GetInstance();

  if(NULL == instance){
    return ;
  }

  delete instance;
  instance = (IBFutureContractFactory*)NULL;
}

IBFutureContractFactory::IBFutureContractFactory():
count(0)
,futureContract((IBFutureContract*)NULL)
{

}

IBFutureContractFactory::~IBFutureContractFactory()
{
  if(this->count > 0){
    free(this->futureContract);
  }
}

IBFutureContract* IBFutureContractFactory::Search(const char* contract)
{
  IBFutureContract* iter;
  int i = 0;
  for(i = 0; i < this->count; i++){
    iter = &this->futureContract[i];
    if(0 == strcmp(contract, iter->instrument)){
      return iter;
    }
  }
  return (IBFutureContract*)NULL;
}

IBFutureContract* IBFutureContractFactory::Search(long tick_id)
{
  int idx = tick_id - TICK_ID_BASE;

  if((idx < 0) || (idx >= this->count)){
    return (IBFutureContract*)NULL;
  }

  return &this->futureContract[idx];
}

int IBFutureContractFactory::Init(int count, void* instruments)
{
  int i;
  int nRet;
  
  mduser_instrument* ppInstruments = (mduser_instrument*)instruments;
  
  this->count = count;
  this->futureContract = (IBFutureContract*)malloc(this->count * sizeof(IBFutureContract));

  for(i = 0; i < this->count; i++){
    memset(&this->futureContract[i], 0, sizeof(IBFutureContract));
    nRet = ::ib_future_contract_make(ppInstruments[i], i + TICK_ID_BASE, &this->futureContract[i]);
    CMN_DEBUG("ib_future_contract_make nRet[%d]\n", nRet);
  }

  return 0;
}


