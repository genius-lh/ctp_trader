#include "QCInfoSimpleSpi.h"
#include <string>
#include "QCValueAddProApi.h"
#include "QCValueAddProApiDataType.h"
#include "QCValueAddProApiStruct.h"

int g_port = 0;
char g_address[32] = { 0 };
char g_userid[128] = { 0 };
char g_passwd[128] = { 0 };
char g_filename[128] = { 0 };

qrycmdmgr* qrycmdmgr::m_instance = NULL;

static void Record(CQCVDRapidSecurityIndexDataField* pRapidSecurityIndexData)
{
  do{
    FILE* fp = fopen(g_filename, "a");
    if(!fp){
      break;
    }

    fprintf(fp, "%s,%d,%s,%f,%lld\n"
      , pRapidSecurityIndexData->UpdateTime
      , pRapidSecurityIndexData->UpdateMillisec
      , pRapidSecurityIndexData->SecurityID
      , pRapidSecurityIndexData->LastPrice
      , pRapidSecurityIndexData->Volume
    );
    
    fclose(fp);
  }while(0);
}


static std::string char2str(char v)
{
	if ((0x00) == v)
	{
		return std::string("");
	}
	else
	{
		char strbuff[2];
		memset(strbuff, 0, sizeof(strbuff));
		strbuff[0] = v;
		return std::string(strbuff);
	}
}

QryThread::QryThread(CQCValueAddProApi* t_api, CQCVDSimpleSpi* t_spi)
{
	m_api = t_api;
	m_spi = t_spi;
	m_thread = NULL;
}
void QryThread::Start()
{
	if (NULL == m_thread)
	{
		m_thread = new std::thread(&QryThread::run, this);
	}
}
void QryThread::Join()
{
	if (m_thread)
	{
		m_thread->join();
	}
}

