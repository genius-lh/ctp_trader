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
/// TXeleMdPriceType��һ���۸�����
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdPriceType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdRatioType��һ����������
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdRatioType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdMoneyType��һ���ʽ�����
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdMoneyType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdLargeVolumeType��һ�������������
/////////////////////////////////////////////////////////////////////////
typedef double TXeleMdLargeVolumeType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdTimeType��һ��ʱ������
/////////////////////////////////////////////////////////////////////////
typedef char TXeleMdTimeType[9];

/////////////////////////////////////////////////////////////////////////
/// TXeleMdMillisecType��һ������޸ĺ�������
/////////////////////////////////////////////////////////////////////////
typedef int TXeleMdMillisecType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdVolumeType��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int TXeleMdVolumeType;

/////////////////////////////////////////////////////////////////////////
/// TXeleMdShortInstrumentIDType��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char TXeleMdShortInstrumentIDType[20];

#pragma pack(push, 1)

struct CXeleCffexMdMessageHead {
  uint8_t Version;
  uint8_t Type;
  uint16_t Length;
};

///һ������
struct CXeleCffexLevelOneMarketData {
  ///��Լ����
  TXeleMdShortInstrumentIDType InstrumentID;
  ///����޸�ʱ��
  TXeleMdTimeType UpdateTime;
  ///����޸ĺ���
  TXeleMdMillisecType UpdateMillisec;
  ///����
  TXeleMdVolumeType Volume;
  ///���¼�
  TXeleMdPriceType LastPrice;
  ///�ɽ����
  TXeleMdMoneyType Turnover;
  ///�ֲ���
  TXeleMdLargeVolumeType OpenInterest;
  ///�����һ
  TXeleMdPriceType BidPrice;
  ///������һ
  TXeleMdPriceType AskPrice;
  ///������һ
  TXeleMdVolumeType BidVolume;
  ///������һ
  TXeleMdVolumeType AskVolume;
};

///���龲̬����
struct CXeleCffexMarketDataStaticField {
  ///��Լ����
  TXeleMdShortInstrumentIDType InstrumentID;
  ///����
  TXeleMdPriceType OpenPrice;
  ///��߼�
  TXeleMdPriceType HighestPrice;
  ///��ͼ�
  TXeleMdPriceType LowestPrice;
  ///������
  TXeleMdPriceType ClosePrice;
  ///��ͣ���
  TXeleMdPriceType UpperLimitPrice;
  ///��ͣ���
  TXeleMdPriceType LowerLimitPrice;
  ///�����
  TXeleMdPriceType SettlementPrice;
  ///����ʵ��
  TXeleMdRatioType CurrDelta;
};
#pragma pack(pop)

#endif  // INCLUDE_XELE_MD_XELE_MD_DATA_IF_H_
