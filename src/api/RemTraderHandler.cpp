
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include <string>
#include "EesTraderDefine.h"
#include "EesTraderApi.h"

#include "RemTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

#ifdef __cplusplus
}
#endif


CRemTraderHandler::CRemTraderHandler()
{

}

CRemTraderHandler::~CRemTraderHandler()
{

}

void CRemTraderHandler::OnConnection(ERR_NO errNo, const char* pErrStr )
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_connected(self);

}

void CRemTraderHandler::OnDisConnection(ERR_NO errNo, const char* pErrStr )
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_disconnected(self, errNo);

}

void CRemTraderHandler::OnUserLogon(EES_LogonResponse* pLogon)
{
  CMN_DEBUG("Enter\n");
  CMN_DEBUG("pLogon->m_Result=[%d]\n"
    "pLogon->m_UserId=[%d]\n"
    "pLogon->m_TradingDate=[%d]\n"
    "pLogon->m_MaxToken=[%d]\n"
    "pLogon->m_OrderFCCount=[%d]\n"
    "pLogon->m_OrderFCInterval=[%d]\n"
    "pLogon->m_CancelFCCount=[%d]\n"
    "pLogon->m_CancelFCCount=[%d]\n"
    ,pLogon->m_Result
    ,pLogon->m_UserId
    ,pLogon->m_TradingDate
    ,pLogon->m_MaxToken
    ,pLogon->m_OrderFCCount
    ,pLogon->m_OrderFCInterval
    ,pLogon->m_CancelFCCount
    ,pLogon->m_CancelFCInterval
  );
  
  trader_trader_api* self = (trader_trader_api*)m_Arg;

    
  int errNo = pLogon->m_Result;
  char* errMsg = NULL;
  if(EES_LOGON_OK != errNo){
    //失败
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  }else{
    // 查询QueryUserAccount
    RESULT ret = m_TraderApi->QueryUserAccount();
    if(ret){
      CMN_ERROR("QueryUserAccount[%d]\n", ret);
    }
  } 
  
  return ;
}

void CRemTraderHandler::OnQueryUserAccount(EES_AccountInfo * pAccoutnInfo, bool bFinish)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  if(!pAccoutnInfo){
    trader_trader_api_on_rsp_user_login(self, EES_LOGON_ACCOUNT_NOT_BOUND, NULL);
    return;
  }

  strncpy(m_Account, pAccoutnInfo->m_Account, sizeof(m_Account));
	
  CMN_DEBUG("bFinish=[%d]\n", bFinish);

  CMN_DEBUG(
    "pAccoutnInfo->m_Account=[%s]\n"
    "pAccoutnInfo->m_Previlege=[%d]\n"
    "pAccoutnInfo->m_InitialBp=[%lf]\n"
    "pAccoutnInfo->m_AvailableBp=[%lf]\n"
    "pAccoutnInfo->m_Margin=[%lf]\n"
    "pAccoutnInfo->m_FrozenMargin=[%lf]\n"
    "pAccoutnInfo->m_CommissionFee=[%lf]\n"
    "pAccoutnInfo->m_FrozenCommission=[%lf]\n"
    ,pAccoutnInfo->m_Account
    ,pAccoutnInfo->m_Previlege
    ,pAccoutnInfo->m_InitialBp
    ,pAccoutnInfo->m_AvailableBp
    ,pAccoutnInfo->m_Margin
    ,pAccoutnInfo->m_FrozenMargin
    ,pAccoutnInfo->m_CommissionFee
    ,pAccoutnInfo->m_FrozenCommission
  );

  if(bFinish){
    trader_trader_api_on_rsp_user_login(self, EES_LOGON_OK, NULL);
  }
}

