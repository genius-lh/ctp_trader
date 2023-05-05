#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "ydApi.h"
#include "ydError.h"
#include "ydUtil.h"

#include "YdTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_trader_api.h"

#include "cmn_log.h"

#ifdef __cplusplus
}
#endif

CYdTraderHandler::CYdTraderHandler(YDExtendedApi* pApi, void* pArg)
: m_Arg(pArg)
, m_TraderApi(pApi)
, m_DestroyMutex(PTHREAD_MUTEX_INITIALIZER)
, m_DestroyCond(PTHREAD_COND_INITIALIZER)
, m_IsLogin(0)
{
}

CYdTraderHandler::~CYdTraderHandler()
{

}

void CYdTraderHandler::notifyBeforeApiDestroy(void)
{
  CMN_DEBUG("Enter\n");
}

void CYdTraderHandler::notifyAfterApiDestroy(void)
{
  CMN_DEBUG("Enter\n");
  pthread_mutex_lock(&m_DestroyMutex);
  pthread_cond_signal(&m_DestroyCond);
  pthread_mutex_unlock(&m_DestroyMutex);
}


void CYdTraderHandler::notifyEvent(int apiEvent)
{
  CMN_DEBUG("Enter\n");
}

void CYdTraderHandler::notifyReadyForLogin(bool hasLoginFailed)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  if(!m_IsLogin){
    trader_trader_api_on_front_connected(self);
  }else{
    CMN_ERROR("Something wrong!\n");
  }

}

void CYdTraderHandler::notifyLogin(int errorNo,int maxOrderRef,bool isMonitor)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
	if (0 == errorNo){
    CMN_DEBUG("login successfully\n");
    m_MaxOrderRef = maxOrderRef;
    m_TradingDate = m_TraderApi->getTradingDay();
    m_IsLogin = 1;
  }else{
    CMN_INFO("login failed, errorNo=%d\n",errorNo);
    trader_trader_api_on_rsp_user_login(self, errorNo, "Failed!");
  }

}

void CYdTraderHandler::notifyFinishInit(void)
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  if(m_IsLogin){
    trader_trader_api_on_rsp_user_login(self, 0, NULL);
  }
}


void CYdTraderHandler::notifyCaughtUp(void)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  if(!m_IsLogin){
    return;
  }
  
  const YDInstrument *pInstrument;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  CMN_DEBUG("m_TraderApi->getInstrumentCount()=[%d]\n", m_TraderApi->getInstrumentCount());
  for(int i = 0; i < m_TraderApi->getInstrumentCount(); i++) {
    pInstrument = m_TraderApi->getInstrument(i);
    if(!pInstrument){
      CMN_DEBUG("NULL == pInstrument\n");
      continue;
    }
    
    strncpy(traderInstrument.InstrumentID, pInstrument->InstrumentID, sizeof(traderInstrument.InstrumentID));
    strncpy(traderInstrument.ExchangeID, pInstrument->m_pExchange->ExchangeID, sizeof(traderInstrument.ExchangeID));
    traderInstrument.VolumeMultiple = pInstrument->Multiple;
    traderInstrument.PriceTick = pInstrument->Tick;
    
    trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, 0);
  }

  
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, 0, NULL, 1);

}

void CYdTraderHandler::notifyOrder(const YDOrder *pOrder,const YDInstrument *pInstrument,const YDAccount *pAccount)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  
  strncpy(traderOrder.ExchangeID, pInstrument->m_pExchange->ExchangeID, sizeof(traderOrder.ExchangeID));
  snprintf(traderOrder.OrderSysID, sizeof(traderOrder.OrderSysID), "%d", pOrder->OrderSysID);
  // 合约代码
  strncpy(traderOrder.InstrumentID, pInstrument->InstrumentID, sizeof(traderOrder.InstrumentID));
  // 本地报单编号
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%08d", pOrder->OrderRef);
  // 买卖
  traderOrder.Direction = '0' + pOrder->Direction;
  // 开平
  traderOrder.OffsetFlag = '0' + pOrder->OffsetFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = '0' + pOrder->HedgeFlag;
  // 报单价格
  traderOrder.LimitPrice = pOrder->Price;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->OrderVolume;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->TradeVolume;
  // 订单状态
  traderOrder.OrderStatus = ConvertOrderStatus(pOrder->OrderStatus);
  ///插入时间
  timeID2String(pOrder->InsertTime, traderOrder.InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
}

