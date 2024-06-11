
#ifndef _TRADER_TRADER_API_HXTS_H_
#define _TRADER_TRADER_API_HXTS_H_
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_hxts_def trader_trader_api_hxts;

struct trader_trader_api_hxts_def {
  void* pTraderApi;
  void* pTraderHandler;
  int nTraderRequestID;
  pthread_t workTreadId;
};


extern trader_trader_api_method* trader_trader_api_hxts_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_HXTS_H_

