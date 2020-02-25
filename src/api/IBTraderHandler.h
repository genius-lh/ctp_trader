#ifndef _IB_TRADER_HANDLER_H_
#define _IB_TRADER_HANDLER_H_

#include "EWrapper.h"
#include "IBTraderApi.h"
#include "IBFutureContractFactory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ib_trader_api_cb_def ib_trader_api_cb;

struct ib_trader_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg);
  void (*xOnPosition)(void* arg, const char* account, const char* contract, int position, double avg_cost);
  void (*xOnOrder)(void* arg, long orderId, const char* status, int filled, int remaining,
     double avgFillPrice, int permId, double lastFillPrice, int clientId, double mktCapPrice);
  void (*xOnNextValidId)(void* arg, long orderId);
  void (*xOnError)(void* arg, int errorCode, const char* errorMsg);
  
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

  void nextValidId( OrderId orderId);
  
  void position( const std::string& account, const Contract& contract, double position, double avgCost);

  void positionEnd();

private:
  ib_trader_api_cb* m_cb;
  void * m_parent;
  IBFutureContractFactory* m_contractFactory;
};

#endif //_IB_TRADER_HANDLER_H_


