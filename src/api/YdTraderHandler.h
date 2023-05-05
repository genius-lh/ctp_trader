#ifndef _YD_TRADER_HANDLER_H_
#define _YD_TRADER_HANDLER_H_
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <string>
#include <map>
#include <unistd.h>
#include "ydApi.h"
#include "ydError.h"

class CYdTraderHandler : public YDListener
{
public:
  CYdTraderHandler(YDExtendedApi* pApi, void* pArg);
  virtual ~CYdTraderHandler();
  
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
public:
  void Join();
  void InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id);
  void QryInstrument();
  void QryInvestorPosition();
  void QryTradingAccount();
  int GetTradingDate();
  int GetMaxOrderRef();
private:
  const char ConvertOrderStatus(int yd_order_status);
private:
  void* m_Arg;
  YDExtendedApi* m_TraderApi;
  int   m_TradingDate;            ///< 交易日，格式为yyyyMMdd的int型值
  int   m_MaxOrderRef;             ///< 以前的最大 token 
  int   m_IsLogin;

  // 退出时使用
  pthread_mutex_t m_DestroyMutex;
  pthread_cond_t m_DestroyCond;
};


#endif //_YD_TRADER_HANDLER_H_

