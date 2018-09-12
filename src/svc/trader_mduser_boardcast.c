
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "queue.h"

#include "trader_mduser_boardcast.h"

static trader_mduser_boardcast_method* trader_mduser_boardcast_method_get();

static int trader_mduser_boardcast_init(trader_mduser_boardcast* self, struct event_base* base, char* ip, int port);
static int trader_mduser_boardcast_accept(trader_mduser_boardcast* self, evutil_socket_t fd);
static int trader_mduser_boardcast_on_cnn_event(trader_mduser_boardcast* self, trader_mduser_boardcast_cnn* cnn);
static int trader_mduser_boardcast_boardcase(trader_mduser_boardcast* self, char* buff, int len);
static int trader_mduser_boardcast_exit(trader_mduser_boardcast* self);

static void trader_mduser_boardcast_read_cb(struct bufferevent *bev, void *arg);
static void trader_mduser_boardcast_write_cb(struct bufferevent *bev, void *arg);
static void trader_mduser_boardcast_evt_cb(struct bufferevent *bev, short event, void *arg);
static void trader_mduser_boardcast_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data);


trader_mduser_boardcast_method* trader_mduser_boardcast_method_get()
{
  static trader_mduser_boardcast_method trader_mduser_boardcast_method_st = {
    trader_mduser_boardcast_init,
    trader_mduser_boardcast_accept,
    trader_mduser_boardcast_on_cnn_event,
    trader_mduser_boardcast_boardcase,
    trader_mduser_boardcast_exit
  };

  return &trader_mduser_boardcast_method_st;
}

int trader_mduser_boardcast_init(trader_mduser_boardcast* self, struct event_base* base, char* ip, int port)
{
  self->base = base;
  strcpy(self->ip, ip);
  self->port = port;
  
  TAILQ_INIT(&self->cnnList);
  
  struct sockaddr_in sin;
  //listen
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(self->ip);
  sin.sin_port = htons(self->port);

  self->listener = evconnlistener_new_bind(self->base, trader_mduser_boardcast_listener_cb, (void *)self,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
    (struct sockaddr*)&sin,
    sizeof(sin));
  
  return 0;
}

int trader_mduser_boardcast_accept(trader_mduser_boardcast* self, evutil_socket_t fd)
{
  trader_mduser_boardcast_cnn* cnn = (trader_mduser_boardcast_cnn*)malloc(sizeof(trader_mduser_boardcast_cnn));

  cnn->parent = self;
  cnn->be = bufferevent_socket_new(self->base, fd, BEV_OPT_CLOSE_ON_FREE);
  
  bufferevent_setcb(cnn->be, trader_mduser_boardcast_read_cb, 
    trader_mduser_boardcast_write_cb, trader_mduser_boardcast_evt_cb, (void*)cnn);
  bufferevent_enable(cnn->be, EV_READ|EV_WRITE);

  TAILQ_INSERT_TAIL(&self->cnnList, cnn, next);

  return 0;
}

int trader_mduser_boardcast_on_cnn_event(trader_mduser_boardcast* self, trader_mduser_boardcast_cnn* cnn)
{
  TAILQ_REMOVE(&self->cnnList, cnn, next);
  bufferevent_free(cnn->be);
  free(cnn);
  return 0;
}

int trader_mduser_boardcast_boardcase(trader_mduser_boardcast* self, char* buff, int len)
{
  trader_mduser_boardcast_cnn* cnn;

  TAILQ_FOREACH(cnn, &self->cnnList, next) {
    bufferevent_write(cnn->be, buff, len);
  }
  return 0;
}

int trader_mduser_boardcast_exit(trader_mduser_boardcast* self)
{
  trader_mduser_boardcast_cnn* cnn;
  while((cnn = TAILQ_FIRST(&self->cnnList))!=NULL){
  	TAILQ_REMOVE(&self->cnnList, cnn, next);
    if(cnn->be){
      bufferevent_free(cnn->be);
    }
    free(cnn);
  }
  evconnlistener_free(self->listener);

  return 0;
}

void trader_mduser_boardcast_read_cb(struct bufferevent *bev, void *arg)
{
  struct evbuffer* pEvBuf = bufferevent_get_input(bev);

  int nLen = evbuffer_get_length(pEvBuf);
  char* pData = (char*)malloc(nLen*sizeof(char));

  bufferevent_read(bev, pData, nLen);
  
  free(pData);

  return ;
}

void trader_mduser_boardcast_write_cb(struct bufferevent *bev, void *arg)
{
  return ;
}

void trader_mduser_boardcast_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_mduser_boardcast_cnn* cnn = (trader_mduser_boardcast_cnn*)arg;

  trader_mduser_boardcast* self = cnn->parent;

  self->method->xOnCnnEvent(self, cnn);

  return ;
}

void trader_mduser_boardcast_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
  trader_mduser_boardcast* self = (trader_mduser_boardcast*)user_data;

  self->method->xAccept(self, fd);

  return ;
}

trader_mduser_boardcast* trader_mduser_boardcast_new()
{
  trader_mduser_boardcast* self = (trader_mduser_boardcast*)malloc(sizeof(trader_mduser_boardcast));

  self->method = trader_mduser_boardcast_method_get();
  
  return self;
}

void trader_mduser_boardcast_free(trader_mduser_boardcast* self)
{

  if(self){
    free(self);
  }
}


