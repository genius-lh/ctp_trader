
#include <string>
#include <thread>

#include <pthread.h>

#include "CXeleFtdcUserApiStruct.h"
#include "CXeleTraderApi.hpp"

#include <common.h>
#include <fair.h>
#include <CFairMsgRecv.h>

#include "XeleTraderOrderApi.h"
#include "XeleTraderOrderApiImp.h"

typedef char InsertTypeTableDefine[4];

CXeleTraderOrderApi* CXeleTraderOrderApi::CreateTraderApi()
{
  CXeleTraderOrderApi* instance = new CXeleTraderOrderApiImp();

  return instance;
}

TXeleFtdcInsertType CXeleTraderOrderApi::ConvertInsertType(char cTimeCondition, char cCombOffsetFlag,
  char cDirection, char cVolumeCondition)
{
/*
#define XELE_FTDC_OF_Open '0'
#define XELE_FTDC_OF_Close '1'
#define XELE_FTDC_OF_ForceClose '2'
#define XELE_FTDC_OF_CloseToday '3'
#define XELE_FTDC_OF_CloseYesterday '4'
#define XELE_FTDC_TC_IOC '1'
#define XELE_FTDC_TC_GFD '3'
#define XELE_FTDC_D_Buy '0'
#define XELE_FTDC_D_Sell '1'
#define XELE_FTDC_VC_AV '1'
#define XELE_FTDC_VC_MV '2'
#define XELE_FTDC_VC_CV '3'
*/
  const InsertTypeTableDefine insertTypeTable[] = {
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_Open, XELE_FTDC_D_Buy, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_Open, XELE_FTDC_D_Sell, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_Close, XELE_FTDC_D_Buy, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_Close, XELE_FTDC_D_Sell, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_CloseToday, XELE_FTDC_D_Buy, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_CloseToday, XELE_FTDC_D_Sell, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_CloseYesterday, XELE_FTDC_D_Buy, XELE_FTDC_VC_AV},
      {XELE_FTDC_TC_GFD, XELE_FTDC_OF_CloseYesterday, XELE_FTDC_D_Sell, XELE_FTDC_VC_AV}
  };

  int size = (int)(sizeof(insertTypeTable) / sizeof(InsertTypeTableDefine));
  
  for(int i = 0; i < size; i++) {
    if((cTimeCondition == insertTypeTable[i][0])
    &&(cCombOffsetFlag == insertTypeTable[i][1])
    &&(cDirection == insertTypeTable[i][2])
    &&(cVolumeCondition == insertTypeTable[i][3])){
      return i+1;
    }
  }

  return 0;
}

CXeleTraderOrderApiImp::CXeleTraderOrderApiImp()
{

}

///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CXeleTraderOrderApiImp::Release()
{
  m_threadLoop = false;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  Join();
  Disconnect();
  
}

///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CXeleTraderOrderApiImp::Init(TXeleFtdcClientIndexType* pClientIndex, TXeleFtdcClientTokenType* pToken)
{
  m_threadLoop = true;
  this->m_clientIndex[0] = pClientIndex[0];
  this->m_clientIndex[1] = pClientIndex[1];
  this->m_clientToken[0] = pToken[0];
  this->m_clientToken[1] = pToken[1];

  bool nRet = CreateUpConnection(this->m_strUpURL);
  if(!nRet){
    //TODO
    exit(-1);
  }
  
  pthread_create(&m_hReadThread, NULL, ThreadNakedProtocol, this);
}