void QryThread::run()
{
	printf("QryThread Start run \n");
	while (true)
	{
		if ((m_spi->m_is_login.load()) && (!m_spi->m_is_qrying.load()))
		{
			qrycmd qry;
			bool ret = qrycmdmgr::GetInstance()->PopQry(qry);
			if (ret)
			{
				if (false)
				{
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqInquiryJZFundField).name()))
				{
					printf("QryThread ReqInquiryJZFund ###########################################################################################\n");
					m_api->ReqInquiryJZFund((CQCVDReqInquiryJZFundField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqInquiryBankAccountFundField).name()))
				{
					printf("QryThread ReqInquiryBankAccountFund ###########################################################################################\n");
					m_api->ReqInquiryBankAccountFund((CQCVDReqInquiryBankAccountFundField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIPOInfoField).name()))
				{
					printf("QryThread ReqQryIPOInfo ###########################################################################################\n");
					m_api->ReqQryIPOInfo((CQCVDQryIPOInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryInvestorField).name()))
				{
					printf("QryThread ReqQryInvestor ###########################################################################################\n");
					m_api->ReqQryInvestor((CQCVDQryInvestorField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryShareholderAccountField).name()))
				{
					printf("QryThread ReqQryShareholderAccount ###########################################################################################\n");
					m_api->ReqQryShareholderAccount((CQCVDQryShareholderAccountField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryOrderField).name()))
				{
					printf("QryThread ReqQryOrder ###########################################################################################\n");
					m_api->ReqQryOrder((CQCVDQryOrderField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryOrderActionField).name()))
				{
					printf("QryThread ReqQryOrderAction ###########################################################################################\n");
					m_api->ReqQryOrderAction((CQCVDQryOrderActionField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryTradeField).name()))
				{
					printf("QryThread ReqQryTrade ###########################################################################################\n");
					m_api->ReqQryTrade((CQCVDQryTradeField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryTradingAccountField).name()))
				{
					printf("QryThread ReqQryTradingAccount ###########################################################################################\n");
					m_api->ReqQryTradingAccount((CQCVDQryTradingAccountField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPositionField).name()))
				{
					printf("QryThread ReqQryPosition ###########################################################################################\n");
					m_api->ReqQryPosition((CQCVDQryPositionField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIPOQuotaField).name()))
				{
					printf("QryThread ReqQryIPOQuota ###########################################################################################\n");
					m_api->ReqQryIPOQuota((CQCVDQryIPOQuotaField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryFundTransferDetailField).name()))
				{
					printf("QryThread ReqQryFundTransferDetail ###########################################################################################\n");
					m_api->ReqQryFundTransferDetail((CQCVDQryFundTransferDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPositionTransferDetailField).name()))
				{
					printf("QryThread ReqQryPositionTransferDetail ###########################################################################################\n");
					m_api->ReqQryPositionTransferDetail((CQCVDQryPositionTransferDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPrematurityRepoOrderField).name()))
				{
					printf("QryThread ReqQryPrematurityRepoOrder ###########################################################################################\n");
					m_api->ReqQryPrematurityRepoOrder((CQCVDQryPrematurityRepoOrderField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPeripheryPositionTransferDetailField).name()))
				{
					printf("QryThread ReqQryPeripheryPositionTransferDetail ###########################################################################################\n");
					m_api->ReqQryPeripheryPositionTransferDetail((CQCVDQryPeripheryPositionTransferDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCondOrderField).name()))
				{
					printf("QryThread ReqQryCondOrder ###########################################################################################\n");
					m_api->ReqQryCondOrder((CQCVDQryCondOrderField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCondOrderActionField).name()))
				{
					printf("QryThread ReqQryCondOrderAction ###########################################################################################\n");
					m_api->ReqQryCondOrderAction((CQCVDQryCondOrderActionField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIPONumberResultField).name()))
				{
					printf("QryThread ReqQryIPONumberResult ###########################################################################################\n");
					m_api->ReqQryIPONumberResult((CQCVDQryIPONumberResultField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIPOMatchNumberResultField).name()))
				{
					printf("QryThread ReqQryIPOMatchNumberResult ###########################################################################################\n");
					m_api->ReqQryIPOMatchNumberResult((CQCVDQryIPOMatchNumberResultField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryShareholderSpecPrivilegeField).name()))
				{
					printf("QryThread ReqQryShareholderSpecPrivilege ###########################################################################################\n");
					m_api->ReqQryShareholderSpecPrivilege((CQCVDQryShareholderSpecPrivilegeField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRationalInfoField).name()))
				{
					printf("QryThread ReqQryRationalInfo ###########################################################################################\n");
					m_api->ReqQryRationalInfo((CQCVDQryRationalInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPeripheryFundTransferDetailField).name()))
				{
					printf("QryThread ReqQryPeripheryFundTransferDetail ###########################################################################################\n");
					m_api->ReqQryPeripheryFundTransferDetail((CQCVDQryPeripheryFundTransferDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryFreeFloatSharesInfoField).name()))
				{
					printf("QryThread ReqQryFreeFloatSharesData ###########################################################################################\n");
					m_api->ReqQryFreeFloatSharesInfo((CQCVDQryFreeFloatSharesInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRightsAdjustmentInfoField).name()))
				{
					printf("QryThread ReqQryRightsAdjustmentInfo ###########################################################################################\n");
					m_api->ReqQryRightsAdjustmentInfo((CQCVDQryRightsAdjustmentInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryHistoryFundsFlowInfoField).name()))
				{
					printf("QryThread ReqQryHistoryFundsFlowInfo ###########################################################################################\n");
					m_api->ReqQryHistoryFundsFlowInfo((CQCVDQryHistoryFundsFlowInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryFinancialIndicatorInfoField).name()))
				{
					printf("QryThread ReqQryFinancialIndicatorInfo ###########################################################################################\n");
					m_api->ReqQryFinancialIndicatorInfo((CQCVDQryFinancialIndicatorInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryDividendInfoField).name()))
				{
					printf("QryThread ReqQryDividendInfo ###########################################################################################\n");
					m_api->ReqQryDividendInfo((CQCVDQryDividendInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRightIssueInfoField).name()))
				{
					printf("QryThread ReqQryRightIssueInfo ###########################################################################################\n");
					m_api->ReqQryRightIssueInfo((CQCVDQryRightIssueInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCompanyDescriptionInfoField).name()))
				{
					printf("QryThread ReqQryCompanyDescriptionInfo ###########################################################################################\n");
					m_api->ReqQryCompanyDescriptionInfo((CQCVDQryCompanyDescriptionInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryEquityStructureInfoField).name()))
				{
					printf("QryThread ReqQryEquityStructureInfo ###########################################################################################\n");
					m_api->ReqQryEquityStructureInfo((CQCVDQryEquityStructureInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQrySalesSegmentInfoField).name()))
				{
					printf("QryThread ReqQrySalesSegmentInfo ###########################################################################################\n");
					m_api->ReqQrySalesSegmentInfo((CQCVDQrySalesSegmentInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryTopTenHoldersInfoField).name()))
				{
					printf("QryThread ReqQryTopTenHoldersInfo ###########################################################################################\n");
					m_api->ReqQryTopTenHoldersInfo((CQCVDQryTopTenHoldersInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryTopTenFloatHoldersInfoField).name()))
				{
					printf("QryThread ReqQryTopTenFloatHoldersInfo ###########################################################################################\n");
					m_api->ReqQryTopTenFloatHoldersInfo((CQCVDQryTopTenFloatHoldersInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIndustryInfoField).name()))
				{
					printf("QryThread ReqQryIndustryInfo ###########################################################################################\n");
					m_api->ReqQryIndustryInfo((CQCVDQryIndustryInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryConceptionInfoField).name()))
				{
					printf("QryThread ReqQryConceptionInfo ###########################################################################################\n");
					m_api->ReqQryConceptionInfo((CQCVDQryConceptionInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRegionInfoField).name()))
				{
					printf("QryThread ReqQryRegionInfo ###########################################################################################\n");
					m_api->ReqQryRegionInfo((CQCVDQryRegionInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIndexDescriptionInfoField).name()))
				{
					printf("QryThread ReqQryIndexDescriptionInfo ###########################################################################################\n");
					m_api->ReqQryIndexDescriptionInfo((CQCVDQryIndexDescriptionInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIndustryConstituentsInfoField).name()))
				{
					printf("QryThread ReqQryIndustryConstituentsInfo ###########################################################################################\n");
					m_api->ReqQryIndustryConstituentsInfo((CQCVDQryIndustryConstituentsInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryConceptionConstituentsInfoField).name()))
				{
					printf("QryThread ReqQryConceptionConstituentsInfo ###########################################################################################\n");
					m_api->ReqQryConceptionConstituentsInfo((CQCVDQryConceptionConstituentsInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryIndustryCodeListField).name()))
				{
					printf("QryThread ReqQryIndustryCodeList ###########################################################################################\n");
					m_api->ReqQryIndustryCodeList((CQCVDQryIndustryCodeListField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryConceptionCodeListField).name()))
				{
					printf("QryThread ReqQryConceptionCodeList ###########################################################################################\n");
					m_api->ReqQryConceptionCodeList((CQCVDQryConceptionCodeListField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRegionCodeListField).name()))
				{
					printf("QryThread ReqQryRegionCodeList ###########################################################################################\n");
					m_api->ReqQryRegionCodeList((CQCVDQryRegionCodeListField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQrySWSIndexDataField).name()))
				{
					printf("QryThread ReqReqQrySWSIndexData ###########################################################################################\n");
					m_api->ReqReqQrySWSIndexData((CQCVDReqQrySWSIndexDataField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryStockDayQuotationField).name()))
				{
					printf("QryThread ReqReqQryStockDayQuotation ###########################################################################################\n");
					m_api->ReqReqQryStockDayQuotation((CQCVDReqQryStockDayQuotationField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryShareCalendarField).name()))
				{
					printf("QryThread ReqReqQryShareCalendar ###########################################################################################\n");
					m_api->ReqReqQryShareCalendar((CQCVDReqQryShareCalendarField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQrySWIndustriesClassField).name()))
				{
					printf("QryThread ReqReqQrySWIndustriesClass ###########################################################################################\n");
					m_api->ReqReqQrySWIndustriesClass((CQCVDReqQrySWIndustriesClassField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryStockAssessIndicatorField).name()))
				{
					printf("QryThread ReqReqQryStockAssessIndicator ###########################################################################################\n");
					m_api->ReqReqQryStockAssessIndicator((CQCVDReqQryStockAssessIndicatorField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryConsensusRollingDataField).name()))
				{
					printf("QryThread ReqReqQryConsensusRollingData ###########################################################################################\n");
					m_api->ReqReqQryConsensusRollingData((CQCVDReqQryConsensusRollingDataField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryIndexDayQuotationField).name()))
				{
					printf("QryThread ReqReqQryIndexDayQuotation ###########################################################################################\n");
					m_api->ReqReqQryIndexDayQuotation((CQCVDReqQryIndexDayQuotationField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryShareDescriptionField).name()))
				{
					printf("QryThread ReqReqQryShareDescription ###########################################################################################\n");
					m_api->ReqReqQryShareDescription((CQCVDReqQryShareDescriptionField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryTopTenHoldersDetailField).name()))
				{
					printf("QryThread ReqReqQryTopTenHoldersDetail ###########################################################################################\n");
					m_api->ReqReqQryTopTenHoldersDetail((CQCVDReqQryTopTenHoldersDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHisShareProfitField).name()))
				{
					printf("QryThread ReqReqQryHisShareProfit ###########################################################################################\n");
					m_api->ReqReqQryHisShareProfit((CQCVDReqQryHisShareProfitField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryAccountIDByInvestorIDField).name()))
				{
					printf("QryThread ReqReqQryAccountIDByInvestorID ###########################################################################################\n");
					m_api->ReqReqQryAccountIDByInvestorID((CQCVDReqQryAccountIDByInvestorIDField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryCustPeriodCapiDataField).name()))
				{
					printf("QryThread ReqReqQryCustPeriodCapiData ###########################################################################################\n");
					m_api->ReqReqQryCustPeriodCapiData((CQCVDReqQryCustPeriodCapiDataField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryCustPeriodProfitDataField).name()))
				{
					printf("QryThread ReqReqQryCustPeriodProfitData ###########################################################################################\n");
					m_api->ReqReqQryCustPeriodProfitData((CQCVDReqQryCustPeriodProfitDataField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryShareIssuanceField).name()))
				{
					printf("QryThread ReqReqQryShareIssuance ###########################################################################################\n");
					m_api->ReqReqQryShareIssuance((CQCVDReqQryShareIssuanceField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryBondIssuanceField).name()))
				{
					printf("QryThread ReqReqQryBondIssuance ###########################################################################################\n");
					m_api->ReqReqQryBondIssuance((CQCVDReqQryBondIssuanceField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryShareEquityPledgeInfoField).name()))
				{
					printf("QryThread ReqReqQryShareEquityPledgeInfo ###########################################################################################\n");
					m_api->ReqReqQryShareEquityPledgeInfo((CQCVDReqQryShareEquityPledgeInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryCapitalField).name()))
				{
					printf("QryThread ReqReqQryHistoryCapital ###########################################################################################\n");
					m_api->ReqReqQryHistoryCapital((CQCVDReqQryHistoryCapitalField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryDeliveryField).name()))
				{
					printf("QryThread ReqReqQryHistoryDelivery ###########################################################################################\n");
					m_api->ReqReqQryHistoryDelivery((CQCVDReqQryHistoryDeliveryField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryFundDetailField).name()))
				{
					printf("QryThread ReqReqQryHistoryFundDetail ###########################################################################################\n");
					m_api->ReqReqQryHistoryFundDetail((CQCVDReqQryHistoryFundDetailField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryHoldField).name()))
				{
					printf("QryThread ReqReqQryHistoryHold ###########################################################################################\n");
					m_api->ReqReqQryHistoryHold((CQCVDReqQryHistoryHoldField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryOrderEXField).name()))
				{
					printf("QryThread ReqReqQryHistoryOrderEX ###########################################################################################\n");
					m_api->ReqReqQryHistoryOrderEX((CQCVDReqQryHistoryOrderEXField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqQryHistoryTradeEXField).name()))
				{
					printf("QryThread ReqReqQryHistoryTradeEX ###########################################################################################\n");
					m_api->ReqReqQryHistoryTradeEX((CQCVDReqQryHistoryTradeEXField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDInputRemarkEventField).name()))
				{
					printf("QryThread ReqInputRemarkEvent ###########################################################################################\n");
					m_api->ReqInputRemarkEvent((CQCVDInputRemarkEventField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDUpdateRemarkEventField).name()))
				{
					printf("QryThread ReqUpdateRemarkEvent ###########################################################################################\n");
					m_api->ReqUpdateRemarkEvent((CQCVDUpdateRemarkEventField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDDeleteRemarkEventField).name()))
				{
					printf("QryThread ReqDeleteRemarkEvent ###########################################################################################\n");
					m_api->ReqDeleteRemarkEvent((CQCVDDeleteRemarkEventField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryRemarkEventField).name()))
				{
					printf("QryThread ReqQryRemarkEvent ###########################################################################################\n");
					m_api->ReqQryRemarkEvent((CQCVDQryRemarkEventField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryPriceDistributionInfoField).name()))
				{
					printf("QryThread ReqQryPriceDistributionInfo ###########################################################################################\n");
					m_api->ReqQryPriceDistributionInfo((CQCVDQryPriceDistributionInfoField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDInquiryQueueingOrdersField).name()))
				{
					printf("QryThread ReqInquiryQueueingOrders ###########################################################################################\n");
					m_api->ReqInquiryQueueingOrders((CQCVDInquiryQueueingOrdersField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDReqOptionGreeceField).name()))
				{
					printf("QryThread ReqReqOptionGreece ###########################################################################################\n");
					m_api->ReqQryOptionGreece((CQCVDReqOptionGreeceField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryBondDayQuotationField).name()))
				{
					printf("QryThread ReqQryBondDayQuotation ###########################################################################################\n");
					m_api->ReqQryBondDayQuotation((CQCVDQryBondDayQuotationField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryGGTEODPricesField).name()))
				{
					printf("QryThread ReqQryGGTEODPrices ###########################################################################################\n");
					m_api->ReqQryGGTEODPrices((CQCVDQryGGTEODPricesField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCBondConvPriceField).name()))
				{
					printf("QryThread ReqQryCBondConvPrice ###########################################################################################\n");
					m_api->ReqQryCBondConvPrice((CQCVDQryCBondConvPriceField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryChinaClosedFundField).name()))
				{
					printf("QryThread ReqQryChinaClosedFund ###########################################################################################\n");
					m_api->ReqQryChinaClosedFund((CQCVDQryChinaClosedFundField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryChinaHedgeFundField).name()))
				{
					printf("QryThread ReqQryChinaHedgeFund ###########################################################################################\n");
					m_api->ReqQryChinaHedgeFund((CQCVDQryChinaHedgeFundField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryChinaMutualFundField).name()))
				{
					printf("QryThread ReqQryChinaMutualFund ###########################################################################################\n");
					m_api->ReqQryChinaMutualFund((CQCVDQryChinaMutualFundField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryChinaOptionEodPricesField).name()))
				{
					printf("QryThread ReqQryChinaOptionEodPrices ###########################################################################################\n");
					m_api->ReqQryChinaOptionEodPrices((CQCVDQryChinaOptionEodPricesField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCindexfutureseodPricesField).name()))
				{
					printf("QryThread ReqQryCindexfutureseodPrices ###########################################################################################\n");
					m_api->ReqQryCindexfutureseodPrices((CQCVDQryCindexfutureseodPricesField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCMoneyMarketDailyFIncomeField).name()))
				{
					printf("QryThread ReqQryCMoneyMarketDailyFIncome ###########################################################################################\n");
					m_api->ReqQryCMoneyMarketDailyFIncome((CQCVDQryCMoneyMarketDailyFIncomeField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryCommodityFuturesPriceField).name()))
				{
					printf("QryThread ReqQryCommodityFuturesPrice ###########################################################################################\n");
					m_api->ReqQryCommodityFuturesPrice((CQCVDQryCommodityFuturesPriceField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}
				else if (0 == strcmp(qry.CmdID.c_str(), typeid(CQCVDQryGoldSpotPricesField).name()))
				{
					printf("QryThread ReqQryGoldSpotPrices ###########################################################################################\n");
					m_api->ReqQryGoldSpotPrices((CQCVDQryGoldSpotPricesField*)qry.CmdData, (++m_spi->m_requestid));
					m_spi->m_is_qrying.store(true);
				}

			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

StartupThread::StartupThread()
{
	m_api = NULL;
	m_spi = NULL;
	m_thread = NULL;
}
void StartupThread::Start()
{
	if (NULL == m_thread)
	{
		m_thread = new std::thread(&StartupThread::run, this);
	}
}
void StartupThread::Join()
{
	if (m_thread)
	{
		m_thread->join();
	}
}

void StartupThread::run()
{
	printf("StartupThread Start run \n");
	printf("CQCValueAddProSpi GetApiVersion=%s \n", CQCValueAddProApi::GetApiVersion());
	m_api = CQCValueAddProApi::CreateInfoQryApi();
	m_api->RegisterFront(g_address, g_port);
	m_spi = new CQCVDSimpleSpi(m_api);
	m_api->RegisterSpi(m_spi);

	QryThread* qrythread = new QryThread(m_api, m_spi);
	qrythread->Start();

	m_api->Run();

	qrythread->Join();

}



CQCVDSimpleSpi::CQCVDSimpleSpi(CQCValueAddProApi* t_api)
{
	m_api = t_api;
	m_requestid = 0;
	m_is_qrying.store(false);
	m_is_connected.store(false);
	m_is_login.store(false);
}

void CQCVDSimpleSpi::OnFrontConnected()
{
	printf("OnFrontConnected \n");

	m_is_connected.store(true);
	m_is_login.store(false);

	CQCVDReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strncpy(req.LogInAccount, g_userid, sizeof(req.LogInAccount) - 1);
	strncpy(req.Password, g_passwd, sizeof(req.Password) - 1);
	req.AuthMode = QCVD_AM_Password;
	m_api->ReqUserLogin(&req, 1);
}

void CQCVDSimpleSpi::OnFrontDisconnected(int nReason)
{
	printf("OnFrontDisconnected Reason=%d \n", nReason);

	m_is_connected.store(false);
	m_is_login.store(false);
}

void CQCVDSimpleSpi::SubscribeMDs()
{


	// char* SubSZSESecurities[2] = { 0 };
	// for (int i = 0; i < sizeof(SubSZSESecurities); ++i)
	// {
		// SubSZSESecurities[i] = new char[31];
	// }
	// strcpy(SubSZSESecurities[0], "000413");
	// strcpy(SubSZSESecurities[1], "002385");
	// char* SubSSESecurities[2] = {(char*)"600000", (char*)"600009"};

	// char* SubIndustryIndexIDs[2] = { (char*)"886063", (char*)"886051"};
	// char* SubConceptionIndexIDs[2] = { (char*)"884001", (char*)"884002"};

	// char securityid[31] = { 0 };

	// m_api->SubscribeConceptionIndexData(SubConceptionIndexIDs, 2);
	// strcpy(securityid, "000001");
	// m_api->SubscribeEffectOrderDetail(QCVD_EXD_SZSE, securityid, 0.01);
	// m_api->SubscribeEffectTradeDetail(QCVD_EXD_SZSE, securityid, 0.01);
	// strcpy(securityid, "600000");
	// m_api->SubscribeEffectTradeDetail(QCVD_EXD_SSE, securityid, 0.01);
	// m_api->SubscribeFundsFlowMarketData(SubSZSESecurities, 2, QCVD_EXD_SZSE);
	// m_api->SubscribeFundsFlowMarketData(SubSSESecurities, 2, QCVD_EXD_SSE);
	// m_api->SubscribeSpecialMarketData(SubSZSESecurities, 2, QCVD_EXD_SZSE);
	// m_api->SubscribeSpecialMarketData(SubSSESecurities, 2, QCVD_EXD_SSE);
	// m_api->SubscribeIndustryIndexData(SubIndustryIndexIDs, 2);

	// for (int i = 0; i < sizeof(SubSZSESecurities); ++i)
	// {
		// if (SubSZSESecurities[i])
		// {
			// delete SubSZSESecurities[i];
			// SubSZSESecurities[i] = NULL;
		// }
	// }

	char index_code[31] = { 0 };
	strcpy(index_code, "000016");
	m_api->SubscribeRapidSecurityIndexData(QCVD_EXD_SSE, index_code);
	strcpy(index_code, "000300");
	m_api->SubscribeRapidSecurityIndexData(QCVD_EXD_SSE, index_code);
	strcpy(index_code, "000905");
	m_api->SubscribeRapidSecurityIndexData(QCVD_EXD_SSE, index_code);
	strcpy(index_code, "000852");
	m_api->SubscribeRapidSecurityIndexData(QCVD_EXD_SSE, index_code);

}

void CQCVDSimpleSpi::ReqQryValueAddDatas()
{

	CQCVDQryDividendInfoField* QryDividendInfoField = new CQCVDQryDividendInfoField();
	memset(QryDividendInfoField, 0, sizeof(CQCVDQryDividendInfoField));
	QryDividendInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryDividendInfoField->SecurityID, "002531");
	strcpy(QryDividendInfoField->Progress, "3");
	strcpy(QryDividendInfoField->BegDate, "20200704");
	strcpy(QryDividendInfoField->EndDate, "20200706");
	QryDividendInfoField->PageCount = 200;
	QryDividendInfoField->PageLocate = 1;
	qrycmd QryDividendInfoFieldCmd(typeid(CQCVDQryDividendInfoField).name(), QryDividendInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryDividendInfoFieldCmd);

	CQCVDQryBondDayQuotationField* QryBondDayQuotationField = new CQCVDQryBondDayQuotationField();
	memset(QryBondDayQuotationField, 0, sizeof(CQCVDQryBondDayQuotationField));
	QryBondDayQuotationField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryBondDayQuotationField->SecurityID, "100303");
	strcpy(QryBondDayQuotationField->BegDate, "20200802");
	strcpy(QryBondDayQuotationField->EndDate, "20200812");
	QryBondDayQuotationField->OrderType = QCVD_ORDST_ASC;
	qrycmd QryBondDayQuotationFieldCmd(typeid(CQCVDQryBondDayQuotationField).name(), QryBondDayQuotationField);
	qrycmdmgr::GetInstance()->AddQry(QryBondDayQuotationFieldCmd);

	CQCVDReqQryHistoryOrderEXField* ReqQryHistoryOrderEXField = new CQCVDReqQryHistoryOrderEXField();
	memset(ReqQryHistoryOrderEXField, 0, sizeof(CQCVDReqQryHistoryOrderEXField));
	ReqQryHistoryOrderEXField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryHistoryOrderEXField->SecurityID, "300747");
	strcpy(ReqQryHistoryOrderEXField->BegDate, "20200820");
	strcpy(ReqQryHistoryOrderEXField->EndDate, "20200827");
	ReqQryHistoryOrderEXField->OrderType = QCVD_ORDST_ASC;
	qrycmd ReqQryHistoryOrderEXFieldCmd(typeid(CQCVDReqQryHistoryOrderEXField).name(), ReqQryHistoryOrderEXField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryOrderEXFieldCmd);

	CQCVDReqQrySWSIndexDataField* ReqQrySWSIndexDataField = new CQCVDReqQrySWSIndexDataField();
	memset(ReqQrySWSIndexDataField, 0, sizeof(CQCVDReqQrySWSIndexDataField));
	strcpy(ReqQrySWSIndexDataField->IndexID, "850363.SI");
	strcpy(ReqQrySWSIndexDataField->BegDate, "20200101");
	strcpy(ReqQrySWSIndexDataField->EndDate, "20200801");
	qrycmd ReqQrySWSIndexDataFieldCmd(typeid(CQCVDReqQrySWSIndexDataField).name(), ReqQrySWSIndexDataField);
	qrycmdmgr::GetInstance()->AddQry(ReqQrySWSIndexDataFieldCmd);

	CQCVDReqQryStockDayQuotationField* ReqQryStockDayQuotationField = new CQCVDReqQryStockDayQuotationField();
	memset(ReqQryStockDayQuotationField, 0, sizeof(CQCVDReqQryStockDayQuotationField));
	//ReqQryStockDayQuotationField->ExchangeID = QCVD_EXD_SZSE;
	ReqQryStockDayQuotationField->OrderType = QCVD_ORDST_DESC;
	//strcpy(ReqQryStockDayQuotationField->SecurityID, "000002");
	strcpy(ReqQryStockDayQuotationField->BegDate, "20200101");
	strcpy(ReqQryStockDayQuotationField->EndDate, "20200801");
	qrycmd ReqQryStockDayQuotationFieldCmd(typeid(CQCVDReqQryStockDayQuotationField).name(), ReqQryStockDayQuotationField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryStockDayQuotationFieldCmd);

	CQCVDReqQryShareCalendarField* ReqQryShareCalendarField = new CQCVDReqQryShareCalendarField();
	memset(ReqQryShareCalendarField, 0, sizeof(CQCVDReqQryShareCalendarField));
	ReqQryShareCalendarField->OrderType = QCVD_ORDST_ASC;
	strcpy(ReqQryShareCalendarField->BegDate, "20200101");
	strcpy(ReqQryShareCalendarField->EndDate, "20200801");
	qrycmd ReqQryShareCalendarFieldCmd(typeid(CQCVDReqQryShareCalendarField).name(), ReqQryShareCalendarField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryShareCalendarFieldCmd);

	CQCVDReqQrySWIndustriesClassField* ReqQrySWIndustriesClassField = new CQCVDReqQrySWIndustriesClassField();
	memset(ReqQrySWIndustriesClassField, 0, sizeof(CQCVDReqQrySWIndustriesClassField));
	//ReqQrySWIndustriesClassField->ExchangeID = QCVD_EXD_SZSE;
	//strcpy(ReqQrySWIndustriesClassField->SWIndustryCode, "6118010100");
	strcpy(ReqQrySWIndustriesClassField->SecurityID, "600683");
	//strcpy(ReqQrySWIndustriesClassField->TradingDay, "20200801");
	ReqQrySWIndustriesClassField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQrySWIndustriesClassFieldCmd(typeid(CQCVDReqQrySWIndustriesClassField).name(), ReqQrySWIndustriesClassField);
	qrycmdmgr::GetInstance()->AddQry(ReqQrySWIndustriesClassFieldCmd);


	CQCVDReqQryStockAssessIndicatorField* ReqQryStockAssessIndicatorField = new CQCVDReqQryStockAssessIndicatorField();
	memset(ReqQryStockAssessIndicatorField, 0, sizeof(CQCVDReqQryStockAssessIndicatorField));
	ReqQryStockAssessIndicatorField->ExchangeID = QCVD_EXD_SZSE;
	//strcpy(ReqQryStockAssessIndicatorField->SecurityID, "000002");
	strcpy(ReqQryStockAssessIndicatorField->BegDate, "20200501");
	strcpy(ReqQryStockAssessIndicatorField->EndDate, "20200801");
	ReqQryStockAssessIndicatorField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryStockAssessIndicatorFieldCmd(typeid(CQCVDReqQryStockAssessIndicatorField).name(), ReqQryStockAssessIndicatorField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryStockAssessIndicatorFieldCmd);

	CQCVDReqQryConsensusRollingDataField* ReqQryConsensusRollingDataField = new CQCVDReqQryConsensusRollingDataField();
	memset(ReqQryConsensusRollingDataField, 0, sizeof(CQCVDReqQryConsensusRollingDataField));
	ReqQryConsensusRollingDataField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryConsensusRollingDataField->SecurityID, "000002");
	strcpy(ReqQryConsensusRollingDataField->BegDate, "20200501");
	strcpy(ReqQryConsensusRollingDataField->EndDate, "20200801");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_FY0);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_FY1);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_FY2);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_FY3);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_YOY);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_YOY2);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_FTTM);
	strcat(ReqQryConsensusRollingDataField->RollingType, "|");
	strcat(ReqQryConsensusRollingDataField->RollingType, QCVD_ROLLINGTYPE_CAGR);
	ReqQryConsensusRollingDataField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryConsensusRollingDataFieldCmd(typeid(CQCVDReqQryConsensusRollingDataField).name(), ReqQryConsensusRollingDataField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryConsensusRollingDataFieldCmd);

	CQCVDReqQryIndexDayQuotationField* ReqQryIndexDayQuotationField = new CQCVDReqQryIndexDayQuotationField();
	memset(ReqQryIndexDayQuotationField, 0, sizeof(CQCVDReqQryIndexDayQuotationField));
	strcpy(ReqQryIndexDayQuotationField->IndexCode, "000001.SH");
	strcpy(ReqQryIndexDayQuotationField->BegDate, "20200501");
	strcpy(ReqQryIndexDayQuotationField->EndDate, "20200801");
	ReqQryIndexDayQuotationField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryIndexDayQuotationFieldCmd(typeid(CQCVDReqQryIndexDayQuotationField).name(), ReqQryIndexDayQuotationField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryIndexDayQuotationFieldCmd);

	CQCVDReqQryShareDescriptionField* ReqQryShareDescriptionField = new CQCVDReqQryShareDescriptionField();
	memset(ReqQryShareDescriptionField, 0, sizeof(CQCVDReqQryShareDescriptionField));
	ReqQryShareDescriptionField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryShareDescriptionField->SecurityID, "000002");
	ReqQryShareDescriptionField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryShareDescriptionFieldCmd(typeid(CQCVDReqQryShareDescriptionField).name(), ReqQryShareDescriptionField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryShareDescriptionFieldCmd);

	CQCVDReqQryTopTenHoldersDetailField* ReqQryTopTenHoldersDetailField = new CQCVDReqQryTopTenHoldersDetailField();
	memset(ReqQryTopTenHoldersDetailField, 0, sizeof(CQCVDReqQryTopTenHoldersDetailField));
	ReqQryTopTenHoldersDetailField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryTopTenHoldersDetailField->SecurityID, "000002");
	strcpy(ReqQryTopTenHoldersDetailField->BegAnnDate, "20200501");
	strcpy(ReqQryTopTenHoldersDetailField->EndAnnDate, "20200831");
	ReqQryTopTenHoldersDetailField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryTopTenHoldersDetailFieldCmd(typeid(CQCVDReqQryTopTenHoldersDetailField).name(), ReqQryTopTenHoldersDetailField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryTopTenHoldersDetailFieldCmd);

	CQCVDReqQryHisShareProfitField* ReqQryHisShareProfitField = new CQCVDReqQryHisShareProfitField();
	memset(ReqQryHisShareProfitField, 0, sizeof(CQCVDReqQryHisShareProfitField));
	ReqQryHisShareProfitField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryHisShareProfitField->SecurityID, "000002");
	strcpy(ReqQryHisShareProfitField->BegDate, "20200501");
	strcpy(ReqQryHisShareProfitField->EndDate, "20200824");
	qrycmd ReqQryHisShareProfitFieldCmd(typeid(CQCVDReqQryHisShareProfitField).name(), ReqQryHisShareProfitField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHisShareProfitFieldCmd);

	CQCVDReqQryAccountIDByInvestorIDField* ReqQryAccountIDByInvestorIDField = new CQCVDReqQryAccountIDByInvestorIDField();
	memset(ReqQryAccountIDByInvestorIDField, 0, sizeof(CQCVDReqQryAccountIDByInvestorIDField));
	qrycmd ReqQryAccountIDByInvestorIDFieldCmd(typeid(CQCVDReqQryAccountIDByInvestorIDField).name(), ReqQryAccountIDByInvestorIDField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryAccountIDByInvestorIDFieldCmd);

	CQCVDReqQryCustPeriodCapiDataField* ReqQryCustPeriodCapiDataField = new CQCVDReqQryCustPeriodCapiDataField();
	memset(ReqQryCustPeriodCapiDataField, 0, sizeof(CQCVDReqQryCustPeriodCapiDataField));
	strcpy(ReqQryCustPeriodCapiDataField->BegDate, "20200101");
	strcpy(ReqQryCustPeriodCapiDataField->EndDate, "20200824");
	qrycmd ReqQryCustPeriodCapiDataFieldCmd(typeid(CQCVDReqQryCustPeriodCapiDataField).name(), ReqQryCustPeriodCapiDataField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryCustPeriodCapiDataFieldCmd);

	CQCVDReqQryCustPeriodProfitDataField* ReqQryCustPeriodProfitDataField = new CQCVDReqQryCustPeriodProfitDataField();
	memset(ReqQryCustPeriodProfitDataField, 0, sizeof(CQCVDReqQryCustPeriodProfitDataField));
	strcpy(ReqQryCustPeriodProfitDataField->BegDate, "20200101");
	strcpy(ReqQryCustPeriodProfitDataField->EndDate, "20200801");
	ReqQryCustPeriodProfitDataField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryCustPeriodProfitDataFieldCmd(typeid(CQCVDReqQryCustPeriodProfitDataField).name(), ReqQryCustPeriodProfitDataField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryCustPeriodProfitDataFieldCmd);

	CQCVDReqQryShareIssuanceField* ReqQryShareIssuanceField = new CQCVDReqQryShareIssuanceField();
	memset(ReqQryShareIssuanceField, 0, sizeof(CQCVDReqQryShareIssuanceField));
	////ReqQryShareIssuanceField->ExchangeID = QCVD_EXD_SZSE;
	/////////////strcpy(ReqQryShareIssuanceField->SecurityID, "000002");
	strcpy(ReqQryShareIssuanceField->BegApplyDate, "20200106");
	strcpy(ReqQryShareIssuanceField->EndApplyDate, "20201024");
	strcpy(ReqQryShareIssuanceField->BegListDate, "20200106");
	strcpy(ReqQryShareIssuanceField->EndListDate, "20201024");
	////ReqQryShareIssuanceField->IsFailure = QCVD_ISSUANCEISFAILURE_FAILURE;
	ReqQryShareIssuanceField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryShareIssuanceFieldCmd(typeid(CQCVDReqQryShareIssuanceField).name(), ReqQryShareIssuanceField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryShareIssuanceFieldCmd);

	CQCVDReqQryBondIssuanceField* ReqQryBondIssuanceField = new CQCVDReqQryBondIssuanceField();
	memset(ReqQryBondIssuanceField, 0, sizeof(CQCVDReqQryBondIssuanceField));
	ReqQryBondIssuanceField->ExchangeID = QCVD_EXD_SZSE;
	//////////strcpy(ReqQryBondIssuanceField->SecurityID, "000002");
	strcpy(ReqQryBondIssuanceField->BegApplyDate, "20200506");
	strcpy(ReqQryBondIssuanceField->EndApplyDate, "20200801");
	strcpy(ReqQryBondIssuanceField->BegListDate, "20200506");
	strcpy(ReqQryBondIssuanceField->EndListDate, "20200801");
	ReqQryBondIssuanceField->IsFailure = QCVD_ISSUANCEISFAILURE_NORMAL;
	ReqQryBondIssuanceField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryBondIssuanceFieldCmd(typeid(CQCVDReqQryBondIssuanceField).name(), ReqQryBondIssuanceField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryBondIssuanceFieldCmd);

	CQCVDQryIndustryInfoField* QryIndustryInfoField = new CQCVDQryIndustryInfoField();
	memset(QryIndustryInfoField, 0, sizeof(CQCVDQryIndustryInfoField));
	QryIndustryInfoField->ExchangeID = QCVD_EXD_SZSE;
	//strcpy(QryIndustryInfoField->SecurityID, "000002");
	qrycmd QryIndustryInfoFieldCmd(typeid(CQCVDQryIndustryInfoField).name(), QryIndustryInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryIndustryInfoFieldCmd);

	CQCVDQryConceptionInfoField* QryConceptionInfoField = new CQCVDQryConceptionInfoField();
	memset(QryConceptionInfoField, 0, sizeof(CQCVDQryConceptionInfoField));
	QryConceptionInfoField->ExchangeID = QCVD_EXD_SZSE;
	//strcpy(QryConceptionInfoField->SecurityID, "000002");
	qrycmd QryConceptionInfoFieldCmd(typeid(CQCVDQryConceptionInfoField).name(), QryConceptionInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryConceptionInfoFieldCmd);

	CQCVDQryIndexDescriptionInfoField* QryIndexDescriptionInfoField = new CQCVDQryIndexDescriptionInfoField();
	memset(QryIndexDescriptionInfoField, 0, sizeof(CQCVDQryIndexDescriptionInfoField));
	//strcpy(QryIndexDescriptionInfoField->IndexID, "000002");
	qrycmd QryIndexDescriptionInfoFieldCmd(typeid(CQCVDQryIndexDescriptionInfoField).name(), QryIndexDescriptionInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryIndexDescriptionInfoFieldCmd);

	CQCVDQryRightIssueInfoField* QryRightIssueInfoField = new CQCVDQryRightIssueInfoField();
	memset(QryRightIssueInfoField, 0, sizeof(CQCVDQryRightIssueInfoField));
	QryRightIssueInfoField->ExchangeID = QCVD_EXD_SSE;
	//strcpy(QryRightIssueInfoField->SecurityID, "000002");
	strcpy(QryRightIssueInfoField->BegDate, "20180610");
	strcpy(QryRightIssueInfoField->EndDate, "20200801");
	qrycmd QryRightIssueInfoFieldCmd(typeid(CQCVDQryRightIssueInfoField).name(), QryRightIssueInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryRightIssueInfoFieldCmd);

	CQCVDQryCompanyDescriptionInfoField* QryCompanyDescriptionInfoField = new CQCVDQryCompanyDescriptionInfoField();
	memset(QryCompanyDescriptionInfoField, 0, sizeof(CQCVDQryCompanyDescriptionInfoField));
	QryCompanyDescriptionInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryCompanyDescriptionInfoField->SecurityID, "000002");
	qrycmd QryCompanyDescriptionInfoFieldCmd(typeid(CQCVDQryCompanyDescriptionInfoField).name(), QryCompanyDescriptionInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryCompanyDescriptionInfoFieldCmd);

	CQCVDQryEquityStructureInfoField* QryEquityStructureInfoField = new CQCVDQryEquityStructureInfoField();
	memset(QryEquityStructureInfoField, 0, sizeof(CQCVDQryEquityStructureInfoField));
	QryEquityStructureInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryEquityStructureInfoField->SecurityID, "000002");
	qrycmd QryEquityStructureInfoFieldCmd(typeid(CQCVDQryEquityStructureInfoField).name(), QryEquityStructureInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryEquityStructureInfoFieldCmd);

	CQCVDQrySalesSegmentInfoField* QrySalesSegmentInfoField = new CQCVDQrySalesSegmentInfoField();
	memset(QrySalesSegmentInfoField, 0, sizeof(CQCVDQrySalesSegmentInfoField));
	QrySalesSegmentInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QrySalesSegmentInfoField->SecurityID, "000002");
	qrycmd QrySalesSegmentInfoFieldCmd(typeid(CQCVDQrySalesSegmentInfoField).name(), QrySalesSegmentInfoField);
	qrycmdmgr::GetInstance()->AddQry(QrySalesSegmentInfoFieldCmd);

	CQCVDQryTopTenHoldersInfoField* QryTopTenHoldersInfoField = new CQCVDQryTopTenHoldersInfoField();
	memset(QryTopTenHoldersInfoField, 0, sizeof(CQCVDQryTopTenHoldersInfoField));
	QryTopTenHoldersInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryTopTenHoldersInfoField->SecurityID, "000002");
	qrycmd QryTopTenHoldersInfoFieldCmd(typeid(CQCVDQryTopTenHoldersInfoField).name(), QryTopTenHoldersInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryTopTenHoldersInfoFieldCmd);

	CQCVDQryTopTenFloatHoldersInfoField* QryTopTenFloatHoldersInfoField = new CQCVDQryTopTenFloatHoldersInfoField();
	memset(QryTopTenFloatHoldersInfoField, 0, sizeof(CQCVDQryTopTenFloatHoldersInfoField));
	QryTopTenFloatHoldersInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryTopTenFloatHoldersInfoField->SecurityID, "000002");
	qrycmd QryTopTenFloatHoldersInfoFieldCmd(typeid(CQCVDQryTopTenFloatHoldersInfoField).name(), QryTopTenFloatHoldersInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryTopTenFloatHoldersInfoFieldCmd);

	CQCVDQryIndustryConstituentsInfoField* QryIndustryConstituentsInfoField = new CQCVDQryIndustryConstituentsInfoField();
	memset(QryIndustryConstituentsInfoField, 0, sizeof(CQCVDQryIndustryConstituentsInfoField));
	strcpy(QryIndustryConstituentsInfoField->IndexID, "886001");
	qrycmd QryIndustryConstituentsInfoFieldCmd(typeid(CQCVDQryIndustryConstituentsInfoField).name(), QryIndustryConstituentsInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryIndustryConstituentsInfoFieldCmd);

	CQCVDQryRegionInfoField* QryRegionInfoField = new CQCVDQryRegionInfoField();
	memset(QryRegionInfoField, 0, sizeof(CQCVDQryRegionInfoField));
	QryRegionInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryRegionInfoField->SecurityID, "000002");
	qrycmd QryRegionInfoFieldCmd(typeid(CQCVDQryRegionInfoField).name(), QryRegionInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryRegionInfoFieldCmd);

	CQCVDQryIndustryCodeListField* QryIndustryCodeListField = new CQCVDQryIndustryCodeListField();
	memset(QryIndustryCodeListField, 0, sizeof(CQCVDQryIndustryCodeListField));
	qrycmd QryIndustryCodeListFieldCmd(typeid(CQCVDQryIndustryCodeListField).name(), QryIndustryCodeListField);
	qrycmdmgr::GetInstance()->AddQry(QryIndustryCodeListFieldCmd);

	CQCVDQryConceptionCodeListField* QryConceptionCodeListField = new CQCVDQryConceptionCodeListField();
	memset(QryConceptionCodeListField, 0, sizeof(CQCVDQryConceptionCodeListField));
	qrycmd QryConceptionCodeListFieldCmd(typeid(CQCVDQryConceptionCodeListField).name(), QryConceptionCodeListField);
	qrycmdmgr::GetInstance()->AddQry(QryConceptionCodeListFieldCmd);

	CQCVDQryRegionCodeListField* QryRegionCodeListField = new CQCVDQryRegionCodeListField();
	memset(QryRegionCodeListField, 0, sizeof(CQCVDQryRegionCodeListField));
	qrycmd QryRegionCodeListFieldCmd(typeid(CQCVDQryRegionCodeListField).name(), QryRegionCodeListField);
	qrycmdmgr::GetInstance()->AddQry(QryRegionCodeListFieldCmd);

	CQCVDQryConceptionConstituentsInfoField* QryConceptionConstituentsInfoField = new CQCVDQryConceptionConstituentsInfoField();
	memset(QryConceptionConstituentsInfoField, 0, sizeof(CQCVDQryConceptionConstituentsInfoField));
	strcpy(QryConceptionConstituentsInfoField->IndexID, "884001");
	qrycmd QryConceptionConstituentsInfoFieldCmd(typeid(CQCVDQryConceptionConstituentsInfoField).name(), QryConceptionConstituentsInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryConceptionConstituentsInfoFieldCmd);

	CQCVDQryGGTEODPricesField* QryGGTEODPricesField = new CQCVDQryGGTEODPricesField();
	memset(QryGGTEODPricesField, 0, sizeof(CQCVDQryGGTEODPricesField));
	strcpy(QryGGTEODPricesField->BegDate, "20201020");
	strcpy(QryGGTEODPricesField->EndDate, "20201109");
	QryGGTEODPricesField->PageCount = 200;
	QryGGTEODPricesField->PageLocate = 1;
	qrycmd QryGGTEODPricesFieldCmd(typeid(CQCVDQryGGTEODPricesField).name(), QryGGTEODPricesField);
	qrycmdmgr::GetInstance()->AddQry(QryGGTEODPricesFieldCmd);

	CQCVDReqQryHistoryTradeEXField* ReqQryHistoryTradeEXField = new CQCVDReqQryHistoryTradeEXField();
	//ReqQryHistoryTradeEXField->ExchangeID = QCVD_EXD_SZSE;
	memset(ReqQryHistoryTradeEXField, 0, sizeof(CQCVDReqQryHistoryTradeEXField));
	//strcpy(ReqQryHistoryTradeEXField->SecurityID, "600000");
	strcpy(ReqQryHistoryTradeEXField->BegDate, "20200101");
	strcpy(ReqQryHistoryTradeEXField->EndDate, "20200824");
	qrycmd ReqQryHistoryTradeEXFieldCmd(typeid(CQCVDReqQryHistoryTradeEXField).name(), ReqQryHistoryTradeEXField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryTradeEXFieldCmd);

	CQCVDQryRightsAdjustmentInfoField* QryRightsAdjustmentInfoField = new CQCVDQryRightsAdjustmentInfoField();
	memset(QryRightsAdjustmentInfoField, 0, sizeof(CQCVDQryRightsAdjustmentInfoField));
	QryRightsAdjustmentInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryRightsAdjustmentInfoField->SecurityID, "000002");
	strcpy(QryRightsAdjustmentInfoField->BegDate, "20180101");
	strcpy(QryRightsAdjustmentInfoField->EndDate, "20200801");
	qrycmd QryRightsAdjustmentInfoFieldCmd(typeid(CQCVDQryRightsAdjustmentInfoField).name(), QryRightsAdjustmentInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryRightsAdjustmentInfoFieldCmd);

	CQCVDQryFinancialIndicatorInfoField* QryFinancialIndicatorInfoField = new CQCVDQryFinancialIndicatorInfoField();
	memset(QryFinancialIndicatorInfoField, 0, sizeof(CQCVDQryFinancialIndicatorInfoField));
	QryFinancialIndicatorInfoField->ExchangeID = QCVD_EXD_SZSE;
	///strcpy(QryFinancialIndicatorInfoField->SecurityID, "000002");
	qrycmd QryFinancialIndicatorInfoFieldCmd(typeid(CQCVDQryFinancialIndicatorInfoField).name(), QryFinancialIndicatorInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryFinancialIndicatorInfoFieldCmd);

	CQCVDQryHistoryFundsFlowInfoField* QryHistoryFundsFlowInfoField = new CQCVDQryHistoryFundsFlowInfoField();
	memset(QryHistoryFundsFlowInfoField, 0, sizeof(CQCVDQryHistoryFundsFlowInfoField));
	///QryHistoryFundsFlowInfoField->ExchangeID = QCVD_EXD_SZSE;
	///strcpy(QryHistoryFundsFlowInfoField->SecurityID, "000002");
	strcpy(QryHistoryFundsFlowInfoField->BegDate, "20200610");
	strcpy(QryHistoryFundsFlowInfoField->EndDate, "20200801");
	qrycmd QryHistoryFundsFlowInfoFieldCmd(typeid(CQCVDQryHistoryFundsFlowInfoField).name(), QryHistoryFundsFlowInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryHistoryFundsFlowInfoFieldCmd);

	CQCVDReqQryShareEquityPledgeInfoField* ReqQryShareEquityPledgeInfoField = new CQCVDReqQryShareEquityPledgeInfoField();
	memset(ReqQryShareEquityPledgeInfoField, 0, sizeof(CQCVDReqQryShareEquityPledgeInfoField));
	ReqQryShareEquityPledgeInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(ReqQryShareEquityPledgeInfoField->SecurityID, "000002");
	strcpy(ReqQryShareEquityPledgeInfoField->BegAnnDate, "20200101");
	strcpy(ReqQryShareEquityPledgeInfoField->EndAnnDate, "20200801");
	ReqQryShareEquityPledgeInfoField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryShareEquityPledgeInfoFieldCmd(typeid(CQCVDReqQryShareEquityPledgeInfoField).name(), ReqQryShareEquityPledgeInfoField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryShareEquityPledgeInfoFieldCmd);

	CQCVDReqQryHistoryCapitalField* ReqQryHistoryCapitalField = new CQCVDReqQryHistoryCapitalField();
	memset(ReqQryHistoryCapitalField, 0, sizeof(CQCVDReqQryHistoryCapitalField));
	strcpy(ReqQryHistoryCapitalField->BegDate, "20200501");
	strcpy(ReqQryHistoryCapitalField->EndDate, "20200801");
	ReqQryHistoryCapitalField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryHistoryCapitalFieldCmd(typeid(CQCVDReqQryHistoryCapitalField).name(), ReqQryHistoryCapitalField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryCapitalFieldCmd);

	CQCVDReqQryHistoryDeliveryField* ReqQryHistoryDeliveryField = new CQCVDReqQryHistoryDeliveryField();
	memset(ReqQryHistoryDeliveryField, 0, sizeof(CQCVDReqQryHistoryDeliveryField));
	///strcpy(ReqQryHistoryDeliveryField->SecurityID, "000002");
	strcpy(ReqQryHistoryDeliveryField->BegDate, "20200101");
	strcpy(ReqQryHistoryDeliveryField->EndDate, "20200801");
	ReqQryHistoryDeliveryField->OrderType = QCVD_ORDST_DESC;
	strcpy(ReqQryHistoryDeliveryField->EntrustType, "1|2|3|");
	qrycmd ReqQryHistoryDeliveryFieldCmd(typeid(CQCVDReqQryHistoryDeliveryField).name(), ReqQryHistoryDeliveryField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryDeliveryFieldCmd);

	CQCVDReqQryHistoryFundDetailField* ReqQryHistoryFundDetailField = new CQCVDReqQryHistoryFundDetailField();
	memset(ReqQryHistoryFundDetailField, 0, sizeof(CQCVDReqQryHistoryFundDetailField));
	strcpy(ReqQryHistoryFundDetailField->BegDate, "20200610");
	strcpy(ReqQryHistoryFundDetailField->EndDate, "20200625");
	ReqQryHistoryFundDetailField->OrderType = QCVD_ORDST_DESC;
	///strcpy(ReqQryHistoryFundDetailField->BusinessAccount, "10113|10213");
	qrycmd ReqQryHistoryFundDetailFieldCmd(typeid(CQCVDReqQryHistoryFundDetailField).name(), ReqQryHistoryFundDetailField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryFundDetailFieldCmd);

	CQCVDReqQryHistoryHoldField* ReqQryHistoryHoldField = new CQCVDReqQryHistoryHoldField();
	memset(ReqQryHistoryHoldField, 0, sizeof(CQCVDReqQryHistoryHoldField));
	/////strcpy(ReqQryHistoryHoldField->SecurityID, "000002");
	strcpy(ReqQryHistoryHoldField->BegDate, "20200610");
	strcpy(ReqQryHistoryHoldField->EndDate, "20200801");
	ReqQryHistoryHoldField->OrderType = QCVD_ORDST_DESC;
	qrycmd ReqQryHistoryHoldFieldCmd(typeid(CQCVDReqQryHistoryHoldField).name(), ReqQryHistoryHoldField);
	qrycmdmgr::GetInstance()->AddQry(ReqQryHistoryHoldFieldCmd);

	CQCVDQryFreeFloatSharesInfoField* QryFreeFloatSharesInfoField = new CQCVDQryFreeFloatSharesInfoField();
	memset(QryFreeFloatSharesInfoField, 0, sizeof(CQCVDQryFreeFloatSharesInfoField));
	QryFreeFloatSharesInfoField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(QryFreeFloatSharesInfoField->SecurityID, "000002");
	strcpy(QryFreeFloatSharesInfoField->BegDate, "20150101");
	strcpy(QryFreeFloatSharesInfoField->EndDate, "20200801");
	qrycmd QryFreeFloatSharesInfoFieldCmd(typeid(CQCVDQryFreeFloatSharesInfoField).name(), QryFreeFloatSharesInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryFreeFloatSharesInfoFieldCmd);

	CQCVDQryCBondConvPriceField* pQryCBondConvPriceField = new CQCVDQryCBondConvPriceField();
	memset(pQryCBondConvPriceField, 0, sizeof(CQCVDQryCBondConvPriceField));
	strcpy(pQryCBondConvPriceField->BegDate, "20201101");
	strcpy(pQryCBondConvPriceField->EndDate, "20210101");
	pQryCBondConvPriceField->ExchangeID = QCVD_EXD_SZSE;
	strcpy(pQryCBondConvPriceField->SecurityID, "123018");
	pQryCBondConvPriceField->OrderType = '1';
	qrycmd QryCBondConvPriceFieldCmd(typeid(CQCVDQryCBondConvPriceField).name(), pQryCBondConvPriceField);
	qrycmdmgr::GetInstance()->AddQry(QryCBondConvPriceFieldCmd);


	CQCVDQryChinaClosedFundField* pQryChinaClosedFundField = new CQCVDQryChinaClosedFundField();
	memset(pQryChinaClosedFundField, 0, sizeof(CQCVDQryChinaClosedFundField));
	strcpy(pQryChinaClosedFundField->BegDate, "20201101");
	strcpy(pQryChinaClosedFundField->EndDate, "20210101");
	strcpy(pQryChinaClosedFundField->FundID, "159957");
	pQryChinaClosedFundField->OrderType = '1';
	qrycmd QryChinaClosedFundFieldCmd(typeid(CQCVDQryChinaClosedFundField).name(), pQryChinaClosedFundField);
	qrycmdmgr::GetInstance()->AddQry(QryChinaClosedFundFieldCmd);

	CQCVDQryChinaHedgeFundField* pQryChinaHedgeFundField = new CQCVDQryChinaHedgeFundField();
	memset(pQryChinaHedgeFundField, 0, sizeof(CQCVDQryChinaHedgeFundField));
	strcpy(pQryChinaHedgeFundField->BegDate, "20201101");
	strcpy(pQryChinaHedgeFundField->EndDate, "20210101");
	strcpy(pQryChinaHedgeFundField->FundID, "XT2006183");
	pQryChinaHedgeFundField->OrderType = '1';
	qrycmd QryChinaHedgeFundFieldCmd(typeid(CQCVDQryChinaHedgeFundField).name(), pQryChinaHedgeFundField);
	qrycmdmgr::GetInstance()->AddQry(QryChinaHedgeFundFieldCmd);

	CQCVDQryChinaMutualFundField* pQryChinaMutualFundField = new CQCVDQryChinaMutualFundField();
	memset(pQryChinaMutualFundField, 0, sizeof(CQCVDQryChinaMutualFundField));
	strcpy(pQryChinaMutualFundField->BegDate, "20201101");
	strcpy(pQryChinaMutualFundField->EndDate, "20210101");
	strcpy(pQryChinaMutualFundField->FundID, "007721");
	pQryChinaMutualFundField->OrderType = '2';
	qrycmd QryChinaMutualFundFieldCmd(typeid(CQCVDQryChinaMutualFundField).name(), pQryChinaMutualFundField);
	qrycmdmgr::GetInstance()->AddQry(QryChinaMutualFundFieldCmd);

	CQCVDQryChinaOptionEodPricesField* pQryChinaOptionEodPricesField = new CQCVDQryChinaOptionEodPricesField();
	memset(pQryChinaOptionEodPricesField, 0, sizeof(CQCVDQryChinaOptionEodPricesField));
	strcpy(pQryChinaOptionEodPricesField->BegDate, "20201101");
	strcpy(pQryChinaOptionEodPricesField->EndDate, "20210101");
	pQryChinaOptionEodPricesField->ExchangeID = '1';
	strcpy(pQryChinaOptionEodPricesField->ExchangeCode, "CFE");
	strcpy(pQryChinaOptionEodPricesField->TradeCode, "IO2012-P-3500");
	pQryChinaOptionEodPricesField->OrderType = '1';
	qrycmd QryChinaOptionEodPricesFieldCmd(typeid(CQCVDQryChinaOptionEodPricesField).name(), pQryChinaOptionEodPricesField);
	qrycmdmgr::GetInstance()->AddQry(QryChinaOptionEodPricesFieldCmd);

	CQCVDQryCindexfutureseodPricesField* pQryCindexfutureseodPricesField = new CQCVDQryCindexfutureseodPricesField();
	memset(pQryCindexfutureseodPricesField, 0, sizeof(CQCVDQryCindexfutureseodPricesField));
	strcpy(pQryCindexfutureseodPricesField->BegDate, "20201101");
	strcpy(pQryCindexfutureseodPricesField->EndDate, "20210101");
	pQryCindexfutureseodPricesField->ExchangeID = '1';
	strcpy(pQryCindexfutureseodPricesField->ExchangeCode, "CFE");
	strcpy(pQryCindexfutureseodPricesField->TradeCode, "IC2012");
	pQryCindexfutureseodPricesField->OrderType = '1';
	qrycmd QryCindexfutureseodPricesFieldCmd(typeid(CQCVDQryCindexfutureseodPricesField).name(), pQryCindexfutureseodPricesField);
	qrycmdmgr::GetInstance()->AddQry(QryCindexfutureseodPricesFieldCmd);

	CQCVDQryCMoneyMarketDailyFIncomeField* pQryCMoneyMarketDailyFIncomeField = new CQCVDQryCMoneyMarketDailyFIncomeField();
	memset(pQryCMoneyMarketDailyFIncomeField, 0, sizeof(CQCVDQryCMoneyMarketDailyFIncomeField));
	strcpy(pQryCMoneyMarketDailyFIncomeField->BegDate, "20201101");
	strcpy(pQryCMoneyMarketDailyFIncomeField->EndDate, "20210101");
	strcpy(pQryCMoneyMarketDailyFIncomeField->TradeCode, "511860");
	pQryCMoneyMarketDailyFIncomeField->OrderType = '1';
	qrycmd QryCMoneyMarketDailyFIncomeFieldCmd(typeid(CQCVDQryCMoneyMarketDailyFIncomeField).name(), pQryCMoneyMarketDailyFIncomeField);
	qrycmdmgr::GetInstance()->AddQry(QryCMoneyMarketDailyFIncomeFieldCmd);

	CQCVDQryCommodityFuturesPriceField* pQryCommodityFuturesPriceField = new CQCVDQryCommodityFuturesPriceField();
	memset(pQryCommodityFuturesPriceField, 0, sizeof(CQCVDQryCommodityFuturesPriceField));
	strcpy(pQryCommodityFuturesPriceField->BegDate, "20201101");
	strcpy(pQryCommodityFuturesPriceField->EndDate, "20210101");
	pQryCommodityFuturesPriceField->ExchangeID = '1';
	strcpy(pQryCommodityFuturesPriceField->ExchangeCode, "DCE");
	strcpy(pQryCommodityFuturesPriceField->TradeCode, "JM2101");
	pQryCommodityFuturesPriceField->OrderType = '2';
	qrycmd QryCommodityFuturesPriceFieldCmd(typeid(CQCVDQryCommodityFuturesPriceField).name(), pQryCommodityFuturesPriceField);
	qrycmdmgr::GetInstance()->AddQry(QryCommodityFuturesPriceFieldCmd);

	CQCVDQryGoldSpotPricesField* pQryGoldSpotPricesField = new CQCVDQryGoldSpotPricesField();
	memset(pQryGoldSpotPricesField, 0, sizeof(CQCVDQryGoldSpotPricesField));
	strcpy(pQryGoldSpotPricesField->BegDate, "20201101");
	strcpy(pQryGoldSpotPricesField->EndDate, "20210101");
	pQryGoldSpotPricesField->ExchangeID = '1';
	strcpy(pQryGoldSpotPricesField->ExchangeCode, "SGE");
	strcpy(pQryGoldSpotPricesField->TradeCode, "AUXCNY2WF");
	pQryGoldSpotPricesField->OrderType = '1';
	qrycmd QryGoldSpotPricesFieldCmd(typeid(CQCVDQryGoldSpotPricesField).name(), pQryGoldSpotPricesField);
	qrycmdmgr::GetInstance()->AddQry(QryGoldSpotPricesFieldCmd);



}
void CQCVDSimpleSpi::ReqQryReportInfos()
{

	CQCVDReqInquiryJZFundField*  ReqInquiryJZFundField = new CQCVDReqInquiryJZFundField();
	::memset(ReqInquiryJZFundField, 0, sizeof(CQCVDReqInquiryJZFundField));
	qrycmd ReqInquiryJZFundFieldCmd(typeid(CQCVDReqInquiryJZFundField).name(), ReqInquiryJZFundField);
	qrycmdmgr::GetInstance()->AddQry(ReqInquiryJZFundFieldCmd);

	//CQCVDReqInquiryBankAccountFundField*  ReqInquiryBankAccountFundField = new CQCVDReqInquiryBankAccountFundField();
	//::memset(ReqInquiryBankAccountFundField, 0, sizeof(CQCVDReqInquiryBankAccountFundField));
	//::strcpy(ReqInquiryBankAccountFundField->BankPassword,"");
	//qrycmd ReqInquiryBankAccountFundFieldCmd(typeid(CQCVDReqInquiryBankAccountFundField).name(), ReqInquiryBankAccountFundField);
	//qrycmdmgr::GetInstance()->AddQry(ReqInquiryBankAccountFundFieldCmd);

	CQCVDQryIPOInfoField*  QryIPOInfoField = new CQCVDQryIPOInfoField();
	memset(QryIPOInfoField, 0, sizeof(CQCVDQryIPOInfoField));
	qrycmd QryIPOInfoFieldCmd(typeid(CQCVDQryIPOInfoField).name(), QryIPOInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryIPOInfoFieldCmd);

	CQCVDQryInvestorField*  QryInvestorField = new CQCVDQryInvestorField();
	memset(QryInvestorField, 0, sizeof(CQCVDQryInvestorField));
	qrycmd QryInvestorFieldCmd(typeid(CQCVDQryInvestorField).name(), QryInvestorField);
	qrycmdmgr::GetInstance()->AddQry(QryInvestorFieldCmd);

	CQCVDQryShareholderAccountField*  QryShareholderAccountField = new CQCVDQryShareholderAccountField();
	memset(QryShareholderAccountField, 0, sizeof(CQCVDQryShareholderAccountField));
	qrycmd QryShareholderAccountFieldCmd(typeid(CQCVDQryShareholderAccountField).name(), QryShareholderAccountField);
	qrycmdmgr::GetInstance()->AddQry(QryShareholderAccountFieldCmd);

	CQCVDQryOrderField*  QryOrderField = new CQCVDQryOrderField();
	memset(QryOrderField, 0, sizeof(CQCVDQryOrderField));
	qrycmd QryOrderFieldCmd(typeid(CQCVDQryOrderField).name(), QryOrderField);
	qrycmdmgr::GetInstance()->AddQry(QryOrderFieldCmd);

	CQCVDQryOrderActionField*  QryOrderActionField = new CQCVDQryOrderActionField();
	memset(QryOrderActionField, 0, sizeof(CQCVDQryOrderActionField));
	qrycmd QryOrderActionFieldCmd(typeid(CQCVDQryOrderActionField).name(), QryOrderActionField);
	qrycmdmgr::GetInstance()->AddQry(QryOrderActionFieldCmd);

	CQCVDQryTradeField*  QryTradeField = new CQCVDQryTradeField();
	memset(QryTradeField, 0, sizeof(CQCVDQryTradeField));
	qrycmd QryTradeFieldCmd(typeid(CQCVDQryTradeField).name(), QryTradeField);
	qrycmdmgr::GetInstance()->AddQry(QryTradeFieldCmd);

	CQCVDQryTradingAccountField*  QryTradingAccountField = new CQCVDQryTradingAccountField();
	memset(QryTradingAccountField, 0, sizeof(CQCVDQryTradingAccountField));
	qrycmd QryTradingAccountFieldCmd(typeid(CQCVDQryTradingAccountField).name(), QryTradingAccountField);
	qrycmdmgr::GetInstance()->AddQry(QryTradingAccountFieldCmd);

	CQCVDQryPositionField*  QryPositionField = new CQCVDQryPositionField();
	memset(QryPositionField, 0, sizeof(CQCVDQryPositionField));
	qrycmd QryPositionFieldCmd(typeid(CQCVDQryPositionField).name(), QryPositionField);
	qrycmdmgr::GetInstance()->AddQry(QryPositionFieldCmd);

	CQCVDQryIPOQuotaField*  QryIPOQuotaField = new CQCVDQryIPOQuotaField();
	memset(QryIPOQuotaField, 0, sizeof(CQCVDQryIPOQuotaField));
	qrycmd QryIPOQuotaFieldCmd(typeid(CQCVDQryIPOQuotaField).name(), QryIPOQuotaField);
	qrycmdmgr::GetInstance()->AddQry(QryIPOQuotaFieldCmd);

	CQCVDQryFundTransferDetailField*  QryFundTransferDetailField = new CQCVDQryFundTransferDetailField();
	memset(QryFundTransferDetailField, 0, sizeof(CQCVDQryFundTransferDetailField));
	qrycmd QryFundTransferDetailFieldCmd(typeid(CQCVDQryFundTransferDetailField).name(), QryFundTransferDetailField);
	qrycmdmgr::GetInstance()->AddQry(QryFundTransferDetailFieldCmd);

	CQCVDQryPositionTransferDetailField*  QryPositionTransferDetailField = new CQCVDQryPositionTransferDetailField();
	memset(QryPositionTransferDetailField, 0, sizeof(CQCVDQryPositionTransferDetailField));
	qrycmd QryPositionTransferDetailFieldCmd(typeid(CQCVDQryPositionTransferDetailField).name(), QryShareholderAccountField);
	qrycmdmgr::GetInstance()->AddQry(QryPositionTransferDetailFieldCmd);

	CQCVDQryPrematurityRepoOrderField*  QryPrematurityRepoOrderField = new CQCVDQryPrematurityRepoOrderField();
	memset(QryPrematurityRepoOrderField, 0, sizeof(CQCVDQryPrematurityRepoOrderField));
	qrycmd QryPrematurityRepoOrderFieldCmd(typeid(CQCVDQryPrematurityRepoOrderField).name(), QryPrematurityRepoOrderField);
	qrycmdmgr::GetInstance()->AddQry(QryPrematurityRepoOrderFieldCmd);

	CQCVDQryPeripheryPositionTransferDetailField*  QryPeripheryPositionTransferDetailField = new CQCVDQryPeripheryPositionTransferDetailField();
	memset(QryPeripheryPositionTransferDetailField, 0, sizeof(CQCVDQryPeripheryPositionTransferDetailField));
	qrycmd QryPeripheryPositionTransferDetailFieldCmd(typeid(CQCVDQryPeripheryPositionTransferDetailField).name(), QryPeripheryPositionTransferDetailField);
	qrycmdmgr::GetInstance()->AddQry(QryPeripheryPositionTransferDetailFieldCmd);

	CQCVDQryCondOrderField*  QryCondOrderField = new CQCVDQryCondOrderField();
	memset(QryCondOrderField, 0, sizeof(CQCVDQryCondOrderField));
	qcvd_set_null(QryCondOrderField->IInfo);
	qcvd_set_null(QryCondOrderField->CondOrderID);
	qrycmd QryCondOrderFieldCmd(typeid(CQCVDQryCondOrderField).name(), QryCondOrderField);
	qrycmdmgr::GetInstance()->AddQry(QryCondOrderFieldCmd);

	CQCVDQryCondOrderActionField*  QryCondOrderActionField = new CQCVDQryCondOrderActionField();
	memset(QryCondOrderActionField, 0, sizeof(CQCVDQryCondOrderActionField));
	qcvd_set_null(QryCondOrderActionField->IInfo);
	qrycmd QryCondOrderActionFieldCmd(typeid(CQCVDQryCondOrderActionField).name(), QryCondOrderActionField);
	qrycmdmgr::GetInstance()->AddQry(QryCondOrderActionFieldCmd);

	CQCVDQryIPONumberResultField*  QryIPONumberResultField = new CQCVDQryIPONumberResultField();
	memset(QryIPONumberResultField, 0, sizeof(CQCVDQryIPONumberResultField));
	qrycmd QryIPONumberResultFieldCmd(typeid(CQCVDQryIPONumberResultField).name(), QryIPONumberResultField);
	qrycmdmgr::GetInstance()->AddQry(QryIPONumberResultFieldCmd);

	CQCVDQryIPOMatchNumberResultField*  QryIPOMatchNumberResultField = new CQCVDQryIPOMatchNumberResultField();
	memset(QryIPOMatchNumberResultField, 0, sizeof(CQCVDQryIPOMatchNumberResultField));
	qrycmd QryIPOMatchNumberResultFieldCmd(typeid(CQCVDQryIPOMatchNumberResultField).name(), QryIPOMatchNumberResultField);
	qrycmdmgr::GetInstance()->AddQry(QryIPOMatchNumberResultFieldCmd);

	CQCVDQryShareholderSpecPrivilegeField*  QryShareholderSpecPrivilegeField = new CQCVDQryShareholderSpecPrivilegeField();
	memset(QryShareholderSpecPrivilegeField, 0, sizeof(CQCVDQryShareholderSpecPrivilegeField));
	qrycmd QryShareholderSpecPrivilegeFieldCmd(typeid(CQCVDQryShareholderSpecPrivilegeField).name(), QryShareholderSpecPrivilegeField);
	qrycmdmgr::GetInstance()->AddQry(QryShareholderSpecPrivilegeFieldCmd);

	CQCVDQryRationalInfoField*  QryRationalInfoField = new CQCVDQryRationalInfoField();
	memset(QryRationalInfoField, 0, sizeof(CQCVDQryRationalInfoField));
	qrycmd QryRationalInfoFieldCmd(typeid(CQCVDQryRationalInfoField).name(), QryRationalInfoField);
	qrycmdmgr::GetInstance()->AddQry(QryRationalInfoFieldCmd);

	CQCVDQryPeripheryFundTransferDetailField*  QryPeripheryFundTransferDetailField = new CQCVDQryPeripheryFundTransferDetailField();
	memset(QryPeripheryFundTransferDetailField, 0, sizeof(CQCVDQryPeripheryFundTransferDetailField));
	qrycmd QryPeripheryFundTransferDetailFieldCmd(typeid(CQCVDQryPeripheryFundTransferDetailField).name(), QryPeripheryFundTransferDetailField);
	qrycmdmgr::GetInstance()->AddQry(QryPeripheryFundTransferDetailFieldCmd);

}

