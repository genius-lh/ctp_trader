#ifndef _EVENT2_MQUEUE_H_
#define _EVENT2_MQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mqueue.h>

#include <sys/queue.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

typedef struct mqueue_context_def mqueue_context;
typedef struct mqueue_async_context_def mqueue_async_context;
typedef struct mqueue_buffer_def mqueue_buffer;

typedef struct mqueue_libevent_def mqueue_libevent;

typedef void (*mqueue_recv_callback)(const char*, int, void*);

struct mqueue_buffer_def {
  TAILQ_ENTRY(mqueue_buffer_def) next;
  int len;
  char data[1];
};

struct mqueue_context_def{
  char mq_name[32];
  mqd_t mq_id;
  int  msg_size;
  char* buff;
};

struct mqueue_async_context_def{
  mqueue_context c;

  /* Event library data and hooks */
  struct {
    void *data;

    /* Hooks that are called when the library expects to start
     * reading/writing. These functions should be idempotent. */
    void (*addRead)(void *privdata);
    void (*delRead)(void *privdata);
    void (*addWrite)(void *privdata);
    void (*delWrite)(void *privdata);
    void (*cleanup)(void *privdata);
  } ev;
  
  struct {
    void *data;
    mqueue_recv_callback callback;
  } recv;
  
  TAILQ_HEAD(mqueue_buffer_list, mqueue_buffer_def) bufflist;
};


struct mqueue_libevent_def{
  mqueue_async_context* context;
  struct event rev;
  struct event wev;
};

extern int mqueue_context_open(mqueue_context* ctx, const char* mqname, long maxmsg, long msgsize);
extern int mqueue_context_get(mqueue_context* ctx, const char* mqname);
extern void mqueue_context_close(mqueue_context* ctx);
extern void mqueue_context_unlink(const char* mqname);

extern int mqueue_context_send(mqueue_context* ctx, const char* data, int len);
extern int mqueue_context_recv(mqueue_context* ctx, char* data, int size);

extern mqueue_async_context* mqueue_async_context_open(const char* mqname, long maxmsg, long msgsize);
extern mqueue_async_context* mqueue_async_context_get(const char* mqname);
extern void mqueue_async_context_close(mqueue_async_context* ctx);
extern void mqueue_async_context_free(mqueue_async_context* ctx);

extern void mqueue_async_context_send(mqueue_async_context* ctx, const char* data, int len);
extern void mqueue_async_context_set_recv_callback(mqueue_async_context* ctx, mqueue_recv_callback fn, void *privdata);

extern int mqueue_async_context_libevent_attach(mqueue_async_context* ctx, struct event_base *base);

#ifdef __cplusplus
}
#endif

#endif

