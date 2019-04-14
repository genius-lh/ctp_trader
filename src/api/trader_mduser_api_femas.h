
#ifndef _TRADER_MDUSER_API_FEMAS_H_
#define _TRADER_MDUSER_API_FEMAS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "trader_mduser_api.h"

typedef struct trader_mduser_api_femas_def  trader_mduser_api_femas;
struct trader_mduser_api_femas_def {
  void* pMdApi;
  void* pHandler;
  int nRequestID;
};

extern trader_mduser_api_method* trader_mduser_api_femas_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_MDUSER_API_FEMAS_H_

