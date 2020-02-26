
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <chrono>
#include <iostream>
#include <thread>

#include <string>

#include "EClientSocket.h"
#include "EPosixClientSocketPlatform.h"

#include "Contract.h"
#include "Order.h"
#include "OrderState.h"
#include "Execution.h"
#include "CommissionReport.h"
#include "ScannerSubscription.h"
#include "executioncondition.h"
#include "PriceCondition.h"
#include "MarginCondition.h"
#include "PercentChangeCondition.h"
#include "TimeCondition.h"
#include "VolumeCondition.h"
#include "CommonDefs.h"

#include "IBFutureContractFactory.h"
#include "IBTraderApiBase.h"

CIbTraderApiBase::CIbTraderApiBase():
m_contractFactory(IBFutureContractFactory::GetInstance())
, m_osSignal(2000)//2-seconds timeout
, m_pReader(0)
, m_hReadThread(pthread_self())
, m_extraAuth(false)
, m_clientId(1)
{

}

void CIbTraderApiBase::RegisterSpi(CIbTraderSpi *pSpi)
{
  this->m_traderSpi = pSpi;
}

///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CIbTraderApiBase::Init()
{  
  m_pClient = new EClientSocket(this->m_traderSpi, &m_osSignal);
  bool res = false;
  int ret = 0;
  
  char host[256];
  int port = 0;

  ret = PraseUrl(this->m_szFrontAddress.c_str(), host, &port);
  if(ret < 0){
    printf( "PraseUrl %s failed!\n", this->m_szFrontAddress.c_str());
  }

  m_host = host;
  m_port = port;

  StartThread();

}

///等待接口线程结束运行
///@return 线程退出代码
int CIbTraderApiBase::Join()
{
  return 0;
}

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CIbTraderApiBase::Release()
{
  Disconnect();
  
  m_threadLoop = false;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  pthread_join(m_hReadThread, NULL);

  if (m_pReader){
    delete m_pReader;
    m_pReader = NULL;
  }
  delete m_pClient;
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CIbTraderApiBase::RegisterFront(const char *pszFrontAddress)
{
  m_szFrontAddress = pszFrontAddress;
}

IBFutureContract* CIbTraderApiBase::GetFutureContract(const char* contract)
{
  return m_contractFactory->Search(contract);
}

IBFutureContract* CIbTraderApiBase::GetFutureContract(long ticker_id)
{
  return m_contractFactory->Search(ticker_id);
}

EClientSocket* CIbTraderApiBase::Api()
{
  return m_pClient;
}

bool CIbTraderApiBase::Connect()
{
  return Connect(m_host.c_str(), m_port, m_clientId);
}

bool CIbTraderApiBase::Connect(const char *host, int port, int clientId)
{
	// trying to connect
	printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);
	
	//! [connect]
	bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);
	//! [connect]
	
	if (bRes) {
		printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
		//! [ereader]
    m_pReader = new EReader(m_pClient, &m_osSignal);
		m_pReader->start();
		//! [ereader]
	}
	else
		printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

	return bRes;
}

void CIbTraderApiBase::Disconnect()
{
	m_pClient->eDisconnect();

	printf ( "Disconnected\n");
}

void CIbTraderApiBase::RegisterClientId(int clientId)
{
  m_clientId = clientId;
}

int CIbTraderApiBase::PraseUrl(const char* url, char* host, int* port)
{
  char* p;
  char* q;
  // 定位://
  p = (char*)strstr(url, "://");
  if(NULL == p){
    return -1;
  }
  // 移动3个字符
  p += 3;
  
  // 定位:
  q = (char*)strstr(p, ":");
  if(NULL == q){
    return -2;
  }

  // 获取host
  memcpy(host, p, q - p);
  host[q-p] = '\0';

  //移动1个字符
  q++;

  // 获取port
  *port = atoi(q);
  return 0;
}

void CIbTraderApiBase::StartThread()
{
  m_threadLoop = true;
  pthread_create(&m_hReadThread, NULL, ProcessThread, this);

}

void* CIbTraderApiBase::ProcessThread(void * lpParam)
{
	CIbTraderApiBase *pThis = reinterpret_cast<CIbTraderApiBase *>(lpParam);
  pThis->ProcessMessage();
	return 0;

}

void CIbTraderApiBase::ProcessMessage()
{
  bool res = false;
  while(m_threadLoop){
    //建立连接
    res = Connect();
    if(!res){
      std::this_thread::sleep_for(std::chrono::seconds(2));
      continue;
    }
    
    // 收取数据
    while(m_pClient->isSocketOK()){
    	m_osSignal.waitForSignal();
    	errno = 0;
    	m_pReader->processMsgs();
    }

    delete m_pReader;
    m_pReader = (EReader*)NULL;
  }
}




