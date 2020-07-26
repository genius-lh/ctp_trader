#if !defined(_XELE_FTDCTRADERORDERAPI_H)

#define _XELE_FTDCTRADERORDERAPI_H

#include "CXeleFtdcUserApiStruct.h"
#include "CXeleTraderApi.hpp"

class CXeleTraderOrderApi
{
public:
  ///����TraderApi
  ///@return ��������UserApi
  ///exchange=1(����������0���������н�����
  static CXeleTraderOrderApi *CreateTraderApi();

  static TXeleFtdcInsertType ConvertInsertType(char cTimeCondition, char cCombOffsetFlag,
    char cDirection, char cVolumeCondition);
  
  ///ɾ���ӿڶ�����
  ///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
  virtual void Release() = 0;

  ///��ʼ��
  ///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
  virtual void Init(TXeleFtdcClientIndexType* pClientIndex, TXeleFtdcClientTokenType* pToken) = 0;

  ///�ȴ��ӿ��߳̽�������
  ///@return �߳��˳�����
  virtual int Join() = 0;

  ///ע��ǰ�û������ַ
  ///@param pszFrontAddress��ǰ�û������ַ��
  ///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001����
  ///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
  virtual void RegisterFront(char *pszUdpFrontAddress, char *pszTcpQueryFrontAddress) = 0;

  ///ע��ص��ӿ�
  ///@param pSpi �����Իص��ӿ����ʵ��
  virtual void RegisterSpi(CXeleTraderSpi *pSpi) = 0;

  ///�û���¼����
  virtual int ReqUserLogin(int nRequestID) = 0;

  ///�û��˳�����
  virtual int ReqUserLogout(CXeleFtdcReqUserLogoutField *pReqUserLogout,
                            int nRequestID) = 0;

  ///����¼������
  virtual int ReqOrderInsert(CXeleFairInputOrderField *pInputOrder,
                             int nRequestID) = 0;

  ///������������
  virtual int ReqOrderAction(CXeleFairOrderActionField *pOrderAction,
                             int nRequestID) = 0;

  virtual ~CXeleTraderOrderApi() {};
};

#endif

