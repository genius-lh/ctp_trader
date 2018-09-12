
#ifndef _CLIENT_AUD_CNN_H_
#define _CLIENT_AUD_CNN_H_

#include "queue.h"
#include <event2/bufferevent.h>

#include "trader_msg_struct.h"
#include "cmn_util.h"

#include "client_aud_pool.h"

#define CLIENT_KEY_CMD "cmd"
#define CLIENT_KEY_TRACENO "transno"


typedef struct client_aud_cnn_def client_aud_cnn;
typedef struct client_aud_cnn_method_def client_aud_cnn_method;

struct client_aud_cnn_def {
  TAILQ_ENTRY(client_aud_cnn_def) next;
  
  client_aud_cnn_method* pMethod;

  // 对交易进程
  struct bufferevent *BufTrader;

  // 对客户端进程
  struct bufferevent *BufClient;
  
  void* parent;

  client_aud_pool* pool;

  cmn_util_map* pUserMap;

  // 登录信息
  int bLogin;
  char UserId[TraderUserIDTypeLen];
  char Passwd[TraderPasswordTypeLen];

  int nFree;
  struct timeval tvLast;
};

struct client_aud_cnn_method_def {
  int (*xOnTraderRecv)(client_aud_cnn* self);
  int (*xOnTraderSend)(client_aud_cnn* self);
  int (*xOnClientRecv)(client_aud_cnn* self);
  int (*xOnClientSend)(client_aud_cnn* self);
  int (*xOnClose)(client_aud_cnn* self);
};

extern client_aud_cnn* client_aud_cnn_new();
extern void client_aud_cnn_free(client_aud_cnn* self);



#endif //_CLIENT_AUD_CNN_H_

