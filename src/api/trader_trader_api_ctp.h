
#ifndef _TRADER_TRADER_API_CTP_H_
#define _TRADER_TRADER_API_CTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_ctp_def trader_trader_api_ctp;

struct trader_trader_api_ctp_def {
  void* pTraderApi;
  void* pTraderHandler;
  int nTraderRequestID;
};


extern trader_trader_api_method* trader_trader_api_ctp_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_CTP_H_

