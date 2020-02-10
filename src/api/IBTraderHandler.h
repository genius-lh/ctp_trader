#ifndef _IB_TRADER_HANDLER_H_
#define _IB_TRADER_HANDLER_H_

#include "EWrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ib_trader_api_cb_def ib_trader_api_cb;

struct ib_trader_api_cb_def{
  void (*xOnTickBidPrice)(void* arg, long tickerId, double price);
  void (*xOnTickAskPrice)(void* arg, long tickerId, double price);
  void (*xOnTickBidSize)(void* arg, long tickerId, int size);
  void (*xOnTickAskSize)(void* arg, long tickerId, int size);
};
#ifdef __cplusplus
}
#endif

class CIBTraderHandler : public CIbTraderSpi
{
public:
  // 构造函数，需要一个有效的指向CUstpFtdcMduserApi实例的指针
  CIBTraderHandler(ib_trader_api_cb* cb, void * parent);
  
  virtual ~CIBTraderHandler() {};

  void error(int id, int errorCode, const std::string& errorString);
  
  void connectAck();
  
  void connectionClosed();
  
  void orderStatus(OrderId orderId, const std::string& status, double filled,
  		double remaining, double avgFillPrice, int permId, int parentId,
  		double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice);
  
  void openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState) ;
  
  void openOrderEnd();

  void completedOrder(const Contract& contract, const Order& order, const OrderState& orderState);

  void completedOrdersEnd() ;

  void nextValidId( OrderId orderId);

private:
  ib_trader_api_cb* m_cb;
  void * m_parent;
};

#endif //_IB_TRADER_HANDLER_H_


