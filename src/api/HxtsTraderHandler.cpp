#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <map>
using namespace std;

#include "hxapitype.h"
#include "hxapistructs.h"
#include "hxapi.h"

#include "HxtsTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

#include "cmn_log.h"

#ifdef __cplusplus
}
#endif

CHxtsTraderHandler::CHxtsTraderHandler(CHxTraderApi* pApi, void* pArg)
: m_Arg(pArg)
, m_TraderApi(pApi)
, m_IsLogin(0)
{
}

CHxtsTraderHandler::~CHxtsTraderHandler()
{

}

void CHxtsTraderHandler::OnRspLogin(rsp_login_field_s* rsp_login_field, error_field_s* error_msg)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_trader_api_on_rsp_user_login(self, error_msg->errorID, error_msg->errorMsg);
  if(rsp_login_field){
    m_SeatCount = rsp_login_field->seatCount;
    m_MaxOrderRef = rsp_login_field->maxOrderRef;
    strncpy(m_TradingDate, rsp_login_field->szTradeDate, sizeof(m_TradingDate));
    strncpy(m_Account, rsp_login_field->szAccount, sizeof(m_Account));
    
  }
  return;
}

void CHxtsTraderHandler::OnRspLogout(error_field_s* error_msg)
{
  if(!error_msg){
    CMN_ERROR("NULL == error_msg\n");
    return;
  }
  CMN_DEBUG(
    "error_msg->errorID=[%d]\n"
    "error_msg->errorMsg=[%s]\n"
    , error_msg->errorID
    , error_msg->errorMsg
  );
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_rsp_user_logout(self, 0, "OK");

  return;
}

// ��̨ϵͳ�ж������д���ʱ���ش���Ϣ��
void CHxtsTraderHandler::OnHxRspOrderInsert(hx_rsp_order_field_s* hx_rsp_order_insert_field, error_field_s* error_msg)
{

  if(!error_msg){
    CMN_ERROR("NULL == error_msg\n");
    return;
  }


  if(!error_msg->errorID){
    return;
  }

  if(!hx_rsp_order_insert_field){
    CMN_ERROR("NULL == hx_rsp_order_insert_field\n");
    return;
  }
  
  CMN_DEBUG(
    "error_msg->errorID=[%d]\n"
    "error_msg->errorMsg=[%s]\n"
    , error_msg->errorID
    , error_msg->errorMsg
  );
  
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order* traderOrder = (trader_order*)GetOrder(hx_rsp_order_insert_field->orderRef);
  if((void*)NULL == traderOrder){
    CMN_ERROR("NULL == traderOrder[%d]\n", hx_rsp_order_insert_field->orderRef);
    return;
  }
  traderOrder->OrderStatus = TRADER_ORDER_OS_CANCELED;
  trader_trader_api_on_rtn_order(self, traderOrder);

  trader_trader_api_on_err_rtn_order_insert(self, error_msg->errorID, error_msg->errorMsg);
  return;
}

// �ӽ��������صı�����Ӧ��Ϣ��
void CHxtsTraderHandler::OnExgRspOrderInsert(rsp_order_field_s* rsp_order_insert_field, error_field_s* error_msg)
{
  if(!error_msg){
    CMN_ERROR("NULL == error_msg\n");
    return;
  }

  CMN_DEBUG(
    "error_msg->errorID=[%d]\n"
    "error_msg->errorMsg=[%s]\n"
    , error_msg->errorID
    , error_msg->errorMsg
  );

  
  if(!rsp_order_insert_field){
    CMN_ERROR("NULL == hx_rsp_order_insert_field\n");
    return;
  }

  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order* traderOrder = (trader_order*)GetOrder(rsp_order_insert_field->orderRef);
  if((void*)NULL == traderOrder){
    CMN_ERROR("NULL == traderOrder[%d]\n", rsp_order_insert_field->orderRef);
    return;
  }
  strncpy(traderOrder->OrderSysID, rsp_order_insert_field->sysID, sizeof(traderOrder->OrderSysID));
  if(!error_msg->errorID){
    traderOrder->OrderStatus = TRADER_ORDER_OS_NOTRADEQUEUEING;
  }else{
    traderOrder->OrderStatus = TRADER_ORDER_OS_CANCELED;
    trader_trader_api_on_err_rtn_order_insert(self, error_msg->errorID, error_msg->errorMsg);
  }
  trader_trader_api_on_rtn_order(self, traderOrder);
  return;
}

void CHxtsTraderHandler::OnRtnOrder(rtn_order_field_s* rtn_order_field)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order* traderOrder = (trader_order*)GetOrder(rtn_order_field->orderRef);
  if((void*)NULL == traderOrder){
    CMN_ERROR("NULL == traderOrder[%d]\n", rtn_order_field->orderRef);
    return;
  }
  if('\0' == traderOrder->OrderSysID[0]){
    strncpy(traderOrder->OrderSysID, rtn_order_field->sysID, sizeof(traderOrder->OrderSysID));
  }
  ///����ʱ��
  if('\0' == traderOrder->InsertTime[0]){
    strncpy(traderOrder->InsertTime, rtn_order_field->insertTime, sizeof(traderOrder->InsertTime));
  }
  traderOrder->OrderStatus = rtn_order_field->orderStatus;
  traderOrder->VolumeTraded = traderOrder->VolumeOriginal - rtn_order_field->leftVolume;
  trader_trader_api_on_rtn_order(self, traderOrder);
}

