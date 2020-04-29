
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std_strategy_struct.h"

#include "stg_mysql_sys.h"

struct strategy_instrument_is_null_def{
  mysqlbool instrumentid;
  mysqlbool exchangeid;
  mysqlbool instrumentname;
  mysqlbool exchangeinstid;
  mysqlbool productid;
  mysqlbool productclass;
  mysqlbool deliveryyear;
  mysqlbool deliverymonth;
  mysqlbool maxmarketordervolume;
  mysqlbool minmarketordervolume;
  mysqlbool maxlimitordervolume;
  mysqlbool minlimitordervolume;
  mysqlbool volumemultiple;
  mysqlbool pricetick;
  mysqlbool createdate;
  mysqlbool opendate;
  mysqlbool expiredate;
  mysqlbool startdelivdate;
  mysqlbool enddelivdate;
  mysqlbool instlifephase;
  mysqlbool istrading;
  mysqlbool positiontype;
  mysqlbool positiondatetype;
  mysqlbool longmarginratio;
  mysqlbool shortmarginratio;
  mysqlbool maxmarginsidealgorithm;
  mysqlbool underlyinginstrid;
  mysqlbool strikeprice;
  mysqlbool optionstype;
  mysqlbool underlyingmultiple;
  mysqlbool combinationtype;
};

struct strategy_instrument_length_def{
  mysqlulong instrumentid;
  mysqlulong exchangeid;
  mysqlulong instrumentname;
  mysqlulong exchangeinstid;
  mysqlulong productid;
  mysqlulong productclass;
  mysqlulong deliveryyear;
  mysqlulong deliverymonth;
  mysqlulong maxmarketordervolume;
  mysqlulong minmarketordervolume;
  mysqlulong maxlimitordervolume;
  mysqlulong minlimitordervolume;
  mysqlulong volumemultiple;
  mysqlulong pricetick;
  mysqlulong createdate;
  mysqlulong opendate;
  mysqlulong expiredate;
  mysqlulong startdelivdate;
  mysqlulong enddelivdate;
  mysqlulong instlifephase;
  mysqlulong istrading;
  mysqlulong positiontype;
  mysqlulong positiondatetype;
  mysqlulong longmarginratio;
  mysqlulong shortmarginratio;
  mysqlulong maxmarginsidealgorithm;
  mysqlulong underlyinginstrid;
  mysqlulong strikeprice;
  mysqlulong optionstype;
  mysqlulong underlyingmultiple;
  mysqlulong combinationtype;
};

static struct strategy_instrument_def strategy_instrument;
static struct strategy_instrument_is_null_def strategy_instrument_is_null;
static struct strategy_instrument_length_def strategy_instrument_length;
    
static void* strategy_instrument_cursor;
static int strategy_instrument_declare(strategy_instrument_t* db_struct, int* sql_code);
static int strategy_instrument_open(strategy_instrument_t* db_struct, int* sql_code);
static int strategy_instrument_fetch(strategy_instrument_t* db_struct, int* sql_code);
static int strategy_instrument_close(strategy_instrument_t* db_struct, int* sql_code);
static int strategy_instrument_fill_result(strategy_instrument_t* db_struct);
static mysqlbind* strategy_instrument_bind_result();


int strategy_instrument_fill_result(strategy_instrument_t* db_struct)
{
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, instrumentid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, exchangeid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, instrumentname);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, exchangeinstid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, productid);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, productclass);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, deliveryyear);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, deliverymonth);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, maxmarketordervolume);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, minmarketordervolume);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, maxlimitordervolume);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, minlimitordervolume);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, volumemultiple);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_instrument, db_struct, pricetick);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, createdate);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, opendate);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, expiredate);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, startdelivdate);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, enddelivdate);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, instlifephase);
  STG_MYSQL_FETCH_RESULT_LONG(strategy_instrument, db_struct, istrading);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, positiontype);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, positiondatetype);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_instrument, db_struct, longmarginratio);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_instrument, db_struct, shortmarginratio);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, maxmarginsidealgorithm);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, underlyinginstrid);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_instrument, db_struct, strikeprice);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, optionstype);
  STG_MYSQL_FETCH_RESULT_DOUBLE(strategy_instrument, db_struct, underlyingmultiple);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_instrument, db_struct, combinationtype);
  return 0;
}

mysqlbind* strategy_instrument_bind_result()
{
  static mysqlbind bind_param[31];
  int i =0;
  memset(bind_param, 0, sizeof(bind_param));
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentname);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeinstid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productclass);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliveryyear);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliverymonth);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, volumemultiple);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, pricetick);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, createdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, opendate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, expiredate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, startdelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, enddelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instlifephase);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, istrading);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiontype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiondatetype);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, longmarginratio);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, shortmarginratio);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, maxmarginsidealgorithm);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, underlyinginstrid);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, strikeprice);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, optionstype);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, underlyingmultiple);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, combinationtype);
  return bind_param;
}

