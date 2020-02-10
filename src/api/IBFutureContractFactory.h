#if !defined(IB_FUTURE_CONTRACT_FACTORY_H)
#define IB_FUTURE_CONTRACT_FACTORY_H

typedef struct IBFutureContractDef IBFutureContract;

struct IBFutureContractDef{
  long tickId;
	char symbol[32];
  char secType[4];
	char lastTradeDateOrContractMonth[8];
	char exchange[8];
	char currency[4];
	char localSymbol[32];
};

class IBFutureContractFactory
{
public:
  static IBFutureContractFactory* GetInstance();
  static void Release();
private:
  static IBFutureContractFactory* instance;
  
public:
  int Init(const char* configFile);
  IBFutureContract* Search(const char* contract);
  
private:
  IBFutureContractFactory();

private:
  int count;
  IBFutureContract* futureContract;
  
public:
  ~IBFutureContractFactory();

};

#endif

