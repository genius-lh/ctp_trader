#ifndef _TRADER_SVR_H_
#define _TRADER_SVR_H_

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "trader_msg_struct.h"
#include "trader_db.h"
//#include "ctp_trader_api.h"
#include "trader_trader_api.h"

#include "trader_strategy_engine.h"

#include "trader_mduser_api.h"

#include "trader_mduser_client.h"

#include "hiredis.h"

#include "queue.h"

typedef struct trader_contract_def trader_contract;
struct trader_contract_def {
  TAILQ_ENTRY(trader_contract_def) next;
  char contract[TraderInstrumentIDTypeLen];
  char ExchangeID[9];
  double PriceTick;
  int nCancelNum;
  int isSHFE;
};



typedef struct trader_svr_def trader_svr;
typedef struct trader_svr_method_def trader_svr_method;
struct trader_svr_def {
  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  trader_mduser_client* pMduserClt;
  char cache[sizeof(trader_mduser_evt)];
  int cacheLen;
  
  // 对客户端进程
  struct bufferevent *BufClient;

  // 对行情线程
  //struct bufferevent *BufMduser;

  // 对交易线程
  struct bufferevent *BufTrader;

  // 行情API
  //ctp_mduser_api* pCtpMduserApi;

  // 交易API
  //ctp_trader_api* pCtpTraderApi;
  trader_trader_api* pCtpTraderApi;
  
  //内置数据库
  trader_db* pTraderDB;

  // 策略Engine
  trader_strategy_engine* pStrategyEngine;

  // Redis
  redisContext* pRedis;
  char pRedisIp[21];
  int nRedisPort;
  char redisInstrumentKey[64];
  
  // 交易行情服务
  int nContractNum;
  TAILQ_HEAD(trader_contract_def_list,trader_contract_def) listTraderContract;

  // 参数
  // 总仓位限制
  int nPositionSize;
  //BrokerID
  char* BrokerId;
  //行情前缀
  char* MdPrefix;
  //行情前置地址
  char* MdFrontAdd;
  //交易前置地址
  char* TrFrontAdd;
  // 工作目录
  char* WorkSpace;
  // 记录行情
  int nStoreMduser;
  // 下单方式
  char cHedgeFlag;
  // 有效期类型
  char cTimeCondition;
  // 行情接收地址端口
  char boardcastAddr[14+1];
  int boardcastPort;

  // 登录信息
  int bProcessing;
  char CurrentTraceNo[36+1];
  char UserId[16];
  char Passwd[41];
  void* pReqMsg;

  // 是否查询
  char TradingDay[9];
  int bQueried;
  
  trader_svr_method* pMethod;
};

struct trader_svr_method_def {
  int (*xInit)(trader_svr* self, evutil_socket_t sock);
  int (*xParmIni)(trader_svr* self, char* cfg_file);
  int (*xRun)(trader_svr* self);
  int (*xExit)(trader_svr* self);
  int (*xOnClientRecv)(trader_svr* self);
  int (*xOnClientSend)(trader_svr* self);
  int (*xOnTraderRecv)(trader_svr* self);
  int (*xOnMduserRecv)(trader_svr* self);
};

extern trader_svr* trader_svr_new();
extern void trader_svr_free(trader_svr* self);


extern int trade_main(char* cfg_file, evutil_socket_t sock);

#endif //_TRADER_SVR_H_

