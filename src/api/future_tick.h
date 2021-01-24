#ifndef TICK_H_
#define TICK_H_



#define DATA_LENGTH 232
#define DATA_WITH_HEAD (DATA_LENGTH + 42)
#define DATA_WITHOUT_HEAD (DATA_LENGTH)

#pragma pack(1)
struct PriceLevel
{
    double     bid_price;             // 买价
    uint32_t   bid_volume;            // 买量
    double     ask_price;             // 卖价
    uint32_t   ask_volume;            // 卖量
};

struct ShfeDataField
{
    uint32_t   info_no;
    uint32_t   instrument_no;         // 合约编码
    uint32_t   quote_no;              // 当前合约增量行情编号
    uint32_t   update_msec;           // 交易所时间戳毫秒
    double     upper_limit;           // 涨停价
    double     lower_limit;           // 跌停价
    double     highest_price;         // 最高价
    double     lowest_price;          // 最低价
    double     last_price;            // 最新价
    uint32_t   volume;                // 成交量
    double     turnover;              // 成交金额
    double     open_interest;         // 持仓量
    PriceLevel depth[5];              // 5档行情
    uint32_t   packet_no;             // 报文编号
    char       product_class;         // 合约类型 '1'期货，'2'期权，'3'组合，'4'即期，'5'期转现
    char       instrument_id[22];     // 合约代码
    char       update_time[9];        // 交易所时间戳，"HH:MM:SS"格式
};
#pragma pack()

#endif

