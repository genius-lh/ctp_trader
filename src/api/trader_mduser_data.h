#ifndef _TRADER_MDUSER_DATA_H_
#define _TRADER_MDUSER_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct trader_mduser_data_def trader_mduser_data;
typedef void (*trader_mduser_data_conv_func)(trader_mduser_data* data, const char* data, int len, void* arg);

#pragma pack(push, 1)
struct trader_mduser_data_def
{
	///合约代码
	char InstrumentID [31];
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
	///申买价一
	double	BidPrice2;
	///申买量一
	int	BidVolume2;
	///申卖价一
	double	AskPrice2;
	///申卖量一
	int	AskVolume2;
	///申买价一
	double	BidPrice3;
	///申买量一
	int	BidVolume3;
	///申卖价一
	double	AskPrice3;
	///申卖量一
	int	AskVolume3;
	///申买价一
	double	BidPrice4;
	///申买量一
	int	BidVolume4;
	///申卖价一
	double	AskPrice4;
	///申卖量一
	int	AskVolume4;
	///申买价一
	double	BidPrice5;
	///申买量一
	int	BidVolume5;
	///申卖价一
	double	AskPrice5;
	///申卖量一
	int	AskVolume5;
	///涨停板价
	double	UpperLimitPrice;
	///跌停板价
	double	LowerLimitPrice;
	///最新价
	double	LastPrice;
  ///保留字段
  long Reserved;
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif // _TRADER_MDUSER_DATA_H_

