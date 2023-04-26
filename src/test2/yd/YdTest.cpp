#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include "ydApi.h"
#include "ydError.h"

#include "YdTest.h"

#define YD_TEST_LOG(...) printf(__VA_ARGS__)

int main(int argc, char* argv[])
{
  char sConfigFilename[128];
  char sUserId[128];
  char sPasswd[128];
  int i = 1;
  if(argc < 4){
    printf("input sConfigFilename:\n");
    scanf("%s", sConfigFilename);
    printf("input sUserId:\n");
    scanf("%s", sUserId);
    printf("input sPasswd:\n");
    scanf("%s", sPasswd);
  }else{
    strncpy(sConfigFilename, argv[i++], sizeof(sConfigFilename));
    strncpy(sUserId, argv[i++], sizeof(sUserId));
    strncpy(sPasswd, argv[i++], sizeof(sPasswd));
  }
  
	YDExtendedApi* pTraderApi = makeYDExtendedApi(sConfigFilename);
  if (pTraderApi == NULL)	{
    YD_TEST_LOG("can not create API\n");
    exit(1);
  }

  CYdTestHandler* pTraderHandler = new CYdTestHandler();
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_Passwd = sPasswd;
  pTraderHandler->m_Loop = 1;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;

  if (!pTraderApi->start(pTraderHandler)){
    YD_TEST_LOG("can not start API\n");
		exit(1);
  }

  sleep(2);
  
  pTraderHandler->Loop();

  delete pTraderHandler;

  return 0;  
}


CYdTestHandler::CYdTestHandler()
{
  YD_TEST_LOG("%s\n", __FUNCTION__);

}

CYdTestHandler::~CYdTestHandler()
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
}

void CYdTestHandler::notifyBeforeApiDestroy(void)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
}

void CYdTestHandler::notifyAfterApiDestroy(void)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
  m_Loop = 0;
}


void CYdTestHandler::notifyEvent(int apiEvent)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);

}

void CYdTestHandler::notifyReadyForLogin(bool hasLoginFailed)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
  
  YDExtendedApi* pTraderApi = (YDExtendedApi*)m_Arg;
	int ret = pTraderApi->login(m_UserId, m_Passwd, NULL, NULL);
  
  if (!ret){
    YD_TEST_LOG("can not login\n");
  }

}

void CYdTestHandler::notifyLogin(int errorNo,int maxOrderRef,bool isMonitor)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
	if (errorNo==0){
    YD_TEST_LOG("login successfully\n");
    m_RequestId = maxOrderRef;
  }else{
    YD_TEST_LOG("login failed, errorNo=%d\n",errorNo);
  }

}

void CYdTestHandler::notifyFinishInit(void)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);

}


void CYdTestHandler::notifyCaughtUp(void)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);

}

void CYdTestHandler::notifyOrder(const YDOrder *pOrder,const YDInstrument *pInstrument,const YDAccount *pAccount)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
  PrintOrder((void*)pOrder);
}

void CYdTestHandler::notifyTrade(const YDTrade *pTrade,const YDInstrument *pInstrument,const YDAccount *pAccount)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
  PrintTrade((void*)pTrade);
}

void CYdTestHandler::notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder,const YDExchange *pExchange,const YDAccount *pAccount)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);

}

void CYdTestHandler::notifyChangePassword(int errorNo)
{
  YD_TEST_LOG("%s\n", __FUNCTION__);
	if (errorNo==0){
    YD_TEST_LOG("ChangePassword successfully\n");
  }else{
    YD_TEST_LOG("ChangePassword failed, errorNo=%d\n",errorNo);
  }

}

void CYdTestHandler::Loop()
{
  int choose;
  while(m_Loop){
    choose = ShowMenu();
    switch(choose){
    case 1:
      QryInstrument();
      break;
    case 2:
      Logout();
      m_Loop = 0;
      sleep(1);
      break;
    case 3:
      OrderInsert();
      break;
    case 4:
      OrderAction();
      break;
    case 5:
      QryOrder();
      break;
    case 6:
      QryTrade();
      break;
    case 7:
      QryUserInvestor();
      break;
    case 8:
      QryInvestorAccount();
      break;
    case 9:
      QryInvestorPosition();
      break;
    case 10:
      ChangePassword();
      break;
    default:
      break;
    }
  }
}

