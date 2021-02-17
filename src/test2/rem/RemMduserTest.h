#ifndef _REM_MDUSER_TEST_H_
#define _REM_MDUSER_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

//using namespace ctp_sopt;

class CRemMduserTestHandler : public EESQuoteEvent
{
public:
  CRemMduserTestHandler();

  virtual ~CRemMduserTestHandler();
  
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

	/// \brief ��־�ӿڣ���ʹ���߰���д��־��
	/// \param nlevel    ��־����
	/// \param pLogText  ��־����
	/// \param nLogLen   ��־����
    virtual void OnWriteTextLog(EesEqsLogLevel nlevel, const char* pLogText, int nLogLen) {}

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
	
	/// \brief ��ѯsymbol�б���Ӧ��Ϣ��ʱ���ã��鲥ģʽ��֧�ֺ�Լ�б��ѯ
	/// \param chInstrumentType  EES��������
	/// \param pSymbol           ��Լ����
	/// \param bLast             ���һ����ѯ��Լ�б���Ϣ�ı�ʶ
	/// \remark ��ѯ��Լ�б���Ӧ, last = trueʱ��������������Ч���ݡ�
    virtual void OnSymbolListResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bLast)  {}


public:
  char* m_BrokerID;
  char* m_UserId;
  char* m_OldPasswd;
  char* m_Filename;
  int m_Loop;

  time_t m_LastTm;
  int m_Gap;
  int m_BufferSize;
  int m_Cursor;
  EESMarketDepthQuoteData* m_Buffer;
  
  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void SubMarketData();
  void UnSubMarketData();
  int Check();
  void Flush();

};


#endif //_REM_MDUSER_TEST_H_

