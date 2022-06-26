#ifndef _IB_MDUSER_HANDLER_H_
#define _IB_MDUSER_HANDLER_H_

#include "EWrapper.h"
#include "IBMduserApi.h"
#include "IBFutureContractFactory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ib_mduser_api_cb_def ib_mduser_api_cb;

struct ib_mduser_api_cb_def{
  void (*xOnTickBidPrice)(void* arg, const char* instrument, double price);
  void (*xOnTickAskPrice)(void* arg, const char* instrument, double price);
  void (*xOnTickBidSize)(void* arg, const char* instrument, int size);
  void (*xOnTickAskSize)(void* arg, const char* instrument, int size);
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg);
  void (*xOnRspError)(void* arg, int errorId, const char* errorMsg);
};
#ifdef __cplusplus
}
#endif

class CIBMduserHandler : public CIbTraderSpi
{
public:
  // 构造函数，需要一个有效的指向CUstpFtdcMduserApi实例的指针
  CIBMduserHandler(ib_mduser_api_cb* cb, void * parent);
  
  virtual ~CIBMduserHandler() {};
  
  void tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attrib);
  
  void tickSize( TickerId tickerId, TickType field, int size);

  void tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, int bidSize, int askSize, const TickAttribBidAsk& tickAttribBidAsk);
    
  void error(int id, int errorCode, const std::string& errorString);

  void connectAck();
  
  void connectionClosed();

private:
  ib_mduser_api_cb* m_cb;
  void * m_parent;
  IBFutureContractFactory* m_contractFactory;
};
#endif //_IB_MDUSER_HANDLER_H_