int CYdTestHandler::ShowMenu()
{
  int ret = 0;
  YD_TEST_LOG("**********************\n"
        "选择需要执行的操作\n"
        "1-QryInstrument\n"
        "2-Logout\n"
        "3-OrderInsert\n"
        "4-OrderAction\n"
        "5-QryOrder\n"
        "6-QryTrade\n"
        "7-QryUserInvestor\n"
        "8-QryInvestorAccount\n"
        "9-QryInvestorPosition\n"
        "10-ChangePassword\n"
        "**********************\n"
        "请选择："
  );
  scanf("%d",&ret);

  return ret;

}

void CYdTestHandler::Logout()
{
  YDExtendedApi* pTraderApi = (YDExtendedApi*)m_Arg;
  pTraderApi->startDestroy();
  sleep(2);

}

void CYdTestHandler::OrderInsert()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;
  char Temp[64];
  char InstrumentID[64];
  int Direction;
  int OffsetFlag;
  double LimitPrice;
  int Volume;

  
  printf("input InstrumentID:\n");
  scanf("%s", InstrumentID);
  printf("input Direction[0|1]:\n");
  scanf("%s", Temp);
  Direction = atoi(Temp);
  printf("input OffsetFlag[0|1|3|4]:\n");
  scanf("%s", Temp);
  OffsetFlag = atoi(Temp);
  printf("input LimitPrice:\n");
  scanf("%lf", &LimitPrice);
  printf("input Volume:\n");
  scanf("%d", &Volume);

  YDInputOrder inputOrder;
  memset(&inputOrder,0,sizeof(inputOrder));

  const YDInstrument* pInstrument = pApi->getInstrumentByID(InstrumentID);

  inputOrder.OffsetFlag = OffsetFlag;
  inputOrder.Price= LimitPrice;
	inputOrder.Direction=Direction;
  inputOrder.HedgeFlag=YD_HF_Speculation;
  inputOrder.OrderVolume=Volume;
  inputOrder.OrderType=YD_ODT_Limit;
  inputOrder.YDOrderFlag=YD_YOF_Normal;
  inputOrder.ConnectionSelectionType=YD_CS_Any;
  inputOrder.ConnectionID=0;
  inputOrder.OrderRef=++m_RequestId;
  
  pApi->insertOrder(&inputOrder, pInstrument);

}

void CYdTestHandler::OrderAction()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;

  char ExchangeID[64];
  int OrderSysID;
  char Temp[64];
  
  printf("input ExchangeID:\n");
  scanf("%s", ExchangeID);
  
  printf("input OrderSysID:\n");
  scanf("%s", Temp);
  OrderSysID = atol(Temp);

  YDCancelOrder cancelOrder;
  memset(&cancelOrder, 0, sizeof(cancelOrder));
  
  cancelOrder.YDOrderFlag = YD_YOF_Normal;
  cancelOrder.OrderGroupID = 0;
  cancelOrder.LongOrderSysID = 0;
  cancelOrder.OrderSysID = OrderSysID;

  const YDExchange* pExchange = pApi->getExchangeByID(ExchangeID);
  
  pApi->cancelOrder(&cancelOrder, pExchange);

}

void CYdTestHandler::QryOrder()
{

}
void CYdTestHandler::QryTrade()
{
}

void CYdTestHandler::QryUserInvestor()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;
  const YDAccount* account = pApi->getMyAccount();
  if(account){
    YD_TEST_LOG("account->AccountRef=[%d]\n"
      "account->AccountID=[%s]\n"
      "account->PreBalance=[%f]\n"
      "account->WarningLevel1=[%f]\n"
      "account->WarningLevel2=[%f]\n"
      "account->MaxMoneyUsage=[%f]\n"
      "account->Deposit=[%f]\n"
      "account->Withdraw=[%f]\n"
      "account->FrozenWithdraw=[%f]\n"
      "account->TradingRight=[%d]\n"
      "account->MaxOrderCount=[%d]\n"
      "account->MaxLoginCount=[%d]\n"
      "account->LoginCount=[%d]\n"
      "account->AccountFlag=[%d]\n"
      , account->AccountRef
      , account->AccountID
      , account->PreBalance
      , account->WarningLevel1
      , account->WarningLevel2
      , account->MaxMoneyUsage
      , account->Deposit
      , account->Withdraw
      , account->FrozenWithdraw
      , account->TradingRight
      , account->MaxOrderCount
      , account->MaxLoginCount
      , account->LoginCount
      , account->AccountFlag
    );
  }
}

