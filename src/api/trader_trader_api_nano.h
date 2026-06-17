#ifndef _TRADER_TRADER_API_NANO_H_
#define _TRADER_TRADER_API_NANO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_nano_def trader_trader_api_nano;

struct trader_trader_api_nano_def {
  void* pTraderApi;
  void* pTraderHandler;
};


extern trader_trader_api_method* trader_trader_api_nano_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_NANO_H_
