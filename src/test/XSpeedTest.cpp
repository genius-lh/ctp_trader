
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#include "XSpeedTest.h"

#define XSPEED_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sAddress[128];
  char sUserId[128];
  char sOldPassword[128];
  char sFilename[128];
  int bLoop = 1;
  int i = 1;

  if(argc < 5){
    printf("input address:\n");
    scanf("%s", sAddress);
    printf("input username:\n");
    scanf("%s", sUserId);
    printf("input password:\n");
    scanf("%s", sOldPassword);
    printf("input sFilename:\n");
    scanf("%s", sFilename);
  }else{
    strncpy(sAddress, argv[i++], sizeof(sAddress));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sOldPassword, argv[i++], sizeof(sOldPassword));
    strncpy(sFilename, argv[i++], sizeof(sFilename));
  }

  DFITCSECTraderApi* pTraderApi = DFITCSECTraderApi::CreateDFITCSECTraderApi();
  CXSpeedTraderHandler* pTraderHandler = new CXSpeedTraderHandler();
  // ��ʼ������
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_NewPasswd = sFilename;
  pTraderHandler->m_Loop = &bLoop;
  
  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
  pTraderApi->SubscribePrivateTopic(TERT_RESUME);
  
  // ���ӽ��׷�����
  pTraderApi->Init(sAddress, pTraderHandler);

  sleep(3);
  while(bLoop){
    pTraderHandler->Loop();
    sleep(1);
  }

  pTraderApi->Release();
  delete pTraderHandler;
  return 0;  
}


CXSpeedTraderHandler::CXSpeedTraderHandler()
{

}

CXSpeedTraderHandler::~CXSpeedTraderHandler()
{

}

/**
 * SEC-��������������Ӧ
 */
void CXSpeedTraderHandler::OnFrontConnected()
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSECReqUserLoginField data3;
  memset(&data3,0,sizeof(data3));
  strcpy(data3.accountID, m_UserId);
  strcpy(data3.password, m_OldPasswd);
  data3.requestID=m_RequestId++;
  pTraderApi->ReqSOPUserLogin(&data3);
  return ;
}

/**
 * SEC-�������Ӳ�������Ӧ
 */
void CXSpeedTraderHandler::OnFrontDisconnected(int nReason)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

}
/**
* ERR-����Ӧ��
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ַ
*/
void CXSpeedTraderHandler::OnRspError(DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

}

/**
* SOP-��¼��Ӧ
* @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
*/
void CXSpeedTraderHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

  if(!pData){
    if(pRspInfo){
      XSPEED_LOG("pRspInfo->errorID=[%ld], pRspInfo->errorMsg=[%s]\n",
        pRspInfo->errorID, pRspInfo->errorMsg);
    }
    *m_Loop = 0;
    return;
  }
    
  XSPEED_LOG("pData->localOrderID=[%ld]\n"
    "pData->loginTime=[%s]\n"
    "pData->tradingDay=[%d]\n"
    "pData->result=[%ld]\n"
    "pData->rtnMsg=[%s]\n",  
    pData->localOrderID,
    pData->loginTime,
    pData->tradingDay,
    pData->result,
    pData->rtnMsg);
  
  return ;
}

/**
 * SOP-�ǳ���Ӧ
 * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
 */
void CXSpeedTraderHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  *m_Loop = 0;
  return ;
}

/**
* SOP-��Ȩ��Լ�����ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û���Ȩ��Լ�����ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ��Լ�����ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ��Լ�����ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CXSpeedTraderHandler::OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pData){
    XSPEED_LOG("%s,%s,%s,%s,%lf\n",
      pData->exchangeID,
      pData->securityOptionID,
      pData->contractID,
      pData->contractName,
      pData->miniPriceChange);
  }

  if(!bIsLast){
    return ;
  }
  
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSECReqUserLogoutField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID=m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  
  pTraderApi->ReqSOPUserLogout(&data3);

  return ;
}

/**
* SOP-������Ӧ
* @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
*/
void CXSpeedTraderHandler::OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XSPEED_LOG("pRspInfo->requestID=[%ld]\n"
      "pRspInfo->sessionID=[%ld]\n"
      "pRspInfo->accountID=[%s]\n"
      "pRspInfo->errorID=[%ld]\n"
      "pRspInfo->localOrderID=[%ld]\n"
      "pRspInfo->spdOrderID=[%d]\n"
      "pRspInfo->errorMsg=[%s]\n",
      pRspInfo->requestID,
      pRspInfo->sessionID,
      pRspInfo->accountID,
      pRspInfo->errorID,
      pRspInfo->localOrderID,
      pRspInfo->spdOrderID,
      pRspInfo->errorMsg);
  }

  if(pData){
    XSPEED_LOG("pData->requestID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->localOrderID=[%ld]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustTime=[%s]\n"
      "pData->freezeFunds=[%lf]\n",
      pData->requestID,
      pData->accountID,
      pData->localOrderID,
      pData->spdOrderID,
      pData->entrustTime,
      pData->freezeFunds);

  }
  return ;
}

