
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
  // ��ʼ��util
  TraderMduserProxyUtil* pMduserUtil = new TraderMduserProxyUtil(argv[1]);
  ret = pMduserUtil->init();
  if(ret < 0){
    // ��ʼ���쳣
    exit(-1);
  }

  // ��ʼ��handler
  TraderMduserProxyCtpHandler* pMduserHandler = new TraderMduserProxyCtpHandler(pMduserUtil);

  // ��ѭ��
  pMduserHandler->loop();
  
  // �ͷ�util  
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

  // ���ӽ��׷�����
  pTraderApi->Init();

  pProxyUtil->loop();

  pTraderApi->Release();

  return ;  
}

void TraderMduserProxyCtpHandler::init()
{
  // ��ȡ����
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_BROKER_ID", m_BrokerID, sizeof(m_BrokerID));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_USER", m_UserId, sizeof(m_UserId));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_PASSWD", m_Passwd, sizeof(m_Passwd));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_ADDR", m_Address, sizeof(m_Address));
  pProxyUtil->getCfgString("MDUSER_CTP", "MDUSER_WORKSPACE", m_Workspace, sizeof(m_Workspace));

  CThostFtdcMdApi* pTraderApi = CThostFtdcMdApi::CreateFtdcMdApi(m_Workspace);
  
  // ��ʼ������
  // ����
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


///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void TraderMduserProxyCtpHandler::OnFrontConnected()
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
void TraderMduserProxyCtpHandler::OnFrontDisconnected(int nReason)
{
  return;
}

///��¼������Ӧ
void TraderMduserProxyCtpHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  subMarketData();
  return;
}

///�ǳ�������Ӧ
void TraderMduserProxyCtpHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///����Ӧ��
void TraderMduserProxyCtpHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///��������Ӧ��
void TraderMduserProxyCtpHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///ȡ����������Ӧ��
void TraderMduserProxyCtpHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  return;
}

///�������֪ͨ
void TraderMduserProxyCtpHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pMarketData)
{
  // ת������
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



