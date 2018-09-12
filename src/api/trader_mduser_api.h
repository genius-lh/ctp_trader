
#ifndef _TRADER_MDUSER_API_H_
#define _TRADER_MDUSER_API_H_

#include <event2/util.h>
#include "trader_data.h"


#define ONFRONTCONNECTED 0
#define ONFRONTDISCONNECTED 1
#define ONHEARTBEATWARNING 2
#define ONRSPERROR 3
#define ONRSPUSERLOGIN 4
#define ONRSPUSERLOGOUT 5
#define ONRSPSUBMARKETDATA 6
#define ONRSPUNSUBMARKETDATA 7
#define ONRTNDEPTHMARKETDATA 8

typedef struct trader_mduser_evt_def trader_mduser_evt;
struct trader_mduser_evt_def {
  int Type;
  int ErrorCd;
  char ErrorMsg[81];
  // ÐÐÇé
  trader_tick Tick;
};


typedef struct trader_mduser_api_def trader_mduser_api;
typedef struct trader_mduser_api_method_def trader_mduser_api_method;

struct trader_mduser_api_def {
  evutil_socket_t fd;

  char* pBrokerID;
  char* pUser;
  char* pPwd;
  char* pAddress;
  char* pWorkspace;

  void* pUserApi;

  trader_mduser_api_method* pMethod;
};

struct trader_mduser_api_method_def {
  void (*xSetUser)(trader_mduser_api* self, char* broker_id, char* user, char* pwd);
  void (*xSetFrontAddr)(trader_mduser_api* self, char* addr);
  void (*xSetWorkspace)(trader_mduser_api* self, char* ws);
  void (*xStart)(trader_mduser_api* self);
  void (*xStop)(trader_mduser_api* self);
  void (*xLogin)(trader_mduser_api* self);
  void (*xLogout)(trader_mduser_api* self);
  void (*xSubscribe)(trader_mduser_api* self, char* instrument);
};

extern trader_mduser_api* trader_mduser_api_new(evutil_socket_t fd, trader_mduser_api_method* method);
extern void trader_mduser_api_free(trader_mduser_api* self);

extern void trader_mduser_api_set_user(trader_mduser_api* self, char* broker_id, char* user, char* pwd);
extern void trader_mduser_api_set_front_addr(trader_mduser_api* self, char* addr);
extern void trader_mduser_api_set_workspace(trader_mduser_api* self, char* ws);

extern void trader_mduser_api_on_front_connected(trader_mduser_api* self);
extern void trader_mduser_api_on_front_disconnected(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rsp_user_login(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rsp_user_logout(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rsp_error(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rsp_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rsp_un_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg);
extern void trader_mduser_api_on_rtn_depth_market_data(trader_mduser_api* self, trader_tick* tick);


#endif // _TRADER_MDUSER_API_H_

