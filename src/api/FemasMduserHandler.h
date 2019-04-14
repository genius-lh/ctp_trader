#ifndef _FEMAS_MDUSER_HANDLER_H_
#define _FEMAS_MDUSER_HANDLER_H_

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct femas_mduser_api_cb_def femas_mduser_api_cb;

struct femas_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUserLogout)(void* arg, CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnDepthMarketData)(void* arg, CUstpFtdcDepthMarketDataField *pMarketData);
  void (*xOnRspError)(void* arg, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspSubMarketData)(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUnSubMarketData)(void* arg, CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
};
#ifdef __cplusplus
}
#endif

class CFemasMduserHandler : public CUstpFtdcMduserSpi
{
public:
    // 构造函数，需要一个有效的指向CUstpFtdcMduserApi实例的指针
    CFemasMduserHandler(femas_mduser_api_cb *pUserCb, void * parent);
    ~CFemasMduserHandler();
    // 当客户端与行情发布服务器建立起通信连接，客户端需要进行登录
    void OnFrontConnected();

    // 当客户端与行情发布服务器通信连接断开时，该方法被调用
    void OnFrontDisconnected(int nReason);

	virtual void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    // 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户退出应答
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// 深度行情通知，行情服务器会主动通知客户端
    void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData);
  
	// 针对用户请求的出错通知
    void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅合约的相关信息
	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///退订合约的相关信息
	void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
  femas_mduser_api_cb *m_pUserCb;
  void * m_parent;

};
#endif

