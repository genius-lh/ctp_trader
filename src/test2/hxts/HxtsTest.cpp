#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#include "hxapitype.h"
#include "hxapistructs.h"
#include "hxapi.h"

#include "HxtsTest.h"

#define HXTS_TEST_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sSvrIp[128];
  char sSvrPort[6];
  char sLocalIp[128];
  char sLocalPort[6];
  char sUserId[128];
  char sPasswd[128];
  char sAppId[128];
  char sAuthCode[128];

  
  int i = 1;
  if(argc < 9){
    printf("input sSvrIp:\n");
    scanf("%s", sSvrIp);
    printf("input sSvrPort:\n");
    scanf("%s", sSvrPort);
    printf("input sLocalIp:\n");
    scanf("%s", sLocalIp);
    printf("input sLocalPort:\n");
    scanf("%s", sLocalPort);
    printf("input sUserId:\n");
    scanf("%s", sUserId);
    printf("input sPasswd:\n");
    scanf("%s", sPasswd);
    printf("input sAppId:\n");
    scanf("%s", sAppId);
    printf("input sAuthCode:\n");
    scanf("%s", sAuthCode);
  }else{
    strncpy(sSvrIp, argv[i++], sizeof(sSvrIp));
    strncpy(sSvrPort, argv[i++], sizeof(sSvrPort));
    strncpy(sLocalIp, argv[i++], sizeof(sLocalIp));
    strncpy(sLocalPort, argv[i++], sizeof(sLocalPort));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sPasswd, argv[i++], sizeof(sPasswd));
    strncpy(sAppId, argv[i++], sizeof(sAppId));
    strncpy(sAuthCode, argv[i++], sizeof(sAuthCode));
  }
  
	CHxTraderApi *pTraderApi = CHxTraderApi::CreateHxTraderApi();

  if (pTraderApi == NULL)	{
    HXTS_TEST_LOG("can not create API\n");
    exit(1);
  }

  CHxtsTestHandler* pTraderHandler = new CHxtsTestHandler();
	pTraderApi->RegisterSpi(pTraderHandler);
	pTraderApi->SetRecvNodeCount(1024*100);

  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_Passwd = sPasswd;
  pTraderHandler->m_AppID = sAppId;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_Loop = 1;

  pTraderHandler->m_TraderApi = (void*)pTraderApi;

  pthread_t tid = pTraderApi->Init(NULL, sSvrIp, atoi(sSvrPort), sLocalIp, atoi(sLocalPort));

  sleep(2);
  
  pTraderHandler->Loop();

  pthread_cancel(tid);

  delete pTraderHandler;

  return 0;  
}


CHxtsTestHandler::CHxtsTestHandler()
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);

}

CHxtsTestHandler::~CHxtsTestHandler()
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
}

void CHxtsTestHandler::OnRspLogin(rsp_login_field_s* rsp_login_field, error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
  
  if(rsp_login_field){
    HXTS_TEST_LOG("rsp_login_field->szAccount=[%s]\n"
      "rsp_login_field->szTradeDate=[%s]\n"
      "rsp_login_field->maxOrderRef=[%d]\n"
      "rsp_login_field->seatCount=[%d]\n"
      , rsp_login_field->szAccount
      , rsp_login_field->szTradeDate
      , rsp_login_field->maxOrderRef
      , rsp_login_field->seatCount
    );
    strncpy(m_TradingDate, rsp_login_field->szTradeDate, sizeof(m_TradingDate));
    m_MaxOrderRef = rsp_login_field->maxOrderRef;
    m_SeatCount = rsp_login_field->seatCount;
  }
  
  
}

void CHxtsTestHandler::OnRspLogout(error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
  m_Loop = 0;
}


void CHxtsTestHandler::OnRspChangePwd(error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
}


// 柜台系统判定报单有错误时返回此消息。
void CHxtsTestHandler::OnHxRspOrderInsert(hx_rsp_order_field_s* hx_rsp_order_insert_field, error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
}


// 从交易所返回的报单响应消息。
void CHxtsTestHandler::OnExgRspOrderInsert(rsp_order_field_s* rsp_order_insert_field, error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
}


