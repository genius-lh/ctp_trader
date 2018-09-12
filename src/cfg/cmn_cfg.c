
/* 读取配置文件 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmn_cfg.h"
#include "glbProfile.h"


static cmn_cfg_method* cmn_cfg_method_get();
static int cmn_cfg_init(cmn_cfg* self);
static pchar cmn_cfg_get_workspace(cmn_cfg* self);
static pchar cmn_cfg_get_log_path(cmn_cfg* self);
static int cmn_cfg_get_log_level(cmn_cfg* self);
static pchar cmn_cfg_get_listen_ip(cmn_cfg* self);
static short cmn_cfg_get_listen_port(cmn_cfg* self);
static int cmn_cfg_get_conn_num(cmn_cfg* self);
static int cmn_cfg_get_pos_limit(cmn_cfg* self);
static pchar cmn_cfg_get_broker_id(cmn_cfg* self);
static pchar cmn_cfg_get_md_prefix(cmn_cfg* self);
static pchar cmn_cfg_get_md_front_add(cmn_cfg* self);
static pchar cmn_cfg_get_tr_front_add(cmn_cfg* self);
static char cmn_cfg_get_hedge_flag(cmn_cfg* self);
static int cmn_cfg_get_store_mduser(cmn_cfg* self);

cmn_cfg* cmn_cfg_new(char* cfg_file)
{
  cmn_cfg* self;
  self = (cmn_cfg*)malloc(sizeof(cmn_cfg));
  self->pMethod = cmn_cfg_method_get();
  self->pCfgFileName = cfg_file;

  return self;
}

void cmn_cfg_free(cmn_cfg* self)
{
  free(self);
}

cmn_cfg_method* cmn_cfg_method_get()
{
  static cmn_cfg_method stSynCfgMethods = {
    cmn_cfg_init,
    cmn_cfg_get_workspace,
    cmn_cfg_get_log_path,
    cmn_cfg_get_log_level,
    cmn_cfg_get_listen_ip,
    cmn_cfg_get_listen_port,
    cmn_cfg_get_conn_num,
    cmn_cfg_get_pos_limit,
    cmn_cfg_get_broker_id,
    cmn_cfg_get_md_prefix,
    cmn_cfg_get_md_front_add,
    cmn_cfg_get_tr_front_add,
    cmn_cfg_get_hedge_flag,
    cmn_cfg_get_store_mduser
  };
  return &stSynCfgMethods;
}

int cmn_cfg_init(cmn_cfg* self)
{
  int nRet = 0;
  char* pCfgFile;

  // 获取配置文件路径
  pCfgFile = self->pCfgFileName;

  // 依次读取参数
  nRet = glbPflGetString("SYSTEM", "WORK_PATH", pCfgFile,self->sWorkPath);
  nRet = glbPflGetString("SYSTEM", "LOG_PATH", pCfgFile,self->sLogPath);
  nRet = glbPflGetInt("SYSTEM", "LOG_LEVEL", pCfgFile,&self->nLogLevel);
  nRet = glbPflGetString("CLIENT", "LISTEN_IP", pCfgFile,self->sListenIP);
  nRet = glbPflGetInt("CLIENT", "LISTEN_PORT", pCfgFile,&self->nListenPort);
  nRet = glbPflGetInt("CLIENT", "CONN_NUM", pCfgFile,&self->nConnNum);
  
  nRet = glbPflGetInt("TRADER", "POSITION_LIMITS", pCfgFile,&self->nPosLimit);
  nRet = glbPflGetString("TRADER", "BROKER_ID", pCfgFile,self->sBrokerId);
  nRet = glbPflGetString("TRADER", "MD_PREFIX", pCfgFile,self->sMdPrefix);
  nRet = glbPflGetString("TRADER", "MD_FRONT_ADD", pCfgFile,self->sMdFrontAdd);
  nRet = glbPflGetString("TRADER", "TR_FRONT_ADD", pCfgFile,self->sTrFrontAdd);
  nRet = glbPflGetString("TRADER", "HEDGE_FLAG", pCfgFile,self->sHedgeFlag);
  nRet = glbPflGetInt("TRADER", "STORE_MDUSER", pCfgFile,&self->nStoreMduser);
  return 0;
}

pchar cmn_cfg_get_workspace(cmn_cfg* self)
{
  return self->sWorkPath;
}

pchar cmn_cfg_get_log_path(cmn_cfg* self)
{
  return self->sLogPath;
}


int cmn_cfg_get_log_level(cmn_cfg* self)
{
  return self->nLogLevel;
}

pchar cmn_cfg_get_listen_ip(cmn_cfg* self)
{
  return self->sListenIP;
}

short cmn_cfg_get_listen_port(cmn_cfg* self)
{
  return (short)self->nListenPort;
}

int cmn_cfg_get_conn_num(cmn_cfg* self)
{
  return self->nConnNum;
}

int cmn_cfg_get_pos_limit(cmn_cfg* self)
{
  return self->nPosLimit;
}

pchar cmn_cfg_get_broker_id(cmn_cfg* self)
{
  return self->sBrokerId;
}

pchar cmn_cfg_get_md_prefix(cmn_cfg* self)
{
  return self->sMdPrefix;
}

pchar cmn_cfg_get_md_front_add(cmn_cfg* self)
{
  return self->sMdFrontAdd;
}

pchar cmn_cfg_get_tr_front_add(cmn_cfg* self)
{
  return self->sTrFrontAdd;
}

char cmn_cfg_get_hedge_flag(cmn_cfg* self)
{
  return self->sHedgeFlag[0];
}

int cmn_cfg_get_store_mduser(cmn_cfg* self)
{
  return self->nStoreMduser;
}



