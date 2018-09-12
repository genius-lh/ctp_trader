#ifndef _TRADER_DB_H_
#define _TRADER_DB_H_

#include "sqlite3.h"
#include "trader_data.h"

#define TRADER_DB_NAME "strategy_db"

struct trader_db_mduser_inf_def {
	///��Լ����
	char InstrumentID [TraderInstrumentIDTypeLen];
	///������
	char	TradingDay [TraderDateTypeLen];
	///����޸�ʱ��
	char UpdateTime [TraderTimeTypeLen];
	///����޸ĺ���
	int	UpdateMillisec;
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
};

//�������,��Լ,����,��ƽ,�ҵ�״̬,�����۸�,��������,��ϸ״̬,����ʱ��,���ɽ�ʱ��,�ɽ�����,������
struct trader_db_order_inf_def {
  //�������
  char	UserOrderLocalID[TraderOrderRefTypeLen];
  //��Լ
  char	InstrumentID[TraderInstrumentIDTypeLen];
  //����
  char Direction[2];
  //��ƽ
  char OffsetFlag[2];
  ///Ͷ���ױ���־
  char HedgeFlag[2];
  //�ҵ�״̬
  char	OrderStatus[2];
  //�����۸�
  double	LimitPrice;
  //��������
  int Volume;
  //����ʱ��
  char InsertTime[TraderTimeTypeLen];
  //���ط���ʱ��
  char LocalCreateTime[27];
  // ���ؽ���ʱ��
  char LocalUpdateTime[27];
};

struct trader_db_trade_inf_def {
  ///��Լ����
  char InstrumentID [TraderInstrumentIDTypeLen];
  ///������
  char TradingDay[TraderDateTypeLen];
  ///�ɽ����
  char TradeID[TraderOrderRefTypeLen];
  ///���ر������
  char UserOrderLocalID[TraderOrderRefTypeLen];
  ///�ɽ��۸�
  double TradePrice;
  ///�ɽ�����
  int TradeVolume;
  ///�ɽ�ʱ��
  char TradeTime[TraderTimeTypeLen];
  // ���ؽ���ʱ��
  char LocalUpdateTime[27];
};

struct trader_db_trade_view_def {
  ///�ɽ�ʱ��
  char TradeTime[TraderTimeTypeLen];
  ///�ɽ�����
  int TradeVolume;
  ///�ɽ��۸�
  double TradePrice;
  ///���ر������
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

