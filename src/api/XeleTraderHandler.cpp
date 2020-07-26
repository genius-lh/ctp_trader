#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CXeleTraderApi.hpp"
#include "XeleTraderHandler.h"
#include "XeleTraderOrderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "cmn_log.h"
#include "trader_trader_api.h"
#include "trader_trader_api_xele.h"

#ifdef __cplusplus
}
#endif

CXeleTraderHandler::CXeleTraderHandler(void* arg)
:m_Arg(arg)
{

}

CXeleTraderHandler::~CXeleTraderHandler()
{

}

void CXeleTraderHandler::OnFrontConnected()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_connected(self);
  return ;
}

void CXeleTraderHandler::OnFrontDisconnected(int nReason)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_on_front_disconnected(self, nReason);
  return;
}

void CXeleTraderHandler::OnRspError(CXeleFtdcRspInfoField *pRspInfo,
                      int nRequestID,
                      bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);
  return;
}

void CXeleTraderHandler::OnRspUserLogin(CXeleFtdcRspUserLoginField *pRspUserLogin,
                          CXeleFtdcRspInfoField *pRspInfo,
                          int nRequestID,
                          bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderOrderApi* pTraderApi = (CXeleTraderOrderApi*)pImp->pTraderApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  do{
    if(errNo){
      trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
      break;
    }
  
    if(pRspUserLogin){  
      // 获取交易日期
      strncpy(pImp->sTradingDay, pRspUserLogin->TradingDay, sizeof(pImp->sTradingDay));

      // 获取最大报单号
      strncpy(pImp->sMaxOrderLocalID, pRspUserLogin->MaxOrderLocalID, sizeof(pImp->sMaxOrderLocalID));
      trader_trader_api_get_max_order_local_id(self, pRspUserLogin->MaxOrderLocalID);

      pTraderApi->Init(pRspUserLogin->ClientIndex, pRspUserLogin->Token);
      
    }

    
  }while(0);
  
  return;
}

void CXeleTraderHandler::OnRspUserLogout(CXeleFtdcRspUserLogoutField *pRspUserLogout,
                           CXeleFtdcRspInfoField *pRspInfo,
                           int nRequestID,
                           bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_user_logout(self, errNo, errMsg);
  return ;
}

void CXeleTraderHandler::OnRspQryClientPosition(CXeleFtdcRspClientPositionField *pRspClientPosition,
                                  CXeleFtdcRspInfoField *pRspInfo,
                                  int nRequestID,
                                  bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_position traderPosition;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pRspClientPosition) {
    CMN_DEBUG( 
      "pRspClientPosition->TradingDay=[%s]"
      "pRspClientPosition->SettlementGroupID=[%s]"
      "pRspClientPosition->SettlementID=[%d]"
      "pRspClientPosition->HedgeFlag=[%c]"
      "pRspClientPosition->LongYdPosition=[%d]"
      "pRspClientPosition->LongPosition=[%d]"
      "pRspClientPosition->ShortYdPosition=[%d]"
      "pRspClientPosition->ShortPosition=[%d]"
      "pRspClientPosition->InstrumentID=[%s]"
      "pRspClientPosition->AccountID=[%s]"
      "\n"
      ,pRspClientPosition->TradingDay
      ,pRspClientPosition->SettlementGroupID
      ,pRspClientPosition->SettlementID
      ,pRspClientPosition->HedgeFlag
      ,pRspClientPosition->LongYdPosition
      ,pRspClientPosition->LongPosition
      ,pRspClientPosition->ShortYdPosition
      ,pRspClientPosition->ShortPosition
      ,pRspClientPosition->InstrumentID
      ,pRspClientPosition->AccountID
    );
    
    memset(&traderPosition, 0, sizeof(traderPosition));
    snprintf(traderPosition.InstrumentID, sizeof(traderPosition.InstrumentID), "%s", pRspClientPosition->InstrumentID);
    traderPosition.PositionDate = '1';
    traderPosition.IsSHFE = 0;
    traderPosition.PosiDirection = TRADER_POSITION_LONG;
    traderPosition.YdPosition = pRspClientPosition->LongYdPosition;
    traderPosition.TodayPosition = 0;
    traderPosition.Position = pRspClientPosition->LongPosition;
    traderPosition.LongFrozen = 0;
    trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, 0);

    traderPosition.PosiDirection = TRADER_POSITION_SHORT;
    traderPosition.YdPosition = pRspClientPosition->ShortYdPosition;
    traderPosition.TodayPosition = 0;
    traderPosition.Position = pRspClientPosition->ShortPosition;
    traderPosition.LongFrozen = 0;    
  }
  
  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);
  return;
}

