#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "cmn_util.h"

#include <sys/prctl.h>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/time.h> 

void cmn_util_change_proc(char* proc_name)
{
  prctl( PR_SET_NAME, proc_name, 0, 0, 0);
}


static int** cmn_util_loop_value_get();

static void cmn_util_sig_term(int signo);

void cmn_util_regist_signal(int* pLoop)
{
  int** ppiLoopValue = cmn_util_loop_value_get();

  *ppiLoopValue = pLoop;
  
  if(signal(SIGTERM, cmn_util_sig_term)==SIG_ERR){
    exit(-1);
  }
  
  if(signal(SIGINT, cmn_util_sig_term)==SIG_ERR){
    exit(-1);
  }
  
}

int** cmn_util_loop_value_get()
{
  static int* stLoopValue = 0;

  return &stLoopValue;
}

void cmn_util_sig_term(int signo)
{
  int** ppiLoopValue = cmn_util_loop_value_get();
  int *piLoop = *ppiLoopValue;
  *piLoop = 0;
}

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#define CMN_UTIL_BUFFER_HEADER_SIZE 4

int cmn_util_bufferevent_recv(struct bufferevent* bev, unsigned char* data, int size)
{
  struct evbuffer* pEvBuf = bufferevent_get_input(bev);
  int nLen = 0;
  int nMsgLen = 0;
  int nRet = 0;
  unsigned char sLength[CMN_UTIL_BUFFER_HEADER_SIZE];
  do {
    nLen = evbuffer_copyout(pEvBuf, sLength, sizeof(sLength));
    if(nLen < CMN_UTIL_BUFFER_HEADER_SIZE){
      // 数据读取未完成，等待
      nRet = -1;
      break;
    }
    
    // 解长度
    nMsgLen = (sLength[0] - '0') * 1000;
    nMsgLen += (sLength[1] - '0') * 100;
    nMsgLen += (sLength[2] - '0') * 10;
    nMsgLen += (sLength[3] - '0');
    
    nLen = evbuffer_get_length(pEvBuf);
    if(nLen < (nMsgLen + CMN_UTIL_BUFFER_HEADER_SIZE)){
      // 数据读取未完成，等待
      nRet = -1;
      break;
    }

    if(nMsgLen > size){
      // 数据超长
      nRet = -2;
      break;
    }

    // 读取长度
    bufferevent_read(bev, sLength, CMN_UTIL_BUFFER_HEADER_SIZE);

    // 读取报文体
    bufferevent_read(bev, data, nMsgLen);

    nRet = nMsgLen;

  }while(0);

  return nRet;

}

static  void cmn_util_evbuffer_fill(struct evbuffer* buffer,unsigned char* data, int size);
void cmn_util_evbuffer_fill(struct evbuffer* buffer,unsigned char* data, int size)
{
  struct evbuffer* pEvBuf = buffer;
  // 长度处理
  unsigned char sLength[CMN_UTIL_BUFFER_HEADER_SIZE + 1];
  snprintf(sLength, sizeof(sLength), "%04d", size);

  // 组长度
  evbuffer_add(pEvBuf, sLength, CMN_UTIL_BUFFER_HEADER_SIZE);
  
  // 组报文体
  evbuffer_add(pEvBuf, data, size);

  return;
}


int cmn_util_bufferevent_send(struct bufferevent* bev, unsigned char* data, int size)
{
  int nRet = 0;
  if(!bev){
    return -1;
  }
  
  struct evbuffer* pEvBuf = evbuffer_new();

  // 组包
  cmn_util_evbuffer_fill(pEvBuf, data, size);

  // 发送
  nRet = bufferevent_write_buffer(bev, pEvBuf);

  bufferevent_enable(bev, EV_WRITE);
  
  evbuffer_free(pEvBuf);

  return nRet;
}


int cmn_util_evbuffer_send(evutil_socket_t fd, unsigned char* data, int size)
{
  int nRet = 0;
   struct evbuffer* pEvBuf = evbuffer_new();
   
   // 组包
   cmn_util_evbuffer_fill(pEvBuf, data, size);
  
   // 发送
   nRet = evbuffer_write(pEvBuf, fd);
   
   evbuffer_free(pEvBuf);
  
   return nRet;

}

int cmn_util_timeval_conv(char* timestamp, struct timeval* tv)
{
  struct tm now;
  localtime_r(&tv->tv_sec, &now);    

  sprintf(timestamp, "%04d-%02d-%02d-%02d.%02d.%02d.%06d",
    now.tm_year+1900, now.tm_mon+1, now.tm_mday,
    now.tm_hour, now.tm_min, now.tm_sec, tv->tv_usec);
  return 0;
}


#include "queue.h"

static cmn_util_map_item* cmn_util_map_item_search(cmn_util_map* self, char* key);
static int   cmn_util_map_put(cmn_util_map* self, char* key, void* value);
static void* cmn_util_map_get(cmn_util_map* self, char* key);
static int   cmn_util_map_remove(cmn_util_map* self, char* key);
static cmn_util_map_method* cmn_util_map_method_get();

cmn_util_map_item* cmn_util_map_item_search(cmn_util_map* self, char* key)
{
  cmn_util_map_item* res = NULL;
  cmn_util_map_item* item;

  LIST_FOREACH(item, &self->hItems, next) {
    if(!strcmp(item->key, key)){
      res = item;
      break;
    }
  }
  return res;
}

int   cmn_util_map_put(cmn_util_map* self, char* key, void* value)
{
  cmn_util_map_item* it = cmn_util_map_item_search(self, key);
  if(it){
    return -1;
  }

  it = (cmn_util_map_item*)malloc(sizeof(cmn_util_map_item));

  it->key = (char*)malloc(sizeof(char) * ( strlen(key)+1));
  it->val = value;

  strcpy(it->key, key);

  LIST_INSERT_HEAD(&self->hItems, it, next);

  return 0;
}

void* cmn_util_map_get(cmn_util_map* self, char* key)
{
  cmn_util_map_item* it = cmn_util_map_item_search(self, key);
  if(!it){
    return NULL;
  }

  return it->val;

}

int   cmn_util_map_remove(cmn_util_map* self, char* key)
{
  cmn_util_map_item* it = cmn_util_map_item_search(self, key);
  if(!it){
    return -1;
  }
  LIST_REMOVE(it, next);
  free(it->key);
  free(it);
  return 0;
}


cmn_util_map_method* cmn_util_map_method_get()
{
  static cmn_util_map_method st_cmn_util_map_method = {
    cmn_util_map_put,
    cmn_util_map_get,
    cmn_util_map_remove
  };

  return &st_cmn_util_map_method;
}


cmn_util_map* cmn_util_map_new()
{
  cmn_util_map* self = (cmn_util_map*)malloc(sizeof(cmn_util_map));

  LIST_INIT(&self->hItems);
  
  self->nSize = 0;

  self->pMethod = cmn_util_map_method_get();

  return self;

}

void cmn_util_map_free(cmn_util_map* self)
{
  cmn_util_map_item* item;

  
  while((item = LIST_FIRST(&self->hItems))!=NULL){
    LIST_REMOVE(item, next);
    free(item->key);
    free(item);
  }

  if(self){
    free(self);
  }

}






