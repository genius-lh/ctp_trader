
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <signal.h>
#include <limits.h>
#include <float.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <time.h>

#include "trader_mduser_shm_http.h"
#include "cmn_log.h"
#include "trader_mduser_shm.h"
#include "trader_data.h"

#define HTTP_SHM_TEST_FILE "IB.MDUSER.INSTRUMENTS"

#define HTTP_SND_CMD "/web/send"

#define HTTP_SND_PORT 8000

static int  trader_mduser_shm_http_init(trader_mduser_shm_http* self);
static int  trader_mduser_shm_http_run(trader_mduser_shm_http* self);
static int  trader_mduser_shm_http_proc(trader_mduser_shm_http* self, const char* buff, int len, void* response);
static void  trader_mduser_shm_http_fini(trader_mduser_shm_http* self);

static trader_mduser_shm_http_method* trader_mduser_shm_http_method_get();

// callback
static void trader_mduser_shm_http_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_mduser_shm_http_post_cb(struct evhttp_request *req, void *arg);

static void trader_mduser_shm_http_calc(trader_mduser_shm_http* self, void* response, const char* t1, const char* t2, const char* w1, const char* w2);

trader_mduser_shm_http_method* trader_mduser_shm_http_method_get()
{
  static trader_mduser_shm_http_method trader_mduser_shm_http_method_st = {
    trader_mduser_shm_http_init,
    trader_mduser_shm_http_run,
    trader_mduser_shm_http_proc,
    trader_mduser_shm_http_fini
  };

  return &trader_mduser_shm_http_method_st;
}

// 初始化
int trader_mduser_shm_http_init(trader_mduser_shm_http* self)
{
  CMN_DEBUG("Enter!\n");

  self->base = event_base_new();

  self->http = evhttp_new(self->base);
  
  self->sig_int = evsignal_new(self->base, SIGINT, trader_mduser_shm_http_signal_cb, (void *)self);
  
  self->sig_term = evsignal_new(self->base, SIGTERM, trader_mduser_shm_http_signal_cb, (void *)self);
  
  trader_mduser_shm_key_file(self->shm_key_file);
  self->hdr = trader_mduser_shm_header_at();

  return 0;
}

// 主循环
int trader_mduser_shm_http_run(trader_mduser_shm_http* self)
{
  CMN_DEBUG("Enter!\n");

  // 绑定http 端口
  int ret = 0; 
  ret = evhttp_bind_socket(self->http, self->hostname, self->port);
  CMN_ASSERT(!ret);
  
  // 绑定http  接口
	ret = evhttp_set_cb(self->http, self->server_name, trader_mduser_shm_http_post_cb, (void*)self);

  // signal 事件
  ret = event_add(self->sig_int, NULL);
  ret = event_add(self->sig_term, NULL);

  ret = event_base_dispatch(self->base);

  return 0;
}

// 报文处理
int trader_mduser_shm_http_proc(trader_mduser_shm_http* self, const char* buff, int len, void* response)
{ 
  CMN_DEBUG("Enter!\n");

  CMN_DEBUG("[\n%s\n]\n", buff);
  const char* pQuery = &buff[self->server_name_length];

  struct evbuffer* evresp = (struct evbuffer*)response;

  // 解析参数
  int nRet = 0;
  struct evkeyvalq headers;
  do{
    nRet = evhttp_parse_query_str(pQuery, &headers);
    
  	const char *t1 = evhttp_find_header(&headers, "p1");
  	const char *t2 = evhttp_find_header(&headers, "p2");
  	const char *w1 = evhttp_find_header(&headers, "x");
  	const char *w2 = evhttp_find_header(&headers, "y");

    trader_mduser_shm_http_calc(self, response, t1, t2, w1, w2);

  	evhttp_clear_headers(&headers);
  }while(0);

  return 0;
}

