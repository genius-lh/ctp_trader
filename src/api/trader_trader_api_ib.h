
#ifndef _TRADER_TRADER_API_IB_H_
#define _TRADER_TRADER_API_IB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_ib_def trader_trader_api_ib;

struct trader_trader_api_ib_def {
  void* pTraderApi;
  void* pTraderHandler;

  void* pTraderOrderMapper;
};


extern trader_trader_api_method* trader_trader_api_ib_method_get();

extern void ib_future_contract_factory_init(const char* config_file, const char* section);


#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_IB_H_

