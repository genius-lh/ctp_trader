
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <limits.h>
#include <float.h>

#include <pthread.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "ctp_mduser_api.h"
#include "trader_data.h"
#include "trader_msg_struct.h"

#include "trader_db.h"

#include "cmn_log.h"


static void comm_tool_pipe_read_cb(struct bufferevent *bev, void *arg);
static void comm_tool_pipe_write_cb(struct bufferevent *bev, void *arg);
static void comm_tool_pipe_evt_cb(struct bufferevent *bev, short event, void *arg);

static void trader_svr_signal_cb(evutil_socket_t fd, short event, void *arg);

int main(int argc, char* argv [])
{

  CMN_LOG_INI("trader.cfg", "aaa");

  evutil_socket_t pair[2];
  if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1){
    exit(-1);
  }
  
  ctp_mduser_api* pApi = ctp_mduser_api_new(pair[0]);

  trader_db* db = trader_db_new("haha");

  db->pMethod->xInit(db, "20150909");

  
  struct event_base* base = event_base_new();
  
  struct bufferevent* pipe = bufferevent_socket_new(base, pair[1], 0);
  bufferevent_setcb(pipe, comm_tool_pipe_read_cb, comm_tool_pipe_write_cb,
    comm_tool_pipe_evt_cb, (void*)db);
  bufferevent_enable(pipe, EV_READ|EV_PERSIST);

  
  struct event* pSigIntEvt = evsignal_new(base, SIGINT, trader_svr_signal_cb, (void *)base);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  struct event* pSigTermEvt = evsignal_new(base, SIGTERM, trader_svr_signal_cb, (void *)base);

  char brokerId[] = "8060";
  char frontAddress[] = "tcp://180.166.65.119:41213";

  pApi->BrokenId = brokerId;
  pApi->FrontAddress = frontAddress;

  pApi->pMethod->xStart(pApi, "99015469", "090537");

  sleep(5);

  pApi->pMethod->xSubscribe(pApi, "rb1601");
  
  pApi->pMethod->xSubscribe(pApi, "m1601");
  
  pApi->pMethod->xSubscribe(pApi, "jd1601");

  pApi->pMethod->xSubscribe(pApi, "sr1601");

  
  event_add(pSigIntEvt, NULL);
  event_add(pSigTermEvt, NULL);

  int nRet = event_base_dispatch(base);

  
  pApi->pMethod->xStop(pApi);

  db->pMethod->xFini(db);

  trader_db_free(db);

  ctp_mduser_api_free(pApi);


  event_free(pSigIntEvt);
  
  event_free(pSigTermEvt);

  bufferevent_free(pipe);

  event_base_free(base);

  CMN_LOG_FINI();


  exit(0);


}

void comm_tool_pipe_read_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_msg_mduser_struct oMsg;

  trader_db* db = (trader_db*)arg;
  do {
    nRet = cmn_util_bufferevent_recv(bev, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    if(oMsg.type != TRADER_RECV_MDUSER_TYPE_INFO){
      continue;
    }
    
    trader_tick* pMsg = &oMsg.oTick;
    
    if((DBL_MAX == pMsg->BidPrice1)
    ||(DBL_MAX == pMsg->AskPrice1)){
      CMN_DEBUG("无效的行情数据!\n");
      return 0;
    }
    CMN_DEBUG("Instrument[%s]\n", pMsg->InstrumentID);
    
    int errCd = 0;
    int nRet = 0;

    struct trader_db_mduser_inf_def dbMduser;
    strcpy(dbMduser.InstrumentID, pMsg->InstrumentID);
    strcpy(dbMduser.TradingDay, pMsg->TradingDay);
    strcpy(dbMduser.UpdateTime, pMsg->UpdateTime);
    dbMduser.UpdateMillisec = pMsg->UpdateMillisec;
    dbMduser.BidPrice1 = pMsg->BidPrice1;
    dbMduser.BidVolume1 = pMsg->BidVolume1;
    dbMduser.AskPrice1 = pMsg->AskPrice1;
    dbMduser.AskVolume1 = pMsg->AskVolume1;
    
    nRet = db->pMethod->xMduserInsert(db, &dbMduser, &errCd);
    
    
  }while(loop);
}

void comm_tool_pipe_write_cb(struct bufferevent *bev, void *arg)
{

}

void comm_tool_pipe_evt_cb(struct bufferevent *bev, short event, void *arg)
{

}

void trader_svr_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  struct event_base* base = (struct event_base*)arg;
  struct timeval delay = { 1, 0 };


  event_base_loopexit(base, &delay);

}



