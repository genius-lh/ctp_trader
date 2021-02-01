#ifndef _REM_TRADER_HANDLER_H_
#define _REM_TRADER_HANDLER_H_
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <string>
#include "EesTraderApi.h"

class CRemTraderHandler : public EESTraderEvent
{
public:
  CRemTraderHandler(EESTraderApi* pApi, void* pArg);
  virtual ~CRemTraderHandler();
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
	
	void OnUserLogon(EES_LogonResponse* pLogon);

	/// �޸�������Ӧ�ص�

	/// \param  nResult                  ��������Ӧ�ĳɹ���񷵻���
	/// \return void 

	virtual void OnRspChangePassword(EES_ChangePasswordResult nResult){}

	/// ��ѯ�û������ʻ��ķ����¼�
	
		/// \param  pAccountInfo	        �ʻ�����Ϣ
		/// \param  bFinish	                ���û�д�����ɣ����ֵ�� false ���������ˣ��Ǹ����ֵΪ true 
		/// \remark ������� bFinish == true����ô�Ǵ������������ pAccountInfoֵ��Ч��
		/// \return void 
	
	void OnQueryUserAccount(EES_AccountInfo * pAccoutnInfo, bool bFinish);

	/// ��ѯ�ʻ������ڻ���λ��Ϣ�ķ����¼�	
		/// \param  pAccount	                �ʻ�ID 	
		/// \param  pAccoutnPosition	        �ʻ��Ĳ�λ��Ϣ					   
		/// \param  nReqId		                ����������Ϣʱ���ID�š�
		/// \param  bFinish	                    ���û�д�����ɣ����ֵ��false���������ˣ��Ǹ����ֵΪ true 
		/// \remark ������� bFinish == true����ô�Ǵ������������ pAccountInfoֵ��Ч��
		/// \return void 	
	void OnQueryAccountPosition(const char* pAccount, EES_AccountPosition* pAccoutnPosition, int nReqId, bool bFinish);

	/// ��ѯ�ʻ�������Ȩ��λ��Ϣ�ķ����¼�, ע������ص�, ����һ��OnQueryAccountPosition, ����һ��QueryAccountPosition�����, �ֱ𷵻�, �ȷ����ڻ�, �ٷ�����Ȩ, ��ʹû����Ȩ��λ, Ҳ�᷵��һ��bFinish=true�ļ�¼
	/// \param  pAccount	                �ʻ�ID 	
	/// \param  pAccoutnPosition	        �ʻ��Ĳ�λ��Ϣ					   
	/// \param  nReqId		                ����������Ϣʱ���ID�š�
	/// \param  bFinish	                    ���û�д�����ɣ����ֵ��false���������ˣ��Ǹ����ֵΪ true 
	/// \remark ������� bFinish == true����ô�Ǵ������������ pAccountInfoֵ��Ч��
	/// \return void 	
	virtual void OnQueryAccountOptionPosition(const char* pAccount, EES_AccountOptionPosition* pAccoutnOptionPosition, int nReqId, bool bFinish) {}


	/// ��ѯ�ʻ������ʽ���Ϣ�ķ����¼�
	
		/// \param  pAccount	                �ʻ�ID 	
		/// \param  pAccoutnPosition	        �ʻ��Ĳ�λ��Ϣ					   
		/// \param  nReqId		                ����������Ϣʱ���ID��
		/// \return void 
	
	void OnQueryAccountBP(const char* pAccount, EES_AccountBP* pAccoutnPosition, int nReqId );

	/// ��ѯ��Լ�б�ķ����¼�
	
		/// \param  pSymbol	                    ��Լ��Ϣ   
		/// \param  bFinish	                    ���û�д�����ɣ����ֵ�� false���������ˣ��Ǹ����ֵΪ true   
		/// \remark ������� bFinish == true����ô�Ǵ������������ pSymbol ֵ��Ч��
		/// \return void 
	
	void OnQuerySymbol(EES_SymbolField* pSymbol, bool bFinish);

	/// ��ѯ�ʻ����ױ�֤��ķ����¼�
	
