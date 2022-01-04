
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EESQuoteDefine.h"
#include "EESQuoteApi.h"

#include "RemMduserHandler.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "trader_data.h"
#include "trader_mduser_api.h"

#ifdef __cplusplus
}
#endif

CRemMduserHandler::CRemMduserHandler(EESQuoteApi * pApi, void * pArg)
: m_Arg(pArg)
, m_QuoteApi(pApi)
{

}

CRemMduserHandler::~CRemMduserHandler()
{

}

/// \brief 当服务器连接成功，登录前调用, 如果是组播模式不会发生, 只需判断InitMulticast返回值即可
void CRemMduserHandler::OnEqsConnected()
{
    trader_mduser_api* self = (trader_mduser_api*)m_Arg;
    trader_mduser_api_on_front_connected(self);
    return;
}

/// \brief 当服务器曾经连接成功，被断开时调用，组播模式不会发生该事件
void CRemMduserHandler::OnEqsDisconnected()
{
    trader_mduser_api* self = (trader_mduser_api*)m_Arg;
    trader_mduser_api_on_front_disconnected(self, -1, "DISCONNECTED");
    return;
}

/// \brief 当登录成功或者失败时调用，组播模式不会发生
/// \param bSuccess 登陆是否成功标志  
/// \param pReason  登陆失败原因  
void CRemMduserHandler::OnLoginResponse(bool bSuccess, const char* pReason)
{
    trader_mduser_api* self = (trader_mduser_api*)m_Arg;

    int errNo = 0;
    char* errMsg = NULL;
    if(1 != bSuccess) {
      errNo = bSuccess;
    }

    trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);
    return;
}

/// \brief 收到行情时调用,具体格式根据instrument_type不同而不同
/// \param chInstrumentType  EES行情类型
/// \param pDepthQuoteData   EES统一行情指针  
void CRemMduserHandler::OnQuoteUpdated(EesEqsIntrumentType chInstrumentType, EESMarketDepthQuoteData* pDepthQuoteData)
{
    trader_mduser_api* self = (trader_mduser_api*)m_Arg;
    trader_tick oTick;
    memset(&oTick, 0, sizeof(trader_tick));

    strcpy(oTick.InstrumentID, pDepthQuoteData->InstrumentID);
    strcpy(oTick.TradingDay, pDepthQuoteData->TradingDay);
    strcpy(oTick.UpdateTime, pDepthQuoteData->UpdateTime);
    oTick.UpdateMillisec = pDepthQuoteData->UpdateMillisec;
    oTick.BidPrice1 = pDepthQuoteData->BidPrice1;
    oTick.BidVolume1= pDepthQuoteData->BidVolume1;
    oTick.AskPrice1 = pDepthQuoteData->AskPrice1;
    oTick.AskVolume1= pDepthQuoteData->AskVolume1;
    oTick.UpperLimitPrice= pDepthQuoteData->UpperLimitPrice;
    oTick.LowerLimitPrice= pDepthQuoteData->LowerLimitPrice;
    oTick.LastPrice = pDepthQuoteData->LastPrice;

    trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

/// \brief 注册symbol响应消息来时调用，组播模式不支持行情注册
/// \param chInstrumentType  EES行情类型
/// \param pSymbol           合约名称
/// \param bSuccess          注册是否成功标志
void CRemMduserHandler::OnSymbolRegisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
  trader_mduser_api* self = (trader_mduser_api*)m_Arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(1 != bSuccess) {
    errNo = bSuccess;
  }
  
  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);
  return;
}

/// \brief  注销symbol响应消息来时调用，组播模式不支持行情注册
/// \param chInstrumentType  EES行情类型
/// \param pSymbol           合约名称
/// \param bSuccess          注册是否成功标志
void CRemMduserHandler::OnSymbolUnregisterResponse(EesEqsIntrumentType chInstrumentType, const char* pSymbol, bool bSuccess)
{
    trader_mduser_api* self = (trader_mduser_api*)m_Arg;

    int errNo = 0;
    char* errMsg = NULL;
    if(1 != bSuccess) {
      errNo = bSuccess;
    }

    trader_mduser_api_on_rsp_un_sub_market_data(self, errNo, errMsg);
    return;
}
