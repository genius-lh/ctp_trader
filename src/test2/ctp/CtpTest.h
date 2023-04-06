#ifndef _XSPEED_TEST_H_
#define _XSPEED_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
//using namespace ctp_sopt;

class CCtpSopTraderHandler : public CThostFtdcTraderSpi
{
public:
  CCtpSopTraderHandler();

  virtual ~CCtpSopTraderHandler();
  
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  void OnFrontConnected();

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  void OnFrontDisconnected(int nReason);

  ///客户端认证响应
  void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登录请求响应
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登出请求响应
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  
  ///报单录入请求响应
  void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单操作请求响应
  void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者结算结果确认响应
  void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///执行宣告录入请求响应
  void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///执行宣告操作请求响应
  void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询报单响应
  void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询合约响应
  void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///错误应答
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单通知
  void OnRtnOrder(CThostFtdcOrderField *pOrder);

  ///成交通知
  void OnRtnTrade(CThostFtdcTradeField *pTrade);

  ///报单录入错误回报
  void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

  ///报单操作错误回报
  void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

  ///执行宣告通知
  void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

  ///执行宣告录入错误回报
  void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);

  ///执行宣告操作错误回报
  void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);
  
	///用户口令更新请求响应
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_NewPasswd;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void QueryContract();
  void SettlementInfoConfirm();
  void InsertOrder(char* inst, char* exchange_id, char buy_sell, char open_close, double price, int vol, char covered);
  void ExecuteOrder(char* inst, char* exchange_id, int vol);
  void WithdrawOrder(char* inst, char* exchange_id, long sys_order_id);
  void QueryOrder(char* inst, char* exchange_id, long sys_order_id);
  
  void CoveredOpen();
  void CoveredClose();
  void CallBuyOpen();
  void CallSellClose();
  void CallSellOpen();
  void CallBuyClose();
  void PutBuyOpen();
  void PutSellClose();
  void PutSellOpen();
  void PutBuyClose();
  void CallExecuteOrder();
  void PutExecuteOrder();
  void LogOut();
  void Query();
  void Withdraw();
  
  void PasswordUpdate();
  void Authenticate();
  void Login();

};


#endif //_XSPEED_TEST_H_

