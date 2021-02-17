
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>


#include "RemMduserTest.h"

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

//using namespace ctp_sopt;

#define CTP_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char remoteTradeIp[128];
  char remoteTradeTCPPort[128];
  char sUserId[128];
  char sOldPassword[128];
  char sFilename[128];
  int i = 1;

  if(argc < 6){
    printf("input remoteTradeIp:\n");
    scanf("%s", remoteTradeIp);
    printf("input remoteTradeTCPPort:\n");
    scanf("%s", remoteTradeTCPPort);
    printf("input username:\n");
    scanf("%s", sUserId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(remoteTradeIp, argv[i++], sizeof(remoteTradeIp));
    strncpy(remoteTradeTCPPort, argv[i++], sizeof(remoteTradeTCPPort));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  EESQuoteApi* pTraderApi = CreateEESQuoteApi();
  CRemMduserTestHandler* pTraderHandler = new CRemMduserTestHandler();
  // ��ʼ������
  EqsTcpInfo svrInfo;
	strncpy(svrInfo.m_eqsIp, remoteTradeIp, sizeof(svrInfo.m_eqsIp));
	svrInfo.m_eqsPort = atoi(remoteTradeTCPPort);

  // ����
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_Filename = sFilename;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;

  // ���ӽ��׷�����
  pTraderApi->ConnServer(svrInfo, pTraderHandler);

  pTraderHandler->Loop();

  pTraderApi->DisConnServer();

  DestroyEESQuoteApi(pTraderApi);
  delete pTraderHandler;
  return 0;  
}

void CRemMduserTestHandler::Loop()
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
    case 0:
      m_Loop = 0;
      sleep(1);
      break;
    default:
      break;
    }
  }while(m_Loop);
  return ;

}

int CRemMduserTestHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "ѡ����Ҫִ�еĲ���\n"
        "1-���ĺ�Լ\n"
        "2-ȡ�����ĺ�Լ\n"
        "0-�˳�\n"
        "**********************\n"
        "��ѡ��"
  );
  scanf("%d",&ret);

  return ret;

}

void CRemMduserTestHandler::SubMarketData()
{
  CTP_LOG("1 - ���ĺ�Լ\n");
  char instrument[31];
  printf("�������Լ:");
  scanf("%s", instrument);

  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;
  pTraderApi->RegisterSymbol(EQS_FUTURE, instrument);

  return ;
}

void CRemMduserTestHandler::UnSubMarketData()
{
  CTP_LOG("2 - ȡ�����ĺ�Լ\n");
  char instrument[31];
  printf("�������Լ:");
  scanf("%s", instrument);
    
  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;
  pTraderApi->UnregisterSymbol(EQS_FUTURE, instrument);

  return ;
}

void CRemMduserTestHandler::Flush()
{
  char line[1000];
  int len;
  if(!m_Cursor){
    return ;
  }

  char* buffer = (char*)malloc(m_BufferSize * sizeof(line));
  
  EESMarketDepthQuoteData* tick;
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

int CRemMduserTestHandler::Check()
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


CRemMduserTestHandler::CRemMduserTestHandler()
{
  m_LastTm = time(NULL);
  m_BufferSize = 1000;
  m_Buffer = (EESMarketDepthQuoteData*)malloc(m_BufferSize * sizeof(EESMarketDepthQuoteData));
  
  m_Gap = 2 * 60;
  m_Cursor = 0;
}

CRemMduserTestHandler::~CRemMduserTestHandler()
{
  Flush();

  free(m_Buffer);
  m_Buffer = NULL;
}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CRemMduserTestHandler::OnEqsConnected()
{
  EESQuoteApi* pTraderApi = (EESQuoteApi*)m_Arg;
  EqsLoginParam loginParam;
  strncpy(loginParam.m_loginId, m_UserId, sizeof(loginParam.m_loginId));
  strncpy(loginParam.m_password, m_OldPasswd, sizeof(loginParam.m_password));

  pTraderApi->LoginToEqs(loginParam);
  return ;

}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CRemMduserTestHandler::OnEqsDisconnected()
{

}

///��¼������Ӧ
void CRemMduserTestHandler::OnLoginResponse(bool bSuccess, const char* pReason)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CTP_LOG("bSuccess=[%d]"
    "pReason=[%s]\n",
    bSuccess,
    pReason);

}

///��������Ӧ��
void CRemMduserTestHandler::OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  CTP_LOG("%s\n", __FUNCTION__);
  
  CTP_LOG("chInstrumentType=[%d]"
    "pSymbol=[%s]"
    "bSuccess=[%d]"
    "\n"
    , chInstrumentType
    , pSymbol
    , bSuccess);
}

///ȡ����������Ӧ��
void CRemMduserTestHandler::OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  CTP_LOG("%s\n", __FUNCTION__);
  CTP_LOG("chInstrumentType=[%d]"
    "pSymbol=[%s]"
    "bSuccess=[%d]"
    "\n"
    , chInstrumentType
    , pSymbol
    , bSuccess);
}

///�������֪ͨ
void CRemMduserTestHandler::OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData)
{
  memcpy(&m_Buffer[m_Cursor], pDepthQuoteData, sizeof(EESMarketDepthQuoteData));
  m_Cursor++;

  if(Check()){
    Flush();
  }
  return ;
}