void CRemTraderHandler::OnQueryAccountPosition(const char* pAccount, EES_AccountPosition* pAccoutnPosition, int nReqId, bool bFinish)
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG("pAccoutnPosition->pAccount=[%s]\n", pAccount);

  CMN_DEBUG(
    "pAccoutnPosition->m_actId=[%s]\n"
    "pAccoutnPosition->m_Symbol=[%s]\n"
    "pAccoutnPosition->m_PosiDirection=[%d]\n"
    "pAccoutnPosition->m_InitOvnQty=[%d]\n"
    "pAccoutnPosition->m_OvnQty=[%d]\n"
    "pAccoutnPosition->m_FrozenOvnQty=[%d]\n"
    "pAccoutnPosition->m_TodayQty=[%d]\n"
    "pAccoutnPosition->m_FrozenTodayQty=[%d]\n"
    "pAccoutnPosition->m_OvnMargin=[%lf]\n"
    "pAccoutnPosition->m_TodayMargin=[%lf]\n"
    "pAccoutnPosition->m_PositionCost=[%lf]\n"
    "pAccoutnPosition->m_HedgeFlag=[%d]\n"
    ,pAccoutnPosition->m_actId
    ,pAccoutnPosition->m_Symbol
    ,pAccoutnPosition->m_PosiDirection
    ,pAccoutnPosition->m_InitOvnQty
    ,pAccoutnPosition->m_OvnQty
    ,pAccoutnPosition->m_FrozenOvnQty
    ,pAccoutnPosition->m_TodayQty
    ,pAccoutnPosition->m_FrozenTodayQty
    ,pAccoutnPosition->m_OvnMargin
    ,pAccoutnPosition->m_TodayMargin
    ,pAccoutnPosition->m_PositionCost
    ,pAccoutnPosition->m_HedgeFlag
  );

  
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  strcpy(traderPosition.InstrumentID, pAccoutnPosition->m_Symbol);
  traderPosition.PositionDate = '1'; //
  traderPosition.PosiDirection = TRADER_POSITION_LONG;
  if(EES_PosiDirection_short == pAccoutnPosition->m_PosiDirection){
    traderPosition.PosiDirection = TRADER_POSITION_SHORT;
  }
  traderPosition.YdPosition = pAccoutnPosition->m_InitOvnQty;
  traderPosition.TodayPosition = pAccoutnPosition->m_TodayQty;
  traderPosition.Position = traderPosition.YdPosition + traderPosition.TodayPosition;
  traderPosition.LongFrozen = pAccoutnPosition->m_FrozenOvnQty + pAccoutnPosition->m_FrozenTodayQty;

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, bFinish);
  return 0;
}

void CRemTraderHandler::OnQueryAccountBP(const char* pAccount, EES_AccountBP* pAccoutnPosition, int nReqId )
{
  CMN_DEBUG("Enter\n");
  CMN_DEBUG(
    "pAccoutnPosition->pAccount=[%s]\n"
    , pAccount);
  
  CMN_DEBUG(
    "pAccoutnPosition->m_account=[%s]\n"
    "pAccoutnPosition->m_InitialBp=[%lf]\n"
    "pAccoutnPosition->m_AvailableBp=[%lf]\n"
    "pAccoutnPosition->m_Margin=[%lf]\n"
    "pAccoutnPosition->m_FrozenMargin=[%lf]\n"
    "pAccoutnPosition->m_CommissionFee=[%lf]\n"
    "pAccoutnPosition->m_FrozenCommission=[%lf]\n"
    "pAccoutnPosition->m_OvnInitMargin=[%lf]\n"
    "pAccoutnPosition->m_TotalLiquidPL=[%lf]\n"
    "pAccoutnPosition->m_TotalMarketPL=[%lf]\n"
    ,pAccoutnPosition->m_account
    ,pAccoutnPosition->m_InitialBp
    ,pAccoutnPosition->m_AvailableBp
    ,pAccoutnPosition->m_Margin
    ,pAccoutnPosition->m_FrozenMargin
    ,pAccoutnPosition->m_CommissionFee
    ,pAccoutnPosition->m_FrozenCommission
    ,pAccoutnPosition->m_OvnInitMargin
    ,pAccoutnPosition->m_TotalLiquidPL
    ,pAccoutnPosition->m_TotalMarketPL
  );

  
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, 0, NULL, 1);

}

