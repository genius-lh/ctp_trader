#ifndef _TRADER_MDUSER_PROXY_STRUCT_H_
#define _TRADER_MDUSER_PROXY_STRUCT_H_
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct trader_tick_def trader_tick;

struct trader_tick_def {
	///合约代码
	char InstrumentID [31];
	///交易日
	char	TradingDay [9];
	///最后修改时间
	char UpdateTime [9];
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
	///涨停板价
	double	UpperLimitPrice;
	///跌停板价
	double	LowerLimitPrice;
	///最新价
	double	LastPrice;
  ///到达时间
  struct timeval ReceiveTime;
  ///保留字段
  long Reserved;
};

#define MDUSERONFRONTCONNECTED 0
#define MDUSERONFRONTDISCONNECTED 1
#define MDUSERONHEARTBEATWARNING 2
#define MDUSERONRSPERROR 3
#define MDUSERONRSPUSERLOGIN 4
#define MDUSERONRSPUSERLOGOUT 5
#define MDUSERONRSPSUBMARKETDATA 6
#define MDUSERONRSPUNSUBMARKETDATA 7
#define MDUSERONRTNDEPTHMARKETDATA 8

typedef struct trader_mduser_evt_def trader_mduser_evt;
struct trader_mduser_evt_def {
  int Type;
  int ErrorCd;
  char ErrorMsg[81];
  // 行情
  trader_tick Tick;
};

#ifdef __cplusplus
}
#endif

#endif //_TRADER_MDUSER_PROXY_STRUCT_H_

