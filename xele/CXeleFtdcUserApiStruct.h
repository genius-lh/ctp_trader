/////////////////////////////////////////////////////////////////////////
///@company 南京艾科朗克信息科技有限公司
///@file CXeleFtdcUserApiStruct.h
///@brief 业务数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef _XELE_TD_FTDCSTRUCT_H

#define _XELE_TD_FTDCSTRUCT_H

#include "CXeleFtdcUserApiDataType.h"

#pragma pack(push, 1)

///信息分发


struct CXeleFtdcDisseminationField {
#ifdef __cplusplus
  CXeleFtdcDisseminationField();
#endif

  ///序列系列号
  TXeleFtdcSequenceSeriesType SequenceSeries;
  ///序列号
  TXeleFtdcSequenceNoType SequenceNo;
};

///响应信息
struct CXeleFtdcRspInfoField {
#ifdef __cplusplus
  CXeleFtdcRspInfoField();
#endif

  ///错误代码
  TXeleFtdcErrorIDType ErrorID;
  ///错误信息
  TXeleFtdcErrorMsgType ErrorMsg;
};

///用户登录请求
struct CXeleFtdcReqUserLoginField {
#ifdef __cplusplus
  CXeleFtdcReqUserLoginField();
#endif
  char unused1_[7];
  ///Session Number
  short Session;
  ///交易用户代码
  TXeleFtdcAccountIDType AccountID;
  char unused2_[14];
  ///密码
  TXeleFtdcPasswordType Password;
  ///用户端产品信息 (程序自动填写)
  TXeleFtdcProductInfoType UserProductInfo;
  ///接口端产品信息 (程序自动填写)
  TXeleFtdcProductInfoType InterfaceProductInfo;
  char unused3_[45];
};

///用户登录应答
struct CXeleFtdcRspUserLoginField {
#ifdef __cplusplus
  CXeleFtdcRspUserLoginField();
#endif

  ///交易日
  TXeleFtdcDateType TradingDay;
  ///登录成功时间
  TXeleFtdcTimeType LoginTime;
  ///最大本地报单号
  TXeleFtdcOrderLocalIDType MaxOrderLocalID;
  ///交易用户代码
  TXeleFtdcAccountIDType AccountID;
  ///当前登录的交易所数目;最大支持同时登录XELE_EXCHANGE_LOGIN_NUM个
  TXeleFtdcExchangeNumType ExchangeNum;  
  ///当前登录交易所的交易用户index
  TXeleFtdcClientIndexType ClientIndex[XELE_EXCHANGE_LOGIN_NUM];
  ///当前登录交易所的交易用户的token
  TXeleFtdcClientTokenType Token[XELE_EXCHANGE_LOGIN_NUM]; 
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///交易系统名称
  TXeleFtdcTradingSystemNameType TradingSystemName;
  ///数据中心代码:0表示系统的查询中心，1 表示交易中心
  TXeleFtdcDataCenterIDType DataCenterID;
  ///会员私有流当前长度
  TXeleFtdcSequenceNoType PrivateFlowSize;
  ///交易员私有流当前长度
  TXeleFtdcSequenceNoType UserFlowSize;
};

///用户登出请求
struct CXeleFtdcReqUserLogoutField {
#ifdef __cplusplus
  CXeleFtdcReqUserLogoutField();
#endif

  ///交易用户代码
  TXeleFtdcAccountIDType AccountID;
  char Padding_Three[3];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
};

///用户登出应答
struct CXeleFtdcRspUserLogoutField {
#ifdef __cplusplus
  CXeleFtdcRspUserLogoutField();
#endif

  ///交易用户代码
  TXeleFtdcAccountIDType AccountID;
  char Padding_Three[3];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
};

///输入报单
struct CXeleFairInputOrderField {
#ifdef __cplusplus
  CXeleFairInputOrderField();
#endif
  ///产品保留字段1，由程序自动填写
  TXeleFtdcProductReserve1Type  ProductReserve1;
  ///客户编号
  TXeleFtdcClientIndexType      ClientIndex;
  ///客户令牌
  TXeleFtdcClientTokenType      ClientToken;
  ///产品保留字段2，由程序自动填写
  TXeleFtdcProductReserve2Type  ProductReserve2;
  ///产品保留字段3，由程序自动填写
  TXeleFtdcProductReserve3Type  ProductReserve3;  
  ///本地报单编号
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///报单价格
  TXeleFtdcPriceType    LimitPrice;  
  ///合约代码
  TXeleFtdcInstruIDType InstrumentID;
  ///数量
  TXeleFtdcVolumeTotalOriginalType VolumeTotalOriginal;
  ///输入报单类型
  TXeleFtdcInsertType   InsertType;
  ///最小成交数量
  TXeleFtdcMinVolumeType MinVolume;
  ///前置信息
  TXeleFtdcExchangeFrontEnumType  ExchangeFront;
  char                  _unused_1[2];
};