void CRemTraderHandler::OnQuerySymbol(EES_SymbolField* pSymbol, bool bFinish)
{
  CMN_DEBUG("Enter\n");
  
  CMN_DEBUG(
    "pSymbol->m_SecType=[%c]\n"
    "pSymbol->m_symbol=[%s]\n"
    "pSymbol->m_symbolName=[%s]\n"
    "pSymbol->m_ExchangeID=[%d]\n"
    "pSymbol->m_ProdID=[%s]\n"
    "pSymbol->m_DeliveryYear=[%d]\n"
    "pSymbol->m_DeliveryMonth=[%d]\n"
    "pSymbol->m_MaxMarketOrderVolume=[%d]\n"
    "pSymbol->m_MinMarketOrderVolume=[%d]\n"
    "pSymbol->m_MaxLimitOrderVolume=[%d]\n"
    "pSymbol->m_MinLimitOrderVolume=[%d]\n"
    "pSymbol->m_VolumeMultiple=[%d]\n"
    "pSymbol->m_PriceTick=[%lf]\n"
    "pSymbol->m_CreateDate=[%d]\n"
    "pSymbol->m_OpenDate=[%d]\n"
    "pSymbol->m_ExpireDate=[%d]\n"
    "pSymbol->m_StartDelivDate=[%d]\n"
    "pSymbol->m_EndDelivDate=[%d]\n"
    "pSymbol->m_InstLifePhase=[%d]\n"
    "pSymbol->m_IsTrading=[%d]\n"
    "pSymbol->m_StrikePrice=[%lf]\n"
    "pSymbol->m_CallPut=[%d]\n"
    "pSymbol->m_UnderlyingSymbol=[%s]\n"
    , pSymbol->m_SecType
    , pSymbol->m_symbol
    , pSymbol->m_symbolName
    , pSymbol->m_ExchangeID
    , pSymbol->m_ProdID
    , pSymbol->m_DeliveryYear
    , pSymbol->m_DeliveryMonth
    , pSymbol->m_MaxMarketOrderVolume
    , pSymbol->m_MinMarketOrderVolume
    , pSymbol->m_MaxLimitOrderVolume
    , pSymbol->m_MinLimitOrderVolume
    , pSymbol->m_VolumeMultiple
    , pSymbol->m_PriceTick
    , pSymbol->m_CreateDate
    , pSymbol->m_OpenDate
    , pSymbol->m_ExpireDate
    , pSymbol->m_StartDelivDate
    , pSymbol->m_EndDelivDate
    , pSymbol->m_InstLifePhase
    , pSymbol->m_IsTrading
    , pSymbol->m_StrikePrice
    , pSymbol->m_CallPut
    , pSymbol->m_UnderlyingSymbol
  );
  
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  strcpy(traderInstrument.InstrumentID, pSymbol->m_symbol);
  //TODO
  strcpy(traderInstrument.ExchangeID, pSymbol->m_ExchangeID);
  traderInstrument.VolumeMultiple = pSymbol->m_VolumeMultiple;
  traderInstrument.PriceTick = pSymbol->m_PriceTick;
  
  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, bFinish);
}

