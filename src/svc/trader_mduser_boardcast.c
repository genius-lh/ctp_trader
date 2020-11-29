
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>  


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
static int trader_mduser_boardcast_init_unix(trader_mduser_boardcast* self, struct event_base* base, char* unixsocket);

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
    trader_mduser_boardcast_exit,
    trader_mduser_boardcast_init_unix
  };

  return &trader_mduser_boardcast_method_st;
}

int trader_mduser_boardcast_init(trader_mduser_boardcast* self, struct event_base* base, char* ip, int port)
{
  self->base = base;
  
  TAILQ_INIT(&self->cnnList);
  
  struct sockaddr_in sin;
  //listen
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(ip);
  sin.sin_port = htons(port);

  self->listener = evconnlistener_new_bind(self->base, trader_mduser_boardcast_listener_cb, (void *)self,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
    (struct sockaddr*)&sin,
    sizeof(sin));
  
  return 0;
}

int trader_mduser_boardcast_init_unix(trader_mduser_boardcast* self, struct event_base* base, char* unixsocket)
{
  self->base = base;

  TAILQ_INIT(&self->cnnList);

  evutil_socket_t fd;
  struct sockaddr_un sun;

	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd < 0) {
		return -1;
  }

  if(evutil_make_socket_nonblocking(fd) < 0){
		evutil_closesocket(fd);
    return -2;
  }

  if(evutil_make_listen_socket_reuseable(fd) < 0){
		evutil_closesocket(fd);
    return -3;
  }

  memset(&sun, 0, sizeof(sun));
  sun.sun_family = AF_LOCAL;
  strncpy(sun.sun_path, unixsocket, sizeof(sun.sun_path)-1);

  if (bind(fd, (struct sockaddr*)&sun, sizeof(sun)) < 0) {
		evutil_closesocket(fd);
    return -4;
  }
  
  if (listen(fd, -1) < 0) {
		evutil_closesocket(fd);
    return -5;
  }

  self->listener = evconnlistener_new(self->base, trader_mduser_boardcast_listener_cb, (void *)self,
    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
    fd);
  
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
  if(self->listener){
    evconnlistener_free(self->listener);
    self->listener = NULL;
  }
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


