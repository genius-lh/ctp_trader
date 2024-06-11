#ifndef _REM_TEST_H_
#define _REM_TEST_H_
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <string>
#include "EesTraderDefine.h"
#include "EesTraderErr.h"
#include "EesTraderApi.h"

#include "EesQueryDefine.h"
#include "EesQueryErr.h"
#include "EesQueryApi.h"

class CRem5TraderHandler : public EESTraderEvent
{
public:
  CRem5TraderHandler(void* handler);
  virtual ~CRem5TraderHandler();

	/// ������Ϣ�Ļص�
	
		///	\brief	�����������¼�
		///	\param  errNo                   ���ӳɹ���������Ϣ
		///	\param  pErrStr                 ������Ϣ
		///	\return void  
	
	void OnConnection(ERR_NO errNo, const char* pErrStr );

	/// ���ӶϿ���Ϣ�Ļص�
	
		/// \brief	�����������Ͽ������յ������Ϣ
		/// \param  ERR_NO errNo         ���ӳɹ���������Ϣ
		/// \param  const char* pErrStr  ������Ϣ
		/// \return void  
	
	void OnDisConnection(ERR_NO errNo, const char* pErrStr );

	/// ��¼��Ϣ�Ļص�
	
		/// \param  pLogon                  ��¼�ɹ�����ʧ�ܵĽṹ
		/// \return void 
	
	void OnUserLogon(EES_LogonResponse* pLogon, MY_SOCKET m_sock = -1);

  
	/// �µ�����̨ϵͳ���ܵ��¼�
	
		/// \brief ��ʾ��������Ѿ�����̨ϵͳ��ʽ�Ľ���
		/// \param  pAccept	                    �����������Ժ����Ϣ��
		/// \return void 
	void OnOrderAccept(EES_OrderAcceptField* pAccept );

  /// �µ����г����ܵ��¼�

      /// \brief ��ʾ��������Ѿ�����������ʽ�Ľ���
      /// \param  pAccept                     �����������Ժ����Ϣ�壬����������г�����ID
      /// \return void 
  void OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept);

  /// �����г�״̬�����仯

  /// \brief ��ʾ�������״̬�����˱仯
  /// \param  pReport                     
  /// \return void 
  void OnOrderMarketReport(EES_OrderMarketReportField* pReport);

	///	�µ����г��ܾ����¼�

	/// \brief	�������г��ܾ��� 
	/// \param  pReject	                    
	/// \return void 

	void OnOrderMarketReject(EES_OrderMarketRejectField* pReject);

  /// �µ�����̨ϵͳ�ܾ����¼�

    /// \brief  ��������̨ϵͳ�ܾ������Բ鿴�﷨�����Ƿ�ؼ�顣 
    /// \param  pReject                     
    /// \return void 

  void OnOrderReject(EES_OrderRejectField* pReject );

  
	///	�����ɽ�����Ϣ�¼�
	
		/// \brief	�ɽ���������˶����г�ID�����������ID��ѯ��Ӧ�Ķ���
		/// \param  pExec	                   
		/// \return void 
	
	void OnOrderExecution(EES_OrderExecutionField* pExec );

	///	������������̨����
	
		/// \brief	
		/// \param  pCxled		               
		/// \return void 
	
	void OnOrderActionAccept(EES_OrderActionAccept* pCxled );
  
	///	������������̨�ܾ�����Ϣ�¼�
	
		/// \brief	һ����ڷ��Ͷ��������Ժ��յ������Ϣ����ʾ�����������ܾ�
		/// \param  pReject	                   �ܾ���Ϣ��
		/// \return void 
	
	void OnOrderActonReject(EES_OrderActionRej* pReject );

private:
  void* m_Handler;

};

class CRem5QueryHandler : public EESQueryEvent
{
public:
  CRem5QueryHandler(void* handler);
  virtual ~CRem5QueryHandler();
	/// ������Ϣ�Ļص�
	
		///	\brief	�����������¼�
		///	\param  errNo                   ���ӳɹ���������Ϣ
		///	\param  pErrStr                 ������Ϣ
		///	\return void  
	
	void OnQueryConnection(ERR_NO errNo, const char* pErrStr );

	/// ���ӶϿ���Ϣ�Ļص�
	
		/// \brief	�����������Ͽ������յ������Ϣ
		/// \param  ERR_NO errNo         ���ӳɹ���������Ϣ
		/// \param  const char* pErrStr  ������Ϣ
		/// \return void  
	
	void OnQueryDisConnection(ERR_NO errNo, const char* pErrStr );

	/// ��¼��Ϣ�Ļص�
	
		/// \param  pLogon                  ��¼�ɹ�����ʧ�ܵĽṹ
		/// \return void 
	
