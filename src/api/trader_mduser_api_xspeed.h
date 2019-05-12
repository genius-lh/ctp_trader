#ifndef _TRADER_MDUSER_API_XSPEED_H_
#define _TRADER_MDUSER_API_XSPEED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_mduser_api.h"

typedef struct trader_mduser_api_xspeed_def  trader_mduser_api_xspeed;
struct trader_mduser_api_xspeed_def {
  void* pMdApi;
  void* pHandler;
  int nRequestID;
  char instPrefix[3];
};

extern trader_mduser_api_method* trader_mduser_api_xspeed_stock_method_get();
extern trader_mduser_api_method* trader_mduser_api_xspeed_sop_method_get();
#ifdef __cplusplus
}
#endif

#endif //_TRADER_MDUSER_API_XSPEED_H_