void CXeleTraderHandler::OnRspQryInstrument(CXeleFtdcRspInstrumentField *pRspInstrument,
                              CXeleFtdcRspInfoField *pRspInfo,
                              int nRequestID,
                              bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_instrument traderInstrument;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(pRspInstrument){
    CMN_DEBUG(
      "pRspInstrument->SettlementGroupID=[%s]"
      "pRspInstrument->ProductID=[%s]"
      "pRspInstrument->ProductGroupID=[%s]"
      "pRspInstrument->UnderlyingInstrID=[%s]"
      "pRspInstrument->ProductClass=[%c]"
      "pRspInstrument->PositionType=[%c]"
      "pRspInstrument->StrikePrice=[%lf]"
      "pRspInstrument->OptionsType=[%c]"
      "pRspInstrument->VolumeMultiple=[%d]"
      "pRspInstrument->UnderlyingMultiple=[%lf]"
      "pRspInstrument->InstrumentID=[%s]"
      "pRspInstrument->InstrumentName=[%s]"
      "pRspInstrument->DeliveryYear=[%d]"
      "pRspInstrument->DeliveryMonth=[%d]"
      "pRspInstrument->AdvanceMonth=[%s]"
      "pRspInstrument->IsTrading=[%d]"
      "pRspInstrument->CreateDate=[%s]"
      "pRspInstrument->OpenDate=[%s]"
      "pRspInstrument->ExpireDate=[%s]"
      "pRspInstrument->StartDelivDate=[%s]"
      "pRspInstrument->EndDelivDate=[%s]"
      "pRspInstrument->BasisPrice=[%lf]"
      "pRspInstrument->MaxMarketOrderVolume=[%d]"
      "pRspInstrument->MinMarketOrderVolume=[%d]"
      "pRspInstrument->MaxLimitOrderVolume=[%d]"
      "pRspInstrument->MinLimitOrderVolume=[%d]"
      "pRspInstrument->PriceTick=[%lf]"
      "pRspInstrument->AllowDelivPersonOpen=[%d]"
      "\n"
      ,pRspInstrument->SettlementGroupID
      ,pRspInstrument->ProductID
      ,pRspInstrument->ProductGroupID
      ,pRspInstrument->UnderlyingInstrID
      ,pRspInstrument->ProductClass
      ,pRspInstrument->PositionType
      ,pRspInstrument->StrikePrice
      ,pRspInstrument->OptionsType
      ,pRspInstrument->VolumeMultiple
      ,pRspInstrument->UnderlyingMultiple
      ,pRspInstrument->InstrumentID
      ,pRspInstrument->InstrumentName
      ,pRspInstrument->DeliveryYear
      ,pRspInstrument->DeliveryMonth
      ,pRspInstrument->AdvanceMonth
      ,pRspInstrument->IsTrading
      ,pRspInstrument->CreateDate
      ,pRspInstrument->OpenDate
      ,pRspInstrument->ExpireDate
      ,pRspInstrument->StartDelivDate
      ,pRspInstrument->EndDelivDate
      ,pRspInstrument->BasisPrice
      ,pRspInstrument->MaxMarketOrderVolume
      ,pRspInstrument->MinMarketOrderVolume
      ,pRspInstrument->MaxLimitOrderVolume
      ,pRspInstrument->MinLimitOrderVolume
      ,pRspInstrument->PriceTick
      ,pRspInstrument->AllowDelivPersonOpen
    );
    memset(&traderInstrument, 0, sizeof(traderInstrument));
    strncpy(traderInstrument.InstrumentID, pRspInstrument->InstrumentID, sizeof(traderInstrument.InstrumentID));
    strncpy(traderInstrument.ExchangeID, "CFFEX", sizeof(traderInstrument.ExchangeID));
    traderInstrument.VolumeMultiple = pRspInstrument->VolumeMultiple;
    traderInstrument.PriceTick = pRspInstrument->PriceTick;

  }
  
  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);
  return;
}

