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

	/// 连接消息的回调
	
		///	\brief	服务器连接事件
		///	\param  errNo                   连接成功能与否的消息
		///	\param  pErrStr                 错误信息
		///	\return void  
	
	void OnConnection(ERR_NO errNo, const char* pErrStr );

	/// 连接断开消息的回调
	
		/// \brief	服务器主动断开，会收到这个消息
		/// \param  ERR_NO errNo         连接成功能与否的消息
		/// \param  const char* pErrStr  错误信息
		/// \return void  
	
	void OnDisConnection(ERR_NO errNo, const char* pErrStr );

	/// 登录消息的回调
	
		/// \param  pLogon                  登录成功或是失败的结构
		/// \return void 
	
	void OnUserLogon(EES_LogonResponse* pLogon, MY_SOCKET m_sock = -1);

  
	/// 下单被柜台系统接受的事件
	
		/// \brief 表示这个订单已经被柜台系统正式的接受
		/// \param  pAccept	                    订单被接受以后的消息体
		/// \return void 
	void OnOrderAccept(EES_OrderAcceptField* pAccept );

  /// 下单被市场接受的事件

      /// \brief 表示这个订单已经被交易所正式的接受
      /// \param  pAccept                     订单被接受以后的消息体，里面包含了市场订单ID
      /// \return void 
  void OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept);

  /// 订单市场状态发生变化

  /// \brief 表示这个订单状态发生了变化
  /// \param  pReport                     
  /// \return void 
  void OnOrderMarketReport(EES_OrderMarketReportField* pReport);

	///	下单被市场拒绝的事件

	/// \brief	订单被市场拒绝。 
	/// \param  pReject	                    
	/// \return void 

	void OnOrderMarketReject(EES_OrderMarketRejectField* pReject);

  /// 下单被柜台系统拒绝的事件

    /// \brief  订单被柜台系统拒绝，可以查看语法检查或是风控检查。 
    /// \param  pReject                     
    /// \return void 

  void OnOrderReject(EES_OrderRejectField* pReject );

  
	///	订单成交的消息事件
	
		/// \brief	成交里面包括了订单市场ID，建议用这个ID查询对应的订单
		/// \param  pExec	                   
		/// \return void 
	
	void OnOrderExecution(EES_OrderExecutionField* pExec );

	///	订单操作被柜台接受
	
		/// \brief	
		/// \param  pCxled		               
		/// \return void 
	
	void OnOrderActionAccept(EES_OrderActionAccept* pCxled );
  
	///	订单操作被柜台拒绝的消息事件
	
		/// \brief	一般会在发送订单操作以后，收到这个消息，表示订单操作被拒绝
		/// \param  pReject	                   拒绝消息体
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
	/// 连接消息的回调
	
		///	\brief	服务器连接事件
		///	\param  errNo                   连接成功能与否的消息
		///	\param  pErrStr                 错误信息
		///	\return void  
	
	void OnQueryConnection(ERR_NO errNo, const char* pErrStr );

	/// 连接断开消息的回调
	
		/// \brief	服务器主动断开，会收到这个消息
		/// \param  ERR_NO errNo         连接成功能与否的消息
		/// \param  const char* pErrStr  错误信息
		/// \return void  
	
	void OnQueryDisConnection(ERR_NO errNo, const char* pErrStr );

	/// 登录消息的回调
	
		/// \param  pLogon                  登录成功或是失败的结构
		/// \return void 
	
	void OnQueryUserLogon(EES_QueryLogonResponse* pLogon);

	/// 修改密码响应回调

	/// \param  nResult                  服务器响应的成功与否返回码
	/// \return void 

	void OnQueryRspChangePassword(EES_ChangePasswordResult nResult);

	/// 查询用户下面帐户的返回事件
	
		/// \param  pAccountInfo	        帐户的信息
		/// \param  bFinish	                如果没有传输完成，这个值是 false ，如果完成了，那个这个值为 true 
		/// \remark 如果碰到 bFinish == true，那么是传输结束，并且 pAccountInfo值无效。
		/// \return void 
	
	void OnQueryUserAccount(EES_AccountInfo * pAccoutnInfo, bool bFinish);

	/// 查询帐户下面期货仓位信息的返回事件	
		/// \param  pAccount	                帐户ID 	
		/// \param  pAccoutnPosition	        帐户的仓位信息					   
		/// \param  bFinish	                    如果没有传输完成，这个值是false，如果完成了，那个这个值为 true 
		/// \remark 如果碰到 bFinish == true，那么是传输结束，并且 pAccountInfo值无效。
		/// \return void 	
	void OnQueryAccountPosition(const char* pAccount, EES_AccountPosition* pAccoutnPosition, bool bFinish);

  
	/// 查询帐户下面资金信息的返回事件
	
		/// \param  pAccount	                帐户ID 	
		/// \param  pAccoutnBP	                帐户的资金信息					   
		/// \return void 
	
	void OnQueryAccountBP(const char* pAccount, EES_AccountBP* pAccoutnBP);


  /// 查询合约列表的返回事件

    /// \param  pSymbol                     合约信息   
    /// \param  bFinish                     如果没有传输完成，这个值是 false，如果完成了，那个这个值为 true   
    /// \remark 如果碰到 bFinish == true，那么是传输结束，并且 pSymbol 值无效。
    /// \return void 

  void OnQuerySymbol(EES_SymbolField* pSymbol, bool bFinish);

  /// 查询订单的返回事件

    /// \brief  查询订单信息时候的回调，这里面也可能包含不是当前用户下的订单
    /// \param  pAccount                 帐户ID 
    /// \param  pQueryOrder              查询订单的结构
    /// \param  bFinish                  如果没有传输完成，这个值是 false，如果完成了，那个这个值为 true    
    /// \remark 如果碰到 bFinish == true，那么是传输结束，并且 pQueryOrder值无效。
    /// \return void 

  void OnQueryTradeOrder(const char* pAccount, EES_QueryAccountOrder* pQueryOrder, bool bFinish  );

  /// 查询订单的返回事件

    /// \brief  查询订单信息时候的回调，这里面也可能包含不是当前用户下的订单成交
    /// \param  pAccount                        帐户ID 
    /// \param  pQueryOrderExec                 查询订单成交的结构
    /// \param  bFinish                         如果没有传输完成，这个值是false，如果完成了，那个这个值为 true    
    /// \remark 如果碰到 bFinish == true，那么是传输结束，并且pQueryOrderExec值无效。
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