///报单操作
struct CXeleFairOrderActionField {
#ifdef __cplusplus
  CXeleFairOrderActionField();
#endif
  ///产品保留字段1，由程序自动填写
  TXeleFtdcProductReserve1Type  ProductReserve1;
  ///客户编号
  TXeleFtdcClientIndexType      ClientIndex;
  ///客户令牌
  TXeleFtdcClientTokenType      ClientToken;
  ///产品保留字段2，由程序自动填写
  TXeleFtdcProductReserve2Type  ProductReserve2;
  ///产品保留字段3，由程序自动填写
  TXeleFtdcProductReserve3Type  ProductReserve3; 
  ///本地报单编号
  TXeleFtdcActionLocalNoType ActionLocalNo;
  ///被撤单柜台编码
  TXeleFtdcOrderSysNoType    OrderSysNo; 
  char _unused_1[28];
};


///输入报单
struct CXeleFtdcInputOrderField {
#ifdef __cplusplus
  CXeleFtdcInputOrderField();
#endif

  ///报单编号
  TXeleFtdcOrderSystemNoType OrderSystemNo;
  char _unused_1[9];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///客户代码
  TXeleFtdcClientIDType ClientID;
  ///交易用户代码
  TXeleFtdcUserIDType UserID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///报单价格条件
  TXeleFtdcOrderPriceTypeType OrderPriceType;
  ///买卖方向
  TXeleFtdcDirectionType Direction;
  ///组合开平标志
  TXeleFtdcCombOffsetFlagType CombOffsetFlag;
  ///组合投机套保标志
  TXeleFtdcCombHedgeFlagType CombHedgeFlag;
  ///价格
  TXeleFtdcPriceType LimitPrice;
  ///数量
  TXeleFtdcVolumeType VolumeTotalOriginal;
  ///有效期类型
  TXeleFtdcTimeConditionType TimeCondition;
  ///GTD日期
  TXeleFtdcDateType GTDDate;
  ///成交量类型
  TXeleFtdcVolumeConditionType VolumeCondition;
  ///最小成交量
  TXeleFtdcVolumeType MinVolume;
  ///触发条件
  TXeleFtdcContingentConditionType ContingentCondition;
  ///止损价
  TXeleFtdcPriceType StopPrice;
  ///强平原因
  TXeleFtdcForceCloseReasonType ForceCloseReason;
  ///本地报单编号
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///未用字段
  char _unused_2[9];
  ///自动挂起标志
  TXeleFtdcBoolType IsAutoSuspend;
  ///交易所报单编号, RspOrderInsert时有意义
  TXeleFtdcExchangeOrderSysIDType ExchangeOrderSysID;
  ///用户定义交易所前置发单, 可选.
  TXeleFtdcExchangeFrontType ExchangeFront;
  ///未用字段
  char _unused_3[9];
};

///报单操作
struct CXeleFtdcOrderActionField {
#ifdef __cplusplus
  CXeleFtdcOrderActionField();
#endif

  ///报单编号
  TXeleFtdcOrderSystemNoType OrderSystemNo;
  ///未用字段
  char _unused_1[9];
  ///本地报单编号
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///未用字段
  char _unused_2[9];
  ///报单操作标志
  TXeleFtdcActionFlagType ActionFlag;
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///客户代码
  TXeleFtdcClientIDType ClientID;
  ///交易用户代码
  TXeleFtdcUserIDType UserID;
  ///价格
  TXeleFtdcPriceType LimitPrice;
  ///数量变化
  TXeleFtdcVolumeType VolumeChange;
  ///操作本地编号
  TXeleFtdcOrderLocalIDType ActionLocalID;
  ///未用字段
  char _unused_3[9];
  ///操作本地num
  TXeleFtdcActionLocalNoType ActionLocalNo;
  ///未用字段
  char _unused_4[12];
};

///用户口令修改
struct CXeleFtdcUserPasswordUpdateField {
#ifdef __cplusplus
  CXeleFtdcUserPasswordUpdateField();
#endif

