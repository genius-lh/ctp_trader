#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <pthread.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

typedef struct comm_tool_def comm_tool;
struct comm_tool_def {
  char* _ip;
  unsigned short _port;
  struct bufferevent* _pipe;
  struct event_base* _base;
  struct bufferevent* _event;
  int _status;
  pthread_t _tid;
};

comm_tool* comm_tool_new(evutil_socket_t fd, char* ip, unsigned short port);

int comm_tool_connect(comm_tool* self);


void comm_tool_free(comm_tool* self);
int comm_tool_main(comm_tool* self);

static void comm_tool_pipe_read_cb(struct bufferevent *bev, void *arg);
static void comm_tool_pipe_write_cb(struct bufferevent *bev, void *arg);
static void comm_tool_pipe_evt_cb(struct bufferevent *bev, short event, void *arg);
static void comm_tool_client_read_cb(struct bufferevent *bev, void *arg);
static void comm_tool_client_write_cb(struct bufferevent *bev, void *arg);
static void comm_tool_client_evt_cb(struct bufferevent *bev, short event, void *arg);


static void* comm_tool_thread(void* arg);

int main(int argc, char* argv [])
{
  char* pIp = argv[1];
  unsigned short sPort = (unsigned short)atol(argv[2]);
  int nRet = 0;
  int nLoop = 1;
  
  evutil_socket_t pair[2];
  if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1){
    exit(-1);
  }

  comm_tool* pTool = comm_tool_new(pair[1], pIp, sPort);


  nRet = pthread_create(&pTool->_tid, NULL, comm_tool_thread, (void*)pTool);

  sleep(2);

  int cmd;
  char buff[10000];
  nLoop = 1;
  while(nLoop){
    printf("0 -- ÍË³ö\n");
    printf("1 -- ·¢ËÍ\n");
    scanf("%d", &cmd);
    switch(cmd){
      case 0:
        nLoop = 0;
        pthread_kill(pTool->_tid, SIGQUIT);
        break;
      case 1:
        printf("input:\n");
        scanf("%s", buff);
        write(pair[0], buff, strlen(buff));
        break;
      default:
        
        break;

    }
    



  }
  void* pRc = (void*)0;
  
  pthread_join(pTool->_tid, &pRc);

  evutil_closesocket(pair[0]);
  evutil_closesocket(pair[1]);

  comm_tool_free(pTool);
  return 0;
}

void* comm_tool_thread(void* arg)
{
  comm_tool* self = (comm_tool*)arg;
  int nRet = comm_tool_main(self);


  return 0;

}


comm_tool* comm_tool_new(evutil_socket_t fd, char* ip, unsigned short port)
{
  comm_tool* self = (comm_tool*)malloc(sizeof(comm_tool));
  self->_base = event_base_new();
  self->_pipe = bufferevent_socket_new(self->_base, fd, 0);
  bufferevent_setcb(self->_pipe, comm_tool_pipe_read_cb, comm_tool_pipe_write_cb,
    comm_tool_pipe_evt_cb, (void*)self);
  bufferevent_enable(self->_pipe, EV_READ|EV_PERSIST);

  
  self->_event = bufferevent_socket_new(self->_base, -1, 0);
  bufferevent_setcb(self->_event, comm_tool_client_read_cb, comm_tool_client_write_cb,
    comm_tool_client_evt_cb, (void*)self);
  
  
  self->_ip = (char*)malloc(sizeof(char)*(strlen(ip)+1));
  strcpy(self->_ip, ip);
  self->_port = port;
  self->_status = 0;

  

  return self;
  


}


void comm_tool_free(comm_tool* self)
{
  if(self){
    if(self->_ip){
      free(self->_ip);
    }

    if(self->_event){
      bufferevent_free(self->_event);
    }
    
    if(self->_pipe){
      bufferevent_free(self->_pipe);
    }

    
    if(self->_base){
      event_base_free(self->_base);
    }    
  }

  return ;
}


int comm_tool_main(comm_tool* self)
{
  int ret;
  comm_tool_connect(self);

  event_base_dispatch(self->_base);

  return 0;
}

int comm_tool_connect(comm_tool* self)
{
  struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(self->_port);
  sin.sin_addr.s_addr = inet_addr(self->_ip);
  
  int ret = bufferevent_socket_connect(self->_event, (struct sockaddr *)&sin, sizeof(sin));
	bufferevent_enable(self->_event, EV_READ);

  return 0;
}



void comm_tool_pipe_read_cb(struct bufferevent *bev, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
  struct evbuffer* buf = evbuffer_new();

  bufferevent_read_buffer(bev, buf);
  
  if(self->_status){
    bufferevent_write_buffer(self->_event, buf);
    bufferevent_enable(self->_event, EV_WRITE);
  }

  evbuffer_free(buf);
}

void comm_tool_pipe_write_cb(struct bufferevent *bev, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
  return ;
}

void comm_tool_pipe_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
  return ;
}

void comm_tool_client_read_cb(struct bufferevent *bev, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
  int len;
  char* buff_prt;
  struct evbuffer* buf = bufferevent_get_input(bev);

  len = evbuffer_get_length(buf);

  buff_prt = (char*)malloc(sizeof(char) * (len + 1));

  bufferevent_read(bev, buff_prt, len);
  buff_prt[len] = '\0';

  printf("response[\n");
  printf("%s", buff_prt);
  printf("]\n");


  free(buff_prt);

}

void comm_tool_client_write_cb(struct bufferevent *bev, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
  return ;
}

void comm_tool_client_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  comm_tool* self = (comm_tool*)arg;
	if (event & BEV_EVENT_ERROR) {
    evutil_closesocket(bufferevent_getfd(bev));
    comm_tool_connect(self);
    return;
	}
	if (event & BEV_EVENT_CONNECTED) {
    self->_status = 1;
    bufferevent_enable(self->_event, EV_READ|EV_PERSIST);
	}
  
	if (event & BEV_EVENT_EOF) {
    evutil_closesocket(bufferevent_getfd(bev));
    comm_tool_connect(self);
	}  
}


