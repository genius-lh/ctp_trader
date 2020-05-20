
#ifndef _TRADER_MDUSER_SHM_HTTP_H_
#define _TRADER_MDUSER_SHM_HTTP_H_

#include <event2/event.h>
#include <event2/http.h>
#include "trader_mduser_shm.h"

typedef struct trader_mduser_shm_http_def trader_mduser_shm_http;
typedef struct trader_mduser_shm_http_method_def trader_mduser_shm_http_method;

struct trader_mduser_shm_http_def {
  struct event_base* base;
  struct evhttp* http;
  struct event* sig_int;
  struct event* sig_term;

  char hostname[16];
  unsigned short port;
  char server_name[16];
  int server_name_length;
  char shm_key_file[512];

  trader_mduser_shm_header* hdr;
  
  trader_mduser_shm_http_method* method;
};

struct trader_mduser_shm_http_method_def{
  int (*xInit)(trader_mduser_shm_http* self);
  int (*xRun)(trader_mduser_shm_http* self);
  int (*xProc)(trader_mduser_shm_http* self, const char* buff, int len, void* response);
  void (*xFini)(trader_mduser_shm_http* self);
};

extern trader_mduser_shm_http* trader_mduser_shm_http_new();

extern void trader_mduser_shm_http_free(trader_mduser_shm_http* self);

#endif //_TRADER_MDUSER_SHM_HTTP_H_