void CYdTestHandler::QryInvestorAccount()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;
  const YDExtendedAccount *account = pApi->getExtendedAccount();

  if(account){
  YD_TEST_LOG("account->CloseProfit=[%f]\n"
    "account->CashIn=[%f]\n"
    "account->OtherCloseProfit=[%f]\n"
    "account->Commission=[%f]\n"
    "account->Margin=[%f]\n"
    "account->PositionProfit=[%f]\n"
    "account->Balance=[%f]\n"
    "account->UsedOrderCount=[%u]\n"
    "account->Useless=[%d]\n"
    "account->ExecMargin=[%f]\n"
    "account->OptionLongPositionCost=[%f]\n"
    "account->OptionLongPositionCostLimit=[%f]\n"
    "account->PrePositionMarketValue=[%f]\n"
    "account->PositionMarketValue=[%f]\n"
    "account->ExecAllocatedAmount=[%f]\n"
    , account->CloseProfit
    , account->CashIn
    , account->OtherCloseProfit
    , account->Commission
    , account->Margin
    , account->PositionProfit
    , account->Balance
    , account->UsedOrderCount
    , account->Useless
    , account->ExecMargin
    , account->OptionLongPositionCost
    , account->OptionLongPositionCostLimit
    , account->PrePositionMarketValue
    , account->PositionMarketValue
    , account->ExecAllocatedAmount
  );
  }

}

void CYdTestHandler::QryInstrument()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;
  for(int i = 0; i < pApi->getInstrumentCount(); i++) {
    const YDInstrument *instrument = pApi->getInstrument(i);
    if(instrument){
      YD_TEST_LOG("instrument->InstrumentID=[%s]\n"
        , instrument->InstrumentID
      );
    }
  }

}

void CYdTestHandler::QryExchange()
{
  YDExtendedApi* pApi = (YDExtendedApi*)m_Arg;
  for(int i = 0; i < pApi->getExchangeCount(); i++) {
    const YDExchange *exchange = pApi->getExchange(i);
    if(exchange){
      YD_TEST_LOG("exchange->ExchangeID=[%s]\n"
        "exchange->ExchangeRef=[%d]\n"
        , exchange->ExchangeID
        , exchange->ExchangeRef
      );
    }
  }

}

void CYdTestHandler::QryInvestorPosition()
{
  YDExtendedApi* pTraderApi = (YDExtendedApi*)m_Arg;
  for(int i = 0; i < pTraderApi->getPrePositionCount(); i++) {
    const YDPrePosition *prePosition = pTraderApi->getPrePosition(i);

    if(prePosition){
    YD_TEST_LOG("prePosition->AccountRef=[%d]\n"
      "prePosition->InstrumentRef=[%d]\n"
      "prePosition->PositionDirection=[%d]\n"
      "prePosition->HedgeFlag=[%d]\n"
      "prePosition->PrePosition=[%d]\n"
      "prePosition->PreSettlementPrice=[%f]\n"
      "prePosition->AverageOpenPrice=[%f]\n"
      "prePosition->InstrumentID=[%s]\n"
      , prePosition->AccountRef
      , prePosition->InstrumentRef
      , prePosition->PositionDirection
      , prePosition->HedgeFlag
      , prePosition->PrePosition
      , prePosition->PreSettlementPrice
      , prePosition->AverageOpenPrice
      , prePosition->m_pInstrument->InstrumentID
    );
    }

  }

}

