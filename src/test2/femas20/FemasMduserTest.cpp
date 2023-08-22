
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
  // 初始化变量
  // 交易
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

  // 连接交易服务器
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
        "选择需要执行的操作\n"
        "1-订阅合约\n"
        "2-取消订阅合约\n"
        "3-查询期货交易所\n"
        "0-退出\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CFemasMduserHandler::LogOut()
{
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  struct CUstpFtdcReqUserLogoutField data3;
  memset(&data3, 0, sizeof(data3));
	///经纪公司代码
	strncpy(data3.BrokerID, m_BrokerID, sizeof(data3.BrokerID));
	///投资者代码
	strncpy(data3.UserID, m_UserId, sizeof(data3.UserID));

  pTraderApi->ReqUserLogout(&data3, m_RequestId++);
  
  m_Loop = 0;

  return ;
}

void CFemasMduserHandler::SubMarketData()
{
  CTP_LOG("1 - 订阅合约\n");
  char instrument[31];
  printf("请输入合约:");
  scanf("%s", instrument);

  char* contracts[] = {instrument};
  
  CUstpFtdcMduserApi* pTraderApi = (CUstpFtdcMduserApi*)m_Arg;
  pTraderApi->SubMarketData(contracts, 1);

  return ;
}

void CFemasMduserHandler::UnSubMarketData()
{
  CTP_LOG("2 - 取消订阅合约\n");
  char instrument[31];
  printf("请输入合约:");
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
  printf("请输入ExchangeID:");
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

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
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

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CFemasMduserHandler::OnFrontDisconnected(int nReason)
{

}

///登录请求响应
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

///登出请求响应
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

///错误应答
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

///订阅行情应答
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

///取消订阅行情应答
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

///深度行情通知
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




