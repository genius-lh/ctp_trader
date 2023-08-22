
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

#include "FemasMduserTest.h"

//using namespace ctp_sopt;

#define CTP_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
  char sFilename[128];
  int i = 1;

  if(argc < 6){
    printf("input sAddress:\n");
    scanf("%s", sAddress);
    printf("input sBrokerID:\n");
    scanf("%s", sBrokerID);
    printf("input username:\n");
    scanf("%s", sUserId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  CUstpFtdcMduserApi* pTraderApi = CUstpFtdcMduserApi::CreateFtdcMduserApi("./");
  CFemasMduserHandler* pTraderHandler = new CFemasMduserHandler();
  // ��ʼ������
  // ����
  pTraderApi->RegisterSpi(pTraderHandler);

  pTraderApi->SubscribeMarketDataTopic (21001, USTP_TERT_QUICK);
  
  pTraderApi->SubscribeMarketDataTopic (31001, USTP_TERT_QUICK);
  
  pTraderApi->SubscribeMarketDataTopic (3101, USTP_TERT_QUICK);
  
  pTraderApi->SubscribeMarketDataTopic (4101, USTP_TERT_QUICK);

  pTraderApi->SubscribeMarketDataTopic (5101, USTP_TERT_QUICK);

  pTraderApi->SubscribeMarketDataTopic (100, USTP_TERT_QUICK);

  pTraderApi->RegisterFront(sAddress);

  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_Filename = sFilename;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;

  // ���ӽ��׷�����
  pTraderApi->Init();

  pTraderHandler->Loop();

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}

void CFemasMduserHandler::Loop()
{
  int choose;
  m_Loop = 1;
  do{
    choose = ShowMenu();
    switch(choose){
    case 1:
      SubMarketData();
      break;
    case 2:
      UnSubMarketData();
      break;
    case 3:
      GetMarketTopic();
      break;
    case 0:
      LogOut();
      sleep(1);
      break;
    default:
      break;
    }
  }while(m_Loop);
  return ;

}

int CFemasMduserHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "ѡ����Ҫִ�еĲ���\n"
        "1-���ĺ�Լ\n"
        "2-ȡ�����ĺ�Լ\n"
        "3-��ѯ�ڻ�������\n"
        "0-�˳�\n"
        "**********************\n"
        "��ѡ��"
  );
  scanf("%d",&ret);

  return ret;

}

void CFemasMduserHandler::LogOut()
{
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  struct CUstpFtdcReqUserLogoutField data3;
  memset(&data3, 0, sizeof(data3));
	///���͹�˾����
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///Ͷ���ߴ���
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));

  pTraderApi->ReqUserLogout(&data3, m_RequestId++);
  
  m_Loop = 0;

  return ;
}

void CFemasMduserHandler::SubMarketData()
{
  CTP_LOG("1 - ���ĺ�Լ\n");
  char instrument[31];
  printf("�������Լ:");
  scanf("%s", instrument);

  char* contracts[] = {instrument};
  
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  pTraderApi->SubMarketData(contracts, 1);

  return ;
}

void CFemasMduserHandler::UnSubMarketData()
{
  CTP_LOG("2 - ȡ�����ĺ�Լ\n");
  char instrument[31];
  printf("�������Լ:");
  scanf("%s", instrument);
  
  char* contracts[] = {instrument};
  
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  pTraderApi->UnSubMarketData(contracts, 1);

  return ;
}

void CFemasMduserHandler::GetMarketTopic()
{
  CTP_LOG("3 - GetMarketTopic\n");
  
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;

  char ExchangeID[16];
  printf("������ExchangeID:");
  scanf("%s", ExchangeID);
  pTraderApi->GetMarketTopic(ExchangeID);

  return ;
}


void CFemasMduserHandler::Flush()
{
  char line[1000];
  int len;
  if(!m_Cursor){
    return ;
  }

  char* buffer = (char*)malloc(m_BufferSize * sizeof(line));
  
  CUstpFtdcDepthMarketDataField* tick;
  int i = 0;
  int pos = 0;
  
  for(i = 0; i < m_Cursor; i++){
    tick = &m_Buffer[i];
    if((DBL_MAX == tick->BidPrice1)
    ||(DBL_MAX == tick->AskPrice1)){
      continue;
    }
    snprintf(line, sizeof(line), "%s,%s,%s,%d,%f,%d,%f,%d,%f,%f\n",
      tick->InstrumentID,
      tick->TradingDay,
      tick->UpdateTime,
      tick->UpdateMillisec,
      tick->BidPrice1,
      tick->BidVolume1,
      tick->AskPrice1,
      tick->AskVolume1,
      tick->UpperLimitPrice,
      tick->LowerLimitPrice);

    len = strlen(line);
    memcpy(&buffer[pos], line, len);
    pos += len;
  }

  do{
    FILE* fp = fopen(m_Filename, "a+");
    if(!fp){
      break;
    }

    fwrite(buffer, pos, 1, fp);
    
    fclose(fp);
  }while(0);
  
  free(buffer);

  //Reset
  m_LastTm = time(NULL);
  m_Cursor = 0;

  return;
}

int CFemasMduserHandler::Check()
{
  // Check
  time_t now = time(NULL);
  if(m_Cursor >= m_BufferSize){
    return 1;
  }
  
  if((m_LastTm + m_Gap) > now){
    return 1;
  }
  
  return 0;
}


CFemasMduserHandler::CFemasMduserHandler()
{
  m_LastTm = time(NULL);
  m_BufferSize = 1000;
  m_Buffer = (CUstpFtdcDepthMarketDataField*)malloc(m_BufferSize * sizeof(CUstpFtdcDepthMarketDataField));
  
  m_Gap = 2 * 60;
  m_Cursor = 0;
}

CFemasMduserHandler::~CFemasMduserHandler()
{
  Flush();

  free(m_Buffer);
  m_Buffer = NULL;
}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CFemasMduserHandler::OnFrontConnected()
{
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  CUstpFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_OldPasswd);

  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
  return ;

}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CFemasMduserHandler::OnFrontDisconnected(int nReason)
{

}

///��¼������Ӧ
void CFemasMduserHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///�ǳ�������Ӧ
void CFemasMduserHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  m_Loop = 0;
}

///����Ӧ��
void CFemasMduserHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///��������Ӧ��
void CFemasMduserHandler::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///ȡ����������Ӧ��
void CFemasMduserHandler::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}

///�������֪ͨ
void CFemasMduserHandler::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData)
{
  memcpy(&m_Buffer[m_Cursor], pDepthMarketData, sizeof(CUstpFtdcDepthMarketDataField));
  m_Cursor++;

  if(Check()){
    Flush();
  }
  return ;
}

void CFemasMduserHandler::OnRspGetMarketTopic(CUstpFtdcRspMarketTopicField *pRspMarketTopic, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  if(pRspMarketTopic){
    CTP_LOG("pRspMarketTopic->TopicID=[%d]"
      "pRspMarketTopic->ExchangeID=[%s]\n"
      , pRspMarketTopic->TopicID
      , pRspMarketTopic->ExchangeID
      );
  }
  return;
}