void CHxtsTraderHandler::OnRtnTrade(rtn_trade_field_s* rtn_trade_field)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  rtn_trade_field_s* pTrade = rtn_trade_field;
  
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  trader_order* traderOrder = (trader_order*)GetOrder(rtn_trade_field->orderRef);
  if((void*)NULL == traderOrder){
    CMN_ERROR("NULL == traderOrder[%d]\n", rtn_trade_field->orderRef);
    return;
  }
  
  ///��Լ����
  strcpy(traderTrade.InstrumentID, traderOrder->InstrumentID);
  ///���ر������
  strcpy(traderTrade.UserOrderLocalID, traderOrder->UserOrderLocalID);
  ///������
  strncpy(traderTrade.TradingDay, m_TradingDate, sizeof(traderTrade.TradingDay));
  ///�ɽ�ʱ��
  strncpy(traderTrade.TradeTime, pTrade->tradedTime, sizeof(traderTrade.TradeTime));
  ///��������
  traderTrade.Direction = traderOrder->Direction;
  ///��ƽ��־
  traderTrade.OffsetFlag = traderOrder->OffsetFlag;
  ///�ɽ��۸�
  traderTrade.TradePrice = pTrade->tradedPrice;
  ///�ɽ�����
  traderTrade.TradeVolume = pTrade->tradedVolume;
  //�ɽ����
  strncpy(traderTrade.TradeID, pTrade->tradeID, sizeof(traderTrade.TradeID));

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void CHxtsTraderHandler::OnRtnInsStatus(rtn_ins_status_field_s* rtn_status_field)
{
  return ;
}

//��̨��������������Ӧ
void CHxtsTraderHandler::OnHxRspCancelOrder(hx_rsp_action_order_field_s* pOrderAction, error_field_s* error_msg)
{

  if(!error_msg){
    CMN_ERROR("NULL == error_msg\n");
    return;
  }

  if(!pOrderAction){
    CMN_ERROR("NULL == pOrderAction\n");
    return;
  }
  
  if(!error_msg->errorID){
    return;
  }
  
  CMN_INFO(
    "pOrderAction->orderNO=[%d]\n"
    "pOrderAction->sysID=[%s]\n"
    "pOrderAction->orderRef=[%d]\n"
    , pOrderAction->orderNO
    , pOrderAction->sysID
    , pOrderAction->orderRef
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_err_rtn_order_action(self, error_msg->errorID, error_msg->errorMsg);

}

//��������������������Ӧ
void CHxtsTraderHandler::OnRspCancelOrder(rsp_action_order_field_s* pOrderAction, error_field_s* error_msg)
{
  if(!error_msg){
    CMN_ERROR("NULL == error_msg\n");
    return;
  }

  if(!pOrderAction){
    CMN_ERROR("NULL == pOrderAction\n");
    return;
  }
  
  CMN_INFO(
    "pOrderAction->orderNO=[%d]\n"
    "pOrderAction->sysID=[%s]\n"
    "pOrderAction->orderRef=[%d]\n"
    , pOrderAction->orderNO
    , pOrderAction->sysID
    , pOrderAction->orderRef
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_rsp_order_action(self, error_msg->errorID, error_msg->errorMsg);
  return;
}

// ���һ������pos_fieldΪNULL��is_lastΪtrue.
void CHxtsTraderHandler::OnQryPos(rsp_pos_field_s* pos_field, bool is_last)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  if(!pos_field){
    trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, is_last);
    return;
  }
  strcpy(traderPosition.InstrumentID, pos_field->ins);
  traderPosition.PositionDate = '3'; //
  traderPosition.PosiDirection = TRADER_POSITION_LONG;
  // ��ǰ���
  traderPosition.YdPosition = pos_field->long_vol_yd;
  // ��ǰ���
  traderPosition.TodayPosition = pos_field->long_vol_td;
  // ����Ľ������
  traderPosition.Position = 0;
  // ������������
  traderPosition.LongFrozen = 0;
  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, false);

  traderPosition.PosiDirection = TRADER_POSITION_SHORT;
  // ��ǰ���
  traderPosition.YdPosition = pos_field->short_vol_yd;
  // ��ǰ���
  traderPosition.TodayPosition = pos_field->short_vol_td;
  // ����Ľ������
  traderPosition.Position = 0;
  // ������������
  traderPosition.LongFrozen = 0;
  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, false);
  return ;
}

