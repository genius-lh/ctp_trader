

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "cmn_log.h"
#include "cmn_log_util.h"
#include "cmn_log_appender.h"
#include "cmn_log_catalog.h"

static void cmn_log_catalog_logger(cmn_log_catalog* self, cmn_log_evt_t* apEvt);
static int cmn_log_catalog_name_equal(cmn_log_catalog* self, char* a_name);
static cmn_log_catalog_method* cmn_log_catalog_method_get();


void cmn_log_catalog_logger(cmn_log_catalog* self, cmn_log_evt_t* apEvt)
{
  cmn_log_appender* pApp = self->pLogApp;
  pApp->pMethod->xAppend(pApp, apEvt);
  return ;

}

int cmn_log_catalog_name_equal(cmn_log_catalog* self, char* a_name)
{
  return (!strcmp(self->sCatalogName, a_name));
}

cmn_log_catalog_method* cmn_log_catalog_method_get()
{
  static cmn_log_catalog_method stSynLogCatalogMethods = {
    cmn_log_catalog_logger,
    cmn_log_catalog_name_equal
  };

  return &stSynLogCatalogMethods;

}

cmn_log_catalog* cmn_log_catalog_new(const char* a_path, const char* a_name)
{
  int nLen = 0;
  char* pAppName;
  char* pAppPath;
  int nPathLen = 0;
  cmn_log_catalog* self = (cmn_log_catalog*)malloc(sizeof(cmn_log_catalog));
  pAppPath = a_path;
  CMN_ASSERT(pAppPath);

  nPathLen = strlen(pAppPath);
  if(nPathLen){
    if(pAppPath[nPathLen-1] == '/'){
      pAppPath[nPathLen-1] = '\0';
    }
  }

  // 初始化模块名
  // 文件路径 + / + 模块名 + .log + \0 
  nLen = strlen(a_name) + strlen(pAppPath) + 5 + 1;
  
  self->sCatalogName = (char*)malloc(nLen*sizeof(char));
  strcpy(self->sCatalogName, a_name);

  // 初始化cmn_log_appender
  pAppName = (char*)malloc(nLen*sizeof(char));
  sprintf(pAppName, "%s/%s.log", pAppPath,a_name);
  
  self->pLogApp = cmn_log_appender_new(pAppName);

  free(pAppName);

  // 方法
  self->pMethod = cmn_log_catalog_method_get();

  return self;

}

void cmn_log_catalog_free(cmn_log_catalog* self)
{
  if(self->sCatalogName){
    free(self->sCatalogName);
  }

  if(self->pLogApp){
    cmn_log_appender_free(self->pLogApp);
  }
  
  if(self){
    free(self);
  }


}

