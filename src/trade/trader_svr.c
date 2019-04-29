
#include "trader_svr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <limits.h>
#include <float.h>

#include <unistd.h>
#include<dirent.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "cmn_util.h"

#include "cmn_log.h"

#include "cmn_cfg.h"
#include "glbProfile.h"

#include "ctp_trader_api.h"
#include "trader_mduser_api.h"

#include "trader_data.h"
#include "trader_msg_struct.h"
#include "trader_db.h"

#include "trader_strategy.h"
#include "trader_strategy_engine.h"

#include "trader_mduser_client.h"

#include "hiredis.h"

static int trader_svr_init(trader_svr* self, evutil_socket_t sock);
static int trader_svr_param_ini(trader_svr* self, char* cfg_file);
static int trader_svr_run(trader_svr* self);
static int trader_svr_exit(trader_svr* self);
static int trader_svr_on_client_recv(trader_svr* self);
static int trader_svr_on_client_send(trader_svr* self);
static int trader_svr_on_trader_recv(trader_svr* self);
static int trader_svr_on_mduser_recv(trader_svr* self);
static trader_svr_method* trader_svr_method_get();

static void trader_svr_mduser_read_cb(struct bufferevent *bev, void *arg);
static void trader_svr_mduser_evt_cb(struct bufferevent *bev, short event, void *arg);
static void trader_svr_trader_read_cb(struct bufferevent *bev, void *arg);
static void trader_svr_trader_evt_cb(struct bufferevent *bev, short event, void *arg);
static void trader_svr_client_read_cb(struct bufferevent *bev, void *arg);
static void trader_svr_client_write_cb(struct bufferevent *bev, void *arg);
static void trader_svr_client_evt_cb(struct bufferevent *bev, short event, void *arg);
static void trader_svr_signal_cb(evutil_socket_t fd, short event, void *arg);

static int trader_svr_proc_client_logout(trader_svr* self);
static int trader_svr_proc_client_login(trader_svr* self, trader_msg_req_struct* req);
static int trader_svr_proc_client_update(trader_svr* self, trader_msg_req_struct* req);
static int trader_svr_proc_client_query_instruments(trader_svr* self, trader_msg_req_struct* req);
static int trader_svr_proc_client_query_positions(trader_svr* self, trader_msg_req_struct* req);
static int trader_svr_proc_client_close_all(trader_svr* self, trader_msg_req_struct* req);
static int trader_svr_proc_client_reset_positions(trader_svr* self, trader_msg_req_struct* req);

static int trader_svr_proc_mduser(trader_svr* self, trader_msg_mduser_struct* msg);
static int trader_svr_proc_trader(trader_svr* self, trader_msg_trader_struct* msg);
static int trader_svr_proc_trader2(trader_svr* self, trader_trader_evt* msg);


static int trader_svr_client_notify_login(trader_svr* self, int err_cd, char* error_msg);

static int trader_svr_chdir(trader_svr* self, char* user_id);

static int trader_svr_check_instrument(trader_svr* self, char* inst);

static int trader_svr_redis_init(trader_svr* self);
static int trader_svr_redis_fini(trader_svr* self);

static void trader_svr_mduser_client_connect_callback(void* user_data);
static void trader_svr_mduser_client_disconnect_callback(void* user_data);
static void trader_svr_mduser_client_recv_callback(void* user_data, void* data, int len);


int trader_svr_init(trader_svr* self, evutil_socket_t sock)
{
  CMN_DEBUG("Enter!\n");
  int nRet;
  // 各种初始化
  CMN_DEBUG("self->pBase\n");
  self->pBase = event_base_new();
  CMN_ASSERT(self->pBase);
  
  //signal event init
  CMN_DEBUG("self->pSigIntEvt\n");
  self->pSigIntEvt = evsignal_new(self->pBase, SIGINT, trader_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigIntEvt);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  self->pSigTermEvt = evsignal_new(self->pBase, SIGTERM, trader_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigTermEvt);
  
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(nRet >= 0);

  // self->BufClient
  CMN_DEBUG("self->BufClient\n");
  self->BufClient = bufferevent_socket_new(self->pBase, sock, 0);
  bufferevent_setcb(self->BufClient, trader_svr_client_read_cb, 
    trader_svr_client_write_cb, trader_svr_client_evt_cb, (void*)self);
  bufferevent_enable(self->BufClient, EV_READ|EV_PERSIST);
  
  evutil_socket_t pair[2];
  
  //self->BufTrader
  CMN_DEBUG("self->BufTrader\n");
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);
  CMN_ASSERT(nRet >= 0);

  self->BufTrader = bufferevent_socket_new(self->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(self->BufTrader, trader_svr_trader_read_cb, 
    NULL, trader_svr_trader_evt_cb, (void*)self);
  bufferevent_enable(self->BufTrader, EV_READ|EV_PERSIST);

  CMN_DEBUG("self->pCtpTraderApi\n");
  /*
  self->pCtpTraderApi = ctp_trader_api_new(pair[1]);
  self->pCtpTraderApi->BrokenId = self->BrokerId;
  self->pCtpTraderApi->FrontAddress = self->TrFrontAdd;
  self->pCtpTraderApi->HedgeFlag= self->cHedgeFlag;
  */
  CMN_DEBUG("BrokenId[%s]\n", self->BrokerId);
  CMN_DEBUG("FrontAddress[%s]\n", self->TrFrontAdd);
  
  CMN_DEBUG("self->pCtpTraderApi\n");
#ifdef LTS
  //LTS
#include "trader_trader_api_lts.h"
  self->pCtpTraderApi = trader_trader_api_new(pair[1], trader_trader_api_lts_method_get());
#endif
  
#ifdef CTP
    //CTP
#include "trader_trader_api_ctp.h"
  self->pCtpTraderApi = trader_trader_api_new(pair[1], trader_trader_api_ctp_method_get());
#endif
  
#ifdef FEMAS
    //FEMAS
#include "trader_trader_api_femas.h"
  self->pCtpTraderApi = trader_trader_api_new(pair[1], trader_trader_api_femas_method_get());
#endif
  self->pCtpTraderApi->pMethod->xSetFrontAddr(self->pCtpTraderApi, self->TrFrontAdd);

  //self->BufMduser
  /*
  CMN_DEBUG("self->BufMduser\n");
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);
  CMN_ASSERT(nRet >= 0);
  
  self->BufMduser = bufferevent_socket_new(self->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(self->BufMduser, trader_svr_mduser_read_cb, 
    NULL, trader_svr_mduser_evt_cb, (void*)self);
  bufferevent_enable(self->BufMduser, EV_READ|EV_PERSIST);

  CMN_DEBUG("self->pCtpMduserApi\n");
  self->pCtpMduserApi = ctp_mduser_api_new(pair[1]);
  self->pCtpMduserApi->TopicId = 100; // 行情Topic固定为100
  self->pCtpMduserApi->BrokenId = self->BrokerId;
  self->pCtpMduserApi->FrontAddress = self->MdFrontAdd;
  CMN_DEBUG("TopicId[%d]\n", self->pCtpMduserApi->TopicId);
  CMN_DEBUG("BrokenId[%s]\n", self->pCtpMduserApi->BrokenId);
  CMN_DEBUG("FrontAddress[%s]\n", self->pCtpMduserApi->FrontAddress);
  */

  nRet = trader_svr_redis_init(self);
  CMN_ASSERT(nRet == 0);

  CMN_DEBUG("self->pMduserClt\n");
  self->pMduserClt = trader_mduser_client_new();
  self->cacheLen = 0;
  
  CMN_DEBUG("boardcastAddr[%s]\n", self->boardcastAddr);
  CMN_DEBUG("boardcastPort[%d]\n", self->boardcastPort);

  self->pMduserClt->method->xInit(self->pMduserClt, self->pBase,
    self->boardcastAddr, self->boardcastPort,
    trader_svr_mduser_client_connect_callback,
    trader_svr_mduser_client_disconnect_callback,
    trader_svr_mduser_client_recv_callback,
    self
    );

  CMN_DEBUG("self->pTraderDB\n");
  self->pTraderDB = trader_db_new(TRADER_DB_NAME);

  CMN_DEBUG("self->pStrategyEngine\n");
  self->pStrategyEngine = trader_strategy_engine_new();
  self->pStrategyEngine->nPositionSize = self->nPositionSize;
  self->pStrategyEngine->pBase = self->pBase;
  self->pStrategyEngine->pCtpTraderApi = self->pCtpTraderApi;
  self->pStrategyEngine->pTraderDb = self->pTraderDB;

  self->nContractNum = 0;
  TAILQ_INIT(&self->listTraderContract);
  
  return 0;

}

