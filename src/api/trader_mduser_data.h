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
	///��Լ����
	char InstrumentID [31];
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
	///�����һ
	double	BidPrice2;
	///������һ
	int	BidVolume2;
	///������һ
	double	AskPrice2;
	///������һ
	int	AskVolume2;
	///�����һ
	double	BidPrice3;
	///������һ
	int	BidVolume3;
	///������һ
	double	AskPrice3;
	///������һ
	int	AskVolume3;
	///�����һ
	double	BidPrice4;
	///������һ
	int	BidVolume4;
	///������һ
	double	AskPrice4;
	///������һ
	int	AskVolume4;
	///�����һ
	double	BidPrice5;
	///������һ
	int	BidVolume5;
	///������һ
	double	AskPrice5;
	///������һ
	int	AskVolume5;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///���¼�
	double	LastPrice;
  ///�����ֶ�
  long Reserved;
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif // _TRADER_MDUSER_DATA_H_

