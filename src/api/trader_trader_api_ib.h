
#ifndef _TRADER_TRADER_API_IB_H_
#define _TRADER_TRADER_API_IB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_ib_def trader_trader_api_ib;

struct trader_trader_api_ib_def {
  void* pTraderHandler;
};


extern trader_trader_api_method* trader_trader_api_ib_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_IB_H_

