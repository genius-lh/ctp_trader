#ifndef _TRADER_MDUSER_CNN_H_
#define _TRADER_MDUSER_CNN_H_

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "trader_data.h"
#include "trader_mduser_api.h"

typedef void (*trader_mduser_cnn_tick_callback)(void* arg, trader_mduser_evt* evt);
typedef struct trader_mduser_cnn_def trader_mduser_cnn;
typedef struct trader_mduser_cnn_method_def trader_mduser_cnn_method;
struct trader_mduser_cnn_def {
  struct event_base *pBase;
  char sBrokerId[16];
  char sUser[16];
  char sPasswd[16];
  char sAddr[64];
  char sWorkspace[64];

  trader_mduser_cnn_tick_callback tickCallback;

  void* userData;
  
  trader_instrument_id_type* instruments;
  int instrumentNumber;

  trader_mduser_api_method* pApiImp;

  
  trader_mduser_api* pMduserApi;

  struct bufferevent* pBufMduser;

  evutil_socket_t nPair;

  trader_mduser_cnn_method* pMethod;

};

struct trader_mduser_cnn_method_def {
  int (*xInit)(trader_mduser_cnn* self,  struct event_base* base,
                char* broker_id, char* user, char* pwd, char* addr, char* ws,
                trader_mduser_cnn_tick_callback tick_callback, void* user_data,
                trader_instrument_id_type* insts, int inst_num,
                trader_mduser_api_method* api_imp);
  
  int (*xStart)(trader_mduser_cnn* self);
  int (*xProc)(trader_mduser_cnn* self, trader_mduser_evt* evt);
  int (*xStop)(trader_mduser_cnn* self);
};


extern trader_mduser_cnn* trader_mduser_cnn_new();
extern void trader_mduser_cnn_free(trader_mduser_cnn* self);

#endif // _TRADER_MDUSER_CNN_H_

