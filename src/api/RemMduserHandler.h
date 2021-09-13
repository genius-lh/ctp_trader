#ifndef _REM_MDUSER_HANDLER_H_
#define _REM_MDUSER_HANDLER_H_

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

class CRemMduserHandler : public EESQuoteEvent
{
public:   
  CRemMduserHandler(EESQuoteApi * pApi, void * pArg);
  ~CRemMduserHandler();
  
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
    void* m_Arg;
    EESQuoteApi* m_QuoteApi;
    
};

#endif //_REM_MDUSER_HANDLER_H_
