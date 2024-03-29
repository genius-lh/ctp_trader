
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "USTPFtdcUserApiDataType.h"
#include "USTPFtdcUserApiStruct.h"
#include "USTPFtdcMduserApi.h"

#include "FemasAFMduserHandler.h"

extern char *INI_FILE_NAME;

CFemasAFMduserHandler::CFemasAFMduserHandler(femas_af_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{

}

CFemasAFMduserHandler::~CFemasAFMduserHandler() 
{

}

// 当客户端与行情发布服务器建立起通信连接，客户端需要进行登录
void CFemasAFMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// 当客户端与行情发布服务器通信连接断开时，该方法被调用
void CFemasAFMduserHandler::OnFrontDisconnected(int nReason) 
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
void CFemasAFMduserHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///用户退出应答
void CFemasAFMduserHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

// 深度行情通知，行情服务器会主动通知客户端
void CFemasAFMduserHandler::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketData);
}

// 针对用户请求的出错通知
void CFemasAFMduserHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

///订阅合约的相关信息
void CFemasAFMduserHandler::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

///订阅合约的相关信息
void CFemasAFMduserHandler::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

