

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <unistd.h> 
#include <fcntl.h>

#include "cmn_log_util.h"
#include "cmn_log_appender.h"

static void cmn_log_appender_append(cmn_log_appender* self, cmn_log_evt_t* apEvt);
static int cmn_log_appender_open(cmn_log_appender* self);
static void cmn_log_appender_close(cmn_log_appender* self);
static cmn_log_appender_method* cmn_log_appender_method_get();

cmn_log_appender_method* cmn_log_appender_method_get()
{
  static cmn_log_appender_method stSynLogAppMethods = {
    cmn_log_appender_append,
    cmn_log_appender_open,
    cmn_log_appender_close
  };
  return &stSynLogAppMethods;

}

void cmn_log_appender_append(cmn_log_appender* self, cmn_log_evt_t* apEvt)
{
  int nRet = 0;
  struct tm now;
  
  // 判断文件是否存在
  nRet = access(self->app_name, F_OK);
  if(nRet){
    self->pMethod->xClose(self);
  }

  // 判断文件是否打开
  if(!self->app_isopen){
    nRet = self->pMethod->xOpen(self);
    if(nRet < 0){
      return;
    }
  }

  FILE* fp = (FILE*)self->app_udata;
  
  localtime_r(&(apEvt->msg_time.tv_sec), &now);    

  nRet = fprintf(fp, "[%02d%02d%02d.%06d]%s",
    now.tm_hour, now.tm_min, now.tm_sec, apEvt->msg_time.tv_usec,
    apEvt->msg_data);
  fflush(fp);
  if(nRet < 0){
    self->pMethod->xClose(self);
  }
  return;
}

int cmn_log_appender_open(cmn_log_appender* self)
{
  FILE* fp;

  int nRet = 0;
  
  // 判断文件是否打开，如果打开，直接返回成功
  if(self->app_isopen){
    return 0;
  }

  // 创建文件
  fp = fopen(self->app_name, "a+");
  if(!fp){
    return -1;
  }
  
  self->app_udata = (void*)fp;
  self->app_isopen++;

  return 0;
}

void cmn_log_appender_close(cmn_log_appender* self)
{
  FILE* fp;

  // 判断文件是否打开，如果没有打开，直接返回成功
  if (!self->app_isopen){
    return ;
  }

  fp = (FILE*)self->app_udata;

  fclose(fp);
  self->app_isopen--;

  return;
  
}

cmn_log_appender* cmn_log_appender_new(const char* a_name)
{
  cmn_log_appender* self;
  
  if (!a_name)
    return NULL;
  
  self = malloc(sizeof(cmn_log_appender));

  self->app_name = malloc(sizeof(char) * (strlen(a_name) + 1));
  strcpy(self->app_name, a_name);
  
  self->app_isopen = 0;
  self->app_udata = 0;
  
  self->pMethod = cmn_log_appender_method_get();
  return self;
}

void cmn_log_appender_free(cmn_log_appender* self)
{

  self->pMethod->xClose(self);

  if(self->app_name){
    free(self->app_name);
  }

  if(self){
    free(self);
  }

}


