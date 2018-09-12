#ifndef CTP_TRADER_API_H_
#define CTP_TRADER_API_H_

#include <event2/util.h>
#include "trader_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ctp_trader_api_method_def ctp_trader_api_method;
typedef struct ctp_trader_api_def ctp_trader_api;

struct ctp_trader_api_def {
  evutil_socket_t fd;
  void* pUserApi;
  void* pHandler;

  int bConnected;
  int bInit;

  char* BrokenId;
  char* FrontAddress;

  char HedgeFlag;
  
  char UserId[16];
  char Passwd[41];
  char InvestorID[19];
  char MaxOrderLocalID[TraderOrderRefTypeLen];

  int nLocalId;
  ctp_trader_api_method* pMethod;
};

struct ctp_trader_api_method_def{
  int (*xStart)(ctp_trader_api* self, char* user_id, char* pwd);
  int (*xStop)(ctp_trader_api* self);
  int (*xOrderInsert)(ctp_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  int (*xOrderAction)(ctp_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
  int (*xQryInstrument)(ctp_trader_api* self);
  int (*xQryUserInvestor)(ctp_trader_api* self);
  int (*xGetTradingDay)(ctp_trader_api* self, char* tradingday);
  int (*xGetMaxOrderLocalID)(ctp_trader_api* self, char* local_id);
  int (*xQrySettlementInfoConfirm)(ctp_trader_api* self);
  int (*xQryInvestorPosition)(ctp_trader_api* self);
};

extern ctp_trader_api* ctp_trader_api_new(evutil_socket_t fd);
extern void ctp_trader_api_free(ctp_trader_api* self);


#ifdef __cplusplus
}
#endif

#endif //CTP_TRADER_API_H_