void CRemTraderHandler::OnOrderAccept(EES_OrderAcceptField* pAccept )
{
  CMN_DEBUG("Enter\n");
  
  CMN_DEBUG(
    "pAccept->m_ClientOrderToken=[%d]\n"
    "pAccept->m_MarketOrderToken=[%lld]\n"
    "pAccept->m_OrderState=[%d]\n"
    "pAccept->m_UserID=[%d]\n"
    "pAccept->m_AcceptTime=[%lld]\n"
    "pAccept->m_Account=[%s]\n"
    "pAccept->m_Side=[%d]\n"
    "pAccept->m_Exchange=[%d]\n"
    "pAccept->m_Symbol=[%s]\n"
    "pAccept->m_SecType=[%d]\n"
    "pAccept->m_Price=[%lf]\n"
    "pAccept->m_Qty=[%d]\n"
    "pAccept->m_OptExecFlag=[%d]\n"
    "pAccept->m_Tif=[%d]\n"
    "pAccept->m_MinQty=[%d]\n"
    "pAccept->m_CustomField=[%lld]\n"
    "pAccept->m_MarketSessionId=[%d]\n"
    "pAccept->m_HedgeFlag=[%d]\n"
    , pAccept->m_ClientOrderToken
    , pAccept->m_MarketOrderToken
    , pAccept->m_OrderState
    , pAccept->m_UserID
    , pAccept->m_AcceptTime
    , pAccept->m_Account
    , pAccept->m_Side
    , pAccept->m_Exchange
    , pAccept->m_Symbol
    , pAccept->m_SecType
    , pAccept->m_Price
    , pAccept->m_Qty
    , pAccept->m_OptExecFlag
    , pAccept->m_Tif
    , pAccept->m_MinQty
    , pAccept->m_CustomField
    , pAccept->m_MarketSessionId
    , pAccept->m_HedgeFlag
  );

  
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  //TODO
	///交易所代码
  strcpy(traderOrder.ExchangeID, pAccept->ExchangeID);
	///系统报单编号
  strcpy(traderOrder.OrderSysID, pAccept->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pAccept->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pAccept->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pAccept->Direction;
  // 开平
  traderOrder.OffsetFlag = pAccept->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pAccept->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pAccept->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pAccept->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pAccept->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pAccept->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pAccept->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
}

void CRemTraderHandler::OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept)
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pAccept->m_Account=[%s]\n"
    "pAccept->m_MarketOrderToken=[%lld]\n"
    "pAccept->m_MarketOrderId=[%s]\n"
    "pAccept->m_MarketTime=[%lld]\n"
    "pAccept->m_UserID=[%d]\n"
    "pAccept->m_ClientOrderToken=[%d]\n"
    "pAccept->m_ExchangeStatus=[%d]\n"
    , pAccept->m_Account
    , pAccept->m_MarketOrderToken
    , pAccept->m_MarketOrderId
    , pAccept->m_MarketTime
    , pAccept->m_UserID
    , pAccept->m_ClientOrderToken
    , pAccept->m_ExchangeStatus
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  //TODO
	///交易所代码
  strcpy(traderOrder.ExchangeID, pAccept->ExchangeID);
	///系统报单编号
  strcpy(traderOrder.OrderSysID, pAccept->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pAccept->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pAccept->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pAccept->Direction;
  // 开平
  traderOrder.OffsetFlag = pAccept->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pAccept->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pAccept->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pAccept->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pAccept->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pAccept->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pAccept->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void CRemTraderHandler::OnOrderReject(EES_OrderRejectField* pReject )
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pReject->m_Userid=[%d]\n"
    "pReject->m_Timestamp=[%lld]\n"
    "pReject->m_ClientOrderToken=[%d]\n"
    "pReject->m_RejectedMan=[%d]\n"
    "pReject->m_ReasonCode=[%d]\n"
    "pReject->m_GrammerResult=[%s]\n"
    "pReject->m_RiskResult=[%s]\n"
    "pReject->m_GrammerText=[%s]\n"
    "pReject->m_RiskText=[%s]\n"
    , pReject->m_ClientOrderToken
    , pReject->m_Timestamp
    , pReject->m_ClientOrderToken
    , pReject->m_RejectedMan
    , pReject->m_ReasonCode
    , pReject->m_GrammerResult
    , pReject->m_RiskResult
    , pReject->m_GrammerText
    , pReject->m_RiskText
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  //TODO
	///交易所代码
  strcpy(traderOrder.ExchangeID, pAccept->ExchangeID);
	///系统报单编号
  strcpy(traderOrder.OrderSysID, pAccept->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pAccept->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pAccept->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pAccept->Direction;
  // 开平
  traderOrder.OffsetFlag = pAccept->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pAccept->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pAccept->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pAccept->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pAccept->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pAccept->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pAccept->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

  
  trader_trader_api_on_err_rtn_order_insert(self, errNo, errMsg);

}

void CRemTraderHandler::OnOrderMarketReject(EES_OrderMarketRejectField* pReject)
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pReject->m_Account=[%s]\n"
    "pReject->m_MarketOrderToken=[%lld]\n"
    "pReject->m_MarketTimestamp=[%lld]\n"
    "pReject->m_ReasonText=[%s]\n"
    "pReject->m_ExchangeId=[%d]\n"
    "pReject->m_ExchangeErrorId=[%d]\n"
    "pReject->m_UserID=[%d]\n"
    "pReject->m_ClientOrderToken=[%d]\n"
    , pReject->m_Account
    , pReject->m_MarketOrderToken
    , pReject->m_MarketTimestamp
    , pReject->m_ReasonText
    , pReject->m_ExchangeId
    , pReject->m_ExchangeErrorId
    , pReject->m_UserID
    , pReject->m_ClientOrderToken
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  //TODO
	///交易所代码
  strcpy(traderOrder.ExchangeID, pAccept->ExchangeID);
	///系统报单编号
  strcpy(traderOrder.OrderSysID, pAccept->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pAccept->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pAccept->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pAccept->Direction;
  // 开平
  traderOrder.OffsetFlag = pAccept->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pAccept->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pAccept->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pAccept->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pAccept->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pAccept->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pAccept->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void CRemTraderHandler::OnOrderExecution(EES_OrderExecutionField* pExec )
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pExec->m_Userid=[%d]\n"
    "pExec->m_Timestamp=[%lld]\n"
    "pExec->m_ClientOrderToken=[%d]\n"
    "pExec->m_MarketOrderToken=[%lld]\n"
    "pExec->m_Quantity=[%d]\n"
    "pExec->m_Price=[%lf]\n"
    "pExec->m_ExecutionID=[%lld]\n"
    "pExec->m_MarketExecID=[%s]\n"
    , pExec->m_Userid
    , pExec->m_Timestamp
    , pExec->m_ClientOrderToken
    , pExec->m_MarketOrderToken
    , pExec->m_Quantity
    , pExec->m_Price
    , pExec->m_ExecutionID
    , pExec->m_MarketExecID
  );
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strcpy(traderTrade.InstrumentID, pTrade->InstrumentID);
  ///本地报单编号
  strcpy(traderTrade.UserOrderLocalID, pTrade->UserOrderLocalID);
  ///交易日
  strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///成交时间
  strcpy(traderTrade.TradeTime, pTrade->TradeTime);
  ///买卖方向
  traderTrade.Direction = pTrade->Direction;
  ///开平标志
  traderTrade.OffsetFlag = pTrade->OffsetFlag;
  ///成交价格
  traderTrade.TradePrice = pTrade->TradePrice;
  ///成交数量
  traderTrade.TradeVolume = pTrade->TradeVolume;
  //成交编号
  strcpy(traderTrade.TradeID, pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);

}

