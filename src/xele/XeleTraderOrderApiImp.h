#if !defined(_XELE_FTDCTRADERORDERAPIIMP_H)

#define _XELE_FTDCTRADERORDERAPIIMP_H
#include <string>
#include <common.h>
#include <fair.h>
#include <CFairMsgRecv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <chrono>
#include <pthread.h>

#include "CXeleFtdcUserApiStruct.h"
#include "CXeleTraderApi.hpp"

class CXeleTraderOrderApiImp : public CXeleTraderOrderApi
{
public:
   void Release();

  ///初始化
  ///@remark 初始化运行环境,只有调用后,接口才开始工作
   void Init(TXeleFtdcClientIndexType* pClientIndex, TXeleFtdcClientTokenType* pToken);

  ///等待接口线程结束运行
  ///@return 线程退出代码
   int Join();

  ///注册前置机网络地址
  ///@param pszFrontAddress：前置机网络地址。
  ///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。
  ///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
   void RegisterFront(char *pszUdpFrontAddress, char *pszTcpQueryFrontAddress);

  ///注册回调接口
  ///@param pSpi 派生自回调接口类的实例
   void RegisterSpi(CXeleTraderSpi *pSpi);

  ///用户登录请求
   int ReqUserLogin(int nRequestID);

  ///用户退出请求
   int ReqUserLogout(CXeleFtdcReqUserLogoutField *pReqUserLogout,
                            int nRequestID);

  ///报单录入请求
   int ReqOrderInsert(CXeleFairInputOrderField *pInputOrder,
                             int nRequestID);

  ///报单操作请求
   int ReqOrderAction(CXeleFairOrderActionField *pOrderAction,
                             int nRequestID);
private:
  CXeleTraderSpi* m_originalTraderSpi;
  TXeleFtdcClientIndexType m_clientIndex[2];
  TXeleFtdcClientTokenType m_clientToken[2];
  pthread_t m_hReadThread;
  std::string m_strUpURL;
  std::string m_strDownURL;
  int m_up_sock;
  int m_down_sock;
  struct sockaddr_in m_ServerAddr;
  int m_down_seqno;

  std::string m_strNakedOrderAddress;
  std::string m_strTradeCenter;
  std::string m_strParticipantId;
  std::string m_strAccountId;

#define CACHE_BUFFER_SIZE 0x10000

  char m_Cache[CACHE_BUFFER_SIZE];
  int m_CachePos;
  
public:
  CXeleTraderOrderApiImp();
  
  static void* ThreadNakedProtocol(void* arg);
  bool m_threadLoop;
  
  void ProcessMessage();

private:
  bool Connect();
  
  void Disconnect();

  bool OnReceive();

  bool CreateUpConnection(std::string strURL);
  
  bool CreateDownConnection(std::string strURL);
  
  int SendReq(const void* data, int len);
  
  int SendOrder(const void* data, int len);
  
  int NakedProtoRspFunc(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRspUserLogin(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRspUserLogout(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRspOrderInsert(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRtnOrder(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRtnTrade(faircpp::FairMessageParse* pFairMessageParse);
  
  int NakedProtoOnRspOrderAction(faircpp::FairMessageParse* pFairMessageParse);

  int NakedProtoOnErrRtnOrderInsert(faircpp::FairMessageParse* pFairMessageParse);

  int NakedProtoOnErrRtnOrderAction(faircpp::FairMessageParse* pFairMessageParse);

  int NakedProtoOnHeartbeat(faircpp::FairMessageParse* pFairMessageParse);


};

#endif


