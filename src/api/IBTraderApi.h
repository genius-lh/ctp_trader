#if !defined(IB_TRADER_API_H)
#define IB_TRADER_API_H

#include "IBTraderApiBase.h"
#include "IBFutureContractFactory.h"

typedef char TIBInstrumentIDType[32];
typedef long TIBUserOrderLocalIDType;
typedef char TIBDirectionType;
typedef double TIBPriceType;
typedef int TIBVolumeType;


///���뱨��
struct CIBInputOrderField
{
	///��Լ����
	TIBInstrumentIDType	InstrumentID;
	///�û����ر�����
	TIBUserOrderLocalIDType	UserOrderLocalID;
	///��������
	TIBDirectionType	Direction;
	///�۸�
	TIBPriceType	LimitPrice;
	///����
	TIBVolumeType	Volume;
};

///��������
struct CIBOrderActionField
{
	///���γ��������ı��ر��
	TIBUserOrderLocalIDType	UserOrderActionLocalID;
};

class CIBTraderApi : public CIbTraderApiBase
{
public:
	static CIBTraderApi *CreateIBTraderApi(const char *pszFlowPath = "");
  
	///����¼������
	int ReqOrderInsert(CIBInputOrderField *pInputOrder);

	///������������
	int ReqOrderAction(CIBOrderActionField *pOrderAction);

  void ReqOrderId();

  void ReqPositions();

private:
  inline const char* ConvOrderAction(char direction);

};


#endif

