#ifndef _HXTS_TEST_H_
#define _HXTS_TEST_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include "hxapitype.h"
#include "hxapistructs.h"
#include "hxapi.h"

class CHxtsTestHandler : public CHxTraderSpi
{
public:
  CHxtsTestHandler();
  virtual ~CHxtsTestHandler();
  
  void OnRspLogin(rsp_login_field_s* rsp_login_field, error_field_s* error_msg);

  void OnRspLogout(error_field_s* error_msg);

	void OnRspChangePwd(error_field_s* error_msg);

  // ��̨ϵͳ�ж������д���ʱ���ش���Ϣ��
  void OnHxRspOrderInsert(hx_rsp_order_field_s* hx_rsp_order_insert_field, error_field_s* error_msg) ;

  // �ӽ��������صı�����Ӧ��Ϣ��
  void OnExgRspOrderInsert(rsp_order_field_s* rsp_order_insert_field, error_field_s* error_msg);

  void OnRtnOrder(rtn_order_field_s* rtn_order_field);

  void OnRtnTrade(rtn_trade_field_s* rtn_trade_field);

  void OnRtnInsStatus(rtn_ins_status_field_s* rtn_status_field);

  //��̨��������������Ӧ
  void OnHxRspCancelOrder(hx_rsp_action_order_field_s* pOrderAction, error_field_s* error_msg);

  //��������������������Ӧ
  void OnRspCancelOrder(rsp_action_order_field_s* pOrderAction, error_field_s* error_msg);

	// ���һ������order_fieldΪNULL��is_lastΪtrue.
	void OnQryOrder(rsp_qry_order_field_s* order_field, bool is_last);

	// ���һ������trade_fieldΪNULL��is_lastΪtrue.
	void OnQryTrade(rtn_trade_field_s* trade_field, bool is_last);

  // ���һ������pos_fieldΪNULL��is_lastΪtrue.
  void OnQryPos(rsp_pos_field_s* pos_field, bool is_last);

  void OnQryFund(rsp_fund_field_s* fund_field);

  // ���һ������ins_fieldΪNULL��is_lastΪtrue.
  void OnQryIns(rsp_instrument_field_s* ins_field, bool is_last);
public:
  void InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id);
  const char* GetTradingDate();
  int GetMaxOrderRef();
public:
  void* m_TraderApi;
  char  m_TradingDate[15];            ///< �����գ���ʽ��֪��
  int   m_MaxOrderRef;             ///< ��ǰ����� token 
  int   m_SeatCount;
public:
  void Loop();
  int ShowMenu();
  void Login();
  void Logout();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryInvestorAccount();
  void QryInstrument();
  void QryInvestorPosition();
  void PrintTrade(void* data);
  void ChangePassword();
public:
  char* m_AppID;
  char* m_AuthCode;
  char* m_UserId;
  char* m_Passwd;
  int m_Loop;

};


#endif //_HXTS_TEST_H_

