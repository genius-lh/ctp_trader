
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/queue.h>

#include <mqueue.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

#include "evmqueue.h"

#define min(a, b)	(a) < (b) ? a : b

int mqueue_context_open(mqueue_context* ctx, const char* mqname, long maxmsg, long msgsize)
{
  mqd_t mq_id;

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = maxmsg;
  attr.mq_msgsize = msgsize;
  attr.mq_curmsgs = 0;

  mq_id = mq_open(mqname, O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK, 0600, &attr);

  if(mq_id < 0){
    return -(errno);
  }

  ctx->mq_id = mq_id;
  strncpy(ctx->mq_name, mqname, sizeof(ctx->mq_name));
  ctx->msg_size = (int)msgsize;
  ctx->buff = (char*)malloc(msgsize);

  return 0;
}

int mqueue_context_get(mqueue_context* ctx, const char* mqname)
{
  int ret = 0;
  mqd_t mq_id;

  struct mq_attr attr;

  mq_id = mq_open(mqname, O_RDWR | O_EXCL | O_NONBLOCK);

  if(mq_id < 0){
    return -(errno);
  }

  ret = mq_getattr(mq_id, &attr);
  if(ret < 0){
    return -(errno);
  }

  ctx->mq_id = mq_id;
  strncpy(ctx->mq_name, mqname, sizeof(ctx->mq_name));
  ctx->msg_size = (int)attr.mq_msgsize;
  ctx->buff = (char*)malloc(attr.mq_msgsize);

  return 0;


}

void mqueue_context_close(mqueue_context* ctx)
{
  int ret = 0;
  ret = mq_close(ctx->mq_id);
  if(ctx->buff){
    free(ctx->buff);
  }
  return;
}

void mqueue_context_unlink(const char* mqname)
{
  int ret = 0;
  ret = mq_unlink(mqname);
  return;
}

int mqueue_context_send(mqueue_context* ctx, const char* data, int len)
{
  int ret = 0;
  ret = mq_send(ctx->mq_id, (char*)data, min(len, ctx->msg_size), 0);
  if(ret < 0){
    return -(errno);
  }
  return ret;
}

int mqueue_context_recv(mqueue_context* ctx, char* data, int size)
{
  int ret = 0;
  ret = mq_receive(ctx->mq_id, data, size, NULL);
  if(ret < 0){
    return -(errno);
  }

  return ret;
}

static int mqueue_async_context_init(mqueue_async_context* self);
static void mqueue_async_context_handle_read(mqueue_async_context* self);
static void mqueue_async_context_handle_write(mqueue_async_context* self);

mqueue_async_context* mqueue_async_context_open(const char* mqname, long maxmsg, long msgsize)
{
  int ret = 0;
  mqueue_async_context* self = (mqueue_async_context*)malloc(sizeof(mqueue_async_context));
  memset(self, 0, sizeof(*self));
  ret = mqueue_context_open(&self->c, mqname, maxmsg, msgsize);
  if(ret < 0){
    mqueue_async_context_free(self);
    return (mqueue_async_context*)NULL;
  }

  ret = mqueue_async_context_init(self);
  if(ret < 0){
    mqueue_async_context_free(self);
    return (mqueue_async_context*)NULL;
  }
  
  return self;
}

mqueue_async_context* mqueue_async_context_get(const char* mqname)
{
  int ret = 0;
  mqueue_async_context* self = (mqueue_async_context*)malloc(sizeof(mqueue_async_context));
  memset(self, 0, sizeof(*self));
  ret = mqueue_context_get(&self->c, mqname);
  if(ret < 0){
    mqueue_async_context_free(self);
    return (mqueue_async_context*)NULL;
  }

  ret = mqueue_async_context_init(self);
  if(ret < 0){
    mqueue_async_context_free(self);
    return (mqueue_async_context*)NULL;
  }
  
  return self;
}

void mqueue_async_context_close(mqueue_async_context* self)
{
  if(!self){
    return;
  }

  if(self->ev.cleanup){
    self->ev.cleanup(self->ev.data);
  }
  
  mqueue_context_close(&self->c);

  return ;
}


void mqueue_async_context_free(mqueue_async_context* self)
{
  if(self){
    free(self);
  }
  return ;
}

void mqueue_async_context_send(mqueue_async_context* self, const char* data, int len)
{
  if(!self){
    return ;
  }
  
  mqueue_buffer* buff = (mqueue_buffer*)malloc(sizeof(mqueue_buffer) + self->c.msg_size);
  buff->len = min(self->c.msg_size, len);
  memcpy(buff->data, data, buff->len);

  TAILQ_INSERT_TAIL(&self->bufflist, buff, next);

  if(self->ev.addWrite){
    self->ev.addWrite(self->ev.data);
  }
  return;
}