int trader_svr_run(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");

  self->pMduserClt->method->xConnect(self->pMduserClt);

  int nRet = event_base_dispatch(self->pBase);
  
  self->pMduserClt->method->xExit(self->pMduserClt);

  return nRet;

}

int trader_svr_exit(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");
  
  if(self->pMduserClt) {
    trader_mduser_client_free(self->pMduserClt);
  }
  /*
  if(self->pCtpMduserApi) {
    ctp_mduser_api_free(self->pCtpMduserApi);
  }

  if(self->BufMduser){
    bufferevent_free(self->BufMduser);
  }
  */
  
  trader_svr_redis_fini(self);
  
  if(self->pCtpTraderApi) {
    //ctp_trader_api_free(self->pCtpTraderApi);
    trader_trader_api_free(self->pCtpTraderApi);
  }

  if(self->pStrategyEngine){
    trader_strategy_engine_free(self->pStrategyEngine);
  }
  
  if(self->BufTrader){
    bufferevent_free(self->BufTrader);
  }

  if(self->BufClient){
    bufferevent_free(self->BufClient);
  }

  if(self->pTraderDB){
    // 数据库释放
    self->pTraderDB->pMethod->xFini(self->pTraderDB);
    trader_db_free(self->pTraderDB);
  }

  event_free(self->pSigIntEvt);
  event_free(self->pSigTermEvt);
  event_base_free(self->pBase);
  
  return 0;
}

int trader_svr_param_ini(trader_svr* self, char* cfg_file)
{
  int nRet;
  cmn_cfg* pCfg = cmn_cfg_new(cfg_file);
  nRet = pCfg->pMethod->xInit(pCfg);
  CMN_ASSERT(!nRet);

  self->nPositionSize = pCfg->pMethod->xGetPosLimit(pCfg);

  self->BrokerId = (char*)malloc(sizeof(char) * (strlen(pCfg->pMethod->xGetBrokerId(pCfg)) + 1));
  strcpy(self->BrokerId, pCfg->pMethod->xGetBrokerId(pCfg));
  CMN_DEBUG("self->BrokerId[%s]\n", self->BrokerId);
  
  self->MdPrefix = (char*)malloc(sizeof(char) * (strlen(pCfg->pMethod->xGetMdPrefix(pCfg)) + 1));
  strcpy(self->MdPrefix, pCfg->pMethod->xGetMdPrefix(pCfg));
  CMN_DEBUG("self->MdPrefix[%s]\n", self->MdPrefix);

  self->MdFrontAdd = (char*)malloc(sizeof(char) * (strlen(pCfg->pMethod->xGetMdFrontAdd(pCfg)) + 1));
  strcpy(self->MdFrontAdd, pCfg->pMethod->xGetMdFrontAdd(pCfg));
  CMN_DEBUG("self->MdFrontAdd[%s]\n", self->MdFrontAdd);

  self->TrFrontAdd = (char*)malloc(sizeof(char) * (strlen(pCfg->pMethod->xGetTrFrontAdd(pCfg)) + 1));
  strcpy(self->TrFrontAdd, pCfg->pMethod->xGetTrFrontAdd(pCfg));
  CMN_DEBUG("self->TrFrontAdd[%s]\n", self->TrFrontAdd);

  self->WorkSpace = (char*)malloc(sizeof(char) * (strlen(pCfg->pMethod->xGetWorkspace(pCfg)) + 1));
  strcpy(self->WorkSpace, pCfg->pMethod->xGetWorkspace(pCfg));
  CMN_DEBUG("self->WorkSpace[%s]\n", self->WorkSpace);

  self->nStoreMduser = pCfg->pMethod->xGetStoreMduser(pCfg);

  self->cHedgeFlag = pCfg->pMethod->xGetHedgeFlag(pCfg);
  cmn_cfg_free(pCfg);
  
  nRet = glbPflGetString("MDUSER", "BOARDCAST_ADDR", cfg_file, self->boardcastAddr);
  nRet = glbPflGetInt("MDUSER", "BOARDCAST_PORT", cfg_file, &self->boardcastPort);
  
  char sTimeCondition[2];
  nRet = glbPflGetString("TRADER", "TIME_CONDITION", cfg_file, sTimeCondition);
  if(nRet){
    self->cTimeCondition = sTimeCondition[0];
  }

  return 0;
}

