#ifndef _TRADER_TRADER_API_H_
#define _TRADER_TRADER_API_H_

#include <event2/util.h>
#include "cmn_util.h"
#include "trader_data.h"

#define TRADERONFRONTCONNECTED 1
#define TRADERONFRONTDISCONNECTED 2
#define TRADERONHEARTBEATWARNING 3
#define TRADERONRSPERROR 4
#define TRADERONRSPUSERLOGIN 5
#define TRADERONRSPUSERLOGOUT 6
#define TRADERONRSPORDERINSERT 7
#define TRADERONRSPORDERACTION 8
#define TRADERONRSPUSERPASSWORDUPDATE 9 // NOT USED
#define TRADERONRSPTRADINGACCOUNTPASSWORDUPDATE 10 // NOT USED
#define TRADERONRSPQRYEXCHANGE 11 // NOT USED
#define TRADERONRSPQRYINSTRUMENT 12
#define TRADERONRSPQRYINVESTOR 13
#define TRADERONRSPQRYTRADINGCODE 14 // NOT USED
#define TRADERONRSPQRYTRADINGACCOUNT 15
#define TRADERONRSPQRYDEPTHMARKETDATA 16 // NOT USED
#define TRADERONRSPQRYBONDINTEREST 17 // NOT USED
#define TRADERONRSPQRYMARKETRATIONINFO 18 // NOT USED
#define TRADERONRSPQRYINSTRUMENTCOMMISSIONRATE 19 // NOT USED
#define TRADERONRSPQRYETFINSTRUMENT 20 // NOT USED
#define TRADERONRSPQRYETFBASKET 21 // NOT USED
#define TRADERONRSPQRYOFINSTRUMENT 22 // NOT USED
#define TRADERONRSPQRYSFINSTRUMENT 23 // NOT USED
#define TRADERONRSPQRYORDER 24 // NOT USED
#define TRADERONRSPQRYTRADE 25 // NOT USED
#define TRADERONRSPQRYINVESTORPOSITION 26
#define TRADERONRTNORDER 27
#define TRADERONRTNTRADE 28
#define TRADERONERRRTNORDERINSERT 29
#define TRADERONERRRTNORDERACTION 30
#define TRADERONRSPFUNDOUTBYLIBER 31 // NOT USED
#define TRADERONRTNFUNDOUTBYLIBER 32 // NOT USED
#define TRADERONERRRTNFUNDOUTBYLIBER 33 // NOT USED
#define TRADERONRTNFUNDINBYBANK 34 // NOT USED
#define TRADERONRSPQRYFUNDTRANSFERSERIAL 35 // NOT USED
#define TRADERONRSPFUNDINTERTRANSFER 36 // NOT USED
#define TRADERONRSPQRYFUNDINTERTRANSFERSERIAL 37 // NOT USED
#define TRADERONRTNFUNDINTERTRANSFERSERIAL 38 // NOT USED
#define TRADERONERRRTNFUNDINTERTRANSFER 39 // NOT USED
#define TRADERONRTNINSTRUMENTSTATUS 40

typedef struct trader_instrument_def trader_instrument;
typedef struct trader_account_def trader_account;
typedef struct trader_position_def trader_position;

struct trader_instrument_def {
	///合约代码
	char InstrumentID [31];
  ///交易所
  char ExchangeID[20];
	///合约数量乘数
	int	VolumeMultiple;
	///最小变动价位
	double	PriceTick;
	///期权单手保证金
	double	UnitMargin;
};

struct trader_account_def {
  char AccountID[15];
};

struct trader_position_def {
	///合约代码
	char InstrumentID [31];
  char PosiDirection;
  int IsSHFE;
  char PositionDate;
  int YdPosition;
  int TodayPosition;
  int Position;
  int LongFrozen;
};

typedef struct trader_trader_evt_def trader_trader_evt;
struct trader_trader_evt_def {
  int Type;
  int IsLast;
  int ErrorCd;
  char ErrorMsg[81];
  union {
    trader_order OrderRsp;
    trader_trade TradeRsp;
    trader_instrument InstrumentRsp;
    char InvestorRsp[21];
    trader_account AccountRsp;
    trader_position PositionRsp;
    instrument_status InstrumentStatusRsp;
  } Body;
};


typedef struct trader_trader_api_method_def trader_trader_api_method;
typedef struct trader_trader_api_def trader_trader_api;

struct trader_trader_api_def {
  evutil_socket_t fd;
  char* pBrokerID;
  char* pUser;
  char* pPwd;
  char* pAddress;
  char* pWorkspace;
  char* pAppID;
  char* pAuthCode;
  long userLocalId;
  char timeCondition;
  char hedgeFlag;

