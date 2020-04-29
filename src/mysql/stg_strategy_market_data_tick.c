
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std_strategy_struct.h"

#include "stg_mysql_sys.h"


struct strategy_market_data_tick_is_null_def{
  mysqlbool instrumentid;
  mysqlbool exchangeid;
  mysqlbool tradingday;
  mysqlbool updatetime;
  mysqlbool updatemillisec;
  mysqlbool bidprice1;
  mysqlbool askprice1;
  mysqlbool bidvolume1;
  mysqlbool askvolume1;
};

struct strategy_market_data_tick_length_def{
  mysqlulong instrumentid;
  mysqlulong exchangeid;
  mysqlulong tradingday;
  mysqlulong updatetime;
  mysqlulong updatemillisec;
  mysqlulong bidprice1;
  mysqlulong askprice1;
  mysqlulong bidvolume1;
  mysqlulong askvolume1;
};

static struct strategy_market_data_tick_def strategy_market_data_tick;
static struct strategy_market_data_tick_is_null_def strategy_market_data_tick_is_null;
static struct strategy_market_data_tick_length_def strategy_market_data_tick_length;


static void* strategy_market_data_tick_cursor;
static int strategy_market_data_tick_declare(strategy_market_data_tick_t* db_struct, int* sql_code);
static int strategy_market_data_tick_open(strategy_market_data_tick_t* db_struct, int* sql_code);
static int strategy_market_data_tick_fetch(strategy_market_data_tick_t* db_struct, int* sql_code);
static int strategy_market_data_tick_close(strategy_market_data_tick_t* db_struct, int* sql_code);
static int strategy_market_data_tick_fill_result(strategy_market_data_tick_t* db_struct);
static mysqlbind* strategy_market_data_tick_bind_result();


int strategy_market_data_tick_fill_result(strategy_market_data_tick_t* db_struct)
{
  STG_MYSQL_FETCH_RESULT_STRING(strategy_market_data_tick, db_struct, instrumentid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_market_data_tick, db_struct, exchangeid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_market_data_tick, db_struct, tradingday);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_market_data_tick, db_struct, updatetime);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_market_data_tick, db_struct, updatemillisec);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_market_data_tick, db_struct, bidprice1);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_market_data_tick, db_struct, askprice1);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_market_data_tick, db_struct, bidvolume1);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_market_data_tick, db_struct, askvolume1);
  return 0;
}

mysqlbind* strategy_market_data_tick_bind_result()
{
  static mysqlbind bind_param[9];
  int i =0;
  memset(bind_param, 0, sizeof(bind_param));
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, tradingday);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, updatetime);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, updatemillisec);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_market_data_tick, bidprice1);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_market_data_tick, askprice1);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, bidvolume1);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, askvolume1);
  return bind_param;
}

