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


// ��λ
typedef struct trade_position_def trade_position;
struct trade_position_def {
  // ��Լһ
  char T1[TraderInstrumentIDTypeLen];
  // ��Լ��
  char T2[TraderInstrumentIDTypeLen];
  // ���Ժ�
  int StageID;
  // ����
  char	Direction;
  // ʵ�ʼ۲�
  double RealPrice;
  // Ŀ��۲�
  double ExpPrice;
  // ����
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

// ����
struct trader_cmd_update_req_def {
  short num;
  struct {
    int StageId; // ���Ա��
    char T1[TraderInstrumentIDTypeLen]; // ��һ
    char T2[TraderInstrumentIDTypeLen]; // �ȶ�
    int	MP; // ��ǰ��������λ
    int	Hold; // ���ڵ�ǰֵ����ƽ��
    int	Mult; // ÿ���µ���
    double	KTOpen; // ��һ���࿪�ּ۲�
    double	KTClose; // ��һ���� ƽ�� �۲�
    double	DTOpen; // ��һ���� ���ּ۲�
    double	DTClose; // ��һ���� ƽ�ּ۲�
    int	T1Over; // ��һ����
    int	T1Wait; // ��һ�ȴ�
    int	T2Over; // �ȶ�����
    int	T2Wait; // �ȶ��ȴ�
    int	PermitVol; // �ȶ����ּ۵Ĺҵ�����
    int STG; // ����0 - ��ָ 1 - ��ָMM 2 - ��Ʒ
    int Used; //���ñ���
    int AutoType; //�Զ�����
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

    double PriceTick; // һ���۸�
    int IsSHFE; // �Ƿ�������
    double T2PriceTick; // һ���۸�
    int T2IsSHFE; // �Ƿ�������
    char T1ExchangeID[9];
    char T2ExchangeID[9];
  } stage[TRADER_CMD_UPDATE_NUM];
};


// ��¼
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
  // ����
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
  // �ɽ���Ϣ
  trader_trade oTrade;
  // ������Ϣ
  trader_order oOrder;
  // �ֲ���Ϣ
  investor_position oPosition;
  // ������Ϣ
  char InstrumentID[TraderInstrumentIDTypeLen];
  char ExchangeID[9];
  double PriceTick;
  int bIsLast;

};



#endif //_TRADER_MSG_STRUCT_H_