///等待接口线程结束运行
///@return 线程退出代码
int CXeleTraderOrderApiImp::Join()
{
  void* ret;
  if(m_hReadThread){
    pthread_join(m_hReadThread, &ret);
  }
  return 0;
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CXeleTraderOrderApiImp::RegisterFront(char *pszUdpFrontAddress, char *pszTcpQueryFrontAddress)
{
  this->m_strUpURL = pszUdpFrontAddress;
  this->m_strDownURL = pszTcpQueryFrontAddress;
}

///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CXeleTraderOrderApiImp::RegisterSpi(CXeleTraderSpi *pSpi)
{
  this->m_originalTraderSpi = pSpi;
}

///用户登录请求
int CXeleTraderOrderApiImp::ReqUserLogin(int nRequestID)
{
  
  faircpp::FairMessageAssemble encoder;
  int length;
  const void *msg = NULL;
  encoder.ResetC2V();
  
  //Try to send order
  encoder.SetFairHeaderMessageIdC2V(MID_REQ_USER_LOGIN);
  encoder.SetFairHeaderClientIndexC2V(m_clientIndex[0]); //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderTokenC2V(m_clientToken[0]);// //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderSeqNoC2V(m_down_seqno++);//Strictly increase continously.
  encoder.SetFairHeaderReqIdC2V(nRequestID);
  
  msg = encoder.CreateMsgC2V(length);
  return SendReq(msg, length);

}

///用户退出请求
int CXeleTraderOrderApiImp::ReqUserLogout(CXeleFtdcReqUserLogoutField *pReqUserLogout,
                          int nRequestID)
{
  faircpp::FairMessageAssemble encoder;
  int length;
  const void *msg = NULL;
  encoder.ResetC2V();
  
  //Try to send order
  encoder.SetFairHeaderMessageIdC2V(MID_REQ_USER_LOGOUT);
  encoder.SetFairHeaderClientIndexC2V(m_clientIndex[0]); //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderTokenC2V(m_clientToken[0]);// //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderSeqNoC2V(m_down_seqno++);//Strictly increase continously.
  encoder.SetFairHeaderReqIdC2V(nRequestID);
  
  msg = encoder.CreateMsgC2V(length);
  return SendReq(msg, length);
}

///报单录入请求
int CXeleTraderOrderApiImp::ReqOrderInsert(CXeleFairInputOrderField *pInputOrder,
                           int nRequestID)
{
  faircpp::FairMessageAssemble encoder;
  int length = 0;
  const void *msg = NULL;
  encoder.ResetC2V();
  
  //Try to send order
  encoder.SetFairHeaderMessageIdC2V(MID_REQ_ORDER_INSERT);
  encoder.SetFairHeaderClientIndexC2V(m_clientIndex[0]); //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderTokenC2V(m_clientToken[0]);// //Use the corresponding value of [shfe, ine]
  encoder.SetFairHeaderSeqNoC2V(m_down_seqno++);//Strictly increase continously.
  encoder.SetFairHeaderReqIdC2V(nRequestID);
  
  CXeleFairInputOrderFieldRaw field;
  memset(&field, 0, sizeof(CXeleFairInputOrderFieldRaw));
  field.OrderLocalNo = pInputOrder->OrderLocalNo; //We incease the ID
  field.LimitPrice = pInputOrder->LimitPrice; //ValidPrice: [316, 516]
  strcpy(field.InstrumentID, pInputOrder->InstrumentID);
  field.VolumeTotalOriginal = pInputOrder->VolumeTotalOriginal;
  field.InsertType = pInputOrder->InsertType;
  field.MinVolume = pInputOrder->MinVolume;
  field.ExchangeFront = 0;
  encoder.AddFieldC2V(sizeof(field), &field);
  
  msg = encoder.CreateMsgC2V(length);
  
  return this->SendOrder((void*)msg, length);
}

///报单操作请求
int CXeleTraderOrderApiImp::ReqOrderAction(CXeleFairOrderActionField *pOrderAction,
                           int nRequestID)
{
  faircpp::FairMessageAssemble encoder;
  int length;
  const void *msg = NULL;
  encoder.ResetC2V();
  
  //
  encoder.SetFairHeaderMessageIdC2V(MID_REQ_ORDER_ACTION);
  encoder.SetFairHeaderClientIndexC2V(m_clientIndex[0]);
  encoder.SetFairHeaderTokenC2V(m_clientToken[0]);
  encoder.SetFairHeaderSeqNoC2V(m_down_seqno++);
  encoder.SetFairHeaderReqIdC2V(nRequestID);
  
  //
  CXeleFairOrderActionFieldRaw field;
  memset(&field, 0, sizeof(CXeleFairOrderActionFieldRaw));
  field.ActionLocalNo = pOrderAction->ActionLocalNo; //fill with random value or keep empty.
  field.OrderSysNo = pOrderAction->OrderSysNo;
  //field.OrderSysNo = 66;
  encoder.AddFieldC2V(sizeof(field), &field);
  //
  msg = encoder.CreateMsgC2V(length);
  return this->SendOrder((void*)msg, length);
}

void* CXeleTraderOrderApiImp::ThreadNakedProtocol(void* arg)
{
  CXeleTraderOrderApiImp* self = (CXeleTraderOrderApiImp*)arg;

  while(self->m_threadLoop){
    self->ProcessMessage();
  }

  pthread_exit((void*)NULL);
}

void CXeleTraderOrderApiImp::ProcessMessage()
{
  bool subLoop = true;
  int nRet = 0;
  
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

	tval.tv_usec = 100 * 1000; //100 ms
	tval.tv_sec = 0;

  m_down_seqno = 0;
  subLoop = Connect();
  if(!subLoop){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return ;
  }
  this->m_originalTraderSpi->OnFrontConnected();

  while(subLoop){
		FD_ZERO( &readSet);
		FD_ZERO( &writeSet);
		FD_ZERO( &errorSet);
    
		FD_SET(m_down_sock, &readSet);
		FD_SET(m_down_sock, &errorSet);

    nRet = select(m_down_sock + 1, &readSet, &writeSet, &errorSet, &tval);
		if( nRet == 0) { // timeout
		  if(this->m_threadLoop){
			  continue;
      }else{
        break;
      }
		}

		if( nRet < 0) {	// error
			break;
		}
    
		if( FD_ISSET( m_down_sock, &errorSet)) {
			// error on socket
			break;
		}

    
		if( FD_ISSET( m_down_sock, &readSet)) {
			// socket is ready for reading
			subLoop = OnReceive();
      continue;
		}

  }

  Disconnect();

}

bool CXeleTraderOrderApiImp::Connect()
{
  bool nRet = CreateDownConnection(this->m_strDownURL);
  if(nRet){
    this->m_originalTraderSpi->OnFrontConnected();
  }
  
  return nRet;
}

void CXeleTraderOrderApiImp::Disconnect()
{
  if(m_down_sock > 0){
    close(m_down_sock);
    m_down_sock = -1;
    this->m_originalTraderSpi->OnFrontDisconnected(0);
  }
}

bool CXeleTraderOrderApiImp::OnReceive()
{
  int n = 0;
  unsigned char type;
  unsigned short extLen;
  unsigned short contentLen;
  int curMsgLen;

  int nPos = 0;
  int nRest = 0;
  char* pData;
  
  n = read(m_down_sock, &m_Cache[m_CachePos], sizeof(m_Cache) - m_CachePos);
  if(0 == n){
    return false;
  }
  
  m_CachePos += n;

  while(nPos < m_CachePos){
    pData = &m_Cache[nPos];
    nRest = m_CachePos - nPos;

    // 判断报文头
    if(nRest < 4){
      break;
    }
    
    // 计算报文长度
    type = *pData;
    extLen = *(pData + 1);
    if (type == 0 && extLen == 2) //Heartbeat packet: 000200000500
    {
        contentLen = 0;
    }
    else
    {
        contentLen = *(unsigned short*)(pData + 2);
    }
    contentLen = *(unsigned short*)(pData + 2);
    curMsgLen = 4 + extLen + contentLen;
    if (nRest < curMsgLen){
      break;
    }

    // 处理报文
    do{
      faircpp::FairMessageParse msg;
      msg.ParseMsg(pData, curMsgLen); //output the message to process.
      NakedProtoRspFunc(&msg);
    }while(0);
    
    nPos += curMsgLen;
  }

  // 移动缓存
  nRest = m_CachePos - nPos;
  if(nRest > 0){
    memmove(m_Cache, &m_Cache[nPos], nRest);
  }
  m_CachePos = nRest;
  
  return true;
}

bool CXeleTraderOrderApiImp::CreateUpConnection(std::string strURL)
{
  string strProto, strIp, strPort;
  ParseURL(strURL, strProto, strIp, strPort);
  int ret = CreateUdpConnection(strProto, strIp, strPort, m_ServerAddr);
  if (ret <= 0)
  {
      return false;
  }
  m_up_sock = ret;
  return true;
}

bool CXeleTraderOrderApiImp::CreateDownConnection(std::string strURL)
{
  string strProto, strIp, strPort;
  ParseURL(strURL, strProto, strIp,strPort);
  int ret = CreateTcpConnection(strProto, strIp, strPort);
  do{
    if (ret <= 0) {
      break;
    }
    
    int flags = fcntl(ret, F_GETFL);
    if (flags == -1){
      break;
    }
    
    // set non-blocking mode
    if ( fcntl(ret, F_SETFL, flags | O_NONBLOCK) < 0){
      break;
    }

    m_down_sock = ret;
    return true;
  }while(0);
  
  if(ret > 0){
    close(ret);
  }
  return false;
}

int CXeleTraderOrderApiImp::SendReq(const void* data, int len)
{
  int ret = send(m_down_sock, (unsigned char*)data, len, 0);
  if (ret < len)
  {
    return -1;
  }
  return 0;
}

int CXeleTraderOrderApiImp::SendOrder(const void* data, int len)
{
  int ret = sendto(m_up_sock, (unsigned char*)data, len, 0, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));
  if (ret < len)
  {
      return -1;
  }
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoRspFunc(faircpp::FairMessageParse* pFairMessageParse)
{
  unsigned char hid = pFairMessageParse->GetFairHeadId();
  unsigned char ehd_len = pFairMessageParse->GetFairHeadExpLength();
  if (hid == 0 && ehd_len == 2)
  {
      this->NakedProtoOnHeartbeat(pFairMessageParse);
      return 0;
  }
  
  //
  unsigned int tid = pFairMessageParse->GetFairContextHeaderTid();
  //DbgPrint("tid: 0x%.4X\n", tid);
  switch (tid) {
  case TID_RSP_USER_LOGIN:
    this->NakedProtoOnRspUserLogin(pFairMessageParse);
    break;
  case TID_RSP_USER_LOGOUT:
    this->NakedProtoOnRspUserLogout(pFairMessageParse);
    break;
  case TID_RSP_ORDER_INSERT:
    this->NakedProtoOnRspOrderInsert(pFairMessageParse);
    break;
  case TID_RSP_ORDER_ACTION:
    this->NakedProtoOnRspOrderAction(pFairMessageParse);
    break;
  case TID_RTN_ORDER:
    this->NakedProtoOnRtnOrder(pFairMessageParse);
    break;
  case TID_RTN_TRADE:
    this->NakedProtoOnRtnTrade(pFairMessageParse);
    break;
  case TID_ERR_RTN_ORDER_INSERT:
    this->NakedProtoOnErrRtnOrderInsert(pFairMessageParse);
    break;
  case TID_ERR_RTN_ORDER_ACTION:
    this->NakedProtoOnErrRtnOrderAction(pFairMessageParse);
    break;
  default:
    break;
  }
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRspUserLogin(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspUserLoginField* pRspUserLoginField = (CXeleFtdcRspUserLoginField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetSecondFtdFieldData());
  int nRequestID = pFairMessageParse->GetFairContextHeaderReqID();
  bool bIsLast = (pFairMessageParse->GetFairContextHeaderChain() == 'L' || pFairMessageParse->GetFairContextHeaderChain() == 'S');
  this->m_originalTraderSpi->OnRspUserLogin(pRspUserLoginField, pRspInfoField, nRequestID, bIsLast);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRspUserLogout(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspUserLogoutField* pRspUserLogoutField = (CXeleFtdcRspUserLogoutField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetSecondFtdFieldData());
  int nRequestID = pFairMessageParse->GetFairContextHeaderReqID();
  bool bIsLast = (pFairMessageParse->GetFairContextHeaderChain() == 'L' || pFairMessageParse->GetFairContextHeaderChain() == 'S');
  this->m_originalTraderSpi->OnRspUserLogout(pRspUserLogoutField, pRspInfoField, nRequestID, bIsLast);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRspOrderInsert(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcInputOrderField* pInputOrder = (CXeleFtdcInputOrderField*)(pFairMessageParse->GetSecondFtdFieldData());
  int nRequestID = pFairMessageParse->GetFairContextHeaderReqID();
  bool bIsLast = (pFairMessageParse->GetFairContextHeaderChain() == 'L' || pFairMessageParse->GetFairContextHeaderChain() == 'S');
  this->m_originalTraderSpi->OnRspOrderInsert(pInputOrder, pRspInfoField, nRequestID, bIsLast);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRspOrderAction(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcOrderActionField* pOrderAction = (CXeleFtdcOrderActionField*)(pFairMessageParse->GetSecondFtdFieldData());
  int nRequestID = pFairMessageParse->GetFairContextHeaderReqID();
  bool bIsLast = (pFairMessageParse->GetFairContextHeaderChain() == 'L' || pFairMessageParse->GetFairContextHeaderChain() == 'S');
  this->m_originalTraderSpi->OnRspOrderAction(pOrderAction, pRspInfoField, nRequestID, bIsLast);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnErrRtnOrderInsert(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcInputOrderField* pInputOrder = (CXeleFtdcInputOrderField*)(pFairMessageParse->GetSecondFtdFieldData());
  this->m_originalTraderSpi->OnErrRtnOrderInsert(pInputOrder, pRspInfoField);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnErrRtnOrderAction(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcRspInfoField* pRspInfoField = (CXeleFtdcRspInfoField*)(pFairMessageParse->GetFirstFtdFieldData());
  CXeleFtdcOrderActionField* pOrderAction = (CXeleFtdcOrderActionField*)(pFairMessageParse->GetSecondFtdFieldData());
  this->m_originalTraderSpi->OnErrRtnOrderAction(pOrderAction, pRspInfoField);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnHeartbeat(faircpp::FairMessageParse* pFairMessageParse)
{
  char buf[12] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->SendReq((void*)buf, sizeof(buf));
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRtnOrder(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcOrderField*  pOrderField = (CXeleFtdcOrderField*)(pFairMessageParse->GetFirstFtdFieldData());
  this->m_originalTraderSpi->OnRtnOrder(pOrderField);
  return 0;
}

int CXeleTraderOrderApiImp::NakedProtoOnRtnTrade(faircpp::FairMessageParse* pFairMessageParse)
{
  CXeleFtdcTradeField*  pOrderField = (CXeleFtdcTradeField*)(pFairMessageParse->GetFirstFtdFieldData());
  this->m_originalTraderSpi->OnRtnTrade(pOrderField);
  return 0;
}