  ///交易用户代码
  TXeleFtdcAccountIDType AccountID;
  char Padding_Three[3];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///旧密码
  TXeleFtdcPasswordType OldPassword;
  ///新密码
  TXeleFtdcPasswordType NewPassword;
};

///报单查询
struct CXeleFtdcQryOrderField {
#ifdef __cplusplus
  CXeleFtdcQryOrderField();
#endif
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///报单编号
  TXeleFtdcOrderSysIDType OrderSysID;
  ///开始时间
  TXeleFtdcTimeType TimeStart;
  ///结束时间
  TXeleFtdcTimeType TimeEnd;
  ///交易所描述符
  TXeleFtdcExchangeDescriptorType ExchangeDescriptor;
};

///成交查询
struct CXeleFtdcQryTradeField {
#ifdef __cplusplus
  CXeleFtdcQryTradeField();
#endif
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///成交编号
  TXeleFtdcTradeIDType TradeID;
  ///开始时间
  TXeleFtdcTimeType TimeStart;
  ///结束时间
  TXeleFtdcTimeType TimeEnd;
  ///交易所描述符
  TXeleFtdcExchangeDescriptorType ExchangeDescriptor;
};

///行情查询
struct CXeleFtdcQryMarketDataField {
#ifdef __cplusplus
  CXeleFtdcQryMarketDataField();
#endif

  ///产品代码
  TXeleFtdcProductIDType ProductID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
};

///客户持仓查询
struct CXeleFtdcQryClientPositionField {
#ifdef __cplusplus
  CXeleFtdcQryClientPositionField();
#endif
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///交易所描述符
  TXeleFtdcExchangeDescriptorType ExchangeDescriptor;
};

///合约查询
struct CXeleFtdcQryInstrumentField {
#ifdef __cplusplus
  CXeleFtdcQryInstrumentField();
#endif

  ///产品代码
  TXeleFtdcProductIDType ProductID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
};

///客户持仓
struct CXeleFtdcRspClientPositionField {
#ifdef __cplusplus
  CXeleFtdcRspClientPositionField();
#endif

  ///交易日
  TXeleFtdcDateType TradingDay;
  ///结算组代码(未使用)
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///结算编号(未使用)
  TXeleFtdcSettlementIDType SettlementID;
  ///投机套保标志
  TXeleFtdcHedgeFlagType HedgeFlag;
  ///多头上日持仓
  TXeleFtdcVolumeType LongYdPosition;
  ///多头今日持仓
  TXeleFtdcVolumeType LongPosition;
  ///空头上日持仓
  TXeleFtdcVolumeType ShortYdPosition;
  ///空头今日持仓
  TXeleFtdcVolumeType ShortPosition;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
};

///合约

struct CXeleFtdcRspInstrumentField {
#ifdef __cplusplus
  CXeleFtdcRspInstrumentField();
#endif

  ///结算组代码(未使用)
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///产品代码
  TXeleFtdcProductIDType ProductID;
  ///产品组代码(未使用)
  TXeleFtdcProductGroupIDType ProductGroupID;
  ///基础商品代码
  TXeleFtdcInstrumentIDType UnderlyingInstrID;
  ///产品类型
  TXeleFtdcProductClassType ProductClass;
  ///持仓类型
  TXeleFtdcPositionTypeType PositionType;
  ///执行价(未使用)
  TXeleFtdcPriceType StrikePrice;
  ///期权类型(未使用)
  TXeleFtdcOptionsTypeType OptionsType;
  ///合约数量乘数
  TXeleFtdcVolumeMultipleType VolumeMultiple;
  ///合约基础商品乘数
  TXeleFtdcUnderlyingMultipleType UnderlyingMultiple;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///合约名称(未使用)
  TXeleFtdcInstrumentNameType InstrumentName;
  ///交割年份
  TXeleFtdcYearType DeliveryYear;
  ///交割月
  TXeleFtdcMonthType DeliveryMonth;
  ///提前月份(未使用)
  TXeleFtdcAdvanceMonthType AdvanceMonth;
  ///当前是否交易
  TXeleFtdcBoolType IsTrading;
  ///创建日
  TXeleFtdcDateType CreateDate;
  ///上市日
  TXeleFtdcDateType OpenDate;
  ///到期日
  TXeleFtdcDateType ExpireDate;
  ///开始交割日
  TXeleFtdcDateType StartDelivDate;
  ///最后交割日
  TXeleFtdcDateType EndDelivDate;
  ///挂牌基准价(未使用)
  TXeleFtdcPriceType BasisPrice;
  ///市价单最大下单量
  TXeleFtdcVolumeType MaxMarketOrderVolume;
  ///市价单最小下单量
  TXeleFtdcVolumeType MinMarketOrderVolume;
  ///限价单最大下单量
  TXeleFtdcVolumeType MaxLimitOrderVolume;
  ///限价单最小下单量
  TXeleFtdcVolumeType MinLimitOrderVolume;
  ///最小变动价位
  TXeleFtdcPriceType PriceTick;
  ///交割月自然人开仓(未使用)
  TXeleFtdcMonthCountType AllowDelivPersonOpen;
};

