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
  
	/// \brief 当服务器连接成功，登录前调用, 如果是组播模式不会发生, 只需判断InitMulticast返回值即可
  void OnEqsConnected();

	/// \brief 当服务器曾经连接成功，被断开时调用，组播模式不会发生该事件
  void OnEqsDisconnected();

	/// \brief 当登录成功或者失败时调用，组播模式不会发生
	/// \param bSuccess 登陆是否成功标志  
	/// \param pReason  登陆失败原因  
  void OnLoginResponse(bool bSuccess, const char* pReason);

	/// \brief 收到行情时调用,具体格式根据instrument_type不同而不同
	/// \param chInstrumentType  EES行情类型
	/// \param pDepthQuoteData   EES统一行情指针  
  void OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData);

	/// \brief 日志接口，让使用者帮助写日志。
	/// \param nlevel    日志级别
	/// \param pLogText  日志内容
	/// \param nLogLen   日志长度
    virtual void OnWriteTextLog(EesEqsLogLevel nlevel, const char* pLogText, int nLogLen) {}

	/// \brief 注册symbol响应消息来时调用，组播模式不支持行情注册
	/// \param chInstrumentType  EES行情类型
	/// \param pSymbol           合约名称
	/// \param bSuccess          注册是否成功标志
  void OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);

	/// \brief  注销symbol响应消息来时调用，组播模式不支持行情注册
	/// \param chInstrumentType  EES行情类型
	/// \param pSymbol           合约名称
	/// \param bSuccess          注册是否成功标志
  void OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess);
	
	/// \brief 查询symbol列表响应消息来时调用，组播模式不支持合约列表查询
	/// \param chInstrumentType  EES行情类型
	/// \param pSymbol           合约名称
	/// \param bLast             最后一条查询合约列表消息的标识
	/// \remark 查询合约列表响应, last = true时，本条数据是无效数据。
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

