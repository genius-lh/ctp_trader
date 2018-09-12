
#ifndef _TRADER_MDUSER_BOARDCAST_H_
#define _TRADER_MDUSER_BOARDCAST_H_

#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "queue.h"

typedef struct trader_mduser_boardcast_cnn_def trader_mduser_boardcast_cnn;
typedef struct trader_mduser_boardcast_def trader_mduser_boardcast;
typedef struct trader_mduser_boardcast_method_def trader_mduser_boardcast_method;

struct trader_mduser_boardcast_cnn_def {
  TAILQ_ENTRY(trader_mduser_boardcast_cnn_def) next;
  
  struct bufferevent *be;

  trader_mduser_boardcast* parent;
};


struct trader_mduser_boardcast_def {
  struct event_base* base;
  struct evconnlistener* listener;
  char ip[15];
  int port;
  
  TAILQ_HEAD(trader_mduser_boardcast_cnn_list, trader_mduser_boardcast_cnn_def) cnnList;

  trader_mduser_boardcast_method* method;

};

struct trader_mduser_boardcast_method_def {
  int (*xInit)(trader_mduser_boardcast* self, struct event_base* base, char* ip, int port);
  int (*xAccept)(trader_mduser_boardcast* self, evutil_socket_t fd);
  int (*xOnCnnEvent)(trader_mduser_boardcast* self, trader_mduser_boardcast_cnn* cnn);
  int (*xBoardcase)(trader_mduser_boardcast* self, char* buff, int len);
  int (*xExit)(trader_mduser_boardcast* self);
};

extern trader_mduser_boardcast* trader_mduser_boardcast_new();
extern void trader_mduser_boardcast_free(trader_mduser_boardcast* self);

#endif //_TRADER_MDUSER_BOARDCAST_H_

