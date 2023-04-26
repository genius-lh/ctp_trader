#ifndef _YD_TEST_H_
#define _YD_TEST_H_
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include "ydApi.h"
#include "ydError.h"

class CYdTestHandler : public YDListener
{
public:
  CYdTestHandler();
  virtual ~CYdTestHandler();

	void notifyBeforeApiDestroy(void);
  
	void notifyAfterApiDestroy(void);

	void notifyEvent(int apiEvent);

	void notifyReadyForLogin(bool hasLoginFailed);

	void notifyLogin(int errorNo,int maxOrderRef,bool isMonitor);

	void notifyFinishInit(void);
  
	void notifyCaughtUp(void);

	void notifyOrder(const YDOrder *pOrder,const YDInstrument *pInstrument,const YDAccount *pAccount);

	void notifyTrade(const YDTrade *pTrade,const YDInstrument *pInstrument,const YDAccount *pAccount);

	void notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder,const YDExchange *pExchange,const YDAccount *pAccount);

  void notifyChangePassword(int errorNo);

public:
  
  char* m_AppID;
  char* m_AuthCode;
  char* m_UserId;
  char* m_Passwd;
  int m_Loop;

  void* m_Arg;
  long m_RequestId;
  char m_InvestorID[16];
  
public:
  void Loop();
  int ShowMenu();
  void Logout();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryUserInvestor();
  void QryInvestorAccount();
  void QryInstrument();
  void QryExchange();
  void QryInvestorPosition();
  void PrintOrder(void* data);
  void PrintTrade(void* data);
  void ChangePassword();

};

#endif //_YD_TEST_H_

