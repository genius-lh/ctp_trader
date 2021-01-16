
#ifndef _TRADER_TRADER_API_REM_H_
#define _TRADER_TRADER_API_REM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_rem_def trader_trader_api_rem;

struct trader_trader_api_rem_def {
  void* pTraderApi;
  void* pTraderHandler;
  char sInvestorID[19];
  int nTraderRequestID;
};


extern trader_trader_api_method* trader_trader_api_rem_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_REM_H_

