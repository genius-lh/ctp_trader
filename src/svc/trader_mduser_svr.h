
#ifndef _TRADER_MDUSER_SVR_H_
#define _TRADER_MDUSER_SVR_H_

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "trader_data.h"

#include "trader_mduser_cnn.h"
#include "trader_mduser_boardcast.h"
#include "hiredis.h"

typedef struct trader_mduser_svr_def trader_mduser_svr;
typedef struct trader_mduser_svr_method_def trader_mduser_svr_method;
struct trader_mduser_svr_def {
  struct event_base *pBase;
  
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  redisContext* pRedisCtx;

  trader_mduser_cnn* pCnnMain;
  trader_mduser_cnn* pCnnBackup;

  trader_mduser_boardcast* pBoardcast;
  
  trader_instrument_id_type* instruments;
  int instrumentNumber;
  
  trader_tick* ticks;

  char mainBrokerId[8];
  char mainUser[16];
  char mainPasswd[16];
  char mainAddr[64];
  char mainWorkspace[64];

  char backupBrokerId[8];
  char backupUser[16];
  char backupPasswd[16];
  char backupAddr[64];
  char backupWorkspace[64];

  char redisInstrumentKey[32];
  
  trader_mduser_svr_method* pMethod;

};

struct trader_mduser_svr_method_def {
  int (*xInit)(trader_mduser_svr* self, int argc, char* argv[]);
  int (*xRun)(trader_mduser_svr* self);
  int (*xProc)(trader_mduser_svr* self, trader_mduser_evt* evt);
};


extern trader_mduser_svr* trader_mduser_svr_new();
extern void trader_mduser_svr_free(trader_mduser_svr* self);

#endif // _TRADER_MDUSER_SVR_H_