int strategy_market_data_tick_declare(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  const char sql[] = ""
    "SELECT "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `tradingday`, "
    "  `updatetime`, "
    "  `updatemillisec`, "
    "  `bidprice1`, "
    "  `askprice1`, "
    "  `bidvolume1`, "
    "  `askvolume1` "
    "FROM "
    "  `strategy_market_data_tick` "
    "ORDER BY "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `tradingday`, "
    "  `updatetime`, "
    "  `updatemillisec` "
    ";";

  ret = stg_mysql_stmt_perpare((void*)&strategy_market_data_tick_cursor, sql, (void*)NULL, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_market_data_tick_open(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_market_data_tick_bind_result();

  ret = stg_mysql_stmt_exec(strategy_market_data_tick_cursor, (void*)bind, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_market_data_tick_fetch(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_fetch(strategy_market_data_tick_cursor, sql_code);

  if(ret){
    return -1;
  }

  strategy_market_data_tick_fill_result(db_struct);

  return ret;
}

int strategy_market_data_tick_close(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_close(strategy_market_data_tick_cursor, sql_code);

  return ret;
}

static void* strategy_market_data_tick_select1_prepare(int* sql_code);
static int strategy_market_data_tick_select1(strategy_market_data_tick_t* db_struct, int* sql_code);


void* strategy_market_data_tick_select1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "SELECT "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `tradingday`, "
    "  `updatetime`, "
    "  `updatemillisec`, "
    "  `bidprice1`, "
    "  `askprice1`, "
    "  `bidvolume1`, "
    "  `askvolume1` "
    "FROM "
    "  `strategy_market_data_tick` "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? AND "
    "  `tradingday` = ? AND "
    "  `updatetime` = ? AND "
    "  `updatemillisec` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[5];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, tradingday);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, updatetime);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, updatemillisec);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_market_data_tick_select1(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_market_data_tick_select1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, exchangeid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, tradingday);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, updatetime);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, updatemillisec);
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_market_data_tick_bind_result();

  ret = stg_mysql_stmt_exec(stmt, (void*)bind, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_stmt_fetch(stmt, sql_code);
  if(ret){
    return -3;
  }

  strategy_market_data_tick_fill_result(db_struct);

  return ret;
}

static void* strategy_market_data_tick_insert1_prepare(int* sql_code);
static int strategy_market_data_tick_insert1(strategy_market_data_tick_t* db_struct, int* sql_code);


void* strategy_market_data_tick_insert1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "INSERT INTO "
    "  `strategy_market_data_tick` "
    "( "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `tradingday`, "
    "  `updatetime`, "
    "  `updatemillisec`, "
    "  `bidprice1`, "
    "  `askprice1`, "
    "  `bidvolume1`, "
    "  `askvolume1` "
    ") "
    "VALUES "
    "( "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ? "
    ") "
    ";";

  // ???bind param
  static mysqlbind bind_param[9];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, tradingday);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, updatetime);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, updatemillisec);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_market_data_tick, bidprice1);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_market_data_tick, askprice1);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, bidvolume1);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, askvolume1);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_market_data_tick_insert1(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_market_data_tick_insert1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, exchangeid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, tradingday);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, updatetime);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, updatemillisec);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_market_data_tick, db_struct, bidprice1);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_market_data_tick, db_struct, askprice1);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, bidvolume1);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, askvolume1);

  ret = stg_mysql_stmt_exec(stmt, (void*)NULL, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_commit(sql_code);

  if(ret){
    return -3;
  }

  ret = stg_mysql_stmt_affected_rows(stmt);
  if(1 != ret){
    return -4;
  }

  return 0;
}

static void* strategy_market_data_tick_update1_prepare(int* sql_code);
static int strategy_market_data_tick_update1(strategy_market_data_tick_t* db_struct, int* sql_code);


void* strategy_market_data_tick_update1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "UPDATE "
    "  `strategy_market_data_tick` "
    "SET "
    "  `bidprice1` = ?, "
    "  `askprice1` = ?, "
    "  `bidvolume1` = ?, "
    "  `askvolume1` = ? "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? AND "
    "  `tradingday` = ? AND "
    "  `updatetime` = ? AND "
    "  `updatemillisec` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[9];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, bidvolume1);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, askvolume1);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, tradingday);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, updatetime);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, updatemillisec);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_market_data_tick_update1(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_market_data_tick_update1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, bidvolume1);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, askvolume1);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, exchangeid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, tradingday);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, updatetime);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, updatemillisec);

  ret = stg_mysql_stmt_exec(stmt, (void*)NULL, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_commit(sql_code);

  if(ret){
    return -3;
  }

  ret = stg_mysql_stmt_affected_rows(stmt);
  if(1 != ret){
    return -4;
  }

  return 0;
}

static void* strategy_market_data_tick_delete1_prepare(int* sql_code);
static int strategy_market_data_tick_delete1(strategy_market_data_tick_t* db_struct, int* sql_code);


void* strategy_market_data_tick_delete1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "DELETE FROM "
    "  `strategy_market_data_tick` "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? AND "
    "  `tradingday` = ? AND "
    "  `updatetime` = ? AND "
    "  `updatemillisec` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[5];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, tradingday);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_market_data_tick, updatetime);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_market_data_tick, updatemillisec);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_market_data_tick_delete1(strategy_market_data_tick_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_market_data_tick_delete1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, exchangeid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, tradingday);
  STG_MYSQL_FILL_PARAM_STRING(strategy_market_data_tick, db_struct, updatetime);
  STG_MYSQL_FILL_PARAM_LONG(strategy_market_data_tick, db_struct, updatemillisec);
  ret = stg_mysql_stmt_exec(stmt, (void*)NULL, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_commit(sql_code);

  if(ret){
    return -3;
  }

  ret = stg_mysql_stmt_affected_rows(stmt);
  if(1 != ret){
    return -4;
  }

  return 0;
}


