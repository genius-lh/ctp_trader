#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/queue.h>

#include <mqueue.h>

#include "mqueue_client.h"

#define min(a, b)	(a) < (b) ? a : b

int mqueue_client_context_init(mqueue_client_context* ctx, const char* mqname)
{
  int ret = 0;
  strncpy(ctx->mq_name, mqname, sizeof(ctx->mq_name));
  ctx->retry = 5;
  ctx->ready = 0;
  time(&ctx->timestamp);

  ret = mqueue_client_context_get(ctx);

  return ret;
}

int mqueue_client_context_get(mqueue_client_context* ctx)
{
  int ret = 0;
  mqd_t mq_id;

  struct mq_attr attr;

  mq_id = mq_open(ctx->mq_name, O_RDWR | O_EXCL | O_NONBLOCK);

  if(mq_id < 0){
    return -(errno);
  }

  ret = mq_getattr(mq_id, &attr);
  if(ret < 0){
    ret = -(errno);
    mq_close(mq_id);
    return ret;
  }

  ctx->mq_id = mq_id;
  ctx->msg_size = (int)attr.mq_msgsize;
  ctx->ready = 1;

  return 0;

}

void mqueue_client_context_close(mqueue_client_context* ctx)
{
  int ret = 0;
  if(ctx->ready){
    ret = mq_close(ctx->mq_id);
    ctx->ready = 0;
  }
  return;
}

int mqueue_client_context_send(mqueue_client_context* ctx, const char* data, int len)
{
  int ret = 0;
  time_t now;
  if(!ctx->ready){
    time(&now);
    if((now - ctx->timestamp) < ctx->retry){
      // TODO
      
      return -1;
    }
    ctx->timestamp = now;
    ret = mqueue_client_context_get(ctx);
    if(ret < 0){
      // TODO

      return -2;
    }
  }
  
  ret = mq_send(ctx->mq_id, (char*)data, min(len, ctx->msg_size), 0);
  if(ret < 0){
    mqueue_client_context_close(ctx);
    return -3;
  }
  return ret;

}