void CYdTraderHandler::notifyTrade(const YDTrade *pTrade,const YDInstrument *pInstrument,const YDAccount *pAccount)
{
  CMN_DEBUG("Enter\n");
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strncpy(traderTrade.InstrumentID, pInstrument->InstrumentID, sizeof(traderTrade.InstrumentID));
  ///本地报单编号
  snprintf(traderTrade.UserOrderLocalID, sizeof(traderTrade.UserOrderLocalID), "%08d", pTrade->OrderRef);
  ///交易日
  snprintf(traderTrade.TradingDay, sizeof(traderTrade.TradingDay), "%d", m_TradingDate);
  ///成交时间
  timeID2String(pTrade->TradeTime, traderTrade.TradeTime);
  ///买卖方向
  traderTrade.Direction = '0' + pTrade->Direction;
  ///开平标志
  traderTrade.OffsetFlag = '0' + pTrade->OffsetFlag;
  ///成交价格
  traderTrade.TradePrice = pTrade->Price;
  ///成交数量
  traderTrade.TradeVolume = pTrade->Volume;
  //成交编号
  snprintf(traderTrade.TradeID, sizeof(traderTrade.TradeID), "%d", pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void CYdTraderHandler::notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder,const YDExchange *pExchange,const YDAccount *pAccount)
{
  CMN_DEBUG("Enter\n");

}

void CYdTraderHandler::Join()
{
  pthread_mutex_lock(&m_DestroyMutex);
  pthread_cond_wait(&m_DestroyCond, &m_DestroyMutex);
  pthread_mutex_unlock(&m_DestroyMutex);
}


void CYdTraderHandler::InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  YDInputOrder inputOrder;
  memset(&inputOrder,0,sizeof(inputOrder));

  const YDInstrument* pInstrument = m_TraderApi->getInstrumentByID(inst);

  inputOrder.OffsetFlag = (open_close & 0xf) ;
  inputOrder.Price= price;
	inputOrder.Direction=(buy_sell & 0xf);
  inputOrder.HedgeFlag=YD_HF_Speculation;
  inputOrder.OrderVolume=vol;
  inputOrder.OrderType=YD_ODT_Limit;
  inputOrder.YDOrderFlag=YD_YOF_Normal;
  inputOrder.ConnectionSelectionType=YD_CS_Any;
  inputOrder.ConnectionID=0;
  inputOrder.OrderRef=atoi(local_id);
  
  m_TraderApi->insertOrder(&inputOrder, pInstrument);
}

void CYdTraderHandler::CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id)
{
  YDCancelOrder cancelOrder;
  memset(&cancelOrder, 0, sizeof(cancelOrder));
  
  cancelOrder.YDOrderFlag = YD_YOF_Normal;
  cancelOrder.OrderGroupID = 0;
  cancelOrder.LongOrderSysID = 0;
  cancelOrder.OrderSysID = atoi(order_sys_id);

  const YDExchange* pExchange = m_TraderApi->getExchangeByID(exchange_id);
  
  m_TraderApi->cancelOrder(&cancelOrder, pExchange);

}

void CYdTraderHandler::QryInstrument()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  const YDInstrument *pInstrument;
  trader_instrument traderInstrument;
  // skip
}

