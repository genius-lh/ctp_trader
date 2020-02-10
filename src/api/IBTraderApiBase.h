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
  
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterSpi(CIbTraderSpi *pSpi);
  
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	void Init();

	///等待接口线程结束运行
	///@return 线程退出代码
	int Join();
  
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	void Release();

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
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