/**
* SOP-ִ��ί����Ӧ
* @param pData:ָ�����ǿ�,�����û�ִ��ί����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ִ��ί������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ִ��ί������ʧ��
*/
void CXSpeedTraderHandler::OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XSPEED_LOG("pRspInfo->requestID=[%ld]\n"
      "pRspInfo->sessionID=[%ld]\n"
      "pRspInfo->accountID=[%s]\n"
      "pRspInfo->errorID=[%ld]\n"
      "pRspInfo->localOrderID=[%ld]\n"
      "pRspInfo->spdOrderID=[%d]\n"
      "pRspInfo->errorMsg=[%s]\n",
      pRspInfo->requestID,
      pRspInfo->sessionID,
      pRspInfo->accountID,
      pRspInfo->errorID,
      pRspInfo->localOrderID,
      pRspInfo->spdOrderID,
      pRspInfo->errorMsg);
  }
  
  if(pData){
    XSPEED_LOG("pData->requestID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->localOrderID=[%ld]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustTime=[%s]\n"
      "pData->freezeFunds=[%lf]\n",
      pData->requestID,
      pData->accountID,
      pData->localOrderID,
      pData->spdOrderID,
      pData->entrustTime,
      pData->freezeFunds);
  }
  
  return ;

}

/**
* SOP-ί�лر���Ӧ
* @param pData:����ί�лر��ṹ��ĵ�ַ
*/
void CXSpeedTraderHandler::OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

  if(pData){
    XSPEED_LOG("pData->localOrderID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->sessionID=[%ld]\n"
      "pData->branchID=[%s]\n"
      "pData->shareholderID=[%s]\n"
      "pData->exchangeID=[%s]\n"
      "pData->securityID=[%s]\n"
      "pData->withdrawFlag=[%s]\n"
      "pData->currency=[%s]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustDirection=[%d]\n"
      "pData->openCloseFlag=[%d]\n"
      "pData->entrustPrice=[%lf]\n"
      "pData->entrustQty=[%d]\n"
      "pData->entrustTime=[%s]\n"
      "pData->coveredFlag=[%d]\n"
      "pData->orderType=[%d]\n"
      "pData->orderExpiryDate=[%d]\n"
      "pData->orderCategory=[%d]\n"
      "pData->declareResult=[%d]\n"
      "pData->noteMsg=[%s]\n",
      pData->localOrderID,
      pData->accountID,
      pData->sessionID,
      pData->branchID,
      pData->shareholderID,
      pData->exchangeID,
      pData->securityID,
      pData->withdrawFlag,
      pData->currency,
      pData->spdOrderID,
      pData->entrustDirection,
      pData->openCloseFlag,
      pData->entrustPrice,
      pData->entrustQty,
      pData->entrustTime,
      pData->coveredFlag,
      pData->orderType,
      pData->orderExpiryDate,
      pData->orderCategory,
      pData->declareResult,
      pData->noteMsg);
  }
  
  return ;
}

