
#ifndef _TRADER_MDUSER_API_IB_H_
#define _TRADER_MDUSER_API_IB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_mduser_api.h"

typedef struct trader_mduser_api_ib_def  trader_mduser_api_ib;
struct trader_mduser_api_ib_def {
  void* pMdApi;
  void* pHandler;
  void* pShmHeader;

  int marketDataType;
};

extern trader_mduser_api_method* trader_mduser_api_ib_method_get();

extern void ib_future_contract_factory_init(const char* config_file, const char* section);

#ifdef __cplusplus
}
#endif

#endif //_TRADER_MDUSER_API_IB_H_

