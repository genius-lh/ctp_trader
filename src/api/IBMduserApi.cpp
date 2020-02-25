
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
  
///���ĺ�Լ���顣
///@param ppInstrumentID ��ԼID  
///@param nCount Ҫ����/�˶�����ĺ�Լ����
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

///�˶���Լ���顣
///@param ppInstrumentID ��ԼID  
///@param nCount Ҫ����/�˶�����ĺ�Լ����
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

