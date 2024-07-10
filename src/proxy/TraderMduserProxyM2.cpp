
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include "cmn_util.h"


#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyStruct.h"
#include "TraderMduserProxyM2.h"

static void* trader_mduser_proxy_m2_thread(void* arg);


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
  TraderMduserProxyM2Handler* pMduserHandler = new TraderMduserProxyM2Handler(pMduserUtil);

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}

static void* trader_mduser_proxy_m2_thread(void* arg)
{
  TraderMduserProxyM2Handler* self = (TraderMduserProxyM2Handler*)arg;
  self->work();
  return (void*)NULL;
}

TraderMduserProxyM2Handler::TraderMduserProxyM2Handler(TraderMduserProxyUtil* util)
  :m_CpuId(-1)
  , pProxyUtil(util)
{
  init();
}

TraderMduserProxyM2Handler::~TraderMduserProxyM2Handler()
{

}

void TraderMduserProxyM2Handler::loop()
{

  // 连接交易服务器
  start();

  pProxyUtil->loop();

  stop();

  return ;  
}

void TraderMduserProxyM2Handler::work()
{
	CInitConfig* pConfig = new CInitConfig;
	memset(pConfig, 0, sizeof(CInitConfig));
	strncpy(pConfig->Aim, m_Aim, sizeof(pConfig->Aim)); //"dcel2"
	strncpy(pConfig->IfName, m_IfName, sizeof(pConfig->IfName));
	CM2Api* pApi = CM2Api::CreateM2Api();
	pApi->RegisterSpi(this);
	pApi->Init(pConfig);
  
  return (void*)NULL;
}

void TraderMduserProxyM2Handler::init()
{
  char tmp[8];
  
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_DZM2", "MDUSER_AIM", m_Aim, sizeof(m_Aim));
  pProxyUtil->getCfgString("MDUSER_DZM2", "MDUSER_IF_NAME", m_IfName, sizeof(m_IfName));
  pProxyUtil->getCfgString("MDUSER_DZM2", "CPU_ID", tmp, sizeof(tmp));
  m_CpuId = atoi(tmp);
  
  m_ThreadId = 0;

  return;
}

void TraderMduserProxyM2Handler::start()
{

  m_LoopFlag = 1;
	pthread_create(&m_ThreadId, NULL, trader_mduser_proxy_m2_thread, (void*)this);

  return ;  
}


void TraderMduserProxyM2Handler::stop()
{
  void* ret;
  m_LoopFlag = 0;
  
  if(m_ThreadId){
    pthread_cancel(m_ThreadId);
    pthread_join(m_ThreadId, &ret);
  }

  
  return ;  
}

///深度行情通知
void TraderMduserProxyM2Handler::OnRtnDepthMarketData(CMarketDataField *pMarketData)
{
  // 转发行情
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;
  
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  strncpy(pTick->InstrumentID, pMarketData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, "20240701", sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pMarketData->UpdateTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 1;
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));

  return ;
}



