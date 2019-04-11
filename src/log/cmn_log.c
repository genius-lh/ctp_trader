
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/time.h>
#include<unistd.h>
#include <time.h>

#include "cmn_cfg.h"

#include "cmn_log_util.h"
#include "cmn_log.h"

void cmn_log_msg_err(char* data, int len)
{
  int i;
  char sScreenPrint[256];
  char* p = sScreenPrint;
  CMN_DEBUG("cmn_log_msg_err len=[%d][\n", len);
  for(i = 0; i < len; i++){
    sprintf(p, "0x%02X ", (unsigned char)data[i]);
    p += 5;
    if((i + 1) % 16 == 0){
      CMN_DEBUG("%s\n", sScreenPrint);
      p = sScreenPrint;
    }
  }
  CMN_DEBUG("]\n");
}

static cmn_log_clt_method* cmn_log_clt_method_get();

cmn_log_clt_method* cmn_log_clt_method_get()
{
  static cmn_log_clt_method stSynLogCltMethods = {
    cmn_log_clt_ini,
    cmn_log_clt_logger_msg,
    cmn_log_clt_fini
  };
  return &stSynLogCltMethods;
}


cmn_log_clt* cmn_log_clt_new()
{
  cmn_log_clt* self = malloc(sizeof(cmn_log_clt));
  self->pMethod = cmn_log_clt_method_get();
  return self;
}

void cmn_log_clt_free(cmn_log_clt* self)
{
  if(self){
    free(self);
  }
}

cmn_log_clt* cmn_log_clt_get()
{
  static cmn_log_clt_method gSynLogCtlMehtods = {
    cmn_log_clt_ini,
    cmn_log_clt_logger_msg,
    cmn_log_clt_fini
  };
  
  static cmn_log_clt stSynLogClt = {
    0,
    0,
    5,
    {0},
    0
  };
  stSynLogClt.pMethod = &gSynLogCtlMehtods;	

  return &stSynLogClt;
}

int cmn_log_clt_ini(cmn_log_clt* self, char* cfg_file, char* a_catalog)
{
  int nRet = 0;
  int iSqlCode = 0;
  
  self->pQueSvr = cmn_log_queue_new();
  CMN_ASSERT(self->pQueSvr);
  
  cmn_cfg* pCfg = cmn_cfg_new(cfg_file);
  CMN_ASSERT(pCfg);
  
  nRet = pCfg->pMethod->xInit(pCfg);
  CMN_ASSERT(!nRet);
  
  nRet = cmn_log_queue_get(self->pQueSvr, pCfg->pMethod->xGetLogPath(pCfg));
  CMN_ASSERT(!nRet);

  self->iSysPriority = pCfg->pMethod->xGetLogLevel(pCfg);

  cmn_cfg_free(pCfg);
  
  strncpy(self->catalog, a_catalog, sizeof(self->catalog));

  return nRet;
  
}

void cmn_log_clt_fini(cmn_log_clt* self)
{
  if(self->pQueSvr){
    cmn_log_queue_free(self->pQueSvr);
  }
}

void cmn_log_clt_logger_msg(cmn_log_clt* self, int a_priority, const char* a_msg)
{
  cmn_log_queue* pSynQueue = self->pQueSvr;
  int nRet = 0;
  int nLen = 0;
  cmn_log_evt_t oLogEvt;

  if(a_priority > self->iSysPriority){
    return ;
  }

  if(!pSynQueue){
    printf("%s>>%s", self->catalog, a_msg);
    return ;
  }
  
  strncpy(oLogEvt.catalog, self->catalog, sizeof(oLogEvt.catalog));
  gettimeofday(&oLogEvt.msg_time, NULL);
  oLogEvt.priority = a_priority;
  oLogEvt.msg_len = strlen(a_msg) + 1;
  memcpy(oLogEvt.msg_data, a_msg, oLogEvt.msg_len);
  nLen = sizeof(cmn_log_evt_t) + oLogEvt.msg_len - sizeof(oLogEvt.msg_data);
  
  nRet = cmn_log_queue_send(pSynQueue, (void*)&oLogEvt, nLen);

}


void cmn_log( cmn_log_clt* self, int aiLevel, const cmn_log_location_info* location, const char *aFormat, ...)
{
  char buf[2000];
  char sTime[14+1] = {0};
  va_list arglist;
  char buf1[2000];

  va_start(arglist,aFormat);
  vsprintf(buf1,aFormat, arglist);
  va_end(arglist);

  switch(aiLevel){
  case CMN_LOG_PRIORITY_FATAL:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][FATAL]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  case CMN_LOG_PRIORITY_ERROR:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][ERROR]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  case CMN_LOG_PRIORITY_WARN:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][WARN]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  case CMN_LOG_PRIORITY_INFO:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][INFO]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  case CMN_LOG_PRIORITY_DEBUG:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][DEBUG]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  case CMN_LOG_PRIORITY_UNKNOWN:
  default:
    snprintf(buf, sizeof(buf), "[%s][%d][%s][%d][UNKNOWN]>>%s",
      location->file, location->line, location->func, location->pid,
      buf1);
    break;
  }

  self->pMethod->xLoggerMsg(self, aiLevel, buf);
  
}

void cmn_log_hex( cmn_log_clt* self, int aiLevel, cmn_log_location_info* location, const char* a_data, int a_len)
{

}