void CRemTraderHandler::OnOrderCxled(EES_OrderCxled* pCxled )
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pCxled->m_Userid=[%d]\n"
    "pCxled->m_Timestamp=[%lld]\n"
    "pCxled->m_ClientOrderToken=[%d]\n"
    "pCxled->m_MarketOrderToken=[%lld]\n"
    "pCxled->m_Decrement=[%d]\n"
    "pCxled->m_Reason=[%d]\n"
    , pCxled->m_Userid
    , pCxled->m_Timestamp
    , pCxled->m_ClientOrderToken
    , pCxled->m_MarketOrderToken
    , pCxled->m_Decrement
    , pCxled->m_Reason
  );
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  //TODO
	///交易所代码
  strcpy(traderOrder.ExchangeID, pAccept->ExchangeID);
	///系统报单编号
  strcpy(traderOrder.OrderSysID, pAccept->OrderSysID);
  // 合约代码
  strcpy(traderOrder.InstrumentID, pAccept->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pAccept->UserOrderLocalID);
  // 买卖
  traderOrder.Direction = pAccept->Direction;
  // 开平
  traderOrder.OffsetFlag = pAccept->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = pAccept->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pAccept->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pAccept->Volume;
  // 成交手数
  traderOrder.VolumeTraded = pAccept->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pAccept->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pAccept->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void CRemTraderHandler::OnCxlOrderReject(EES_CxlOrderRej* pReject )
{
  CMN_DEBUG("Enter\n");
  
  
  CMN_DEBUG(
    "pReject->m_account=[%s]\n"
    "pReject->m_MarketOrderToken=[%lld]\n"
    "pReject->m_ReasonCode=[%d]\n"
    "pReject->m_ReasonText=[%s]\n"
    "pReject->m_UserID=[%d]\n"
    "pReject->m_ClientOrderToken=[%d]\n"
    "pReject->m_ExchangeID=[%d]\n"
    "pReject->m_ActionFlag=[%d]\n"
    , pReject->m_account
    , pReject->m_MarketOrderToken
    , pReject->m_ReasonCode
    , pReject->m_ReasonText
    , pReject->m_UserID
    , pReject->m_ClientOrderToken
    , pReject->m_ExchangeID
    , pReject->m_ActionFlag
  );

  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_err_rtn_order_action(self, errNo, errMsg);
}