	void OnQueryUserLogon(EES_QueryLogonResponse* pLogon);

	/// �޸�������Ӧ�ص�

	/// \param  nResult                  ��������Ӧ�ĳɹ���񷵻���
	/// \return void 

	void OnQueryRspChangePassword(EES_ChangePasswordResult nResult);

	/// ��ѯ�û������ʻ��ķ����¼�
	
		/// \param  pAccountInfo	        �ʻ�����Ϣ
		/// \param  bFinish	                ���û�д�����ɣ����ֵ�� false ���������ˣ��Ǹ����ֵΪ true 
		/// \remark ������� bFinish == true����ô�Ǵ������������ pAccountInfoֵ��Ч��
		/// \return void 
	
	void OnQueryUserAccount(EES_AccountInfo * pAccoutnInfo, bool bFinish);

	/// ��ѯ�ʻ������ڻ���λ��Ϣ�ķ����¼�	
		/// \param  pAccount	                �ʻ�ID 	
		/// \param  pAccoutnPosition	        �ʻ��Ĳ�λ��Ϣ					   
		/// \param  bFinish	                    ���û�д�����ɣ����ֵ��false���������ˣ��Ǹ����ֵΪ true 
		/// \remark ������� bFinish == true����ô�Ǵ������������ pAccountInfoֵ��Ч��
		/// \return void 	
	void OnQueryAccountPosition(const char* pAccount, EES_AccountPosition* pAccoutnPosition, bool bFinish);

  
	/// ��ѯ�ʻ������ʽ���Ϣ�ķ����¼�
	
		/// \param  pAccount	                �ʻ�ID 	
		/// \param  pAccoutnBP	                �ʻ����ʽ���Ϣ					   
		/// \return void 
	
	void OnQueryAccountBP(const char* pAccount, EES_AccountBP* pAccoutnBP);


  /// ��ѯ��Լ�б�ķ����¼�

    /// \param  pSymbol                     ��Լ��Ϣ   
    /// \param  bFinish                     ���û�д�����ɣ����ֵ�� false���������ˣ��Ǹ����ֵΪ true   
    /// \remark ������� bFinish == true����ô�Ǵ������������ pSymbol ֵ��Ч��
    /// \return void 

  void OnQuerySymbol(EES_SymbolField* pSymbol, bool bFinish);

  /// ��ѯ�����ķ����¼�

    /// \brief  ��ѯ������Ϣʱ��Ļص���������Ҳ���ܰ������ǵ�ǰ�û��µĶ���
    /// \param  pAccount                 �ʻ�ID 
    /// \param  pQueryOrder              ��ѯ�����Ľṹ
    /// \param  bFinish                  ���û�д�����ɣ����ֵ�� false���������ˣ��Ǹ����ֵΪ true    
    /// \remark ������� bFinish == true����ô�Ǵ������������ pQueryOrderֵ��Ч��
    /// \return void 

  void OnQueryTradeOrder(const char* pAccount, EES_QueryAccountOrder* pQueryOrder, bool bFinish  );

  /// ��ѯ�����ķ����¼�

    /// \brief  ��ѯ������Ϣʱ��Ļص���������Ҳ���ܰ������ǵ�ǰ�û��µĶ����ɽ�
    /// \param  pAccount                        �ʻ�ID 
    /// \param  pQueryOrderExec                 ��ѯ�����ɽ��Ľṹ
    /// \param  bFinish                         ���û�д�����ɣ����ֵ��false���������ˣ��Ǹ����ֵΪ true    
    /// \remark ������� bFinish == true����ô�Ǵ������������pQueryOrderExecֵ��Ч��
    /// \return void 

  void OnQueryTradeOrderExec(const char* pAccount, EES_QueryOrderExecution* pQueryOrderExec, bool bFinish  );

private:
  void* m_Handler;
};

class CRem5TestHandler
{
public:
  CRem5TestHandler();
  virtual ~CRem5TestHandler();

public:
  
  char* m_AppID;
  char* m_AuthCode;
  char* m_UserId;
  char* m_Passwd;
  int m_Loop;

  void* m_TraderApi;
  void* m_QueryApi;
  
  long m_RequestId;
  char m_InvestorID[16];
  
public:
  void Loop();
  int ShowMenu();
  void Login();
  void Logout();
  void OrderInsert();
  void OrderAction();
  void QryOrder();
  void QryTrade();
  void QryUserInvestor();
  void QryInvestorAccount();
  void QryInstrument();
  void QryExchange();
  void QryInvestorPosition();
  void PrintOrder(void* data);
  void PrintTrade(void* data);
  void ChangePassword();

};

#endif //_REM_TEST_H_