void CHxtsTestHandler::OnRtnOrder(rtn_order_field_s* rtn_order_field)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(!rtn_order_field){
    return;
  }

  HXTS_TEST_LOG(
    "rtn_order_field->ins=[%s]\n"
    "rtn_order_field->sysID=[%s]\n"
    "rtn_order_field->insertTime=[%s]\n"
    "rtn_order_field->orderNO=[%d]\n"
    "rtn_order_field->orderRef=[%d]\n"
    "rtn_order_field->leftVolume=[%d]\n"
    "rtn_order_field->orderStatus=[%d]\n"
    , rtn_order_field->ins
    , rtn_order_field->sysID
    , rtn_order_field->insertTime
    , rtn_order_field->orderNO
    , rtn_order_field->orderRef
    , rtn_order_field->leftVolume
    , rtn_order_field->orderStatus
  );

}


void CHxtsTestHandler::OnRtnTrade(rtn_trade_field_s* rtn_trade_field)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  PrintTrade((void*)rtn_trade_field);
}


void CHxtsTestHandler::OnRtnInsStatus(rtn_ins_status_field_s* rtn_status_field)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
}


//柜台报单操作请求响应
void CHxtsTestHandler::OnHxRspCancelOrder(hx_rsp_action_order_field_s* pOrderAction, error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
}


//交易所报单操作请求响应
void CHxtsTestHandler::OnRspCancelOrder(rsp_action_order_field_s* pOrderAction, error_field_s* error_msg)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  if(error_msg){
    HXTS_TEST_LOG(
      "error_msg->errorID=[%d]\n"
      "error_msg->errorMsg=[%s]\n"
      , error_msg->errorID
      , error_msg->errorMsg
    );
  }
}


// 最后一条数据order_field为NULL，is_last为true.
void CHxtsTestHandler::OnQryOrder(rsp_qry_order_field_s* order_field, bool is_last)
{
  HXTS_TEST_LOG(
    "order_field->ins=[%s]\n"
    "order_field->sysID=[%s]\n"
    "order_field->insertTime=[%s]\n"
    "order_field->orderNO=[%d]\n"
    "order_field->orderRef=[%d]\n"
    "order_field->leftVolume=[%d]\n"
    "order_field->orderStatus=[%d]\n"
    , order_field->ins
    , order_field->sysID
    , order_field->insertTime
    , order_field->orderNO
    , order_field->orderRef
    , order_field->leftVolume
    , order_field->orderStatus
  );
}

// 最后一条数据trade_field为NULL，is_last为true.
void CHxtsTestHandler::OnQryTrade(rtn_trade_field_s* trade_field, bool is_last)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
  PrintTrade((void*)trade_field);
}


// 最后一条数据pos_field为NULL，is_last为true.
void CHxtsTestHandler::OnQryPos(rsp_pos_field_s* pos_field, bool is_last)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
}


void CHxtsTestHandler::OnQryFund(rsp_fund_field_s* fund_field)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
}


// 最后一条数据ins_field为NULL，is_last为true.
void CHxtsTestHandler::OnQryIns(rsp_instrument_field_s* ins_field, bool is_last)
{
  HXTS_TEST_LOG("%s\n", __FUNCTION__);
}


void CHxtsTestHandler::Loop()
{
  int choose;
  while(m_Loop){
    choose = ShowMenu();
    switch(choose){
    case 0:
      Login();
      break;
    case 1:
      QryInstrument();
      break;
    case 2:
      Logout();
      m_Loop = 0;
      sleep(1);
      break;
    case 3:
      OrderInsert();
      break;
    case 4:
      OrderAction();
      break;
    case 5:
      QryOrder();
      break;
    case 6:
      QryTrade();
      break;
    case 7:
      QryInvestorAccount();
      break;
    case 8:
      QryInvestorPosition();
      break;
    case 9:
      ChangePassword();
      break;
    default:
      break;
    }
  }
}

int CHxtsTestHandler::ShowMenu()
{
  int ret = 0;
  HXTS_TEST_LOG("**********************\n"
        "选择需要执行的操作\n"
        "0-Login\n"
        "1-QryInstrument\n"
        "2-Logout\n"
        "3-OrderInsert\n"
        "4-OrderAction\n"
        "5-QryOrder\n"
        "6-QryTrade\n"
        "7-QryInvestorAccount\n"
        "8-QryInvestorPosition\n"
        "9-ChangePassword\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CHxtsTestHandler::Login()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	req_login_field_s req_login;
	strncpy(req_login.szAccount, m_UserId, sizeof(req_login.szAccount));
	strncpy(req_login.szPwd, m_Passwd, sizeof(req_login.szPwd));
	strncpy(req_login.szAppid, m_AppID, sizeof(req_login.szAppid));
	strncpy(req_login.szAuthenticateId, m_AuthCode, sizeof(req_login.szAuthenticateId));

	pTraderApi->ReqLogin(&req_login);
  sleep(2);

}


void CHxtsTestHandler::Logout()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
  pTraderApi->ReqLogout();
  sleep(2);

}