	    /// \param  pAccount                    �ʻ�ID 
		/// \param  pSymbolMargin               �ʻ��ı�֤����Ϣ 
		/// \param  bFinish	                    ���û�д�����ɣ����ֵ�� false�������ɣ��Ǹ����ֵΪ true 
		/// \remark ������� bFinish == true����ô�Ǵ������������ pSymbolMargin ֵ��Ч��
		/// \return void 
	
	virtual void OnQueryAccountTradeMargin(const char* pAccount, EES_AccountMargin* pSymbolMargin, bool bFinish ){}

	/// ��ѯ�ʻ����׷��õķ����¼�
	
		/// \param  pAccount                    �ʻ�ID 
		/// \param  pSymbolFee	                �ʻ��ķ�����Ϣ	 
		/// \param  bFinish	                    ���û�д�����ɣ����ֵ�� false���������ˣ��Ǹ����ֵΪ true    
		/// \remark ������� bFinish == true ����ô�Ǵ������������ pSymbolFee ֵ��Ч��
		/// \return void 
	
	virtual void OnQueryAccountTradeFee(const char* pAccount, EES_AccountFee* pSymbolFee, bool bFinish ){}

	/// �µ�����̨ϵͳ���ܵ��¼�
	
		/// \brief ��ʾ��������Ѿ�����̨ϵͳ��ʽ�Ľ���
		/// \param  pAccept	                    �����������Ժ����Ϣ��
		/// \return void 
	
	void OnOrderAccept(EES_OrderAcceptField* pAccept );


	/// �µ����г����ܵ��¼�

