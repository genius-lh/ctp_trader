/* ¹«¹²º¯Êý */
#ifndef _CMN_UTIL_H_
#define _CMN_UTIL_H_

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

extern void cmn_util_regist_signal(int* pLoop);
extern void cmn_util_change_proc(char* proc_name);

extern int cmn_util_bufferevent_recv(struct bufferevent* bev, unsigned char* data, int size);
extern int cmn_util_bufferevent_send(struct bufferevent* bev, unsigned char* data, int size);
extern int cmn_util_evbuffer_send(evutil_socket_t fd, unsigned char* data, int size);

extern int cmn_util_timeval_conv(char* timestamp, struct timeval* tv);


#include "queue.h"
typedef struct cmn_util_map_item_def cmn_util_map_item;
typedef struct cmn_util_map_def cmn_util_map;
typedef struct cmn_util_map_method_def cmn_util_map_method;

struct cmn_util_map_item_def {
  LIST_ENTRY(cmn_util_map_item_def) next;
  char* key;
  void* val;

};

struct cmn_util_map_def {
  int nSize;

  LIST_HEAD(cmn_util_map_item_list, cmn_util_map_item_def) hItems;
    
  cmn_util_map_method* pMethod;

};

typedef void* cmn_util_pvoid;

struct cmn_util_map_method_def {
  int (*xPut)(cmn_util_map* self, char* key, void* value);
  cmn_util_pvoid (*xGet)(cmn_util_map* self, char* key);
  int (*xRemove)(cmn_util_map* self, char* key);
};


extern cmn_util_map* cmn_util_map_new();
extern void cmn_util_map_free(cmn_util_map* self);



#endif //_CMN_UTIL_H_