void trader_mduser_shm_http_calc(trader_mduser_shm_http* self, void* response, const char* t1, const char* t2, const char* w1, const char* w2)
{
  int nRet = 0;
  const char SUCCESS_RESPONSE[] = "{\"success\":true,\"data\":[{\"asix\":%ld,\"sell\":%lf,\"buy\":%lf}] }";
  const char FAILED_RESPONSE[] = "{\"success\":false}";

  do{
    trader_tick* t1Tick = NULL;
    trader_tick* t2Tick = NULL;
    trader_tick* t3Tick = NULL;
    double t1Weight = atof(w1);
    double t2Weight = atof(w2);
    double d1;
    double d2;
    long asix;
    struct evbuffer *evb = (struct evbuffer *)response;
    trader_mduser_shm_header* hdr = self->hdr;
    trader_tick* tick = (trader_tick*)hdr->pData;
    int i;
    for(i = 0; i < hdr->nFieldNum; i++){
      if(!strcmp(tick->InstrumentID, t1)){
        t1Tick = tick;
      }else if(!strcmp(tick->InstrumentID, t2)){
        t2Tick = tick;
      }else if(!strcmp(tick->InstrumentID, USDCNH)){
        t3Tick = tick;
      }
      tick++;
    }

    if(t1Tick && t2Tick){
      if((DBL_MAX != t1Tick->BidPrice1)
      &&(DBL_MAX != t1Tick->AskPrice1)
      &&(DBL_MAX != t2Tick->BidPrice1)
      &&(DBL_MAX != t2Tick->AskPrice1)
      &&(0 != t1Tick->AskPrice1)
      &&(0 != t2Tick->AskPrice1)){

        if((!memcmp(t1, "GC", 2)) || (!memcmp(t1, "SI", 2))) {
          d1 = t1Weight * t3Tick->BidPrice1 * t1Tick->AskPrice1 / OZ - t2Weight * t2Tick->AskPrice1;
          d2 = t1Weight * t3Tick->AskPrice1 * t1Tick->BidPrice1 / OZ - t2Weight * t2Tick->BidPrice1;
        }else{
          d1 = t1Weight * t1Tick->AskPrice1 - t2Weight * t2Tick->AskPrice1;
          d2 = t1Weight * t1Tick->BidPrice1 - t2Weight * t2Tick->BidPrice1;
        }

        // 价差变换
        //d1 /= t2Weight;
        //d2 /= t2Weight;
        asix = time(NULL) * 1000;

        evbuffer_add_printf(evb, SUCCESS_RESPONSE, asix, d1, d2);
        break;
      }
    }

    evbuffer_add_printf(evb, FAILED_RESPONSE);
  }while(0);

  return ;
}

// 释放
void trader_mduser_shm_http_fini(trader_mduser_shm_http* self)
{
  CMN_DEBUG("Enter!\n");

  trader_mduser_shm_header_dt(self->hdr);

  event_free(self->sig_term);
  event_free(self->sig_int);

  evhttp_free(self->http);

  event_base_free(self->base);

  return;
}

// 退出
void trader_mduser_shm_http_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_shm_http* self = (trader_mduser_shm_http*)arg;

  struct event_base *base = self->base;
  struct timeval delay = { 1, 0 };

  // 删除注册事件
  event_del(self->sig_int);
  event_del(self->sig_term);

  // 退出loop
  event_base_loopexit(base, &delay);

}

void trader_mduser_shm_http_post_cb(struct evhttp_request *req, void *arg)
{
  trader_mduser_shm_http* self = (trader_mduser_shm_http*)arg;

  // 判断是否是POST请求
	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
    evhttp_send_reply(req, 405, "NOT GET", NULL);
    return;
	}

  // 应答buffer
  struct evbuffer *evresp = evbuffer_new();

  const char* buff = evhttp_request_get_uri(req);
  int len = strlen(buff);  

  // 处理请求报文体
  self->method->xProc(self, buff, len, (void*)evresp);

  evhttp_send_reply(req, HTTP_OK, "OK", evresp);

	evbuffer_free(evresp);
  
  return;
}

trader_mduser_shm_http* trader_mduser_shm_http_new()
{
  trader_mduser_shm_http* self = (trader_mduser_shm_http*)malloc(sizeof(trader_mduser_shm_http));
  
  self->method = trader_mduser_shm_http_method_get();

  return self;
}

void trader_mduser_shm_http_free(trader_mduser_shm_http* self)
{
  if(self){
    free(self);
  }
}

int main(int argc, char* argv[])
{
  int lnRet;
  if(argc < 5){
    return -1;
  }

  trader_mduser_shm_http* svr = trader_mduser_shm_http_new();

  // 初始化参数
  int i = 0;
  strncpy(svr->hostname, argv[++i], sizeof(svr->hostname));
  svr->port = (unsigned short)atoi(argv[++i]);
  strncpy(svr->server_name, argv[++i], sizeof(svr->server_name));
  strncpy(svr->shm_key_file, argv[++i], sizeof(svr->shm_key_file));

  svr->server_name_length = 1 + strnlen(svr->server_name, sizeof(svr->server_name));

  int ret = svr->method->xInit(svr);

  if(!ret){
    ret = svr->method->xRun(svr);
  }
  
  svr->method->xFini(svr);

  trader_mduser_shm_http_free(svr);

  return 0;
}