	    /// \brief ��ʾ��������Ѿ�����������ʽ�Ľ���
	    /// \param  pAccept	                    �����������Ժ����Ϣ�壬����������г�����ID
	    /// \return void 
	void OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept);


	///	�µ�����̨ϵͳ�ܾ����¼�
	
		/// \brief	��������̨ϵͳ�ܾ������Բ鿴�﷨�����Ƿ�ؼ�顣 
		/// \param  pReject	                    �����������Ժ����Ϣ��
		/// \return void 
	
	void OnOrderReject(EES_OrderRejectField* pReject );


	///	�µ����г��ܾ����¼�

	/// \brief	�������г��ܾ������Բ鿴�﷨�����Ƿ�ؼ�顣 
	/// \param  pReject	                    �����������Ժ����Ϣ�壬����������г�����ID
	/// \return void 

	void OnOrderMarketReject(EES_OrderMarketRejectField* pReject);


	///	�����ɽ�����Ϣ�¼�
	
		/// \brief	�ɽ���������˶����г�ID�����������ID��ѯ��Ӧ�Ķ���
		/// \param  pExec	                   �����������Ժ����Ϣ�壬����������г�����ID
		/// \return void 
	
	void OnOrderExecution(EES_OrderExecutionField* pExec );

	///	�����ɹ������¼�
	
		/// \brief	�ɽ���������˶����г�ID�����������ID��ѯ��Ӧ�Ķ���
		/// \param  pCxled		               �����������Ժ����Ϣ�壬����������г�����ID
		/// \return void 
	
	void OnOrderCxled(EES_OrderCxled* pCxled );

	///	�������ܾ�����Ϣ�¼�
	
		/// \brief	һ����ڷ��ͳ����Ժ��յ������Ϣ����ʾ�������ܾ�
		/// \param  pReject	                   �������ܾ���Ϣ��
		/// \return void 
	
	void OnCxlOrderReject(EES_CxlOrderRej* pReject );

	///	��ѯ�����ķ����¼�
	
		/// \brief	��ѯ������Ϣʱ��Ļص���������Ҳ���ܰ������ǵ�ǰ�û��µĶ���
		/// \param  pAccount                 �ʻ�ID 
		/// \param  pQueryOrder	             ��ѯ�����Ľṹ
		/// \param  bFinish	                 ���û�д�����ɣ����ֵ�� false���������ˣ��Ǹ����ֵΪ true    
		/// \remark ������� bFinish == true����ô�Ǵ������������ pQueryOrderֵ��Ч��
		/// \return void 
	
	void OnQueryTradeOrder(const char* pAccount, EES_QueryAccountOrder* pQueryOrder, bool bFinish  );

	///	��ѯ�����ķ����¼�
	
		/// \brief	��ѯ������Ϣʱ��Ļص���������Ҳ���ܰ������ǵ�ǰ�û��µĶ����ɽ�
		/// \param  pAccount                        �ʻ�ID 
		/// \param  pQueryOrderExec	                ��ѯ�����ɽ��Ľṹ
		/// \param  bFinish	                        ���û�д�����ɣ����ֵ��false���������ˣ��Ǹ����ֵΪ true    
		/// \remark ������� bFinish == true����ô�Ǵ������������pQueryOrderExecֵ��Ч��
		/// \return void 
	
	void OnQueryTradeOrderExec(const char* pAccount, EES_QueryOrderExecution* pQueryOrderExec, bool bFinish  );

	///	�����ⲿ��������Ϣ
	
		/// \brief	һ�����ϵͳ�������������˹�������ʱ���õ���
		/// \param  pPostOrder	                    ��ѯ�����ɽ��Ľṹ
		/// \return void 
	
	virtual void OnPostOrder(EES_PostOrder* pPostOrder ){}	

	///	�����ⲿ�����ɽ�����Ϣ
	
		/// \brief	һ�����ϵͳ�������������˹�������ʱ���õ���
		/// \param  pPostOrderExecution	             ��ѯ�����ɽ��Ľṹ
		/// \return void 
	
	virtual void OnPostOrderExecution(EES_PostOrderExecution* pPostOrderExecution ){}

	///	��ѯ�������������ӵ���Ӧ

	/// \brief	ÿ����ǰϵͳ֧�ֵĻ㱨һ�Σ���bFinish= trueʱ����ʾ���н���������Ӧ���ѵ����������Ϣ�����������õ���Ϣ��
	/// \param  pPostOrderExecution	             ��ѯ�����ɽ��Ľṹ
	/// \return void 
	virtual void OnQueryMarketSession(EES_ExchangeMarketSession* pMarketSession, bool bFinish) {}

	///	����������״̬�仯���棬

	/// \brief	�����������ӷ�������/�Ͽ�ʱ�����״̬
	/// \param  MarketSessionId: ���������Ӵ���
	/// \param  ConnectionGood: true��ʾ����������������false��ʾ���������ӶϿ��ˡ�
	/// \return void 
	virtual void OnMarketSessionStatReport(EES_MarketSessionId MarketSessionId, bool ConnectionGood) {}

	///	��Լ״̬�仯����

	/// \brief	����Լ״̬�����仯ʱ����
	/// \param  pSymbolStatus: �μ�EES_SymbolStatus��Լ״̬�ṹ�嶨��
	/// \return void 
	void OnSymbolStatusReport(EES_SymbolStatus* pSymbolStatus);


	///	��Լ״̬��ѯ��Ӧ

	/// \brief  ��Ӧ��Լ״̬��ѯ����
	/// \param  pSymbolStatus: �μ�EES_SymbolStatus��Լ״̬�ṹ�嶨��
	/// \param	bFinish: ��Ϊtrueʱ����ʾ��ѯ���н�����ء���ʱpSymbolStatusΪ��ָ��NULL
	/// \return void 
	virtual void OnQuerySymbolStatus(EES_SymbolStatus* pSymbolStatus, bool bFinish) {}

	/// ��������ѯ��Ӧ
	/// \param	pMarketMBLData: �μ�EES_MarketMBLData�������ṹ�嶨��
	/// \param	bFinish: ��Ϊtrueʱ����ʾ��ѯ���н�����ء���ʱpMarketMBLData������,��m_RequestId��Ч
	/// \return void 
	virtual void OnQueryMarketMBLData(EES_MarketMBLData* pMarketMBLData, bool bFinish) {}

public 
  const char* GetAccountId();
  unsigned int GetTradingDate();
  EES_ClientToken GetMaxToken();

  void InsertOrder(char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
  void CancelOrder(char* inst, char* local_id, char* org_local_id, char* exchange_id, char* order_sys_id);

private:  
  void* m_Arg;
  EESTraderApi* m_TraderApi;
  EES_Account m_Account;
	unsigned int		m_TradingDate;						///< �����գ���ʽΪyyyyMMdd��int��ֵ
	EES_ClientToken		m_MaxToken;							///< ��ǰ����� token 
	std:map<EES_ClientToken, void*> mapOrder;
};

#endif //_REM_TRADER_HANDLER_H_

