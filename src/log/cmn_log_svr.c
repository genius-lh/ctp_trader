
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <unistd.h>

#include "queue.h"
#include "cmn_cfg.h"
#include "cmn_util.h"
#include "cmn_log_util.h"
#include "cmn_log.h"
#include "cmn_log_catalog.h"
#include "cmn_log_svr.h"


static void cmn_log_svr_run(cmn_log_svr* self);
static int cmn_log_svr_proc(cmn_log_svr* self, cmn_log_evt_t* apEvt);
static int cmn_log_svr_catalog(cmn_log_svr* self, cmn_log_catalog** catalog, char* a_name);
static int cmn_log_svr_ini(cmn_log_svr* self);
static void cmn_log_svr_fini(cmn_log_svr* self);
static void cmn_log_svr_priority(cmn_log_svr* self, int a_priority);
static int cmn_log_svr_priority_enable(cmn_log_svr* self, int a_priority);
static void cmn_log_svr_path_set(cmn_log_svr* self, char* a_path);

static cmn_log_svr_method* cmn_log_svr_method_get();

static int cmn_log_svr_catalog_name(cmn_log_evt_t* apEvt, char* catalog);

cmn_log_svr_method* cmn_log_svr_method_get()
{
  static cmn_log_svr_method stSynLogSvrMethods = {
    cmn_log_svr_run,
    cmn_log_svr_proc,
    cmn_log_svr_catalog,
    cmn_log_svr_ini,
    cmn_log_svr_fini,
    cmn_log_svr_priority,
    cmn_log_svr_priority_enable,
    cmn_log_svr_path_set
  };
  return &stSynLogSvrMethods;
}

int cmn_log_svr_catalog_name(cmn_log_evt_t* apEvt, char* catalog)
{
  cmn_log_evt_t* pEvt = apEvt;

  
  struct tm now;
  localtime_r(&(pEvt->msg_time.tv_sec), &now);    

  sprintf(catalog, "%s.%04d%02d%02d", pEvt->catalog, 
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  
  return 0;
}


void cmn_log_svr_run(cmn_log_svr* self)
{
  int nRet = 0;
  static int nLoop = 1;
  cmn_log_evt_t oEvt;
  cmn_log_queue* pSynQueue = self->pQueueSvr;

  // 注册信号
  cmn_util_regist_signal(&nLoop);

  while(nLoop){
    // 获取oEvt
    nRet = cmn_log_queue_recv(pSynQueue, (void*)&oEvt, sizeof(oEvt));
    if(nRet < 0){
      sleep(1);
      continue;
    }

    // 处理
    self->pMethod->xProc(self, &oEvt);

  }

  return ;

}

int cmn_log_svr_proc(cmn_log_svr* self, cmn_log_evt_t* apEvt)
{
  int nRet = 0;
  cmn_log_catalog* pCatalog;
  
  char sCatalog[512];

  // 检查权限
  nRet = self->pMethod->xPriorityEnable(self, apEvt->priority);
  if(!nRet){
    return 0;
  }
  
  //  获取catalog name
  cmn_log_svr_catalog_name(apEvt, sCatalog);
  
  // 获取catalog
  nRet = self->pMethod->xCatalog(self, &pCatalog, sCatalog);
  if(!nRet){
    // 输出
    pCatalog->pMethod->xLogger(pCatalog, apEvt);
  }
  return nRet;
}

int cmn_log_svr_catalog(cmn_log_svr* self, cmn_log_catalog** catalog, char* a_name)
{
  cmn_log_catalog* pCatalog;
  int bFound = 0;
  // 查找catalog是否已经创建
  TAILQ_FOREACH(pCatalog, &self->hCatalogList, next){
    if(pCatalog->pMethod->xNameEqual(pCatalog, a_name)){
      bFound++;
      break;
    }
  }
  
  // 没有创建过catalog，则新建
  if(!bFound){
    pCatalog = cmn_log_catalog_new(self->pAppPath, a_name);
    TAILQ_INSERT_TAIL(&self->hCatalogList, pCatalog, next);
  }

  *catalog = pCatalog;

  return 0;
}

int cmn_log_svr_ini(cmn_log_svr* self)
{
  int nRet;
  cmn_log_queue* pSynQueue = self->pQueueSvr;
  
  nRet = cmn_log_queue_init(pSynQueue, self->pAppPath);
  if(nRet<0){
    printf("cmn_log_queue_init[%d]\n", nRet);
    return nRet;
  }

	TAILQ_INIT(&self->hCatalogList);

  return 0;

}

void cmn_log_svr_fini(cmn_log_svr* self)
{
  cmn_log_catalog* pCatalog;
	while((pCatalog = TAILQ_FIRST(&self->hCatalogList))!=NULL){
  	TAILQ_REMOVE(&self->hCatalogList, pCatalog, next);
  	cmn_log_catalog_free(pCatalog);
  }

  int nRet;
  cmn_log_queue* pSynQueue = self->pQueueSvr;

  nRet = cmn_log_queue_close(pSynQueue);
}

void cmn_log_svr_priority(cmn_log_svr* self, int a_priority)
{
  self->iAllPriority = a_priority;
}

int cmn_log_svr_priority_enable(cmn_log_svr* self, int a_priority)
{
  return (self->iAllPriority >= a_priority);
}

void cmn_log_svr_path_set(cmn_log_svr* self, char* a_path)
{
  if(!a_path){
    return;
  }

  int nPathLen;

  self->pAppPath = (char*)malloc(sizeof(char) * (strlen(a_path) + 1));
  strcpy(self->pAppPath, a_path);
  
  nPathLen = strlen(self->pAppPath);
  if(nPathLen){
    if(self->pAppPath[nPathLen-1] == '/'){
      self->pAppPath[nPathLen-1] = '\0';
    }
  }

  return;
}


cmn_log_svr* cmn_log_svr_new()
{
  cmn_log_svr* self;

  self = (cmn_log_svr*)malloc(sizeof(cmn_log_svr));

  self->pQueueSvr = cmn_log_queue_new();

  self->pMethod = cmn_log_svr_method_get();

  self->pAppPath = (char*)NULL;
  
  return self;

}

void cmn_log_svr_free(cmn_log_svr* self)
{
  if(self->pQueueSvr){
    cmn_log_queue_free(self->pQueueSvr);
  }

  if(self->pAppPath){
    free(self->pAppPath);
  }

  if(self){
    free(self);
  }
}

int log_main(char* cfg_file)
{
  int nRet = 0;
  int iRet = 0;
  int iSqlCode = 0;
  cmn_util_change_proc("traderLogSvr");
  cmn_log_svr* pLogSvr = cmn_log_svr_new();
  
  cmn_cfg* pCfg = cmn_cfg_new(cfg_file);
  pCfg->pMethod->xInit(pCfg);
  
  pLogSvr->pMethod->xPathSet(pLogSvr, pCfg->pMethod->xGetLogPath(pCfg));
  pLogSvr->pMethod->xPriority(pLogSvr, pCfg->pMethod->xGetLogLevel(pCfg));

  nRet = pLogSvr->pMethod->xIni(pLogSvr);


  if(!nRet){
    pLogSvr->pMethod->xRun(pLogSvr);
  }
  
  pLogSvr->pMethod->xFini(pLogSvr);
  
  cmn_cfg_free(pCfg);
  cmn_log_svr_free(pLogSvr);

  return 0;
}



