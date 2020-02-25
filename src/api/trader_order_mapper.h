#ifndef _TRADER_ORDER_MAPPER_H_
#define _TRADER_ORDER_MAPPER_H_


typedef struct trader_order_view_def trader_order_view;
typedef struct trader_order_mapper_def trader_order_mapper;
typedef struct trader_order_mapper_method_def trader_order_mapper_method;

#define TRADER_ORDER_DEFAULT_SIZE 64

struct trader_order_view_def{
  long sysOrderId;
  long localUserOrderId;
  char used;
  char status;
  char direction;
  char offsetFlag;
  int total;
  int filled;
  int remaining;
  char contract[32];
};

struct trader_order_mapper_def{
  long nSize;
  long nBaseOrderId;
  trader_order_view* pOrderView;

  trader_order_mapper_method* pMethod;
};

struct trader_order_mapper_method_def{
  int (*xInit)(trader_order_mapper* self, long nBaseOrderId, long nSize);
  int (*xInsertOrder)(trader_order_mapper* self, char* contract, long sysOrderId, long localUserOrderId, char direction, char offset, int volumn);
  int (*xUpdateOrder)(trader_order_mapper* self, long sysOrderId, int filled, int remaining, char status);
  int (*xFindOrder)(trader_order_mapper* self, long sysOrderId, trader_order_view** orderView);
};

extern trader_order_mapper* trader_order_mapper_new();
extern void trader_order_mapper_free(trader_order_mapper* self);

#endif //_TRADER_ORDER_MAPPER_H_


