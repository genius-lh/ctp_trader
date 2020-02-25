#if !defined(IB_TRADER_API_H)
#define IB_TRADER_API_H

#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

typedef char TIBInstrumentIDType[32];
typedef long TIBUserOrderLocalIDType;
typedef char TIBDirectionType;
typedef double TIBPriceType;
typedef int TIBVolumeType;


///输入报单
struct CIBInputOrderField
{
	///合约代码
	TIBInstrumentIDType	InstrumentID;
	///用户本地报单号
	TIBUserOrderLocalIDType	UserOrderLocalID;
	///买卖方向
	TIBDirectionType	Direction;
	///价格
	TIBPriceType	LimitPrice;
	///数量
	TIBVolumeType	Volume;
};

///报单操作
struct CIBOrderActionField
{
	///本次撤单操作的本地编号
	TIBUserOrderLocalIDType	UserOrderActionLocalID;
};

class CIBTraderApi : public CIbTraderApiBase
{
public:
	static CIBTraderApi *CreateIBTraderApi(const char *pszFlowPath = "");
  
	///报单录入请求
	int ReqOrderInsert(CIBInputOrderField *pInputOrder);

	///报单操作请求
	int ReqOrderAction(CIBOrderActionField *pOrderAction);

  void ReqOrderId();

  void ReqPositions();

private:
  inline const char* ConvOrderAction(char direction);

};


#endif