int trader_svr_on_client_recv(trader_svr* self)
{
  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_msg_req_struct oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(self->BufClient, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }
    switch(oMsg.cmd){
    case TRADER_CMD_TYPE_LOGIN:
      if(self->bProcessing){
        //重复登录
        trader_msg_rsp_struct oRsp;
        oRsp.cmd = TRADER_CMD_TYPE_LOGIN;
        strcpy(oRsp.traceNo, oMsg.traceNo);
        oRsp.req = oMsg.req;
        oRsp.ErrorID = -30000;
        strcpy(&oRsp.ErrorMsg[0], "登录中，请稍候");
        
        unsigned char* pPkg = (unsigned char*)&oRsp;
        int nPkgSize = offsetof(trader_msg_rsp_struct, recNum);
        
        cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);
      }else{
        self->bProcessing = 1;
        trader_svr_proc_client_login(self, &oMsg);
      }
      break;
    case TRADER_CMD_TYPE_UPDATE:
      trader_svr_proc_client_update(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_UPDATE_ALL:
      trader_svr_proc_client_update(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_QUERY_INSTRUMENT:
      trader_svr_proc_client_query_instruments(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_QUERY_POSITION:
      trader_svr_proc_client_query_positions(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_CLOSE_ALL:
      trader_svr_proc_client_close_all(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_RESET_POSITION:
      trader_svr_proc_client_reset_positions(self, &oMsg);
      break;
    case TRADER_CMD_TYPE_LOGOUT:
      trader_svr_proc_client_logout(self);
      break;
    }
  }while(loop);

  return 0;
}

int trader_svr_on_client_send(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");
  return 0;
}

int trader_svr_on_trader_recv(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");
  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  //trader_msg_trader_struct oMsg;
  trader_trader_evt oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(self->BufTrader, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    nRet = trader_svr_proc_trader2(self, &oMsg);
    
  }while(loop);

  return 0;

}

int trader_svr_on_mduser_recv(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");
  /*
  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_msg_mduser_struct oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(self->BufMduser, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    nRet = trader_svr_proc_mduser(self, &oMsg);
  }while(loop);
  */

  return 0;

}

int trader_svr_proc_mduser(trader_svr* self, trader_msg_mduser_struct* msg)
{
  CMN_DEBUG("Enter!\n");
  trader_msg_mduser_struct* pMsg = msg;
  trader_tick* pTick = &(pMsg->oTick);
  switch(pMsg->type){
  case TRADER_RECV_MDUSER_TYPE_LOGIN:
    CMN_DEBUG("登录应答!\n");
    if(!pMsg->ErrorCd){
      // 订阅行情
      /*
      trader_contract* pTraderContract;
      TAILQ_FOREACH(pTraderContract, &self->listTraderContract, next){
        self->pCtpMduserApi->pMethod->xSubscribe(self->pCtpMduserApi,
          pTraderContract->contract);
      }
      */

      // 查询持仓
      sleep(1);
      self->pCtpTraderApi->pMethod->xQryInvestorPosition(self->pCtpTraderApi);
    }
    
    trader_svr_client_notify_login(self, pMsg->ErrorCd, pMsg->ErrorMsg);
    break;
  case TRADER_RECV_MDUSER_TYPE_SUB:

    break;
  case TRADER_RECV_MDUSER_TYPE_INFO:
    if((DBL_MAX == pTick->BidPrice1)
    ||(DBL_MAX == pTick->AskPrice1)){
      CMN_DEBUG("无效的行情数据!\n");
      return 0;
    }

    if(self->nStoreMduser){
      CMN_DEBUG("更新数据库!\n");
      int errCd = 0;
      int nRet = 0;
      struct trader_db_mduser_inf_def dbMduser;

      strcpy(dbMduser.InstrumentID, pTick->InstrumentID);
      strcpy(dbMduser.TradingDay, pTick->TradingDay);
      strcpy(dbMduser.UpdateTime, pTick->UpdateTime);
      dbMduser.UpdateMillisec = pTick->UpdateMillisec;
      dbMduser.BidPrice1 = pTick->BidPrice1;
      dbMduser.BidVolume1 = pTick->BidVolume1;
      dbMduser.AskPrice1 = pTick->AskPrice1;
      dbMduser.AskVolume1 = pTick->AskVolume1;
      nRet = self->pTraderDB->pMethod->xMduserInsert(self->pTraderDB, &dbMduser, &errCd);
      if(!nRet){
        CMN_ERROR("xMduserInsert ErrCd[%d]\n", errCd);
      }
    }
    
    CMN_DEBUG("通知策略引擎行情更新!\n");
    self->pStrategyEngine->pMethod->xUpdateTick(self->pStrategyEngine, &pMsg->oTick);

    break;
  case TRADER_RECV_MDUSER_TYPE_ERROR:
    CMN_ERROR("行情出错!\n");
    // TODO
    
    break;
  }


  return 0;

}

int trader_svr_proc_trader(trader_svr* self, trader_msg_trader_struct* msg)
{
  CMN_DEBUG("Enter!\n");
  trader_msg_trader_struct* pMsg = msg;
  trader_order* pOrder = &(pMsg->oOrder);
  trader_trade* pTrade = &(pMsg->oTrade);
  investor_position* pInvestorPosition = &(pMsg->oPosition);
  int bFound;  
  trader_contract* iter;
  
  switch(pMsg->type){
  case TRADER_RECV_TRADER_ON_LOGIN:    
    if(pMsg->ErrorCd){
      // 登陆失败
      trader_svr_client_notify_login(self, pMsg->ErrorCd, pMsg->ErrorMsg);
      return -1;
    }
    
    // 查询交易日期
    char sTradingDay[9];
    self->pCtpTraderApi->pMethod->xGetTradingDay(self->pCtpTraderApi, sTradingDay);
    CMN_DEBUG("sTradingDay[%s]\n", sTradingDay);
    self->pStrategyEngine->pMethod->xTradingDaySet(self->pStrategyEngine, sTradingDay);

    
    // 设置MaxUserOrderLocalID
    long sMaxOrderLocalID = 0;
    self->pCtpTraderApi->pMethod->xGetMaxOrderLocalID(self->pCtpTraderApi, &sMaxOrderLocalID);
    CMN_DEBUG("sMaxOrderLocalID[%s]\n", sMaxOrderLocalID);
    self->pStrategyEngine->pMethod->xLocalUserIdSet(self->pStrategyEngine, sMaxOrderLocalID);

    if(0 != strcmp(sTradingDay, self->TradingDay)){
      strcpy(self->TradingDay, sTradingDay);
      self->bQueried = 0;
    }

    // 查询合约
    self->pCtpTraderApi->pMethod->xQryInstrument(self->pCtpTraderApi);
    break;
  case TRADER_RECV_TRADER_ON_QRY_INVESTOR_POSITION:
    pInvestorPosition->IsSHFE = 0;
    TAILQ_FOREACH(iter, &self->listTraderContract, next){
      CMN_DEBUG("pInvestorPosition->InstrumentID[%s]"
        "iter->contract[%s]"
        "iter->isSHFE[%d]\n",
        pInvestorPosition->InstrumentID,
        iter->contract,
        iter->isSHFE
      );
      if(!strcmp(pInvestorPosition->InstrumentID, iter->contract)){
        pInvestorPosition->IsSHFE = iter->isSHFE;
        break;
      }
    }

    CMN_INFO("pInvestorPosition->InstrumentID[%s]\n"
      "pInvestorPosition->PosiDirection[%c]\n"
      "pInvestorPosition->IsSHFE[%d]\n"
      "pInvestorPosition->PositionDate[%c]\n"
      "pInvestorPosition->YdPosition[%d]\n"
      "pInvestorPosition->TodayPosition[%d]\n"
      "pInvestorPosition->Position[%d]\n"
      "pInvestorPosition->LongFrozen[%d]\n",
      pInvestorPosition->InstrumentID,
      pInvestorPosition->PosiDirection,
      pInvestorPosition->IsSHFE,
      pInvestorPosition->PositionDate,
      pInvestorPosition->YdPosition,
      pInvestorPosition->TodayPosition,
      pInvestorPosition->Position,
      pInvestorPosition->LongFrozen
    );
    
    self->pStrategyEngine->pMethod->xInitInvestorPosition(self->pStrategyEngine, &pMsg->oPosition);
    if(pMsg->bIsLast){

    }
    break;
  case TRADER_RECV_TRADER_ON_QRY_USER_INVESTOR:
      self->bQueried = 1;
      // 通知登陆成功
      trader_svr_client_notify_login(self, 0, "SUCCESS");
      
      // 查询持仓
      sleep(1);
      self->pCtpTraderApi->pMethod->xQryInvestorPosition(self->pCtpTraderApi);

    break;
  case TRADER_RECV_TRADER_ON_QRY_INSTRUMENT:
    if(!pMsg->ErrorCd){
      bFound = trader_svr_check_instrument(self, pMsg->InstrumentID);
      if( bFound ){
        // 添加到行情列表中
        trader_contract* pTraderContract = (trader_contract*)malloc(sizeof(trader_contract));
        strcpy(pTraderContract->contract,  pMsg->InstrumentID);
        strcpy(pTraderContract->ExchangeID,  pMsg->ExchangeID);
        pTraderContract->PriceTick =  pMsg->PriceTick;
        pTraderContract->nCancelNum = 0;
        pTraderContract->isSHFE = 0;
        if(!strcmp(pMsg->ExchangeID, "SHFE")
        ||!strcmp(pMsg->ExchangeID, "INE")){
          pTraderContract->isSHFE = 1;
        }        
        self->nContractNum++;
        
        CMN_DEBUG("pMsg->InstrumentID[%s]\n", pMsg->InstrumentID);
        CMN_DEBUG("pMsg->ExchangeID[%s]\n", pMsg->ExchangeID);
        CMN_DEBUG("pMsg->isSHFE[%d]\n", pTraderContract->isSHFE);
        
        TAILQ_INSERT_TAIL(&self->listTraderContract, pTraderContract, next);
      }
    }

    if(pMsg->bIsLast){
      // 查询投资者编号
      sleep(1);
      self->pCtpTraderApi->pMethod->xQryUserInvestor(self->pCtpTraderApi);
    }

    break;
  case TRADER_RECV_TRADER_ON_ORDER:
    CMN_INFO("订单变化!\n");
    CMN_INFO("pOrder->ExchangeID[%s]\n", pOrder->ExchangeID);
    CMN_INFO("pOrder->OrderSysID[%s]\n", pOrder->OrderSysID);
    CMN_INFO("pOrder->InstrumentID[%s]\n", pOrder->InstrumentID);
    CMN_INFO("pOrder->UserOrderLocalID[%s]\n", pOrder->UserOrderLocalID);
    CMN_INFO("pOrder->Direction[%c]\n", pOrder->Direction);
    CMN_INFO("pOrder->OffsetFlag[%c]\n", pOrder->OffsetFlag);
    CMN_INFO("pOrder->HedgeFlag[%c]\n", pOrder->HedgeFlag);
    CMN_INFO("pOrder->LimitPrice[%lf]\n", pOrder->LimitPrice);
    CMN_INFO("pOrder->VolumeOriginal[%d]\n", pOrder->VolumeOriginal);
    CMN_INFO("pOrder->VolumeTraded[%d]\n", pOrder->VolumeTraded);
    CMN_INFO("pOrder->OrderStatus[%c]\n", pOrder->OrderStatus);
    CMN_INFO("pOrder->InsertTime[%s]\n", pOrder->InsertTime);

    // 更新撤单笔数
    if(TRADER_ORDER_OS_CANCELED == pOrder->OrderStatus){
      trader_contract* iter;
      TAILQ_FOREACH(iter, &self->listTraderContract, next){
        if(!strcmp(pOrder->InstrumentID, iter->contract)){
          iter->nCancelNum++;
          break;
        }
      }
    }
    
    self->pStrategyEngine->pMethod->xUpdateOrder(self->pStrategyEngine, &pMsg->oOrder);
    break;
  case TRADER_RECV_TRADER_ON_TRADE:
    
    CMN_INFO("成交回报!\n");
    CMN_INFO("InstrumentID[%s]\n", pTrade->InstrumentID);
    CMN_INFO("UserOrderLocalID[%s]\n", pTrade->UserOrderLocalID);
    CMN_INFO("TradingDay[%s]\n", pTrade->TradingDay);
    CMN_INFO("TradeTime[%s]\n", pTrade->TradeTime);
    CMN_INFO("Direction[%c]\n", pTrade->Direction);
    CMN_INFO("OffsetFlag[%c]\n", pTrade->OffsetFlag);
    CMN_INFO("TradePrice[%lf]\n", pTrade->TradePrice);
    CMN_INFO("TradeVolume[%d]\n", pTrade->TradeVolume);
    CMN_INFO("TradeID[%s]\n", pTrade->TradeID);

    self->pStrategyEngine->pMethod->xUpdateTrade(self->pStrategyEngine, &pMsg->oTrade);
    break;
  case TRADER_RECV_TRADER_ORDER_ERROR:
    CMN_ERROR("交易出错!\n");
    // TODO
    
    break;
  }
}

int trader_svr_proc_trader2(trader_svr* self, trader_trader_evt* msg)
{
  int nType = msg->Type;
  trader_trader_evt* pMsg = msg;
  int bFound;
  trader_contract* iter;
  
  trader_instrument* pInstrument = &(pMsg->Body.InstrumentRsp);
  trader_position* pInvestorPosition = &(pMsg->Body.PositionRsp);
  investor_position traderPosition;
  trader_order* pOrder = &(pMsg->Body.OrderRsp);
  trader_trade* pTrade = &(pMsg->Body.TradeRsp);

  switch(nType){
  case TRADERONFRONTCONNECTED:    
    CMN_DEBUG("trader connected!\n");
    if(self->bProcessing){      
      self->pCtpTraderApi->pMethod->xLogin(self->pCtpTraderApi);
    }
    break;
  case TRADERONFRONTDISCONNECTED:
    CMN_DEBUG("trader disconnected!\n");
    
    break;
  case TRADERONRSPUSERLOGOUT:    
    CMN_DEBUG("trader disconnected!\n");
    self->bProcessing = 0;    
    self->pCtpTraderApi->pMethod->xStop(self->pCtpTraderApi);
    break;
  case TRADERONRSPUSERLOGIN:
    if(pMsg->ErrorCd){
      // 登陆失败
      trader_svr_client_notify_login(self, pMsg->ErrorCd, pMsg->ErrorMsg);
      self->bProcessing = 0;
      return -1;
    }
    
    // 查询交易日期
    char sTradingDay[9];
    self->pCtpTraderApi->pMethod->xGetTradingDay(self->pCtpTraderApi, sTradingDay);
    CMN_DEBUG("sTradingDay[%s]\n", sTradingDay);
    self->pStrategyEngine->pMethod->xTradingDaySet(self->pStrategyEngine, sTradingDay);

    
    // 设置MaxUserOrderLocalID
    long sMaxOrderLocalID = 0;
    self->pCtpTraderApi->pMethod->xGetMaxOrderLocalID(self->pCtpTraderApi, &sMaxOrderLocalID);
    CMN_DEBUG("sMaxOrderLocalID[%ld]\n", sMaxOrderLocalID);
    self->pStrategyEngine->pMethod->xLocalUserIdSet(self->pStrategyEngine, sMaxOrderLocalID);

    if(0 != strcmp(sTradingDay, self->TradingDay)){
      strcpy(self->TradingDay, sTradingDay);
      self->bQueried = 0;
    }

    // 查询合约
    self->pCtpTraderApi->pMethod->xQryInstrument(self->pCtpTraderApi);
    break;
  case TRADERONRSPQRYINVESTOR:
    self->bQueried = 1;
    // 通知登陆成功
    trader_svr_client_notify_login(self, 0, "SUCCESS");
    
    // 查询持仓
    sleep(1);
    self->pCtpTraderApi->pMethod->xQryInvestorPosition(self->pCtpTraderApi);
    break;
  case TRADERONRSPQRYINSTRUMENT:
    if(!pMsg->ErrorCd){
      bFound = trader_svr_check_instrument(self, pInstrument->InstrumentID);
      if( bFound ){
        // 添加到行情列表中
        trader_contract* pTraderContract = (trader_contract*)malloc(sizeof(trader_contract));
        strcpy(pTraderContract->contract,  pInstrument->InstrumentID);
        strcpy(pTraderContract->ExchangeID,  pInstrument->ExchangeID);
        pTraderContract->PriceTick =  pInstrument->PriceTick;
        pTraderContract->nCancelNum = 0;
        pTraderContract->isSHFE = 0;
        if(!strcmp(pInstrument->ExchangeID, "SHFE")
        ||!strcmp(pInstrument->ExchangeID, "INE")){
          pTraderContract->isSHFE = 1;
        }        
        self->nContractNum++;
        
        CMN_DEBUG("pMsg->InstrumentID[%s]\n", pInstrument->InstrumentID);
        CMN_DEBUG("pMsg->ExchangeID[%s]\n", pInstrument->ExchangeID);
        CMN_DEBUG("pMsg->isSHFE[%d]\n", pTraderContract->isSHFE);
        
        TAILQ_INSERT_TAIL(&self->listTraderContract, pTraderContract, next);
      }
    }

    if(pMsg->IsLast){
      // 查询投资者编号
      sleep(1);
      self->pCtpTraderApi->pMethod->xQryUserInvestor(self->pCtpTraderApi);
    }
    break;
  case TRADERONRSPQRYINVESTORPOSITION:
    pInvestorPosition->IsSHFE = 0;
    TAILQ_FOREACH(iter, &self->listTraderContract, next){
      CMN_DEBUG("pInvestorPosition->InstrumentID[%s]"
        "iter->contract[%s]"
        "iter->isSHFE[%d]\n",
        pInvestorPosition->InstrumentID,
        iter->contract,
        iter->isSHFE
      );
      if(!strcmp(pInvestorPosition->InstrumentID, iter->contract)){
        pInvestorPosition->IsSHFE = iter->isSHFE;
        break;
      }
    }

    CMN_INFO("pInvestorPosition->InstrumentID[%s]\n"
      "pInvestorPosition->PosiDirection[%c]\n"
      "pInvestorPosition->IsSHFE[%d]\n"
      "pInvestorPosition->PositionDate[%c]\n"
      "pInvestorPosition->YdPosition[%d]\n"
      "pInvestorPosition->TodayPosition[%d]\n"
      "pInvestorPosition->Position[%d]\n"
      "pInvestorPosition->LongFrozen[%d]\n",
      pInvestorPosition->InstrumentID,
      pInvestorPosition->PosiDirection,
      pInvestorPosition->IsSHFE,
      pInvestorPosition->PositionDate,
      pInvestorPosition->YdPosition,
      pInvestorPosition->TodayPosition,
      pInvestorPosition->Position,
      pInvestorPosition->LongFrozen
    );

    strcpy(traderPosition.InstrumentID, pInvestorPosition->InstrumentID);
    traderPosition.PosiDirection = pInvestorPosition->PosiDirection;
    traderPosition.IsSHFE = pInvestorPosition->IsSHFE;
    traderPosition.PositionDate = pInvestorPosition->PositionDate;
    traderPosition.YdPosition = pInvestorPosition->YdPosition;
    traderPosition.TodayPosition = pInvestorPosition->TodayPosition;
    traderPosition.Position = pInvestorPosition->Position;
    traderPosition.LongFrozen = pInvestorPosition->LongFrozen;
    
    self->pStrategyEngine->pMethod->xInitInvestorPosition(self->pStrategyEngine, &traderPosition);

    if(pMsg->IsLast){

    }
    break;
  case TRADERONRTNORDER:
    
    CMN_INFO("订单变化!\n");
    CMN_INFO("pOrder->ExchangeID[%s]\n", pOrder->ExchangeID);
    CMN_INFO("pOrder->OrderSysID[%s]\n", pOrder->OrderSysID);
    CMN_INFO("pOrder->InstrumentID[%s]\n", pOrder->InstrumentID);
    CMN_INFO("pOrder->UserOrderLocalID[%s]\n", pOrder->UserOrderLocalID);
    CMN_INFO("pOrder->Direction[%c]\n", pOrder->Direction);
    CMN_INFO("pOrder->OffsetFlag[%c]\n", pOrder->OffsetFlag);
    CMN_INFO("pOrder->HedgeFlag[%c]\n", pOrder->HedgeFlag);
    CMN_INFO("pOrder->LimitPrice[%lf]\n", pOrder->LimitPrice);
    CMN_INFO("pOrder->VolumeOriginal[%d]\n", pOrder->VolumeOriginal);
    CMN_INFO("pOrder->VolumeTraded[%d]\n", pOrder->VolumeTraded);
    CMN_INFO("pOrder->OrderStatus[%c]\n", pOrder->OrderStatus);
    CMN_INFO("pOrder->InsertTime[%s]\n", pOrder->InsertTime);

    // 更新撤单笔数
    if(TRADER_ORDER_OS_CANCELED == pOrder->OrderStatus){
      trader_contract* iter;
      TAILQ_FOREACH(iter, &self->listTraderContract, next){
        if(!strcmp(pOrder->InstrumentID, iter->contract)){
          iter->nCancelNum++;
          break;
        }
      }
    }
    
    self->pStrategyEngine->pMethod->xUpdateOrder(self->pStrategyEngine, pOrder);
    break;
  case TRADERONRTNTRADE:
    
    CMN_INFO("成交回报!\n");
    CMN_INFO("InstrumentID[%s]\n", pTrade->InstrumentID);
    CMN_INFO("UserOrderLocalID[%s]\n", pTrade->UserOrderLocalID);
    CMN_INFO("TradingDay[%s]\n", pTrade->TradingDay);
    CMN_INFO("TradeTime[%s]\n", pTrade->TradeTime);
    CMN_INFO("Direction[%c]\n", pTrade->Direction);
    CMN_INFO("OffsetFlag[%c]\n", pTrade->OffsetFlag);
    CMN_INFO("TradePrice[%lf]\n", pTrade->TradePrice);
    CMN_INFO("TradeVolume[%d]\n", pTrade->TradeVolume);
    CMN_INFO("TradeID[%s]\n", pTrade->TradeID);

    self->pStrategyEngine->pMethod->xUpdateTrade(self->pStrategyEngine, pTrade);
    break;
  case TRADERONRSPERROR:
    CMN_ERROR("交易出错!\n");
    // TODO
    break;
  default:
    CMN_ERROR("nType[%d]\n", nType);
    // TODO
    break;
  }

}


int trader_svr_proc_client_login(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  struct trader_cmd_login_req_def* pLoginInf = &(req->body.login);
  CMN_DEBUG("UserID=[%s]\n", pLoginInf->UserID);
  
  // 参数保存
  strcpy(&self->CurrentTraceNo[0], req->traceNo);
  strcpy(&self->UserId[0], pLoginInf->UserID);
  strcpy(&self->Passwd[0], pLoginInf->Password);
  self->pReqMsg = req->req;

  // 进入工作目录
  trader_svr_chdir(self, pLoginInf->UserID);

  // 数据库初始化
  time_t tt = time(NULL);
  struct tm now;
  localtime_r(&tt, &now);
  char sDate[9];
  sprintf(sDate, "%04d%02d%02d",
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  
  self->pTraderDB->pMethod->xInit(self->pTraderDB, sDate);

  // 交易登录
  
  self->pCtpTraderApi->pMethod->xSetWorkspace(self->pCtpTraderApi, ".");
  self->pCtpTraderApi->pMethod->xSetUser(self->pCtpTraderApi, self->BrokerId, 
    &self->UserId[0], &self->Passwd[0]);
  
  self->pCtpTraderApi->pMethod->xStart(self->pCtpTraderApi);
  /*
  self->pCtpTraderApi->pMethod->xStart(self->pCtpTraderApi,
    &self->UserId[0], &self->Passwd[0]);
  */
  return 0;
}

int trader_svr_proc_client_logout(trader_svr* self)
{
  CMN_DEBUG("Enter!\n");

  // 特殊处理
  self->pStrategyEngine->pMethod->xCloseAll(self->pStrategyEngine);
  
  // 行情退出
  /*
  self->pCtpMduserApi->pMethod->xStop(self->pCtpMduserApi);
  */
  
  // 交易退出
  self->pCtpTraderApi->pMethod->xLogout(self->pCtpTraderApi);

  return 0;
}

int trader_svr_proc_client_update(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  int i;
  struct trader_cmd_update_req_def *pUpdate = &(req->body.update);

  //查询T1/T2的信息
  for(i = 0; i < pUpdate->num; i++){
    trader_contract* iter;
    TAILQ_FOREACH(iter, &self->listTraderContract, next){
      if(!strcmp(pUpdate->stage[i].T1, iter->contract)){
        pUpdate->stage[i].PriceTick = iter->PriceTick;
        pUpdate->stage[i].IsSHFE = iter->isSHFE;
      }else if(!strcmp(pUpdate->stage[i].T2, iter->contract)){
        pUpdate->stage[i].T2PriceTick = iter->PriceTick;
        pUpdate->stage[i].T2IsSHFE = iter->isSHFE;
      }
    }
  }

  // 更新交易策略
  CMN_DEBUG("通知策略引擎行情更新!\n");
  self->pStrategyEngine->pMethod->xUpdateStrategy(self->pStrategyEngine, pUpdate);
  
  // 更新成功返回
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = req->cmd;
  strcpy(oRsp.traceNo, req->traceNo);
  oRsp.req = req->req;
  oRsp.ErrorID = 0;
  oRsp.ErrorMsg[0] = '\0';

  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, recNum);

  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);

  return 0;
}

int trader_svr_proc_client_query_instruments(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  //遍历所有合约
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = TRADER_CMD_TYPE_QUERY_INSTRUMENT;
  strcpy(oRsp.traceNo, req->traceNo);
  oRsp.req = req->req;
  oRsp.ErrorID = 0;
  oRsp.ErrorMsg[0] = '\0';

  int i;
  int nCnt = self->nContractNum;
  CMN_DEBUG("instruments nCnt[%d]\n", nCnt);
  oRsp.recNum = nCnt;
  trader_contract* pTraderContract;
  i = 0;
  TAILQ_FOREACH(pTraderContract, &self->listTraderContract, next){
    memcpy(&oRsp.buff[i*TraderInstrumentIDTypeLen], pTraderContract->contract, sizeof(pTraderContract->contract));
    CMN_DEBUG("instruments [%s]\n", &oRsp.buff[i*TraderInstrumentIDTypeLen]);
    i++;
  }
  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, buff) 
    + nCnt * TraderInstrumentIDTypeLen;
  CMN_DEBUG("cmn_util_bufferevent_send [%d]\n", nPkgSize);
  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);
  return 0;
  
}

