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
  
  // �Կͻ��˽���
  struct bufferevent *BufClient;

  // �������߳�
  //struct bufferevent *BufMduser;

  // �Խ����߳�
  struct bufferevent *BufTrader;

  // ����API
  //ctp_mduser_api* pCtpMduserApi;

  // ����API
  //ctp_trader_api* pCtpTraderApi;
  trader_trader_api* pCtpTraderApi;
  
  //�������ݿ�
  trader_db* pTraderDB;

  // ����Engine
  trader_strategy_engine* pStrategyEngine;

  // Redis
  redisContext* pRedis;
  char pRedisIp[21];
  int nRedisPort;
  char redisInstrumentKey[64];
  
  // �����������
  int nContractNum;
  TAILQ_HEAD(trader_contract_def_list,trader_contract_def) listTraderContract;

  // ����
  // �ܲ�λ����
  int nPositionSize;
  //BrokerID
  char* BrokerId;
  //����ǰ׺
  char* MdPrefix;
  //����ǰ�õ�ַ
  char* MdFrontAdd;
  //����ǰ�õ�ַ
  char* TrFrontAdd;
  // ����Ŀ¼
  char* WorkSpace;
  // ��¼����
  int nStoreMduser;
  // �µ���ʽ
  char cHedgeFlag;
  // ��Ч������
  char cTimeCondition;
  // ������յ�ַ�˿�
  char boardcastAddr[14+1];
  int boardcastPort;

  // ��¼��Ϣ
  int bProcessing;
  char CurrentTraceNo[36+1];
  char UserId[16];
  char Passwd[41];
  void* pReqMsg;

  // �Ƿ��ѯ
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

