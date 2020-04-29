#ifndef _CTP_SOP_MDUSER_TEST_H_
#define _CTP_SOP_MDUSER_TEST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

//using namespace ctp_sopt;

class CFemasMduserHandler : public CUstpFtdcMduserSpi
{
public:
  CFemasMduserHandler();

  virtual ~CFemasMduserHandler();
  
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void OnFrontDisconnected(int nReason);

	///����Ӧ��
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ǰ��ϵͳ�û���¼Ӧ��
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��˳�Ӧ��
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯӦ��
	void OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData);

	///���ĺ�Լ�������Ϣ
	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�˶���Լ�������Ϣ
	void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


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
  CUstpFtdcDepthMarketDataField* m_Buffer;
  
  void* m_Arg;
  long m_RequestId;


public:
  void Loop();
  int ShowMenu();
  void LogOut();
  void SubMarketData();
  void UnSubMarketData();
  int Check();
  void Flush();

};


#endif //_CTP_SOP_MDUSER_TEST_H_

