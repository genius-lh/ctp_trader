

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trader_data.h"
#include "trader_msg_struct.h"
#include "queue.h"

#include "cmn_log.h"

#include "trader_strategy_limit.h"


static void trader_strategy_limit_on_init(trader_strategy_limit* self, investor_position* p_position);
static void trader_strategy_limit_on_trade(trader_strategy_limit* self, trader_trade* p_trade, int is_shfe);
static void trader_strategy_limit_on_order(trader_strategy_limit* self, trader_order* p_order, int is_shfe);
static int trader_strategy_limit_get_available(trader_strategy_limit* self, char* instrument_id, char direction, int* yd_volumn);

static trader_strategy_limit_method* trader_strategy_limit_method_get();

static trader_strategy_limit_position* trader_strategy_limit_position_search(trader_strategy_limit* self, char* instrument_id, char direction);

trader_strategy_limit_method* trader_strategy_limit_method_get()
{
  static trader_strategy_limit_method trader_strategy_limit_method_st = {
    trader_strategy_limit_on_init,
    trader_strategy_limit_on_trade,
    trader_strategy_limit_on_order,
    trader_strategy_limit_get_available
  };

  return &trader_strategy_limit_method_st;
}

trader_strategy_limit_position* trader_strategy_limit_position_search(trader_strategy_limit* self, char* instrument_id, char direction)
{
  trader_strategy_limit_position* pLimitPosition = NULL;
  trader_strategy_limit_position* pIter;
  
  TAILQ_FOREACH(pIter, &self->hPositionHead, next) {
    if(!strcmp(pIter->InstrumentID, instrument_id)){
      if(direction == pIter->PosiDirection){
        pLimitPosition = pIter;
        break;
      }
    }
  }
  return pLimitPosition;
}


void trader_strategy_limit_on_init(trader_strategy_limit* self, investor_position* p_position)
{
  investor_position* pInvestorPosition = p_position;

  trader_strategy_limit_position* pLimitPosition;
  int TdPosition;

  pLimitPosition = trader_strategy_limit_position_search(self, pInvestorPosition->InstrumentID, pInvestorPosition->PosiDirection);

  if(!pLimitPosition){
    pLimitPosition = (trader_strategy_limit_position*)malloc(sizeof(trader_strategy_limit_position));
    strcpy(pLimitPosition->InstrumentID, pInvestorPosition->InstrumentID);
    pLimitPosition->PosiDirection = pInvestorPosition->PosiDirection;
    pLimitPosition->IsSHFE = pInvestorPosition->IsSHFE;
    pLimitPosition->Position = 0;
    pLimitPosition->Fronze = 0;
    pLimitPosition->YdPosition = 0;
    pLimitPosition->YdFronze = 0;
    TAILQ_INSERT_TAIL(&self->hPositionHead, pLimitPosition, next);
  }
  if(!pInvestorPosition->IsSHFE) {
    pLimitPosition->YdPosition = pInvestorPosition->Position;
    pLimitPosition->YdFronze = pInvestorPosition->LongFrozen;
  }else{
    if(TRADER_POSITION_DATE_TODAY == pInvestorPosition->PositionDate){
      pLimitPosition->Position = pInvestorPosition->Position;
      pLimitPosition->Fronze = pInvestorPosition->LongFrozen;
    }else{
      pLimitPosition->YdPosition = pInvestorPosition->Position;
      pLimitPosition->YdFronze = pInvestorPosition->LongFrozen;
    }
  }
  

  CMN_DEBUG("\n"
    "pLimitPosition->InstrumentID=[%s]\n"
    "pLimitPosition->PosiDirection=[%c]\n"
    "pLimitPosition->IsSHFE=[%d]\n"
    "pLimitPosition->Position=[%d]\n"
    "pLimitPosition->Fronze=[%d]\n"
    "pLimitPosition->YdPosition=[%d]\n"
    "pLimitPosition->YdFronze=[%d]\n",
    pLimitPosition->InstrumentID,
    pLimitPosition->PosiDirection,
    pLimitPosition->IsSHFE,
    pLimitPosition->Position,
    pLimitPosition->Fronze,
    pLimitPosition->YdPosition,
    pLimitPosition->YdFronze
  );


  return ;
}