void CXeleTraderHandler::OnRspQryClientAccount(CXeleFtdcRspClientAccountField *pClientAccount,
                                 CXeleFtdcRspInfoField *pRspInfo,
                                 int nRequestID,
                                 bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_account traderAccount;
  int errNo = 0;
  char* errMsg = NULL;

  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(pClientAccount){
    CMN_DEBUG(
      "pClientAccount->TradingDay=[%s]"
      "pClientAccount->SettlementGroupID=[%s]"
      "pClientAccount->SettlementID=[%d]"
      "pClientAccount->PreBalance=[%lf]"
      "pClientAccount->CurrMargin=[%lf]"
      "pClientAccount->CloseProfit=[%lf]"
      "pClientAccount->Premium=[%lf]"
      "pClientAccount->Deposit=[%lf]"
      "pClientAccount->Withdraw=[%lf]"
      "pClientAccount->Balance=[%lf]"
      "pClientAccount->Available=[%lf]"
      "pClientAccount->AccountID=[%s]"
      "pClientAccount->FrozenMargin=[%lf]"
      "pClientAccount->FrozenPremium=[%lf]"
      "pClientAccount->BaseReserve=[%lf]"
      "pClientAccount->floatProfitAndLoss=[%lf]"
      "\n"
      ,pClientAccount->TradingDay
      ,pClientAccount->SettlementGroupID
      ,pClientAccount->SettlementID
      ,pClientAccount->PreBalance
      ,pClientAccount->CurrMargin
      ,pClientAccount->CloseProfit
      ,pClientAccount->Premium
      ,pClientAccount->Deposit
      ,pClientAccount->Withdraw
      ,pClientAccount->Balance
      ,pClientAccount->Available
      ,pClientAccount->AccountID
      ,pClientAccount->FrozenMargin
      ,pClientAccount->FrozenPremium
      ,pClientAccount->BaseReserve
      ,pClientAccount->floatProfitAndLoss
    );
    
    memset(&traderAccount, 0, sizeof(traderAccount));
    strncpy(traderAccount.AccountID, pClientAccount->AccountID, sizeof(traderAccount.AccountID));
  }
  
  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, errNo, errMsg, bIsLast);

}



void CXeleTraderHandler::OnRtnInsInstrument(CXeleFtdcInstrumentField *pInstrument)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  if(pInstrument){
    CMN_DEBUG(
      "pInstrument->SettlementGroupID=[%s]"
      "pInstrument->ProductID=[%s]"
      "pInstrument->ProductGroupID=[%s]"
      "pInstrument->UnderlyingInstrID=[%s]"
      "pInstrument->ProductClass=[%c]"
      "pInstrument->PositionType=[%c]"
      "pInstrument->StrikePrice=[%lf]"
      "pInstrument->OptionsType=[%c]"
      "pInstrument->VolumeMultiple=[%d]"
      "pInstrument->UnderlyingMultiple=[%lf]"
      "pInstrument->InstrumentID=[%s]"
      "pInstrument->InstrumentName=[%s]"
      "pInstrument->DeliveryYear=[%d]"
      "pInstrument->DeliveryMonth=[%d]"
      "pInstrument->AdvanceMonth=[%s]"
      "pInstrument->IsTrading=[%d]"
      "\n"
      ,pInstrument->SettlementGroupID
      ,pInstrument->ProductID
      ,pInstrument->ProductGroupID
      ,pInstrument->UnderlyingInstrID
      ,pInstrument->ProductClass
      ,pInstrument->PositionType
      ,pInstrument->StrikePrice
      ,pInstrument->OptionsType
      ,pInstrument->VolumeMultiple
      ,pInstrument->UnderlyingMultiple
      ,pInstrument->InstrumentID
      ,pInstrument->InstrumentName
      ,pInstrument->DeliveryYear
      ,pInstrument->DeliveryMonth
      ,pInstrument->AdvanceMonth
      ,pInstrument->IsTrading
    );
  }

}



void CXeleTraderHandler::OnRspQryOrder(CXeleFtdcOrderField* pOrderField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  PrintOrder(pOrderField);
}

