#ifndef _TRADER_DB_H_
#define _TRADER_DB_H_

#include "sqlite3.h"
#include "trader_data.h"

#define TRADER_DB_NAME "strategy_db"

struct trader_db_mduser_inf_def {
	///合约代码
	char InstrumentID [TraderInstrumentIDTypeLen];
	///交易日
	char	TradingDay [TraderDateTypeLen];
	///最后修改时间
	char UpdateTime [TraderTimeTypeLen];
	///最后修改毫秒
	int	UpdateMillisec;
	///申买价一
	double	BidPrice1;
	///申买量一
	int	BidVolume1;
	///申卖价一
	double	AskPrice1;
	///申卖量一
	int	AskVolume1;
};

//报单编号,合约,买卖,开平,挂单状态,报单价格,报单手数,详细状态,报单时间,最后成交时间,成交均价,交易所
struct trader_db_order_inf_def {
  //报单编号
  char	UserOrderLocalID[TraderOrderRefTypeLen];
  //合约
  char	InstrumentID[TraderInstrumentIDTypeLen];
  //买卖
  char Direction[2];
  //开平
  char OffsetFlag[2];
  ///投机套保标志
  char HedgeFlag[2];
  //挂单状态
  char	OrderStatus[2];
  //报单价格
  double	LimitPrice;
  //报单手数
  int Volume;
  //报单时间
  char InsertTime[TraderTimeTypeLen];
  //本地发送时间
  char LocalCreateTime[27];
  // 本地接收时间
  char LocalUpdateTime[27];
};

struct trader_db_trade_inf_def {
  ///合约代码
  char InstrumentID [TraderInstrumentIDTypeLen];
  ///交易日
  char TradingDay[TraderDateTypeLen];
  ///成交编号
  char TradeID[TraderOrderRefTypeLen];
  ///本地报单编号
  char UserOrderLocalID[TraderOrderRefTypeLen];
  ///成交价格
  double TradePrice;
  ///成交数量
  int TradeVolume;
  ///成交时间
  char TradeTime[TraderTimeTypeLen];
  // 本地接收时间
  char LocalUpdateTime[27];
};

struct trader_db_trade_view_def {
  ///成交时间
  char TradeTime[TraderTimeTypeLen];
  ///成交数量
  int TradeVolume;
  ///成交价格
  double TradePrice;
  ///本地报单编号
  char UserOrderLocalID[TraderOrderRefTypeLen];
};


typedef struct trader_db_def trader_db;

typedef struct trader_db_method_def trader_db_method;

struct trader_db_def {
  sqlite3* db;
  sqlite3_stmt* stmtMduserInsert;
  sqlite3_stmt* stmtMduserSelect;
  sqlite3_stmt* stmtTradeInsert;
  sqlite3_stmt* stmtTradeSelect;
  sqlite3_stmt* stmtOrderInsert;
  sqlite3_stmt* stmtOrderSelect;
  sqlite3_stmt* stmtTradeView;
  char* pDbName;
  trader_db_method* pMethod;
};

struct trader_db_method_def {
  int (*xMduserInsert)(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
  int (*xMduserDeclare)(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
  int (*xMduserOpen)(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
  int (*xMduserFetch)(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
  int (*xMduserClose)(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
  int (*xOrderInsert)(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
  int (*xOrderDeclare)(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
  int (*xOrderOpen)(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
  int (*xOrderFetch)(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
  int (*xOrderClose)(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
  int (*xTradeInsert)(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
  int (*xTradeDeclare)(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
  int (*xTradeOpen)(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
  int (*xTradeFetch)(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
  int (*xTradeClose)(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
  int (*xTradeView)(trader_db* self, struct trader_db_trade_view_def* p_data, int* errcd);
  int (*xInit)(trader_db* self, char* date);
  int (*xFini)(trader_db* self);

};

extern trader_db* trader_db_new(char* db_path);
extern void trader_db_free(trader_db* self);



#endif //_TRADER_DB_H_

