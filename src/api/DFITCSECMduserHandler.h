#ifndef _DFITC_SEC_MDUSER_HANDLER_H_
#define _DFITC_SEC_MDUSER_HANDLER_H_
#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dfitc_sec_mduser_api_cb_def dfitc_sec_mduser_api_cb;

struct dfitc_sec_mduser_api_cb_def{
  /** 
   * 网络连接正常响应
   */
 void (*xOnFrontConnected)(void* arg);

  /**
   * 网络连接不正常响应
   */
 void (*xOnFrontDisconnected)(void* arg, int nReason);

  /**
   * SEC-消息通知
   */
 void (*xOnRtnNotice)(void* arg, DFITCSECRspNoticeField *pNotice);

  /**
   * ERR-错误应答
   * @param pRspInfo:若请求失败，返回错误信息地址
   */
 void (*xOnRspError)(void* arg, DFITCSECRspInfoField *pRspInfo);

  /**
   * STOCK-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo::指针若非空，返回错误信息地址，表明登录请求失败
   */
 void (*xOnRspStockUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void (*xOnRspStockUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
   */
 void (*xOnRspSOPUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void (*xOnRspSOPUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
   */
 void (*xOnRspFASLUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void (*xOnRspFASLUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
 
  /**
   * STOCK-行情订阅响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
   */
 void (*xOnRspStockSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-取消订阅行情响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   */
 void (*xOnRspStockUnSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
  
  /**
   * SOP-行情订阅响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
   */
 void (*xOnRspSOPSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-取消订阅行情响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   */
 void (*xOnRspSOPUnSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-行情推送响应
   * @param pMarketDataField:指针若非空,返回行情推送响应结构地址
   */
 void (*xOnStockMarketData)(void* arg, DFITCStockDepthMarketDataField * pMarketDataField);

  /**
   * SOP-行情推送响应
   * @param pMarketDataField:指针若非空,返回行情推送响应结构地址
   */
 void (*xOnSOPMarketData)(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField);

  /**
   * STOCK-可用行情响应
   * @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   * @param flag  为真是标示最后一条，为假时表示还有后续。
   */
 void (*xOnRspStockAvailableQuot)(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);

  /**
   * SOP-可用行情响应
   * @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   * @param flag  为真是标示最后一条，为假时表示还有后续。
   */
 void (*xOnRspSopAvailableQuot)(void* arg, DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, int flag);

  /**
   * 密码更新请求响应
   * @param pMDPasswordUpdate:指针若非空,返回用户行情密码响应信息结构地址,表明密码修改成功。
   * @param pRspInfo:指针若非空，返回错误信息地址，表明密码修改失败。
   */
 void (*xOnRspUserMDPasswordUpdate)(void* arg, DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);
};
#ifdef __cplusplus
}
#endif

class CDfitcSecMduserHandler : public DFITCSECMdSpi
{
public:
  /** 
   * 网络连接正常响应
   */
 void OnFrontConnected();

  /**
   * 网络连接不正常响应
   */
 void OnFrontDisconnected(int nReason);

  /**
   * SEC-消息通知
   */
 void OnRtnNotice(DFITCSECRspNoticeField *pNotice);

  /**
   * ERR-错误应答
   * @param pRspInfo:若请求失败，返回错误信息地址
   */
 void OnRspError(DFITCSECRspInfoField *pRspInfo);

  /**
   * STOCK-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo::指针若非空，返回错误信息地址，表明登录请求失败
   */
 void OnRspStockUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void OnRspStockUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
   */
 void OnRspSOPUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void OnRspSOPUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-登录响应
   * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
   */
 void OnRspFASLUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
   * FASL-登出响应
   * @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
 void OnRspFASLUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
 
  /**
   * STOCK-行情订阅响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
   */
 void OnRspStockSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-取消订阅行情响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   */
 void OnRspStockUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
  
  /**
   * SOP-行情订阅响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
   */
 void OnRspSOPSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * SOP-取消订阅行情响应
   * @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   */
 void OnRspSOPUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
   * STOCK-行情推送响应
   * @param pMarketDataField:指针若非空,返回行情推送响应结构地址
   */
 void OnStockMarketData(DFITCStockDepthMarketDataField * pMarketDataField);

  /**
   * SOP-行情推送响应
   * @param pMarketDataField:指针若非空,返回行情推送响应结构地址
   */
 void OnSOPMarketData(DFITCSOPDepthMarketDataField * pMarketDataField);

  /**
   * STOCK-可用行情响应
   * @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   * @param flag  为真是标示最后一条，为假时表示还有后续。
   */
 void OnRspStockAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag);

  /**
   * SOP-可用行情响应
   * @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
   * @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
   * @param flag  为真是标示最后一条，为假时表示还有后续。
   */
 void OnRspSopAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag);

  /**
   * 密码更新请求响应
   * @param pMDPasswordUpdate:指针若非空,返回用户行情密码响应信息结构地址,表明密码修改成功。
   * @param pRspInfo:指针若非空，返回错误信息地址，表明密码修改失败。
   */
 void OnRspUserMDPasswordUpdate(DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo);
  
public:
  CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb *pUserCb, void * parent);
  ~CDfitcSecMduserHandler();

private:
  dfitc_sec_mduser_api_cb *m_pUserCb;
  void * m_parent;
};

#endif // _DFITC_SEC_MDUSER_HANDLER_H_