void CXeleTraderHandler::OnRspQryTrade(CXeleFtdcTradeField* pTradeField, CXeleFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  
  PrintTrade(pTradeField);
}

void CXeleTraderHandler::PrintTrade(CXeleFtdcTradeField *pTrade)
{
  CMN_DEBUG(
    "pTrade->TradingDay=[%s]"
    "pTrade->SettlementGroupID=[%s]"
    "pTrade->SettlementID=[%d]"
    "pTrade->TradeID=[%s]"
    "pTrade->Direction=[%c]"
    "pTrade->OrderSystemNo=[%d]"
    "pTrade->ParticipantID=[%s]"
    "pTrade->ClientID=[%s]"
    "pTrade->TradingRole=[%c]"
    "pTrade->AccountID=[%s]"
    "pTrade->InstrumentID=[%s]"
    "pTrade->OffsetFlag=[%c]"
    "pTrade->HedgeFlag=[%c]"
    "pTrade->Price=[%lf]"
    "pTrade->Volume=[%d]"
    "pTrade->TradeTime=[%s]"
    "pTrade->TradeType=[%c]"
    "pTrade->PriceSource=[%c]"
    "pTrade->UserID=[%s]"
    "pTrade->OrderLocalNo=[%d]"
    "pTrade->ExchangeOrderSysID=[%s]"
    "\n"
    ,pTrade->TradingDay
    ,pTrade->SettlementGroupID
    ,pTrade->SettlementID
    ,pTrade->TradeID
    ,pTrade->Direction
    ,pTrade->OrderSystemNo
    ,pTrade->ParticipantID
    ,pTrade->ClientID
    ,pTrade->TradingRole
    ,pTrade->AccountID
    ,pTrade->InstrumentID
    ,pTrade->OffsetFlag
    ,pTrade->HedgeFlag
    ,pTrade->Price
    ,pTrade->Volume
    ,pTrade->TradeTime
    ,pTrade->TradeType
    ,pTrade->PriceSource
    ,pTrade->UserID
    ,pTrade->OrderLocalNo
    ,pTrade->ExchangeOrderSysID
  );


}

void CXeleTraderHandler::PrintOrder(CXeleFtdcOrderField *pOrder)
{
  CMN_DEBUG(
    "pOrder->TradingDay=[%s]"
    "pOrder->SettlementGroupID=[%s]"
    "pOrder->SettlementID=[%d]"
    "pOrder->OrderSystemNo=[%d]"
    "pOrder->ParticipantID=[%s]"
    "pOrder->ClientID=[%s]"
    "pOrder->UserID=[%s]"
    "pOrder->InstrumentID=[%s]"
    "pOrder->OrderPriceType=[%c]"
    "pOrder->Direction=[%c]"
    "pOrder->CombOffsetFlag=[%s]"
    "pOrder->CombHedgeFlag=[%s]"
    "pOrder->LimitPrice=[%lf]"
    "pOrder->VolumeTotalOriginal=[%d]"
    "pOrder->TimeCondition=[%c]"
    "pOrder->GTDDate=[%s]"
    "pOrder->VolumeCondition=[%c]"
    "pOrder->MinVolume=[%d]"
    "pOrder->ContingentCondition=[%c]"
    "pOrder->StopPrice=[%lf]"
    "pOrder->ForceCloseReason=[%c]"
    "pOrder->OrderLocalNo=[%d]"
    "pOrder->IsAutoSuspend=[%d]"
    "pOrder->OrderSource=[%c]"
    "pOrder->OrderStatus=[%c]"
    "pOrder->OrderType=[%c]"
    "pOrder->VolumeTraded=[%d]"
    "pOrder->VolumeTotal=[%d]"
    "pOrder->InsertDate=[%s]"
    "pOrder->InsertTime=[%s]"
    "pOrder->ActiveTime=[%s]"
    "pOrder->SuspendTime=[%s]"
    "pOrder->UpdateTime=[%s]"
    "pOrder->CancelTime=[%s]"
    "pOrder->ActiveUserID=[%s]"
    "pOrder->Priority=[%d]"
    "pOrder->TimeSortID=[%d]"
    "pOrder->ExchangeOrderSysID=[%s]"
    "\n"
    ,pOrder->TradingDay
    ,pOrder->SettlementGroupID
    ,pOrder->SettlementID
    ,pOrder->OrderSystemNo
    ,pOrder->ParticipantID
    ,pOrder->ClientID
    ,pOrder->UserID
    ,pOrder->InstrumentID
    ,pOrder->OrderPriceType
    ,pOrder->Direction
    ,pOrder->CombOffsetFlag
    ,pOrder->CombHedgeFlag
    ,pOrder->LimitPrice
    ,pOrder->VolumeTotalOriginal
    ,pOrder->TimeCondition
    ,pOrder->GTDDate
    ,pOrder->VolumeCondition
    ,pOrder->MinVolume
    ,pOrder->ContingentCondition
    ,pOrder->StopPrice
    ,pOrder->ForceCloseReason
    ,pOrder->OrderLocalNo
    ,pOrder->IsAutoSuspend
    ,pOrder->OrderSource
    ,pOrder->OrderStatus
    ,pOrder->OrderType
    ,pOrder->VolumeTraded
    ,pOrder->VolumeTotal
    ,pOrder->InsertDate
    ,pOrder->InsertTime
    ,pOrder->ActiveTime
    ,pOrder->SuspendTime
    ,pOrder->UpdateTime
    ,pOrder->CancelTime
    ,pOrder->ActiveUserID
    ,pOrder->Priority
    ,pOrder->TimeSortID
    ,pOrder->ExchangeOrderSysID
  );

}