///Ffex合约
struct CXeleFtdcInstrumentField {
#ifdef __cplusplus
  CXeleFtdcInstrumentField();
#endif

  ///结算组代码
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///产品代码
  TXeleFtdcProductIDType ProductID;
  ///产品组代码
  TXeleFtdcProductGroupIDType ProductGroupID;
  ///基础商品代码
  TXeleFtdcInstrumentIDType UnderlyingInstrID;
  ///产品类型
  TXeleFtdcProductClassType ProductClass;
  ///持仓类型
  TXeleFtdcPositionTypeType PositionType;
  ///执行价
  TXeleFtdcPriceType StrikePrice;
  ///期权类型
  TXeleFtdcOptionsTypeType OptionsType;
  ///合约数量乘数
  TXeleFtdcVolumeMultipleType VolumeMultiple;
  ///合约基础商品乘数
  TXeleFtdcUnderlyingMultipleType UnderlyingMultiple;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///合约名称
  TXeleFtdcInstrumentNameType InstrumentName;
  ///交割年份
  TXeleFtdcYearType DeliveryYear;
  ///交割月
  TXeleFtdcMonthType DeliveryMonth;
  ///提前月份
  TXeleFtdcAdvanceMonthType AdvanceMonth;
  ///当前是否交易
  TXeleFtdcBoolType IsTrading;
};

///成交
struct CXeleFtdcTradeField {
#ifdef __cplusplus
  CXeleFtdcTradeField();
#endif

  ///交易日
  TXeleFtdcDateType TradingDay;
  ///结算组代码
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///结算编号
  TXeleFtdcSettlementIDType SettlementID;
  ///成交编号
  TXeleFtdcTradeIDType TradeID;
  ///买卖方向
  TXeleFtdcDirectionType Direction;
  ///报单编号
  TXeleFtdcOrderSystemNoType OrderSystemNo;
  ///未用字段
  char _unused_1[9];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///客户代码
  TXeleFtdcClientIDType ClientID;
  ///交易角色(未使用)
  TXeleFtdcTradingRoleType TradingRole;
  ///资金帐号(未使用)
  TXeleFtdcAccountIDType AccountID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///开平标志
  TXeleFtdcOffsetFlagType OffsetFlag;
  ///投机套保标志
  TXeleFtdcHedgeFlagType HedgeFlag;
  ///价格
  TXeleFtdcPriceType Price;
  ///数量
  TXeleFtdcVolumeType Volume;
  ///成交时间
  TXeleFtdcTimeType TradeTime;
  ///成交类型(未使用)
  TXeleFtdcTradeTypeType TradeType;
  ///成交价来源(未使用)
  TXeleFtdcPriceSourceType PriceSource;
  ///交易用户代码
  TXeleFtdcUserIDType UserID;
  ///本地报单编号
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///未用字段
  char _unused_2[9];
  ///交易所报单编号
  TXeleFtdcExchangeOrderSysIDType ExchangeOrderSysID;
  char _unused_3[32];
};

///报单
struct CXeleFtdcOrderField {
#ifdef __cplusplus
  CXeleFtdcOrderField();
#endif

