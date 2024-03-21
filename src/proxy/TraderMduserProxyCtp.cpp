
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include <time.h>
#include <sys/time.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcMdApi.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyCtp.h"

#include "TraderMduserProxyStruct.h"

#ifdef CTP_SOPT
using namespace ctp_sopt;
#endif

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
  TraderMduserProxyCtpHandler* pMduserHandler = new TraderMduserProxyCtpHandler(pMduserUtil);

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}


TraderMduserProxyCtpHandler::TraderMduserProxyCtpHandler(TraderMduserProxyUtil* util)
  :pProxyUtil(util)
{

}

TraderMduserProxyCtpHandler::~TraderMduserProxyCtpHandler()
{

}

void TraderMduserProxyCtpHandler::loop()
{
  CThostFtdcMdApi* pTraderApi = (CThostFtdcMdApi*)m_Arg;

  // 连接交易服务器
  pTraderApi->Init();

  pProxyUtil->loop();

  pTraderApi->Release();

  return ;  
}

void TraderMduserProxyCtpHandler::init()
{
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_BROKER_ID", m_BrokerID, sizeof(m_BrokerID));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_USER", m_UserId, sizeof(m_UserId));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_PASSWD", m_Passwd, sizeof(m_Passwd));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_ADDR", m_Address, sizeof(m_Address));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_WORKSPACE", m_Workspace, sizeof(m_Workspace));

  CThostFtdcMdApi* pTraderApi = CThostFtdcMdApi::CreateFtdcMdApi(m_Workspace);
  
  // 初始化变量
  // 交易
  pTraderApi->RegisterSpi(this);
  pTraderApi->RegisterFront(m_Address);

  m_Arg = (void*)pTraderApi;
  m_RequestId = 1;

  return;
}

void TraderMduserProxyCtpHandler::login()
{
  CThostFtdcMdApi* pTraderApi = (CThostFtdcMdApi*)m_Arg;
  CThostFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_Passwd);

  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
  return;
}

void TraderMduserProxyCtpHandler::subMarketData()
{
  int contractNum = pProxyUtil->getContractNum();
  int i;
  char** contracts = new char*[contractNum];
  for(i = 0; i < contractNum; i++){
    contracts[i] = pProxyUtil->getContractId(i);
  }
  
  CThostFtdcMdApi* pTraderApi = (CThostFtdcMdApi*)m_Arg;
  pTraderApi->SubscribeMarketData(contracts, contractNum);

  delete [] contracts;

  return ;
}


///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void TraderMduserProxyCtpHandler::OnFrontConnected()
{
  login();
  return ;

}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void TraderMduserProxyCtpHandler::OnFrontDisconnected(int nReason)
{
  return;
}

///登录请求响应
void TraderMduserProxyCtpHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  subMarketData();
  return;
}

///登出请求响应
void TraderMduserProxyCtpHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///错误应答
void TraderMduserProxyCtpHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///订阅行情应答
void TraderMduserProxyCtpHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///取消订阅行情应答
void TraderMduserProxyCtpHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///深度行情通知
void TraderMduserProxyCtpHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pMarketData)
{
  // 转发行情
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;
  
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  strncpy(pTick->InstrumentID, pMarketData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, pMarketData->TradingDay, sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pMarketData->UpdateTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = pMarketData->UpperLimitPrice;
  pTick->LowerLimitPrice = pMarketData->LowerLimitPrice;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 0;
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  return ;
}