void CRemTraderHandler::OnQueryTradeOrder(const char* pAccount, EES_QueryAccountOrder* pQueryOrder, bool bFinish  )
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pQueryOrder->m_Userid=[%d]\n"
    "pQueryOrder->m_Timestamp=[%lld]\n"
    "pQueryOrder->m_ClientOrderToken=[%d]\n"
    "pQueryOrder->m_SideType=[%d]\n"
    "pQueryOrder->m_Quantity=[%d]\n"
    "pQueryOrder->m_InstrumentType=[%d]\n"
    "pQueryOrder->m_symbol=[%s]\n"
    "pQueryOrder->m_Price=[%lf]\n"
    "pQueryOrder->m_account=[%s]\n"
    "pQueryOrder->m_ExchengeID=[%d]\n"
    "pQueryOrder->m_OptExecFlag=[%d]\n"
    "pQueryOrder->m_MarketOrderToken=[%lld]\n"
    "pQueryOrder->m_OrderStatus=[%d]\n"
    "pQueryOrder->m_CloseTime=[%lld]\n"
    "pQueryOrder->m_FilledQty=[%d]\n"
    "pQueryOrder->m_Tif=[%d]\n"
    "pQueryOrder->m_MinQty=[%d]\n"
    "pQueryOrder->m_CustomField=[%lld]\n"
    "pQueryOrder->m_MarketOrderId=[%s]\n"
    "pQueryOrder->m_HedgeFlag=[%d]\n"
    , pQueryOrder->m_Userid
    , pQueryOrder->m_Timestamp
    , pQueryOrder->m_ClientOrderToken
    , pQueryOrder->m_SideType
    , pQueryOrder->m_Quantity
    , pQueryOrder->m_InstrumentType
    , pQueryOrder->m_symbol
    , pQueryOrder->m_Price
    , pQueryOrder->m_account
    , pQueryOrder->m_ExchengeID
    , pQueryOrder->m_OptExecFlag
    , pQueryOrder->m_MarketOrderToken
    , pQueryOrder->m_OrderStatus
    , pQueryOrder->m_CloseTime
    , pQueryOrder->m_FilledQty
    , pQueryOrder->m_Tif
    , pQueryOrder->m_MinQty
    , pQueryOrder->m_CustomField
    , pQueryOrder->m_MarketOrderId
    , pQueryOrder->m_HedgeFlag
  );

}

void CRemTraderHandler::OnQueryTradeOrderExec(const char* pAccount, EES_QueryOrderExecution* pQueryOrderExec, bool bFinish  )
{
  CMN_DEBUG("Enter\n");
  
  CMN_DEBUG(
    "pQueryOrderExec->m_Userid=[%d]\n"
    "pQueryOrderExec->m_Timestamp=[%lld]\n"
    "pQueryOrderExec->m_ClientOrderToken=[%d]\n"
    "pQueryOrderExec->m_MarketOrderToken=[%lld]\n"
    "pQueryOrderExec->m_ExecutedQuantity=[%d]\n"
    "pQueryOrderExec->m_ExecutionPrice=[%lf]\n"
    "pQueryOrderExec->m_ExecutionID=[%lld]\n"
    "pQueryOrderExec->m_MarketExecID=[%s]\n"
    , pQueryOrderExec->m_Userid
    , pQueryOrderExec->m_Timestamp
    , pQueryOrderExec->m_ClientOrderToken
    , pQueryOrderExec->m_MarketOrderToken
    , pQueryOrderExec->m_ExecutedQuantity
    , pQueryOrderExec->m_ExecutionPrice
    , pQueryOrderExec->m_ExecutionID
    , pQueryOrderExec->m_MarketExecID
  );

}

void CRemTraderHandler::OnSymbolStatusReport(EES_SymbolStatus* pSymbolStatus)
{
  CMN_DEBUG("Enter\n");

  CMN_DEBUG(
    "pSymbolStatus->m_ExchangeID=[%d]\n"
    "pSymbolStatus->m_Symbol=[%s]\n"
    "pSymbolStatus->m_InstrumentStatus=[%c]\n"
    "pSymbolStatus->m_TradingSegmentSN=[%d]\n"
    "pSymbolStatus->m_EnterTime=[%s]\n"
    "pSymbolStatus->m_EnterReason=[%c]\n"
    , pSymbolStatus->m_ExchangeID
    , pSymbolStatus->m_Symbol
    , pSymbolStatus->m_InstrumentStatus
    , pSymbolStatus->m_TradingSegmentSN
    , pSymbolStatus->m_EnterTime
    , pSymbolStatus->m_EnterReason
  );

  
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  instrument_status oInstrumentStatus;
  strncpy(oInstrumentStatus.InstrumentID, pSymbolStatus->m_Symbol, sizeof(oInstrumentStatus.InstrumentID));
  oInstrumentStatus.InstrumentStatus = pSymbolStatus->m_InstrumentStatus;
  
  trader_trader_api_on_rtn_instrument_status(self, &oInstrumentStatus);

}


