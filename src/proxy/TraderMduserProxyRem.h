#ifndef _TRADER_MDUSER_PROXY_REM_H_
#define _TRADER_MDUSER_PROXY_REM_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

#include "TraderMduserProxyUtil.h"

class TraderMduserProxyRemHandler : public EESQuoteEvent
{
public:
  TraderMduserProxyRemHandler(TraderMduserProxyUtil* util);

  virtual ~TraderMduserProxyRemHandler();
  
	/// \brief �����������ӳɹ�����¼ǰ����, ������鲥ģʽ���ᷢ��, ֻ���ж�InitMulticast����ֵ����
  void OnEqsConnected();

	/// \brief ���������������ӳɹ������Ͽ�ʱ���ã��鲥ģʽ���ᷢ�����¼�
  void OnEqsDisconnected();

	/// \brief ����¼�ɹ�����ʧ��ʱ���ã��鲥ģʽ���ᷢ��
	/// \param bSuccess ��½�Ƿ�ɹ���־  
	/// \param pReason  ��½ʧ��ԭ��  
  void OnLoginResponse(bool bSuccess, const char* pReason);

	/// \brief �յ�����ʱ����,�����ʽ����instrument_type��ͬ����ͬ
	/// \param chInstrumentType  EES��������
	/// \param pDepthQuoteData   EESͳһ����ָ��  
  void OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData);

	/// \brief ע��symbol��Ӧ��Ϣ��ʱ���ã��鲥ģʽ��֧������ע��
	/// \param chInstrumentType  EES��������
	/// \param pSymbol           ��Լ����
	/// \param bSuccess          ע���Ƿ�ɹ���־
  void OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);

	/// \brief  ע��symbol��Ӧ��Ϣ��ʱ���ã��鲥ģʽ��֧������ע��
	/// \param chInstrumentType  EES��������
	/// \param pSymbol           ��Լ����
	/// \param bSuccess          ע���Ƿ�ɹ���־
  void OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);

private:
  char m_BrokerID[64];
  char m_UserId[64];
  char m_Passwd[64];
  char m_AddressIp[32];
  char m_AddressPort[8];
  
  void* m_Arg;
  long m_RequestId;

  int m_ContractIdx;

public:
  void loop();
  
private:
  void init();
  void login();
  void subMarketData();

private:
  TraderMduserProxyUtil* pProxyUtil;

};


#endif //_TRADER_MDUSER_PROXY_REM_H_