int trader_svr_proc_client_query_positions(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  //遍历所有仓位
  int nCnt = 0;
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = TRADER_CMD_TYPE_QUERY_POSITION;
  strcpy(oRsp.traceNo, req->traceNo);
  oRsp.req = req->req;
  oRsp.ErrorID = 0;
  oRsp.ErrorMsg[0] = '\0';

  trade_position oTdPos;
  
  int i;
  nCnt = 0;
  for(i = 0; i < TRADER_CMD_UPDATE_NUM; i++){
    self->pStrategyEngine->pMethod->xQueryPosition(self->pStrategyEngine,
      i, TRADER_POSITION_BUY, &oTdPos);
    if(oTdPos.Volume > 0){      
      oTdPos.StageID = i;
      memcpy(&oRsp.buff[nCnt*sizeof(trade_position)], &oTdPos, sizeof(oTdPos));
      nCnt++;
    }
    self->pStrategyEngine->pMethod->xQueryPosition(self->pStrategyEngine,
      i, TRADER_POSITION_SELL, &oTdPos);
    if(oTdPos.Volume > 0){
      oTdPos.StageID = i;
      memcpy(&oRsp.buff[nCnt*sizeof(trade_position)], &oTdPos, sizeof(oTdPos));
      nCnt++;
    }
  }


  // 打印当前撤单笔数
  trader_contract* iter;
  TAILQ_FOREACH(iter, &self->listTraderContract, next){
    if(iter->nCancelNum > 0){
      CMN_DEBUG("instruments [%s] cancel number[%d]\n", iter->contract, iter->nCancelNum);
      oTdPos.StageID = -1;
      strcpy(oTdPos.T1, iter->contract);
      oTdPos.Volume = iter->nCancelNum;
      memcpy(&oRsp.buff[nCnt*sizeof(trade_position)], &oTdPos, sizeof(oTdPos));
      nCnt++;
    }
  }
  
  oRsp.recNum = nCnt;
  
  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, buff) 
    + nCnt * sizeof(trade_position);
  CMN_DEBUG("cmn_util_bufferevent_send[%d][%d][%d]\n", 
    nPkgSize, nCnt, sizeof(trade_position));
  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);

  return 0;
}

