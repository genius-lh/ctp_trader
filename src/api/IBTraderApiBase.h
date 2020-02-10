#if !defined(IB_TRADER_API_BASE_H)
#define IB_TRADER_API_BASE_H

#include <pthread.h>

#include <string>

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "EClientSocket.h"

#include "IBFutureContractFactory.h"

class CIbTraderSpi: public EWrapper
{
public: 
	// events
	#ifdef EWRAPPER_VIRTUAL_IMPL
    #undef EWRAPPER_VIRTUAL_IMPL
  #endif
  #define EWRAPPER_VIRTUAL_IMPL {}
  #include "EWrapper_prototypes.h"
  #undef EWRAPPER_VIRTUAL_IMPL
};

class CIbTraderApiBase
{
public:
  CIbTraderApiBase();
  virtual ~CIbTraderApiBase() {};
  
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterSpi(CIbTraderSpi *pSpi);
  
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	void Init();

	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	int Join();
  
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	void Release();

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(const char *pszFrontAddress);

  void RegisterClientId(int clientId);

  void RegisterConfig(const char* config);

  IBFutureContract* GetFutureContract(const char* contract);

protected:
  EClientSocket* Api();

private:
  bool Connect(const char *host, int port, int clientId);
  void Disconnect();
  void StartThread();
  void ProcessMessage();

  static void* ProcessThread(void * lpParam);
  inline int PraseUrl(const char* url, char* host, int* port);

private:
  IBFutureContractFactory* m_contractFactory;
  
  std::string m_szFrontAddress;
  std::string m_configFilename;
  CIbTraderSpi* m_traderSpi;
  
  //! [socket_declare]
  EReaderOSSignal m_osSignal;
  EClientSocket* m_pClient;
  //! [socket_declare]

  EReader *m_pReader;

  pthread_t m_hReadThread;
  bool m_threadLoop;
  
  bool m_extraAuth;
  int m_clientId;
};

#endif

