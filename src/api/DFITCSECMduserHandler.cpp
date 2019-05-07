
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#include "DFITCSECMduserHandler.h"

CDfitcSecMduserHandler::CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CDfitcSecMduserHandler::~CDfitcSecMduserHandler() 
{
}

/** 
* 网络连接正常响应
*/
void CDfitcSecMduserHandler::OnFrontConnected(){
m_pUserCb->xOnFrontConnected(m_parent);
}

/**
* 网络连接不正常响应
*/
void CDfitcSecMduserHandler::OnFrontDisconnected(int nReason){
m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

/**
* SEC-消息通知
*/
void CDfitcSecMduserHandler::OnRtnNotice(DFITCSECRspNoticeField *pNotice) {
m_pUserCb->xOnRtnNotice(m_parent, pNotice);
}

/**
* ERR-错误应答
* @param pRspInfo:若请求失败，返回错误信息地址
*/
void CDfitcSecMduserHandler::OnRspError(DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspError(m_parent, pRspInfo);
}

/**
* STOCK-登录响应
* @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
* @param pRspInfo::指针若非空，返回错误信息地址，表明登录请求失败
*/
void CDfitcSecMduserHandler::OnRspStockUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* STOCK-登出响应
* @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
*/
void CDfitcSecMduserHandler::OnRspStockUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* SOP-登录响应
* @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
*/
void CDfitcSecMduserHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* SOP-登出响应
* @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
*/
void CDfitcSecMduserHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* FASL-登录响应
* @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
*/
void CDfitcSecMduserHandler::OnRspFASLUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspFASLUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

/**
* FASL-登出响应
* @param pRspUsrLogout:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
*/
void CDfitcSecMduserHandler::OnRspFASLUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspFASLUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

/**
* STOCK-行情订阅响应
* @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
*/
void CDfitcSecMduserHandler::OnRspStockSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* STOCK-取消订阅行情响应
* @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
*/
void CDfitcSecMduserHandler::OnRspStockUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspStockUnSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* SOP-行情订阅响应
* @param pSpecificInstrument:指针若非空,返回用户指定合约行情订阅响应结构地址,表明指定合约行情订阅成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情订阅失败
*/
void CDfitcSecMduserHandler::OnRspSOPSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* SOP-取消订阅行情响应
* @param pSpecificInstrument:指针若非空,返回用户指定合约行情取消订阅响应结构地址,表明指定合约行情取消订阅成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
*/
void CDfitcSecMduserHandler::OnRspSOPUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {
m_pUserCb->xOnRspSOPUnSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

/**
* STOCK-行情推送响应
* @param pMarketDataField:指针若非空,返回行情推送响应结构地址
*/
void CDfitcSecMduserHandler::OnStockMarketData(DFITCStockDepthMarketDataField * pMarketDataField) {
m_pUserCb->xOnStockMarketData(m_parent, pMarketDataField);
}

/**
* SOP-行情推送响应
* @param pMarketDataField:指针若非空,返回行情推送响应结构地址
*/
void CDfitcSecMduserHandler::OnSOPMarketData(DFITCSOPDepthMarketDataField * pMarketDataField) {
m_pUserCb->xOnSOPMarketData(m_parent, pMarketDataField);
}

/**
* STOCK-可用行情响应
* @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
* @param flag  为真是标示最后一条，为假时表示还有后续。
*/
void CDfitcSecMduserHandler::OnRspStockAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, bool flag) {
m_pUserCb->xOnRspStockAvailableQuot(m_parent, pAvailableQuotInfo, pRspInfo, flag);
}

/**
* SOP-可用行情响应
* @param pAvailableQuotInfo:指针若非空,返回可用的行情信息
* @param pRspInfo:指针若非空，返回错误信息地址，表明指定合约行情取消订阅失败
* @param flag  为真是标示最后一条，为假时表示还有后续。
*/
void CDfitcSecMduserHandler::OnRspSopAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo, bool flag) {
m_pUserCb->xOnRspSopAvailableQuot(m_parent, pAvailableQuotInfo, pRspInfo, flag);
}

/**
* 密码更新请求响应
* @param pMDPasswordUpdate:指针若非空,返回用户行情密码响应信息结构地址,表明密码修改成功。
* @param pRspInfo:指针若非空，返回错误信息地址，表明密码修改失败。
*/
void CDfitcSecMduserHandler::OnRspUserMDPasswordUpdate(DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo){
m_pUserCb->xOnRspUserMDPasswordUpdate(m_parent, pMDPasswordUpdate, pRspInfo);
}