void CHxtsTestHandler::OrderInsert()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
  char Temp[64];
  char InstrumentID[64];
  char Direction;
  char OffsetFlag;
  double LimitPrice;
  int Volume;
  static int m_seat_ind = 0;
  
  printf("input InstrumentID:\n");
  scanf("%s", InstrumentID);
  printf("input Direction[0|1]:\n");
  scanf("%s", Temp);
  Direction = Temp[0];
  printf("input OffsetFlag[0|1|3|4]:\n");
  scanf("%s", Temp);
  OffsetFlag = Temp[0];
  printf("input LimitPrice:\n");
  scanf("%lf", &LimitPrice);
  printf("input Volume:\n");
  scanf("%d", &Volume);

  req_insert_order_field_s order_field;
	order_field.dir = Direction;
	strncpy(order_field.ins, InstrumentID, sizeof(order_field.ins));	
	order_field.offset = OffsetFlag;
	order_field.volume = Volume;
	order_field.volumeCondition = HXTS_VC_AV;
	order_field.priceType = HXTS_OPT_LimitPrice;
	order_field.timeCondition = HXTS_TC_GFD;
	order_field.price = LimitPrice;
		
	m_seat_ind++;
	order_field.seat_id = m_seat_ind % m_SeatCount;
	order_field.OrderRef = ++m_MaxOrderRef;

	pTraderApi->ReqOrderInsert(&order_field);


}

void CHxtsTestHandler::OrderAction()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;

  char OrderSysID[32];
  
  printf("input OrderSysID:\n");
  scanf("%s", OrderSysID);

	req_action_order_field_s cancel_order;
	cancel_order.orderNO = 0;
	strncpy(cancel_order.sysID, OrderSysID, sizeof(cancel_order.sysID));
	pTraderApi->ReqCancelOrder(&cancel_order);

}

void CHxtsTestHandler::QryOrder()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	pTraderApi->ReqQryOrder("", "", 0);
  return;
}
void CHxtsTestHandler::QryTrade()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	pTraderApi->ReqQryTrade(NULL, NULL, 0);
  return;
}

void CHxtsTestHandler::QryInvestorAccount()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	pTraderApi->ReqQryFund();
  return;
}

void CHxtsTestHandler::QryInstrument()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	pTraderApi->ReqQryIns("");
  return;
}

void CHxtsTestHandler::QryInvestorPosition()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
	pTraderApi->ReqQryPos(NULL);
  return;
}

void CHxtsTestHandler::PrintTrade(void* data)
{
  rtn_trade_field_s* pTrade = (rtn_trade_field_s*)data;

  HXTS_TEST_LOG(
    "pTrade->ins=[%s]\n"
    "pTrade->dir=[%c]\n"
    "pTrade->offset=[%c]\n"
    "pTrade->sysID=[%s]\n"
    "pTrade->tradeID=[%s]\n"
    "pTrade->orderNO=[%d]\n"
    "pTrade->orderRef=[%d]\n"
    "pTrade->tradedPrice=[%lf]\n"
    "pTrade->tradedVolume=[%d]\n"
    "pTrade->tradedTime=[%s]\n"
    "pTrade->feeMoney=[%lf]\n"
    , pTrade->ins
    , pTrade->dir
    , pTrade->offset
    , pTrade->sysID
    , pTrade->tradeID
    , pTrade->orderNO
    , pTrade->orderRef
    , pTrade->tradedPrice
    , pTrade->tradedVolume
    , pTrade->tradedTime
    , pTrade->feeMoney
  );


}

void CHxtsTestHandler::ChangePassword()
{
  CHxTraderApi* pTraderApi = (CHxTraderApi*)m_TraderApi;
  char Temp[64];
  printf("input Password:\n");
  scanf("%s", Temp);
  req_change_pwd_field_s req_change_pwd;
  strcpy(req_change_pwd.szAccount, m_UserId);
  strcpy(req_change_pwd.szOldPwd, m_Passwd);
  strcpy(req_change_pwd.szNewPwd, Temp);
  pTraderApi->ReqChangePwd(&req_change_pwd);
  return;
}


