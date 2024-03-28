#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_mduser_api_ef_vi.h"

#include "trader_data.h"

typedef struct __attribute__((__packed__)) 
{
  unsigned short  Length;					//包长度
  int 			PacketNo;				//全0
  unsigned int	ChangeNo;				//增量编号
  short			InstrumentNo;			//合约编码
  char			InstrumentID[10];		//合约
  unsigned int	UpdateTime;				//最后更新时间(秒)
  unsigned short	UpdateMillisec;			//最后更新时间(毫秒)
  int				Volume;
  int				OpenInterest;
  int				BidVolume1;
  int				BidVolume2;
  int				BidVolume3;
  int				BidVolume4;
  int				BidVolume5;
  int				AskVolume1;
  int				AskVolume2;
  int				AskVolume3;
  int				AskVolume4;
  int				AskVolume5;
  double			LastPrice;
  double			BidPrice1;
  double			BidPrice2;
  double			BidPrice3;
  double			BidPrice4;
  double			BidPrice5;
  double			AskPrice1;
  double			AskPrice2;
  double			AskPrice3;
  double			AskPrice4;
  double			AskPrice5;
  double			Turnover;
}dzqh_fut_md_t;

extern int md_package_size_dzfut();
extern int md_package_id_dzfut();
extern int md_package_fill_dzfut(void* tick, void* obj);

typedef struct __attribute__((__packed__)) efh3_2_fut_lev1
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_symbol[8];			///<合约
	char			m_update_time_h;		///<最后更新的时间hh
	char			m_update_time_m;		///<最后更新的时间mm
	char			m_update_time_s;		///<最后更新的时间ss
	unsigned short  m_millisecond;		    ///<最后更新的毫秒数         

	double			m_last_px;				///<最新价
	unsigned int	m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量
	double			m_bid_px;				///<买一价
	unsigned int	m_bid_share;			///<买一量
	double			m_ask_px;				///<卖一价
	unsigned int	m_ask_share;			///<卖一量
	char            m_reserve;  			///<保留字段             
}efh3_2_fut_lev1_t;

extern int md_package_size_efh32_l1();
extern int md_package_id_efh32_l1();
extern int md_package_fill_efh32_l1(void* tick, void* obj);


typedef struct __attribute__((__packed__)) cffex_l2
{
    char          PackageHeader[20];
    char          Tag2439[4];
    char          InstrumentID[31];
    char          UpdateTime[9];
    int           UpdateMillisec;
    char          TAG2432[4];
    long long     OpenPrice;
    long long     HighestPrice;
    long long     LowestPrice;
    long long     ClosePrice;
    long long     UpperLimitPrice;
    long long     LowerLimitPrice;
    long long     SettlementPrice;
    long long     CurrDelta;
    char          Tag2433[4];
    char          Val2433[0x1c];
    char          Tag2434[4];
    char          Val2434[0x18];
    char          Tag2435[4];
    char          Val2435[0x18];
    char          Tag2436[4];
    char          Val2436[0x18];
    char          Tag2437[4];
    char          Val2437[0x18];
    char          Tag2438[4];
    char          Val2438[0x18];
}cffex_l2_t;

typedef struct __attribute__((__packed__)) cffex_l2_data
{
  long long    BidPrice1;
  int          BidVolume1;
  long long    AskPrice1;
  int          AskVolume1;
}cffex_l2_data_t;

extern int md_package_size_cffex_l2();
extern int md_package_id_cffex_l2();
extern int md_package_fill_cffex_l2(void* tick, void* obj);

typedef struct __attribute__((__packed__)) efh3_0_fut_lev1
{
	unsigned int	m_sequence;				///<会话编号
	char			m_exchange_id;			///<市场  0 表示中金  1表示上期
	char			m_channel_id;			///<通道编号
	char			m_quote_flag;			///<行情标志  0 无time sale,无lev1, 
											///           1 有time sale,无lev1, 
											///           2 无time sale,有lev1, 
											///           3 有time sale,有lev1
	char			m_symbol[8];			///<合约
	char			m_update_time[9];		///<最后更新时间(秒)
	int				m_millisecond;			///<最后更新时间(毫秒)

	double			m_last_px;				///<最新价
	int				m_last_share;			///<最新成交量
	double			m_total_value;			///<成交金额
	double			m_total_pos;			///<持仓量
	double			m_bid_px;				///<最新买价
	int				m_bid_share;			///<最新买量
	double			m_ask_px;				///<最新卖价
	int				m_ask_share;			///<最新卖量
}efh3_0_fut_lev1_t;

extern int md_package_size_efh30_l1();
extern int md_package_id_efh30_l1();
extern int md_package_fill_efh30_l1(void* tick, void* obj);