/**
* SOP-�ɽ��ر���Ӧ
* @param pData:���سɽ��ر��ṹ��ĵ�ַ
*/
void CXSpeedTraderHandler::OnSOPTradeRtn(DFITCSOPTradeRtnField * pData)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

  if(pData){
    XSPEED_LOG("pData->localOrderID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->sessionID=[%ld]\n"
      "pData->shareholderID=[%s]\n"
      "pData->exchangeID=[%s]\n"
      "pData->securityID=[%s]\n"
      "pData->withdrawFlag=[%s]\n"
      "pData->currency=[%s]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustDirection=[%d]\n"
      "pData->openCloseFlag=[%d]\n"
      "pData->coveredFlag=[%d]\n"
      "pData->orderCategory=[%d]\n"
      "pData->tradePrice=[%lf]\n"
      "pData->tradeQty=[%d]\n"
      "pData->tradeID=[%s]\n"
      "pData->rtnSerialID=[%d]\n"
      "pData->declareOrderID=[%s]\n"
      "pData->declareResult=[%d]\n"
      "pData->noteMsg=[%s]\n"
      "pData->tradeTime=[%s]\n"
      "pData->groupCode=[%s]\n",
      pData->localOrderID,
      pData->accountID,
      pData->sessionID,
      pData->shareholderID,
      pData->exchangeID,
      pData->securityID,
      pData->withdrawFlag,
      pData->currency,
      pData->spdOrderID,
      pData->entrustDirection,
      pData->openCloseFlag,
      pData->coveredFlag,
      pData->orderCategory,
      pData->tradePrice,
      pData->tradeQty,
      pData->tradeID,
      pData->rtnSerialID,
      pData->declareOrderID,
      pData->declareResult,
      pData->noteMsg,
      pData->tradeTime,
      pData->groupCode);
  }
  return ;

}

/**
* SOP-������Ӧ
* @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
*/
void CXSpeedTraderHandler::OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XSPEED_LOG("pRspInfo->requestID=[%ld]\n"
      "pRspInfo->sessionID=[%ld]\n"
      "pRspInfo->accountID=[%s]\n"
      "pRspInfo->errorID=[%ld]\n"
      "pRspInfo->localOrderID=[%ld]\n"
      "pRspInfo->spdOrderID=[%d]\n"
      "pRspInfo->errorMsg=[%s]\n",
      pRspInfo->requestID,
      pRspInfo->sessionID,
      pRspInfo->accountID,
      pRspInfo->errorID,
      pRspInfo->localOrderID,
      pRspInfo->spdOrderID,
      pRspInfo->errorMsg);
  }
  
  if(pData){
    XSPEED_LOG("pData->requestID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->localOrderID=[%ld]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustTime=[%s]\n"
      "pData->cancelMsg=[%s]\n",
      pData->requestID,
      pData->accountID,
      pData->localOrderID,
      pData->spdOrderID,
      pData->entrustTime,
      pData->cancelMsg);
  }
  
  return ;
}