void CQCVDSimpleSpi::ReqAddminOperations()
{

}


void CQCVDSimpleSpi::OnRspUserLogin(CQCVDRspUserLoginField *pRspUserLoginField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUserLogin LogInAccount=%s ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspUserLoginField->LogInAccount, pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
	if (0 == pRspInfo->ErrorID)
	{
		m_is_login.store(true);

		SubscribeMDs();

	//	ReqQryValueAddDatas();
	//	ReqQryReportInfos();

	}
}

//登出应答	
void CQCVDSimpleSpi::OnRspUserLogout(CQCVDUserLogoutField *pUserLogoutField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUserLogout UserID=%s \n", pUserLogoutField->UserID);
	m_is_login.store(false);
}



void CQCVDSimpleSpi::OnRspInquiryJZFund(CQCVDRspInquiryJZFundField *pRspInquiryJZFundField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspInquiryJZFundField) {
		printf("CQCValueAddProSpi OnRspInquiryJZFund IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AccountID=%s CurrencyID=%c UsefulMoney=%f FetchLimit=%f DepartmentID=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspInquiryJZFundField->AccountID, pRspInquiryJZFundField->CurrencyID, pRspInquiryJZFundField->UsefulMoney, pRspInquiryJZFundField->FetchLimit, pRspInquiryJZFundField->DepartmentID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryJZFund End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspInquiryBankAccountFund(CQCVDRspInquiryBankAccountFundField *pRspInquiryBankAccountFundField, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspInquiryBankAccountFundField) {
		printf("CQCValueAddProSpi OnRspInquiryBankAccountFund IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s DepartmentID=%s AccountID=%s CurrencyID=%c BankID=%s BankAccountID=%s Balance=%f   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspInquiryBankAccountFundField->DepartmentID, pRspInquiryBankAccountFundField->AccountID, pRspInquiryBankAccountFundField->CurrencyID, char2str(pRspInquiryBankAccountFundField->BankID).c_str(), pRspInquiryBankAccountFundField->BankAccountID, pRspInquiryBankAccountFundField->Balance);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryBankAccountFund End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryIPOInfo(CQCVDIPOInfoField *pIPOInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pIPOInfo) {
		printf("CQCValueAddProSpi OnRspQryIPOInfo IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s MarketID=%s ProductID=%s SecurityType=%s CurrencyID=%c SecurityName=%s UnderlyingSecurityID=%s UnderlyingSecurityName=%s IssueMode=%s TradingDay=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pIPOInfo->ExchangeID).c_str(), pIPOInfo->SecurityID, char2str(pIPOInfo->MarketID).c_str(), char2str(pIPOInfo->ProductID).c_str(), char2str(pIPOInfo->SecurityType).c_str(), pIPOInfo->CurrencyID, pIPOInfo->SecurityName, pIPOInfo->UnderlyingSecurityID, pIPOInfo->UnderlyingSecurityName, char2str(pIPOInfo->IssueMode).c_str(), pIPOInfo->TradingDay);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryIPOInfo End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryInvestor(CQCVDInvestorField *pInvestor, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pInvestor) {
		printf("CQCValueAddProSpi OnRspQryInvestor IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s InvestorName=%s IdCardType=%s IdCardNo=%s Telephone=%s Address=%s OpenDate=%s Mobile=%s Operways=%s ProfInvestorType=%s DepartmentID=%s IsActive=%d LoginLimit=%d   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pInvestor->InvestorID, pInvestor->InvestorName, char2str(pInvestor->IdCardType).c_str(), pInvestor->IdCardNo, pInvestor->Telephone, pInvestor->Address, pInvestor->OpenDate, pInvestor->Mobile, pInvestor->Operways, char2str(pInvestor->ProfInvestorType).c_str(), pInvestor->DepartmentID, pInvestor->IsActive, pInvestor->LoginLimit);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryInvestor End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryShareholderAccount(CQCVDShareholderAccountField *pShareholderAccount, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pShareholderAccount) {
		printf("CQCValueAddProSpi OnRspQryShareholderAccount IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s ExchangeID=%s ShareholderID=%s MarketID=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pShareholderAccount->InvestorID, char2str(pShareholderAccount->ExchangeID).c_str(), pShareholderAccount->ShareholderID, char2str(pShareholderAccount->MarketID).c_str());
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryShareholderAccount End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryOrder(CQCVDOrderField *pOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pOrder) {
		printf("CQCValueAddProSpi OnRspQryOrder IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s SecurityID=%s OrderRef=%d UserID=%s OrderPriceType=%s Direction=%s LimitPrice=%f VolumeTotalOriginal=%d TimeCondition=%s VolumeCondition=%s RequestID=%d OrderLocalID=%s ExchangeID=%s MarketID=%s ShareholderID=%s ExchangeInstID=%s TraderID=%s OrderSubmitStatus=%s TradingDay=%s OrderSysID=%s OrderStatus=%s OrderType=%s VolumeTraded=%d VolumeTotal=%d InsertDate=%s InsertTime=%s CancelTime=%s FrontID=%d SessionID=%d UserProductInfo=%s StatusMsg=%s BusinessUnitID=%s AccountID=%s IPAddress=%s MacAddress=%s LotType=%s SInfo=%s IInfo=%d Operway=%s DepartmentID=%s CondCheck=%s Turnover=%f RtnFloatInfo=%f RtnIntInfo=%d GTDate=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pOrder->InvestorID, pOrder->SecurityID, pOrder->OrderRef, pOrder->UserID, char2str(pOrder->OrderPriceType).c_str(), char2str(pOrder->Direction).c_str(), pOrder->LimitPrice, pOrder->VolumeTotalOriginal, char2str(pOrder->TimeCondition).c_str(), char2str(pOrder->VolumeCondition).c_str(), pOrder->RequestID, pOrder->OrderLocalID, char2str(pOrder->ExchangeID).c_str(), char2str(pOrder->MarketID).c_str(), pOrder->ShareholderID, pOrder->ExchangeInstID, pOrder->TraderID, char2str(pOrder->OrderSubmitStatus).c_str(), pOrder->TradingDay, pOrder->OrderSysID, char2str(pOrder->OrderStatus).c_str(), char2str(pOrder->OrderType).c_str(), pOrder->VolumeTraded, pOrder->VolumeTotal, pOrder->InsertDate, pOrder->InsertTime, pOrder->CancelTime, pOrder->FrontID, pOrder->SessionID, pOrder->UserProductInfo, pOrder->StatusMsg, pOrder->BusinessUnitID, pOrder->AccountID, pOrder->IPAddress, pOrder->MacAddress, char2str(pOrder->LotType).c_str(), pOrder->SInfo, pOrder->IInfo, char2str(pOrder->Operway).c_str(), pOrder->DepartmentID, char2str(pOrder->CondCheck).c_str(), pOrder->Turnover, pOrder->RtnFloatInfo, pOrder->RtnIntInfo, pOrder->GTDate);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryOrder End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryOrderAction(CQCVDOrderActionField *pOrderAction, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pOrderAction) {
		printf("CQCValueAddProSpi OnRspQryOrderAction IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s OrderActionRef=%d OrderRef=%d RequestID=%d FrontID=%d SessionID=%d ExchangeID=%s OrderSysID=%s ActionFlag=%s ActionDate=%s ActionTime=%s TraderID=%s OrderLocalID=%s ActionLocalID=%s ShareholderID=%s OrderActionStatus=%s UserID=%s StatusMsg=%s IPAddress=%s MacAddress=%s SInfo=%s IInfo=%d Operway=%s MarketID=%s \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef, pOrderAction->RequestID, pOrderAction->FrontID, pOrderAction->SessionID, char2str(pOrderAction->ExchangeID).c_str(), pOrderAction->OrderSysID, char2str(pOrderAction->ActionFlag).c_str(), pOrderAction->ActionDate, pOrderAction->ActionTime, pOrderAction->TraderID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ShareholderID, char2str(pOrderAction->OrderActionStatus).c_str(), pOrderAction->UserID, pOrderAction->StatusMsg, pOrderAction->IPAddress, pOrderAction->MacAddress, pOrderAction->SInfo, pOrderAction->IInfo, char2str(pOrderAction->Operway).c_str(), char2str(pOrderAction->MarketID).c_str());
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryOrderAction End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryTrade(CQCVDTradeField *pTrade, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pTrade) {
		printf("CQCValueAddProSpi OnRspQryTrade IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s SecurityID=%s UserID=%s ExchangeID=%s TradeID=%s Direction=%s OrderSysID=%s ShareholderID=%s ExchangeInstID=%s Price=%f Volume=%d TradeDate=%s TradeTime=%s TraderID=%s OrderLocalID=%s TradingDay=%s BusinessUnitID=%s AccountID=%s OrderRef=%d DepartmentID=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pTrade->InvestorID, pTrade->SecurityID, pTrade->UserID, char2str(pTrade->ExchangeID).c_str(), pTrade->TradeID, char2str(pTrade->Direction).c_str(), pTrade->OrderSysID, pTrade->ShareholderID, pTrade->ExchangeInstID, pTrade->Price, pTrade->Volume, pTrade->TradeDate, pTrade->TradeTime, pTrade->TraderID, pTrade->OrderLocalID, pTrade->TradingDay, pTrade->BusinessUnitID, pTrade->AccountID, pTrade->OrderRef, pTrade->DepartmentID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryTrade End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryTradingAccount(CQCVDTradingAccountField *pTradingAccount, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pTradingAccount) {
		printf("CQCValueAddProSpi OnRspQryTradingAccount IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AccountID=%s CurrencyID=%c Deposit=%f Withdraw=%f UnDeliveredMoney=%f FrozenCash=%f FrozenCommission=%f PreUnDeliveredMoney=%f Commission=%f AccountType=%s AccountOwner=%s DepartmentID=%s BankID=%s BankAccountID=%s UnDeliveredFrozenCash=%f UnDeliveredFrozenCommission=%f UnDeliveredCommission=%f   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pTradingAccount->AccountID, pTradingAccount->CurrencyID, pTradingAccount->Deposit, pTradingAccount->Withdraw, pTradingAccount->UnDeliveredMoney, pTradingAccount->FrozenCash, pTradingAccount->FrozenCommission, pTradingAccount->PreUnDeliveredMoney, pTradingAccount->Commission, char2str(pTradingAccount->AccountType).c_str(), pTradingAccount->AccountOwner, pTradingAccount->DepartmentID, char2str(pTradingAccount->BankID).c_str(), pTradingAccount->BankAccountID, pTradingAccount->UnDeliveredFrozenCash, pTradingAccount->UnDeliveredFrozenCommission, pTradingAccount->UnDeliveredCommission);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryTradingAccount End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryPosition(CQCVDPositionField *pPosition, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPosition) {
		printf("CQCValueAddProSpi OnRspQryPosition IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SecurityID=%s InvestorID=%s BusinessUnitID=%s ExchangeID=%s MarketID=%s ShareholderID=%s TradingDay=%s HistoryPos=%d HistoryPosFrozen=%d TodayBSPos=%d TodayBSFrozen=%d TodayPRPos=%d TodayPRFrozen=%d TotalPosCost=%f TodaySMPos=%d TodaySMPosFrozen=%d PrePosition=%d AvailablePosition=%d CurrentPosition=%d LastPrice=%f OpenPosCost=%f SecurityName=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPosition->SecurityID, pPosition->InvestorID, pPosition->BusinessUnitID, char2str(pPosition->ExchangeID).c_str(), char2str(pPosition->MarketID).c_str(), pPosition->ShareholderID, pPosition->TradingDay, pPosition->HistoryPos, pPosition->HistoryPosFrozen, pPosition->TodayBSPos, pPosition->TodayBSFrozen, pPosition->TodayPRPos, pPosition->TodayPRFrozen, pPosition->TotalPosCost, pPosition->TodaySMPos, pPosition->TodaySMPosFrozen, pPosition->PrePosition, pPosition->AvailablePosition, pPosition->CurrentPosition, pPosition->LastPrice, pPosition->OpenPosCost, pPosition->SecurityName);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryPosition End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryIPOQuota(CQCVDIPOQuotaField *pIPOQuota, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pIPOQuota) {
		printf("CQCValueAddProSpi OnRspQryIPOQuota IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s ExchangeID=%s MarketID=%s ShareholderID=%s MaxVolume=%lld KCMaxVolume=%lld   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIPOQuota->InvestorID, char2str(pIPOQuota->ExchangeID).c_str(), char2str(pIPOQuota->MarketID).c_str(), pIPOQuota->ShareholderID, pIPOQuota->MaxVolume, pIPOQuota->KCMaxVolume);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryIPOQuota End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryFundTransferDetail(CQCVDFundTransferDetailField *pFundTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pFundTransferDetail) {
		printf("CQCValueAddProSpi OnRspQryFundTransferDetail IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s FundSerial=%d ApplySerial=%d FrontID=%d SessionID=%d AccountID=%s CurrencyID=%c TransferDirection=%s Amount=%f TransferStatus=%s OperateSource=%s OperatorID=%s OperateDate=%s OperateTime=%s StatusMsg=%s DepartmentID=%s BankID=%s BankAccountID=%s IPAddress=%s MacAddress=%s InvestorID=%s ExternalNodeID=%d   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pFundTransferDetail->FundSerial, pFundTransferDetail->ApplySerial, pFundTransferDetail->FrontID, pFundTransferDetail->SessionID, pFundTransferDetail->AccountID, pFundTransferDetail->CurrencyID, char2str(pFundTransferDetail->TransferDirection).c_str(), pFundTransferDetail->Amount, char2str(pFundTransferDetail->TransferStatus).c_str(), char2str(pFundTransferDetail->OperateSource).c_str(), pFundTransferDetail->OperatorID, pFundTransferDetail->OperateDate, pFundTransferDetail->OperateTime, pFundTransferDetail->StatusMsg, pFundTransferDetail->DepartmentID, char2str(pFundTransferDetail->BankID).c_str(), pFundTransferDetail->BankAccountID, pFundTransferDetail->IPAddress, pFundTransferDetail->MacAddress, pFundTransferDetail->InvestorID, pFundTransferDetail->ExternalNodeID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryFundTransferDetail End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryPositionTransferDetail(CQCVDPositionTransferDetailField *pPositionTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPositionTransferDetail) {
		printf("CQCValueAddProSpi OnRspQryPositionTransferDetail IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s PositionSerial=%d ApplySerial=%d FrontID=%d SessionID=%d InvestorID=%s ExchangeID=%s ShareholderID=%s MarketID=%s SecurityID=%s TradingDay=%s TransferDirection=%s TransferPositionType=%s TransferStatus=%s HistoryVolume=%d TodayBSVolume=%d TodayPRVolume=%d OperatorID=%s OperateDate=%s OperateTime=%s BusinessUnitID=%s StatusMsg=%s IPAddress=%s MacAddress=%s ExternalNodeID=%d   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPositionTransferDetail->PositionSerial, pPositionTransferDetail->ApplySerial, pPositionTransferDetail->FrontID, pPositionTransferDetail->SessionID, pPositionTransferDetail->InvestorID, char2str(pPositionTransferDetail->ExchangeID).c_str(), pPositionTransferDetail->ShareholderID, char2str(pPositionTransferDetail->MarketID).c_str(), pPositionTransferDetail->SecurityID, pPositionTransferDetail->TradingDay, char2str(pPositionTransferDetail->TransferDirection).c_str(), char2str(pPositionTransferDetail->TransferPositionType).c_str(), char2str(pPositionTransferDetail->TransferStatus).c_str(), pPositionTransferDetail->HistoryVolume, pPositionTransferDetail->TodayBSVolume, pPositionTransferDetail->TodayPRVolume, pPositionTransferDetail->OperatorID, pPositionTransferDetail->OperateDate, pPositionTransferDetail->OperateTime, pPositionTransferDetail->BusinessUnitID, pPositionTransferDetail->StatusMsg, pPositionTransferDetail->IPAddress, pPositionTransferDetail->MacAddress, pPositionTransferDetail->ExternalNodeID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryPositionTransferDetail End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryPrematurityRepoOrder(CQCVDPrematurityRepoOrderField *pPrematurityRepoOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPrematurityRepoOrder) {
		printf("CQCValueAddProSpi OnRspQryPrematurityRepoOrder IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s MarketID=%s InvestorID=%s ShareholderID=%s BusinessUnitID=%s TradeDay=%s ExpireDay=%s OrderLocalID=%s SecurityID=%s SecurityName=%s ProductID=%s SecurityType=%s Direction=%s VolumeTraded=%d Price=%f Turnover=%f TradeID=%s RepoTotalMoney=%f InterestAmount=%f   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pPrematurityRepoOrder->ExchangeID).c_str(), char2str(pPrematurityRepoOrder->MarketID).c_str(), pPrematurityRepoOrder->InvestorID, pPrematurityRepoOrder->ShareholderID, pPrematurityRepoOrder->BusinessUnitID, pPrematurityRepoOrder->TradeDay, pPrematurityRepoOrder->ExpireDay, pPrematurityRepoOrder->OrderLocalID, pPrematurityRepoOrder->SecurityID, pPrematurityRepoOrder->SecurityName, char2str(pPrematurityRepoOrder->ProductID).c_str(), char2str(pPrematurityRepoOrder->SecurityType).c_str(), char2str(pPrematurityRepoOrder->Direction).c_str(), pPrematurityRepoOrder->VolumeTraded, pPrematurityRepoOrder->Price, pPrematurityRepoOrder->Turnover, pPrematurityRepoOrder->TradeID, pPrematurityRepoOrder->RepoTotalMoney, pPrematurityRepoOrder->InterestAmount);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryPrematurityRepoOrder End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryPeripheryPositionTransferDetail(CQCVDPeripheryPositionTransferDetailField *pPeripheryPositionTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPeripheryPositionTransferDetail) {
		printf("CQCValueAddProSpi OnRspQryPeripheryPositionTransferDetail IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s PositionSerial=%d ApplySerial=%d FrontID=%d SessionID=%d TransferDirection=%s ExchangeID=%s MarketID=%s InvestorID=%s BusinessUnitID=%s ShareholderID=%s SecurityID=%s TodayBSPos=%d TodayPRPos=%d HistoryPos=%d TradingDay=%s TransferReason=%s TransferStatus=%s OperateDate=%s OperateTime=%s RepealDate=%s RepealTime=%s RepealReason=%s StatusMsg=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPeripheryPositionTransferDetail->PositionSerial, pPeripheryPositionTransferDetail->ApplySerial, pPeripheryPositionTransferDetail->FrontID, pPeripheryPositionTransferDetail->SessionID, char2str(pPeripheryPositionTransferDetail->TransferDirection).c_str(), char2str(pPeripheryPositionTransferDetail->ExchangeID).c_str(), char2str(pPeripheryPositionTransferDetail->MarketID).c_str(), pPeripheryPositionTransferDetail->InvestorID, pPeripheryPositionTransferDetail->BusinessUnitID, pPeripheryPositionTransferDetail->ShareholderID, pPeripheryPositionTransferDetail->SecurityID, pPeripheryPositionTransferDetail->TodayBSPos, pPeripheryPositionTransferDetail->TodayPRPos, pPeripheryPositionTransferDetail->HistoryPos, pPeripheryPositionTransferDetail->TradingDay, pPeripheryPositionTransferDetail->TransferReason, char2str(pPeripheryPositionTransferDetail->TransferStatus).c_str(), pPeripheryPositionTransferDetail->OperateDate, pPeripheryPositionTransferDetail->OperateTime, pPeripheryPositionTransferDetail->RepealDate, pPeripheryPositionTransferDetail->RepealTime, pPeripheryPositionTransferDetail->RepealReason, pPeripheryPositionTransferDetail->StatusMsg);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryPeripheryPositionTransferDetail End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryCondOrder(CQCVDCondOrderField *pCondOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pCondOrder) {
		printf("CQCValueAddProSpi OnRspQryCondOrder IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s InvestorID=%s BusinessUnitID=%s ShareholderID=%s SecurityID=%s Direction=%s OrderPriceType=%s TriggerOrderVolumeType=%s TimeCondition=%s VolumeCondition=%s LimitPrice=%f VolumeTotalOriginal=%d CondOrderRef=%d UserID=%s RequestID=%d IPAddress=%s MacAddress=%s CondOrderID=%d SInfo=%s IInfo=%d Operway=%s CondCheck=%s ContingentCondition=%s ConditionPrice=%f PriceTicks=%d VolumeMultiple=%d RelativeFrontID=%d RelativeSessionID=%d RelativeParam=%s AppendContingentCondition=%s AppendConditionPrice=%f AppendRelativeFrontID=%d AppendRelativeSessionID=%d AppendRelativeParam=%s TradingDay=%s CondOrderStatus=%s InsertDate=%s InsertTime=%s CancelTime=%s CancelUser=%s FrontID=%d SessionID=%d UserProductInfo=%s StatusMsg=%s DepartmentID=%s ActiveDate=%s ActiveTime=%s LotType=%s TriggerOrderPriceType=%s TriggerRelativeParam=%s AppendCondParam=%s GTDate=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pCondOrder->ExchangeID).c_str(), pCondOrder->InvestorID, pCondOrder->BusinessUnitID, pCondOrder->ShareholderID, pCondOrder->SecurityID, char2str(pCondOrder->Direction).c_str(), char2str(pCondOrder->OrderPriceType).c_str(), char2str(pCondOrder->TriggerOrderVolumeType).c_str(), char2str(pCondOrder->TimeCondition).c_str(), char2str(pCondOrder->VolumeCondition).c_str(), pCondOrder->LimitPrice, pCondOrder->VolumeTotalOriginal, pCondOrder->CondOrderRef, pCondOrder->UserID, pCondOrder->RequestID, pCondOrder->IPAddress, pCondOrder->MacAddress, pCondOrder->CondOrderID, pCondOrder->SInfo, pCondOrder->IInfo, char2str(pCondOrder->Operway).c_str(), char2str(pCondOrder->CondCheck).c_str(), char2str(pCondOrder->ContingentCondition).c_str(), pCondOrder->ConditionPrice, pCondOrder->PriceTicks, pCondOrder->VolumeMultiple, pCondOrder->RelativeFrontID, pCondOrder->RelativeSessionID, pCondOrder->RelativeParam, char2str(pCondOrder->AppendContingentCondition).c_str(), pCondOrder->AppendConditionPrice, pCondOrder->AppendRelativeFrontID, pCondOrder->AppendRelativeSessionID, pCondOrder->AppendRelativeParam, pCondOrder->TradingDay, char2str(pCondOrder->CondOrderStatus).c_str(), pCondOrder->InsertDate, pCondOrder->InsertTime, pCondOrder->CancelTime, pCondOrder->CancelUser, pCondOrder->FrontID, pCondOrder->SessionID, pCondOrder->UserProductInfo, pCondOrder->StatusMsg, pCondOrder->DepartmentID, pCondOrder->ActiveDate, pCondOrder->ActiveTime, char2str(pCondOrder->LotType).c_str(), char2str(pCondOrder->TriggerOrderPriceType).c_str(), pCondOrder->TriggerRelativeParam, pCondOrder->AppendCondParam, pCondOrder->GTDate);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCondOrder End==========================================================================================================\n ");
		fflush(stdout);
	}
}

void CQCVDSimpleSpi::OnRspQryCondOrderAction(CQCVDCondOrderActionField *pCondOrderAction, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pCondOrderAction) {
		printf("CQCValueAddProSpi OnRspQryCondOrderAction IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s RequestID=%d CondOrderActionRef=%d CondOrderRef=%d FrontID=%d SessionID=%d CondOrderID=%d ActionFlag=%s InvestorID=%s UserID=%s CancelCondOrderID=%d IPAddress=%s MacAddress=%s SInfo=%s IInfo=%d Operway=%s BusinessUnitID=%s ShareholderID=%s ActionDate=%s ActionTime=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pCondOrderAction->ExchangeID).c_str(), pCondOrderAction->RequestID, pCondOrderAction->CondOrderActionRef, pCondOrderAction->CondOrderRef, pCondOrderAction->FrontID, pCondOrderAction->SessionID, pCondOrderAction->CondOrderID, char2str(pCondOrderAction->ActionFlag).c_str(), pCondOrderAction->InvestorID, pCondOrderAction->UserID, pCondOrderAction->CancelCondOrderID, pCondOrderAction->IPAddress, pCondOrderAction->MacAddress, pCondOrderAction->SInfo, pCondOrderAction->IInfo, char2str(pCondOrderAction->Operway).c_str(), pCondOrderAction->BusinessUnitID, pCondOrderAction->ShareholderID, pCondOrderAction->ActionDate, pCondOrderAction->ActionTime);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCondOrderAction End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryIPONumberResult(CQCVDIPONumberResultField *pIPONumberResult, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pIPONumberResult) {
		printf("CQCValueAddProSpi OnRspQryIPONumberResult IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s Day=%s SecurityName=%s ShareholderID=%s SecurityType=%s BeginNumberID=%s Volume=%d   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pIPONumberResult->ExchangeID).c_str(), pIPONumberResult->SecurityID, pIPONumberResult->Day, pIPONumberResult->SecurityName, pIPONumberResult->ShareholderID, char2str(pIPONumberResult->SecurityType).c_str(), pIPONumberResult->BeginNumberID, pIPONumberResult->Volume);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryIPONumberResult End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryIPOMatchNumberResult(CQCVDIPOMatchNumberResultField *pIPOMatchNumberResult, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pIPOMatchNumberResult) {
		printf("CQCValueAddProSpi OnRspQryIPOMatchNumberResult IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s Day=%s SecurityName=%s ShareholderID=%s SecurityType=%s MatchNumberID=%s Volume=%d Price=%f Amout=%f   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pIPOMatchNumberResult->ExchangeID).c_str(), pIPOMatchNumberResult->SecurityID, pIPOMatchNumberResult->Day, pIPOMatchNumberResult->SecurityName, pIPOMatchNumberResult->ShareholderID, char2str(pIPOMatchNumberResult->SecurityType).c_str(), pIPOMatchNumberResult->MatchNumberID, pIPOMatchNumberResult->Volume, pIPOMatchNumberResult->Price, pIPOMatchNumberResult->Amout);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryIPOMatchNumberResult End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryShareholderSpecPrivilege(CQCVDShareholderSpecPrivilegeField *pShareholderSpecPrivilege, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pShareholderSpecPrivilege) {
		printf("CQCValueAddProSpi OnRspQryShareholderSpecPrivilege IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s ShareholderID=%s MarketID=%s SpecPrivilegeType=%s Direction=%s bForbidden=%d InvestorID=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, char2str(pShareholderSpecPrivilege->ExchangeID).c_str(), pShareholderSpecPrivilege->ShareholderID, char2str(pShareholderSpecPrivilege->MarketID).c_str(), char2str(pShareholderSpecPrivilege->SpecPrivilegeType).c_str(), char2str(pShareholderSpecPrivilege->Direction).c_str(), pShareholderSpecPrivilege->bForbidden, pShareholderSpecPrivilege->InvestorID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryShareholderSpecPrivilege End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryRationalInfo(CQCVDRationalInfoField *pRationalInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRationalInfo) {
		printf("CQCValueAddProSpi OnRspQryRationalInfo IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s ExchangeID=%s SecurityID=%s Price=%f MarketID=%s ProductID=%s SecurityType=%s SecurityName=%s UnderlyingSecurityID=%s UnderlyingSecurityName=%s \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRationalInfo->TradingDay, char2str(pRationalInfo->ExchangeID).c_str(), pRationalInfo->SecurityID, pRationalInfo->Price, char2str(pRationalInfo->MarketID).c_str(), char2str(pRationalInfo->ProductID).c_str(), char2str(pRationalInfo->SecurityType).c_str(), pRationalInfo->SecurityName, pRationalInfo->UnderlyingSecurityID, pRationalInfo->UnderlyingSecurityName);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryRationalInfo End==========================================================================================================\n ");
	}
}

void CQCVDSimpleSpi::OnRspQryPeripheryFundTransferDetail(CQCVDPeripheryFundTransferDetailField *pPeripheryFundTransferDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPeripheryFundTransferDetail) {
		printf("CQCValueAddProSpi OnRspQryPeripheryFundTransferDetail IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s FundSerial=%d ApplySerial=%d FrontID=%d SessionID=%d DepartmentID=%s AccountID=%s CurrencyID=%c TransferDirection=%s Amount=%f InvestorID=%s TransferStatus=%s TransferReason=%s OperateDate=%s OperateTime=%s RepealDate=%s RepealTime=%s RepealReason=%s StatusMsg=%s   \n ", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPeripheryFundTransferDetail->FundSerial, pPeripheryFundTransferDetail->ApplySerial, pPeripheryFundTransferDetail->FrontID, pPeripheryFundTransferDetail->SessionID, pPeripheryFundTransferDetail->DepartmentID, pPeripheryFundTransferDetail->AccountID, pPeripheryFundTransferDetail->CurrencyID, char2str(pPeripheryFundTransferDetail->TransferDirection).c_str(), pPeripheryFundTransferDetail->Amount, pPeripheryFundTransferDetail->InvestorID, char2str(pPeripheryFundTransferDetail->TransferStatus).c_str(), pPeripheryFundTransferDetail->TransferReason, pPeripheryFundTransferDetail->OperateDate, pPeripheryFundTransferDetail->OperateTime, pPeripheryFundTransferDetail->RepealDate, pPeripheryFundTransferDetail->RepealTime, pPeripheryFundTransferDetail->RepealReason, pPeripheryFundTransferDetail->StatusMsg);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryPeripheryFundTransferDetail End==========================================================================================================\n ");
	}
}

///查询自由流通股本数据应答
void CQCVDSimpleSpi::OnRspInquiryFreeFloatShares(CQCVDFreeFloatSharesDataField *pFreeFloatSharesData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pFreeFloatSharesData) {
		printf("CQCValueAddProSpi OnRspInquiryFreeFloatShares IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ChangeDateEX=%s AuthMode=%s ChangeDateList=%s ExchangeID=%s FreeShares=%f SecurityID=%s PageLocate=%d\n ",
			bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg,
			pFreeFloatSharesData->AnnouncementDate, pFreeFloatSharesData->ChangeDateEX, pFreeFloatSharesData->ChangeDateList, char2str(pFreeFloatSharesData->ExchangeID).c_str(),
			pFreeFloatSharesData->FreeShares, pFreeFloatSharesData->SecurityID, pFreeFloatSharesData->PageLocate);
	}
	if (NULL == pFreeFloatSharesData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryFreeFloatShares End==========================================================================================================\n ");
	}
}

///查询复权信息应答
void CQCVDSimpleSpi::OnRspInquiryRightsAdjustment(CQCVDRightsAdjustmentDataField *pRightsAdjustmentData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pRightsAdjustmentData) {
		printf("CQCValueAddProSpi OnRspInquiryRightsAdjustment IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s ADJPreClose=%f ADJOpen=%f ADJHigh=%f ADJLow=%f ADJClose=%f ADJFactor=%f ExchangeID=%s SecurityID=%s PageLocate=%d Open=%f High=%f Low=%f Close=%f CrncyCode=%s Change=%f PCTChange=%f Volume=%f Amount=%f AVGPrice=%f TradeStatus=%s \n ",
			bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg,
			pRightsAdjustmentData->TradingDay, pRightsAdjustmentData->ADJPreClose, pRightsAdjustmentData->ADJOpen, pRightsAdjustmentData->ADJHigh, pRightsAdjustmentData->ADJLow,
			pRightsAdjustmentData->ADJClose, pRightsAdjustmentData->ADJFactor, char2str(pRightsAdjustmentData->ExchangeID).c_str(), pRightsAdjustmentData->SecurityID, pRightsAdjustmentData->PageLocate,
			pRightsAdjustmentData->Open, pRightsAdjustmentData->High, pRightsAdjustmentData->Low, pRightsAdjustmentData->Close, pRightsAdjustmentData->CrncyCode,
			pRightsAdjustmentData->Change, pRightsAdjustmentData->PCTChange, pRightsAdjustmentData->Volume, pRightsAdjustmentData->Amount, pRightsAdjustmentData->AVGPrice,
			pRightsAdjustmentData->TradeStatus);
	}
	if (NULL == pRightsAdjustmentData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryRightsAdjustment End==========================================================================================================\n ");
	}
}

//查询历史资金流向数据应答
void CQCVDSimpleSpi::OnRspInquiryHistoryFundsFlowData(CQCVDHistoryFundsFlowDataField *pHistoryFundsFlowData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryFundsFlowData)
	{
		std::string pHistoryFundsFlowData_ExchangeID_str = char2str(pHistoryFundsFlowData->ExchangeID);
		const char*  pHistoryFundsFlowData_ExchangeID_str_ptr = pHistoryFundsFlowData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryHistoryFundsFlowData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s BuyValueExlargeOrder=%f SellValueExlargeOrder=%f BuyValueLargeOrder=%f SellValueLargeOrder=%f BuyValueMedOrder=%f SellValueMedOrder=%f BuyValueSmallOrder=%f SellValueSmallOrder=%f BuyVolumeExlargeOrder=%f SellVolumeExlargeOrder=%f BuyVolumeLargeOrder=%f SellVolumeLargeOrder=%f BuyVolumeMedOrder=%f SellVolumeMedOrder=%f BuyVolumeSmallOrder=%f SellVolumeSmallOrder=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryFundsFlowData->TradingDay, pHistoryFundsFlowData->BuyValueExlargeOrder, pHistoryFundsFlowData->SellValueExlargeOrder, pHistoryFundsFlowData->BuyValueLargeOrder, pHistoryFundsFlowData->SellValueLargeOrder, pHistoryFundsFlowData->BuyValueMedOrder, pHistoryFundsFlowData->SellValueMedOrder, pHistoryFundsFlowData->BuyValueSmallOrder, pHistoryFundsFlowData->SellValueSmallOrder, pHistoryFundsFlowData->BuyVolumeExlargeOrder, pHistoryFundsFlowData->SellVolumeExlargeOrder, pHistoryFundsFlowData->BuyVolumeLargeOrder, pHistoryFundsFlowData->SellVolumeLargeOrder, pHistoryFundsFlowData->BuyVolumeMedOrder, pHistoryFundsFlowData->SellVolumeMedOrder, pHistoryFundsFlowData->BuyVolumeSmallOrder, pHistoryFundsFlowData->SellVolumeSmallOrder, pHistoryFundsFlowData_ExchangeID_str_ptr, pHistoryFundsFlowData->SecurityID, pHistoryFundsFlowData->PageLocate);
	}
	if (NULL == pHistoryFundsFlowData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryFundsFlowData End==========================================================================================================\n ");
	}
}
//查询财务指标信息应答
void CQCVDSimpleSpi::OnRspInquiryFinancialIndicatorData(CQCVDFinancialIndicatorDataField *pFinancialIndicatorData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pFinancialIndicatorData)
	{
		std::string pFinancialIndicatorData_ExchangeID_str = char2str(pFinancialIndicatorData->ExchangeID);
		const char*  pFinancialIndicatorData_ExchangeID_str_ptr = pFinancialIndicatorData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryFinancialIndicatorData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AnnouncementDate=%s ReportPeriod=%s EPSBasic=%f BPS=%f SurplusCapitalPS=%f UndistributedPS=%f OCFPS=%f ROE=%f ROA=%f ORPS=%f TotOperRev=%f OperProfit=%f TotProfit=%f NetProfitInclMinIntInc=%f NetProfitExclMinIntInc=%f Tot_Assets=%f Tot_Liab=%f MinorityInt=%f TotShrhldrEqyExclMinInt=%f TotShrhldrEqyInclMinInt=%f TotLiabShrhldrEqy=%f NetCashFlowsOperAct=%f NetCashFlowsInvAct=%f NetCashFlowsFncAct=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pFinancialIndicatorData->AnnouncementDate, pFinancialIndicatorData->ReportPeriod, pFinancialIndicatorData->EPSBasic, pFinancialIndicatorData->BPS, pFinancialIndicatorData->SurplusCapitalPS, pFinancialIndicatorData->UndistributedPS, pFinancialIndicatorData->OCFPS, pFinancialIndicatorData->ROE, pFinancialIndicatorData->ROA, pFinancialIndicatorData->ORPS, pFinancialIndicatorData->TotOperRev, pFinancialIndicatorData->OperProfit, pFinancialIndicatorData->TotProfit, pFinancialIndicatorData->NetProfitInclMinIntInc, pFinancialIndicatorData->NetProfitExclMinIntInc, pFinancialIndicatorData->Tot_Assets, pFinancialIndicatorData->Tot_Liab, pFinancialIndicatorData->MinorityInt, pFinancialIndicatorData->TotShrhldrEqyExclMinInt, pFinancialIndicatorData->TotShrhldrEqyInclMinInt, pFinancialIndicatorData->TotLiabShrhldrEqy, pFinancialIndicatorData->NetCashFlowsOperAct, pFinancialIndicatorData->NetCashFlowsInvAct, pFinancialIndicatorData->NetCashFlowsFncAct, pFinancialIndicatorData_ExchangeID_str_ptr, pFinancialIndicatorData->SecurityID, pFinancialIndicatorData->PageLocate);
	}
	if (NULL == pFinancialIndicatorData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryFinancialIndicatorData End==========================================================================================================\n ");
	}
}
//查询分红信息应答
void CQCVDSimpleSpi::OnRspInquiryDividendData(CQCVDDividendDataField *pDividendData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pDividendData)
	{
		std::string pDividendData_ExchangeID_str = char2str(pDividendData->ExchangeID);
		const char*  pDividendData_ExchangeID_str_ptr = pDividendData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryDividendData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AnnouncementDate=%s Progress=%s ExDate=%s STKDvdPerSh=%f CashDvdPerShPreTax=%f CashDvdPerShAfterTax=%f EqyRecordDate=%s DvdPayoutDate=%s ListingDateOfDvdShr=%s PrelanDate=%s SMTGDate=%s DvdAnnDate=%s BaseDate=%s BaseShare=%f CrncyCode=%s IsChanged=%d ReportPeriod=%s Change=%s BonusRate=%f ConversedRate=%f Memo=%s PreAnnDate=%s DivObject=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pDividendData->AnnouncementDate, pDividendData->Progress, pDividendData->ExDate, pDividendData->STKDvdPerSh, pDividendData->CashDvdPerShPreTax, pDividendData->CashDvdPerShAfterTax, pDividendData->EqyRecordDate, pDividendData->DvdPayoutDate, pDividendData->ListingDateOfDvdShr, pDividendData->PrelanDate, pDividendData->SMTGDate, pDividendData->DvdAnnDate, pDividendData->BaseDate, pDividendData->BaseShare, pDividendData->CrncyCode, pDividendData->IsChanged, pDividendData->ReportPeriod, pDividendData->Change, pDividendData->BonusRate, pDividendData->ConversedRate, pDividendData->Memo, pDividendData->PreAnnDate, pDividendData->DivObject, pDividendData_ExchangeID_str_ptr, pDividendData->SecurityID, pDividendData->PageLocate);
	}
	if (NULL == pDividendData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryDividendData End==========================================================================================================\n ");
	}
}
//查询送股配股信息应答
void CQCVDSimpleSpi::OnRspInquiryRightIssueData(CQCVDRightIssueDataField *pRightIssueData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pRightIssueData)
	{
		std::string pRightIssueData_ExchangeID_str = char2str(pRightIssueData->ExchangeID);
		const char*  pRightIssueData_ExchangeID_str_ptr = pRightIssueData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryRightIssueData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AnnouncementDate=%s Progress=%s Price=%f Ratio=%f Amount=%f AmountAct=%f NetCollection=%f RegDateShare=%s ExDividendDate=%s ListedDate=%s PayStartDate=%s PayEndDate=%s PrePlanDate=%s SMTGAnnceDate=%s PassDate=%s ApprovedDate=%s AnnceDate=%s ResultDate=%s ListAnnDate=%s Guarantor=%s Guartype=%f Code=%s Year=%s Content=%s Name=%s RatioDenominator=%f RatioMolecular=%f SubscriptionMethod=%s ExpectedFundRaising=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRightIssueData->AnnouncementDate, pRightIssueData->Progress, pRightIssueData->Price, pRightIssueData->Ratio, pRightIssueData->Amount, pRightIssueData->AmountAct, pRightIssueData->NetCollection, pRightIssueData->RegDateShare, pRightIssueData->ExDividendDate, pRightIssueData->ListedDate, pRightIssueData->PayStartDate, pRightIssueData->PayEndDate, pRightIssueData->PrePlanDate, pRightIssueData->SMTGAnnceDate, pRightIssueData->PassDate, pRightIssueData->ApprovedDate, pRightIssueData->AnnceDate, pRightIssueData->ResultDate, pRightIssueData->ListAnnDate, pRightIssueData->Guarantor, pRightIssueData->Guartype, pRightIssueData->Code, pRightIssueData->Year, pRightIssueData->Content, pRightIssueData->Name, pRightIssueData->RatioDenominator, pRightIssueData->RatioMolecular, pRightIssueData->SubscriptionMethod, pRightIssueData->ExpectedFundRaising, pRightIssueData_ExchangeID_str_ptr, pRightIssueData->SecurityID, pRightIssueData->PageLocate);
	}
	if (NULL == pRightIssueData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryRightIssueData End==========================================================================================================\n ");
	}
}
//查询公司资料信息应答
void CQCVDSimpleSpi::OnRspInquiryCompanyDescriptionData(CQCVDCompanyDescriptionDataField *pCompanyDescriptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCompanyDescriptionData)
	{
		std::string pCompanyDescriptionData_ExchangeID_str = char2str(pCompanyDescriptionData->ExchangeID);
		const char*  pCompanyDescriptionData_ExchangeID_str_ptr = pCompanyDescriptionData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryCompanyDescriptionData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s CompName=%s ListDate=%s ChineseIntroduction=%s BusinessScope=%s Office=%s IPOPrice=%f IPOAmount=%f AMTByPlacing=%f AMTToJur=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCompanyDescriptionData->CompName, pCompanyDescriptionData->ListDate, pCompanyDescriptionData->ChineseIntroduction, pCompanyDescriptionData->BusinessScope, pCompanyDescriptionData->Office, pCompanyDescriptionData->IPOPrice, pCompanyDescriptionData->IPOAmount, pCompanyDescriptionData->AMTByPlacing, pCompanyDescriptionData->AMTToJur, pCompanyDescriptionData_ExchangeID_str_ptr, pCompanyDescriptionData->SecurityID, pCompanyDescriptionData->PageLocate);
	}
	if (NULL == pCompanyDescriptionData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryCompanyDescriptionData End==========================================================================================================\n ");
	}
}
//查询股本结构信息应答
void CQCVDSimpleSpi::OnRspInquiryEquityStructureData(CQCVDEquityStructureDataField *pEquityStructureData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pEquityStructureData)
	{
		std::string pEquityStructureData_ExchangeID_str = char2str(pEquityStructureData->ExchangeID);
		const char*  pEquityStructureData_ExchangeID_str_ptr = pEquityStructureData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryEquityStructureData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s TotalShareToday=%f FloatShareToday=%f HolderTotalNum=%f AnnouncementDate=%s HolderEndDate=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pEquityStructureData->TradingDay, pEquityStructureData->TotalShareToday, pEquityStructureData->FloatShareToday, pEquityStructureData->HolderTotalNum, pEquityStructureData->AnnouncementDate, pEquityStructureData->HolderEndDate, pEquityStructureData_ExchangeID_str_ptr, pEquityStructureData->SecurityID, pEquityStructureData->PageLocate);
	}
	if (NULL == pEquityStructureData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryEquityStructureData End==========================================================================================================\n ");
	}
}
//查询主营业务信息应答
void CQCVDSimpleSpi::OnRspInquirySalesSegmentData(CQCVDSalesSegmentDataField *pSalesSegmentData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pSalesSegmentData)
	{
		std::string pSalesSegmentData_ExchangeID_str = char2str(pSalesSegmentData->ExchangeID);
		const char*  pSalesSegmentData_ExchangeID_str_ptr = pSalesSegmentData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquirySalesSegmentData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ReportPeriod=%s CrncyCode=%s ItemCode=%d Item=%s Sales=%f Profit=%f Cost=%f IsPublishedValue=%d ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pSalesSegmentData->ReportPeriod, pSalesSegmentData->CrncyCode, pSalesSegmentData->ItemCode, pSalesSegmentData->Item, pSalesSegmentData->Sales, pSalesSegmentData->Profit, pSalesSegmentData->Cost, pSalesSegmentData->IsPublishedValue, pSalesSegmentData_ExchangeID_str_ptr, pSalesSegmentData->SecurityID, pSalesSegmentData->PageLocate);
	}
	if (NULL == pSalesSegmentData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquirySalesSegmentData End==========================================================================================================\n ");
	}
}
//查询十大股东信息应答
void CQCVDSimpleSpi::OnRspInquiryTopTenHoldersData(CQCVDTopTenHoldersDataField *pTopTenHoldersData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pTopTenHoldersData)
	{
		std::string pTopTenHoldersData_ExchangeID_str = char2str(pTopTenHoldersData->ExchangeID);
		const char*  pTopTenHoldersData_ExchangeID_str_ptr = pTopTenHoldersData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryTopTenHoldersData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AnnouncementDate=%s HolderName=%s HolderQuantity=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pTopTenHoldersData->AnnouncementDate, pTopTenHoldersData->HolderName, pTopTenHoldersData->HolderQuantity, pTopTenHoldersData_ExchangeID_str_ptr, pTopTenHoldersData->SecurityID, pTopTenHoldersData->PageLocate);
	}
	if (NULL == pTopTenHoldersData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryTopTenHoldersData End==========================================================================================================\n ");
	}
}
//查询十大流通股东信息应答
void CQCVDSimpleSpi::OnRspInquiryTopTenFloatHoldersData(CQCVDTopTenFloatHoldersDataField *pTopTenFloatHoldersData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pTopTenFloatHoldersData)
	{
		std::string pTopTenFloatHoldersData_ExchangeID_str = char2str(pTopTenFloatHoldersData->ExchangeID);
		const char*  pTopTenFloatHoldersData_ExchangeID_str_ptr = pTopTenFloatHoldersData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryTopTenFloatHoldersData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s AnnouncementDate=%s HolderName=%s HolderQuantity=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pTopTenFloatHoldersData->AnnouncementDate, pTopTenFloatHoldersData->HolderName, pTopTenFloatHoldersData->HolderQuantity, pTopTenFloatHoldersData_ExchangeID_str_ptr, pTopTenFloatHoldersData->SecurityID, pTopTenFloatHoldersData->PageLocate);
	}
	if (NULL == pTopTenFloatHoldersData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryTopTenFloatHoldersData End==========================================================================================================\n ");
	}
}
//查询个股所属行业板块信息应答
void CQCVDSimpleSpi::OnRspInquiryIndustryData(CQCVDIndustryDataField *pIndustryData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pIndustryData)
	{
		std::string pIndustryData_ExchangeID_str = char2str(pIndustryData->ExchangeID);
		const char*  pIndustryData_ExchangeID_str_ptr = pIndustryData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryIndustryData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s EntryDate=%s RemoveDate=%s IndustriesCode=%s IndustriesName=%s LevelNum=%d Used=%d IndustriesAlias=%s Sequence=%d Memo=%s ChineseDfinition=%s IndustriesNameEng=%s IndexCode=%s Name=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIndustryData->EntryDate, pIndustryData->RemoveDate, pIndustryData->IndustriesCode, pIndustryData->IndustriesName, pIndustryData->LevelNum, pIndustryData->Used, pIndustryData->IndustriesAlias, pIndustryData->Sequence, pIndustryData->Memo, pIndustryData->ChineseDfinition, pIndustryData->IndustriesNameEng, pIndustryData->IndexCode, pIndustryData->Name, pIndustryData_ExchangeID_str_ptr, pIndustryData->SecurityID, pIndustryData->PageLocate);
	}
	if (NULL == pIndustryData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryIndustryData End==========================================================================================================\n ");
	}
}
//查询个股所属概念板块信息应答
void CQCVDSimpleSpi::OnRspInquiryConceptionData(CQCVDConceptionDataField *pConceptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pConceptionData)
	{
		std::string pConceptionData_ExchangeID_str = char2str(pConceptionData->ExchangeID);
		const char*  pConceptionData_ExchangeID_str_ptr = pConceptionData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryConceptionData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ConceptionCode=%s ConceptionName=%s EntryDate=%s RemoveDate=%s CurSign=%s IndustriesCode=%s IndustriesName=%s LevelNum=%d Used=%d IndustriesAlias=%s Sequence=%d Memo=%s ChineseDfinition=%s IndustriesNameEng=%s IndexCode=%s Name=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pConceptionData->ConceptionCode, pConceptionData->ConceptionName, pConceptionData->EntryDate, pConceptionData->RemoveDate, pConceptionData->CurSign, pConceptionData->IndustriesCode, pConceptionData->IndustriesName, pConceptionData->LevelNum, pConceptionData->Used, pConceptionData->IndustriesAlias, pConceptionData->Sequence, pConceptionData->Memo, pConceptionData->ChineseDfinition, pConceptionData->IndustriesNameEng, pConceptionData->IndexCode, pConceptionData->Name, pConceptionData_ExchangeID_str_ptr, pConceptionData->SecurityID, pConceptionData->PageLocate);
	}
	if (NULL == pConceptionData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryConceptionData End==========================================================================================================\n ");
	}
}
//查询个股所属地域板块信息应答
void CQCVDSimpleSpi::OnRspInquiryRegionData(CQCVDRegionDataField *pRegionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pRegionData)
	{
		std::string pRegionData_ExchangeID_str = char2str(pRegionData->ExchangeID);
		const char*  pRegionData_ExchangeID_str_ptr = pRegionData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryRegionData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s IndustriesCode=%s IndustriesName=%s LevelNum=%d Used=%d IndustriesAlias=%s Sequence=%d Memo=%s ChineseDfinition=%s IndustriesNameEng=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRegionData->IndustriesCode, pRegionData->IndustriesName, pRegionData->LevelNum, pRegionData->Used, pRegionData->IndustriesAlias, pRegionData->Sequence, pRegionData->Memo, pRegionData->ChineseDfinition, pRegionData->IndustriesNameEng, pRegionData_ExchangeID_str_ptr, pRegionData->SecurityID, pRegionData->PageLocate);
	}
	if (NULL == pRegionData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryRegionData End==========================================================================================================\n ");
	}
}
//查询指数描述信息应答
void CQCVDSimpleSpi::OnRspInquiryIndexDescriptionData(CQCVDIndexDescriptionDataField *pIndexDescriptionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pIndexDescriptionData)
	{

		printf("CQCValueAddProSpi OnRspInquiryIndexDescriptionData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s Name=%s IndustryCode=%s IndustryName=%s IndustryCode2=%s IndustryNameEng=%s IndexID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIndexDescriptionData->Name, pIndexDescriptionData->IndustryCode, pIndexDescriptionData->IndustryName, pIndexDescriptionData->IndustryCode2, pIndexDescriptionData->IndustryNameEng, pIndexDescriptionData->IndexID, pIndexDescriptionData->PageLocate);
	}
	if (NULL == pIndexDescriptionData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryIndexDescriptionData End==========================================================================================================\n ");
	}
}
//查询行业板块成分股信息应答
void CQCVDSimpleSpi::OnRspInquiryIndustryConstituentsData(CQCVDIndustryConstituentsDataField *pIndustryConstituentsData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pIndustryConstituentsData)
	{
		std::string pIndustryConstituentsData_ExchangeID_str = char2str(pIndustryConstituentsData->ExchangeID);
		const char*  pIndustryConstituentsData_ExchangeID_str_ptr = pIndustryConstituentsData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryIndustryConstituentsData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s WindIndCode=%s IndexID=%s EntryDate=%s RemoveDate=%s CurSign=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIndustryConstituentsData->WindIndCode, pIndustryConstituentsData->IndexID, pIndustryConstituentsData->EntryDate, pIndustryConstituentsData->RemoveDate, pIndustryConstituentsData->CurSign, pIndustryConstituentsData_ExchangeID_str_ptr, pIndustryConstituentsData->SecurityID, pIndustryConstituentsData->PageLocate);
	}
	if (NULL == pIndustryConstituentsData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryIndustryConstituentsData End==========================================================================================================\n ");
	}
}
//查询概念板块成分股信息应答
void CQCVDSimpleSpi::OnRspInquiryConceptionConstituentsData(CQCVDConceptionConstituentsDataField *pConceptionConstituentsData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pConceptionConstituentsData)
	{
		std::string pConceptionConstituentsData_ExchangeID_str = char2str(pConceptionConstituentsData->ExchangeID);
		const char*  pConceptionConstituentsData_ExchangeID_str_ptr = pConceptionConstituentsData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryConceptionConstituentsData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s WindSecCode=%s IndexID=%s EntryDate=%s RemoveDate=%s CurSign=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pConceptionConstituentsData->WindSecCode, pConceptionConstituentsData->IndexID, pConceptionConstituentsData->EntryDate, pConceptionConstituentsData->RemoveDate, pConceptionConstituentsData->CurSign, pConceptionConstituentsData_ExchangeID_str_ptr, pConceptionConstituentsData->SecurityID, pConceptionConstituentsData->PageLocate);
	}
	if (NULL == pConceptionConstituentsData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryConceptionConstituentsData End==========================================================================================================\n ");
	}
}
//查询板块指数实时行情信息应答
void CQCVDSimpleSpi::OnRspInquiryIndustryCodeListData(CQCVDIndustryCodeListDataField *pIndustryCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pIndustryCodeListData)
	{

		printf("CQCValueAddProSpi OnRspInquiryIndustryCodeListData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ChineseDfinition=%s TradingDay=%s PreClosePoint=%f IndexID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIndustryCodeListData->ChineseDfinition, pIndustryCodeListData->TradingDay, pIndustryCodeListData->PreClosePoint, pIndustryCodeListData->IndexID, pIndustryCodeListData->PageLocate);
	}
	if (NULL == pIndustryCodeListData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryIndustryCodeListData End==========================================================================================================\n ");
	}
}
//查询概念指数实时行情信息应答
void CQCVDSimpleSpi::OnRspInquiryConceptionCodeListData(CQCVDConceptionCodeListDataField *pConceptionCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pConceptionCodeListData)
	{

		printf("CQCValueAddProSpi OnRspInquiryConceptionCodeListData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ChineseDfinition=%s TradingDay=%s PreClosePoint=%f IndexID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pConceptionCodeListData->ChineseDfinition, pConceptionCodeListData->TradingDay, pConceptionCodeListData->PreClosePoint, pConceptionCodeListData->IndexID, pConceptionCodeListData->PageLocate);
	}
	if (NULL == pConceptionCodeListData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryConceptionCodeListData End==========================================================================================================\n ");
	}
}
//查询地域版块代码列表应答
void CQCVDSimpleSpi::OnRspInquiryRegionCodeListData(CQCVDRegionCodeListDataField *pRegionCodeListData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pRegionCodeListData)
	{

		printf("CQCValueAddProSpi OnRspInquiryRegionCodeListData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ChineseDfinition=%s IndexID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRegionCodeListData->ChineseDfinition, pRegionCodeListData->IndexID, pRegionCodeListData->PageLocate);
	}
	if (NULL == pRegionCodeListData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryRegionCodeListData End==========================================================================================================\n ");
	}
}