int trader_svr_proc_client_close_all(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  //关闭所有策略
  int nCnt = 0;
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = TRADER_CMD_TYPE_CLOSE_ALL;
  strcpy(oRsp.traceNo, req->traceNo);
  oRsp.req = req->req;
  oRsp.ErrorID = 0;
  oRsp.ErrorMsg[0] = '\0';

  self->pStrategyEngine->pMethod->xCloseAll(self->pStrategyEngine);
  
  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, recNum);
  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);
  return 0;
  
}

int trader_svr_proc_client_reset_positions(trader_svr* self, trader_msg_req_struct* req)
{
  CMN_DEBUG("Enter!\n");
  //遍历所有合约
  int nCnt = 0;
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = TRADER_CMD_TYPE_RESET_POSITION;
  strcpy(oRsp.traceNo, req->traceNo);
  oRsp.req = req->req;
  oRsp.ErrorID = 0;
  oRsp.ErrorMsg[0] = '\0';
  
  struct trader_cmd_reset_req_def* pResetInf = &(req->body.reset);

  self->pStrategyEngine->pMethod->xResetPosition(self->pStrategyEngine,
    pResetInf->StageID, pResetInf->Direction, pResetInf->Volume);

  
  self->pCtpTraderApi->pMethod->xQryInvestorPosition(self->pCtpTraderApi);

  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, recNum);
  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);
  return 0;
}



