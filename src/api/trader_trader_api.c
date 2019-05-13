
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/util.h>

#include "cmn_util.h"

#include "trader_trader_api.h"

trader_trader_api* trader_trader_api_new(evutil_socket_t fd, trader_trader_api_method* method)
{
  trader_trader_api* self = (trader_trader_api*)malloc(sizeof(trader_trader_api));
  self->fd = fd;
  self->pMethod = method;

  return self;

}

void trader_trader_api_free(trader_trader_api* self)
{
  if(self){
    free(self);
  }
}


void trader_trader_api_set_user(trader_trader_api* self, char* broker_id, char* user, char* pwd)
{
  self->pBrokerID = broker_id;
  self->pUser = user;
  self->pPwd = pwd;
}

void trader_trader_api_set_front_addr(trader_trader_api* self, char* addr)
{
  self->pAddress = addr;
}

void trader_trader_api_set_workspace(trader_trader_api* self, char* ws)
{
  self->pWorkspace = ws;
}

void trader_trader_api_set_param(trader_trader_api* self, char* key, char* val)
{
  if(!strcmp("TIME_CONDITION", key)){
    self->timeCondition = val[0];
  }
  
  if(!strcmp("HEDGE_FLAG", key)){
    self->hedgeFlag = val[0];
  }
}

int trader_trader_api_get_trading_day(trader_trader_api* self, char* tradingday)
{
  return 0;
}

int trader_trader_api_get_max_order_local_id(trader_trader_api* self, char* local_id)
{
  int nIdLen;
  char* sOrderId;
  self->userLocalId = 1;
  nIdLen = strlen(local_id);
  if(nIdLen > 1){
    sOrderId = (char*)malloc(sizeof(char) * nIdLen);
    memcpy(sOrderId, local_id, nIdLen - 1);
    sOrderId[nIdLen - 1] = '\0';
    self->userLocalId = atol(sOrderId);
    free(sOrderId);
  }
  return 0;
}

void trader_trader_api_on_front_connected(trader_trader_api* self)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONFRONTCONNECTED;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_front_disconnected(trader_trader_api* self, int nReason)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONFRONTDISCONNECTED;
  oEvent.ErrorCd = nReason;
  oEvent.ErrorMsg[0] = '\0';
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rsp_error(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPERROR;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}

void trader_trader_api_on_rsp_user_login(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPUSERLOGIN;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rsp_user_logout(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPUSERLOGOUT;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rsp_order_insert(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPORDERINSERT;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rsp_order_action(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPORDERACTION;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rtn_trade(trader_trader_api* self, trader_trade* trade)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRTNTRADE;
  oEvent.ErrorCd = 0;
  oEvent.IsLast = 1;
  memcpy(&(oEvent.Body.TradeRsp), trade, sizeof(oEvent.Body.TradeRsp));
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_rtn_order(trader_trader_api* self, trader_order* order)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRTNORDER;
  oEvent.ErrorCd = 0;
  oEvent.IsLast = 1;
  memcpy(&(oEvent.Body.OrderRsp), order, sizeof(oEvent.Body.OrderRsp));
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}

void trader_trader_api_on_err_rtn_order_insert(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONERRRTNORDERINSERT;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}

void trader_trader_api_on_err_rtn_order_action(trader_trader_api* self, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONERRRTNORDERACTION;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  oEvent.IsLast = 1;

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
}


void trader_trader_api_on_rsp_qry_user_investor(trader_trader_api* self, char* investor_id, int err_cd, char* err_msg)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPQRYINVESTOR;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  oEvent.IsLast = 1;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  strcpy(oEvent.Body.InvestorRsp, investor_id);
  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}

void trader_trader_api_on_rsp_qry_instrument(trader_trader_api* self, trader_instrument *pRspInstrument, int err_cd, char* err_msg, int is_last)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPQRYINSTRUMENT;
  oEvent.IsLast = is_last;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  memcpy(&(oEvent.Body.InstrumentRsp), pRspInstrument, sizeof(oEvent.Body.InstrumentRsp));

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}

void trader_trader_api_on_rsp_qry_investor_position(trader_trader_api* self, trader_position *pInvestorPosition, int err_cd, char* err_msg, int is_last)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPQRYINVESTORPOSITION;
  oEvent.IsLast = is_last;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  memcpy(&(oEvent.Body.PositionRsp), pInvestorPosition, sizeof(oEvent.Body.PositionRsp));

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}

void trader_trader_api_on_rsp_qry_trading_account(trader_trader_api* self, trader_account *pTradingAcount, int err_cd, char* err_msg, int is_last)
{
  trader_trader_evt oEvent;
  oEvent.Type = TRADERONRSPQRYTRADINGACCOUNT;
  oEvent.IsLast = is_last;
  oEvent.ErrorCd = err_cd;
  oEvent.ErrorMsg[0] = '\0';
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }
  
  memcpy(&(oEvent.Body.AccountRsp), pTradingAcount, sizeof(oEvent.Body.AccountRsp));

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));

}