//查询申万指数行情应答
void CQCVDSimpleSpi::OnRspInquirySWSIndexData(CQCVDSWSIndexDataField *pSWSIndexData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pSWSIndexData)
	{

		printf("CQCValueAddProSpi OnRspInquirySWSIndexData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s IndexID=%s IndexName=%s TradingDay=%s PreClosePrice=%f OpenPrice=%f HighPrice=%f LowPrice=%f ClosePrice=%f Volume=%f Amount=%f PERatio=%f PBRatio=%f FloatMV=%f TotalMV=%f PageLocate=%d PageTotal=%d  \n ", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pSWSIndexData->IndexID, pSWSIndexData->IndexName, pSWSIndexData->TradingDay, pSWSIndexData->PreClosePrice, pSWSIndexData->OpenPrice, pSWSIndexData->HighPrice, pSWSIndexData->LowPrice, pSWSIndexData->ClosePrice, pSWSIndexData->Volume, pSWSIndexData->Amount, pSWSIndexData->PERatio, pSWSIndexData->PBRatio, pSWSIndexData->FloatMV, pSWSIndexData->TotalMV, pSWSIndexData->PageLocate, pSWSIndexData->PageTotal);
	}
	if (NULL == pSWSIndexData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquirySWSIndexData End==========================================================================================================\n ");
	}
}
//股票日K行情请求包应答
void CQCVDSimpleSpi::OnRspInquiryStockDayQuotation(CQCVDStockDayQuotationField *pStockDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pStockDayQuotation)
	{
		std::string pStockDayQuotation_ExchangeID_str = char2str(pStockDayQuotation->ExchangeID);
		const char*  pStockDayQuotation_ExchangeID_str_ptr = pStockDayQuotation_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryStockDayQuotation IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s TradingDay=%s Currency=%s PreClosePrice=%f OpenPrice=%f HighPrice=%f LowPrice=%f ClosePrice=%f Change=%f PercentChange=%f Volume=%f Turnover=%f AdjustPreClosePrice=%f AdjustOpenPrice=%f AdjustHighPrice=%f AdjustLowPrice=%f AdjustClosePrice=%f AdjustFactor=%f AveragePrice=%f TradeStatus=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pStockDayQuotation_ExchangeID_str_ptr, pStockDayQuotation->SecurityID, pStockDayQuotation->TradingDay, pStockDayQuotation->Currency, pStockDayQuotation->PreClosePrice, pStockDayQuotation->OpenPrice, pStockDayQuotation->HighPrice, pStockDayQuotation->LowPrice, pStockDayQuotation->ClosePrice, pStockDayQuotation->Change, pStockDayQuotation->PercentChange, pStockDayQuotation->Volume, pStockDayQuotation->Turnover, pStockDayQuotation->AdjustPreClosePrice, pStockDayQuotation->AdjustOpenPrice, pStockDayQuotation->AdjustHighPrice, pStockDayQuotation->AdjustLowPrice, pStockDayQuotation->AdjustClosePrice, pStockDayQuotation->AdjustFactor, pStockDayQuotation->AveragePrice, pStockDayQuotation->TradeStatus, pStockDayQuotation->PageLocate, pStockDayQuotation->PageTotal);
	}
	if (NULL == pStockDayQuotation)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryStockDayQuotation End==========================================================================================================\n ");
	}
}
//A股交易日历请求包应答
void CQCVDSimpleSpi::OnRspInquiryShareCalendar(CQCVDShareCalendarField *pShareCalendar, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pShareCalendar)
	{

		printf("CQCValueAddProSpi OnRspInquiryShareCalendar IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pShareCalendar->TradingDay, pShareCalendar->PageLocate, pShareCalendar->PageTotal);
	}
	if (NULL == pShareCalendar)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryShareCalendar End==========================================================================================================\n ");
	}
}
//申万行业分类请求包应答
void CQCVDSimpleSpi::OnRspInquirySWIndustriesClass(CQCVDSWIndustriesClassField *pSWIndustriesClass, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pSWIndustriesClass)
	{
		std::string pSWIndustriesClass_ExchangeID_str = char2str(pSWIndustriesClass->ExchangeID);
		const char*  pSWIndustriesClass_ExchangeID_str_ptr = pSWIndustriesClass_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquirySWIndustriesClass IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s SWIndustryCode=%s SWIndustryName=%s EntryDate=%s RemoveDate=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pSWIndustriesClass_ExchangeID_str_ptr, pSWIndustriesClass->SecurityID, pSWIndustriesClass->SWIndustryCode, pSWIndustriesClass->SWIndustryName, pSWIndustriesClass->EntryDate, pSWIndustriesClass->RemoveDate, pSWIndustriesClass->PageLocate, pSWIndustriesClass->PageTotal);
	}
	if (NULL == pSWIndustriesClass)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquirySWIndustriesClass End==========================================================================================================\n ");
	}
}
//A股日行情估值指标请求包应答
void CQCVDSimpleSpi::OnRspInquiryStockAssessIndicator(CQCVDStockAssessIndicatorField *pStockAssessIndicator, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pStockAssessIndicator)
	{
		std::string pStockAssessIndicator_ExchangeID_str = char2str(pStockAssessIndicator->ExchangeID);
		const char*  pStockAssessIndicator_ExchangeID_str_ptr = pStockAssessIndicator_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryStockAssessIndicator IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s TradingDay=%s Currency=%s TotalMV=%f FloatMV=%f HighPrice_52W=%f LowPrice_52W=%f PERatio=%f PBRatio=%f PE_TTMRatio=%f PCF_OCFRatio=%f PCF_OCFTTMRatio=%f PCF_NCFRatio=%f PCF_NCFTTMRatio=%f PSRatio=%f PS_TTMRatio=%f TurnRatio=%f FreeTurnOverRatio=%f TotalShareToday=%f FloatShareToday=%f ClosePrice=%f PriceDivDps=%f AdjustHighPrice_52W=%f AdjustLowPrice_52W=%f FreeShareToday=%f NetProfitParentCompTTM=%f NetProfitParentCompLYR=%f NstAssetsToday=%f NetCashFlowsOperActTTM=%f NetCashFlowsOperActLYR=%f EvrTTM=%f EvrLYR=%f NetIncrCashEquTTM=%f NetIncrCashEquLYR=%f UpDownLimitStatus=%s LowestHighestStatus=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pStockAssessIndicator_ExchangeID_str_ptr, pStockAssessIndicator->SecurityID, pStockAssessIndicator->TradingDay, pStockAssessIndicator->Currency, pStockAssessIndicator->TotalMV, pStockAssessIndicator->FloatMV, pStockAssessIndicator->HighPrice_52W, pStockAssessIndicator->LowPrice_52W, pStockAssessIndicator->PERatio, pStockAssessIndicator->PBRatio, pStockAssessIndicator->PE_TTMRatio, pStockAssessIndicator->PCF_OCFRatio, pStockAssessIndicator->PCF_OCFTTMRatio, pStockAssessIndicator->PCF_NCFRatio, pStockAssessIndicator->PCF_NCFTTMRatio, pStockAssessIndicator->PSRatio, pStockAssessIndicator->PS_TTMRatio, pStockAssessIndicator->TurnRatio, pStockAssessIndicator->FreeTurnOverRatio, pStockAssessIndicator->TotalShareToday, pStockAssessIndicator->FloatShareToday, pStockAssessIndicator->ClosePrice, pStockAssessIndicator->PriceDivDps, pStockAssessIndicator->AdjustHighPrice_52W, pStockAssessIndicator->AdjustLowPrice_52W, pStockAssessIndicator->FreeShareToday, pStockAssessIndicator->NetProfitParentCompTTM, pStockAssessIndicator->NetProfitParentCompLYR, pStockAssessIndicator->NstAssetsToday, pStockAssessIndicator->NetCashFlowsOperActTTM, pStockAssessIndicator->NetCashFlowsOperActLYR, pStockAssessIndicator->EvrTTM, pStockAssessIndicator->EvrLYR, pStockAssessIndicator->NetIncrCashEquTTM, pStockAssessIndicator->NetIncrCashEquLYR, pStockAssessIndicator->UpDownLimitStatus, pStockAssessIndicator->LowestHighestStatus, pStockAssessIndicator->PageLocate, pStockAssessIndicator->PageTotal);
	}
	if (NULL == pStockAssessIndicator)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryStockAssessIndicator End==========================================================================================================\n ");
	}
}
//一致预测个股滚动指标请求包应答
void CQCVDSimpleSpi::OnRspInquiryConsensusRollingData(CQCVDConsensusRollingDataField *pConsensusRollingData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pConsensusRollingData)
	{
		std::string pConsensusRollingData_ExchangeID_str = char2str(pConsensusRollingData->ExchangeID);
		const char*  pConsensusRollingData_ExchangeID_str_ptr = pConsensusRollingData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryConsensusRollingData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s TradingDay=%s RollingType=%s NetProfit=%f EPS=%f PERatio=%f PEGRatio=%f PBRatio=%f ROERatio=%f OperRevenue=%f CFPS=%f DPS=%f BPS=%f EBIT=%f EBITDA=%f TotalProfit=%f OperProfit=%f OperCost=%f BenchmarkYear=%s BaseShare=%f PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pConsensusRollingData_ExchangeID_str_ptr, pConsensusRollingData->SecurityID, pConsensusRollingData->TradingDay, pConsensusRollingData->RollingType, pConsensusRollingData->NetProfit, pConsensusRollingData->EPS, pConsensusRollingData->PERatio, pConsensusRollingData->PEGRatio, pConsensusRollingData->PBRatio, pConsensusRollingData->ROERatio, pConsensusRollingData->OperRevenue, pConsensusRollingData->CFPS, pConsensusRollingData->DPS, pConsensusRollingData->BPS, pConsensusRollingData->EBIT, pConsensusRollingData->EBITDA, pConsensusRollingData->TotalProfit, pConsensusRollingData->OperProfit, pConsensusRollingData->OperCost, pConsensusRollingData->BenchmarkYear, pConsensusRollingData->BaseShare, pConsensusRollingData->PageLocate, pConsensusRollingData->PageTotal);
	}
	if (NULL == pConsensusRollingData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryConsensusRollingData End==========================================================================================================\n ");
	}
}
//指数日行情请求包应答
void CQCVDSimpleSpi::OnRspInquiryIndexDayQuotation(CQCVDIndexDayQuotationField *pIndexDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pIndexDayQuotation)
	{

		printf("CQCValueAddProSpi OnRspInquiryIndexDayQuotation IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s IndexCode=%s TradingDay=%s Currency=%s PreClosePrice=%f OpenPrice=%f HighPrice=%f LowPrice=%f ClosePrice=%f Change=%f PercentChange=%f Volume=%f Turnover=%f SecID=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pIndexDayQuotation->IndexCode, pIndexDayQuotation->TradingDay, pIndexDayQuotation->Currency, pIndexDayQuotation->PreClosePrice, pIndexDayQuotation->OpenPrice, pIndexDayQuotation->HighPrice, pIndexDayQuotation->LowPrice, pIndexDayQuotation->ClosePrice, pIndexDayQuotation->Change, pIndexDayQuotation->PercentChange, pIndexDayQuotation->Volume, pIndexDayQuotation->Turnover, pIndexDayQuotation->SecID, pIndexDayQuotation->PageLocate, pIndexDayQuotation->PageTotal);
	}
	if (NULL == pIndexDayQuotation)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryIndexDayQuotation End==========================================================================================================\n ");
	}
}
//A股基本资料请求包应答
void CQCVDSimpleSpi::OnRspInquiryShareDescription(CQCVDShareDescriptionField *pShareDescription, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pShareDescription)
	{
		std::string pShareDescription_ExchangeID_str = char2str(pShareDescription->ExchangeID);
		const char*  pShareDescription_ExchangeID_str_ptr = pShareDescription_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryShareDescription IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s SecurityName=%s CompanyName=%s CompanyNameEnglish=%s ISINCode=%s ListBoard=%s ListDate=%s DeListDate=%s Currency=%s PinYin=%s ListBoardName=%s SHSC=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pShareDescription_ExchangeID_str_ptr, pShareDescription->SecurityID, pShareDescription->SecurityName, pShareDescription->CompanyName, pShareDescription->CompanyNameEnglish, pShareDescription->ISINCode, pShareDescription->ListBoard, pShareDescription->ListDate, pShareDescription->DeListDate, pShareDescription->Currency, pShareDescription->PinYin, pShareDescription->ListBoardName, pShareDescription->SHSC, pShareDescription->PageLocate, pShareDescription->PageTotal);
	}
	if (NULL == pShareDescription)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryShareDescription End==========================================================================================================\n ");
	}
}
//十大股东信息请求包应答
void CQCVDSimpleSpi::OnRspInquiryTopTenHoldersDetail(CQCVDTopTenHoldersDetailField *pTopTenHoldersDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pTopTenHoldersDetail)
	{
		std::string pTopTenHoldersDetail_ExchangeID_str = char2str(pTopTenHoldersDetail->ExchangeID);
		const char*  pTopTenHoldersDetail_ExchangeID_str_ptr = pTopTenHoldersDetail_ExchangeID_str.c_str();
		std::string pTopTenHoldersDetail_HolderCategory_str = char2str(pTopTenHoldersDetail->HolderCategory);
		const char*  pTopTenHoldersDetail_HolderCategory_str_ptr = pTopTenHoldersDetail_HolderCategory_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryTopTenHoldersDetail IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s AnnDate=%s HolderEndDate=%s HolderCategory=%s HolderName=%s HolderQuantity=%f HolderPercent=%f HolderRestrictedQuantity=%f HolderShareCategoryName=%s HolderMemo=%s ReportPeriod=%s HolderNature=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pTopTenHoldersDetail_ExchangeID_str_ptr, pTopTenHoldersDetail->SecurityID, pTopTenHoldersDetail->AnnDate, pTopTenHoldersDetail->HolderEndDate, pTopTenHoldersDetail_HolderCategory_str_ptr, pTopTenHoldersDetail->HolderName, pTopTenHoldersDetail->HolderQuantity, pTopTenHoldersDetail->HolderPercent, pTopTenHoldersDetail->HolderRestrictedQuantity, pTopTenHoldersDetail->HolderShareCategoryName, pTopTenHoldersDetail->HolderMemo, pTopTenHoldersDetail->ReportPeriod, pTopTenHoldersDetail->HolderNature, pTopTenHoldersDetail->PageLocate, pTopTenHoldersDetail->PageTotal);
	}
	if (NULL == pTopTenHoldersDetail)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryTopTenHoldersDetail End==========================================================================================================\n ");
	}
}
//客户历史周期股票盈亏请求包应答
void CQCVDSimpleSpi::OnRspInquiryHisShareProfit(CQCVDHisShareProfitField *pHisShareProfit, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHisShareProfit)
	{
		std::string pHisShareProfit_ExchangeID_str = char2str(pHisShareProfit->ExchangeID);
		const char*  pHisShareProfit_ExchangeID_str_ptr = pHisShareProfit_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryHisShareProfit IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s ExchangeID=%s SecurityID=%s MarketValueBeg=%f MarketValueEnd=%f OpenAmount=%f CloseAmount=%f TotalProfit=%f RealEndDate=%s   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHisShareProfit->InvestorID, pHisShareProfit_ExchangeID_str_ptr, pHisShareProfit->SecurityID, pHisShareProfit->MarketValueBeg, pHisShareProfit->MarketValueEnd, pHisShareProfit->OpenAmount, pHisShareProfit->CloseAmount, pHisShareProfit->TotalProfit, pHisShareProfit->RealEndDate);
	}
	if (NULL == pHisShareProfit)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHisShareProfit End==========================================================================================================\n ");
	}
}
//根据客户号查询客户资金账户请求包应答
void CQCVDSimpleSpi::OnRspInquiryAccountIDByInvestorID(CQCVDAccountIDByInvestorIDField *pAccountIDByInvestorID, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pAccountIDByInvestorID)
	{

		printf("CQCValueAddProSpi OnRspInquiryAccountIDByInvestorID IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s AccountID=%s   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pAccountIDByInvestorID->InvestorID, pAccountIDByInvestorID->AccountID);
	}
	if (NULL == pAccountIDByInvestorID)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryAccountIDByInvestorID End==========================================================================================================\n ");
	}
}
//客户周期资金请求包应答
void CQCVDSimpleSpi::OnRspInquiryCustPeriodCapiData(CQCVDCustPeriodCapiDataField *pCustPeriodCapiData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCustPeriodCapiData)
	{
		std::string pCustPeriodCapiData_StandardType_str = char2str(pCustPeriodCapiData->StandardType);
		const char*  pCustPeriodCapiData_StandardType_str_ptr = pCustPeriodCapiData_StandardType_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryCustPeriodCapiData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s PeroidDate=%s TotalAsset=%f MoneyIn=%f MoneyOut=%f PeroidProfit=%f StandardType=%s   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCustPeriodCapiData->InvestorID, pCustPeriodCapiData->PeroidDate, pCustPeriodCapiData->TotalAsset, pCustPeriodCapiData->MoneyIn, pCustPeriodCapiData->MoneyOut, pCustPeriodCapiData->PeroidProfit, pCustPeriodCapiData_StandardType_str_ptr);
	}
	if (NULL == pCustPeriodCapiData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryCustPeriodCapiData End==========================================================================================================\n ");
	}
}
//客户周期盈亏请求包应答
void CQCVDSimpleSpi::OnRspInquiryCustPeriodProfitData(CQCVDCustPeriodProfitDataField *pCustPeriodProfitData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCustPeriodProfitData)
	{

		printf("CQCValueAddProSpi OnRspInquiryCustPeriodProfitData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s ExchangeID=%s SecurityID=%s PeroidProfit=%f PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCustPeriodProfitData->InvestorID, pCustPeriodProfitData->ExchangeID, pCustPeriodProfitData->SecurityID, pCustPeriodProfitData->PeroidProfit, pCustPeriodProfitData->PageLocate, pCustPeriodProfitData->PageTotal);
	}
	if (NULL == pCustPeriodProfitData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryCustPeriodProfitData End==========================================================================================================\n ");
	}
}
//中国A股发行请求包应答
void CQCVDSimpleSpi::OnRspInquiryShareIssuance(CQCVDShareIssuanceField *pShareIssuance, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pShareIssuance)
	{
		std::string pShareIssuance_ExchangeID_str = char2str(pShareIssuance->ExchangeID);
		const char*  pShareIssuance_ExchangeID_str_ptr = pShareIssuance_ExchangeID_str.c_str();
		std::string pShareIssuance_IsFailure_str = char2str(pShareIssuance->IsFailure);
		const char*  pShareIssuance_IsFailure_str_ptr = pShareIssuance_IsFailure_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryShareIssuance IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s SecurityName=%s OnlineCode=%s OnlineName=%s OnlineDate=%s OnlinePrice=%f PurchaseUpLimit=%d ListDate=%s ListBoardName=%s AnnDate=%s IsFailure=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pShareIssuance_ExchangeID_str_ptr, pShareIssuance->SecurityID, pShareIssuance->SecurityName, pShareIssuance->OnlineCode, pShareIssuance->OnlineName, pShareIssuance->OnlineDate, pShareIssuance->OnlinePrice, pShareIssuance->PurchaseUpLimit, pShareIssuance->ListDate, pShareIssuance->ListBoardName, pShareIssuance->AnnDate, pShareIssuance_IsFailure_str_ptr, pShareIssuance->PageLocate, pShareIssuance->PageTotal);
	}
	if (NULL == pShareIssuance)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryShareIssuance End==========================================================================================================\n ");
	}
}
//中国可转债发行请求包应答
void CQCVDSimpleSpi::OnRspInquiryBondIssuance(CQCVDBondIssuanceField *pBondIssuance, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pBondIssuance)
	{
		std::string pBondIssuance_ExchangeID_str = char2str(pBondIssuance->ExchangeID);
		const char*  pBondIssuance_ExchangeID_str_ptr = pBondIssuance_ExchangeID_str.c_str();
		std::string pBondIssuance_IsFailure_str = char2str(pBondIssuance->IsFailure);
		const char*  pBondIssuance_IsFailure_str_ptr = pBondIssuance_IsFailure_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryBondIssuance IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s SecurityName=%s OnlineDate=%s OnlineCode=%s OnlineName=%s OnlinePrice=%f PurchaseUpLimit=%d RationDate=%s RationCheckInDate=%s RationPayDate=%s RationCode=%s RationName=%s RationPrice=%f RationRatioDenominator=%f RationRatioMolecule=%f ListDate=%s AnnDate=%s IsFailure=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pBondIssuance_ExchangeID_str_ptr, pBondIssuance->SecurityID, pBondIssuance->SecurityName, pBondIssuance->OnlineDate, pBondIssuance->OnlineCode, pBondIssuance->OnlineName, pBondIssuance->OnlinePrice, pBondIssuance->PurchaseUpLimit, pBondIssuance->RationDate, pBondIssuance->RationCheckInDate, pBondIssuance->RationPayDate, pBondIssuance->RationCode, pBondIssuance->RationName, pBondIssuance->RationPrice, pBondIssuance->RationRatioDenominator, pBondIssuance->RationRatioMolecule, pBondIssuance->ListDate, pBondIssuance->AnnDate, pBondIssuance_IsFailure_str_ptr, pBondIssuance->PageLocate, pBondIssuance->PageTotal);
	}
	if (NULL == pBondIssuance)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryBondIssuance End==========================================================================================================\n ");
	}
}
//中国A股股权质押信息请求包应答
void CQCVDSimpleSpi::OnRspInquiryShareEquityPledgeInfo(CQCVDShareEquityPledgeInfoField *pShareEquityPledgeInfo, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pShareEquityPledgeInfo)
	{
		std::string pShareEquityPledgeInfo_ExchangeID_str = char2str(pShareEquityPledgeInfo->ExchangeID);
		const char*  pShareEquityPledgeInfo_ExchangeID_str_ptr = pShareEquityPledgeInfo_ExchangeID_str.c_str();
		std::string pShareEquityPledgeInfo_Discharge_str = char2str(pShareEquityPledgeInfo->Discharge);
		const char*  pShareEquityPledgeInfo_Discharge_str_ptr = pShareEquityPledgeInfo_Discharge_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryShareEquityPledgeInfo IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s AnnDate=%s PledgeBegDate=%s PledgeEndDate=%s HolderName=%s PledgeShares=%f Pledgor=%s DischargeDate=%s Remark=%s Discharge=%s HolderTypeCode=%s HolderID=%s PledgorTypeCode=%s PledgorID=%s ShrCategoryCode=%s TotalHoldingShr=%f TotalPledgeShr=%f PledgeShrRatio=%f TotalHoldingShrRatio=%f EquityPledgeRepo=%d PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pShareEquityPledgeInfo_ExchangeID_str_ptr, pShareEquityPledgeInfo->SecurityID, pShareEquityPledgeInfo->AnnDate, pShareEquityPledgeInfo->PledgeBegDate, pShareEquityPledgeInfo->PledgeEndDate, pShareEquityPledgeInfo->HolderName, pShareEquityPledgeInfo->PledgeShares, pShareEquityPledgeInfo->Pledgor, pShareEquityPledgeInfo->DischargeDate, pShareEquityPledgeInfo->Remark, pShareEquityPledgeInfo_Discharge_str_ptr, pShareEquityPledgeInfo->HolderTypeCode, pShareEquityPledgeInfo->HolderID, pShareEquityPledgeInfo->PledgorTypeCode, pShareEquityPledgeInfo->PledgorID, pShareEquityPledgeInfo->ShrCategoryCode, pShareEquityPledgeInfo->TotalHoldingShr, pShareEquityPledgeInfo->TotalPledgeShr, pShareEquityPledgeInfo->PledgeShrRatio, pShareEquityPledgeInfo->TotalHoldingShrRatio, pShareEquityPledgeInfo->EquityPledgeRepo, pShareEquityPledgeInfo->PageLocate, pShareEquityPledgeInfo->PageTotal);
	}
	if (NULL == pShareEquityPledgeInfo)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryShareEquityPledgeInfo End==========================================================================================================\n ");
	}
}
//历史资金请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryCapital(CQCVDHistoryCapitalField *pHistoryCapital, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryCapital)
	{

		printf("CQCValueAddProSpi OnRspInquiryHistoryCapital IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s InvestorID=%s TotalAsset=%f CustFund=%f StockValue=%f MoneyIn=%f MoneyOut=%f TodayProfit=%f LastTotalAsset=%f PageLocate=%d PageTotal=%d undueBondValue=%f fundMarketValue=%f OTCAsset=%f SpecFundAsset=%f   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryCapital->TradingDay, pHistoryCapital->InvestorID, pHistoryCapital->TotalAsset, pHistoryCapital->CustFund, pHistoryCapital->StockValue, pHistoryCapital->MoneyIn, pHistoryCapital->MoneyOut, pHistoryCapital->TodayProfit, pHistoryCapital->LastTotalAsset, pHistoryCapital->PageLocate, pHistoryCapital->PageTotal, pHistoryCapital->undueBondValue, pHistoryCapital->fundMarketValue, pHistoryCapital->OTCAsset, pHistoryCapital->SpecFundAsset);
	}
	if (NULL == pHistoryCapital)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryCapital End==========================================================================================================\n ");
	}
}
//历史交割单请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryDelivery(CQCVDHistoryDeliveryField *pHistoryDelivery, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryDelivery)
	{

		printf("CQCValueAddProSpi OnRspInquiryHistoryDelivery IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s TradeDate=%s TradeTime=%s SecurityID=%s SecurityName=%s ExchangeID=%s ExchangeName=%s Currency=%s EntrustType=%s EntrustTypeName=%s ActuallyPayMoney=%f Turnover=%f Volume=%d Price=%f StampTax=%f TransferFee=%f ActualBrokerage=%f TransactionFee=%f SerialNo=%s ShareholderID=%s StockBalance=%d FundBalance=%f PageLocate=%d PageTotal=%d OpenAvg=%f OpenAvgProfit=%f   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryDelivery->InvestorID, pHistoryDelivery->TradeDate, pHistoryDelivery->TradeTime, pHistoryDelivery->SecurityID, pHistoryDelivery->SecurityName, pHistoryDelivery->ExchangeID, pHistoryDelivery->ExchangeName, pHistoryDelivery->Currency, pHistoryDelivery->EntrustType, pHistoryDelivery->EntrustTypeName, pHistoryDelivery->ActuallyPayMoney, pHistoryDelivery->Turnover, pHistoryDelivery->Volume, pHistoryDelivery->Price, pHistoryDelivery->StampTax, pHistoryDelivery->TransferFee, pHistoryDelivery->ActualBrokerage, pHistoryDelivery->TransactionFee, pHistoryDelivery->SerialNo, pHistoryDelivery->ShareholderID, pHistoryDelivery->StockBalance, pHistoryDelivery->FundBalance, pHistoryDelivery->PageLocate, pHistoryDelivery->PageTotal, pHistoryDelivery->OpenAvg, pHistoryDelivery->OpenAvgProfit);
	}
	if (NULL == pHistoryDelivery)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryDelivery End==========================================================================================================\n ");
	}
}
//历史资金流水请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryFundDetail(CQCVDHistoryFundDetailField *pHistoryFundDetail, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryFundDetail)
	{

		printf("CQCValueAddProSpi OnRspInquiryHistoryFundDetail IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SerialNo=%s TradeDate=%s TradeTime=%s InvestorID=%s InvestorName=%s AccountID=%s BusinessAccount=%s BusinessAccountName=%s MoneyIn=%f MoneyOut=%f FundBalance=%f OperSummary=%s RelevantMarket=%s RelevantVariety=%s RelevantAccount=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryFundDetail->SerialNo, pHistoryFundDetail->TradeDate, pHistoryFundDetail->TradeTime, pHistoryFundDetail->InvestorID, pHistoryFundDetail->InvestorName, pHistoryFundDetail->AccountID, pHistoryFundDetail->BusinessAccount, pHistoryFundDetail->BusinessAccountName, pHistoryFundDetail->MoneyIn, pHistoryFundDetail->MoneyOut, pHistoryFundDetail->FundBalance, pHistoryFundDetail->OperSummary, pHistoryFundDetail->RelevantMarket, pHistoryFundDetail->RelevantVariety, pHistoryFundDetail->RelevantAccount, pHistoryFundDetail->PageLocate, pHistoryFundDetail->PageTotal);
	}
	if (NULL == pHistoryFundDetail)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryFundDetail End==========================================================================================================\n ");
	}
}
//历史持仓请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryHold(CQCVDHistoryHoldField *pHistoryHold, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryHold)
	{

		printf("CQCValueAddProSpi OnRspInquiryHistoryHold IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s TradingDay=%s InvestorID=%s InvestorName=%s ShareholderID=%s ExchangeID=%s ExchangeName=%s SecurityID=%s Currency=%s SecurityType=%s SecurityTypeName=%s Volume=%d UncirculatedVolume=%d HoldCost=%f TotalProfit=%f PageLocate=%d PageTotal=%d OpenAvg=%f   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryHold->TradingDay, pHistoryHold->InvestorID, pHistoryHold->InvestorName, pHistoryHold->ShareholderID, pHistoryHold->ExchangeID, pHistoryHold->ExchangeName, pHistoryHold->SecurityID, pHistoryHold->Currency, pHistoryHold->SecurityType, pHistoryHold->SecurityTypeName, pHistoryHold->Volume, pHistoryHold->UncirculatedVolume, pHistoryHold->HoldCost, pHistoryHold->TotalProfit, pHistoryHold->PageLocate, pHistoryHold->PageTotal, pHistoryHold->OpenAvg);
	}
	if (NULL == pHistoryHold)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryHold End==========================================================================================================\n ");
	}
}

