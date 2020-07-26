/////////////////////////////////////////////////////////////////////////
///@company 南京艾科朗克信息科技有限公司
///@file CXeleFairApiStruct.h
///@brief 业务数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef _XELE_TD_FAIRAPI_STRUCT_H

#define _XELE_TD_FAIRAPI_STRUCT_H

#include "CXeleFtdcUserApiDataType.h"

#pragma pack(push, 1)

///FAIR 消息头
struct CXeleFairHeader {
  ///消息id
  TXeleFtdcMsgIdType        MessageId;
  ///客户端index
  TXeleFtdcClientIndexType  ClientIndex;
  ///客户端token
  TXeleFtdcClientTokenType  Token;
  ///消息序列号
  TXeleFtdcSequenceNoType   SeqNo;
  ///请求id
  TXeleFtdcReqIdType        RequestID;
};

///输入报单消息域
struct CXeleFairInputOrderFieldRaw{
  ///报单本地id
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///报单价格
  TXeleFtdcPriceType      LimitPrice;  
  ///合约代码
  TXeleFtdcInstruIDType   InstrumentID;
  ///数量
  TXeleFtdcVolumeTotalOriginalType   VolumeTotalOriginal;
  ///输入报单类型
  TXeleFtdcInsertType     InsertType;
  ///最小成交数量
  TXeleFtdcMinVolumeType   MinVolume;
  ///前置信息
  TXeleFtdcExchangeFrontEnumType    ExchangeFront;
  ///未用字段
  char                    _unused_1[2];
};
///输入报单报文消息: FAIR头+输入报单消息域
struct CXeleFairInputOrderMsg {
  ///消息id
  TXeleFtdcMsgIdType        MessageId;
  ///客户端index
  TXeleFtdcClientIndexType  ClientIndex;
  ///客户端token
  TXeleFtdcClientTokenType  Token;
  ///消息序列号
  TXeleFtdcSequenceNoType   SeqNo;
  ///请求id
  TXeleFtdcReqIdType        RequestID;
  
  ///报单本地id
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///报单价格
  TXeleFtdcPriceType      LimitPrice;  
  ///合约代码
  TXeleFtdcInstruIDType   InstrumentID;
  ///数量
  TXeleFtdcVolumeTotalOriginalType   VolumeTotalOriginal;
  ///输入报单类型
  TXeleFtdcInsertType     InsertType;
  ///最小成交数量
  TXeleFtdcMinVolumeType   MinVolume;
  ///前置信息
  TXeleFtdcExchangeFrontEnumType    ExchangeFront;
  ///未用字段
  char                    _unused_1[2];
};

//报单操作消息域
struct CXeleFairOrderActionFieldRaw {
  ///本地报单编号
  TXeleFtdcActionLocalNoType ActionLocalNo;
  ///被撤单柜台编码
  TXeleFtdcOrderSysNoType    OrderSysNo; 
  ///未用字段
  char _unused_1[28];
};

//报单操作报文消息: FAIR头+报单操作消息域
struct CXeleFairOrderActionMsg{
  ///消息id, 报单操作为103
  TXeleFtdcMsgIdType        MessageId;
  ///客户端index
  TXeleFtdcClientIndexType  ClientIndex;
  ///客户端token
  TXeleFtdcClientTokenType  Token;
  ///消息序列号
  TXeleFtdcSequenceNoType   SeqNo;
  ///请求id
  TXeleFtdcReqIdType        RequestID;

  ///本地报单编号
  TXeleFtdcActionLocalNoType  ActionLocalNo;
  ///被撤单柜台编码
  TXeleFtdcOrderSysNoType     OrderSysNo; 
  ///未用字段
  char _unused_1[28];
};

#pragma pack(pop)

#endif
