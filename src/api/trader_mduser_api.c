
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/util.h>

#include "cmn_util.h"

#include "trader_mduser_api.h"

trader_mduser_api* trader_mduser_api_new(evutil_socket_t fd, trader_mduser_api_method* method)
{
  trader_mduser_api* self = (trader_mduser_api*)malloc(sizeof(trader_mduser_api));
  memset(self, 0, sizeof(trader_mduser_api));

  self->fd = fd;
  self->pMethod = method;

  return self;
}

void trader_mduser_api_free(trader_mduser_api* self)
{
  if(self){
    free(self);
  }
}

void trader_mduser_api_set_user(trader_mduser_api* self, char* broker_id, char* user, char* pwd)
{
  self->pBrokerID = broker_id;
  self->pUser = user;
  self->pPwd = pwd;
}

void trader_mduser_api_set_front_addr(trader_mduser_api* self, char* addr)
{
  self->pAddress = addr;
}

void trader_mduser_api_set_workspace(trader_mduser_api* self, char* ws)
{
  self->pWorkspace = ws;
}

void trader_mduser_api_on_front_connected(trader_mduser_api* self)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONFRONTCONNECTED;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_front_disconnected(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONFRONTDISCONNECTED;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_user_login(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRSPUSERLOGIN;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_user_logout(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRSPUSERLOGOUT;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_error(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRSPERROR;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRSPSUBMARKETDATA;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rsp_un_sub_market_data(trader_mduser_api* self, int err_cd, char* err_msg)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRSPUNSUBMARKETDATA;
  oEvent.ErrorCd = err_cd;
  if(err_msg){
    strcpy(oEvent.ErrorMsg, err_msg);
  }

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

void trader_mduser_api_on_rtn_depth_market_data(trader_mduser_api* self, trader_tick* tick)
{
  trader_mduser_evt oEvent;
  oEvent.Type = ONRTNDEPTHMARKETDATA;
  oEvent.ErrorCd = 0;
  oEvent.ErrorMsg[0] = '\0';
  memcpy(&oEvent.Tick, tick, sizeof(trader_tick));

  cmn_util_evbuffer_send(self->fd, (unsigned char*)&oEvent, sizeof(oEvent));
  return ;
}