CXeleTraderOrderHandler::CXeleTraderOrderHandler(void* arg)
:m_Arg(arg)
{

}

CXeleTraderOrderHandler::~CXeleTraderOrderHandler()
{

}

void CXeleTraderOrderHandler::OnFrontConnected()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  CXeleTraderOrderApi* pTraderApi = (CXeleTraderOrderApi*)pImp->pTraderApi;
  //trader_trader_api_on_front_connected(self);
  pTraderApi->ReqUserLogin(pImp->nTraderRequestID++);
  return ;
}

void CXeleTraderOrderHandler::OnFrontDisconnected(int nReason)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  //trader_trader_api_on_front_disconnected(self, nReason);
  return;
}

void CXeleTraderOrderHandler::OnRspUserLogin(CXeleFtdcRspUserLoginField *pRspUserLogin,
                          CXeleFtdcRspInfoField *pRspInfo,
                          int nRequestID,
                          bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trader_api_xele* pImp = (trader_trader_api_xele*)self->pUserApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
    trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  }

  return;
}

void CXeleTraderOrderHandler::OnRspUserLogout(CXeleFtdcRspUserLogoutField *pRspUserLogout,
                           CXeleFtdcRspInfoField *pRspInfo,
                           int nRequestID,
                           bool bIsLast)
{
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
  }
  return ;
}

void CXeleTraderOrderHandler::OnRspOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                            CXeleFtdcRspInfoField *pRspInfo,
                            int nRequestID,
                            bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  trader_trader_api_on_rsp_order_insert(self, errNo, errMsg);
  return;
}

void CXeleTraderOrderHandler::OnRspOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                            CXeleFtdcRspInfoField *pRspInfo,
                            int nRequestID,
                            bool bIsLast)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo){
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  trader_trader_api_on_rsp_order_action(self, errNo, errMsg);
  return;

}

