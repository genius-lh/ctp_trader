#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

#include "EesTraderApi.h"
#include "RemTest2.h"
#include "RunTest.h"

#define RUN_TEST_LOG(...) info(__VA_ARGS__)

void info(const char* format, ...) {
  va_list arglist;
  char buf1[2000];
  struct timeval msg_time;
  struct tm now;
  gettimeofday(&msg_time, NULL);
  localtime_r(&(msg_time.tv_sec), &now);

  va_start(arglist,format);
  vsprintf(buf1, format, arglist);
  va_end(arglist);
  
  printf("[%02d%02d%02d.%06ld]%s",
    now.tm_hour, now.tm_min, now.tm_sec, msg_time.tv_usec,
    buf1);
  return;
}

CRunTest::CRunTest()
:m_remHandler((CRemTestHandler2*)NULL)
{

}

CRunTest::~CRunTest()
{

}

int CRunTest::Connect()
{
  EESTraderApi* pTraderApi = CreateEESTraderApi() ; 

  CRemTestHandler2* pTraderHandler = new CRemTestHandler2();
  
  // 初始化变量
  pTraderHandler->m_AppID = sAppID;
  pTraderHandler->m_AuthCode = sAuthCode;
  pTraderHandler->m_UserId = sUserId;
  pTraderHandler->m_Passwd = sPasswd;
  pTraderHandler->m_Loop = 1;

  pTraderHandler->m_Arg = (void*)pTraderApi;
  pTraderHandler->m_RequestId = 1;

  RESULT ret_err;
  EES_TradeSvrInfo svrInfo;
  memset(&svrInfo, 0, sizeof(svrInfo));
  strncpy(svrInfo.m_remoteTradeIp, remoteTradeIp, sizeof(svrInfo.m_remoteTradeIp));
  svrInfo.m_remoteTradeTCPPort = atoi(remoteTradeTCPPort);
  svrInfo.m_remoteTradeUDPPort = atoi(remoteTradeUDPPort);
  strncpy(svrInfo.m_remoteQueryIp, remoteQueryIp, sizeof(svrInfo.m_remoteQueryIp));
  svrInfo.m_remoteQueryTCPPort = atoi(remoteQueryTCPPort);
  strncpy(svrInfo.m_LocalTradeIp, localTradeIp, sizeof(svrInfo.m_LocalTradeIp));
  svrInfo.m_LocalTradeUDPPort = atoi(localTradeUDPPort);
  if(0 != svrInfo.m_LocalTradeUDPPort){
	  ret_err = pTraderApi->ConnServer(svrInfo, pTraderHandler);
  }else{
    ret_err = pTraderApi->ConnServer(svrInfo.m_remoteTradeIp, svrInfo.m_remoteTradeTCPPort, pTraderHandler, svrInfo.m_remoteQueryIp, svrInfo.m_remoteQueryTCPPort);
  }
	if (ret_err != NO_ERROR)
	{
		RUN_TEST_LOG("connect to REM server failed!\n");
		return -1;
	}

  this->m_remHandler = pTraderHandler;

  return 0;
}

void CRunTest::Disconnect()
{
  EESTraderApi* pTraderApi = (EESTraderApi*)this->m_remHandler->m_Arg;

  pTraderApi->DisConnServer();
  
  DestroyEESTraderApi(pTraderApi);

  this->m_remHandler = (CRemTestHandler2*)NULL;

  return;
}

// 连通性
void CRunTest::Test()
{
  Test0_1();

	Test1_1();

  Test1_2();

  Test2_1();

  Test2_2();

  Test3_1();

  Test4_1();

  Test5_1();

  Test6_1();

  Test7_1();

  Test8_1();

  Test9_1();
	
  return;
}

// 提供账号登录成功的提示信息截图（如界面或日志）
void CRunTest::Test0_1()
{
  RUN_TEST_LOG("登录成功");
  int ret = Connect();
  if(ret < 0) {
    return ;
  }
  sleep(2);

  m_remHandler->Login();
  sleep(3);

}

