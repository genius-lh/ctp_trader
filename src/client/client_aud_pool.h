#ifndef _CLIENT_AUD_POOL_H_
#define _CLIENT_AUD_POOL_H_

#include <event2/bufferevent.h>

#include "hiredis.h"

#include "queue.h"

#include "trader_msg_struct.h"

enum {
  CLIENT_AUD_TRADER_STATUS_EMPTY = 0,
  CLIENT_AUD_TRADER_STATUS_LOGINED,
  CLIENT_AUD_TRADER_STATUS_LOGOUTED,
};

typedef struct client_aud_trader_def client_aud_trader;

typedef struct client_aud_pool_def client_aud_pool;
typedef struct client_aud_pool_method_def client_aud_pool_method;

struct client_aud_trader_def {
  TAILQ_ENTRY(client_aud_trader_def) next;
  int nStatus; //0 - not login 1 -logined 2 - logouted
  char UserId[TraderUserIDTypeLen];
  char Passwd[TraderPasswordTypeLen];
  struct bufferevent *BufTrader;
};

struct client_aud_pool_def {

  TAILQ_HEAD(client_aud_trader_list, client_aud_trader_def) hCnnTraderList;


  redisContext* redis;
  
  client_aud_pool_method* pMethod;
};

struct client_aud_pool_method_def {
  int (*xAddTrader)(client_aud_pool* self, struct bufferevent* buff_trader);
  int (*xGetTrader)(client_aud_pool* self, struct bufferevent** buff_trader, char* user_id, char* passwd);
  int (*xFreeTrader)(client_aud_pool* self, char* user_id);
};

extern client_aud_pool* client_aud_pool_new();
extern void client_aud_pool_free(client_aud_pool* self);


#endif //_CLIENT_AUD_POOL_H_

