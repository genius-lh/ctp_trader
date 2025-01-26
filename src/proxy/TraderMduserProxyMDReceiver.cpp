
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include <iostream>

#include "cmn_util.h"

#include "MDReceiver.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyMDReceiver.h"

#include "TraderMduserProxyStruct.h"

using namespace std;

#pragma pack(1)
struct MarketData {
    char MsgID;             // 消息标识
    char InstrumentID[16];  // 合约代码
    char SnapTime[9];       // 快照秒级时间
    uint32_t SnapMillisec;  // 快照毫秒级时间
    double HighestPrice;    // 最高价
    double LowestPrice;     // 最低价
    double LastPrice;       // 最新价
    int32_t Volume;         // 成交量
    int32_t Reserved;       // 预留位
    double Turnover;        // 成交额
    double OpenInterest;    // 持仓量
    double Bid1Price;       // 买一价
    int32_t Bid1Volume;     // 买一量
    double Ask1Price;       // 卖一价
    int32_t Ask1Volume;     // 卖一量
    double Bid2Price;       // 买二价
    int32_t Bid2Volume;     // 买二量
    double Bid3Price;       // 买三价
    int32_t Bid3Volume;     // 买三量
    double Ask2Price;       // 卖二价
    int32_t Ask2Volume;     // 卖二量
    double Ask3Price;       // 卖三价
    int32_t Ask3Volume;     // 卖三量
    double Bid4Price;       // 买四价
    int32_t Bid4Volume;     // 买四量
    double Bid5Price;       // 买五价
    int32_t Bid5Volume;     // 买五量
    double Ask4Price;       // 卖四价
    int32_t Ask4Volume;     // 卖四量
    double Ask5Price;       // 卖五价
    int32_t Ask5Volume;     // 卖五量
    double OpenPrice;       // 开盘价
};
#pragma pack()

static void* trader_mduser_proxy_md_receiver_thread(void* arg);
static void dataHandler(const uint8_t* data, uint32_t size, void* opaque);
static TraderMduserProxyMdReceiverHandler* s_pMduserHandler;


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
  TraderMduserProxyMdReceiverHandler* pMduserHandler = new TraderMduserProxyMdReceiverHandler(pMduserUtil);

  s_pMduserHandler = pMduserHandler;

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}

static void* trader_mduser_proxy_md_receiver_thread(void* arg)
{
  TraderMduserProxyMdReceiverHandler* self = (TraderMduserProxyMdReceiverHandler*)arg;
  self->work();
  return (void*)NULL;
}

void dataHandler(const uint8_t* data, uint32_t size, void* opaque) {
    if (sizeof(MarketData) == size) {
        s_pMduserHandler->OnRecvMessage((char*)data, size);
    }
}

TraderMduserProxyMdReceiverHandler::TraderMduserProxyMdReceiverHandler(TraderMduserProxyUtil* util)
  :m_CpuId(-1)
  , pProxyUtil(util)
{
  init();
}

TraderMduserProxyMdReceiverHandler::~TraderMduserProxyMdReceiverHandler()
{

}

void TraderMduserProxyMdReceiverHandler::loop()
{

  // 连接交易服务器
  start();

  pProxyUtil->loop();

  stop();

  return ;  
}

void TraderMduserProxyMdReceiverHandler::work()
{
	cmn_util_bind_cpu(m_CpuId);

  try {
      MDReceiver receiver(m_BusId, string(m_FilterName));
      while (m_LoopFlag) {
          receiver.getAndProcess(dataHandler);
      }
  } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
  }

  return (void*)NULL;
}

void TraderMduserProxyMdReceiverHandler::init()
{
  char tmp[8];
  
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_XHGZ", "BUS_ID", tmp, sizeof(tmp));
  m_BusId = atoi(tmp);
  pProxyUtil->getCfgString("MDUSER_XHGZ", "FILTER_NAME", m_FilterName, sizeof(m_FilterName));
  pProxyUtil->getCfgString("MDUSER_XHGZ", "CPU_ID", tmp, sizeof(tmp));
  m_CpuId = atoi(tmp);
  
  m_ThreadId = 0;

  return;
}

void TraderMduserProxyMdReceiverHandler::start()
{

  m_LoopFlag = 1;
	pthread_create(&m_ThreadId, NULL, trader_mduser_proxy_md_receiver_thread, (void*)this);

  return ;  
}


void TraderMduserProxyMdReceiverHandler::stop()
{
  void* ret;
  m_LoopFlag = 0;
  
  if(m_ThreadId){
    pthread_join(m_ThreadId, &ret);
  }

  return ;  
}

///深度行情通知
int TraderMduserProxyMdReceiverHandler::OnRecvMessage(char* data, int size)
{
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;

  MarketData* pMarketData = (MarketData*)data;

  const char* InstrumentID = pMarketData->InstrumentID;
  
  int found = pProxyUtil->findContractById(InstrumentID);
  if(!found){
    return size;
  }

  // 转发行情
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  strncpy(pTick->InstrumentID, pMarketData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, "20241030", sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pMarketData->SnapTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pMarketData->SnapMillisec;
  pTick->BidPrice1 = pMarketData->Bid1Price;
  pTick->BidVolume1 = pMarketData->Bid1Volume;
  pTick->AskPrice1 = pMarketData->Ask1Price;
  pTick->AskVolume1 = pMarketData->Ask1Volume;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 0;
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  return size;
}