/**
* SOP-ί�в�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,����ί�в�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ί�в�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CXSpeedTraderHandler::OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XSPEED_LOG("pRspInfo->requestID=[%ld]\n"
      "pRspInfo->sessionID=[%ld]\n"
      "pRspInfo->accountID=[%s]\n"
      "pRspInfo->errorID=[%ld]\n"
      "pRspInfo->localOrderID=[%ld]\n"
      "pRspInfo->spdOrderID=[%d]\n"
      "pRspInfo->errorMsg=[%s]\n",
      pRspInfo->requestID,
      pRspInfo->sessionID,
      pRspInfo->accountID,
      pRspInfo->errorID,
      pRspInfo->localOrderID,
      pRspInfo->spdOrderID,
      pRspInfo->errorMsg);
  }
  
  if(pData){    
    XSPEED_LOG("pData->requestID=[%ld]"
      "pData->accountID=[%s]"
      "pData->spdOrderID=[%d]"
      "pData->withdrawOrderID=[%s]"
      "pData->declareOrderID=[%s]"
      "pData->exchangeID=[%s]"
      "pData->shareholderID=[%s]"
      "pData->entrustDirection=[%d]"
      "pData->withdrawFlag=[%s]"
      "pData->securityOptionID=[%s]"
      "pData->optType=[%d]"
      "pData->openCloseFlag=[%d]"
      "pData->coveredFlag=[%d]"
      "pData->entrustQty=[%d]"
      "pData->entrustPrice=[%lf]"
      "pData->entrustDate=[%d]"
      "pData->entrustTime=[%s]"
      "pData->offerShareholderID=[%s]"
      "pData->declareDate=[%d]"
      "pData->declareTime=[%s]"
      "pData->declareSerialID=[%d]"
      "pData->declareResult=[%d]"
      "pData->noteMsg=[%s]"
      "pData->withdrawQty=[%d]"
      "pData->tradeQty=[%d]"
      "pData->turnover=[%lf]"
      "pData->tradePrice=[%lf]"
      "pData->currency=[%s]"
      "pData->freezeFunds=[%lf]"
      "pData->clearFunds=[%lf]"
      "pData->entrustType=[%d]"
      "pData->macAddr=[%s]"
      "pData->ipAddr=[%s]"
      "pData->entrustBatchID=[%d]"
      "pData->orderType=[%d]"
      "pData->incQryIndex=[%s]"
      "pData->capitalID=[%s]"
      "pData->orderExpiryDate=[%d]\n",
      pData->requestID,
      pData->accountID,
      pData->spdOrderID,
      pData->withdrawOrderID,
      pData->declareOrderID,
      pData->exchangeID,
      pData->shareholderID,
      pData->entrustDirection,
      pData->withdrawFlag,
      pData->securityOptionID,
      pData->optType,
      pData->openCloseFlag,
      pData->coveredFlag,
      pData->entrustQty,
      pData->entrustPrice,
      pData->entrustDate,
      pData->entrustTime,
      pData->offerShareholderID,
      pData->declareDate,
      pData->declareTime,
      pData->declareSerialID,
      pData->declareResult,
      pData->noteMsg,
      pData->withdrawQty,
      pData->tradeQty,
      pData->turnover,
      pData->tradePrice,
      pData->currency,
      pData->freezeFunds,
      pData->clearFunds,
      pData->entrustType,
      pData->macAddr,
      pData->ipAddr,
      pData->entrustBatchID,
      pData->orderType,
      pData->incQryIndex,
      pData->capitalID,
      pData->orderExpiryDate);

  }
  
  return ;

}

void CXSpeedTraderHandler::OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  if(pRspInfo){
    XSPEED_LOG("pRspInfo->requestID=[%ld]\n"
      "pRspInfo->sessionID=[%ld]\n"
      "pRspInfo->accountID=[%s]\n"
      "pRspInfo->errorID=[%ld]\n"
      "pRspInfo->localOrderID=[%ld]\n"
      "pRspInfo->spdOrderID=[%d]\n"
      "pRspInfo->errorMsg=[%s]\n",
      pRspInfo->requestID,
      pRspInfo->sessionID,
      pRspInfo->accountID,
      pRspInfo->errorID,
      pRspInfo->localOrderID,
      pRspInfo->spdOrderID,
      pRspInfo->errorMsg);
  }
  
  if(pData){    
    XSPEED_LOG("pData->requestID=[%ld]"
      "pData->accountID=[%s]"
      "pData->exchangeID=[%s]"
      "pData->securityOptionID=[%s]"
      "pData->contractID=[%s]"
      "pData->contractName=[%s]"
      "pData->entrustDirection=[%d]"
      "pData->coveredFlag=[%d]"
      "pData->executeDate=[%d]"
      "pData->totalQty=[%d]"
      "pData->availableQty=[%d]\n",
      pData->requestID,
      pData->accountID,
      pData->exchangeID,
      pData->securityOptionID,
      pData->contractID,
      pData->contractName,
      pData->entrustDirection,
      pData->coveredFlag,
      pData->executeDate,
      pData->totalQty,
      pData->availableQty);
  }
}


/**
* SOP-�����ر���Ӧ
* @param pData:���س����ر��ṹ��ĵ�ַ
*/
void CXSpeedTraderHandler::OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData)
{
  XSPEED_LOG("%s\n", __FUNCTION__);

  if(pData){
    XSPEED_LOG("pData->localOrderID=[%ld]\n"
      "pData->accountID=[%s]\n"
      "pData->sessionID=[%ld]\n"
      "pData->shareholderID=[%s]\n"
      "pData->exchangeID=[%s]\n"
      "pData->securityID=[%s]\n"
      "pData->withdrawFlag=[%s]\n"
      "pData->currency=[%s]\n"
      "pData->spdOrderID=[%d]\n"
      "pData->entrustDirection=[%d]\n"
      "pData->openCloseFlag=[%d]\n"
      "pData->withdrawQty=[%d]\n"
      "pData->tradeQty=[%d]\n"
      "pData->declareResult=[%d]\n"
      "pData->noteMsg=[%s]\n"
      "pData->wdUnFreezeFunds=[%lf]\n",
      pData->localOrderID,
      pData->accountID,
      pData->sessionID,
      pData->shareholderID,
      pData->exchangeID,
      pData->securityID,
      pData->withdrawFlag,
      pData->currency,
      pData->spdOrderID,
      pData->entrustDirection,
      pData->openCloseFlag,
      pData->withdrawQty,
      pData->tradeQty,
      pData->declareResult,
      pData->noteMsg,
      pData->wdUnFreezeFunds);
  }
  
  return ;
}

