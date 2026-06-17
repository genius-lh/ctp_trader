#ifndef QDInfoSimpleSpi_h
#define QDInfoSimpleSpi_h

#include "QCValueAddProApi.h"
#include <string.h>
#include <stdio.h>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <typeinfo>
using namespace std;

using namespace QCVALUEADDPROAPI;

extern int g_port;
extern char g_address[32];
extern char g_userid[128];
extern char g_passwd[128];
extern char g_filename[128];

struct qrycmd
{
	qrycmd()
	{
		CmdID = "";
		CmdData = NULL;
	}
	qrycmd(const char* cmdid, void* cmddata)
	{
		CmdID = cmdid;
		CmdData = cmddata;
	}
	void Release()
	{
		CmdID = "";
		if (CmdData)
		{
			//delete CmdData;
			CmdData = NULL;
		}
	}
	std::string CmdID;
	void* CmdData;
};

class qrycmdmgr
{
private:
	queue<qrycmd> m_qrycmd_queue;
	mutex m_mutex;
	static qrycmdmgr* m_instance;
private:
	qrycmdmgr(const qrycmdmgr &obj);
	qrycmdmgr& operator=(const qrycmdmgr &obj);

	qrycmdmgr(){};
public:
	static  qrycmdmgr* GetInstance()
	{
		if (NULL == m_instance)
		{
			m_instance = new qrycmdmgr();
		}
		return m_instance;
	}

	void AddQry(qrycmd qry)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_qrycmd_queue.push(qry);
	}

	bool PopQry(qrycmd& qry)
	{
		bool ret = false;
		std::lock_guard<std::mutex> guard(m_mutex);
		if (!m_qrycmd_queue.empty())
		{
			ret = true;
			qry = m_qrycmd_queue.front();
			m_qrycmd_queue.pop();
		}
		return ret;
	}

	void ClearAllQrys()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		while (!m_qrycmd_queue.empty())
		{
			m_qrycmd_queue.pop();
		}
	}
};

class CQCVDSimpleSpi : public CQCValueAddProSpi
{
	CQCValueAddProApi* m_api;
private:
	void SubscribeMDs();
	void ReqQryValueAddDatas();
	void ReqQryReportInfos();
	void ReqAddminOperations();
public:
	int m_requestid;
	atomic_bool m_is_qrying;
	atomic_bool m_is_login;
	atomic_bool m_is_connected;

	CQCVDSimpleSpi(CQCValueAddProApi* t_api);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	virtual void OnFrontDisconnected(int nReason);

