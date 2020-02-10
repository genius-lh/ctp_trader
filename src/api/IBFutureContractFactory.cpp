
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IBFutureContractFactory.h"

IBFutureContractFactory* IBFutureContractFactory::GetInstance()
{
  if(NULL == instance){
    instance = new IBFutureContractFactory();
  }
  
  return instance;
}

void IBFutureContractFactory::Release()
{
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
    if(0 == strcmp(contract, iter->localSymbol)){
      return iter;
    }
  }
  return (IBFutureContract*)NULL;

}

int IBFutureContractFactory::Init(const char* configFile)
{
  //TODO
  IBFutureContract future = {
    1001,
    "CL",
    "FUT",
    "202002",
    "NYMEX",
    "USD",
    "CLH0"
  };
  this->futureContract = (IBFutureContract*)malloc(sizeof(IBFutureContract));
  this->count = 1;
  memcpy(this->futureContract, &future, sizeof(IBFutureContract));
  return 0;
}


  