trader_svr_method* trader_svr_method_get()
{
  static trader_svr_method st_trader_svr_method = {
    trader_svr_init,
    trader_svr_param_ini,
    trader_svr_run,
    trader_svr_exit,
    trader_svr_on_client_recv,
    trader_svr_on_client_send,
    trader_svr_on_trader_recv,
    trader_svr_on_mduser_recv
  };

  return &st_trader_svr_method;
}

int trader_svr_client_notify_login(trader_svr* self, int err_cd, char* error_msg)
{

  if(!self->bProcessing){
    // 非登陆状态
    return 0;
  }
  
  trader_msg_rsp_struct oRsp;
  oRsp.cmd = TRADER_CMD_TYPE_LOGIN;
  strcpy(oRsp.traceNo, self->CurrentTraceNo);
  oRsp.req = self->pReqMsg;
  oRsp.ErrorID = err_cd;
  strcpy(&oRsp.ErrorMsg[0], error_msg);

  unsigned char* pPkg = (unsigned char*)&oRsp;
  int nPkgSize = offsetof(trader_msg_rsp_struct, recNum);
  cmn_util_bufferevent_send(self->BufClient, pPkg, nPkgSize);

  self->bProcessing = 0;

  return 0;
}

int trader_svr_chdir(trader_svr* self, char* user_id)
{
  int len = strlen(self->WorkSpace) + strlen(user_id) + 1 + 1;
  char* pPath = (char*)malloc(len * sizeof(char));
  sprintf(pPath, "%s/%s", self->WorkSpace, user_id);
  // 检查目录是否存在
  DIR* dp = opendir(pPath);
  if(!dp){
    // 创建目录
    mkdir(pPath, S_IRWXU);
  }else {
    closedir(dp);
  }

  chdir(pPath);

  free(pPath);

  return 0;

}

