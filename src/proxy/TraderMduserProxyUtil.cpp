#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>


#include "TraderMduserProxyUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "hiredis.h"
#include "trader_tick_dict.h"
#include "mqueue_client.h"

#include "glbProfile.h"

#include "cmn_log.h"

static void trader_mduser_proxy_signal_cb(evutil_socket_t fd, short event, void *arg);
#ifdef __cplusplus
}
#endif

#define MDUSER_INSTRUMENTS "MDUSER.INSTRUMENTS"

void trader_mduser_proxy_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  TraderMduserProxyUtil* self = (TraderMduserProxyUtil*)arg;
  self->stop();
  
}


TraderMduserProxyUtil::TraderMduserProxyUtil(const char* cfgFile)
{
  strncpy(m_cfgFile, cfgFile, sizeof(m_cfgFile));
  m_contractNum = 0;
  m_contracts = (contract_id_t*)NULL;
  m_contractDict = trader_tick_dict_new();
}

TraderMduserProxyUtil::~TraderMduserProxyUtil()
{
  if(m_contracts){
    free(m_contracts);
  }

  if(m_contractDict){
    trader_tick_dict_free(m_contractDict);
  }
}

// 初始化
int TraderMduserProxyUtil::init()
{
  int ret;

  char sTemp[32];
  char sRedisIp[32];
  int nRedisPort = 0;
  char sContractKey[32]; 
  char sMqName[32];
  
  redisContext* pRedisCtx;

  // 各种初始化
  CMN_DEBUG("self->pBase\n");
  m_pBase = event_base_new();
  CMN_ASSERT(m_pBase);
  
  //signal event init
  CMN_DEBUG("self->pSigIntEvt\n");
  m_pSigIntEvt = evsignal_new(m_pBase, SIGINT, trader_mduser_proxy_signal_cb, (void *)this);
  CMN_ASSERT(m_pSigIntEvt);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  m_pSigTermEvt = evsignal_new(m_pBase, SIGTERM, trader_mduser_proxy_signal_cb, (void *)this);
  CMN_ASSERT(m_pSigIntEvt);
  
  // 获取读取配置文件
  ret = getCfgString("RUN_CONFIG", "REDIS_ADDR", sRedisIp, sizeof(sRedisIp));
  CMN_ASSERT(ret >= 0);

  ret = getCfgString("RUN_CONFIG", "REDIS_PORT", sTemp, sizeof(sTemp));
  CMN_ASSERT(ret >= 0);
  nRedisPort = atoi(sTemp);

  ret = getCfgString("RUN_CONFIG", "REDIS_INST_KEY", sContractKey, sizeof(sContractKey));
  CMN_ASSERT(ret >= 0);

  // 连接redis
  // todo
  CMN_DEBUG("self->pRedisCtx\n");
  struct timeval tv = {
    5, 0
  };
  
  pRedisCtx = redisConnectWithTimeout(sRedisIp, nRedisPort, tv);
  CMN_ASSERT(pRedisCtx);
  CMN_ASSERT(0 == pRedisCtx->err);

  // 读取合约
  redisReply* reply = (redisReply*)redisCommand(pRedisCtx, "SMEMBERS %s", sContractKey);
  redisReply* r;
  int i;
  do {
    if(REDIS_REPLY_ARRAY == reply->type){

      m_contractNum = reply->elements;
      m_contracts = (contract_id_t*)malloc(m_contractNum * sizeof(contract_id_t));
      for(i = 0; i < m_contractNum; i++){
        r = reply->element[i];
        strncpy(m_contracts[i], r->str, sizeof(contract_id_t));
        m_contractDict->pMethod->xAdd(m_contractDict, m_contracts[i], (void*)NULL);
      }
      break;
    }
  }while(0);
  freeReplyObject(reply);
  redisFree(pRedisCtx);

  ret = getCfgString("RUN_CONFIG", "MQ_NAME", sMqName, sizeof(sMqName));
  CMN_ASSERT(ret >= 0);

  ret = mqueue_client_context_init(&m_mqClientCtx, sMqName);
  CMN_ASSERT(ret >= 0);

  i = 0;
  do{
    m_client[i].used = 0;
    ret = getCfgString("RUN_CONFIG", "MQ_NAME1", sMqName, sizeof(sMqName));
    if(ret < 0){
      break;
    }
    ret = mqueue_client_context_init(&m_client[i].m_mqClientCtx, sMqName);
    if(ret < 0){
      break;
    }
    m_client[i].used = 1;
  }while(0);
  i++;
  
  do{
    m_client[i].used = 0;
    ret = getCfgString("RUN_CONFIG", "MQ_NAME2", sMqName, sizeof(sMqName));
    if(ret < 0){
      break;
    }
    ret = mqueue_client_context_init(&m_client[i].m_mqClientCtx, sMqName);
    if(ret < 0){
      break;
    }
    m_client[i].used = 1;
  }while(0);
  i++;

  return 0;
}

// 主循环
void TraderMduserProxyUtil::loop()
{
  int i;
  int ret;
  ret = event_add(m_pSigIntEvt, NULL);
  CMN_ASSERT(ret >= 0);
  
  ret = event_add(m_pSigTermEvt, NULL);
  CMN_ASSERT(ret >= 0);

  ret = event_base_dispatch(m_pBase);

  mqueue_client_context_close(&m_mqClientCtx);
  for(i = 0; i < BACKUP_MQ_COUNT; i++){
    if(!m_client[i].used){
      continue;
    }
  	mqueue_client_context_close(&m_client[i].m_mqClientCtx);
  }

  return;
}

// 退出
void TraderMduserProxyUtil::stop()
{
  struct event_base *base = m_pBase;
  struct timeval delay = { 1, 0 };

  event_del(m_pSigIntEvt);
  event_del(m_pSigTermEvt);

  event_base_loopexit(base, &delay);
  
  return;
}


// 获取配置信息
int TraderMduserProxyUtil::getCfgString(const char* section, const char* entry, char* value, int size)
{
  int ret;
  char temp[1024];
  ret = glbPflGetString(section, entry, m_cfgFile, temp);
  if(ret < 0){
    return -1;
  }
  strncpy(value, temp, size);
  return 0;
}

// 获取合约个数
int TraderMduserProxyUtil::getContractNum()
{
  return m_contractNum;
}

// 获取指定需要合约名称
const char* TraderMduserProxyUtil::getContractId(int idx)
{
  if(idx < 0 || idx > m_contractNum){
    return (char*)NULL;
  }
  return m_contracts[idx];
}

// 判断合约是否存在
int TraderMduserProxyUtil::findContractById(const char* contractId)
{
  int ret;
  void* savePrt;
  ret = m_contractDict->pMethod->xFind(m_contractDict, contractId, &savePrt);
  return ret;
}

// 发送行情数据
int TraderMduserProxyUtil::sendData(void* data, int size)
{
  int ret;
  ret = mqueue_client_context_send(&m_mqClientCtx, (char*)data, size);

  int i;
  for(i = 0; i < BACKUP_MQ_COUNT; i++){
    if(!m_client[i].used){
      continue;
    }
  	mqueue_client_context_send(&m_client[i].m_mqClientCtx, (char*)data, size);
  }
  return ret;
}
  