void CXSpeedTraderHandler::Loop()
{
  int choose = ShowMenu();
  switch(choose){
  case 1:
    QueryContract();
    break;
  case 2:
    CoveredOpen();
    break;
  case 3:
    CoveredClose();
    break;
  case 4:
    CallBuyOpen();
    break;
  case 5:
    CallSellClose();
    break;
  case 6:
    CallSellOpen();
    break;
  case 7:
    CallBuyClose();
    break;
  case 8:
    PutBuyOpen();
    break;
  case 9:
    PutSellClose();
    break;
  case 10:
    PutSellOpen();
    break;
  case 11:
    PutBuyClose();
    break;
  case 12:
    CallExecuteOrder();
    break;
  case 13:
    PutExecuteOrder();
    break;
  case 14:
    Query();
    break;
  case 15:
    Withdraw();
    break;
  case 16:
    QueryPosition();
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

int CXSpeedTraderHandler::ShowMenu()
{
  int ret = 0;
  printf("**********************\n"
        "ѡ����Ҫִ�еĲ���\n"
        "1-��Լ��ѯ\n"
        "2-���ҿ���\n"
        "3-����ƽ��\n"
        "4-�Ϲ����뿪��\n"
        "5-�Ϲ�����ƽ��\n"
        "6-�Ϲ���������\n"
        "7-�Ϲ�����ƽ��\n"
        "8-�Ϲ����뿪��\n"
        "9-�Ϲ�����ƽ��\n"
        "10-�Ϲ���������\n"
        "11-�Ϲ�����ƽ��\n"
        "12-�Ϲ���Ȩ\n"
        "13-�Ϲ���Ȩ\n"
        "14-������ѯ\n"
        "15-����\n"
        "16-��ѯ�ֲ�\n"
        "0-�˳�\n"
        "**********************\n"
        "��ѡ��"
  );
  scanf("%d",&ret);

  return ret;

}

void CXSpeedTraderHandler::QueryContract()
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSOPReqQryContactField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID=m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  strcpy(data3.exchangeID, "SZ");
  strcpy(data3.securityID, "159919");

  pTraderApi->ReqSOPQryContactInfo(&data3);

  return ;
}

void CXSpeedTraderHandler::InsertOrder(char* inst, int buy_sell, int open_close, double price, int vol, int covered)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  
  DFITCSOPReqEntrustOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
  
  //����ί�к�(Y)
	inputOrderField.localOrderID = m_RequestId;
  //����ID(Y)
  inputOrderField.requestID = m_RequestId++;
  //�ͻ���(Y)
	strcpy(inputOrderField.accountID, m_UserId);
  //����������(Y)
	memcpy(inputOrderField.exchangeID, inst, 2);
  //֤������(Y)
	strncpy(inputOrderField.securityID, &inst[2], sizeof(inputOrderField.securityID));
  //���˻�����(N)(Ԥ���ֶ�)
	strcpy(inputOrderField.subAccountID, "");
  //ί������(Y)
	inputOrderField.entrustQty = vol;
  //ί�м۸�(N)
	inputOrderField.entrustPrice = price;
  //ί�����(Y)
	  inputOrderField.entrustDirection = buy_sell;
  //��ƽ��־(Y)
  inputOrderField.openCloseFlag = open_close;
  //��������(Y)
	inputOrderField.coveredFlag = covered;
  //��������(Y)
	inputOrderField.orderType = DFITCSEC_SOP_LimitPrice;
  //����ʱЧ����(N)
  inputOrderField.orderExpiryDate = DFITCSEC_OE_NONE;
  //ί�е����(Y)
  inputOrderField.orderCategory = DFITCSEC_OC_GeneralOrder;

  pTraderApi->ReqSOPEntrustOrder(&inputOrderField);

  return ;
}

void CXSpeedTraderHandler::ExecuteOrder(char* inst, int direction, int open_close, int vol)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSOPReqExectueOrderField data3;
  memset(&data3, 0, sizeof(data3));
  strcpy(data3.accountID, m_UserId);
  data3.localOrderID = m_RequestId;
  memcpy(data3.exchangeID, inst, 2);
  strncpy(data3.securityOptionID, &inst[2], sizeof(data3.securityOptionID));
  data3.entrustQty = vol;
  data3.entrustDirection = direction;
  data3.openCloseFlag = open_close;
  data3.requestID = m_RequestId++;
  
  pTraderApi->ReqSOPExectueOrder(&data3);

  return ;
}

