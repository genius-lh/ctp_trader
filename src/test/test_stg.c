#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stg_mysql_sys.h"
#include "stg_strategy_struct.h"

extern int strategy_market_data_tick_insert1(strategy_market_data_tick_t* db_struct, int* sql_code);
extern int strategy_market_data_tick_delete1(strategy_market_data_tick_t* db_struct, int* sql_code);

static void test_market_data_insert();


void test_market_data_insert()
{
  int sqlCode = 0;
  int nRet;
  strategy_market_data_tick_t dbTick;
  memset(&dbTick, 0, sizeof(dbTick));
  char instrumentid[30+1];
  char exchangeid[8+1];
  char tradingday[8+1];
  char updatetime[8+1];
  long updatemillisec;
  double bidprice1;
  double askprice1;
  long bidvolume1;
  long askvolume1;

  strncpy(dbTick.instrumentid, "sc2011", sizeof(dbTick.instrumentid)); 
  strncpy(dbTick.exchangeid, "INE", sizeof(dbTick.instrumentid)); 
  strncpy(dbTick.tradingday, "20200505", sizeof(dbTick.instrumentid)); 
  strncpy(dbTick.updatetime, "09:00:00", sizeof(dbTick.instrumentid)); 
  dbTick.updatemillisec = 0;
  dbTick.bidprice1 = 0;
  dbTick.askprice1 = 0;
  dbTick.bidvolume1 = 0;
  dbTick.askvolume1 = 0;

  nRet = strategy_market_data_tick_delete1(&dbTick, &sqlCode);
  if(nRet < 0){
    printf("strategy_market_data_tick_delete1 failed!nRet=[%d]sqlCode=[%d]\n", nRet, sqlCode);
    stg_mysql_rollback(&sqlCode);
  }else{
    stg_mysql_commit(&sqlCode);
  }
  nRet = strategy_market_data_tick_insert1(&dbTick, &sqlCode);

  if(nRet < 0){
    printf("strategy_market_data_tick_insert1 failed!nRet=[%d]sqlCode=[%d]\n", nRet, sqlCode);
    stg_mysql_rollback(&sqlCode);
  }else{
    stg_mysql_commit(&sqlCode);
  }

  return ;
}


int main(int argc, char* argv[])
{
  // 获取连库参数
  char* dbName;
  char* dbUser;
  char* dbPwd;
  char* dbHost;
  int dbPort;
  int sqlCode = 0;
  int nRet;
  int i = 1;
  if(argc < 6){
    return -1;
  }

  dbName = strdup(argv[i++]);
  dbUser = strdup(argv[i++]);
  dbPwd = strdup(argv[i++]);
  dbHost = strdup(argv[i++]);
  dbPort = atoi(argv[i++]);
  
  // 连接数据库
  do{
    nRet = stg_mysql_connect(dbName, dbUser, dbPwd, dbHost, dbPort, &sqlCode);
    if(nRet < 0){
      printf("stg_mysql_connect failed!nRet=[%d]sqlCode=[%d]\n", nRet, sqlCode);
      break;
    }

    // test
    test_market_data_insert();
    
    // 断开数据库
    stg_mysql_disconnect(&sqlCode);
    
  }while(0);

  free(dbName);
  free(dbUser);
  free(dbPwd);
  free(dbHost);
  return 0;
}


