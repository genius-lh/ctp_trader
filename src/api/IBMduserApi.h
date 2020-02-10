#if !defined(IB_MDUSER_API_H)
#define IB_MDUSER_API_H

#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

class CIBMduserApi : public CIbTraderApiBase
{
public:
	static CIBMduserApi *CreateIBMduserApi(const char *pszFlowPath = "");
  
	///���ĺ�Լ���顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int SubMarketData(char *ppInstrumentID[], int nCount);

	///�˶���Լ���顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int UnSubMarketData(char *ppInstrumentID[], int nCount);

  void ReqMarketDataType(int marketDataType);
  
private:
  void ReqMarketData(IBFutureContract* future);
  void CancelMarketData(IBFutureContract* future);

};

#endif
