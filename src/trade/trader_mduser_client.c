
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "trader_mduser_client.h"

static trader_mduser_client_method* trader_mduser_client_method_get();
static int trader_mduser_client_init(trader_mduser_client* self, struct event_base* base, char* ip, int port,
                                        trader_mduser_client_connect_callback connect_cb,
                                        trader_mduser_client_disconnect_callback disconnect_cb,
                                        trader_mduser_client_recv_callback recv_data_cb,
                                        void* user_data);
static int trader_mduser_client_connect(trader_mduser_client* self);
static int trader_mduser_client_on_event(trader_mduser_client* self);
static int trader_mduser_client_on_recv(trader_mduser_client* self);
static int trader_mduser_client_exit(trader_mduser_client* self);

static void trader_mduser_client_read_cb(struct bufferevent *bev, void *arg);
static void trader_mduser_client_evt_cb(struct bufferevent *bev, short event, void *arg);

trader_mduser_client_method* trader_mduser_client_method_get()
{
  static trader_mduser_client_method trader_mduser_client_method_st = {
    trader_mduser_client_init,
    trader_mduser_client_connect,
    trader_mduser_client_on_event,
    trader_mduser_client_on_recv,
    trader_mduser_client_exit
  };

  return &trader_mduser_client_method_st;
}

int trader_mduser_client_init(trader_mduser_client* self, struct event_base* base, char* ip, int port,
                                  trader_mduser_client_connect_callback connect_cb,
                                  trader_mduser_client_disconnect_callback disconnect_cb,
                                  trader_mduser_client_recv_callback recv_data_cb,
                                  void* user_data)
{
  self->base = base;
  strcpy(self->ip, ip);
  self->port = port;
  self->connect_cb = connect_cb;
  self->disconnect_cb = disconnect_cb;
  self->recv_data_cb = recv_data_cb;
  self->user_data = user_data;
  self->bev = (struct bufferevent*)NULL;
  
}

int trader_mduser_client_connect(trader_mduser_client* self)
{

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(self->ip);
  sin.sin_port = htons(self->port);
  
  self->bev = bufferevent_socket_new(self->base, -1,
		BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);

  bufferevent_setcb(self->bev, trader_mduser_client_read_cb, NULL, trader_mduser_client_evt_cb, (void*)self);
  
  bufferevent_enable(self->bev, EV_READ);

  bufferevent_socket_connect(self->bev, (struct sockaddr*)&sin, sizeof(sin));

  return 0;

}

int trader_mduser_client_on_event(trader_mduser_client* self)
{
  if(self->bev){
    bufferevent_free(self->bev);
    self->bev = (struct bufferevent*)NULL;
  }
  return 0;
}

int trader_mduser_client_on_recv(trader_mduser_client* self)
{
  struct bufferevent *bev = self->bev;

  struct evbuffer* pEvBuf = bufferevent_get_input(bev);

  int nLen = evbuffer_get_length(pEvBuf);
  char* pData = (char*)malloc(nLen*sizeof(char));

  bufferevent_read(bev, pData, nLen);

  if(self->recv_data_cb){
    (self->recv_data_cb)(self->user_data, pData, nLen);
  }
  
  free(pData);

  return 0;
}

int trader_mduser_client_exit(trader_mduser_client* self)
{
  self->method->xOnEvent(self);
  return 0;
}

void trader_mduser_client_read_cb(struct bufferevent *bev, void *arg)
{
  trader_mduser_client* self = (trader_mduser_client*)arg;

  self->method->xOnRecv(self);
}

void trader_mduser_client_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_mduser_client* self = (trader_mduser_client*)arg;
  
	if (event & BEV_EVENT_ERROR) {
    self->method->xOnEvent(self);
    if(self->disconnect_cb){
      (self->disconnect_cb)(self->user_data);
    }
		return;
	}
  
	if (event & BEV_EVENT_CONNECTED) {
		bufferevent_enable(bev, EV_READ | EV_PERSIST);
    if(self->connect_cb){
      (self->connect_cb)(self->user_data);
    }
	}
  
	if (event & BEV_EVENT_EOF) {
    self->method->xOnEvent(self);
    if(self->disconnect_cb){
      (self->disconnect_cb)(self->user_data);
    }
	}
}


trader_mduser_client* trader_mduser_client_new()
{
  trader_mduser_client* self = (trader_mduser_client*)malloc(sizeof(trader_mduser_client));

  self->method = trader_mduser_client_method_get();

  return self;
}

void trader_mduser_client_free(trader_mduser_client* self)
{
  if(self){
    free(self);
  }
}

