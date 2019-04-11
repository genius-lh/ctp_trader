
#ifndef _CMN_LOG_APPENDER_H_
#define _CMN_LOG_APPENDER_H_

#include "cmn_log_util.h"

typedef struct cmn_log_appender_method_def cmn_log_appender_method;
typedef struct cmn_log_appender_def cmn_log_appender;

struct cmn_log_appender_def{
  char* app_name;
  int		app_isopen;
  void*	app_udata;
  cmn_log_appender_method* pMethod;
};

struct cmn_log_appender_method_def{
  void (*xAppend)(cmn_log_appender* self, cmn_log_evt_t* apEvt);
  int (*xOpen)(cmn_log_appender* self);
  void (*xClose)(cmn_log_appender* self);
};

extern cmn_log_appender* cmn_log_appender_new(const char* a_name);
extern void cmn_log_appender_free(cmn_log_appender* self);

#endif //_CMN_LOG_APPENDER_H_