typedef struct __attribute__((__packed__)) xqn_fut_lev1
{
    char Market[3];					//市场说明
    char Status;					//字段状态
    char Instrument[7];				//合约代码
    char UpdateTime[9];				//更新时间
    int UpdateMillisec;				//最后更新时间毫秒
    double LastPrice;				//最新价
    int	Volume;					//成交量
    double Turnover;				//成交金额
    double OpenInterest;				//持仓量
    double BidPrice;				//买一价
    int BidVolume;					//买一量
    double AskPrice;				//卖一价
    int AskVolume;					//卖一量
}xqn_fut_lev1_t;

extern int md_package_size_xqn_l1();
extern int md_package_id_xqn_l1();
extern int md_package_fill_xqn_l1(void* tick, void* obj);

typedef struct __attribute__((__packed__)) 
{
	unsigned char 	Flag;					//协议标志
	char 			TypeID;					//协议版本
	unsigned short  Length;					//包长度
	int 			PacketNo;				//全0
	unsigned int	ChangeNo;				//增量编号
	short			InstrumentNo;			//合约编码
	char			InstrumentID[10];		//合约
	unsigned int	UpdateTime;				//最后更新时间(秒)
	unsigned short	UpdateMillisec;			//最后更新时间(毫秒)
	int				Volume;
	int				OpenInterest;
	int				BidVolume1;
	int				BidVolume2;
	int				BidVolume3;
	int				BidVolume4;
	int				BidVolume5;
	int				AskVolume1;
	int				AskVolume2;
	int				AskVolume3;
	int				AskVolume4;
	int				AskVolume5;
	double			LastPrice;
	double			BidPrice1;
	double			BidPrice2;
	double			BidPrice3;
	double			BidPrice4;
	double			BidPrice5;
	double			AskPrice1;
	double			AskPrice2;
	double			AskPrice3;
	double			AskPrice4;
	double			AskPrice5;
	double			Turnover;
}dzqh_zbp05_md_t;

extern int md_package_size_dzqh_zbp05();
extern int md_package_id_dzqh_zbp05();
extern int md_package_fill_dzqh_zbp05(void* tick, void* obj);

typedef struct __attribute__((__packed__)) 
{
	unsigned char 	Flag;					//协议标志
	char 			TypeID;					//协议版本
	unsigned short  Length;					//包长度
	int 			PacketNo;				//全0
	unsigned int	ChangeNo;				//增量编号
	short			InstrumentNo;			//合约编码
	char			InstrumentID[10];		//合约
	unsigned int	UpdateTime;				//最后更新时间(秒)
	unsigned short	UpdateMillisec;			//最后更新时间(毫秒)
	int				Volume;
	int				OpenInterest;
	int				BidVolume1;
	int				BidVolume2;
	int				BidVolume3;
	int				BidVolume4;
	int				BidVolume5;
	int				AskVolume1;
	int				AskVolume2;
	int				AskVolume3;
	int				AskVolume4;
	int				AskVolume5;
	double			LastPrice;
	double			BidPrice1;
	double			BidPrice2;
	double			BidPrice3;
	double			BidPrice4;
	double			BidPrice5;
	double			AskPrice1;
	double			AskPrice2;
	double			AskPrice3;
	double			AskPrice4;
	double			AskPrice5;
	double			Turnover;
}dzqh_zbp06_md_t;

extern int md_package_size_dzqh_zbp06();
extern int md_package_id_dzqh_zbp06();
extern int md_package_fill_dzqh_zbp06(void* tick, void* obj);

#ifdef __cplusplus
}
#endif

int md_package_size_dzfut()
{
  return (int)sizeof(dzqh_fut_md_t);
}

int md_package_id_dzfut()
{
  return (int)offsetof(dzqh_fut_md_t, InstrumentID);
}