//历史委托数据请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryOrderEX(CQCVDHistoryOrderEXField *pHistoryOrderEX, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryOrderEX)
	{
		std::string pHistoryOrderEX_OrderPriceType_str = char2str(pHistoryOrderEX->OrderPriceType);
		const char*  pHistoryOrderEX_OrderPriceType_str_ptr = pHistoryOrderEX_OrderPriceType_str.c_str();
		std::string pHistoryOrderEX_Direction_str = char2str(pHistoryOrderEX->Direction);
		const char*  pHistoryOrderEX_Direction_str_ptr = pHistoryOrderEX_Direction_str.c_str();
		std::string pHistoryOrderEX_TimeCondition_str = char2str(pHistoryOrderEX->TimeCondition);
		const char*  pHistoryOrderEX_TimeCondition_str_ptr = pHistoryOrderEX_TimeCondition_str.c_str();
		std::string pHistoryOrderEX_VolumeCondition_str = char2str(pHistoryOrderEX->VolumeCondition);
		const char*  pHistoryOrderEX_VolumeCondition_str_ptr = pHistoryOrderEX_VolumeCondition_str.c_str();
		std::string pHistoryOrderEX_ForceCloseReason_str = char2str(pHistoryOrderEX->ForceCloseReason);
		const char*  pHistoryOrderEX_ForceCloseReason_str_ptr = pHistoryOrderEX_ForceCloseReason_str.c_str();
		std::string pHistoryOrderEX_ExchangeID_str = char2str(pHistoryOrderEX->ExchangeID);
		const char*  pHistoryOrderEX_ExchangeID_str_ptr = pHistoryOrderEX_ExchangeID_str.c_str();
		std::string pHistoryOrderEX_MarketID_str = char2str(pHistoryOrderEX->MarketID);
		const char*  pHistoryOrderEX_MarketID_str_ptr = pHistoryOrderEX_MarketID_str.c_str();
		std::string pHistoryOrderEX_OrderSubmitStatus_str = char2str(pHistoryOrderEX->OrderSubmitStatus);
		const char*  pHistoryOrderEX_OrderSubmitStatus_str_ptr = pHistoryOrderEX_OrderSubmitStatus_str.c_str();
		std::string pHistoryOrderEX_OrderStatus_str = char2str(pHistoryOrderEX->OrderStatus);
		const char*  pHistoryOrderEX_OrderStatus_str_ptr = pHistoryOrderEX_OrderStatus_str.c_str();
		std::string pHistoryOrderEX_OrderType_str = char2str(pHistoryOrderEX->OrderType);
		const char*  pHistoryOrderEX_OrderType_str_ptr = pHistoryOrderEX_OrderType_str.c_str();
		std::string pHistoryOrderEX_LotType_str = char2str(pHistoryOrderEX->LotType);
		const char*  pHistoryOrderEX_LotType_str_ptr = pHistoryOrderEX_LotType_str.c_str();
		std::string pHistoryOrderEX_Operway_str = char2str(pHistoryOrderEX->Operway);
		const char*  pHistoryOrderEX_Operway_str_ptr = pHistoryOrderEX_Operway_str.c_str();
		std::string pHistoryOrderEX_ProperCtrlBusinessType_str = char2str(pHistoryOrderEX->ProperCtrlBusinessType);
		const char*  pHistoryOrderEX_ProperCtrlBusinessType_str_ptr = pHistoryOrderEX_ProperCtrlBusinessType_str.c_str();
		std::string pHistoryOrderEX_ProperCtrlPassFlag_str = char2str(pHistoryOrderEX->ProperCtrlPassFlag);
		const char*  pHistoryOrderEX_ProperCtrlPassFlag_str_ptr = pHistoryOrderEX_ProperCtrlPassFlag_str.c_str();
		std::string pHistoryOrderEX_CondCheck_str = char2str(pHistoryOrderEX->CondCheck);
		const char*  pHistoryOrderEX_CondCheck_str_ptr = pHistoryOrderEX_CondCheck_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryHistoryOrderEX IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s SecurityID=%s OrderRef=%s UserID=%s OrderPriceType=%s Direction=%s CombOffsetFlag=%s CombHedgeFlag=%s LimitPrice=%f VolumeTotalOriginal=%d TimeCondition=%s VolumeCondition=%s MinVolume=%d ForceCloseReason=%s RequestID=%d OrderLocalID=%s ExchangeID=%s MarketID=%s ShareholderID=%s ExchangeInstID=%s TraderID=%s OrderSubmitStatus=%s TradingDay=%s OrderSysID=%s OrderStatus=%s OrderType=%s VolumeTraded=%d VolumeTotal=%d InsertDate=%s InsertTime=%s CancelTime=%s ActiveTraderID=%s FrontID=%d SessionID=%d UserProductInfo=%s StatusMsg=%s UserForceClose=%d ActiveUserID=%s BusinessUnitID=%s AccountID=%s IPAddress=%s MacAddress=%s LotType=%s BInfo=%s SInfo=%s IInfo=%d TransfereePbuID=%s Operway=%s DepartmentID=%s ProperCtrlBusinessType=%s ProperCtrlPassFlag=%s CondCheck=%s IsCacheOrder=%d Turnover=%f RtnFloatInfo=%f RtnIntInfo=%d HDSerial=%s Mobile=%s GTDate=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryOrderEX->InvestorID, pHistoryOrderEX->SecurityID, pHistoryOrderEX->OrderRef, pHistoryOrderEX->UserID, pHistoryOrderEX_OrderPriceType_str_ptr, pHistoryOrderEX_Direction_str_ptr, pHistoryOrderEX->CombOffsetFlag, pHistoryOrderEX->CombHedgeFlag, pHistoryOrderEX->LimitPrice, pHistoryOrderEX->VolumeTotalOriginal, pHistoryOrderEX_TimeCondition_str_ptr, pHistoryOrderEX_VolumeCondition_str_ptr, pHistoryOrderEX->MinVolume, pHistoryOrderEX_ForceCloseReason_str_ptr, pHistoryOrderEX->RequestID, pHistoryOrderEX->OrderLocalID, pHistoryOrderEX_ExchangeID_str_ptr, pHistoryOrderEX_MarketID_str_ptr, pHistoryOrderEX->ShareholderID, pHistoryOrderEX->ExchangeInstID, pHistoryOrderEX->TraderID, pHistoryOrderEX_OrderSubmitStatus_str_ptr, pHistoryOrderEX->TradingDay, pHistoryOrderEX->OrderSysID, pHistoryOrderEX_OrderStatus_str_ptr, pHistoryOrderEX_OrderType_str_ptr, pHistoryOrderEX->VolumeTraded, pHistoryOrderEX->VolumeTotal, pHistoryOrderEX->InsertDate, pHistoryOrderEX->InsertTime, pHistoryOrderEX->CancelTime, pHistoryOrderEX->ActiveTraderID, pHistoryOrderEX->FrontID, pHistoryOrderEX->SessionID, pHistoryOrderEX->UserProductInfo, pHistoryOrderEX->StatusMsg, pHistoryOrderEX->UserForceClose, pHistoryOrderEX->ActiveUserID, pHistoryOrderEX->BusinessUnitID, pHistoryOrderEX->AccountID, pHistoryOrderEX->IPAddress, pHistoryOrderEX->MacAddress, pHistoryOrderEX_LotType_str_ptr, pHistoryOrderEX->BInfo, pHistoryOrderEX->SInfo, pHistoryOrderEX->IInfo, pHistoryOrderEX->TransfereePbuID, pHistoryOrderEX_Operway_str_ptr, pHistoryOrderEX->DepartmentID, pHistoryOrderEX_ProperCtrlBusinessType_str_ptr, pHistoryOrderEX_ProperCtrlPassFlag_str_ptr, pHistoryOrderEX_CondCheck_str_ptr, pHistoryOrderEX->IsCacheOrder, pHistoryOrderEX->Turnover, pHistoryOrderEX->RtnFloatInfo, pHistoryOrderEX->RtnIntInfo, pHistoryOrderEX->HDSerial, pHistoryOrderEX->Mobile, pHistoryOrderEX->GTDate, pHistoryOrderEX->PageLocate, pHistoryOrderEX->PageTotal);
	}
	if (NULL == pHistoryOrderEX)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryOrderEX End==========================================================================================================\n ");
	}
}
//历史成交请求包应答
void CQCVDSimpleSpi::OnRspInquiryHistoryTradeEX(CQCVDHistoryTradeEXField *pHistoryTradeEX, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pHistoryTradeEX)
	{
		std::string pHistoryTradeEX_ExchangeID_str = char2str(pHistoryTradeEX->ExchangeID);
		const char*  pHistoryTradeEX_ExchangeID_str_ptr = pHistoryTradeEX_ExchangeID_str.c_str();
		std::string pHistoryTradeEX_Direction_str = char2str(pHistoryTradeEX->Direction);
		const char*  pHistoryTradeEX_Direction_str_ptr = pHistoryTradeEX_Direction_str.c_str();
		std::string pHistoryTradeEX_MarketID_str = char2str(pHistoryTradeEX->MarketID);
		const char*  pHistoryTradeEX_MarketID_str_ptr = pHistoryTradeEX_MarketID_str.c_str();
		std::string pHistoryTradeEX_OffsetFlag_str = char2str(pHistoryTradeEX->OffsetFlag);
		const char*  pHistoryTradeEX_OffsetFlag_str_ptr = pHistoryTradeEX_OffsetFlag_str.c_str();
		std::string pHistoryTradeEX_HedgeFlag_str = char2str(pHistoryTradeEX->HedgeFlag);
		const char*  pHistoryTradeEX_HedgeFlag_str_ptr = pHistoryTradeEX_HedgeFlag_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryHistoryTradeEX IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s InvestorID=%s SecurityID=%s UserID=%s ExchangeID=%s TradeID=%s Direction=%s OrderSysID=%s MarketID=%s ShareholderID=%s ExchangeInstID=%s OffsetFlag=%s HedgeFlag=%s Price=%f Volume=%d TradeDate=%s TradeTime=%s TraderID=%s OrderLocalID=%s TradingDay=%s BusinessUnitID=%s AccountID=%s OrderRef=%s DepartmentID=%s ActualBrokerage=%f PageLocate=%d PageTotal=%d  \n ", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pHistoryTradeEX->InvestorID, pHistoryTradeEX->SecurityID, pHistoryTradeEX->UserID, pHistoryTradeEX_ExchangeID_str_ptr, pHistoryTradeEX->TradeID, pHistoryTradeEX_Direction_str_ptr, pHistoryTradeEX->OrderSysID, pHistoryTradeEX_MarketID_str_ptr, pHistoryTradeEX->ShareholderID, pHistoryTradeEX->ExchangeInstID, pHistoryTradeEX_OffsetFlag_str_ptr, pHistoryTradeEX_HedgeFlag_str_ptr, pHistoryTradeEX->Price, pHistoryTradeEX->Volume, pHistoryTradeEX->TradeDate, pHistoryTradeEX->TradeTime, pHistoryTradeEX->TraderID, pHistoryTradeEX->OrderLocalID, pHistoryTradeEX->TradingDay, pHistoryTradeEX->BusinessUnitID, pHistoryTradeEX->AccountID, pHistoryTradeEX->OrderRef, pHistoryTradeEX->DepartmentID, pHistoryTradeEX->ActualBrokerage, pHistoryTradeEX->PageLocate, pHistoryTradeEX->PageTotal);
	}
	if (NULL == pHistoryTradeEX)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryHistoryTradeEX End==========================================================================================================\n ");
	}
}

