#ifndef _CLIENT_AUD_SVR_H_
#define _CLIENT_AUD_SVR_H_

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "client_aud_cnn.h"
#include "client_aud_pool.h"
#include "queue.h"
#include "cmn_util.h"

typedef struct client_aud_svr_def client_aud_svr;
typedef struct client_aud_svr_method_def client_aud_svr_method;

struct client_aud_svr_def {
  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;
  
  struct event* pTmrGc;
  
  struct evconnlistener *pListener;
  char sIp[15+1];
  int nPort;
  
  int nPoolSize;
  client_aud_cnn* pCnnPool;
  client_aud_pool* pBufPool;
  
  
  TAILQ_HEAD(client_aud_cnn_free_list, client_aud_cnn_def) hCnnFreeList;
  TAILQ_HEAD(client_aud_cnn_used_list, client_aud_cnn_def) hCnnUsedList;

  cmn_util_map* pUserMap;
	
  client_aud_svr_method* pMethod;

};

struct client_aud_svr_method_def {
  int (*xInit)(client_aud_svr* self, evutil_socket_t* socks, int num);
  int (*xRun)(client_aud_svr* self);
  int (*xAccept)(client_aud_svr* self, evutil_socket_t fd);
  int (*xOnTraderEvent)(client_aud_svr* self, client_aud_cnn* cnn);
  int (*xOnClientEvent)(client_aud_svr* self, client_aud_cnn* cnn);
  int (*xExit)(client_aud_svr* self);
  int (*xGC)(client_aud_svr* self);
};


extern client_aud_svr* client_aud_svr_new();
extern void client_aud_svr_free(client_aud_svr* self);


extern int aud_main(char* cfg_file, evutil_socket_t* socks, int num);

#endif

