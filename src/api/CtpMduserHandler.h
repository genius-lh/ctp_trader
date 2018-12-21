#ifndef _CTP_MDUSER_HANDLER_H_
#define _CTP_MDUSER_HANDLER_H_

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ctp_mduser_api_cb_def ctp_mduser_api_cb;

struct ctp_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUserLogout)(void* arg, CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRtnDepthMarketData)(void* arg, CThostFtdcDepthMarketDataField *pMarketData);
  void (*xOnRspError)(void* arg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspSubMarketData)(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
  void (*xOnRspUnSubMarketData)(void* arg, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};
#ifdef __cplusplus
}
#endif

class CCtpMduserHandler : public CThostFtdcMdSpi
{
  public:
    
  CCtpMduserHandler(ctp_mduser_api_cb *pUserCb, void * parent);

public:
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  void OnFrontConnected();
  
  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  void OnFrontDisconnected(int nReason);
    
  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  virtual void OnHeartBeatWarning(int nTimeLapse){};
  

  ///登录请求响应
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登出请求响应
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///错误应答
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///订阅行情应答
  void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///取消订阅行情应答
  void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///订阅询价应答
  virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

  ///取消订阅询价应答
  virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

  ///深度行情通知
  void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

  ///询价通知
  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

  private:
    void* m_parent;
    ctp_mduser_api_cb* m_pUserCb;
    
};

#endif //_CTP_MDUSER_HANDLER_H_
