
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Contract.h"
#include "EClientSocket.h"

#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

#include "IBMduserApi.h"

CIBMduserApi* CIBMduserApi::CreateIBMduserApi(const char *pszFlowPath)
{
  CIBMduserApi* self = new CIBMduserApi();
  return self;
}
  
///订阅合约行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CIBMduserApi::SubMarketData(char *ppInstrumentID[], int nCount)
{
  int i = 0;
  char* instrumentID;
  IBFutureContract* future;

  for(i = 0; i < nCount; i++){
    instrumentID = ppInstrumentID[i];
    future = GetFutureContract(instrumentID);
    if(NULL != future){
      ReqMarketData(future);
    }
  }
  return 0;

}

///退订合约行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CIBMduserApi::UnSubMarketData(char *ppInstrumentID[], int nCount)
{
  int i = 0;
  char* instrumentID;
  IBFutureContract* future;

  for(i = 0; i < nCount; i++){
    instrumentID = ppInstrumentID[i];
    future = GetFutureContract(instrumentID);
    if(NULL != future){
      CancelMarketData(future);
    }
  }
  return 0;
}

void CIBMduserApi::ReqMarketDataType(int marketDataType)
{
  Api()->reqMarketDataType(marketDataType);
}

void CIBMduserApi::ReqMarketData(IBFutureContract* future)
{
	Contract contract;
	contract.localSymbol = future->localSymbol;
  contract.secType = future->secType;
  contract.exchange = future->exchange;
  contract.currency = future->currency;

	Api()->reqMktData(future->tickId, contract, "", false, false, TagValueListSPtr());

}

void CIBMduserApi::CancelMarketData(IBFutureContract* future)
{
  Api()->cancelMktData(future->tickId);
}

