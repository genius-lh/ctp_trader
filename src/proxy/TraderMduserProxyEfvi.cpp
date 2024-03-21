
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include "efvi_receive_depend.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyEfvi.h"

#include "TraderMduserProxyStruct.h"

static void* trader_mduser_proxy_ef_vi_thread(void* arg);
static int trader_mduser_proxy_ef_vi_read(void* arg, const char* data, int size);


int main(int argc, char* argv[])
{
  int ret;
  // 初始化util
  TraderMduserProxyUtil* pMduserUtil = new TraderMduserProxyUtil(argv[1]);
  ret = pMduserUtil->init();
  if(ret < 0){
    // 初始化异常
    exit(-1);
  }

  // 初始化handler
  TraderMduserProxyEfviHandler* pMduserHandler = new TraderMduserProxyEfviHandler(pMduserUtil);

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}

static void* trader_mduser_proxy_ef_vi_thread(void* arg)
{
  TraderMduserProxyEfviHandler* self = (TraderMduserProxyEfviHandler*)arg;
  self->work();
  return (void*)NULL;
}

int trader_mduser_proxy_ef_vi_read(void* arg, const char* data, int size)
{
  TraderMduserProxyEfviHandler* self = (TraderMduserProxyEfviHandler*)arg;
  self->OnRecvMessage(data, size);
  return 0;
}

TraderMduserProxyEfviHandler::TraderMduserProxyEfviHandler(TraderMduserProxyUtil* util)
  :pProxyUtil(util)
{

}

TraderMduserProxyEfviHandler::~TraderMduserProxyEfviHandler()
{

}

void TraderMduserProxyEfviHandler::loop()
{

  // 连接交易服务器
  start();

  pProxyUtil->loop();

  stop();

  return ;  
}

void TraderMduserProxyEfviHandler::work()
{
  int ev = 0;

  while(m_LoopFlag){
    ev = poll_resources(&m_Res);
    if(!ev){
      //wait_resources(&m_Res);
    }
  }

  return (void*)NULL;
  

}

void TraderMduserProxyEfviHandler::init()
{
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_EFVI", "MDUSER_REMOTE_IP", m_RemoteIp, sizeof(m_RemoteIp));
  pProxyUtil->getCfgString("MDUSER_EFVI", "MDUSER_REMOTE_PORT", m_RemotePort, sizeof(m_RemotePort));
  pProxyUtil->getCfgString("MDUSER_EFVI", "MDUSER_ETH_NAME", m_EthName, sizeof(m_EthName));
  pProxyUtil->getCfgString("MDUSER_EFVI", "MDUSER_DATA_TYPE", m_DataType, sizeof(m_DataType));
  m_ThreadId = 0;

  trader_mduser_api_ef_vi_ops_init(&m_Ops, atoi(m_DataType));

  return;
}

void TraderMduserProxyEfviHandler::start()
{

  init_resources(&m_Res, m_EthName, m_RemoteIp, atoi(m_RemotePort));
  m_Res.read = trader_mduser_proxy_ef_vi_read;
  m_Res.arg = (void*)this;

  m_LoopFlag = 1;
	pthread_create(&m_ThreadId, NULL, trader_mduser_proxy_ef_vi_thread, (void*)this);

  return ;  
}


void TraderMduserProxyEfviHandler::stop()
{
  void* ret;
  m_LoopFlag = 0;
  
  if(m_ThreadId){
    pthread_join(m_ThreadId, &ret);
  }

  
  free_resources(&m_Res);
  return ;  
}

///深度行情通知
int TraderMduserProxyEfviHandler::OnRecvMessage(char* data, int size)
{
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;

  const char* InstrumentID = data + m_Ops.m_md_id_pos;

  int md_size = m_Ops.m_md_size;
  if(md_size > size){    
    return md_size;
  }
  
  int found = pProxyUtil->findContractById(InstrumentID);
  if(!found){
    return md_size;
  }

  // 转发行情
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  m_Ops.md_fill(pTick, data);
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  return md_size;
}