void CHxtsTraderHandler::OnQryFund(rsp_fund_field_s* fund_field)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  rsp_fund_field_s* pTradingAccount = fund_field;
  trader_account traderAccount;
  if(!pTradingAccount){
    return;
  }
  
  CMN_INFO(
    "pTradingAccount->beginBalance[%lf]"
    "pTradingAccount->curBalance[%lf]"
    "pTradingAccount->closeProfit[%lf]"
    "pTradingAccount->availableMoney[%lf]"
    "pTradingAccount->frozenMoney[%lf]"
    "pTradingAccount->marginMoney[%lf]"
    "pTradingAccount->feeMoney[%lf]"
    "pTradingAccount->declareFee[%lf]"
    "pTradingAccount->syncMoney[%lf]"
    "\n"
    , pTradingAccount->beginBalance
    , pTradingAccount->curBalance
    , pTradingAccount->closeProfit
    , pTradingAccount->availableMoney
    , pTradingAccount->frozenMoney
    , pTradingAccount->marginMoney
    , pTradingAccount->feeMoney
    , pTradingAccount->declareFee
    , pTradingAccount->syncMoney
  );

  strcpy(traderAccount.AccountID, m_Account);

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, 0, NULL, true);

}

// ���һ������ins_fieldΪNULL��is_lastΪtrue.
void CHxtsTraderHandler::OnQryIns(rsp_instrument_field_s* ins_field, bool is_last)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  if(!ins_field){
    trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, is_last);
    return;
  }

  strcpy(traderInstrument.InstrumentID, ins_field->ins);
  // Ŀǰֻ֧������
  strcpy(traderInstrument.ExchangeID, "SHFE");
  traderInstrument.PriceTick = ins_field->price_tick;
  traderInstrument.VolumeMultiple = ins_field->vol_multi;

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, false);
  
  return;
}

void CHxtsTraderHandler::InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  static char seatId = 0;
  // ��ʼ������
  trader_order* traderOrder = (trader_order*)malloc(sizeof(trader_order));
  memset(traderOrder, 0, sizeof(trader_order));
	///����������
  strncpy(traderOrder->ExchangeID, GetExchangeId(inst), sizeof(traderOrder->ExchangeID));
	///ϵͳ�������
  // ��Լ����
  strncpy(traderOrder->InstrumentID, inst, sizeof(traderOrder->InstrumentID));
  // ���ر������
  strncpy(traderOrder->UserOrderLocalID, local_id, sizeof(traderOrder->UserOrderLocalID));
  // ����
  traderOrder->Direction = buy_sell;
  // ��ƽ
  // ƽ�����⴦��
  if(TRADER_POSITION_CLOSE == open_close){
    open_close = TRADER_POSITION_CLOSE_YESTERDAY;
  }
  traderOrder->OffsetFlag = open_close;
  ///Ͷ���ױ���־
  traderOrder->HedgeFlag = '0';
  // �����۸�
  traderOrder->LimitPrice = price;
  // ��������
  traderOrder->VolumeOriginal = vol;
  // �ɽ�����
  traderOrder->VolumeTraded = 0;
  // ����״̬
  traderOrder->OrderStatus = TRADER_ORDER_OS_UNKNOW;
  ///����ʱ��

  unsigned int orderRef = atol(local_id);
  mapOrder.insert(map<unsigned int, void*>::value_type(orderRef, (void*)traderOrder));
  

  req_insert_order_field_s order_field;
  memset(&order_field, 0, sizeof(order_field));
	order_field.dir = buy_sell;
	strncpy(order_field.ins, inst, sizeof(order_field.ins));	
	order_field.offset = open_close;
	order_field.volume = vol;
	order_field.volumeCondition = HXTS_VC_AV;
	order_field.priceType = HXTS_OPT_LimitPrice;
	order_field.timeCondition = HXTS_TC_GFD;
	order_field.price = price;
		
	order_field.seat_id = seatId % m_SeatCount;
	order_field.OrderRef = orderRef;
  seatId++;

	m_TraderApi->ReqOrderInsert(&order_field);
  return;

}

void CHxtsTraderHandler::CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id)
{
	req_action_order_field_s cancel_order;
  memset(&cancel_order, 0, sizeof(cancel_order));
	cancel_order.orderNO = 0;
	strncpy(cancel_order.sysID, order_sys_id, sizeof(cancel_order.sysID));
	m_TraderApi->ReqCancelOrder(&cancel_order);
  return;
}

void CHxtsTraderHandler::QryInstrument()
{
	m_TraderApi->ReqQryIns("");

  return;
}

void CHxtsTraderHandler::QryInvestorPosition()
{
	m_TraderApi->ReqQryPos(NULL);

  return;
}

void CHxtsTraderHandler::QryTradingAccount()
{
	m_TraderApi->ReqQryFund();

  return;
}

const char* CHxtsTraderHandler::GetTradingDate()
{
  return m_TradingDate;
}
int CHxtsTraderHandler::GetMaxOrderRef()
{
  return m_MaxOrderRef;
}

const char* CHxtsTraderHandler::GetExchangeId(const char* instrument)
{
  // TODO
  return "SHFE";
}

const void* CHxtsTraderHandler::GetOrder(unsigned int orderRef)
{
  map<unsigned int, void*>::iterator iter = mapOrder.find(orderRef);
  if(iter == mapOrder.end()){
		CMN_ERROR("find order failed(%u)\n", orderRef);
    return (void*)NULL;
  }
  
  return iter->second;
}



