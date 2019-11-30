
#ifndef _TRADER_TRADER_API_XSPEED_H_
#define _TRADER_TRADER_API_XSPEED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_xspeed_def trader_trader_api_xspeed;

struct trader_trader_api_xspeed_def {
  void* pTraderApi;
  void* pTraderHandler;
  int nTraderRequestID;
  int nTradingDay;
};


extern trader_trader_api_method* trader_trader_api_xspeed_sop_method_get();
extern trader_trader_api_method* trader_trader_api_xspeed_stock_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_XSPEED_H_

