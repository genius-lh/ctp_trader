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
  // ��ʼ��
  int init();
  // ��ѭ��
  void loop();
  // �˳�
  void stop();
  // ��ȡ������Ϣ
  int getCfgString(const char* section, const char* entry, char* value, int size);
  // ��ȡ��Լ����
  int getContractNum();
  // ��ȡָ����Ҫ��Լ����
  const char* getContractId(int idx);
  // �жϺ�Լ�Ƿ����
  int findContractById(const char* contractId);
  // ������������
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

