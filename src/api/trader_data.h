#ifndef _TRADER_DATA_H_
#define _TRADER_DATA_H_

#include <time.h>

/////////////////////////////////////////////////////////////////////////
///TFtdcUstpInstrumentIDType��һ����Լ�������
/////////////////////////////////////////////////////////////////////////
#define TraderInstrumentIDTypeLen 31
///TraderDateTypeLen��һ����������
/////////////////////////////////////////////////////////////////////////
#define TraderDateTypeLen 9
/////////////////////////////////////////////////////////////////////////
///TraderTimeTypeLen��һ��ʱ������
/////////////////////////////////////////////////////////////////////////
#define TraderTimeTypeLen 9
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen��һ���û����ر���������
/////////////////////////////////////////////////////////////////////////
#define TraderOrderRefTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderOrderSysTypeLen��һ��ϵͳ����������
/////////////////////////////////////////////////////////////////////////
#define TraderOrderSysTypeLen 31
/////////////////////////////////////////////////////////////////////////
///TraderExchangeIDTypeLen��һ����������������
/////////////////////////////////////////////////////////////////////////
#define TraderExchangeIDTypeLen 11
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen��һ���ɽ��������
/////////////////////////////////////////////////////////////////////////
#define TraderTradeIDTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderUserIDTypeLen��һ���ɽ��������
/////////////////////////////////////////////////////////////////////////
#define TraderUserIDTypeLen 21
/////////////////////////////////////////////////////////////////////////
///TraderOrderRefTypeLen��һ���ɽ��������
/////////////////////////////////////////////////////////////////////////
#define TraderPasswordTypeLen 41

typedef char trader_instrument_id_type[31];

typedef struct trader_tick_def trader_tick;
typedef struct trader_order_def trader_order;
typedef struct trader_trade_def trader_trade;
typedef struct instrument_status_def instrument_status;

struct trader_tick_def {
	///��Լ����
	char InstrumentID [31];
	///������
	char	TradingDay [9];
	///����޸�ʱ��
	char UpdateTime [9];
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
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
};


///ȫ���ɽ�
#define TRADER_ORDER_OS_BEGIN '-'
///ȫ���ɽ�
#define TRADER_ORDER_OS_UNKNOW 'a'
///ȫ���ɽ�
#define TRADER_ORDER_OS_ALLTRADED '0'
///���ֳɽ����ڶ�����
#define TRADER_ORDER_OS_PARTTRADEDQUEUEING '1'
///���ֳɽ����ڶ�����
#define TRADER_ORDER_OS_PARTTRADEDNOTQUEUEING '2'
///δ�ɽ����ڶ�����
#define TRADER_ORDER_OS_NOTRADEQUEUEING '3'
///δ�ɽ����ڶ�����
#define TRADER_ORDER_OS_NOTRADENOTQUEUEING '4'
///����
#define TRADER_ORDER_OS_CANCELED '5'
///�����ѱ��뽻����δӦ��
#define TRADER_ORDER_OS_ACCEPTEDNOREPLY '6'
///�����ύʧ��
#define TRADER_ORDER_OS_FAILED '7'

struct trader_order_def {
	///����������
	char	ExchangeID[TraderExchangeIDTypeLen];
	///ϵͳ�������
	char	OrderSysID[TraderOrderSysTypeLen];
  // ��Լ����
  char InstrumentID [TraderInstrumentIDTypeLen];
  // ���ر������
  char UserOrderLocalID[TraderOrderRefTypeLen];
  // ����
  char	Direction;
  // ��ƽ
  char	OffsetFlag;
  ///Ͷ���ױ���־
  char HedgeFlag;
  // �����۸�
  double LimitPrice;
  // ��������
  int VolumeOriginal;
  // �ɽ�����
  int VolumeTraded;
  // ����״̬
  char OrderStatus;
  ///����ʱ��
  char	InsertTime[TraderTimeTypeLen];
  // ����ʱ��
  struct timeval CreateTime;
  // �ر�ʱ��
  struct timeval UpdateTime;
  
};

struct trader_trade_def {
  ///��Լ����
  char InstrumentID [TraderInstrumentIDTypeLen];
  ///���ر������
  char UserOrderLocalID[TraderOrderRefTypeLen];
  ///������
  char	TradingDay[TraderDateTypeLen];
  ///�ɽ�ʱ��
  char	TradeTime[TraderTimeTypeLen];
  char	Direction;
  char	OffsetFlag;
  ///�ɽ��۸�
  double	TradePrice;
  ///�ɽ�����
  int	TradeVolume;
  //�ɽ����
  char TradeID[TraderTradeIDTypeLen];
  // �ر�ʱ��
  struct timeval UpdateTime;
};

///����ǰ
#define INSTRUMENT_STATUS_BEFORE_TRADING '0'
///�ǽ���
#define INSTRUMENT_STATUS_NO_TRADING '1'
///��������
#define INSTRUMENT_STATUS_CONTINOUS '2'
///���Ͼ��۱���
#define INSTRUMENT_STATUS_AUCTION_ORDERING '3'
///���Ͼ��ۼ۸�ƽ��
#define INSTRUMENT_STATUS_AUCTION_BALANCE '4'
///���Ͼ��۴��
#define INSTRUMENT_STATUS_AUCTION_MATCH '5'
///����
#define INSTRUMENT_STATUS_CLOSED '6'

struct instrument_status_def{
	///��Լ����
	char InstrumentID [31];
  char InstrumentStatus;
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
#define TRADER_POSITION_TYPE_OPEN_T3 '8'

typedef char mduser_instrument[32];

#define USDCNH "USD.CNH"

#define OZ 31.1034768f

#endif //_TRADER_DATA_H_