void CXeleTraderOrderHandler::OnRtnTrade(CXeleFtdcTradeField *pTrade)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  CXeleTraderHandler::PrintTrade(pTrade);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strncpy(traderTrade.InstrumentID, pTrade->InstrumentID, sizeof(traderTrade.InstrumentID));
  ///本地报单编号
  snprintf(traderTrade.UserOrderLocalID, sizeof(traderTrade.UserOrderLocalID), "%08d", pTrade->OrderLocalNo);
  ///交易日
  strncpy(traderTrade.TradingDay, pTrade->TradingDay, sizeof(traderTrade.TradingDay));
  ///成交时间
  strncpy(traderTrade.TradeTime, pTrade->TradeTime, sizeof(traderTrade.TradeTime));
  ///买卖方向
  traderTrade.Direction = pTrade->Direction;
  ///开平标志
  traderTrade.OffsetFlag = pTrade->OffsetFlag;
  ///成交价格
  traderTrade.TradePrice = pTrade->Price;
  ///成交数量
  traderTrade.TradeVolume = pTrade->Volume;
  //成交编号
  strncpy(traderTrade.TradeID, pTrade->TradeID, sizeof(traderTrade.TradeID));

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void CXeleTraderOrderHandler::OnRtnOrder(CXeleFtdcOrderField *pOrder)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  CXeleTraderHandler::PrintOrder(pOrder);
  
  trader_trader_api* self = (trader_trader_api*)m_Arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
	///交易所代码
  strncpy(traderOrder.ExchangeID, "CFFEX", sizeof(traderOrder.ExchangeID));
	///系统报单编号
  snprintf(traderOrder.OrderSysID, sizeof(traderOrder.OrderSysID), "%d", pOrder->OrderSystemNo);
  // 合约代码
  strncpy(traderOrder.InstrumentID, pOrder->InstrumentID, sizeof(traderOrder.InstrumentID));
  // 本地报单编号
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%08d", pOrder->OrderLocalNo);
  // 买卖
  traderOrder.Direction = pOrder->Direction;
  // 开平
  traderOrder.OffsetFlag = pOrder->CombOffsetFlag[0];
  ///投机套保标志
  traderOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  // 报单价格
  traderOrder.LimitPrice = pOrder->LimitPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->VolumeTotal;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pOrder->OrderStatus;
  ///插入时间
  strncpy(traderOrder.InsertTime, pOrder->InsertTime, sizeof(traderOrder.InsertTime));

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void CXeleTraderOrderHandler::OnErrRtnOrderInsert(CXeleFtdcInputOrderField *pInputOrder,
                               CXeleFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  trader_order traderOrder;

  if(pInputOrder){
    memset(&traderOrder, 0, sizeof(traderOrder));
  	///交易所代码
    strncpy(traderOrder.ExchangeID, "CFFEX", sizeof(traderOrder.ExchangeID));
  	///系统报单编号
    snprintf(traderOrder.OrderSysID, sizeof(traderOrder.OrderSysID), "%d", pInputOrder->OrderSystemNo);
    // 合约代码
    strncpy(traderOrder.InstrumentID, pInputOrder->InstrumentID, sizeof(traderOrder.InstrumentID));
    // 本地报单编号
    snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%08d", pInputOrder->OrderLocalNo);
    // 买卖
    traderOrder.Direction = pInputOrder->Direction;
    // 开平
    traderOrder.OffsetFlag = pInputOrder->CombOffsetFlag[0];
    ///投机套保标志
    traderOrder.HedgeFlag = pInputOrder->CombHedgeFlag[0];
    // 报单价格
    traderOrder.LimitPrice = pInputOrder->LimitPrice;
    // 报单手数
    traderOrder.VolumeOriginal = pInputOrder->VolumeTotalOriginal;
    // 成交手数
    traderOrder.VolumeTraded = 0;
    // 订单状态
    traderOrder.OrderStatus = TRADER_ORDER_OS_CANCELED;

    trader_trader_api_on_rtn_order(self, &traderOrder);
  }
  
  trader_trader_api_on_err_rtn_order_insert(self, errNo, errMsg);
  return;
}

void CXeleTraderOrderHandler::OnErrRtnOrderAction(CXeleFtdcOrderActionField *pOrderAction,
                               CXeleFtdcRspInfoField *pRspInfo)
{
  CMN_DEBUG("%s\n", __FUNCTION__);
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    CMN_DEBUG("pRspInfo->ErrorID=[%d]\n"
      "pRspInfo->ErrorMsg=[%s]\n",
      pRspInfo->ErrorID,
      pRspInfo->ErrorMsg);
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  if(pOrderAction){
    CMN_DEBUG(
      "pOrderAction->OrderSysID[%d]\n"
      "pOrderAction->UserID[%s]\n"
      "pOrderAction->UserOrderActionLocalID[%s]\n"
      "pOrderAction->UserOrderLocalID[%d]\n"
      , pOrderAction->OrderSystemNo
      , pOrderAction->ClientID
      , pOrderAction->ActionLocalID
      , pOrderAction->OrderLocalNo
      );
  }
  trader_trader_api_on_err_rtn_order_action(self, errNo, errMsg);
  return;
}