	//登录应答	
	virtual void OnRspUserLogin(CQCVDRspUserLoginField *pRspUserLoginField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//登出应答	
	virtual void OnRspUserLogout(CQCVDUserLogoutField *pUserLogoutField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	//查询集中交易系统资金响应	
	virtual void OnRspInquiryJZFund(CQCVDRspInquiryJZFundField *pRspInquiryJZFundField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询银行账户余额响应	
	virtual void OnRspInquiryBankAccountFund(CQCVDRspInquiryBankAccountFundField *pRspInquiryBankAccountFundField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询新股信息
	virtual void OnRspQryIPOInfo(CQCVDIPOInfoField *pIPOInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询投资者
	virtual void OnRspQryInvestor(CQCVDInvestorField *pInvestor, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询股东账户
	virtual void OnRspQryShareholderAccount(CQCVDShareholderAccountField *pShareholderAccount, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//报单查询
	virtual void OnRspQryOrder(CQCVDOrderField *pOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询撤单
	virtual void OnRspQryOrderAction(CQCVDOrderActionField *pOrderAction, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//成交查询
	virtual void OnRspQryTrade(CQCVDTradeField *pTrade, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//资金账户查询
	virtual void OnRspQryTradingAccount(CQCVDTradingAccountField *pTradingAccount, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//投资者持仓查询
	virtual void OnRspQryPosition(CQCVDPositionField *pPosition, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询新股申购额度
	virtual void OnRspQryIPOQuota(CQCVDIPOQuotaField *pIPOQuota, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询资金转移流水
	virtual void OnRspQryFundTransferDetail(CQCVDFundTransferDetailField *pFundTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询持仓转移流水
	virtual void OnRspQryPositionTransferDetail(CQCVDPositionTransferDetailField *pPositionTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询未到期债券质押回购委托响应
	virtual void OnRspQryPrematurityRepoOrder(CQCVDPrematurityRepoOrderField *pPrematurityRepoOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询外围系统仓位调拨流水
	virtual void OnRspQryPeripheryPositionTransferDetail(CQCVDPeripheryPositionTransferDetailField *pPeripheryPositionTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询条件单响应
	virtual void OnRspQryCondOrder(CQCVDCondOrderField *pCondOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询条件单撤单响应
	virtual void OnRspQryCondOrderAction(CQCVDCondOrderActionField *pCondOrderAction, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询新股申购配号结果响应
	virtual void OnRspQryIPONumberResult(CQCVDIPONumberResultField *pIPONumberResult, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询新股申购中签结果响应
	virtual void OnRspQryIPOMatchNumberResult(CQCVDIPOMatchNumberResultField *pIPOMatchNumberResult, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询交易协议响应
	virtual void OnRspQryShareholderSpecPrivilege(CQCVDShareholderSpecPrivilegeField *pShareholderSpecPrivilege, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询配股配债信息
	virtual void OnRspQryRationalInfo(CQCVDRationalInfoField *pRationalInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询外围系统资金调拨流水
	virtual void OnRspQryPeripheryFundTransferDetail(CQCVDPeripheryFundTransferDetailField *pPeripheryFundTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///查询自由流通股本数据应答
	virtual void OnRspInquiryFreeFloatShares(CQCVDFreeFloatSharesDataField *pFreeFloatSharesData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);

	///查询复权信息应答
	virtual void OnRspInquiryRightsAdjustment(CQCVDRightsAdjustmentDataField *pRightsAdjustmentData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);

	//查询历史资金流向数据应答
	virtual void OnRspInquiryHistoryFundsFlowData(CQCVDHistoryFundsFlowDataField *pHistoryFundsFlowData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询财务指标信息应答
	virtual void OnRspInquiryFinancialIndicatorData(CQCVDFinancialIndicatorDataField *pFinancialIndicatorData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询分红信息应答
	virtual void OnRspInquiryDividendData(CQCVDDividendDataField *pDividendData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询送股配股信息应答
	virtual void OnRspInquiryRightIssueData(CQCVDRightIssueDataField *pRightIssueData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询公司资料信息应答
	virtual void OnRspInquiryCompanyDescriptionData(CQCVDCompanyDescriptionDataField *pCompanyDescriptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询股本结构信息应答
	virtual void OnRspInquiryEquityStructureData(CQCVDEquityStructureDataField *pEquityStructureData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询主营业务信息应答
	virtual void OnRspInquirySalesSegmentData(CQCVDSalesSegmentDataField *pSalesSegmentData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询十大股东信息应答
	virtual void OnRspInquiryTopTenHoldersData(CQCVDTopTenHoldersDataField *pTopTenHoldersData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询十大流通股东信息应答
	virtual void OnRspInquiryTopTenFloatHoldersData(CQCVDTopTenFloatHoldersDataField *pTopTenFloatHoldersData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询个股所属行业板块信息应答
	virtual void OnRspInquiryIndustryData(CQCVDIndustryDataField *pIndustryData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询个股所属概念板块信息应答
	virtual void OnRspInquiryConceptionData(CQCVDConceptionDataField *pConceptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询个股所属地域板块信息应答
	virtual void OnRspInquiryRegionData(CQCVDRegionDataField *pRegionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询指数描述信息应答
	virtual void OnRspInquiryIndexDescriptionData(CQCVDIndexDescriptionDataField *pIndexDescriptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询行业板块成分股信息应答
	virtual void OnRspInquiryIndustryConstituentsData(CQCVDIndustryConstituentsDataField *pIndustryConstituentsData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询概念板块成分股信息应答
	virtual void OnRspInquiryConceptionConstituentsData(CQCVDConceptionConstituentsDataField *pConceptionConstituentsData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询板块指数实时行情信息应答
	virtual void OnRspInquiryIndustryCodeListData(CQCVDIndustryCodeListDataField *pIndustryCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询概念指数实时行情信息应答
	virtual void OnRspInquiryConceptionCodeListData(CQCVDConceptionCodeListDataField *pConceptionCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询地域版块代码列表应答
	virtual void OnRspInquiryRegionCodeListData(CQCVDRegionCodeListDataField *pRegionCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询申万指数行情应答
	virtual void OnRspInquirySWSIndexData(CQCVDSWSIndexDataField *pSWSIndexData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//股票日K行情请求包应答
	virtual void OnRspInquiryStockDayQuotation(CQCVDStockDayQuotationField *pStockDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//A股交易日历请求包应答
	virtual void OnRspInquiryShareCalendar(CQCVDShareCalendarField *pShareCalendar, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//申万行业分类请求包应答
	virtual void OnRspInquirySWIndustriesClass(CQCVDSWIndustriesClassField *pSWIndustriesClass, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//A股日行情估值指标请求包应答
	virtual void OnRspInquiryStockAssessIndicator(CQCVDStockAssessIndicatorField *pStockAssessIndicator, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//一致预测个股滚动指标请求包应答
	virtual void OnRspInquiryConsensusRollingData(CQCVDConsensusRollingDataField *pConsensusRollingData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//指数日行情请求包应答
	virtual void OnRspInquiryIndexDayQuotation(CQCVDIndexDayQuotationField *pIndexDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//A股基本资料请求包应答
	virtual void OnRspInquiryShareDescription(CQCVDShareDescriptionField *pShareDescription, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//十大股东信息请求包应答
	virtual void OnRspInquiryTopTenHoldersDetail(CQCVDTopTenHoldersDetailField *pTopTenHoldersDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//客户历史周期股票盈亏请求包应答
	virtual void OnRspInquiryHisShareProfit(CQCVDHisShareProfitField *pHisShareProfit, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//根据客户号查询客户资金账户请求包应答
	virtual void OnRspInquiryAccountIDByInvestorID(CQCVDAccountIDByInvestorIDField *pAccountIDByInvestorID, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//客户周期资金请求包应答
	virtual void OnRspInquiryCustPeriodCapiData(CQCVDCustPeriodCapiDataField *pCustPeriodCapiData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//客户周期盈亏请求包应答
	virtual void OnRspInquiryCustPeriodProfitData(CQCVDCustPeriodProfitDataField *pCustPeriodProfitData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//中国A股发行请求包应答
	virtual void OnRspInquiryShareIssuance(CQCVDShareIssuanceField *pShareIssuance, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//中国可转债发行请求包应答
	virtual void OnRspInquiryBondIssuance(CQCVDBondIssuanceField *pBondIssuance, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//中国A股股权质押信息请求包应答
	virtual void OnRspInquiryShareEquityPledgeInfo(CQCVDShareEquityPledgeInfoField *pShareEquityPledgeInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史资金请求包应答
	virtual void OnRspInquiryHistoryCapital(CQCVDHistoryCapitalField *pHistoryCapital, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史交割单请求包应答
	virtual void OnRspInquiryHistoryDelivery(CQCVDHistoryDeliveryField *pHistoryDelivery, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史资金流水请求包应答
	virtual void OnRspInquiryHistoryFundDetail(CQCVDHistoryFundDetailField *pHistoryFundDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史持仓请求包应答
	virtual void OnRspInquiryHistoryHold(CQCVDHistoryHoldField *pHistoryHold, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史委托数据请求包应答
	virtual void OnRspInquiryHistoryOrderEX(CQCVDHistoryOrderEXField *pHistoryOrderEX, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//历史成交请求包应答
	virtual void OnRspInquiryHistoryTradeEX(CQCVDHistoryTradeEXField *pHistoryTradeEX, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//录入标记事件应答
	virtual void OnRspInputRemarkEvent(CQCVDRspInputRemarkEventField *pRspInputRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//更新标记事件应答
	virtual void OnRspUpdateRemarkEvent(CQCVDRspUpdateRemarkEventField *pRspUpdateRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//删除标记事件应答
	virtual void OnRspDeleteRemarkEvent(CQCVDRspDeleteRemarkEventField *pRspDeleteRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//查询历史标记事件应答
	virtual void OnRspQryRemarkEvent(CQCVDRemarkEventField *pRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//查询持仓量价分布信息应答
	virtual void OnRspQryPriceDistributionData(CQCVDPriceDistributionDataField *pPriceDistributionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//查询历史极值价格信息应答
	virtual void OnRspQryPriceExtremumData(CQCVDPriceExtremumDataField *pPriceExtremumData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询盘口委托应答
	virtual void OnRspInquiryQueueingOrder(CQCVDQueueingOrderField *pQueueingOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//计算期权希腊值请求包应答
	virtual void OnRspQryOptionGreece(CQCVDRspOptionGreeceField *pRspOptionGreece, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//查询债券日K行情请求应答
	virtual void OnRspInquiryBondDayQuotation(CQCVDBondDayQuotationField *pBondDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	//查询港股通资金流向信息请求应答
	virtual void OnRspInquiryGGTEODPrices(CQCVDGGTEODPricesField *pGGTEODPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);
	///订阅特定行情应答
	virtual void OnRspSubSpecialMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅特定行情应答
	virtual void OnRspUnSubSpecialMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅资金流向行情应答
	virtual void OnRspSubFundsFlowMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅资金流向行情应答
	virtual void OnRspUnSubFundsFlowMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行业指数行情应答
	virtual void OnRspSubIndustryIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行业指数行情应答
	virtual void OnRspUnSubIndustryIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅概念指数行情应答
	virtual void OnRspSubConceptionIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅概念指数行情应答
	virtual void OnRspUnSubConceptionIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅价格异常波动委托明细应答
	virtual void OnRspSubEffectOrderDetail(CQCVDEffectDetailItemField *pEffectDetailItem, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅价格异常波动委托明细应答
	virtual void OnRspUnSubEffectOrderDetail(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅价格异常波动成交明细应答
	virtual void OnRspSubEffectTradeDetail(CQCVDEffectDetailItemField *pEffectDetailItem, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅价格异常波动成交明细应答
	virtual void OnRspUnSubEffectTradeDetail(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///特定行情通知
	virtual void OnRtnSpecialMarketData(CQCVDSpecialMarketDataField *pSpecialMarketData);

	///资金流向行情通知
	virtual void OnRtnFundsFlowMarketData(CQCVDFundsFlowMarketDataField *pFundsFlowMarketData);

	///价格波动异常行情通知
	virtual void OnRtnEffectPriceMarketData(CQCVDEffectPriceMarketDataField *pEffectPriceMarketData);

	///数量波动异常行情通知
	virtual void OnRtnEffectVolumeMarketData(CQCVDEffectVolumeMarketDataField *pEffectVolumeMarketData);

	///价格异常波动委托明细通知
	virtual void OnRtnEffectOrderDetail(CQCVDEffectOrderDetailField *pEffectOrderDetail);

	///价格异常波动成交明细通知
	virtual void OnRtnEffectTradeDetail(CQCVDEffectTradeDetailField *pEffectTradeDetail);

	///行业指数行情通知
	virtual void OnRtnIndustryIndexData(CQCVDIndustryIndexDataField *pIndustryIndexData);

	///概念指数行情通知
	virtual void OnRtnConceptionIndexData(CQCVDConceptionIndexDataField *pConceptionIndexData);

	virtual void OnRspQryCBondConvPrice(CQCVDCBondConvPriceField *pCBondConvPrice, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryChinaClosedFund(CQCVDChinaClosedFundField *pChinaClosedFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryChinaHedgeFund(CQCVDChinaHedgeFundField *pChinaHedgeFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryChinaMutualFund(CQCVDChinaMutualFundField *pChinaMutualFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryChinaOptionEodPrices(CQCVDChinaOptionEodPricesField *pChinaOptionEodPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryCindexfutureseodPrices(CQCVDCindexfutureseodPricesField *pCindexfutureseodPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryCMoneyMarketDailyFIncome(CQCVDCMoneyMarketDailyFIncomeField *pCMoneyMarketDailyFIncome, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryCommodityFuturesPrice(CQCVDCommodityFuturesPriceField *pCommodityFuturesPrice, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);


	virtual void OnRspQryGoldSpotPrices(CQCVDGoldSpotPricesField *pGoldSpotPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast);

	///订阅快速股票指数行情应答
	virtual void OnRspSubRapidSecurityIndexData(CQCVDSpecificSecurityField* pSpecificSecurity, CQCVDRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///快速股票指数行情通知
	virtual void OnRtnRapidSecurityIndexData(CQCVDRapidSecurityIndexDataField* pRapidSecurityIndexData);

};

class QryThread{
	CQCValueAddProApi* m_api;
	CQCVDSimpleSpi* m_spi;
	std::thread* m_thread;
public:
	QryThread(CQCValueAddProApi* t_api, CQCVDSimpleSpi* t_spi);
	void Start();
	void Join();
	void run();
};

class StartupThread{
	CQCValueAddProApi* m_api;
	CQCVDSimpleSpi* m_spi;
	std::thread* m_thread;
public:
	StartupThread();
	void Start();
	void Join();
	void run();
};


#endif