int trader_svr_check_instrument(trader_svr* self, char* inst)
{
  int found = 0;

  redisReply *reply;
  reply = redisCommand(self->pRedis, "SISMEMBER %s %s", "MDUSER.INSTRUMENTS", inst);
  found = (int)reply->integer;
  freeReplyObject(reply);

  return found;  
}

void trader_svr_mduser_read_cb(struct bufferevent *bev, void *arg)
{
  trader_svr* self = (trader_svr*)arg;

  self->pMethod->xOnMduserRecv(self);

}

void trader_svr_mduser_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_ERROR("Cannot reach here!\n");
}

void trader_svr_trader_read_cb(struct bufferevent *bev, void *arg)
{
  trader_svr* self = (trader_svr*)arg;

  self->pMethod->xOnTraderRecv(self);

}

void trader_svr_trader_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_ERROR("Cannot reach here!\n");
}

void trader_svr_client_read_cb(struct bufferevent *bev, void *arg)
{
  trader_svr* self = (trader_svr*)arg;

  self->pMethod->xOnClientRecv(self);

}

void trader_svr_client_write_cb(struct bufferevent *bev, void *arg)
{
  trader_svr* self = (trader_svr*)arg;

  self->pMethod->xOnClientSend(self);
}

void trader_svr_client_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_ERROR("Cannot reach here!\n");
}

