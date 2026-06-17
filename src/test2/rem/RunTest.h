#ifndef _RUN_TEST_H_
#define _RUN_TEST_H_

#include "RemTest2.h"

class CRunTest
{
public:
  CRunTest();
  ~CRunTest();

  // 主入口
  void Test();

  // 连通性
  void Test0_1();
  
  // 基础交易功能测试
  void Test1_1();
  void Test1_2();

  // 系统连接状态异常监测功能
  void Test2_1();
  void Test2_2();

  // 报撤单笔数监测功能
  void Test3_1();

  // 重复报单监测功能
  void Test4_1();
  
  // 阈值设置及预警功能
  void Test5_1();

  // 交易指令检查功能
  void Test6_1();

  // 错误提示功能
  void Test7_1();

  // 暂停交易功能
  void Test8_1();

  // 批量撤单功能
  void Test9_1();

private:
  int Connect();

  void Disconnect();

private:
  CRemTestHandler2* m_remHandler;

public:
  char remoteTradeIp[128];
  char remoteTradeTCPPort[128];
  char remoteTradeUDPPort[128];
  char remoteQueryIp[128];
  char remoteQueryTCPPort[128];
  char localTradeIp[128];
  char localTradeUDPPort[128];
  
  char sAppID[128];
  char sAuthCode[128];
  char sUserId[128];
  char sPasswd[128];
  
};

#endif // _RUN_TEST_H_
