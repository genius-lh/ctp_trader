
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>
#include <arpa/inet.h>
#include <exanic/exanic.h>
#include <exanic/fifo_rx.h>
#include <time.h>

#include "cmn_util.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyExanic.h"

#include "TraderMduserProxyStruct.h"

#pragma pack(push, 1)
typedef struct
{
	unsigned char 	Flag;					//Э���־ F1
	char 			TypeID;					//Э��汾 ������Lv2 Type 09
	unsigned short  Length;					//������
	char			InstrumentID[19];		//��Լ
	char			UpdateTime[13];
	char			Padding[4];				//ռλ����������
	double			LastPrice;		
	int				Volume;
	int				OpenInterest;
	double			Turnover;
	double			BidPrice1;
	int				BidVolume1;
	double			AskPrice1;
	int				AskVolume1;
  double       Reserved;
}Zbp09Best;
#pragma pack(pop)


static void* trader_mduser_proxy_exanic_thread(void* arg);


int main(int argc, char* argv[])
{
  int ret;
  
  printf("sizeof(Zbp09Best): %ld\n", sizeof(Zbp09Best));
  // ��ʼ��util
  TraderMduserProxyUtil* pMduserUtil = new TraderMduserProxyUtil(argv[1]);
  ret = pMduserUtil->init();
  if(ret < 0){
    // ��ʼ���쳣
    exit(-1);
  }

  // ��ʼ��handler
  TraderMduserProxyExanicHandler* pMduserHandler = new TraderMduserProxyExanicHandler(pMduserUtil);

  // ��ѭ��
  pMduserHandler->loop();
  
  // �ͷ�util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}

static void* trader_mduser_proxy_exanic_thread(void* arg)
{
  TraderMduserProxyExanicHandler* self = (TraderMduserProxyExanicHandler*)arg;
  self->work();
  return (void*)NULL;
}


TraderMduserProxyExanicHandler::TraderMduserProxyExanicHandler(TraderMduserProxyUtil* util)
  :m_CpuId(-1)
  , pProxyUtil(util)
{
  init();
}

TraderMduserProxyExanicHandler::~TraderMduserProxyExanicHandler()
{

}

void TraderMduserProxyExanicHandler::loop()
{

  // ���ӽ��׷�����
  start();

  pProxyUtil->loop();

  stop();

  return ;  
}

void TraderMduserProxyExanicHandler::work()
{
  char *device = m_EthName;
  int port = 0;
  
  exanic_t *exanic = exanic_acquire_handle(device);
  if (!exanic)
  {
    fprintf(stderr, "exanic_acquire_handle: %s\n", exanic_get_last_error());
    exit(1);
  }
  
  exanic_rx_t *rx = exanic_acquire_rx_buffer(exanic, port, 0);
  if (!rx)
  {
    fprintf(stderr, "exanic_acquire_rx_buffer: %s\n", exanic_get_last_error());
    exit(1);
  }
  
  char buf[2048];
  exanic_cycles32_t timestamp;
  
	cmn_util_bind_cpu(m_CpuId);
  
  while (m_LoopFlag)
  {
    ssize_t sz = exanic_receive_frame(rx, buf, sizeof(buf), &timestamp);
    if (sz > 4)
    {
      OnRecvMessage(buf, (int)sz);
    }
  }
  
  exanic_release_rx_buffer(rx);
  exanic_release_handle(exanic);

  return (void*)NULL;
}

void TraderMduserProxyExanicHandler::init()
{
  char tmp[8];

  // ��ȡ����
  pProxyUtil->getCfgString("MDUSER_EXANIC", "MDUSER_ETH_NAME", m_EthName, sizeof(m_EthName));
  pProxyUtil->getCfgString("MDUSER_EXANIC", "MDUSER_DATA_TYPE", m_DataType, sizeof(m_DataType));
  pProxyUtil->getCfgString("MDUSER_EXANIC", "CPU_ID", tmp, sizeof(tmp));
  m_CpuId = atoi(tmp);
  m_ThreadId = 0;

  trader_mduser_api_ef_vi_ops_init(&m_Ops, atoi(m_DataType));

  return;
}

void TraderMduserProxyExanicHandler::start()
{
  m_LoopFlag = 1;
	pthread_create(&m_ThreadId, NULL, trader_mduser_proxy_exanic_thread, (void*)this);

  return ;  
}


void TraderMduserProxyExanicHandler::stop()
{
  void* ret;
  m_LoopFlag = 0;
  
  if(m_ThreadId){
    pthread_join(m_ThreadId, &ret);
  }

  return ;  
}

///�������֪ͨ
int TraderMduserProxyExanicHandler::OnRecvMessage(char* data, int size)
{
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;
  Zbp09Best* pMarketData = (Zbp09Best*)data;
  const char* InstrumentID = pMarketData->InstrumentID;

  int md_size = sizeof(Zbp09Best);
  if(md_size != size){    
    return size;
  }
  
  int found = pProxyUtil->findContractById(InstrumentID);
  if(!found){
    return md_size;
  }

  // ת������
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  strncpy(pTick->InstrumentID, pMarketData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, "20240101", sizeof(pTick->TradingDay));
  memcpy(pTick->UpdateTime, pMarketData->UpdateTime, sizeof(pTick->UpdateTime)-1);
  pTick->UpdateTime[sizeof(pTick->UpdateTime)-1] = '\0';
  pTick->UpdateMillisec = atoi(&pMarketData->UpdateTime[9]);
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 1;
  
  int ret = pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  if(ret < 0){
    fprintf(stderr, "pProxyUtil->sendData: %d\n", ret);
  }
  return md_size;
}



