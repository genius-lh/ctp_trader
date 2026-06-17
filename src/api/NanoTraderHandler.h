#ifndef _NANO_TRADER_HANDLER_H_
#define _NANO_TRADER_HANDLER_H_

#include "HSDataType.h"
#include "HSStruct.h"
#include "HSExtendTradeApi.h"
#include "HSExtendStruct.h"

#include <map>
#include <string>

class CNanoTraderHandler : public CHSExtendTradeSpi
{
public:
  CNanoTraderHandler(CHSExtendTradeApi* pApi, void* pArg);
  virtual ~CNanoTraderHandler();
  void OnFrontConnected() override;
  void OnFrontDisconnected(int nResult) override;
  void OnRspAuthenticate(CHSRspAuthenticateField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspUserLogin(CHSRspUserLoginField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspErrorOrderInsert(CHSRspOrderInsertField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspOrderAction(CHSRspOrderActionField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQryPosition(CHSRspQryPositionField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQryTradingAccount(CHSRspQryTradingAccountField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQryInstrument(CHSRspQryInstrumentField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQryPositionDetail(CHSRspQryPositionDetailField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQryDepthMarketData(CHSDepthMarketDataField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRspQrySeatID(CHSRspQrySeatIDField*, CHSRspInfoField*, int nRequestID, bool bIsLast) override;
  void OnRtnTrade(CHSTradeField*) override;
  void OnRtnOrder(CHSOrderField*) override;
  void OnErrRtnOrderAction(CHSOrderActionField*) override;

public:  
  void InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void CancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id);
  void Login();
  void QryInstrument();
  void QryPosition();
  void QryTradingAccount();

  long GetMaxOrderRef();
  const char* GetHSExchangeID(const char* inst);
  inline char FromHSOffsetFlag(HSOffsetFlag offset);
  inline char FromHSDirection(HSDirection direction);
  inline char FromHSOrderStatus(HSOrderStatus order_status);
  inline int FromHSOrderRef(char* local_id, HSRef order_ref, int size);
  inline int FromHSTime(char* trade_time, int size, HSTime hhmmss);

  void EfivInsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void EfivCancelOrder(char* inst, char* exchange_id, char* local_id, char* org_local_id, char* order_sys_id);

private:
  void* m_Arg;
  CHSExtendTradeApi* m_TraderApi;
  int m_TraderRequestID;
  long m_MaxOrderRef;
  std::map<long, void*> mapOrder;
  std::map<std::string, std::string> mapHSExchangeID;
};

#endif // _NANO_TRADER_HANDLER_H_
