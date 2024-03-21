
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

#include "TraderMduserProxyUtil.h"
#include "TraderMduserProxyRem.h"

#include "TraderMduserProxyStruct.h"

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
  TraderMduserProxyRemHandler* pMduserHandler = new TraderMduserProxyRemHandler(pMduserUtil);

  // 主循环
  pMduserHandler->loop();
  
  // 释放util  
  delete pMduserUtil;
  
  delete pMduserHandler;

  return 0;  
}


TraderMduserProxyRemHandler::TraderMduserProxyRemHandler(TraderMduserProxyUtil* util)
  :pProxyUtil(util)
{

}

TraderMduserProxyRemHandler::~TraderMduserProxyRemHandler()
{

}

void TraderMduserProxyRemHandler::loop()
{
  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;

  // 初始化变量
  EqsTcpInfo svrInfo;
	strncpy(svrInfo.m_eqsIp, m_AddressIp, sizeof(svrInfo.m_eqsIp));
	svrInfo.m_eqsPort = atoi(m_AddressPort);

  // 连接交易服务器
  pTraderApi->ConnServer(svrInfo, this);

  pProxyUtil->loop();

  pTraderApi->DisConnServer();

  return ;  
}

void TraderMduserProxyRemHandler::init()
{
  // 读取参数
  pProxyUtil->getCfgString("MDUSER_REM", "MDUSER_BROKER_ID", m_BrokerID, sizeof(m_BrokerID));
  pProxyUtil->getCfgString("MDUSER_REM", "MDUSER_USER", m_UserId, sizeof(m_UserId));
  pProxyUtil->getCfgString("MDUSER_REM", "MDUSER_PASSWD", m_Passwd, sizeof(m_Passwd));
  pProxyUtil->getCfgString("MDUSER_REM", "MDUSER_ADDR_IP", m_AddressIp, sizeof(m_AddressIp));
  pProxyUtil->getCfgString("MDUSER_REM", "MDUSER_ADDR_PORT", m_AddressPort, sizeof(m_AddressPort));

  EESQuoteApi* pTraderApi = CreateEESQuoteApi();

  m_Arg = (void*)pTraderApi;
  m_RequestId = 1;
  m_ContractIdx = 0;

  return;
}

void TraderMduserProxyRemHandler::login()
{
  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;
  EqsLoginParam loginParam;
  strncpy(loginParam.m_loginId, m_UserId, sizeof(loginParam.m_loginId));
  strncpy(loginParam.m_password, m_Passwd, sizeof(loginParam.m_password));

  pTraderApi->LoginToEqs(loginParam);
  return;
}

void TraderMduserProxyRemHandler::subMarketData()
{
  int contractNum = pProxyUtil->getContractNum();
  if(m_ContractIdx >= contractNum){
    return;
  }
  
  char* instrument = pProxyUtil->getContractId(m_ContractIdx);
  m_ContractIdx++;
  
  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;
  
  pTraderApi->RegisterSymbol(EQS_FUTURE, instrument);


  return ;
}

void TraderMduserProxyRemHandler::OnEqsConnected()
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
void TraderMduserProxyRemHandler::OnEqsDisconnected()
{
  return;
}

///登录请求响应
void TraderMduserProxyRemHandler::OnLoginResponse(bool bSuccess, const char* pReason)
{
  if(!bSuccess){
    return;
  }
  m_ContractIdx = 0;
  subMarketData();
  return;
}

///订阅行情应答
void TraderMduserProxyRemHandler::OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  subMarketData();
  return;
}

///取消订阅行情应答
void TraderMduserProxyRemHandler::OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  return;
}

///深度行情通知
void TraderMduserProxyRemHandler::OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData)
{
  // 转发行情
  trader_mduser_evt oEvent;
  trader_tick* pTick = &oEvent.Tick;
  
  oEvent.Type = MDUSERONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  
  strncpy(pTick->InstrumentID, pDepthQuoteData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, pDepthQuoteData->TradingDay, sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pDepthQuoteData->UpdateTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pDepthQuoteData->UpdateMillisec;
  pTick->BidPrice1 = pDepthQuoteData->BidPrice1;
  pTick->BidVolume1 = pDepthQuoteData->BidVolume1;
  pTick->AskPrice1 = pDepthQuoteData->AskPrice1;
  pTick->AskVolume1 = pDepthQuoteData->AskVolume1;
  pTick->UpperLimitPrice = pDepthQuoteData->UpperLimitPrice;
  pTick->LowerLimitPrice = pDepthQuoteData->LowerLimitPrice;
  pTick->LastPrice = pDepthQuoteData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 0;
  
  pProxyUtil->sendData((void*)&oEvent, sizeof(oEvent));
  return ;
}



