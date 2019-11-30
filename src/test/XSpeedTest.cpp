
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#include "XSpeedTest.h"

#define XSPEED_LOG(...) printf(__VA_ARGS__)


int main(int argc, char* argv[])
{
  char sAddress[128];
  char sAppId[128];
  char sAuthCode[128];
  char sBrokerID[128];
  char sUserId[128];
  char sOldPassword[128];
  int bLoop = 1;

  strncpy(sAddress, "", sizeof(sAddress));
  strncpy(sAppId, "", sizeof(sAppId));
  strncpy(sAuthCode, "", sizeof(sAuthCode));
  strncpy(sBrokerID, "0177", sizeof(sBrokerID));

  printf("input username:\n");
  scanf("%s", sUserId);
  printf("input password:\n");
  scanf("%s", sOldPassword);

  DFITCSECTraderApi* pTraderApi = DFITCSECTraderApi::CreateDFITCSECTraderApi();
  CXSpeedTraderHandler* pTraderHandler = new CXSpeedTraderHandler();
  // 初始化变量
  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_BrokerID = sBrokerID;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_OldPasswd = sOldPassword;
  pTraderHandler->m_Loop = &bLoop;
  
  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;
  
  pTraderApi->SubscribePrivateTopic(USTP_TERT_RESUME);
  
  // 连接交易服务器
  pTraderApi->Init(sAddress, pTraderHandler);

  while(bLoop){
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
 * SEC-网络连接正常响应
 */
void CXSpeedTraderHandler::OnFrontConnected()
{
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  struct DFITCSECReqUserLoginField data3;
  memset(&data3,0,sizeof(data3));
  strcpy(data3.accountID, m_UserId);
  strcpy(data3.password, m_OldPasswd);
  data3.requestID=m_RequestId++
  pTraderApi->ReqSOPUserLogin(&data3);
  return ;
}

/**
 * SEC-网络连接不正常响应
 */
void CXSpeedTraderHandler::OnFrontDisconnected(int nReason)
{

}
/**
* ERR-错误应答
* @param pRspInfo:指针若非空，返回错误信息结构地址
*/
void CXSpeedTraderHandler::OnRspError(DFITCSECRspInfoField *pRspInfo)
{

}

/**
* SOP-登录响应
* @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
*/
void CXSpeedTraderHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo)
{
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)m_Arg;
  if(!pData){
    if(pRspInfo){
      XSPEED_LOG("pRspInfo->errorID=[%d], pRspInfo->errorMsg=[%s]\n",
        pRspInfo->errorID, pRspInfo->errorMsg);
    }
    return -1;
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
    pData->rtnMsg
  );

  

  struct DFITCSOPReqQryContactField data3;
  memset(&data3, 0, sizeof(data3));
  data3.requestID=m_RequestId++
  strcpy(data3.accountID, m_UserId);

  pTraderApi->ReqSOPQryContactInfo(&data3);
  
  return 0;
}

/**
 * SOP-登出响应
 * @param pData:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
 * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
 */
void CXSpeedTraderHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo)
{
  *m_Loop = 0;
  return ;
}

/**
* SOP-期权合约代码查询响应
* @param pData:指针若非空,返回用户期权合约代码查询响应信息结构地址,表明客户期权合约代码查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权合约代码查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast)
{
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
  data3.requestID=m_RequestId++
  strcpy(data3.accountID, m_UserId);
  
  pTraderApi->ReqSOPUserLogout(&data3);

  return ;
}



