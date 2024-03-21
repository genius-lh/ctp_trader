
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
  // ��ʼ��util
  TraderMduserProxyUtil* pMduserUtil = new TraderMduserProxyUtil(argv[1]);
  ret = pMduserUtil->init();
  if(ret < 0){
    // ��ʼ���쳣
    exit(-1);
  }

  // ��ʼ��handler
  TraderMduserProxyRemHandler* pMduserHandler = new TraderMduserProxyRemHandler(pMduserUtil);

  // ��ѭ��
  pMduserHandler->loop();
  
  // �ͷ�util  
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

  // ��ʼ������
  EqsTcpInfo svrInfo;
	strncpy(svrInfo.m_eqsIp, m_AddressIp, sizeof(svrInfo.m_eqsIp));
	svrInfo.m_eqsPort = atoi(m_AddressPort);

  // ���ӽ��׷�����
  pTraderApi->ConnServer(svrInfo, this);

  pProxyUtil->loop();

  pTraderApi->DisConnServer();

  return ;  
}

void TraderMduserProxyRemHandler::init()
{
  // ��ȡ����
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

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void TraderMduserProxyRemHandler::OnEqsDisconnected()
{
  return;
}

///��¼������Ӧ
void TraderMduserProxyRemHandler::OnLoginResponse(bool bSuccess, const char* pReason)
{
  if(!bSuccess){
    return;
  }
  m_ContractIdx = 0;
  subMarketData();
  return;
}

///��������Ӧ��
void TraderMduserProxyRemHandler::OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  subMarketData();
  return;
}

///ȡ����������Ӧ��
void TraderMduserProxyRemHandler::OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  return;
}

///�������֪ͨ
void TraderMduserProxyRemHandler::OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData)
{
  // ת������
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