int md_package_fill_dzfut(void* tick, void* obj)
{
  dzqh_fut_md_t* pMarketData = (dzqh_fut_md_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  struct tm now;
  time_t current = (time_t)pMarketData->UpdateTime;
  localtime_r(&current, &now);    

  strcpy(pTick->InstrumentID, (char*)pMarketData->InstrumentID);
  snprintf(pTick->TradingDay, sizeof(pTick->TradingDay), "%04d%02d%02d", now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(pTick->UpdateTime, sizeof(pTick->UpdateTime), "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = (long)pMarketData->ChangeNo;
  return 0;
}

int md_package_size_efh32_l1()
{
  return (int)sizeof(efh3_2_fut_lev1_t);
}

int md_package_id_efh32_l1()
{
  return (int)offsetof(efh3_2_fut_lev1_t, m_symbol);
}

int md_package_fill_efh32_l1(void* tick, void* obj)
{
  efh3_2_fut_lev1_t* pMarketData = (efh3_2_fut_lev1_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  strcpy(pTick->InstrumentID, pMarketData->m_symbol);
  strcpy(pTick->TradingDay, "20230101");
  snprintf(pTick->UpdateTime, sizeof(pTick->UpdateTime), "%02d:%02d:%02d",
    (int)pMarketData->m_update_time_h,
    (int)pMarketData->m_update_time_m,
    (int)pMarketData->m_update_time_s
  );
  pTick->UpdateMillisec = pMarketData->m_millisecond;
  pTick->BidPrice1 = pMarketData->m_bid_px;
  pTick->BidVolume1 = pMarketData->m_bid_share;
  pTick->AskPrice1 = pMarketData->m_ask_px;
  pTick->AskVolume1 = pMarketData->m_ask_share;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->m_last_px;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = pMarketData->m_sequence;
  return 0;
}

int md_package_size_efh30_l1()
{
  return (int)sizeof(efh3_0_fut_lev1_t);
}

int md_package_id_efh30_l1()
{
  return (int)offsetof(efh3_0_fut_lev1_t, m_symbol);
}

int md_package_fill_efh30_l1(void* tick, void* obj)
{
  efh3_0_fut_lev1_t* pMarketData = (efh3_0_fut_lev1_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  strncpy(pTick->InstrumentID, pMarketData->m_symbol, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, "20230101", sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pMarketData->m_update_time, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pMarketData->m_millisecond;
  pTick->BidPrice1 = pMarketData->m_bid_px;
  pTick->BidVolume1 = pMarketData->m_bid_share;
  pTick->AskPrice1 = pMarketData->m_ask_px;
  pTick->AskVolume1 = pMarketData->m_ask_share;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->m_last_px;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = pMarketData->m_sequence;
  return 0;
}

int md_package_size_xqn_l1()
{
  return (int)sizeof(xqn_fut_lev1_t);
}

int md_package_id_xqn_l1()
{
  return (int)offsetof(xqn_fut_lev1_t, Instrument);

}

int md_package_fill_xqn_l1(void* tick, void* obj)
{
  xqn_fut_lev1_t* pMarketData = (xqn_fut_lev1_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  strncpy(pTick->InstrumentID, pMarketData->Instrument, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, "20230101", sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, pMarketData->UpdateTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice;
  pTick->BidVolume1 = pMarketData->BidVolume;
  pTick->AskPrice1 = pMarketData->AskPrice;
  pTick->AskVolume1 = pMarketData->AskVolume;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 0;
  return 0;
}

extern int cffex_l2_state_load();
extern int cffex_l2_state_init(void* original_data, void* masked_data);
extern int cffex_l2_state_unmask(char* data, int len);
extern int cffex_l2_state_ready();
extern char* cffex_l2_state_date();


#include <byteswap.h>

static inline int conv_int(int x)
{
  return bswap_32(x);
}

static inline double conv_double(long long x)
{
  union {
    long long ll;
    double d;
  } r;

  r.ll = bswap_64(x);

  return r.d;
}

int md_package_size_cffex_l2()
{
  return (int)sizeof(cffex_l2_t);
}

int md_package_id_cffex_l2()
{
  cffex_l2_state_load();
  return (int)offsetof(cffex_l2_t, InstrumentID);
}

int md_package_fill_cffex_l2(void* tick, void* obj)
{
  cffex_l2_t* pMarketData = (cffex_l2_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  
  cffex_l2_state_init((void*)&pMarketData->OpenPrice, (void*)&pMarketData->Val2433[0]);

  if(!cffex_l2_state_ready()){
    return -1;
  }
  
  cffex_l2_state_unmask(pMarketData->Val2434, sizeof(pMarketData->Val2434));
  cffex_l2_state_unmask(pMarketData->Val2433, sizeof(pMarketData->Val2433));

  cffex_l2_data_t* price = (cffex_l2_data_t*)pMarketData->Val2434;
  cffex_l2_data_t* price2 = (cffex_l2_data_t*)pMarketData->Val2433;
  
  strncpy(pTick->InstrumentID, (char*)pMarketData->InstrumentID, sizeof(pTick->InstrumentID));
  strncpy(pTick->TradingDay, cffex_l2_state_date(), sizeof(pTick->TradingDay));
  strncpy(pTick->UpdateTime, (char*)pMarketData->UpdateTime, sizeof(pTick->UpdateTime));
  pTick->UpdateMillisec = conv_int(pMarketData->UpdateMillisec);
  pTick->BidPrice1 = conv_double(price->BidPrice1);
  pTick->BidVolume1 = conv_int(price->BidVolume1);
  pTick->AskPrice1 = conv_double(price->AskPrice1);
  pTick->AskVolume1 = conv_int(price->AskVolume1);
  pTick->UpperLimitPrice = conv_double(pMarketData->UpperLimitPrice);
  pTick->LowerLimitPrice = conv_double(pMarketData->LowerLimitPrice);
  pTick->LastPrice = conv_double(price2->BidPrice1);
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = 0;
  
  return 0;
}

int md_package_size_dzqh_zbp05()
{
  return (int)sizeof(dzqh_zbp05_md_t);
}

int md_package_id_dzqh_zbp05()
{
  return (int)offsetof(dzqh_zbp05_md_t, InstrumentID);
}

int md_package_fill_dzqh_zbp05(void* tick, void* obj)
{
  dzqh_zbp05_md_t* pMarketData = (dzqh_zbp05_md_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  struct tm now;
  time_t current = (time_t)pMarketData->UpdateTime;
  localtime_r(&current, &now);    

  strcpy(pTick->InstrumentID, (char*)pMarketData->InstrumentID);
  snprintf(pTick->TradingDay, sizeof(pTick->TradingDay), "%04d%02d%02d", now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(pTick->UpdateTime, sizeof(pTick->UpdateTime), "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = (long)pMarketData->ChangeNo;
  return 0;
}

int md_package_size_dzqh_zbp06()
{
  return (int)sizeof(dzqh_zbp06_md_t);
}

int md_package_id_dzqh_zbp06()
{
  return (int)offsetof(dzqh_zbp06_md_t, InstrumentID);
}

int md_package_fill_dzqh_zbp06(void* tick, void* obj)
{
  dzqh_zbp06_md_t* pMarketData = (dzqh_zbp06_md_t*)obj;
  trader_tick* pTick = (trader_tick*)tick;
  struct tm now;
  time_t current = (time_t)pMarketData->UpdateTime;
  localtime_r(&current, &now);    

  strcpy(pTick->InstrumentID, (char*)pMarketData->InstrumentID);
  snprintf(pTick->TradingDay, sizeof(pTick->TradingDay), "%04d%02d%02d", now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(pTick->UpdateTime, sizeof(pTick->UpdateTime), "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
  pTick->UpdateMillisec = pMarketData->UpdateMillisec;
  pTick->BidPrice1 = pMarketData->BidPrice1;
  pTick->BidVolume1 = pMarketData->BidVolume1;
  pTick->AskPrice1 = pMarketData->AskPrice1;
  pTick->AskVolume1 = pMarketData->AskVolume1;
  pTick->UpperLimitPrice = 0;
  pTick->LowerLimitPrice = 0;
  pTick->LastPrice = pMarketData->LastPrice;
  gettimeofday(&pTick->ReceiveTime, NULL);
  pTick->Reserved = (long)pMarketData->ChangeNo;
  return 0;
}

void trader_mduser_api_ef_vi_ops_init(trader_mduser_api_ef_vi_ops* ops, int type)
{
  if(0 == type){
    ops->m_md_size = md_package_size_dzfut();
    ops->m_md_id_pos = md_package_id_dzfut();
    ops->md_fill = md_package_fill_dzfut;
  }else if(1 == type){
    ops->m_md_size = md_package_size_efh32_l1();
    ops->m_md_id_pos = md_package_id_efh32_l1();
    ops->md_fill = md_package_fill_efh32_l1;
  }else if(2 == type){
    ops->m_md_size = md_package_size_cffex_l2();
    ops->m_md_id_pos = md_package_id_cffex_l2();
    ops->md_fill = md_package_fill_cffex_l2;
  }else if(3 == type){
    ops->m_md_size = md_package_size_efh30_l1();
    ops->m_md_id_pos = md_package_id_efh30_l1();
    ops->md_fill = md_package_fill_efh30_l1;
  }else if(4 == type){
    ops->m_md_size = md_package_size_xqn_l1();
    ops->m_md_id_pos = md_package_id_xqn_l1();
    ops->md_fill = md_package_fill_xqn_l1;
  }else if(5 == type){
    ops->m_md_size = md_package_size_dzqh_zbp05();
    ops->m_md_id_pos = md_package_id_dzqh_zbp05();
    ops->md_fill = md_package_fill_dzqh_zbp05;
  }else if(6 == type){
    ops->m_md_size = md_package_size_dzqh_zbp06();
    ops->m_md_id_pos = md_package_id_dzqh_zbp06();
    ops->md_fill = md_package_fill_dzqh_zbp06;
  }else{
    exit(-1);
  }
}

