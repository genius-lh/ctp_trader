
#ifndef _TRADER_MDUSER_CLIENT_H_
#define _TRADER_MDUSER_CLIENT_H_

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>

typedef struct trader_mduser_client_def trader_mduser_client;
typedef struct trader_mduser_client_method_def trader_mduser_client_method;

typedef void (*trader_mduser_client_connect_callback)(void* user_data);
typedef void (*trader_mduser_client_disconnect_callback)(void* user_data);
typedef void (*trader_mduser_client_recv_callback)(void* user_data, void* data, int len);

struct trader_mduser_client_def {
  
  struct event_base* base;
  struct bufferevent *bev;

  char ip[15];
  int port;

  trader_mduser_client_connect_callback connect_cb;
  trader_mduser_client_disconnect_callback disconnect_cb;
  trader_mduser_client_recv_callback recv_data_cb;
  void* user_data;

  trader_mduser_client_method* method;

};

struct trader_mduser_client_method_def {
  int (*xInit)(trader_mduser_client* self, struct event_base* base, char* ip, int port,
    trader_mduser_client_connect_callback connect_cb,
    trader_mduser_client_disconnect_callback disconnect_cb,
    trader_mduser_client_recv_callback recv_data_cb,
    void* user_data);
  int (*xConnect)(trader_mduser_client* self);
  int (*xOnEvent)(trader_mduser_client* self);
  int (*xOnRecv)(trader_mduser_client* self);
  int (*xExit)(trader_mduser_client* self);
};

extern trader_mduser_client* trader_mduser_client_new();
extern void trader_mduser_client_free(trader_mduser_client* self);


#endif //_TRADER_MDUSER_CLIENT_H_