///##########################################################################################
//录入标记事件应答
void CQCVDSimpleSpi::OnRspInputRemarkEvent(CQCVDRspInputRemarkEventField *pRspInputRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspInputRemarkEvent)
	{
		std::string pRspInputRemarkEvent_ExchangeID_str = char2str(pRspInputRemarkEvent->ExchangeID);
		const char*  pRspInputRemarkEvent_ExchangeID_str_ptr = pRspInputRemarkEvent_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInputRemarkEvent IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SequenceNo=%s EventDate=%s EventTime=%s ExchangeID=%s SecurityID=%s Remark=%s InvestorID=%s   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspInputRemarkEvent->SequenceNo, pRspInputRemarkEvent->EventDate, pRspInputRemarkEvent->EventTime, pRspInputRemarkEvent_ExchangeID_str_ptr, pRspInputRemarkEvent->SecurityID, pRspInputRemarkEvent->Remark, pRspInputRemarkEvent->InvestorID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInputRemarkEvent End==========================================================================================================\n ");
	}
}
//更新标记事件应答
void CQCVDSimpleSpi::OnRspUpdateRemarkEvent(CQCVDRspUpdateRemarkEventField *pRspUpdateRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspUpdateRemarkEvent)
	{
		std::string pRspUpdateRemarkEvent_ExchangeID_str = char2str(pRspUpdateRemarkEvent->ExchangeID);
		const char*  pRspUpdateRemarkEvent_ExchangeID_str_ptr = pRspUpdateRemarkEvent_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspUpdateRemarkEvent IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SequenceNo=%s EventDate=%s EventTime=%s ExchangeID=%s SecurityID=%s Remark=%s InvestorID=%s   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspUpdateRemarkEvent->SequenceNo, pRspUpdateRemarkEvent->EventDate, pRspUpdateRemarkEvent->EventTime, pRspUpdateRemarkEvent_ExchangeID_str_ptr, pRspUpdateRemarkEvent->SecurityID, pRspUpdateRemarkEvent->Remark, pRspUpdateRemarkEvent->InvestorID);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspUpdateRemarkEvent End==========================================================================================================\n ");
	}
}
//删除标记事件应答
void CQCVDSimpleSpi::OnRspDeleteRemarkEvent(CQCVDRspDeleteRemarkEventField *pRspDeleteRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspDeleteRemarkEvent)
	{

		printf("CQCValueAddProSpi OnRspDeleteRemarkEvent IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SequenceNo=%s   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspDeleteRemarkEvent->SequenceNo);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspDeleteRemarkEvent End==========================================================================================================\n ");
	}
}
//查询历史标记事件应答
void CQCVDSimpleSpi::OnRspQryRemarkEvent(CQCVDRemarkEventField *pRemarkEvent, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRemarkEvent)
	{
		std::string pRemarkEvent_ExchangeID_str = char2str(pRemarkEvent->ExchangeID);
		const char*  pRemarkEvent_ExchangeID_str_ptr = pRemarkEvent_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspQryRemarkEvent IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s SequenceNo=%s EventDate=%s EventTime=%s ExchangeID=%s SecurityID=%s Remark=%s InvestorID=%s PageLocate=%d   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRemarkEvent->SequenceNo, pRemarkEvent->EventDate, pRemarkEvent->EventTime, pRemarkEvent_ExchangeID_str_ptr, pRemarkEvent->SecurityID, pRemarkEvent->Remark, pRemarkEvent->InvestorID, pRemarkEvent->PageLocate);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryRemarkEvent End==========================================================================================================\n ");
	}
}
//查询持仓量价分布信息应答
void CQCVDSimpleSpi::OnRspQryPriceDistributionData(CQCVDPriceDistributionDataField *pPriceDistributionData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pPriceDistributionData)
	{
		std::string pPriceDistributionData_ExchangeID_str = char2str(pPriceDistributionData->ExchangeID);
		const char*  pPriceDistributionData_ExchangeID_str_ptr = pPriceDistributionData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryPriceDistributionData bIsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s DataDate=%s PercentNum=%d PercentValue=%f percentNow=%d DistributionType=%d DistributionValue=%s ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPriceDistributionData->DataDate, pPriceDistributionData->PercentNum, pPriceDistributionData->PercentValue, pPriceDistributionData->percentNow, pPriceDistributionData->DistributionType, pPriceDistributionData->DistributionValue, pPriceDistributionData_ExchangeID_str_ptr, pPriceDistributionData->SecurityID, pPriceDistributionData->PageLocate);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryPriceDistributionData End==========================================================================================================\n ");
	}
}
//查询历史极值价格信息应答
void CQCVDSimpleSpi::OnRspQryPriceExtremumData(CQCVDPriceExtremumDataField *pPriceExtremumData, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pPriceExtremumData)
	{
		std::string pPriceExtremumData_ExchangeID_str = char2str(pPriceExtremumData->ExchangeID);
		const char*  pPriceExtremumData_ExchangeID_str_ptr = pPriceExtremumData_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryPriceExtremumData IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s HighestTradingDay=%s HighestUpdateTime=%s HighestUpdateMillisec=%d HighestPrice=%f LowestTradingDay=%s LowestUpdateTime=%s LowestUpdateMillisec=%d LowestPrice=%f ExchangeID=%s SecurityID=%s PageLocate=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pPriceExtremumData->HighestTradingDay, pPriceExtremumData->HighestUpdateTime, pPriceExtremumData->HighestUpdateMillisec, pPriceExtremumData->HighestPrice, pPriceExtremumData->LowestTradingDay, pPriceExtremumData->LowestUpdateTime, pPriceExtremumData->LowestUpdateMillisec, pPriceExtremumData->LowestPrice, pPriceExtremumData_ExchangeID_str_ptr, pPriceExtremumData->SecurityID, pPriceExtremumData->PageLocate);
	}
	if (NULL == pPriceExtremumData)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryPriceExtremumData End==========================================================================================================\n ");
	}
}
//查询盘口委托应答
void CQCVDSimpleSpi::OnRspInquiryQueueingOrder(CQCVDQueueingOrderField *pQueueingOrder, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pQueueingOrder)
	{
		std::string pQueueingOrder_ExchangeID_str = char2str(pQueueingOrder->ExchangeID);
		const char*  pQueueingOrder_ExchangeID_str_ptr = pQueueingOrder_ExchangeID_str.c_str();
		std::string pQueueingOrder_OrderType_str = char2str(pQueueingOrder->OrderType);
		const char*  pQueueingOrder_OrderType_str_ptr = pQueueingOrder_OrderType_str.c_str();
		std::string pQueueingOrder_Side_str = char2str(pQueueingOrder->Side);
		const char*  pQueueingOrder_Side_str_ptr = pQueueingOrder_Side_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryQueueingOrder bIsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s Price=%f Volume=%lld OrderType=%s Side=%s OrderSeq1=%d OrderSeq2=%d   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pQueueingOrder_ExchangeID_str_ptr, pQueueingOrder->SecurityID, pQueueingOrder->Price, pQueueingOrder->Volume, pQueueingOrder_OrderType_str_ptr, pQueueingOrder_Side_str_ptr, pQueueingOrder->OrderSeq1, pQueueingOrder->OrderSeq2);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryQueueingOrder End==========================================================================================================\n ");
	}
}
//计算期权希腊值请求包应答
void CQCVDSimpleSpi::OnRspQryOptionGreece(CQCVDRspOptionGreeceField *pRspOptionGreece, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (NULL != pRspOptionGreece)
	{
		std::string pRspOptionGreece_ExchangeID_str = char2str(pRspOptionGreece->ExchangeID);
		const char*  pRspOptionGreece_ExchangeID_str_ptr = pRspOptionGreece_ExchangeID_str.c_str();
		std::string pRspOptionGreece_OptionsType_str = char2str(pRspOptionGreece->OptionsType);
		const char*  pRspOptionGreece_OptionsType_str_ptr = pRspOptionGreece_OptionsType_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryRspOptionGreece IsLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s OptionsType=%s Premium=%f UnderlyingSecurityPrice=%f StrikePrice=%f RiskFreeInterestRate=%f ImpliedVolatility=%f IntrinsicValue=%f TimeValue=%f Leverage=%f RealLeverage=%f Delta=%f Gamma=%f Theta=%f Vega=%f Rho=%f   \n", bIsLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pRspOptionGreece_ExchangeID_str_ptr, pRspOptionGreece->SecurityID, pRspOptionGreece_OptionsType_str_ptr, pRspOptionGreece->Premium, pRspOptionGreece->UnderlyingSecurityPrice, pRspOptionGreece->StrikePrice, pRspOptionGreece->RiskFreeInterestRate, pRspOptionGreece->ImpliedVolatility, pRspOptionGreece->IntrinsicValue, pRspOptionGreece->TimeValue, pRspOptionGreece->Leverage, pRspOptionGreece->RealLeverage, pRspOptionGreece->Delta, pRspOptionGreece->Gamma, pRspOptionGreece->Theta, pRspOptionGreece->Vega, pRspOptionGreece->Rho);
	}
	if (bIsLast)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryRspOptionGreece End==========================================================================================================\n ");
	}
}

