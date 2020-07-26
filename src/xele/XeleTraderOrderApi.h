#if !defined(_XELE_FTDCTRADERORDERAPI_H)

#define _XELE_FTDCTRADERORDERAPI_H

#include "CXeleFtdcUserApiStruct.h"
#include "CXeleTraderApi.hpp"

class CXeleTraderOrderApi
{
public:
  ///创建TraderApi
  ///@return 创建出的UserApi
  ///exchange=1(上期所），0或其他（中金所）
  static CXeleTraderOrderApi *CreateTraderApi();

  static TXeleFtdcInsertType ConvertInsertType(char cTimeCondition, char cCombOffsetFlag,
    char cDirection, char cVolumeCondition);
  
  ///删除接口对象本身
  ///@remark 不再使用本接口对象时,调用该函数删除接口对象
  virtual void Release() = 0;

  ///初始化
  ///@remark 初始化运行环境,只有调用后,接口才开始工作
  virtual void Init(TXeleFtdcClientIndexType* pClientIndex, TXeleFtdcClientTokenType* pToken) = 0;

  ///等待接口线程结束运行
  ///@return 线程退出代码
  virtual int Join() = 0;

  ///注册前置机网络地址
  ///@param pszFrontAddress：前置机网络地址。
  ///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。
  ///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
  virtual void RegisterFront(char *pszUdpFrontAddress, char *pszTcpQueryFrontAddress) = 0;

  ///注册回调接口
  ///@param pSpi 派生自回调接口类的实例
  virtual void RegisterSpi(CXeleTraderSpi *pSpi) = 0;

  ///用户登录请求
  virtual int ReqUserLogin(int nRequestID) = 0;

  ///用户退出请求
  virtual int ReqUserLogout(CXeleFtdcReqUserLogoutField *pReqUserLogout,
                            int nRequestID) = 0;

  ///报单录入请求
  virtual int ReqOrderInsert(CXeleFairInputOrderField *pInputOrder,
                             int nRequestID) = 0;

  ///报单操作请求
  virtual int ReqOrderAction(CXeleFairOrderActionField *pOrderAction,
                             int nRequestID) = 0;

  virtual ~CXeleTraderOrderApi() {};
};

#endif

