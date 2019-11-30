#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#include "XSpeedTraderHandler.h"

CXSpeedTraderHandler::CXSpeedTraderHandler(xspeed_trader_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CXSpeedTraderHandler::~CXSpeedTraderHandler() 
{
}

/**
 * SEC-网络连接正常响应
 */
void CXSpeedTraderHandler::OnFrontConnected() {
m_pUserCb->xOnFrontConnected(m_parent);
}
/**
 * SEC-网络连接不正常响应
 */
void CXSpeedTraderHandler::OnFrontDisconnected(int nReason) {
m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}
/**
 * SEC-消息通知
 */
void CXSpeedTraderHandler::OnRtnNotice(DFITCSECRspNoticeField *pNotice) {
m_pUserCb->xOnRtnNotice(m_parent, pNotice);
}
/**
* ERR-错误应答
* @param pRspInfo:指针若非空，返回错误信息结构地址
*/
void CXSpeedTraderHandler::OnRspError(DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspError(m_parent, pRspInfo);
}
/**
* STOCK-登录响应
* @param pData:指针若非空,返回用户登录响应信息结构体的地址,表明登录请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登录请求失败
*/
void CXSpeedTraderHandler::OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserLogin(m_parent, pData, pRspInfo);
}
/**
* STOCK-登出响应
* @param pData:指针若非空,返回用户登出响应信息结构体的地址,表明登出请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登出请求失败
*/
void CXSpeedTraderHandler::OnRspStockUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserLogout(m_parent, pData, pRspInfo);
}
/**
* STOCK-密码更新响应
* @param pData:指针若非空,返回用户密码更新响应信息结构体的地址,表明密码更新请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明密码更新请求失败
*/
void CXSpeedTraderHandler::OnRspStockUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserPasswordUpdate(m_parent, pData, pRspInfo);
}
/**
* STOCK-委托报单响应
* @param pData:指针若非空,返回用户委托报单响应信息结构体的地址,表明报单请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托报单请求失败
*/
void CXSpeedTraderHandler::OnRspStockEntrustOrder(DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-委托撤单响应
* @param pData:指针若非空,返回用户委托撤单响应信息结构体的地址,表明撤单请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托撤单请求失败
*/
void CXSpeedTraderHandler::OnRspStockWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-委托查询响应
* @param pData:指针若非空,返回用户委托查询响应信息结构体的地址,表明查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryEntrustOrder(DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-实时成交查询响应
* @param pData:指针若非空,返回用户实时成交查询响应信息结构体的地址,表明实时成交查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明实时成交查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryRealTimeTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-分笔成交查询响应
* @param pData:指针若非空,返回用户分笔成交查询响应信息结构体的地址,表明分笔成交查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明分笔成交查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQrySerialTrade(DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-持仓查询响应
* @param pData:指针若非空,返回用户持仓查询响应信息结构体的地址,表明持仓查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明持仓查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-资金账号查询响应
* @param pData:指针若非空,返回用户资金账号查询响应信息结构体的地址,表明资金账号查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金账号查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryCapitalAccountInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-交易账号查询响应
* @param pData:指针若非空,返回用户交易账号查询响应信息结构体的地址,表明交易账号查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易账号查询请求失败
*/
void CXSpeedTraderHandler::OnRspStockQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* STOCK-股东号查询响应
* @param pData:指针若非空,返回用户股东号查询响应信息结构体的地址,表明股东号查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明股东号查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryShareholderInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-资金调拨响应
* @param pData:指针若非空,返回用户资金调拨响应信息结构体的地址,表明股资金调拨请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金调拨请求失败
*/
void CXSpeedTraderHandler::OnRspStockTransferFunds(DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockTransferFunds(m_parent, pData, pRspInfo);
}
/**
* STOCK-批量委托响应
* @param pData:指针若非空,返回用户批量委托响应信息结构体的地址,表明批量委托请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量委托请求失败
*/
void CXSpeedTraderHandler::OnRspStockEntrustBatchOrder(DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockEntrustBatchOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-批量撤单响应
* @param pData:指针若非空,返回用户批量撤单响应信息结构体的地址,表明批量撤单请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量撤单请求失败
*/
void CXSpeedTraderHandler::OnRspStockWithdrawBatchOrder(DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockWithdrawBatchOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-计算可委托数量响应
* @param pData:指针若非空,返回用户计算可委托数量响应信息结构体的地址,表明计算可委托数量请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算可委托数量请求失败
*/
void CXSpeedTraderHandler::OnRspStockCalcAbleEntrustQty(DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockCalcAbleEntrustQty(m_parent, pData, pRspInfo);
}
/**
* STOCK-计算申购ETF数量响应
* @param pData:指针若非空,返回用户计算申购ETF数量响应信息结构体的地址,表明计算申购ETF数量请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算申购ETF数量请求失败
*/
void CXSpeedTraderHandler::OnRspStockCalcAblePurchaseETFQty(DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockCalcAblePurchaseETFQty(m_parent, pData, pRspInfo);
}
/**
* STOCK-冻结资金明细查询响应
* @param pData:指针若非空,返回用户冻结资金明细查询响应信息结构体的地址,表明冻结资金明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结资金明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryFreezeFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-冻结证券明细查询响应
* @param pData:指针若非空,返回用户冻结证券明细查询响应信息结构体的地址,表明冻结证券明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结证券明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryFreezeStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-调拨证券明细查询响应
* @param pData:指针若非空,返回用户调拨证券明细查询响应信息结构体的地址,表明调拨证券明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨证券明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryTransferStockDetail(DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryTransferStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-调拨资金明细查询响应
* @param pData:指针若非空,返回用户调拨资金明细查询响应信息结构体的地址,表明调拨资金明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨资金明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryTransferFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-证券信息查询响应
* @param pData:指针若非空,返回用户证券信息查询响应信息结构体的地址,表明证券信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryStockInfo(DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryStockInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-证券静态信息查询响应
* @param pData:指针若非空,返回用户证券静态信息查询响应信息结构体的地址,表明证券静态信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券静态信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspStockQryStockStaticInfo(DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryStockStaticInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-交易时间查询响应
* @param pData:指针若非空,返回用户交易时间查询响应信息结构体的地址,表明交易时间查询请求成功
* @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易时间查询请求失败
*/
void CXSpeedTraderHandler::OnRspStockQryTradeTime(DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockQryTradeTime(m_parent, pData, pRspInfo);
}
/**
* STOCK-委托回报响应
* @param pData:返回委托回报结构体的地址
*/
void CXSpeedTraderHandler::OnStockEntrustOrderRtn(DFITCStockEntrustOrderRtnField * pData){
m_pUserCb->xOnStockEntrustOrderRtn(m_parent, pData);
}
/**
* STOCK-成交回报响应
* @param pData:返回成交回报结构体的地址
*/
void CXSpeedTraderHandler::OnStockTradeRtn(DFITCStockTradeRtnField * pData){
m_pUserCb->xOnStockTradeRtn(m_parent, pData);
}
/**
* STOCK-撤单回报响应
* @param pData:返回撤单回报结构体的地址
*/
void CXSpeedTraderHandler::OnStockWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField * pData){
m_pUserCb->xOnStockWithdrawOrderRtn(m_parent, pData);
}

/**
* SOP-登录响应
* @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
*/
void CXSpeedTraderHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserLogin(m_parent, pData, pRspInfo);
}
/**
 * SOP-登出响应
 * @param pData:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
 * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
 */
void CXSpeedTraderHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserLogout(m_parent, pData, pRspInfo);
}
/**
* SOP-用户口令更新响应
* @param pData:指针若非空,返回用户口令更新响应信息结构地址,表明用户口令更新请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明用户口令更新请求失败
*/
void CXSpeedTraderHandler::OnRspSOPUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserPasswordUpdate(m_parent, pData, pRspInfo);
}
/**
* SOP-报单响应
* @param pData:指针若非空,返回用户报单响应信息结构地址,表明报单请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明报单请求失败
*/
void CXSpeedTraderHandler::OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-组合拆分委托响应
* @param pData:指针若非空,返回用户响应信息结构地址,表明组合拆分委托请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明组合拆分委托请求失败
*/
void CXSpeedTraderHandler::OnRspSOPGroupSplit(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPGroupSplit(m_parent, pData, pRspInfo);
}
/**
* SOP-查询客户组合持仓明细响应
* @param pData:指针若非空,返回用户查询客户组合持仓明细响应结构地址,表明查询客户组合持仓明细请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明查询客户组合持仓明细请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryGroupPosition(DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryGroupPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-证券锁定解锁响应
* @param pData:指针若非空,返回用户证券锁定解锁响应信息结构地址,表明证券锁定解锁请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明证券锁定解锁请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPLockOUnLockStock(DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPLockOUnLockStock(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-撤单响应
* @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
*/
void CXSpeedTraderHandler::OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-委托查询响应
* @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明委托查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明委托查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-分笔成交查询响应
* @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明分笔成交查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明分笔成交查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQrySerialTrade(DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-持仓查询响应
* @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明持仓查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明持仓查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-客户担保持仓查询响应
* @param pData:指针若非空,返回用户客户担保持仓查询响应信息结构地址,表明客户担保持仓查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保持仓查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryCollateralPosition(DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryCollateralPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-客户资金查询响应
* @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
*/
void CXSpeedTraderHandler::OnRspSOPQryCapitalAccountInfo(DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryCapitalAccountInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-客户信息查询响应
* @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
*/
void CXSpeedTraderHandler::OnRspSOPQryAccountInfo(DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-股东信息查询响应
* @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
*/
void CXSpeedTraderHandler::OnRspSOPQryShareholderInfo(DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryShareholderInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-可委托数量查询响应
* @param pData:指针若非空,返回用户可委托数量查询响应信息结构地址,表明客户可委托数量查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户可委托数量查询请求失败
*/
void CXSpeedTraderHandler::OnRspSOPCalcAbleEntrustQty(DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPCalcAbleEntrustQty(m_parent, pData, pRspInfo);
}
/**
* SOP-客户可锁定证券查询响应
* @param pData:指针若非空,返回用户客户可锁定证券查询响应信息结构地址,表明客户客户可锁定证券查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可锁定证券查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryAbleLockStock(DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryAbleLockStock(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-期权合约代码查询响应
* @param pData:指针若非空,返回用户期权合约代码查询响应信息结构地址,表明客户期权合约代码查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权合约代码查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryContactInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-执行委托响应
* @param pData:指针若非空,返回用户执行委托响应信息结构地址,表明客户执行委托请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户执行委托请求失败
*/
void CXSpeedTraderHandler::OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPExectueOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-客户行权指派信息查询响应
* @param pData:指针若非空,返回用户客户行权指派信息查询响应信息结构地址,表明客户客户行权指派信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户行权指派信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryExecAssiInfo(DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryExecAssiInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-查询交易时间响应
* @param pData:指针若非空,返回用户查询交易时间响应信息结构地址,表明客户查询交易时间请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询交易时间请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryTradeTime(DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryTradeTime(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-获取所有交易所参数响应
* @param pData:指针若非空,返回用户获取所有交易所参数响应信息结构地址,表明客户获取所有交易所参数请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户获取所有交易所参数请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryExchangeInfo(DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryExchangeInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-查询手续费参数响应
* @param pData:指针若非空,返回用户查询手续费参数响应信息结构地址,表明客户查询手续费参数请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询手续费参数请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryCommission(DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryCommission(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-查询保证金率参数响应
* @param pData:指针若非空,返回用户查询保证金率参数响应信息结构地址,表明客户查询保证金率参数请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询保证金率参数请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryDeposit(DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryDeposit(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-期权标的信息查询响应
* @param pData:指针若非空,返回用户期权标的信息查询响应信息结构地址,表明客户期权标的信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权标的信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspSOPQryContractObjectInfo(DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryContractObjectInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-委托回报响应
* @param pData:返回委托回报结构体的地址
*/
void CXSpeedTraderHandler::OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData){
m_pUserCb->xOnSOPEntrustOrderRtn(m_parent, pData);
}
/**
* SOP-成交回报响应
* @param pData:返回成交回报结构体的地址
*/
void CXSpeedTraderHandler::OnSOPTradeRtn(DFITCSOPTradeRtnField * pData){
m_pUserCb->xOnSOPTradeRtn(m_parent, pData);
}
/**
* SOP-撤单回报响应
* @param pData:返回撤单回报结构体的地址
*/
void CXSpeedTraderHandler::OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData){
m_pUserCb->xOnSOPWithdrawOrderRtn(m_parent, pData);
}

/**
* FASL-登录响应
* @param pData:指针若非空,返回用户融资融券登录响应信息结构地址,表明客户融资融券登录请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登录请求失败
*/
void CXSpeedTraderHandler::OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLUserLogin(m_parent, pData, pRspInfo);
}
/**
* FASL-登出响应
* @param pData:指针若非空,返回用户融资融券登出响应信息结构地址,表明客户融资融券登出请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登出请求失败
*/
void CXSpeedTraderHandler::OnRspFASLUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLUserLogout(m_parent, pData, pRspInfo);
}
/**
* FASL-客户可融资信息响应
* @param pData:指针若非空,返回用户客户可融资信息响应信息结构地址,表明客户客户可融资信息请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融资信息请求失败
*/
void CXSpeedTraderHandler::OnRspFASLQryAbleFinInfo(DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryAbleFinInfo(m_parent, pData, pRspInfo);
}
/**
* FASL-客户可融券信息响应
* @param pData:指针若非空,返回用户客户可融券信息响应信息结构地址,表明客户客户可融券信息请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融券信息请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryAbleSloInfo(DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryAbleSloInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-担保物划转响应
* @param pData:指针若非空,返回用户担保物划转响应信息结构地址,表明客户担保物划转请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保物划转请求失败
*/
void CXSpeedTraderHandler::OnRspFASLTransferCollateral(DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLTransferCollateral(m_parent, pData, pRspInfo);
}
/**
* FASL-直接还款响应
* @param pData:指针若非空,返回用户直接还款响应信息结构地址,表明客户直接还款请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户直接还款请求失败
*/
void CXSpeedTraderHandler::OnRspFASLDirectRepayment(DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLDirectRepayment(m_parent, pData, pRspInfo);
}
/**
* FASL-还券划转响应
* @param pData:指针若非空,返回用户还券划转响应信息结构地址,表明客户还券划转请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户还券划转请求失败
*/
void CXSpeedTraderHandler::OnRspFASLRepayStockTransfer(DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLRepayStockTransfer(m_parent, pData, pRspInfo);
}
/**
* FASL-信用交易响应
* @param pData:指针若非空,返回用户信用交易响应信息结构地址,表明客户信用交易请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用交易请求失败
*/
void CXSpeedTraderHandler::OnRspFASLEntrustCrdtOrder(DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLEntrustCrdtOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-融资融券交易响应
* @param pData:指针若非空,返回用户融资融券交易响应信息结构地址,表明客户融资融券交易请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券交易请求失败
*/
void CXSpeedTraderHandler::OnRspFASLEntrustOrder(DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-信用可委托数量查询响应
* @param pData:指针若非空,返回用户信用可委托数量查询响应信息结构地址,表明客户信用可委托数量查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用可委托数量查询请求失败
*/
void CXSpeedTraderHandler::OnRspFASLCalcAbleEntrustCrdtQty(DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLCalcAbleEntrustCrdtQty(m_parent, pData, pRspInfo);
}
/**
* FASL-查询信用资金响应
* @param pData:指针若非空,返回用户查询信用资金响应信息结构地址,表明客户查询信用资金请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询信用资金请求失败
*/
void CXSpeedTraderHandler::OnRspFASLQryCrdtFunds(DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryCrdtFunds(m_parent, pData, pRspInfo);
}
/**
* FASL-信用合约信息响应
* @param pData:指针若非空,返回用户信用合约信息响应信息结构地址,表明客户信用合约信息请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约信息请求失败
*/
void CXSpeedTraderHandler::OnRspFASLQryCrdtContract(DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCrdtContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-信用合约变动信息查询响应
* @param pData:指针若非空,返回用户信用合约变动信息查询响应信息结构地址,表明客户信用合约变动信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约变动信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryCrdtConChangeInfo(DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCrdtConChangeInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-资金调转响应
* @param pData:指针若非空,返回用户资金调转响应信息结构地址,表明客户资金调转请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调转请求失败
*/
void CXSpeedTraderHandler::OnRspFASLTransferFunds(DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLTransferFunds(m_parent, pData, pRspInfo);
}
/**
* FASL-客户信息查询响应
* @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
*/
void CXSpeedTraderHandler::OnRspFASLQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* FASL-客户资金查询响应
* @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCapitalAccountInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-股东信息查询响应
* @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryShareholderInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-持仓查询响应
* @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明客户持仓查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户持仓查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-委托查询响应
* @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明客户委托查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户委托查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryEntrustOrder(DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-分笔成交查询响应
* @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明客户分笔成交查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户分笔成交查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQrySerialTrade(DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-实时成交查询响应
* @param pData:指针若非空,返回用户实时成交查询响应信息结构地址,表明客户实时成交查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户实时成交查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryRealTimeTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-资金冻结明细查询响应
* @param pData:指针若非空,返回用户资金冻结明细查询响应信息结构地址,表明客户资金冻结明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金冻结明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryFreezeFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-证券冻结明细查询响应
* @param pData:指针若非空,返回用户证券冻结明细查询响应信息结构地址,表明客户证券冻结明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户证券冻结明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryFreezeStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-资金调拨明细查询响应
* @param pData:指针若非空,返回用户资金调拨明细查询响应信息结构地址,表明客户资金调拨明细查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调拨明细查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryTransferFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-撤单响应
* @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
*/
void CXSpeedTraderHandler::OnRspFASLWithdrawOrder(DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-当前系统时间查询请求响应
* @param pData:指针若非空,返回用户系统时间查询响应信息结构地址,表明系统时间查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明系统时间查询请求失败
*/
void CXSpeedTraderHandler::OnRspFASLQrySystemTime(DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQrySystemTime(m_parent, pData, pRspInfo);
}
/**
* FASL-可转入担保证券查询请求响应
* @param pData:指针若非空,返回可转入担保证券查询响应信息结构地址,表明可转入担保证券查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明可转入担保证券查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryTransferredContract(DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryTransferredContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-客户可取资金调出请求响应
* @param pData:指针若非空,返回客户可取资金调出响应信息结构地址,表明客户可取资金调出请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明客户可取资金调出请求失败
*/
void CXSpeedTraderHandler::OnRspFASLDesirableFundsOut(DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLDesirableFundsOut(m_parent, pData, pRspInfo);
}
/**
* FASL-担保证券查询请求响应
* @param pData:指针若非空,返回担保证券查询响应信息结构地址,表明担保证券查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明担保证券查询请求失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryGuaranteedContract(DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryGuaranteedContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-标的证券查询请求响应
* @param pData:指针若非空,返回标的证券查询响应信息结构地址,表明标的证券查询请求成功
* @param pRspInfo:指针若非空，返回错误信息地址，表明标的证券查询失败
* @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
*/
void CXSpeedTraderHandler::OnRspFASLQryUnderlyingContract(DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryUnderlyingContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-委托回报响应
* @param pData:返回委托回报结构体的地址
*/
void CXSpeedTraderHandler::OnFASLEntrustOrderRtn(DFITCStockEntrustOrderRtnField *pData){
m_pUserCb->xOnFASLEntrustOrderRtn(m_parent, pData);
}
/**
* FASL-成交回报响应
* @param pData:返回成交回报结构体的地址
*/
void CXSpeedTraderHandler::OnFASLTradeRtn(DFITCStockTradeRtnField *pData){
m_pUserCb->xOnFASLTradeRtn(m_parent, pData);
}
/**
* FASL-撤单回报响应
* @param pData:返回撤单回报结构体的地址
*/
void CXSpeedTraderHandler::OnFASLWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField *pData){
m_pUserCb->xOnFASLWithdrawOrderRtn(m_parent, pData);
}