void mqueue_async_context_set_recv_callback(mqueue_async_context* self, mqueue_recv_callback fn, void *privdata)
{
  mqueue_context *c = &(self->c);
  self->recv.data = privdata;
  self->recv.callback = fn;

  if(self->ev.addRead){
    self->ev.addRead(self->ev.data);
  }
  return;
}

int mqueue_async_context_init(mqueue_async_context* self)
{
  TAILQ_INIT(&self->bufflist);
  
  return 0;
}

void mqueue_async_context_handle_read(mqueue_async_context* self)
{
  do{
    mqueue_context *c = &(self->c);
    int ret = mqueue_context_recv(c, c->buff, c->msg_size);
    if(ret < 0){
      break;
    }

    if(self->recv.callback){
      self->recv.callback(c->buff, ret, self->recv.data);
    }
  }while(0);
  
  if(self->ev.addRead){
    self->ev.addRead(self->ev.data);
  }

  return ;
}

void mqueue_async_context_handle_write(mqueue_async_context* self)
{
  do{
    mqueue_buffer* buff = TAILQ_FIRST(&self->bufflist);
    if(!buff){
      break;
    }

    TAILQ_REMOVE(&self->bufflist, buff, next);

    mqueue_context *c = &(self->c);
    int ret = mqueue_context_send(c, buff->data, buff->len);
    (void)ret;

    free(buff);

    if(!TAILQ_EMPTY(&self->bufflist)){
      if(self->ev.addWrite){
        self->ev.addWrite(self->ev.data);
      }
    }
  }while(0);

  return ;
}

static void mqueue_libevent_read_event(int fd, short event, void *arg);
static void mqueue_libevent_write_event(int fd, short event, void *arg);

static void mqueue_libevent_add_read(void *privdata);
static void mqueue_libevent_del_read(void *privdata);
static void mqueue_libevent_add_write(void *privdata);
static void mqueue_libevent_del_write(void *privdata);
static void mqueue_libevent_cleanup(void *privdata);

int mqueue_async_context_libevent_attach(mqueue_async_context* self, struct event_base *base)
{
  mqueue_context *c = &(self->c);
  mqueue_libevent *e;

  /* Create container for context and r/w events */
  e = (mqueue_libevent*)malloc(sizeof(*e));
  e->context = self;
  
  /* Register functions to start/stop listening for events */
  self->ev.addRead = mqueue_libevent_add_read;
  self->ev.delRead = mqueue_libevent_del_read;
  self->ev.addWrite = mqueue_libevent_add_write;
  self->ev.delWrite = mqueue_libevent_del_write;
  self->ev.cleanup = mqueue_libevent_cleanup;
  self->ev.data = e;
  
  /* Initialize and install read/write events */
  event_set(&e->rev, c->mq_id, EV_READ, mqueue_libevent_read_event, e);
  event_set(&e->wev, c->mq_id, EV_WRITE, mqueue_libevent_write_event, e);
  event_base_set(base,&e->rev);
  event_base_set(base,&e->wev);
  return 0;
}

void mqueue_libevent_read_event(int fd, short event, void *arg)
{
  mqueue_libevent *e = (mqueue_libevent*)arg;
  mqueue_async_context_handle_read(e->context);
}

void mqueue_libevent_write_event(int fd, short event, void *arg)
{
  mqueue_libevent *e = (mqueue_libevent*)arg;
  mqueue_async_context_handle_write(e->context);
}

void mqueue_libevent_add_read(void *privdata)
{
  mqueue_libevent *e = (mqueue_libevent*)privdata;
  event_add(&e->rev,NULL);
}

void mqueue_libevent_del_read(void *privdata)
{
  mqueue_libevent *e = (mqueue_libevent*)privdata;
  event_del(&e->rev);
}

void mqueue_libevent_add_write(void *privdata)
{
  mqueue_libevent *e = (mqueue_libevent*)privdata;
  event_add(&e->wev,NULL);
}

void mqueue_libevent_del_write(void *privdata)
{
  mqueue_libevent *e = (mqueue_libevent*)privdata;
  event_del(&e->wev);
}

void mqueue_libevent_cleanup(void *privdata)
{
  mqueue_libevent *e = (mqueue_libevent*)privdata;
  event_del(&e->rev);
  event_del(&e->wev);
  free(e);
}