int strategy_instrument_declare(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  const char sql[] = ""
    "SELECT "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `instrumentname`, "
    "  `exchangeinstid`, "
    "  `productid`, "
    "  `productclass`, "
    "  `deliveryyear`, "
    "  `deliverymonth`, "
    "  `maxmarketordervolume`, "
    "  `minmarketordervolume`, "
    "  `maxlimitordervolume`, "
    "  `minlimitordervolume`, "
    "  `volumemultiple`, "
    "  `pricetick`, "
    "  `createdate`, "
    "  `opendate`, "
    "  `expiredate`, "
    "  `startdelivdate`, "
    "  `enddelivdate`, "
    "  `instlifephase`, "
    "  `istrading`, "
    "  `positiontype`, "
    "  `positiondatetype`, "
    "  `longmarginratio`, "
    "  `shortmarginratio`, "
    "  `maxmarginsidealgorithm`, "
    "  `underlyinginstrid`, "
    "  `strikeprice`, "
    "  `optionstype`, "
    "  `underlyingmultiple`, "
    "  `combinationtype` "
    "FROM "
    "  `strategy_instrument` "
    "ORDER BY "
    "  `instrumentid`, "
    "  `exchangeid` "
    ";";

  ret = stg_mysql_stmt_perpare((void*)&strategy_instrument_cursor, sql, (void*)NULL, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_instrument_open(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_instrument_bind_result();

  ret = stg_mysql_stmt_exec(strategy_instrument_cursor, (void*)bind, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_instrument_fetch(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_fetch(strategy_instrument_cursor, sql_code);

  if(ret){
    return -1;
  }

  strategy_instrument_fill_result(db_struct);

  return ret;
}

int strategy_instrument_close(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_close(strategy_instrument_cursor, sql_code);

  return ret;
}

static void* strategy_instrument_select1_prepare(int* sql_code);
static int strategy_instrument_select1(strategy_instrument_t* db_struct, int* sql_code);


void* strategy_instrument_select1_prepare(int* sql_code)
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
    "  `instrumentname`, "
    "  `exchangeinstid`, "
    "  `productid`, "
    "  `productclass`, "
    "  `deliveryyear`, "
    "  `deliverymonth`, "
    "  `maxmarketordervolume`, "
    "  `minmarketordervolume`, "
    "  `maxlimitordervolume`, "
    "  `minlimitordervolume`, "
    "  `volumemultiple`, "
    "  `pricetick`, "
    "  `createdate`, "
    "  `opendate`, "
    "  `expiredate`, "
    "  `startdelivdate`, "
    "  `enddelivdate`, "
    "  `instlifephase`, "
    "  `istrading`, "
    "  `positiontype`, "
    "  `positiondatetype`, "
    "  `longmarginratio`, "
    "  `shortmarginratio`, "
    "  `maxmarginsidealgorithm`, "
    "  `underlyinginstrid`, "
    "  `strikeprice`, "
    "  `optionstype`, "
    "  `underlyingmultiple`, "
    "  `combinationtype` "
    "FROM "
    "  `strategy_instrument` "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[2];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeid);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_instrument_select1(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_instrument_select1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeid);
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_instrument_bind_result();

  ret = stg_mysql_stmt_exec(stmt, (void*)bind, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_stmt_fetch(stmt, sql_code);
  if(ret){
    return -3;
  }

  strategy_instrument_fill_result(db_struct);

  return ret;
}

static void* strategy_instrument_insert1_prepare(int* sql_code);
static int strategy_instrument_insert1(strategy_instrument_t* db_struct, int* sql_code);


void* strategy_instrument_insert1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "INSERT INTO "
    "  `strategy_instrument` "
    "( "
    "  `instrumentid`, "
    "  `exchangeid`, "
    "  `instrumentname`, "
    "  `exchangeinstid`, "
    "  `productid`, "
    "  `productclass`, "
    "  `deliveryyear`, "
    "  `deliverymonth`, "
    "  `maxmarketordervolume`, "
    "  `minmarketordervolume`, "
    "  `maxlimitordervolume`, "
    "  `minlimitordervolume`, "
    "  `volumemultiple`, "
    "  `pricetick`, "
    "  `createdate`, "
    "  `opendate`, "
    "  `expiredate`, "
    "  `startdelivdate`, "
    "  `enddelivdate`, "
    "  `instlifephase`, "
    "  `istrading`, "
    "  `positiontype`, "
    "  `positiondatetype`, "
    "  `longmarginratio`, "
    "  `shortmarginratio`, "
    "  `maxmarginsidealgorithm`, "
    "  `underlyinginstrid`, "
    "  `strikeprice`, "
    "  `optionstype`, "
    "  `underlyingmultiple`, "
    "  `combinationtype` "
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
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
    "  ?, "
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
  static mysqlbind bind_param[31];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentname);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeinstid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productclass);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliveryyear);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliverymonth);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, volumemultiple);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, pricetick);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, createdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, opendate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, expiredate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, startdelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, enddelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instlifephase);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, istrading);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiontype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiondatetype);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, longmarginratio);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, shortmarginratio);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, maxmarginsidealgorithm);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, underlyinginstrid);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, strikeprice);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, optionstype);
  STG_MYSQL_BIND_DOUBLE(bind_param, i, strategy_instrument, underlyingmultiple);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, combinationtype);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_instrument_insert1(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_instrument_insert1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentname);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeinstid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, productid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, productclass);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, deliveryyear);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, deliverymonth);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, maxmarketordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, minmarketordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, maxlimitordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, minlimitordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, volumemultiple);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_instrument, db_struct, pricetick);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, createdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, opendate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, expiredate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, startdelivdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, enddelivdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instlifephase);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, istrading);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, positiontype);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, positiondatetype);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_instrument, db_struct, longmarginratio);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_instrument, db_struct, shortmarginratio);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, maxmarginsidealgorithm);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, underlyinginstrid);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_instrument, db_struct, strikeprice);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, optionstype);
  STG_MYSQL_FILL_PARAM_DOUBLE(strategy_instrument, db_struct, underlyingmultiple);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, combinationtype);

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

