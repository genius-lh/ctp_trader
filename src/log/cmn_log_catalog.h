
#ifndef _CMN_LOG_CATALOG_H_
#define _CMN_LOG_CATALOG_H_

#include "queue.h"
#include "cmn_log_util.h"
#include "cmn_log_appender.h"

typedef struct cmn_log_catalog_method_def cmn_log_catalog_method;
typedef struct cmn_log_catalog_def cmn_log_catalog;

struct cmn_log_catalog_def{
  TAILQ_ENTRY(cmn_log_catalog_def) next;
  char* sCatalogName;
  cmn_log_appender* pLogApp;
  cmn_log_catalog_method* pMethod;
};

struct cmn_log_catalog_method_def{
  void (*xLogger)(cmn_log_catalog* self, cmn_log_evt_t* apEvt);
  int (*xNameEqual)(cmn_log_catalog* self, char* a_name);
  
};

extern cmn_log_catalog* cmn_log_catalog_new(const char* a_path, const char* a_name);
extern void cmn_log_catalog_free(cmn_log_catalog* self);

#endif //_CMN_LOG_CATALOG_H_

