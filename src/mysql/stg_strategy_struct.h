
#ifndef _STG_STRATEGY_STRUCT_H_
#define _STG_STRATEGY_STRUCT_H_

struct strategy_instrument_def{
  char instrumentid[30+1];
  char exchangeid[8+1];
  char instrumentname[60+1];
  char exchangeinstid[30+1];
  char productid[30+1];
  char productclass[1+1];
  long deliveryyear;
  long deliverymonth;
  long maxmarketordervolume;
  long minmarketordervolume;
  long maxlimitordervolume;
  long minlimitordervolume;
  long volumemultiple;
  double pricetick;
  char createdate[8+1];
  char opendate[8+1];
  char expiredate[8+1];
  char startdelivdate[8+1];
  char enddelivdate[8+1];
  char instlifephase[1+1];
  long istrading;
  char positiontype[1+1];
  char positiondatetype[1+1];
  double longmarginratio;
  double shortmarginratio;
  char maxmarginsidealgorithm[1+1];
  char underlyinginstrid[30+1];
  double strikeprice;
  char optionstype[1+1];
  double underlyingmultiple;
  char combinationtype[1+1];
};

struct strategy_config_def{
  char configkey[20+1];
  char configvalue[40+1];
  char configdesc[100+1];
};

struct strategy_market_data_tick_def{
  char instrumentid[30+1];
  char exchangeid[8+1];
  char tradingday[8+1];
  char updatetime[8+1];
  long updatemillisec;
  double bidprice1;
  double askprice1;
  long bidvolume1;
  long askvolume1;
};

typedef struct strategy_config_def strategy_config_t;
typedef struct strategy_instrument_def strategy_instrument_t;
typedef struct strategy_market_data_tick_def strategy_market_data_tick_t;

#endif //_STG_STRATEGY_STRUCT_H_

