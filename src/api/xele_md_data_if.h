/**
 * @file xele_md_data_if.h
 * @brief Data interface
 * This file defines the format of sending
 * and receiving data on a CQDS.
 */
//
// Created by changfengc on 4/30/19.
//

#ifndef INCLUDE_XELE_MD_XELE_MD_DATA_IF_H_
#define INCLUDE_XELE_MD_XELE_MD_DATA_IF_H_

#include <stdint.h>

#define MESSAGE_MARKET_DATA        0x01
#define MESSAGE_DEPTH              0x02
#define MESSAGE_MARKET_DATA_STATIC 0x03
#define MESSAGE_HEART_BEAT    0x00
#define XELE_MD_DATA_VERSION       0x01

/////////////////////////////////////////////////////////////////////////
/// TXeleMdPriceType是一个价格类型
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdPriceType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdRatioType是一个比率类型
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdRatioType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdMoneyType是一个资金类型
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdMoneyType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdLargeVolumeType是一个大额数量类型
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdLargeVolumeType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdTimeType是一个时间类型
/////////////////////////////////////////////////////////////////////////
typedef char TXeleMdTimeType[9];

/////////////////////////////////////////////////////////////////////////
/// TXeleMdMillisecType是一个最后修改毫秒类型
/////////////////////////////////////////////////////////////////////////
typedef int TXeleMdMillisecType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdVolumeType是一个数量类型
/////////////////////////////////////////////////////////////////////////
typedef int TXeleMdVolumeType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdShortInstrumentIDType是一个合约代码类型
/////////////////////////////////////////////////////////////////////////
typedef char TXeleMdShortInstrumentIDType[20];

#pragma pack(push, 1)

struct CXeleCffexMdMessageHead {
  uint8_t Version;
  uint8_t Type;
  uint16_t Length;
};

///一档行情
struct CXeleCffexLevelOneMarketData {
  ///合约代码
  TXeleMdShortInstrumentIDType InstrumentID;
  ///最后修改时间
  TXeleMdTimeType UpdateTime;
  ///最后修改毫秒
  TXeleMdMillisecType UpdateMillisec;
  ///数量
  TXeleMdVolumeType Volume;
  ///最新价
  TXeleMdPriceType LastPrice;
  ///成交金额
  TXeleMdMoneyType Turnover;
  ///持仓量
  TXeleMdLargeVolumeType OpenInterest;
  ///申买价一
  TXeleMdPriceType BidPrice;
  ///申卖价一
  TXeleMdPriceType AskPrice;
  ///申买量一
  TXeleMdVolumeType BidVolume;
  ///申卖量一
  TXeleMdVolumeType AskVolume;
};

///行情静态属性
struct CXeleCffexMarketDataStaticField {
  ///合约代码
  TXeleMdShortInstrumentIDType InstrumentID;
  ///今开盘
  TXeleMdPriceType OpenPrice;
  ///最高价
  TXeleMdPriceType HighestPrice;
  ///最低价
  TXeleMdPriceType LowestPrice;
  ///今收盘
  TXeleMdPriceType ClosePrice;
  ///涨停板价
  TXeleMdPriceType UpperLimitPrice;
  ///跌停板价
  TXeleMdPriceType LowerLimitPrice;
  ///今结算
  TXeleMdPriceType SettlementPrice;
  ///今虚实度
  TXeleMdRatioType CurrDelta;
};
#pragma pack(pop)

#endif  // INCLUDE_XELE_MD_XELE_MD_DATA_IF_H_