  ///交易日
  TXeleFtdcDateType TradingDay;
  ///结算组代码(未使用)
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///结算编号(未使用)
  TXeleFtdcSettlementIDType SettlementID;
  ///本地报单编号
  TXeleFtdcOrderSystemNoType OrderSystemNo;
  ///未用字段
  char _unused_1[9];
  ///会员代码
  TXeleFtdcParticipantIDType ParticipantID;
  ///客户代码
  TXeleFtdcClientIDType ClientID;
  ///交易用户代码
  TXeleFtdcUserIDType UserID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///报单价格条件
  TXeleFtdcOrderPriceTypeType OrderPriceType;
  ///买卖方向
  TXeleFtdcDirectionType Direction;
  ///组合开平标志
  TXeleFtdcCombOffsetFlagType CombOffsetFlag;
  ///组合投机套保标志
  TXeleFtdcCombHedgeFlagType CombHedgeFlag;
  ///价格
  TXeleFtdcPriceType LimitPrice;
  ///数量
  TXeleFtdcVolumeType VolumeTotalOriginal;
  ///有效期类型
  TXeleFtdcTimeConditionType TimeCondition;
  ///GTD日期
  TXeleFtdcDateType GTDDate;
  ///成交量类型
  TXeleFtdcVolumeConditionType VolumeCondition;
  ///最小成交量
  TXeleFtdcVolumeType MinVolume;
  ///触发条件
  TXeleFtdcContingentConditionType ContingentCondition;
  ///止损价
  TXeleFtdcPriceType StopPrice;
  ///强平原因
  TXeleFtdcForceCloseReasonType ForceCloseReason;
  ///本地报单编号
  TXeleFtdcOrderLocalNoType OrderLocalNo;
  ///未用字段
  char _unused_2[9];
  ///自动挂起标志
  TXeleFtdcBoolType IsAutoSuspend;
  ///报单来源(未使用)
  TXeleFtdcOrderSourceType OrderSource;
  ///报单状态
  TXeleFtdcOrderStatusType OrderStatus;
  ///报单类型(未使用)
  TXeleFtdcOrderTypeType OrderType;
  ///今成交数量(未使用)
  TXeleFtdcVolumeType VolumeTraded;
  ///剩余数量
  TXeleFtdcVolumeType VolumeTotal;
  ///报单日期
  TXeleFtdcDateType InsertDate;
  ///插入时间
  TXeleFtdcTimeType InsertTime;
  ///激活时间(未使用)
  TXeleFtdcTimeType ActiveTime;
  ///挂起时间(未使用)
  TXeleFtdcTimeType SuspendTime;
  ///最后修改时间(未使用)
  TXeleFtdcTimeType UpdateTime;
  ///撤销时间(未使用)
  TXeleFtdcTimeType CancelTime;
  ///最后修改交易用户代码
  TXeleFtdcUserIDType ActiveUserID;
  ///优先权(未使用)
  TXeleFtdcPriorityType Priority;
  ///按时间排队的序号(未使用)
  TXeleFtdcTimeSortIDType TimeSortID;
  ///交易所报单编号
  TXeleFtdcExchangeOrderSysIDType ExchangeOrderSysID;
  ///未用字段
  char _unused_3[32];
};

///合约状态
struct CXeleFtdcInstrumentStatusField {
#ifdef __cplusplus
  CXeleFtdcInstrumentStatusField();
#endif

  ///结算组代码
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///合约交易状态
  TXeleFtdcInstrumentStatusType InstrumentStatus;
  ///交易阶段编号
  TXeleFtdcTradingSegmentSNType TradingSegmentSN;
  ///进入本状态时间
  TXeleFtdcTimeType EnterTime;
  ///进入本状态原因
  TXeleFtdcInstStatusEnterReasonType EnterReason;
};

///客户资金查询
struct CXeleFtdcQryClientAccountField {
#ifdef __cplusplus
  CXeleFtdcQryClientAccountField();
#endif
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
};

///客户资金
struct CXeleFtdcRspClientAccountField {
#ifdef __cplusplus
  CXeleFtdcRspClientAccountField();
#endif

  ///交易日
  TXeleFtdcDateType TradingDay;
  ///结算组代码(未使用)
  TXeleFtdcSettlementGroupIDType SettlementGroupID;
  ///结算编号(未使用)
  TXeleFtdcSettlementIDType SettlementID;
  ///上次结算准备金
  TXeleFtdcMoneyType PreBalance;
  ///当前保证金总额
  TXeleFtdcMoneyType CurrMargin;
  ///平仓盈亏
  TXeleFtdcMoneyType CloseProfit;
  ///期权权利金收支
  TXeleFtdcMoneyType Premium;
  ///入金金额
  TXeleFtdcMoneyType Deposit;
  ///出金金额
  TXeleFtdcMoneyType Withdraw;
  ///期货结算准备金
  TXeleFtdcMoneyType Balance;
  ///可提资金
  TXeleFtdcMoneyType Available;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///冻结的保证金
  TXeleFtdcMoneyType FrozenMargin;
  ///冻结的权利金
  TXeleFtdcMoneyType FrozenPremium;
  ///基本准备金(未使用)
  TXeleFtdcMoneyType BaseReserve;
  ///浮动盈亏
  TXeleFtdcMoneyType floatProfitAndLoss;
};

