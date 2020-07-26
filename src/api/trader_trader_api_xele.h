
#ifndef _TRADER_TRADER_API_XELE_H_
#define _TRADER_TRADER_API_XELE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

typedef struct trader_trader_api_xele_def trader_trader_api_xele;

struct trader_trader_api_xele_def {
  void* pQueryApi;
  void* pQueryHandler;
  void* pTraderApi;
  void* pTraderHandler;
  char sTradingDay[9];
  char sMaxOrderLocalID[13];
  char sClientID[11];
  int nTraderRequestID;
};

extern trader_trader_api_method* trader_trader_api_xele_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_XELE_H_