void CXSpeedTraderHandler::WithdrawOrder(int sys_order_id)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSECReqWithdrawOrderField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID = m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  data3.spdOrderID = sys_order_id;

  pTraderApi->ReqSOPWithdrawOrder(&data3);

  return ;
}

void CXSpeedTraderHandler::QueryOrder(int sys_order_id)
{
  XSPEED_LOG("%s\n", __FUNCTION__);
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSOPReqQryEntrustOrderField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID = m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  //data3.spdOrderID = sys_order_id;

  pTraderApi->ReqSOPQryEntrustOrder(&data3);

  return ;
}


void CXSpeedTraderHandler::CoveredOpen()
{
  XSPEED_LOG("2-���ҿ���\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Sell, DFITCSEC_OCF_Open, price, 1, DFITCSEC_CF_Covered);
  
}

void CXSpeedTraderHandler::CoveredClose()
{
  XSPEED_LOG("3-����ƽ��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Buy, DFITCSEC_OCF_Close, price, 1, DFITCSEC_CF_Covered);

}

void CXSpeedTraderHandler::CallBuyOpen()
{
  XSPEED_LOG("4-�Ϲ����뿪��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Buy, DFITCSEC_OCF_Open, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::CallSellClose()
{
  XSPEED_LOG("5-�Ϲ�����ƽ��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Sell, DFITCSEC_OCF_Close, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::CallSellOpen()
{
  XSPEED_LOG("6-�Ϲ���������\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Sell, DFITCSEC_OCF_Open, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::CallBuyClose()
{
  XSPEED_LOG("7-�Ϲ�����ƽ��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Buy, DFITCSEC_OCF_Close, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::PutBuyOpen()
{
  XSPEED_LOG("8-�Ϲ����뿪��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Buy, DFITCSEC_OCF_Open, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::PutSellClose()
{
  XSPEED_LOG("9-�Ϲ�����ƽ��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Sell, DFITCSEC_OCF_Close, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::PutSellOpen()
{
  XSPEED_LOG("10-�Ϲ���������\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Sell, DFITCSEC_OCF_Open, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::PutBuyClose()
{
  XSPEED_LOG("11-�Ϲ�����ƽ��\n");
  char instrument[31];
  double price;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������۸�:");
  scanf("%lf", &price);
  
  InsertOrder(instrument, DFITCSEC_ED_Buy, DFITCSEC_OCF_Close, price, 1, DFITCSEC_CF_UnCovered);
}

void CXSpeedTraderHandler::CallExecuteOrder()
{
  XSPEED_LOG("12-�Ϲ���Ȩ\n");
  char instrument[31];
  int director;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������ί�����:");
  scanf("%d", &director);
  ExecuteOrder(instrument, director, DFITCSEC_OCF_Execute, 1);
}

void CXSpeedTraderHandler::PutExecuteOrder()
{
  XSPEED_LOG("13-�Ϲ���Ȩ\n");
  char instrument[31];
  int director;
  printf("�������Լ:");
  scanf("%s", instrument);
  printf("������ί�����:");
  scanf("%d", &director);
  ExecuteOrder(instrument, director, DFITCSEC_OCF_Execute, 1);

}

void CXSpeedTraderHandler::LogOut()
{
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSECReqUserLogoutField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID=m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  
  pTraderApi->ReqSOPUserLogout(&data3);

  return ;
}

void CXSpeedTraderHandler::Query()
{
  XSPEED_LOG("14-������ѯ\n");
  int sysOrderId;
  printf("������ϵͳ������:");
  scanf("%d", &sysOrderId);
  QueryOrder(sysOrderId);
}

void CXSpeedTraderHandler::QueryPosition()
{
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSOPReqQryPositionField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID=m_RequestId++;
  strcpy(data3.accountID, m_UserId);
  strcpy(data3.exchangeID, "SZ");

  pTraderApi->ReqSOPQryPosition(&data3);

  return ;
}

void CXSpeedTraderHandler::Withdraw()
{
  XSPEED_LOG("15-����\n");
  int sysOrderId;
  printf("������ϵͳ������:");
  scanf("%d", &sysOrderId);
  WithdrawOrder(sysOrderId);
  
}



