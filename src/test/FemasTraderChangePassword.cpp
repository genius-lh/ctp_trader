#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcTraderApi.h"

#include "FemasTraderChangePassword.h"

#define FEMAS_LOG(...) printf(__VA_ARGS__)

CFemasTraderChangePassword::CFemasTraderChangePassword()
{

}

CFemasTraderChangePassword::~CFemasTraderChangePassword()
{

}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CFemasTraderChangePassword::OnFrontConnected()
{
  FEMAS_LOG("OnFrontConnected\n");
  sleep(1);
  
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcDSUserInfoField reqDSUserInfoField;
  
  memset(&reqDSUserInfoField, 0, sizeof(reqDSUserInfoField));
  strncpy(reqDSUserInfoField.AppID, m_AppID, sizeof(reqDSUserInfoField.AppID));
  strncpy(reqDSUserInfoField.AuthCode, m_AuthCode, sizeof(reqDSUserInfoField.AuthCode));
  reqDSUserInfoField.EncryptType = '1';

  FEMAS_LOG("reqDSUserInfoField.AppID[%s]\n", reqDSUserInfoField.AppID);
  FEMAS_LOG("reqDSUserInfoField.AuthCode[%s]\n", reqDSUserInfoField.AuthCode);

  pTraderApi->ReqDSUserCertification(&reqDSUserInfoField, m_RequestId++);
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
void CFemasTraderChangePassword::OnFrontDisconnected(int nReason)
{
}

///����Ӧ��
void CFemasTraderChangePassword::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

}

///���ǰ��ϵͳ�û���¼Ӧ��
void CFemasTraderChangePassword::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  FEMAS_LOG("OnRspUserLogin ErrorID[%d]\n", pRspInfo->ErrorID);
  if(pRspInfo->ErrorID){
    *m_Loop = 0;
    return ;
  }
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;

  CUstpFtdcUserPasswordUpdateField reqUserPasswordUpdateField;
  memset(&reqUserPasswordUpdateField, 0, sizeof(reqUserPasswordUpdateField));
  strcpy(reqUserPasswordUpdateField.BrokerID, m_BrokerID);
  strcpy(reqUserPasswordUpdateField.UserID, m_UserId);
  strcpy(reqUserPasswordUpdateField.OldPassword, m_OldPasswd);
  strcpy(reqUserPasswordUpdateField.NewPassword, m_NewPasswd);

  pTraderApi->ReqUserPasswordUpdate(&reqUserPasswordUpdateField, m_RequestId++);

}

///�û��˳�Ӧ��
void CFemasTraderChangePassword::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS_LOG("OnRspUserLogout ErrorID[%d]\n", pRspInfo->ErrorID);
  *m_Loop = 0;
}

///��͸��ܿͻ���֤Ӧ��
void CFemasTraderChangePassword::OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  FEMAS_LOG("OnRspDSUserCertification ErrorID[%d]\n", pRspInfo->ErrorID);
  if(pRspInfo->ErrorID){
    *m_Loop = 0;
    return ;
  }
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, m_BrokerID);
  strcpy(reqUserLoginField.UserID, m_UserId);
  strcpy(reqUserLoginField.Password, m_OldPasswd);
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, m_RequestId++);
}

///�û������޸�Ӧ��
void CFemasTraderChangePassword::OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  if(!pRspInfo->ErrorID){
    FEMAS_LOG("Password is updated!\n");

  }else{
    FEMAS_LOG("ErrorID[%d]ErrorMsg[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
  }
  
  CUstpFtdcTraderApi* pTraderApi = (CUstpFtdcTraderApi*)m_Arg;
  CUstpFtdcReqUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, m_BrokerID);
  strcpy(userLogoutField.UserID, m_UserId);
  pTraderApi->ReqUserLogout(&userLogoutField, m_RequestId++);
}


int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
  char sNewPassword[128];
  char sNewPassword2[128];
  int bLoop = 1;

  strncpy(sAddress, "", sizeof(sAddress));
  strncpy(sAppId, "", sizeof(sAppId));
  strncpy(sAuthCode, "", sizeof(sAuthCode));
  strncpy(sBrokerID, "0177", sizeof(sBrokerID));

  printf("input username:\n");
  scanf("%s", sUserId);
  printf("input password:\n");
  scanf("%s", sOldPassword);
  printf("input new password:\n");
  scanf("%s", sNewPassword);
  printf("input new password again:\n");
  scanf("%s", sNewPassword2);

  if(strncmp(sNewPassword, sNewPassword2, sizeof(sNewPassword2))){
    FEMAS_LOG("Passwords not equal!\n");
    return -1;
  }

  FEMAS_LOG("%s %s %s\n", sUserId, sOldPassword, sNewPassword);
  
  CUstpFtdcTraderApi* pTraderApi = CUstpFtdcTraderApi::CreateFtdcTraderApi();
  CFemasTraderChangePassword* pTraderHandler = new CFemasTraderChangePassword();
  // ��ʼ������
  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_NewPasswd = sNewPassword;
  pTraderHandler->m_Loop = &bLoop;
  
  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);

  pTraderApi->SubscribePublicTopic(USTP_TERT_RESUME);
  
  // ����
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(sAddress);
  
  // ���ӽ��׷�����
  pTraderApi->Init();

  while(bLoop){
    sleep(1);
  }

  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}

