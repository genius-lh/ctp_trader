
#ifndef _CMN_LOG_SVR_H_
#define _CMN_LOG_SVR_H_

#include "queue.h"
#include "cmn_log_util.h"
#include "cmn_log_catalog.h"

typedef struct cmn_log_svr_method_def cmn_log_svr_method;
typedef struct cmn_log_svr_def cmn_log_svr;

struct cmn_log_svr_def{
  cmn_log_queue* pQueueSvr;
  int iAllPriority;
  char* pAppPath;
  
  TAILQ_HEAD(cmn_log_catalog_list, cmn_log_catalog_def) hCatalogList;

  cmn_log_svr_method* pMethod;
};

struct cmn_log_svr_method_def{
  void (*xRun)(cmn_log_svr* self);
  int (*xProc)(cmn_log_svr* self, cmn_log_evt_t* apEvt);
  int (*xCatalog)(cmn_log_svr* self, cmn_log_catalog** catalog, char* a_name);
  int (*xIni)(cmn_log_svr* self);
  void (*xFini)(cmn_log_svr* self);
  void (*xPriority)(cmn_log_svr* self, int a_priority);
  int (*xPriorityEnable)(cmn_log_svr* self, int a_priority);
  void (*xPathSet)(cmn_log_svr* self, char* a_path);
};

extern cmn_log_svr* cmn_log_svr_new();
extern void cmn_log_svr_free(cmn_log_svr* self);

#endif //_CMN_LOG_SVR_H_