void CYdTraderHandler::QryInvestorPosition()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  const YDExtendedPosition* positions[128];
  YDExtendedPositionFilter filter;
  memset(&filter, 0, sizeof(filter));
  filter.PositionDate = -1;
  filter.PositionDirection = -1;
  filter.HedgeFlag = -1;
  filter.pInstrument = NULL;
  unsigned count = m_TraderApi->findExtendedPositions(&filter, sizeof(positions) / sizeof(YDExtendedPosition*), positions);

  for(int j = 0; j < (int)count; j++){
    CMN_INFO("positions[j]->getInstrument()->InstrumentID=[%s]\n"
      "positions->PositionDate=[%d]\n"
      "positions->PositionDirection=[%d]\n"
      "positions->HedgeFlag=[%d]\n"
      "positions->OpenFrozen=[%d]\n"
      "positions->Position=[%d]\n"
      , positions[j]->getInstrument()->InstrumentID
      , positions[j]->PositionDate
      , positions[j]->PositionDirection
      , positions[j]->HedgeFlag
      , positions[j]->OpenFrozen
      , positions[j]->Position
    );

    strncpy(traderPosition.InstrumentID, positions[j]->getInstrument()->InstrumentID, sizeof(traderPosition.InstrumentID));
    traderPosition.PositionDate = '0' + positions[j]->PositionDate;
    traderPosition.PosiDirection = '0' + positions[j]->PositionDirection;
    if(positions[j]->getInstrument()->m_pExchange->UseTodayPosition){
      traderPosition.IsSHFE = 1;
      if(YD_PSD_Today == positions[j]->PositionDirection){
        traderPosition.YdPosition = 0;
        traderPosition.TodayPosition = positions[j]->Position;
        traderPosition.Position = positions[j]->Position;
      }else{
        traderPosition.YdPosition = positions[j]->Position;
        traderPosition.TodayPosition = 0;
        traderPosition.Position = positions[j]->Position;
      }
    }else{
      traderPosition.IsSHFE = 0;
      traderPosition.YdPosition = positions[j]->Position;
      traderPosition.TodayPosition = 0;
      traderPosition.Position = positions[j]->Position;
    
    }
    traderPosition.Position = positions[j]->Position;
    traderPosition.LongFrozen = positions[j]->CloseFrozen;
    trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, 0, NULL, (j == (count-1)));
  }
  return;
}

void CYdTraderHandler::QryTradingAccount()
{
  trader_trader_api* self = (trader_trader_api*)m_Arg;
  const YDExtendedAccount *account = m_TraderApi->getExtendedAccount();
  CMN_INFO("account->CloseProfit=[%f]\n"
    "account->CashIn=[%f]\n"
    "account->OtherCloseProfit=[%f]\n"
    "account->Commission=[%f]\n"
    "account->Margin=[%f]\n"
    "account->PositionProfit=[%f]\n"
    "account->Balance=[%f]\n"
    "account->UsedOrderCount=[%u]\n"
    "account->Useless=[%d]\n"
    "account->ExecMargin=[%f]\n"
    "account->OptionLongPositionCost=[%f]\n"
    "account->OptionLongPositionCostLimit=[%f]\n"
    "account->PrePositionMarketValue=[%f]\n"
    "account->PositionMarketValue=[%f]\n"
    "account->ExecAllocatedAmount=[%f]\n"
    , account->CloseProfit
    , account->CashIn
    , account->OtherCloseProfit
    , account->Commission
    , account->Margin
    , account->PositionProfit
    , account->Balance
    , account->UsedOrderCount
    , account->Useless
    , account->ExecMargin
    , account->OptionLongPositionCost
    , account->OptionLongPositionCostLimit
    , account->PrePositionMarketValue
    , account->PositionMarketValue
    , account->ExecAllocatedAmount
  );

  
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, 0, NULL, 1);

}

int CYdTraderHandler::GetTradingDate()
{
  return m_TradingDate;
}
int CYdTraderHandler::GetMaxOrderRef()
{
  return m_MaxOrderRef;
}

const char CYdTraderHandler::ConvertOrderStatus(int yd_order_status)
{
  
  const char ORDER_STATUS_TABLE[] = {
    //const int YD_OS_Accepted=0;
    TRADER_ORDER_OS_UNKNOW,
    //const int YD_OS_Queuing=1;
    TRADER_ORDER_OS_NOTRADEQUEUEING,
    //const int YD_OS_Canceled=2;
    TRADER_ORDER_OS_CANCELED,
    //const int YD_OS_AllTraded=3;
    TRADER_ORDER_OS_ALLTRADED,
    //const int YD_OS_Rejected=4;
    TRADER_ORDER_OS_CANCELED
  };

  return ORDER_STATUS_TABLE[yd_order_status];

}