  void* pUserApi;
  
  trader_trader_api_method* pMethod;
};
/*
xQryInstrument 查询合约信息
xQryUserInvestor 查询投资者
xQryInvestorPosition 查询持仓
xQryTradingAccount 查询账号资金

xOrderInsert 发单
xOrderAction 撤单
*/
struct trader_trader_api_method_def{
  void (*xSetUser)(trader_trader_api* self, char* broker_id, char* user, char* pwd);
  void (*xSetFrontAddr)(trader_trader_api* self, char* addr);
  void (*xSetWorkspace)(trader_trader_api* self, char* ws);
  void (*xSetAppID)(trader_trader_api* self, char* appid);
  void (*xSetAuthCode)(trader_trader_api* self, char* authcode);
  void (*xSetParam)(trader_trader_api* self, char* key, char* val);
  int (*xGetTradingDay)(trader_trader_api* self, char* tradingday);
  int (*xGetMaxOrderLocalID)(trader_trader_api* self, long* local_id);
  
  void (*xStart)(trader_trader_api* self);
  void (*xStop)(trader_trader_api* self);
  void (*xLogin)(trader_trader_api* self);
  void (*xLogout)(trader_trader_api* self);
  
  int (*xOrderInsert)(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  int (*xOrderAction)(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);
  
  int (*xQryInstrument)(trader_trader_api* self);
  int (*xQryUserInvestor)(trader_trader_api* self);
  int (*xQryInvestorPosition)(trader_trader_api* self);
  int (*xQryTradingAccount)(trader_trader_api* self);
};

extern trader_trader_api* trader_trader_api_new(evutil_socket_t fd, trader_trader_api_method* method);
extern void trader_trader_api_free(trader_trader_api* self);

extern void trader_trader_api_set_user(trader_trader_api* self, char* broker_id, char* user, char* pwd);
extern void trader_trader_api_set_front_addr(trader_trader_api* self, char* addr);
extern void trader_trader_api_set_workspace(trader_trader_api* self, char* ws);
extern void trader_trader_api_set_app_id(trader_trader_api* self, char* appid);
extern void trader_trader_api_set_auth_code(trader_trader_api* self, char* authcode);

extern void trader_trader_api_set_param(trader_trader_api* self, char* key, char* val);
extern int trader_trader_api_get_trading_day(trader_trader_api* self, char* tradingday);
extern int trader_trader_api_get_max_order_local_id(trader_trader_api* self, char* local_id);

extern void trader_trader_api_on_front_connected(trader_trader_api* self);
extern void trader_trader_api_on_front_disconnected(trader_trader_api* self, int nReason);
extern void trader_trader_api_on_rsp_error(trader_trader_api* self, int err_cd, const char* err_msg);
extern void trader_trader_api_on_rsp_user_login(trader_trader_api* self, int err_cd, char* err_msg);
extern void trader_trader_api_on_rsp_user_logout(trader_trader_api* self, int err_cd, char* err_msg);
extern void trader_trader_api_on_rsp_order_insert(trader_trader_api* self, int err_cd, char* err_msg);
extern void trader_trader_api_on_rsp_order_action(trader_trader_api* self, int err_cd, char* err_msg);
extern void trader_trader_api_on_rtn_trade(trader_trader_api* self, trader_trade* trade);
extern void trader_trader_api_on_rtn_order(trader_trader_api* self, trader_order* order);
extern void trader_trader_api_on_err_rtn_order_insert(trader_trader_api* self, int err_cd, char* err_msg);
extern void trader_trader_api_on_err_rtn_order_action(trader_trader_api* self, int err_cd, char* err_msg);

extern void trader_trader_api_on_rsp_qry_user_investor(trader_trader_api* self, char* investor_id, int err_cd, char* err_msg);
extern void trader_trader_api_on_rsp_qry_instrument(trader_trader_api* self, trader_instrument *pRspInstrument, int err_cd, char* err_msg, int is_last);
extern void trader_trader_api_on_rsp_qry_investor_position(trader_trader_api* self, trader_position *pInvestorPosition, int err_cd, char* err_msg, int is_last);
extern void trader_trader_api_on_rsp_qry_trading_account(trader_trader_api* self, trader_account *pTradingAcount, int err_cd, char* err_msg, int is_last);
extern void trader_trader_api_on_rtn_instrument_status(trader_trader_api* self, instrument_status* status);

extern int trader_trader_api_load_instruments(const char* configFile, const char* section, mduser_instrument** insts);

#endif //_TRADER_TRADER_API_H_