static void* strategy_instrument_update1_prepare(int* sql_code);
static int strategy_instrument_update1(strategy_instrument_t* db_struct, int* sql_code);


void* strategy_instrument_update1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "UPDATE "
    "  `strategy_instrument` "
    "SET "
    "  `instrumentname` = ?, "
    "  `exchangeinstid` = ?, "
    "  `productid` = ?, "
    "  `productclass` = ?, "
    "  `deliveryyear` = ?, "
    "  `deliverymonth` = ?, "
    "  `maxmarketordervolume` = ?, "
    "  `minmarketordervolume` = ?, "
    "  `maxlimitordervolume` = ?, "
    "  `minlimitordervolume` = ?, "
    "  `volumemultiple` = ?, "
    "  `pricetick` = ?, "
    "  `createdate` = ?, "
    "  `opendate` = ?, "
    "  `expiredate` = ?, "
    "  `startdelivdate` = ?, "
    "  `enddelivdate` = ?, "
    "  `instlifephase` = ?, "
    "  `istrading` = ?, "
    "  `positiontype` = ?, "
    "  `positiondatetype` = ?, "
    "  `longmarginratio` = ?, "
    "  `shortmarginratio` = ?, "
    "  `maxmarginsidealgorithm` = ?, "
    "  `underlyinginstrid` = ?, "
    "  `strikeprice` = ?, "
    "  `optionstype` = ?, "
    "  `underlyingmultiple` = ?, "
    "  `combinationtype` = ? "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[31];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentname);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeinstid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, productclass);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliveryyear);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, deliverymonth);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minmarketordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, maxlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, minlimitordervolume);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, volumemultiple);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, createdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, opendate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, expiredate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, startdelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, enddelivdate);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instlifephase);
  STG_MYSQL_BIND_LONG(bind_param, i, strategy_instrument, istrading);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiontype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, positiondatetype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, maxmarginsidealgorithm);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, underlyinginstrid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, optionstype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, combinationtype);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeid);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_instrument_update1(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_instrument_update1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentname);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeinstid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, productid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, productclass);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, deliveryyear);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, deliverymonth);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, maxmarketordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, minmarketordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, maxlimitordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, minlimitordervolume);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, volumemultiple);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, createdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, opendate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, expiredate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, startdelivdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, enddelivdate);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instlifephase);
  STG_MYSQL_FILL_PARAM_LONG(strategy_instrument, db_struct, istrading);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, positiontype);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, positiondatetype);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, maxmarginsidealgorithm);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, underlyinginstrid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, optionstype);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, combinationtype);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeid);

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

static void* strategy_instrument_delete1_prepare(int* sql_code);
static int strategy_instrument_delete1(strategy_instrument_t* db_struct, int* sql_code);


void* strategy_instrument_delete1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "DELETE FROM "
    "  `strategy_instrument` "
    "WHERE "
    "  `instrumentid` = ? AND "
    "  `exchangeid` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[2];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, instrumentid);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_instrument, exchangeid);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_instrument_delete1(strategy_instrument_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_instrument_delete1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, instrumentid);
  STG_MYSQL_FILL_PARAM_STRING(strategy_instrument, db_struct, exchangeid);
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