void trader_strategy_limit_on_trade(trader_strategy_limit* self, trader_trade* p_trade, int is_shfe)
{
  CMN_DEBUG("Enter!\n");
  trader_strategy_limit_position* pLimitPosition;

  trader_trade* pTrade = p_trade;
  
  char cPosiDirection = TRADER_POSITION_LONG;
  if(TRADER_POSITION_OPEN == pTrade->OffsetFlag){
    if(TRADER_POSITION_SELL == pTrade->Direction){
      cPosiDirection = TRADER_POSITION_SHORT;
    }
  }else{
    if(TRADER_POSITION_BUY == pTrade->Direction){
      cPosiDirection = TRADER_POSITION_SHORT;
    }
  }

  pLimitPosition = trader_strategy_limit_position_search(self, pTrade->InstrumentID, cPosiDirection);
  if(!pLimitPosition){
    pLimitPosition = (trader_strategy_limit_position*)malloc(sizeof(trader_strategy_limit_position));
    strcpy(pLimitPosition->InstrumentID, pTrade->InstrumentID);
    pLimitPosition->PosiDirection = cPosiDirection;
    pLimitPosition->IsSHFE = is_shfe;
    pLimitPosition->Position = 0;
    pLimitPosition->Fronze = 0;
    pLimitPosition->YdPosition = 0;
    pLimitPosition->YdFronze = 0;
    TAILQ_INSERT_TAIL(&self->hPositionHead, pLimitPosition, next);
  }
  
  if(pLimitPosition->IsSHFE){
    if(TRADER_POSITION_OPEN == pTrade->OffsetFlag){
      pLimitPosition->Position += pTrade->TradeVolume;
    }else{
      if(TRADER_POSITION_CLOSE_TODAY == pTrade->OffsetFlag){
        pLimitPosition->Position -= pTrade->TradeVolume;
        pLimitPosition->Fronze -= pTrade->TradeVolume;
      }else{
        pLimitPosition->YdFronze -= pTrade->TradeVolume;
        pLimitPosition->YdPosition -= pTrade->TradeVolume;
      }
    }
  }else{
    if(TRADER_POSITION_OPEN == pTrade->OffsetFlag){
      pLimitPosition->YdPosition += pTrade->TradeVolume;
    }else{
      pLimitPosition->YdFronze -= pTrade->TradeVolume;
      pLimitPosition->YdPosition -= pTrade->TradeVolume;
    }
  }
  
  CMN_DEBUG("\n"
    "pLimitPosition->InstrumentID=[%s]\n"
    "pLimitPosition->PosiDirection=[%c]\n"
    "pLimitPosition->IsSHFE=[%d]\n"
    "pLimitPosition->Position=[%d]\n"
    "pLimitPosition->Fronze=[%d]\n"
    "pLimitPosition->YdPosition=[%d]\n"
    "pLimitPosition->YdFronze=[%d]\n",
    pLimitPosition->InstrumentID,
    pLimitPosition->PosiDirection,
    pLimitPosition->IsSHFE,
    pLimitPosition->Position,
    pLimitPosition->Fronze,
    pLimitPosition->YdPosition,
    pLimitPosition->YdFronze
  );


  
  return;
}

