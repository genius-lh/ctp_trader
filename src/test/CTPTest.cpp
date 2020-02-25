
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"

#include "CTPTest.h"

#define CTP_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
  int bLoop = 1;
  int i = 1;

  if(argc < 7){
    printf("input username:\n");
    scanf("%s", sUserId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sAddress:\n");
    scanf("%s", sAddress);
    printf("input sAppId:\n");
    scanf("%s", sAppId);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
    printf("input sBrokerID:\n");
    scanf("%s", sBrokerID);
  }else{
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
    strncpy(sBrokerID, argv[i++], sizeof(sBrokerID));
  }
  
  CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".");
  CCtpTraderHandler* pTraderHandler = new CCtpTraderHandler();
  
  // ����
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
  pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
  pTraderApi->RegisterFront(sAddress);

  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_Loop = &bLoop;

  pTraderHandler->m_NewPasswd = "instrument.txt";
  
  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
  FILE* fp;
  fp = fopen(pTraderHandler->m_NewPasswd, "w+");
  if(NULL != fp){
    fclose(fp);
  }

  // ���ӽ��׷�����
  pTraderApi->Init();

  sleep(2);
  while(bLoop){
    pTraderHandler->Loop();
    sleep(1);
  }

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}


CCtpTraderHandler::CCtpTraderHandler()
{

}

CCtpTraderHandler::~CCtpTraderHandler()
{

}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CCtpTraderHandler::OnFrontConnected()
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(0 == (*m_Loop)){
    return ;
  }
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcReqAuthenticateField reqAuthenticateField;
  memset(&reqAuthenticateField, 0, sizeof(reqAuthenticateField));
  strcpy(reqAuthenticateField.BrokerID, m_BrokerID);
  strcpy(reqAuthenticateField.UserID, m_UserId);
  strcpy(reqAuthenticateField.AppID, m_AppID);
  strcpy(reqAuthenticateField.AuthCode, m_AuthCode);
  pTraderApi->ReqAuthenticate(&reqAuthenticateField, m_RequestId++);
  return ;

}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CCtpTraderHandler::OnFrontDisconnected(int nReason)
{
  CTP_LOG("%s\n", __FUNCTION__);
}

///�ͻ�����֤��Ӧ
void CCtpTraderHandler::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_OldPasswd);

  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);

}

///��¼������Ӧ
void CCtpTraderHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }

}

///�ǳ�������Ӧ
void CCtpTraderHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  *m_Loop = 0;
}

///�����ѯ��Լ��Ӧ
void CCtpTraderHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  if(!pInstrument){
    return ;
  }

  FILE* fp;
  fp = fopen(m_NewPasswd, "a+");
  if(NULL == fp){
    return ;
  }

  fprintf(fp, "%s,%s,%s,%lf\n",
    pInstrument->InstrumentID,
    pInstrument->ExchangeID,
    pInstrument->ProductID,
    pInstrument->PriceTick);
 
  fclose(fp);

  return;
}

void CCtpTraderHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  CTP_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CTP_LOG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
}


void CCtpTraderHandler::Loop()
{
  int choose = ShowMenu();
  switch(choose){
  case 1:
    QueryContract();
    break;
  case 0:
    LogOut();
    sleep(1);
    break;
  default:
    break;
  }
  return ;
}

int CCtpTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "ѡ����Ҫִ�еĲ���\n"
        "1-��Լ��ѯ\n"
        "0-�˳�\n"
        "**********************\n"
        "��ѡ��"
  );
  scanf("%d",&ret);

  return ret;

}

void CCtpTraderHandler::QueryContract()
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcQryInstrumentField qryInstrumentField;
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));

  pTraderApi->ReqQryInstrument(&qryInstrumentField, m_RequestId++);

  return ;
}

void CCtpTraderHandler::LogOut()
{
  CTP_LOG("%s\n", __FUNCTION__);
  CThostFtdcTraderApi* pTraderApi = (CThostFtdcTraderApi*)m_Arg;
  CThostFtdcUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, m_BrokerID);
  strcpy(userLogoutField.UserID, m_UserId);
  pTraderApi->ReqUserLogout(&userLogoutField, m_RequestId++);
  
  return ;
}




