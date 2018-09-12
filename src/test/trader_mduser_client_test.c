
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/time.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "trader_data.h"
#include "trader_mduser_api.h"

#include "trader_mduser_client.h"
#include "trader_db.h"

typedef struct trader_mduser_client_test_def trader_mduser_client_test;
struct trader_mduser_client_test_def {

  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;
  
  trader_mduser_client* pApi;
  trader_db* pTraderDb;

};

static void trader_mduser_client_test_connect_callback(void* user_data);
static void trader_mduser_client_test_disconnect_callback(void* user_data);
static void trader_mduser_client_test_recv_callback(void* user_data, void* data, int len);


static void client_test_signal_cb(evutil_socket_t fd, short event, void *arg);

void client_test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_client_test* test = (trader_mduser_client_test*)arg;
  struct timeval tv = {
    2, 0
  };
  event_base_loopexit(test->pBase, &tv);
}

void trader_mduser_client_test_connect_callback(void* user_data)
{
  printf("connected!\n");
}

void trader_mduser_client_test_disconnect_callback(void* user_data)
{
  printf("disconnected!\n");
  sleep(1);
  trader_mduser_client_test* test = (trader_mduser_client_test*)user_data;
  test->pApi->method->xConnect(test->pApi);
}

void trader_mduser_client_test_recv_callback(void* user_data, void* data, int len)
{
  trader_mduser_client_test* test = (trader_mduser_client_test*)user_data;
  trader_mduser_evt* pEvt = (trader_mduser_evt*)data;

  trader_tick* tick_data = &pEvt->Tick;
  int nRet = 0;
  int errCd = 0;
  
  struct trader_db_mduser_inf_def dbMduser;
  
  strcpy(dbMduser.InstrumentID, tick_data->InstrumentID);
  strcpy(dbMduser.TradingDay, tick_data->TradingDay);
  strcpy(dbMduser.UpdateTime, tick_data->UpdateTime);
  dbMduser.UpdateMillisec = tick_data->UpdateMillisec;
  dbMduser.BidPrice1 = tick_data->BidPrice1;
  dbMduser.BidVolume1 = tick_data->BidVolume1;
  dbMduser.AskPrice1 = tick_data->AskPrice1;
  dbMduser.AskVolume1 = tick_data->AskVolume1;
  nRet = test->pTraderDb->pMethod->xMduserInsert(test->pTraderDb, &dbMduser, &errCd);
  
}


int main(int argc, char* argv[])
{
  trader_mduser_client_test trader_mduser_client_test_st;
  trader_mduser_client_test* test = &trader_mduser_client_test_st;

  test->pBase = event_base_new();

  test->pSigIntEvt = evsignal_new(test->pBase, SIGINT, client_test_signal_cb, (void *)test);
  test->pSigTermEvt = evsignal_new(test->pBase, SIGTERM, client_test_signal_cb, (void *)test);
  
  test->pApi = trader_mduser_client_new();

  test->pApi->method->xInit(test->pApi, test->pBase, "0.0.0.0", 22347,
    trader_mduser_client_test_connect_callback,
    trader_mduser_client_test_disconnect_callback,
    trader_mduser_client_test_recv_callback,
    test
    );

  test->pTraderDb = trader_db_new(TRADER_DB_NAME);
  
  test->pTraderDb->pMethod->xInit(test->pTraderDb, argv[1]);

  test->pApi->method->xConnect(test->pApi);

  event_base_dispatch(test->pBase);

  test->pApi->method->xExit(test->pApi);

  test->pTraderDb->pMethod->xFini(test->pTraderDb);

  trader_db_free(test->pTraderDb);

  event_free(test->pSigTermEvt);
  event_free(test->pSigIntEvt);

  trader_mduser_client_free(test->pApi);

  return 0;
}


