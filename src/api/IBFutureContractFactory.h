#if !defined(IB_FUTURE_CONTRACT_FACTORY_H)
#define IB_FUTURE_CONTRACT_FACTORY_H

typedef struct IBFutureContractDef IBFutureContract;

struct IBFutureContractDef{
  long tickId;
	char symbol[32];
  char secType[8];
	char exchange[16];
	char currency[4];
	char localSymbol[32];
  char instrument[64];
  double priceTick;
};

class IBFutureContractFactory
{
public:
  static IBFutureContractFactory* GetInstance();
  static void Release();
  
public:
  int Init(int count, void* instruments);
  IBFutureContract* Search(const char* contract);
  IBFutureContract* Search(long tick_id);
  
private:
  IBFutureContractFactory();

private:
  int count;
  IBFutureContract* futureContract;
  
public:
  ~IBFutureContractFactory();

};

#endif

