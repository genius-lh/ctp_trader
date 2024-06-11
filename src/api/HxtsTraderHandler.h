#ifndef _HXTS_TRADER_HANDLER_H_
#define _HXTS_TRADER_HANDLER_H_
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <string>
#include <map>
#include <unistd.h>

#include "hxapitype.h"
#include "hxapistructs.h"
#include "hxapi.h"

class CHxtsTraderHandler : public CHxTraderSpi
{
public:
  CHxtsTraderHandler(CHxTraderApi* pApi, void* pArg);
  virtual ~CHxtsTraderHandler();
  
  void OnRspLogin(rsp_login_field_s* rsp_login_field, error_field_s* error_msg);

  void OnRspLogout(error_field_s* error_msg);

  // 柜台系统判定报单有错误时返回此消息。
  void OnHxRspOrderInsert(hx_rsp_order_field_s* hx_rsp_order_insert_field, error_field_s* error_msg) ;

  // 从交易所返回的报单响应消息。
  void OnExgRspOrderInsert(rsp_order_field_s* rsp_order_insert_field, error_field_s* error_msg);

  void OnRtnOrder(rtn_order_field_s* rtn_order_field);

  void OnRtnTrade(rtn_trade_field_s* rtn_trade_field);

  void OnRtnInsStatus(rtn_ins_status_field_s* rtn_status_field);

  //柜台报单操作请求响应
  void OnHxRspCancelOrder(hx_rsp_action_order_field_s* pOrderAction, error_field_s* error_msg);

  //交易所报单操作请求响应
  void OnRspCancelOrder(rsp_action_order_field_s* pOrderAction, error_field_s* error_msg);

  // 最后一条数据pos_field为NULL，is_last为true.
  void OnQryPos(rsp_pos_field_s* pos_field, bool is_last);

  void OnQryFund(rsp_fund_field_s* fund_field);

  // 最后一条数据ins_field为NULL，is_last为true.
  void OnQryIns(rsp_instrument_field_s* ins_field, bool is_last);
public:
  void InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id);
  void QryInstrument();
  void QryInvestorPosition();
  void QryTradingAccount();
  const char* GetTradingDate();
  int GetMaxOrderRef();
  const char* GetExchangeId(const char* instrument);
  const void* GetOrder(unsigned int orderRef);
private:
  void* m_Arg;
  CHxTraderApi* m_TraderApi;
  char  m_Account[15];
  char  m_TradingDate[15];            ///< 交易日，格式不知道
  int   m_MaxOrderRef;             ///< 以前的最大 token 
  int   m_SeatCount;
  int   m_IsLogin;
  
	std::map<unsigned int, void*> mapOrder;

};


#endif //_HXTS_TRADER_HANDLER_H_