void CYdTestHandler::PrintOrder(void* data)
{
  YDOrder* pOrder = (YDOrder*)data;
  if(!pOrder){
    return;
  }
  
  YD_TEST_LOG("pOrder->Direction=[%c]\n"
    "pOrder->OffsetFlag=[%c]\n"
    "pOrder->HedgeFlag=[%c]\n"
    "pOrder->ConnectionSelectionType=[%c]\n"
    "pOrder->Price=[%f]\n"
    "pOrder->OrderVolume=[%d]\n"
    "pOrder->OrderRef=[%d]\n"
    "pOrder->OrderType=[%c]\n"
    "pOrder->YDOrderFlag=[%c]\n"
    "pOrder->ConnectionID=[%c]\n"
    "pOrder->RealConnectionID=[%c]\n"
    "pOrder->ErrorNo=[%d]\n"
    "pOrder->ExchangeRef=[%d]\n"
    "pOrder->OrderSysID=[%d]\n"
    "pOrder->OrderStatus=[%d]\n"
    "pOrder->TradeVolume=[%d]\n"
    "pOrder->InsertTime=[%d]\n"
    "pOrder->OrderLocalID=[%d]\n"
    "pOrder->OrderGroupID=[%c]\n"
    "pOrder->GroupOrderRefControl=[%c]\n"
    "pOrder->OrderTriggerType=[%c]\n"
    "pOrder->TriggerPrice=[%f]\n"
    "pOrder->OrderTriggerStatus=[%d]\n"
    "pOrder->InsertTimeStamp=[%d]\n"
    "pOrder->LongOrderSysID=[%lld]\n"
    , pOrder->Direction
    , pOrder->OffsetFlag
    , pOrder->HedgeFlag
    , pOrder->ConnectionSelectionType
    , pOrder->Price
    , pOrder->OrderVolume
    , pOrder->OrderRef
    , pOrder->OrderType
    , pOrder->YDOrderFlag
    , pOrder->ConnectionID
    , pOrder->RealConnectionID
    , pOrder->ErrorNo
    , pOrder->ExchangeRef
    , pOrder->OrderSysID
    , pOrder->OrderStatus
    , pOrder->TradeVolume
    , pOrder->InsertTime
    , pOrder->OrderLocalID
    , pOrder->OrderGroupID
    , pOrder->GroupOrderRefControl
    , pOrder->OrderTriggerType
    , pOrder->TriggerPrice
    , pOrder->OrderTriggerStatus
    , pOrder->InsertTimeStamp
    , pOrder->LongOrderSysID
  );
}

void CYdTestHandler::PrintTrade(void* data)
{
  YDTrade* pTrade = (YDTrade*)data;
  if(!pTrade){
    return;
  }
  YD_TEST_LOG("pTrade->AccountRef=[%d]\n"
    "pTrade->InstrumentRef=[%d]\n"
    "pTrade->Direction=[%c]\n"
    "pTrade->OffsetFlag=[%c]\n"
    "pTrade->HedgeFlag=[%c]\n"
    "pTrade->TradeID=[%d]\n"
    "pTrade->OrderSysID=[%d]\n"
    "pTrade->Price=[%f]\n"
    "pTrade->Volume=[%d]\n"
    "pTrade->TradeTime=[%d]\n"
    "pTrade->Commission=[%f]\n"
    "pTrade->OrderLocalID=[%d]\n"
    "pTrade->OrderRef=[%d]\n"
    "pTrade->OrderGroupID=[%c]\n"
    "pTrade->RealConnectionID=[%c]\n"
    "pTrade->TradeTimeStamp=[%d]\n"
    "pTrade->LongOrderSysID=[%lld]\n"
    "pTrade->LongTradeID=[%lld]\n"
    , pTrade->AccountRef
    , pTrade->InstrumentRef
    , pTrade->Direction
    , pTrade->OffsetFlag
    , pTrade->HedgeFlag
    , pTrade->TradeID
    , pTrade->OrderSysID
    , pTrade->Price
    , pTrade->Volume
    , pTrade->TradeTime
    , pTrade->Commission
    , pTrade->OrderLocalID
    , pTrade->OrderRef
    , pTrade->OrderGroupID
    , pTrade->RealConnectionID
    , pTrade->TradeTimeStamp
    , pTrade->LongOrderSysID
    , pTrade->LongTradeID
  );

}

void CYdTestHandler::ChangePassword()
{
  YDExtendedApi* pTraderApi = (YDExtendedApi*)m_Arg;
  char Temp[64];
  printf("input Password:\n");
  scanf("%s", Temp);
  pTraderApi->changePassword(m_UserId, m_Passwd, Temp);
  
}


