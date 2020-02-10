#if !defined(IB_MDUSER_API_H)
#define IB_MDUSER_API_H

#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

class CIBMduserApi : public CIbTraderApiBase
{
public:
	static CIBMduserApi *CreateIBMduserApi(const char *pszFlowPath = "");
  
	///订阅合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int SubMarketData(char *ppInstrumentID[], int nCount);

	///退订合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int UnSubMarketData(char *ppInstrumentID[], int nCount);

  void ReqMarketDataType(int marketDataType);
  
private:
  void ReqMarketData(IBFutureContract* future);
  void CancelMarketData(IBFutureContract* future);

};

#endif
