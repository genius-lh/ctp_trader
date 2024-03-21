#ifndef _TRADER_MDUSER_PROXY_UTIL_H_
#define _TRADER_MDUSER_PROXY_UTIL_H_

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "hiredis.h"
#include "trader_tick_dict.h"
#include "mqueue_client.h"

typedef char contract_id_t[32];


class TraderMduserProxyUtil 
{
public:
  TraderMduserProxyUtil(const char* cfgFile);
  ~TraderMduserProxyUtil();
  // 初始化
  int init();
  // 主循环
  void loop();
  // 退出
  void stop();
  // 获取配置信息
  int getCfgString(const char* section, const char* entry, char* value, int size);
  // 获取合约个数
  int getContractNum();
  // 获取指定需要合约名称
  const char* getContractId(int idx);
  // 判断合约是否存在
  int findContractById(const char* contractId);
  // 发送行情数据
  int sendData(void* data, int size);

private:
  char m_cfgFile[256];
  int m_contractNum;
  contract_id_t* m_contracts;
  trader_tick_dict* m_contractDict;
	mqueue_client_context m_mqClientCtx;
  
  struct event_base* m_pBase;
  struct event* m_pSigIntEvt;
  struct event* m_pSigTermEvt;
  
};

#endif //_TRADER_MDUSER_PROXY_UTIL_H_

