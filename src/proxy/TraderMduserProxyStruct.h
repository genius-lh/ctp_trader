#ifndef _TRADER_MDUSER_PROXY_STRUCT_H_
#define _TRADER_MDUSER_PROXY_STRUCT_H_
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct trader_tick_def trader_tick;

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
	///���¼�
	double	LastPrice;
  ///����ʱ��
  struct timeval ReceiveTime;
  ///�����ֶ�
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
  // ����
  trader_tick Tick;
};

#ifdef __cplusplus
}
#endif

#endif //_TRADER_MDUSER_PROXY_STRUCT_H_

