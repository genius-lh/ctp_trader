#ifndef _TRADER_MDUSER_RECORD_H_
#define _TRADER_MDUSER_RECORD_H_

struct trader_mduser_tick_buff_def {
  char instrumentId[30];
  char tradingDay[9];
  char updateTime[9];
  int  updateMillisec;
	double upperLimitPrice;
	double lowerLimitPrice;
	double lastPrice;
	int volume;
	double bidPrice1;
	int bidVolume1;
	double askPrice1;
	int askVolume1;
	double bidPrice2;
	int bidVolume2;
	double askPrice2;
	int askVolume2;
	double bidPrice3;
	int bidVolume3;
	double askPrice3;
	int askVolume3;
	double bidPrice4;
	int bidVolume4;
	double askPrice4;
	int askVolume4;
	double bidPrice5;
	int bidVolume5;
	double askPrice5;
	int askVolume5;
};

typedef struct trader_mduser_tick_buff_def trader_mduser_tick_buff;

struct trader_mduser_record_def{

  void* dickTick;
  int commitCount;
  int linePos;
  struct trader_mduser_tick_buff_def* tickBuffer;
  char csvFile[256];
  char tradingDay[9];
};

typedef struct trader_mduser_record_def trader_mduser_record;

extern void trader_mduser_record_init(trader_mduser_record* self);
extern void trader_mduser_record_on_tick(trader_mduser_record* self, struct trader_mduser_tick_buff_def* pMarketData);
extern void trader_mduser_record_flush(trader_mduser_record* self);
extern void trader_mduser_api_gf_record_free(trader_mduser_record* self);

#endif //_TRADER_MDUSER_RECORD_H_