///订阅特定行情应答
void CQCVDSimpleSpi::OnRspSubSpecialMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubSpecialMarketData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}

///取消订阅特定行情应答
void CQCVDSimpleSpi::OnRspUnSubSpecialMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubSpecialMarketData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}

///订阅资金流向行情应答
void CQCVDSimpleSpi::OnRspSubFundsFlowMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubFundsFlowMarketData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}

///取消订阅资金流向行情应答
void CQCVDSimpleSpi::OnRspUnSubFundsFlowMarketData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubFundsFlowMarketData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///订阅行业指数行情应答
void CQCVDSimpleSpi::OnRspSubIndustryIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubIndustryIndexData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///取消订阅行业指数行情应答
void CQCVDSimpleSpi::OnRspUnSubIndustryIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubIndustryIndexData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///订阅概念指数行情应答
void CQCVDSimpleSpi::OnRspSubConceptionIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubConceptionIndexData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///取消订阅概念指数行情应答
void CQCVDSimpleSpi::OnRspUnSubConceptionIndexData(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubConceptionIndexData ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///订阅价格异常波动委托明细应答
void CQCVDSimpleSpi::OnRspSubEffectOrderDetail(CQCVDEffectDetailItemField *pEffectDetailItem, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubEffectOrderDetail ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///取消订阅价格异常波动委托明细应答
void CQCVDSimpleSpi::OnRspUnSubEffectOrderDetail(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubEffectOrderDetail ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///订阅价格异常波动成交明细应答
void CQCVDSimpleSpi::OnRspSubEffectTradeDetail(CQCVDEffectDetailItemField *pEffectDetailItem, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubEffectTradeDetail ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}
///取消订阅价格异常波动成交明细应答
void CQCVDSimpleSpi::OnRspUnSubEffectTradeDetail(CQCVDSpecificSecurityField *pSpecificSecurity, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspUnSubEffectTradeDetail ErrorID=%d ErrorMsg=%s RequestID=%d \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID);
}

///特定行情通知
void CQCVDSimpleSpi::OnRtnSpecialMarketData(CQCVDSpecialMarketDataField *pSpecialMarketData)
{
	printf("OnRtnSpecialMarketData TradingDay=%s SecurityID=%s ExchangeID=%c SecurityName=%s MovingAvgPrice=%f MovingAvgPriceSamplingNum=%d UpdateTime=%s UpdateMillisec=%d \n",
		pSpecialMarketData->TradingDay, pSpecialMarketData->SecurityID, pSpecialMarketData->ExchangeID,
		pSpecialMarketData->SecurityName, pSpecialMarketData->MovingAvgPrice, pSpecialMarketData->MovingAvgPriceSamplingNum,
		pSpecialMarketData->UpdateTime, pSpecialMarketData->UpdateMillisec);
}

///资金流向行情通知
void CQCVDSimpleSpi::OnRtnFundsFlowMarketData(CQCVDFundsFlowMarketDataField *pFundsFlowMarketData)
{
	printf("OnRtnFundsFlowMarketData SecurityID=%s ExchangeID=%c RetailBuyTurnover=%f RetailBuyVolume=%lld RetailBuyAmount=%lld RetailSellTurnover=%f RetailSellVolume=%lld RetailSellAmount=%lld UpdateTime=%s UpdateMillisec=%d \n",
		pFundsFlowMarketData->SecurityID, pFundsFlowMarketData->ExchangeID,
		pFundsFlowMarketData->RetailBuyTurnover, pFundsFlowMarketData->RetailBuyVolume, pFundsFlowMarketData->RetailBuyAmount,
		pFundsFlowMarketData->RetailSellTurnover, pFundsFlowMarketData->RetailSellVolume, pFundsFlowMarketData->RetailSellAmount,
		pFundsFlowMarketData->UpdateTime, pFundsFlowMarketData->UpdateMillisec);
}

///价格波动异常行情通知
void CQCVDSimpleSpi::OnRtnEffectPriceMarketData(CQCVDEffectPriceMarketDataField *pEffectPriceMarketData)
{
	printf("OnRtnEffectPriceMarketData SecurityID=%s ExchangeID=%c LastPrice=%f TradeVol=%lld TradeTurnover=%f UpdateTime=%s UpdateMillisec=%d \n",
		pEffectPriceMarketData->SecurityID, pEffectPriceMarketData->ExchangeID,
		pEffectPriceMarketData->LastPrice, pEffectPriceMarketData->TradeVol, pEffectPriceMarketData->TradeTurnover,
		pEffectPriceMarketData->UpdateTime, pEffectPriceMarketData->UpdateMillisec);
}

///数量波动异常行情通知
void CQCVDSimpleSpi::OnRtnEffectVolumeMarketData(CQCVDEffectVolumeMarketDataField *pEffectVolumeMarketData)
{
	printf("OnRtnEffectVolumeMarketData SecurityID=%s ExchangeID=%c LastPrice=%f TradeVol=%lld TradeTurnover=%f UpdateTime=%s UpdateMillisec=%d\n",
		pEffectVolumeMarketData->SecurityID, pEffectVolumeMarketData->ExchangeID,
		pEffectVolumeMarketData->LastPrice, pEffectVolumeMarketData->TradeVol, pEffectVolumeMarketData->TradeTurnover,
		pEffectVolumeMarketData->UpdateTime, pEffectVolumeMarketData->UpdateMillisec);
}

///价格异常波动委托明细通知
void CQCVDSimpleSpi::OnRtnEffectOrderDetail(CQCVDEffectOrderDetailField *pEffectOrderDetail)
{
	printf("OnRtnEffectOrderDetailSecurityID=%s ExchangeID=%c Price=%f Volume=%lld OrderType=%c Side=%c EffectRatio=%f OrderSeq1=%d OrderSeq2=%d UpdateTime=%s UpdateMillisec=%d \n",
		pEffectOrderDetail->SecurityID, pEffectOrderDetail->ExchangeID,
		pEffectOrderDetail->Price, pEffectOrderDetail->Volume, pEffectOrderDetail->OrderType,
		pEffectOrderDetail->Side, pEffectOrderDetail->EffectRatio, pEffectOrderDetail->OrderSeq1,
		pEffectOrderDetail->OrderSeq2, pEffectOrderDetail->UpdateTime, pEffectOrderDetail->UpdateMillisec);
}

///价格异常波动成交明细通知
void CQCVDSimpleSpi::OnRtnEffectTradeDetail(CQCVDEffectTradeDetailField *pEffectTradeDetail)
{
	printf("OnRtnEffectTradeDetail=%s ExchangeID=%c TradePrice=%f TradeVolume=%lld ExecType=%c EffectRatio=%f TradeSeq1=%d TradeSeq2=%d BuySideSeq=%d SelSideSeq=%d UpdateTime=%s UpdateMillisec=%d \n",
		pEffectTradeDetail->SecurityID, pEffectTradeDetail->ExchangeID,
		pEffectTradeDetail->TradePrice, pEffectTradeDetail->TradeVolume, pEffectTradeDetail->ExecType,
		pEffectTradeDetail->EffectRatio, pEffectTradeDetail->TradeSeq1, pEffectTradeDetail->TradeSeq2,
		pEffectTradeDetail->BuySideSeq, pEffectTradeDetail->SelSideSeq, pEffectTradeDetail->UpdateTime, pEffectTradeDetail->UpdateMillisec);
}

///行业指数行情通知
void CQCVDSimpleSpi::OnRtnIndustryIndexData(CQCVDIndustryIndexDataField *pIndustryIndexData)
{
	printf("OnRtnIndustryIndexData TradingDay=%s IndexID=%s IndexPoint=%f PreClosePoint=%f OpenPoint=%f ClosePoint=%f  HighestPoint=%f LowestPoint=%f PageLocate=%d UpdateTime=%s UpdateMillisec=%d \n",
		pIndustryIndexData->TradingDay, pIndustryIndexData->IndexID, pIndustryIndexData->IndexPoint,
		pIndustryIndexData->PreClosePoint, pIndustryIndexData->OpenPoint, pIndustryIndexData->ClosePoint,
		pIndustryIndexData->HighestPoint, pIndustryIndexData->LowestPoint, pIndustryIndexData->PageLocate,
		pIndustryIndexData->UpdateTime, pIndustryIndexData->UpdateMillisec);
}

///概念指数行情通知
void CQCVDSimpleSpi::OnRtnConceptionIndexData(CQCVDConceptionIndexDataField *pConceptionIndexData)
{
	printf("OnRtnConceptionIndexData TradingDay=%s IndexID=%s IndexPoint=%f PreClosePoint=%f OpenPoint=%f ClosePoint=%f  HighestPoint=%f LowestPoint=%f PageLocate=%d UpdateTime=%s UpdateMillisec=%d \n",
		pConceptionIndexData->TradingDay, pConceptionIndexData->IndexID, pConceptionIndexData->IndexPoint,
		pConceptionIndexData->PreClosePoint, pConceptionIndexData->OpenPoint, pConceptionIndexData->ClosePoint,
		pConceptionIndexData->HighestPoint, pConceptionIndexData->LowestPoint, pConceptionIndexData->PageLocate,
		pConceptionIndexData->UpdateTime, pConceptionIndexData->UpdateMillisec);
}

//查询债券日K行情请求应答
void CQCVDSimpleSpi::OnRspInquiryBondDayQuotation(CQCVDBondDayQuotationField *pBondDayQuotation, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pBondDayQuotation)
	{
		std::string pBondDayQuotation_ExchangeID_str = char2str(pBondDayQuotation->ExchangeID);
		const char*  pBondDayQuotation_ExchangeID_str_ptr = pBondDayQuotation_ExchangeID_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryBondDayQuotation IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ExchangeID=%s SecurityID=%s TradingDay=%s CurrencyID=%s PreClosePrice=%f OpenPrice=%f HighestPrice=%f LowestPrice=%f ClosePrice=%f PriceChange=%f PriceChangePercentage=%f Volume=%f Amount=%f AveragePrice=%f TradeStatus=%s PageLocate=%d PageTotal=%d   \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pBondDayQuotation_ExchangeID_str_ptr, pBondDayQuotation->SecurityID, pBondDayQuotation->TradingDay, pBondDayQuotation->CurrencyID, pBondDayQuotation->PreClosePrice, pBondDayQuotation->OpenPrice, pBondDayQuotation->HighestPrice, pBondDayQuotation->LowestPrice, pBondDayQuotation->ClosePrice, pBondDayQuotation->PriceChange, pBondDayQuotation->PriceChangePercentage, pBondDayQuotation->Volume, pBondDayQuotation->Amount, pBondDayQuotation->AveragePrice, pBondDayQuotation->TradeStatus, pBondDayQuotation->PageLocate, pBondDayQuotation->PageTotal);
	}
	if (NULL == pBondDayQuotation)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryBondDayQuotation End==========================================================================================================\n ");
	}
}
//查询港股通资金流向信息请求应答
void CQCVDSimpleSpi::OnRspInquiryGGTEODPrices(CQCVDGGTEODPricesField *pGGTEODPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pGGTEODPrices)
	{
		std::string pGGTEODPrices_market_str = char2str(pGGTEODPrices->market);
		const char*  pGGTEODPrices_market_str_ptr = pGGTEODPrices_market_str.c_str();

		printf("CQCValueAddProSpi OnRspInquiryGGTEODPrices IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s ID=%lld market=%s dayNetAmtIn=%f dayAmtRemain=%f dayAmtThreshold=%f netBuyAmt=%f netSellAmt=%f rise=%d flat=%d fall=%d leadingStock=%s leadingStockPrice=%f leadingStockPercent=%f TradingDay=%s UpdateTime=%s leadingStockCode=%s PageLocate=%d PageTotal=%d  \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pGGTEODPrices->ID, pGGTEODPrices_market_str_ptr, pGGTEODPrices->dayNetAmtIn, pGGTEODPrices->dayAmtRemain, pGGTEODPrices->dayAmtThreshold, pGGTEODPrices->netBuyAmt, pGGTEODPrices->netSellAmt, pGGTEODPrices->rise, pGGTEODPrices->flat, pGGTEODPrices->fall, pGGTEODPrices->leadingStock, pGGTEODPrices->leadingStockPrice, pGGTEODPrices->leadingStockPercent, pGGTEODPrices->TradingDay, pGGTEODPrices->UpdateTime, pGGTEODPrices->leadingStockCode, pGGTEODPrices->PageLocate, pGGTEODPrices->PageTotal);
	}
	if (NULL == pGGTEODPrices)
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspInquiryGGTEODPrices End==========================================================================================================\n ");
	}
}
//
void CQCVDSimpleSpi::OnRspQryCBondConvPrice(CQCVDCBondConvPriceField *pCBondConvPrice, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCBondConvPrice)
	{
		printf("CQCValueAddProSpi OnRspQryCBondConvPrice IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s announcementDate=%s changeReason=%s convPrice=%f endDate=%s exchangeID=%c PageLocate=%d PageTotal=%d securityID=%s \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCBondConvPrice->announcementDate, pCBondConvPrice->changeReason, pCBondConvPrice->convPrice, pCBondConvPrice->endDate, pCBondConvPrice->exchangeID, pCBondConvPrice->pageLocate, pCBondConvPrice->pageTotal, pCBondConvPrice->securityID);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCBondConvPrice End==========================================================================================================\n ");
	}
}