void trader_svr_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_svr* self = (trader_svr*)arg;
  struct event_base *base = self->pBase;
  struct timeval delay = { 1, 0 };

  event_del(self->pSigIntEvt);
  event_del(self->pSigTermEvt);

  event_base_loopexit(base, &delay);

}

trader_svr* trader_svr_new()
{
  trader_svr* self = (trader_svr*)malloc(sizeof(trader_svr));

  self->pMethod = trader_svr_method_get();

  self->BrokerId = 0;
  self->MdPrefix = 0;
  self->MdFrontAdd = 0;
  self->TrFrontAdd = 0;
  self->WorkSpace = 0;

  self->bProcessing = 0;
  self->CurrentTraceNo[0] = '\0';
  self->UserId[0] = '\0';
  self->Passwd[0] = '\0';

  self->bQueried = 0;
  self->TradingDay[0] = '\0';
  self->pRedis = NULL;
  
  return self;
}

void trader_svr_free(trader_svr* self)
{
  if(self->BrokerId){
    free(self->BrokerId);
  }
  
  if(self->MdPrefix){
    free(self->MdPrefix);
  }
  
  if(self->MdFrontAdd){
    free(self->MdFrontAdd);
  }
  
  if(self->TrFrontAdd){
    free(self->TrFrontAdd);
  }

  if(self->WorkSpace){
    free(self->WorkSpace);
  }

  
  if(self){
    free(self);
  }
}

void trader_svr_mduser_client_connect_callback(void* user_data)
{
  CMN_INFO("connected!\n");
}

void trader_svr_mduser_client_disconnect_callback(void* user_data)
{
  CMN_ERROR("disconnected!\n");
  trader_svr* self = (trader_svr*)user_data;
  self->pMduserClt->method->xConnect(self->pMduserClt);
}

void trader_svr_mduser_client_recv_callback(void* user_data, void* data, int len)
{
  trader_svr* self = (trader_svr*)user_data;
  trader_mduser_evt* pEvt = (trader_mduser_evt*)NULL;
  int nPos = 0;
  char pBuff[sizeof(trader_mduser_evt)];
  trader_tick* tick_data = (trader_tick*)NULL;
  char* pData = (char*)data;
  
  if((self->cacheLen + len) < sizeof(trader_mduser_evt)){
    memcpy(&self->cache[self->cacheLen], pData, len);
    self->cacheLen += len;
    return ;
  }

  if(self->cacheLen > 0){
    nPos = sizeof(trader_mduser_evt) - self->cacheLen;
    memcpy(pBuff, self->cache, self->cacheLen);
    memcpy(&pBuff[self->cacheLen], pData, nPos);
    self->cacheLen = 0;
    
    pEvt = (trader_mduser_evt*)pBuff;
    tick_data = &pEvt->Tick;
    self->pStrategyEngine->pMethod->xUpdateTick(self->pStrategyEngine, tick_data);
  }

  while(nPos < len){
    if((nPos + sizeof(trader_mduser_evt)) > len){
      self->cacheLen = len - nPos;
      memcpy(&self->cache[0], &pData[nPos], self->cacheLen);
      break;
    }

    pEvt = (trader_mduser_evt*)&pData[nPos];
    tick_data = &pEvt->Tick;
    self->pStrategyEngine->pMethod->xUpdateTick(self->pStrategyEngine, tick_data);

    nPos += sizeof(trader_mduser_evt);
  }

  //trader_tick* tick_data = &pEvt->Tick;
  
  //printf("tick[%s]UpdateTime[%s]UpdateMillisec[%d]\n", tick_data->InstrumentID, tick_data->UpdateTime, tick_data->UpdateMillisec);

  //CMN_DEBUG("通知策略引擎行情更新!\n");
  //self->pStrategyEngine->pMethod->xUpdateTick(self->pStrategyEngine, tick_data);

}

int trader_svr_redis_init(trader_svr* self)
{
  // 链接redis
  redisContext *c;
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  c = redisConnectWithTimeout("localhost", 6379, timeout);
  if (c == NULL || c->err) {
    if (c) {
      redisFree(c);
      c = NULL;
    } else {
    
    }
    return -1;
  }

  self->pRedis = c;

  return 0;
}

int trader_svr_redis_fini(trader_svr* self)
{
  redisContext* c = self->pRedis;
  if (c) {
    redisFree(c);
  }

  return 0;

}


int trade_main(char* cfg_file, evutil_socket_t sock)
{
  CMN_LOG_INI(cfg_file, "traderSvr");
  cmn_util_change_proc("traderSvr");
  int nRet = 0;
  trader_svr* server = trader_svr_new();

  nRet = server->pMethod->xParmIni(server, cfg_file);
  CMN_ASSERT(!nRet);
  
  nRet = server->pMethod->xInit(server, sock);
  CMN_ASSERT(!nRet);

  nRet = server->pMethod->xRun(server);
  
  nRet = server->pMethod->xExit(server);

  trader_svr_free(server);
  
  CMN_LOG_FINI();
  
  return 0;
}

