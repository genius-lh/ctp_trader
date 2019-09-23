
#ifndef _TRADER_MDUSER_API_IB_H_
#define _TRADER_MDUSER_API_IB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_mduser_api.h"

typedef struct trader_mduser_api_ib_def  trader_mduser_api_ib;
struct trader_mduser_api_ib_def {
  void* pHandler;
	//! [socket_declare]
	void* pSignal;
	void* pClient;
	void* pReader;
	//! [socket_declare]

  void* contractMap;

  long tickId;
  int reqId;
};

extern trader_mduser_api_method* trader_mduser_api_ib_method_get();

#ifdef __cplusplus
}
#endif

#include "EWraper.h"
#include "Contract.h"

#include<vector>

typedef struct contract_info
{
  Contract contract;
  TickerId tickerId;
  void* tickerData;
}ContractInfo;

class ContractMap
{
public:
  static ContractMap* getInstance();
  bool getContractInfo(char* instrument, ContractInfo& contractInfo);
  void* getTickerData(TickerId tickerId);
  void addContractInfo(long tickerId, char* symbol, char* secType, char* currency, char* exchange, void* tickerdata);
  
private:
  static ContractMap* instance = NULL;
  const int TICKER_ID_BASE = 1000;
private:
  ContractMap();
  ~ContractMap();

  void load();

  vector<ContractInfo*> contracts;
};



#endif //_TRADER_MDUSER_API_IB_H_