//
void CQCVDSimpleSpi::OnRspQryChinaClosedFund(CQCVDChinaClosedFundField *pChinaClosedFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pChinaClosedFund)
	{
		printf("CQCValueAddProSpi OnRspQryChinaClosedFund IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s adjClose=%f adjFactor=%f adjHigh=%f adjLow=%f adjOpen=%f adjPreClose=%f amount=%f change=%f close=%f crncyCode=%s discountRate=%f fundId=%s high=%f low=%f open=%f pageLocate=%d pageTotal=%d preClose=%f ptcChange=%f tardeDate=%s tradeCount=%d volume=%d \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pChinaClosedFund->adjClose, pChinaClosedFund->adjFactor, pChinaClosedFund->adjHigh, pChinaClosedFund->adjLow, pChinaClosedFund->adjOpen, pChinaClosedFund->adjPreClose, pChinaClosedFund->amount, pChinaClosedFund->change, pChinaClosedFund->close, pChinaClosedFund->crncyCode, pChinaClosedFund->discountRate, pChinaClosedFund->fundId, pChinaClosedFund->high, pChinaClosedFund->low, pChinaClosedFund->open, pChinaClosedFund->pageLocate, pChinaClosedFund->pageTotal, pChinaClosedFund->preClose, pChinaClosedFund->ptcChange, pChinaClosedFund->tardeDate, pChinaClosedFund->tradeCount, pChinaClosedFund->volume);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryChinaClosedFund End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryChinaHedgeFund(CQCVDChinaHedgeFundField *pChinaHedgeFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pChinaHedgeFund)
	{
		printf("CQCValueAddProSpi OnRspQryChinaHedgeFund IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s annDate=%s crncyCode=%s fundId=%s navAccumulated=%f navDivAccumulated=%f navFactor=%f navUnit=%f pageLocate=%d pageTotal=%d priceDate=%s \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pChinaHedgeFund->annDate, pChinaHedgeFund->crncyCode, pChinaHedgeFund->fundId, pChinaHedgeFund->navAccumulated, pChinaHedgeFund->navDivAccumulated, pChinaHedgeFund->navFactor, pChinaHedgeFund->navUnit, pChinaHedgeFund->pageLocate, pChinaHedgeFund->pageTotal, pChinaHedgeFund->priceDate);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryChinaHedgeFund End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryChinaMutualFund(CQCVDChinaMutualFundField *pChinaMutualFund, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pChinaMutualFund)
	{
		printf("CQCValueAddProSpi OnRspQryChinaMutualFund IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s annDate=%s assetMergedSharesOrNot=%d crncyCode=%s exdividendDate=%d navAccumulated=%f navAdjFactor=%f navAdjusted=%f navDistribution=%f navDivaccumulated=%f navUnit=%f netAssetTotal=%f pageLocate=%d pageTotal=%d priceDate=%s prtNetasset=%f tradeCode=%s \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pChinaMutualFund->annDate, pChinaMutualFund->assetMergedSharesOrNot, pChinaMutualFund->crncyCode, pChinaMutualFund->exdividendDate, pChinaMutualFund->navAccumulated, pChinaMutualFund->navAdjFactor, pChinaMutualFund->navAdjusted, pChinaMutualFund->navDistribution, pChinaMutualFund->navDivaccumulated, pChinaMutualFund->navUnit, pChinaMutualFund->netAssetTotal, pChinaMutualFund->pageLocate, pChinaMutualFund->pageTotal, pChinaMutualFund->priceDate, pChinaMutualFund->prtNetasset, pChinaMutualFund->tradeCode);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryChinaMutualFund End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryChinaOptionEodPrices(CQCVDChinaOptionEodPricesField *pChinaOptionEodPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pChinaOptionEodPrices)
	{
		printf("CQCValueAddProSpi OnRspQryChinaOptionEodPrices IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s closePrice=%f endPrice=%f endPriorPrice=%f exchangeCode=%s highPrice=%f lowPrice=%f openPrice=%f pageLocate=%d pageTotal=%d priorPrice=%f saveVolume=%d sucessAmout=%f sucessVolume=%d totalPriorPrice=%f tradeCode=%s tradeDt=%s \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pChinaOptionEodPrices->closePrice, pChinaOptionEodPrices->endPrice, pChinaOptionEodPrices->endPriorPrice, pChinaOptionEodPrices->exchangeCode, pChinaOptionEodPrices->highPrice, pChinaOptionEodPrices->lowPrice, pChinaOptionEodPrices->openPrice, pChinaOptionEodPrices->pageLocate, pChinaOptionEodPrices->pageTotal, pChinaOptionEodPrices->priorPrice, pChinaOptionEodPrices->saveVolume, pChinaOptionEodPrices->sucessAmout, pChinaOptionEodPrices->sucessVolume, pChinaOptionEodPrices->totalPriorPrice, pChinaOptionEodPrices->tradeCode, pChinaOptionEodPrices->tradeDt);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryChinaOptionEodPrices End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryCindexfutureseodPrices(CQCVDCindexfutureseodPricesField *pCindexfutureseodPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCindexfutureseodPrices)
	{
		printf("CQCValueAddProSpi OnRspQryCindexfutureseodPrices IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s changeCount=%f closePrice=%f endPrice=%f exchangeCode=%s highPrice=%f infoType=%c lowPrice=%f openPrice=%f pageLocate=%d pageTotal=%d priorPrice=%f saveVolume=%d tradeCode=%s sucessAmout=%f sucessVolume=%d tradeDt=%s \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCindexfutureseodPrices->changeCount, pCindexfutureseodPrices->closePrice, pCindexfutureseodPrices->endPrice, pCindexfutureseodPrices->exchangeCode, pCindexfutureseodPrices->highPrice, pCindexfutureseodPrices->infoType, pCindexfutureseodPrices->lowPrice, pCindexfutureseodPrices->openPrice, pCindexfutureseodPrices->pageLocate, pCindexfutureseodPrices->pageTotal, pCindexfutureseodPrices->priorPrice, pCindexfutureseodPrices->saveVolume, pCindexfutureseodPrices->tradeCode, pCindexfutureseodPrices->sucessAmout, pCindexfutureseodPrices->sucessVolume, pCindexfutureseodPrices->tradeDt);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCindexfutureseodPrices End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryCMoneyMarketDailyFIncome(CQCVDCMoneyMarketDailyFIncomeField *pCMoneyMarketDailyFIncome, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCMoneyMarketDailyFIncome)
	{
		printf("CQCValueAddProSpi OnRspQryCMoneyMarketDailyFIncome IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s accunitNav=%f annDate=%s beginDate=%s endDate=%s incomePerMillion=%f pageLocate=%d pageTotal=%d tradeCode=%s unityield=%f yearlyROE=%f \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCMoneyMarketDailyFIncome->accunitNav, pCMoneyMarketDailyFIncome->annDate, pCMoneyMarketDailyFIncome->beginDate, pCMoneyMarketDailyFIncome->endDate, pCMoneyMarketDailyFIncome->incomePerMillion, pCMoneyMarketDailyFIncome->pageLocate, pCMoneyMarketDailyFIncome->pageTotal, pCMoneyMarketDailyFIncome->tradeCode, pCMoneyMarketDailyFIncome->unityield, pCMoneyMarketDailyFIncome->yearlyROE);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCMoneyMarketDailyFIncome End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryCommodityFuturesPrice(CQCVDCommodityFuturesPriceField *pCommodityFuturesPrice, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pCommodityFuturesPrice)
	{
		printf("CQCValueAddProSpi OnRspQryCommodityFuturesPrice IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s amount=%f change=%f close=%f exchangeCode=%s fsInfoType=%c high=%f low=%f oi=%d oiChange=%d open=%f pageLocate=%d pageTotal=%d presettle=%f settle=%f tradeCode=%s tradeDate=%s volume=%d \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pCommodityFuturesPrice->amount, pCommodityFuturesPrice->change, pCommodityFuturesPrice->close, pCommodityFuturesPrice->exchangeCode, pCommodityFuturesPrice->fsInfoType, pCommodityFuturesPrice->high, pCommodityFuturesPrice->low, pCommodityFuturesPrice->oi, pCommodityFuturesPrice->oiChange, pCommodityFuturesPrice->open, pCommodityFuturesPrice->pageLocate, pCommodityFuturesPrice->pageTotal, pCommodityFuturesPrice->presettle, pCommodityFuturesPrice->settle, pCommodityFuturesPrice->tradeCode, pCommodityFuturesPrice->tradeDate, pCommodityFuturesPrice->volume);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryCommodityFuturesPrice End==========================================================================================================\n");
	}
}

//
void CQCVDSimpleSpi::OnRspQryGoldSpotPrices(CQCVDGoldSpotPricesField *pGoldSpotPrices, CQCVDRspInfoField *pRspInfo, int nRequestID, bool bIsPageLast, bool bIsTotalLast)
{
	if (NULL != pGoldSpotPrices)
	{
		printf("CQCValueAddProSpi OnRspQryGoldSpotPrices IsPageLast=%d bIsTotalLast=%d RequestID=%d ErrorID=%d ErrorMsg=%s amount=%f avgPrice=%f close=%f delAmt=%d delayPayTypeCode=%s exchangeCode=%s high=%f low=%f oi=%d open=%f pageLocate=%d pageTotal=%d pctChg=%f settle=%f tradeCode=%s tradeDate=%s volume=%d \n", bIsPageLast, bIsTotalLast, nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg, pGoldSpotPrices->amount, pGoldSpotPrices->avgPrice, pGoldSpotPrices->close, pGoldSpotPrices->delAmt, pGoldSpotPrices->delayPayTypeCode, pGoldSpotPrices->exchangeCode, pGoldSpotPrices->high, pGoldSpotPrices->low, pGoldSpotPrices->oi, pGoldSpotPrices->open, pGoldSpotPrices->pageLocate, pGoldSpotPrices->pageTotal, pGoldSpotPrices->pctChg, pGoldSpotPrices->settle, pGoldSpotPrices->tradeCode, pGoldSpotPrices->tradeDate, pGoldSpotPrices->volume);
	}
	else
	{
		this->m_is_qrying.store(false);
		printf("CQCValueAddProSpi OnRspQryGoldSpotPrices End==========================================================================================================\n");
	}
}

///订阅快速股票指数行情应答
void CQCVDSimpleSpi::OnRspSubRapidSecurityIndexData(CQCVDSpecificSecurityField* pSpecificSecurity, CQCVDRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspSubRapidSecurityIndexData: ErrorID=%d ErrorMsg=%s\n",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}

///快速股票指数行情通知
void CQCVDSimpleSpi::OnRtnRapidSecurityIndexData(CQCVDRapidSecurityIndexDataField* pRapidSecurityIndexData)
{
	if (pRapidSecurityIndexData)
	{
	  ::Record(pRapidSecurityIndexData);
    /*
		printf("OnRtnRapidSecurityIndexData:TradingDay=%s ExchangeID=%c SecurityID=%s SecurityName=%s PreClosePrice=%f"
											"OpenPrice=%f Volume=%lld Turnover=%f LastPrice=%f HighestPrice=%f"
											"LowestPrice=%f ClosePrice=%f UpdateTime=%s UpdateMillisec=%d\n",
			pRapidSecurityIndexData->TradingDay, pRapidSecurityIndexData->ExchangeID, pRapidSecurityIndexData->SecurityID, pRapidSecurityIndexData->SecurityName, pRapidSecurityIndexData->PreClosePrice, 
			pRapidSecurityIndexData->OpenPrice, pRapidSecurityIndexData->Volume, pRapidSecurityIndexData->Turnover, pRapidSecurityIndexData->LastPrice, pRapidSecurityIndexData->HighestPrice, 
			pRapidSecurityIndexData->LowestPrice, pRapidSecurityIndexData->ClosePrice, pRapidSecurityIndexData->UpdateTime, pRapidSecurityIndexData->UpdateMillisec);		
		*/
	}
}

