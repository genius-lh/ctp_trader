#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct MarketDepthUpdate 
{
    int8_t          ETH_IP_UDP[42];
    int8_t          InstrumentID[31];
    int8_t          UpdateTime[9];
    int32_t         UpdateMillisec;
    int32_t         Volume;
    double          LastPrice;
    double          Turnover;
    double          OpenInterest;
    int32_t         BidVolume1;
    int32_t         AskVolume1;
    int32_t         BidVolume2;
    int32_t         AskVolume2;
    int32_t         BidVolume3;
    int32_t         AskVolume3;
    int32_t         BidVolume4;
    int32_t         AskVolume4;
    int32_t         BidVolume5;
    int32_t         AskVolume5;
    double          BidPrice1;
    double          AskPrice1;
    double          BidPrice2;
    double          AskPrice2;
    double          BidPrice3;
    double          AskPrice3;
    double          BidPrice4;
    double          AskPrice4;
    double          BidPrice5;
    double          AskPrice5;
    double          HighestPrice;
    double          LowestPrice;
    double          OpenPrice;
    double          ClosePrice;
    double          UpperLimitPrice;
    double          LowerLimitPrice;
    double          SettlementPrice;
    double          CurrDelta;
}cffex_md_t;
#pragma pack()

#ifdef __cplusplus
}
#endif