struct CXeleFtdcQryInstrumentMarginRateField {
#ifdef __cplusplus
  CXeleFtdcQryInstrumentMarginRateField();
#endif

  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///投机套保标志
  TXeleFtdcHedgeFlagType HedgeFlag;
};

struct CXeleFtdcRspInstrumentMarginRateField {
#ifdef __cplusplus
  CXeleFtdcRspInstrumentMarginRateField();
#endif

  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///投机套保标志
  TXeleFtdcHedgeFlagType HedgeFlag;
  ///多头保证金率
  TXeleFtdcRatioType LongMarginRatioByMoney;
  ///多头保证金费
  TXeleFtdcRatioType LongMarginRatioByVolume;
  ///空头保证金率
  TXeleFtdcRatioType ShortMarginRatioByMoney;
  ///空头保证金费
  TXeleFtdcRatioType ShortMarginRatioByVolume;
  ///是否相对交易所收取
  TXeleFtdcBoolType IsRelative;
};

struct CXeleFtdcQryInstrumentCommissionRateField {
#ifdef __cplusplus
  CXeleFtdcQryInstrumentCommissionRateField();
#endif

  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
};

struct CXeleFtdcRspInstrumentCommissionRateField {
#ifdef __cplusplus
  CXeleFtdcRspInstrumentCommissionRateField();
#endif

  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///资金帐号
  TXeleFtdcAccountIDType AccountID;
  ///开仓手续费率
  TXeleFtdcRatioType OpenRatioByMoney;
  ///开仓手续费
  TXeleFtdcRatioType OpenRatioByVolume;
  ///平仓手续费率
  TXeleFtdcRatioType CloseRatioByMoney;
  ///平仓手续费
  TXeleFtdcRatioType CloseRatioByVolume;
  ///平今手续费率
  TXeleFtdcRatioType CloseTodayRatioByMoney;
  ///平今手续费
  TXeleFtdcRatioType CloseTodayRatioByVolume;
};

///合约状态查询
struct CXeleFtdcQryInstrumentStatusField {
#ifdef __cplusplus
  CXeleFtdcQryInstrumentStatusField();
#endif

  ///起始合约代码
  TXeleFtdcInstrumentIDType InstIDStart;
  ///结束合约代码
  TXeleFtdcInstrumentIDType InstIDEnd;
};

///合约状态查询
struct CXeleFtdcRspInstrumentPriceField {
#ifdef __cplusplus
  CXeleFtdcRspInstrumentPriceField();
#endif

  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
  ///涨停价
  TXeleFtdcPriceType UpperLimitPrice;
  ///跌停价
  TXeleFtdcPriceType LowerLimitPrice;
};

///合约状态查询
struct CXeleFtdcReqInstrumentPriceField {
#ifdef __cplusplus
  CXeleFtdcReqInstrumentPriceField();
#endif

  ///产品代码
  TXeleFtdcProductIDType ProductID;
  ///合约代码
  TXeleFtdcInstrumentIDType InstrumentID;
};

///Internal version identify field
struct CXeleFtdcInternalVersionIdentifyField {
#ifdef __cplusplus
  CXeleFtdcInternalVersionIdentifyField();
#endif

  ///Version information
  TXeleFtdcUserIDType VersionInfo;
};

///Exchange Identify field
struct CXeleFtdcExchangeIdentifyField {
#ifdef __cplusplus
  CXeleFtdcExchangeIdentifyField();
#endif

  ///Version information
  TXeleFtdcUserIDType ExchangeInfo;
};

///交易所交易前置代码查询
struct CXeleFtdcRspExchangeFrontField {
#ifdef __cplusplus
  CXeleFtdcRspExchangeFrontField();
#endif
  ///交易所标志 0:NULL, 1:SHFE, 2:INE, 3:DCE, 4:CZCE, 5:CFFEX
  int ExchangeID;
  ///交易前置数量
  short FrontCount;
  ///交易前置代码列表
  TXeleFtdcFrontListType FrontList;
};

struct CXeleFtdcAuthenticationInfoField {
#ifdef __cplusplus
  CXeleFtdcAuthenticationInfoField();
#endif
  ///终端软件AppID
  TXeleFtdcAppIDType AppID;
  ///终端软件授权码
  TXeleFtdcAuthCodeType AuthCode;
};
#pragma pack(pop)

#endif
