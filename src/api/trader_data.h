#ifndef _TRADER_DATA_H_
#define _TRADER_DATA_H_

#include <time.h>

/////////////////////////////////////////////////////////////////////////
///TFtdcUstpInstrumentIDType是一个合约编号类型
/////////////////////////////////////////////////////////////////////////
#define TraderInstrumentIDTypeLen 31
///TraderDateTypeLen是一个日期类型
/////////////////////////////////////////////////////////////////////////
#define TraderDateTypeLen 9
/////////////////////////////////////////////////////////////////////////
///TraderTimeTypeLen是一个时间类型
/////////////////////////////////////////////////////////////////////////
#define TraderTimeTypeLen 9
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen是一个用户本地报单号类型
/////////////////////////////////////////////////////////////////////////
#define TraderOrderRefTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderOrderSysTypeLen是一个系统报单号类型
/////////////////////////////////////////////////////////////////////////
#define TraderOrderSysTypeLen 31
/////////////////////////////////////////////////////////////////////////
///TraderExchangeIDTypeLen是一个交易所编码类型
/////////////////////////////////////////////////////////////////////////
#define TraderExchangeIDTypeLen 11
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen是一个成交编号类型
/////////////////////////////////////////////////////////////////////////
#define TraderTradeIDTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderUserIDTypeLen是一个成交编号类型
/////////////////////////////////////////////////////////////////////////
#define TraderUserIDTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen是一个成交编号类型
/////////////////////////////////////////////////////////////////////////
#define TraderPasswordTypeLen 41

typedef char trader_instrument_id_type[31];

typedef struct trader_tick_def trader_tick;
typedef struct trader_order_def trader_order;
typedef struct trader_trade_def trader_trade;

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
};


///全部成交
#define TRADER_ORDER_OS_BEGIN '-'
///全部成交
#define TRADER_ORDER_OS_UNKNOW 'a'
///全部成交
#define TRADER_ORDER_OS_ALLTRADED '0'
///部分成交还在队列中
#define TRADER_ORDER_OS_PARTTRADEDQUEUEING '1'
///部分成交不在队列中
#define TRADER_ORDER_OS_PARTTRADEDNOTQUEUEING '2'
///未成交还在队列中
#define TRADER_ORDER_OS_NOTRADEQUEUEING '3'
///未成交不在队列中
#define TRADER_ORDER_OS_NOTRADENOTQUEUEING '4'
///撤单
#define TRADER_ORDER_OS_CANCELED '5'
///订单已报入交易所未应答
#define TRADER_ORDER_OS_ACCEPTEDNOREPLY '6'
///订单提交失败
#define TRADER_ORDER_OS_FAILED '7'

struct trader_order_def {
	///交易所代码
	char	ExchangeID[TraderExchangeIDTypeLen];
	///系统报单编号
	char	OrderSysID[TraderOrderSysTypeLen];
  // 合约代码
  char InstrumentID [TraderInstrumentIDTypeLen];
  // 本地报单编号
  char UserOrderLocalID[TraderOrderRefTypeLen];
  // 买卖
  char	Direction;
  // 开平
  char	OffsetFlag;
  ///投机套保标志
  char HedgeFlag;
  // 报单价格
  double LimitPrice;
  // 报单手数
  int VolumeOriginal;
  // 成交手数
  int VolumeTraded;
  // 订单状态
  char OrderStatus;
  ///插入时间
  char	InsertTime[TraderTimeTypeLen];
  // 插入时间
  struct timeval CreateTime;
  // 回报时间
  struct timeval UpdateTime;
  
};

struct trader_trade_def {
  ///合约代码
  char InstrumentID [TraderInstrumentIDTypeLen];
  ///本地报单编号
  char UserOrderLocalID[TraderOrderRefTypeLen];
  ///交易日
  char	TradingDay[TraderDateTypeLen];
  ///成交时间
  char	TradeTime[TraderTimeTypeLen];
  char	Direction;
  char	OffsetFlag;
  ///成交价格
  double	TradePrice;
  ///成交数量
  int	TradeVolume;
  //成交编号
  char TradeID[TraderTradeIDTypeLen];
  // 回报时间
  struct timeval UpdateTime;
};

#define TRADER_POSITION_BUY '0'
#define TRADER_POSITION_SELL '1'
#define TRADER_POSITION_OPEN '0'
#define TRADER_POSITION_CLOSE '1'
#define TRADER_POSITION_CLOSE_TODAY '3'
#define TRADER_POSITION_CLOSE_YESTERDAY '4'

#define TRADER_POSITION_LONG '2'
#define TRADER_POSITION_SHORT '3'

#define TRADER_POSITION_TYPE_OPEN_T1 '0'
#define TRADER_POSITION_TYPE_OPEN_T2 '1'
#define TRADER_POSITION_TYPE_CLOSE_T1 '2'
#define TRADER_POSITION_TYPE_CLOSE_T2 '3'
#define TRADER_POSITION_TYPE_OPEN_T1_ACTION '4'
#define TRADER_POSITION_TYPE_CLOSE_T1_ACTION '5'
#define TRADER_POSITION_TYPE_OPEN_T2_ACTION '6'
#define TRADER_POSITION_TYPE_CLOSE_T2_ACTION '7'


#endif //_TRADER_DATA_H_