void trader_strategy_limit_on_order(trader_strategy_limit* self, trader_order* p_order, int is_shfe)
{
  CMN_DEBUG("Enter!\n");
  trader_strategy_limit_position* pLimitPosition;

  trader_order* pOrder = p_order;

  if(TRADER_POSITION_OPEN == pOrder->OffsetFlag){
    return ;
  }
  
  if((TRADER_ORDER_OS_CANCELED != pOrder->OrderStatus)
  &&(TRADER_ORDER_OS_BEGIN != pOrder->OrderStatus)
  &&(TRADER_ORDER_OS_ACCEPTEDNOREPLY != pOrder->OrderStatus)) {
    return ;
  }
  
  char cPosiDirection = TRADER_POSITION_LONG;
  if(TRADER_POSITION_BUY == pOrder->Direction){
    cPosiDirection = TRADER_POSITION_SHORT;
  }

  pLimitPosition = trader_strategy_limit_position_search(self, pOrder->InstrumentID, cPosiDirection);
  if(!pLimitPosition){
    CMN_ERROR("Cannot reach hear!\n");
    pLimitPosition = (trader_strategy_limit_position*)malloc(sizeof(trader_strategy_limit_position));
    strcpy(pLimitPosition->InstrumentID, pOrder->InstrumentID);
    pLimitPosition->PosiDirection = cPosiDirection;
    pLimitPosition->IsSHFE = is_shfe;
    pLimitPosition->Position = 0;
    pLimitPosition->Fronze = 0;
    pLimitPosition->YdPosition = 0;
    pLimitPosition->YdFronze = 0;
    TAILQ_INSERT_TAIL(&self->hPositionHead, pLimitPosition, next);
  }

  if(TRADER_POSITION_CLOSE == pOrder->OffsetFlag){
    if(TRADER_ORDER_OS_BEGIN == pOrder->OrderStatus){
        pLimitPosition->YdFronze += pOrder->VolumeOriginal;
    }else if((TRADER_ORDER_OS_CANCELED == pOrder->OrderStatus)
    ||(TRADER_ORDER_OS_ACCEPTEDNOREPLY == pOrder->OrderStatus)){
      pLimitPosition->YdFronze -= (pOrder->VolumeOriginal - pOrder->VolumeTraded);
    }
  }else{
    if(TRADER_ORDER_OS_BEGIN == pOrder->OrderStatus){
        pLimitPosition->Fronze += pOrder->VolumeOriginal;
    }else if((TRADER_ORDER_OS_CANCELED == pOrder->OrderStatus)
    ||(TRADER_ORDER_OS_ACCEPTEDNOREPLY == pOrder->OrderStatus)){
      pLimitPosition->Fronze -= (pOrder->VolumeOriginal - pOrder->VolumeTraded);
    }
  }
  
  CMN_DEBUG("\n"
    "pLimitPosition->InstrumentID=[%s]\n"
    "pLimitPosition->PosiDirection=[%c]\n"
    "pLimitPosition->IsSHFE=[%d]\n"
    "pLimitPosition->Position=[%d]\n"
    "pLimitPosition->Fronze=[%d]\n"
    "pLimitPosition->YdPosition=[%d]\n"
    "pLimitPosition->YdFronze=[%d]\n",
    pLimitPosition->InstrumentID,
    pLimitPosition->PosiDirection,
    pLimitPosition->IsSHFE,
    pLimitPosition->Position,
    pLimitPosition->Fronze,
    pLimitPosition->YdPosition,
    pLimitPosition->YdFronze
  );


  
  return;
}

int trader_strategy_limit_get_available(trader_strategy_limit* self, char* instrument_id, char direction, int* yd_volumn)
{
  trader_strategy_limit_position* pLimitPosition;
  int nAvailable;
  int nYdVolumn;
  
  char cPosiDirection = TRADER_POSITION_LONG;
  if(TRADER_POSITION_BUY == direction){
    cPosiDirection = TRADER_POSITION_SHORT;
  }

  pLimitPosition = trader_strategy_limit_position_search(self, instrument_id, cPosiDirection);

  *yd_volumn = 0;
  if(!pLimitPosition){
    return 0;
  }

  CMN_DEBUG("\n"
    "pLimitPosition->InstrumentID=[%s]\n"
    "pLimitPosition->PosiDirection=[%c]\n"
    "pLimitPosition->IsSHFE=[%d]\n"
    "pLimitPosition->Position=[%d]\n"
    "pLimitPosition->Fronze=[%d]\n"
    "pLimitPosition->YdPosition=[%d]\n"
    "pLimitPosition->YdFronze=[%d]\n",
    pLimitPosition->InstrumentID,
    pLimitPosition->PosiDirection,
    pLimitPosition->IsSHFE,
    pLimitPosition->Position,
    pLimitPosition->Fronze,
    pLimitPosition->YdPosition,
    pLimitPosition->YdFronze
  );
  
  nYdVolumn = pLimitPosition->YdPosition - pLimitPosition->YdFronze;
  if(!pLimitPosition->IsSHFE){
    nAvailable = nYdVolumn;
    nYdVolumn = 0;
  }else{
    nAvailable = pLimitPosition->Position - pLimitPosition->Fronze;
    nAvailable += nYdVolumn;
  }
  *yd_volumn = nYdVolumn;
  
  return nAvailable;
}


trader_strategy_limit* trader_strategy_limit_new()
{
  trader_strategy_limit* self = (trader_strategy_limit*)malloc(sizeof(trader_strategy_limit));

  TAILQ_INIT(&self->hPositionHead);
  
  self->pMethod = trader_strategy_limit_method_get();
  
  return self;
}


void trader_strategy_limit_free(trader_strategy_limit* self)
{
  trader_strategy_limit_position* pIter;
  while((pIter = TAILQ_FIRST(&self->hPositionHead))!=NULL){
  	TAILQ_REMOVE(&self->hPositionHead, pIter, next);
    free(pIter);
  }

  if(self){
    free(self);
  }
}



