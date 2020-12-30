
#ifndef _TRADER_TRADER_API_FEMAS_AF_H_
#define _TRADER_TRADER_API_FEMAS_AF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_femas_af_def trader_trader_api_femas_af;

struct trader_trader_api_femas_af_def {
  void* pTraderApi;
  void* pTraderHandler;
  char sInvestorID[19];
  int nTraderRequestID;
};


extern trader_trader_api_method* trader_trader_api_femas_af_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_FEMAS_AF_H_

