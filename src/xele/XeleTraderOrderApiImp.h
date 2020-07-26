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

  ///��ʼ��
  ///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
   void Init(TXeleFtdcClientIndexType* pClientIndex, TXeleFtdcClientTokenType* pToken);

  ///�ȴ��ӿ��߳̽�������
  ///@return �߳��˳�����
   int Join();

  ///ע��ǰ�û������ַ
  ///@param pszFrontAddress��ǰ�û������ַ��
  ///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001����
  ///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
   void RegisterFront(char *pszUdpFrontAddress, char *pszTcpQueryFrontAddress);

  ///ע��ص��ӿ�
  ///@param pSpi �����Իص��ӿ����ʵ��
   void RegisterSpi(CXeleTraderSpi *pSpi);

  ///�û���¼����
   int ReqUserLogin(int nRequestID);

  ///�û��˳�����
   int ReqUserLogout(CXeleFtdcReqUserLogoutField *pReqUserLogout,
                            int nRequestID);

  ///����¼������
   int ReqOrderInsert(CXeleFairInputOrderField *pInputOrder,
                             int nRequestID);

  ///������������
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