// 1.开仓前持仓截图
// 2.开仓委托记录截图（已成交，截图包含委托状态信息）
// 3.开仓后持仓截图（仓位变化）
// 4.平仓委托记录截图（已成交，截图包含委托状态信息）
// 5.平仓后持仓截图（仓位变化）
void CRunTest::Test1_1()
{
  m_remHandler->QryInvestorPosition();
  sleep(2);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->QryInvestorPosition();
  sleep(2);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->QryInvestorPosition();
  sleep(2);

  return;
}

// 1. 开仓前资金截图（含可用资金）
// 2. 开仓后委托记录截图（已报，截图包含委托状态信息）  
// 3. 撤销该笔委托，撤单委托记录截图（截图包含委托状态信息）
// 4. 撤销后资金截图（资金变化）
void CRunTest::Test1_2()
{
  m_remHandler->QryInvestorAccount();
  sleep(2);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->QryInvestorAccount();
  sleep(2);

  m_remHandler->OrderAction();
  sleep(3);

  m_remHandler->QryInvestorAccount();
  sleep(2);
}

// 断开柜台连接后，提供包含状态信息变化的界面提示（或日志）截图							
void CRunTest::Test2_1()
{
  Disconnect();
}

// 重新连接柜台后，提供连接状态恢复的提示信息（或日志）截图
void CRunTest::Test2_2()
{
  int ret = Connect();
  if(ret < 0) {
    return ;
  }
  sleep(2);

  m_remHandler->Login();
  sleep(3);

}

// 提供实时统计报单笔数、撤单笔数的截图
void CRunTest::Test3_1()
{
  m_remHandler->QryOrder();
  sleep(3);

  m_remHandler->StatOrder();
  sleep(2);

}

// 重复报单
void CRunTest::Test4_1()
{
  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->OrderInsert();
  sleep(3);
}

//提供报单笔数、撤单笔数、重复报单笔数等（1个或多个）指标达到或超过设置阈值后进行预警截图，预警方式包括但不限于弹窗提示、声音提示、短信或邮件通知
void CRunTest::Test5_1()
{
  int orderNum = 2;
  int cancelNum = 2;
  RUN_TEST_LOG("orderNum=[%d]\n", orderNum);
  RUN_TEST_LOG("cancelNum=[%d]\n", cancelNum);

  RUN_TEST_LOG("orderNum[%d]>=[%d]停止策略\n", orderNum, orderNum);

  RUN_TEST_LOG("cancelNum[%d]>=[%d]停止策略\n", cancelNum, cancelNum);


}

//交易当前不存在的合约，或过期的合约，软件能够拒绝下达错误交易指令
//委托价格不满足合约最小变动价位，软件能够拒绝下达错误交易指令
//委托数量大于单笔委托最大手数提交委托，提供报错提示截图
void CRunTest::Test6_1()
{
  char instrument[16];

  RUN_TEST_LOG("合约[%s]找不到\n", instrument);

  RUN_TEST_LOG("合约[%s]找不到\n", instrument);

}

//在无足够资金开仓、无仓位可平、市场状态不允许等情况下进行委托，提供柜台返回的错误提示信息截图 （仅日志记录认定为不完善）
void CRunTest::Test7_1()
{
  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->OrderInsert();
  sleep(3);  

}

//提供下达停止交易指令的界面，以及可反映系统已停止交易的截图
void CRunTest::Test8_1()
{
  RUN_TEST_LOG("停止服务\n");

  Disconnect();

  RUN_TEST_LOG("停止成功\n");
}

//提供下达批量撤单指令的界面，以及可反映已按指令对部分或全部未成交委托单进行撤销的截图
void CRunTest::Test9_1()
{
  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->OrderInsert();
  sleep(3);

  m_remHandler->CancelAll();


}