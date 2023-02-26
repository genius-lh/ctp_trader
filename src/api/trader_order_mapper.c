
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trader_order_mapper.h"

static trader_order_mapper_method* trader_order_mapper_method_get();
static int trader_order_mapper_init(trader_order_mapper* self, long nBaseOrderId, long nSize);
static int trader_order_mapper_insert_order(trader_order_mapper* self, char* contract, long sysOrderId,
  long localUserOrderId, char direction, char offset, int volumn);
static int trader_order_mapper_update_order(trader_order_mapper* self, long sysOrderId, int filled, int remaining, char status);
static int trader_order_mapper_find_order(trader_order_mapper* self, long sysOrderId, trader_order_view** orderView);

trader_order_mapper_method* trader_order_mapper_method_get()
{
  static trader_order_mapper_method trader_order_mapper_method_st = {
    trader_order_mapper_init,
    trader_order_mapper_insert_order,
    trader_order_mapper_update_order,
    trader_order_mapper_find_order
  };
  return &trader_order_mapper_method_st;
}

trader_order_mapper* trader_order_mapper_new()
{
  trader_order_mapper* self = (trader_order_mapper*)malloc(sizeof(trader_order_mapper));

  self->nSize = 0;
  self->nBaseOrderId = 0;
  self->pOrderView = NULL;

  self->pMethod = trader_order_mapper_method_get();

  return self;
}

void trader_order_mapper_free(trader_order_mapper* self)
{
  if(self->nSize > 0){
    free(self->pOrderView);
  }
}

int trader_order_mapper_init(trader_order_mapper* self, long nBaseOrderId, long nSize)
{
  int i;
  self->nBaseOrderId = nBaseOrderId;
  self->nSize = nSize;
  self->pOrderView = (trader_order_view*)malloc(self->nSize * sizeof(trader_order_view));
  for(i = 0; i < self->nSize; i++){
    self->pOrderView[i].used = 0;
  }
  return 0;
}

int trader_order_mapper_insert_order(trader_order_mapper* self, char* contract, long sysOrderId, long localUserOrderId, char direction, char offset, int volumn)
{
  int orderIndex = sysOrderId - self->nBaseOrderId;
  trader_order_view* pOrderView;
  int i;
  int nOldSize = self->nSize;
  if(orderIndex >= self->nSize){
    self->nSize *= 2;
    self->pOrderView = realloc(self->pOrderView, self->nSize * sizeof(trader_order_view));
    for(i = nOldSize; i < self->nSize; i++){
      self->pOrderView[i].used = 0;
    }
  }

  pOrderView = &self->pOrderView[orderIndex];
  if(0 != pOrderView->used){
    return -1;
  }
  
  pOrderView->used = 1;
  pOrderView->sysOrderId = sysOrderId;
  pOrderView->localUserOrderId = localUserOrderId;
  pOrderView->status = '-';
  pOrderView->direction = direction;
  pOrderView->offsetFlag = offset;
  pOrderView->total = volumn;
  pOrderView->filled = 0;
  pOrderView->remaining = volumn;
  strncpy(pOrderView->contract, contract, sizeof(pOrderView->contract));

  return 0;
}

int trader_order_mapper_update_order(trader_order_mapper* self, long sysOrderId, int filled, int remaining, char status)
{
  int orderIndex = sysOrderId - self->nBaseOrderId;
  trader_order_view* pOrderView;
  pOrderView = &self->pOrderView[orderIndex];
  if(0 == pOrderView->used){
    return -1;
  }

  if(filled <= pOrderView->filled){
    return -2;
  }
  
  pOrderView->filled = filled;
  pOrderView->remaining = remaining;
  pOrderView->status = status;

  return 0;
}

int trader_order_mapper_find_order(trader_order_mapper* self, long sysOrderId, trader_order_view** orderView)
{
  int orderIndex = sysOrderId - self->nBaseOrderId;
  trader_order_view* pOrderView = NULL;
  int nRet = 0;

  do{
    if(orderIndex > self->nSize){
      nRet = -1;
      break;
    }
    pOrderView = &self->pOrderView[orderIndex];
    if(0 == pOrderView->used){
      nRet = -2;
      break;
    }
  }while(0);

  *orderView = pOrderView;
  return nRet;
}


