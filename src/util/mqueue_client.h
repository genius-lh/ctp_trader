#ifndef _MQUEUE_CLIENT_H_
#define _MQUEUE_CLIENT_H_

#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <sys/time.h>

typedef struct mqueue_client_context_def mqueue_client_context;

struct mqueue_client_context_def{
  char mq_name[32];
  mqd_t mq_id;
  int msg_size;
  int retry;
  int ready;
  time_t timestamp;
};

extern int mqueue_client_context_init(mqueue_client_context* ctx, const char* mqname);
extern int mqueue_client_context_get(mqueue_client_context* ctx);
extern void mqueue_client_context_close(mqueue_client_context* ctx);
extern int mqueue_client_context_send(mqueue_client_context* ctx, const char* data, int len);

#endif //_MQUEUE_CLIENT_H_

