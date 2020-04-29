#ifndef _TRADER_MD_DATA_SVR_H_
#define _TRADER_MD_DATA_SVR_H_

#include <event2/event.h>
#include <event2/http.h>

typedef struct trader_md_data_svr_def trader_md_data_svr;
typedef struct trader_md_data_svr_method_def trader_md_data_svr_method;

struct trader_md_data_svr_def{
  struct event_base *pBase;
  
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;
  struct event* pEvTimer;
  
  struct evhttp* pHttp;

  void* pShmHdr;

  char* pDataCache;

  // ²ÎÊý
  int nMdSize;
  int nMdHeadPos;
  int nMdTailPos;

  int nContractSize;
  int nHttpPort;
  int nInterval;

  char* pShmKeyFile;
  char* pServiceName;

  trader_md_data_svr_method* pMethod;
};


struct trader_md_data_svr_method_def{
  int (*xInitParam)(trader_md_data_svr* self, int argc, char* argv[]);
  int (*xRun)(trader_md_data_svr* self);
  int (*xOnTickTimer)(trader_md_data_svr* self);
  int (*xReqTickData)(trader_md_data_svr* self, const char* t1, const char* t2, double w1, double w2, double tp, double* bid, double* ask);
};


extern trader_md_data_svr* trader_md_data_svr_new();
extern void trader_md_data_svr_free(trader_md_data_svr* self);


#endif //_TRADER_MD_DATA_SVR_H_

