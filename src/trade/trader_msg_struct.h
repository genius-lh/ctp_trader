#ifndef _TRADER_MSG_STRUCT_H_
#define _TRADER_MSG_STRUCT_H_

#include "trader_data.h"

enum trader_cmd_type {
  TRADER_CMD_TYPE_LOGIN = 0,
  TRADER_CMD_TYPE_UPDATE,
  TRADER_CMD_TYPE_UPDATE_ALL,
  TRADER_CMD_TYPE_QUERY_INSTRUMENT,
  TRADER_CMD_TYPE_QUERY_POSITION,
  TRADER_CMD_TYPE_CLOSE_ALL,
  TRADER_CMD_TYPE_RESET_POSITION,
  TRADER_CMD_TYPE_LOGOUT
};


// 仓位
typedef struct trade_position_def trade_position;
struct trade_position_def {
  // 合约一
  char T1[TraderInstrumentIDTypeLen];
  // 合约二
  char T2[TraderInstrumentIDTypeLen];
  // 策略号
  int StageID;
  // 方向
  char	Direction;
  // 实际价差
  double RealPrice;
  // 目标价差
  double ExpPrice;
  // 数量
  int Volume;
};

#define TRADER_STG_GUZHI 0
#define TRADER_STG_GUZHI_MM 1
#define TRADER_STG_SHANGPIN 2
#define TRADER_STG_SHANGPIN_2 3
#define TRADER_STG_OPTION 4
#define TRADER_STG_OPTION_MM 5

#define IS_STG_GUZHI(_stg) ((TRADER_STG_GUZHI == (_stg)) || (TRADER_STG_GUZHI_MM == (_stg)))
#define IS_STG_SHANGPIN(_stg) ((TRADER_STG_SHANGPIN == (_stg)) || (TRADER_STG_SHANGPIN_2 == (_stg)))
#define IS_STG_MM(_stg) ((TRADER_STG_GUZHI_MM == (_stg)) || (TRADER_STG_OPTION_MM == (_stg)))
#define IS_STG_OPTION(_stg) ((TRADER_STG_OPTION == (_stg)) || (TRADER_STG_OPTION_MM == (_stg)))

#define TRADER_CMD_UPDATE_NUM 32

// 更新
struct trader_cmd_update_req_def {
  short num;
  struct {
    int StageId; // 策略编号
    char T1[TraderInstrumentIDTypeLen]; // 腿一
    char T2[TraderInstrumentIDTypeLen]; // 腿二
    int	MP; // 当前策略最大仓位
    int	Hold; // 低于当前值，不平仓
    int	Mult; // 每单下单量
    double	KTOpen; // 腿一做多开仓价差
    double	KTClose; // 腿一做多 平仓 价差
    double	DTOpen; // 腿一做空 开仓价差
    double	DTClose; // 腿一做空 平仓价差
    int	T1Over; // 腿一超价
    int	T1Wait; // 腿一等待
    int	T2Over; // 腿二超价
    int	T2Wait; // 腿二等待
    int	PermitVol; // 腿二对手价的挂单数量
    int STG; // 策略0 - 股指 1 - 股指MM 2 - 商品
    int Used; //启用标致
    int AutoType; //自动报单
    double AutoKTOpen;
    double AutoKTClose;
    double AutoDTOpen;
    double AutoDTClose;
    
    double T1Weight;
    double T2Weight;
    int T1Ratio;
    int T2Ratio;
    int NightClosingTime;
    int TriggerType;

    double PriceTick; // 一跳价格
    int IsSHFE; // 是否是上期
    double T2PriceTick; // 一跳价格
    int T2IsSHFE; // 是否是上期
    char T1ExchangeID[9];
    char T2ExchangeID[9];
  } stage[TRADER_CMD_UPDATE_NUM];
};


// 登录
struct trader_cmd_login_req_def {
	char	UserID[TraderUserIDTypeLen];
	char	Password[TraderPasswordTypeLen];
};

struct trader_cmd_reset_req_def {
	int StageID;
  char Direction;
  int Volume;
};

struct trader_cmd_query_instrument_rsp_def {
  int nNumber;
  char Instruments[];
};

struct trader_cmd_query_position_rsp_def {
  int nNumber;
  trade_position Positions[];
};

typedef struct trader_msg_req_def trader_msg_req_struct;
typedef struct trader_msg_rsp_def trader_msg_rsp_struct;

struct trader_msg_req_def {
  enum trader_cmd_type cmd;
  void* req;
  char traceNo[36+1];
  union trader_msg_req_body {
    struct trader_cmd_update_req_def update;
    struct trader_cmd_login_req_def login;
    struct trader_cmd_reset_req_def reset;
    char reserved[1];
  } body;
};

struct trader_msg_rsp_def {
  enum trader_cmd_type cmd;
  void* req;
  char traceNo[36+1];
  int  ErrorID;
  char ErrorMsg[100];
  int recNum;
  char buff[9000];
};

typedef struct trader_msg_mduser_struct_def trader_msg_mduser_struct;
typedef struct trader_msg_trader_struct_def trader_msg_trader_struct;

enum trader_recv_type {
  TRADER_RECV_MDUSER_TYPE_LOGIN = 0,
  TRADER_RECV_MDUSER_TYPE_INFO,
  TRADER_RECV_MDUSER_TYPE_SUB,
  TRADER_RECV_MDUSER_TYPE_ERROR,
  TRADER_RECV_TRADER_ON_QRY_INSTRUMENT,
  TRADER_RECV_TRADER_ON_QRY_USER_INVESTOR,
  TRADER_RECV_TRADER_ON_QRY_INVESTOR_POSITION,
  TRADER_RECV_TRADER_ON_LOGIN,
  TRADER_RECV_TRADER_ON_ORDER,
  TRADER_RECV_TRADER_ON_TRADE,
  TRADER_RECV_TRADER_ORDER_ERROR
};

struct trader_msg_mduser_struct_def {
  enum trader_recv_type type;
  int ErrorCd;
  char ErrorMsg[81];
  // 行情
  trader_tick oTick;
};


typedef struct investor_position_def investor_position;

struct investor_position_def {
  char InstrumentID[TraderInstrumentIDTypeLen];
  char PosiDirection;
  int IsSHFE;
  char PositionDate;
  int YdPosition;
  int TodayPosition;
  int Position;
  int LongFrozen;
  int ShortFrozen;
};

struct trader_msg_trader_struct_def {
  enum trader_recv_type type;
  int ErrorCd;
  char ErrorMsg[81];
  // 成交信息
  trader_trade oTrade;
  // 订单信息
  trader_order oOrder;
  // 持仓信息
  investor_position oPosition;
  // 行情信息
  char InstrumentID[TraderInstrumentIDTypeLen];
  char ExchangeID[9];
  double PriceTick;
  int bIsLast;

};



#endif //_TRADER_MSG_STRUCT_H_

