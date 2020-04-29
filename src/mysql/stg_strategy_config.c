

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std_strategy_struct.h"

#include "stg_mysql_sys.h"

struct strategy_config_is_null_def{
  mysqlbool configkey;
  mysqlbool configvalue;
  mysqlbool configdesc;
};

struct strategy_config_length_def{
  mysqlulong configkey;
  mysqlulong configvalue;
  mysqlulong configdesc;
};

static struct strategy_config_def strategy_config;
static struct strategy_config_is_null_def strategy_config_is_null;
static struct strategy_config_length_def strategy_config_length;

static void* strategy_config_cursor;
static int strategy_config_declare(strategy_config_t* db_struct, int* sql_code);
static int strategy_config_open(strategy_config_t* db_struct, int* sql_code);
static int strategy_config_fetch(strategy_config_t* db_struct, int* sql_code);
static int strategy_config_close(strategy_config_t* db_struct, int* sql_code);
static int strategy_config_fill_result(strategy_config_t* db_struct);
static mysqlbind* strategy_config_bind_result();


int strategy_config_fill_result(strategy_config_t* db_struct)
{
  STG_MYSQL_FETCH_RESULT_STRING(strategy_config, db_struct, configkey);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_config, db_struct, configvalue);
  STG_MYSQL_FETCH_RESULT_STRING(strategy_config, db_struct, configdesc);
  return 0;
}

mysqlbind* strategy_config_bind_result()
{
  static mysqlbind bind_param[3];
  int i =0;
  memset(bind_param, 0, sizeof(bind_param));
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configkey);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configvalue);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configdesc);
  return bind_param;
}

int strategy_config_declare(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  const char sql[] = ""
    "SELECT "
    "  `configkey`, "
    "  `configvalue`, "
    "  `configdesc` "
    "FROM "
    "  `strategy_config` "
    "ORDER BY "
    "  `configkey` "
    ";";

  ret = stg_mysql_stmt_perpare((void*)&strategy_config_cursor, sql, (void*)NULL, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_config_open(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_config_bind_result();

  ret = stg_mysql_stmt_exec(strategy_config_cursor, (void*)bind, sql_code);

  if(ret){
    return -1;
  }

  return ret;
}

int strategy_config_fetch(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_fetch(strategy_config_cursor, sql_code);

  if(ret){
    return -1;
  }

  strategy_config_fill_result(db_struct);

  return ret;
}

int strategy_config_close(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;

  ret = stg_mysql_stmt_close(strategy_config_cursor, sql_code);

  return ret;
}
    
static void* strategy_config_select1_prepare(int* sql_code);
static int strategy_config_select1(strategy_config_t* db_struct, int* sql_code);


void* strategy_config_select1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ∂®“ÂSQL
  const char sql[] = ""
    "SELECT "
    "  `configkey`, "
    "  `configvalue`, "
    "  `configdesc` "
    "FROM "
    "  `strategy_config` "
    "WHERE "
    "  `configkey` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[1];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configkey);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_config_select1(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_config_select1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configkey);
  mysqlbind* bind = (mysqlbind*)NULL;
  bind = strategy_config_bind_result();

  ret = stg_mysql_stmt_exec(stmt, (void*)bind, sql_code);

  if(ret){
    return -2;
  }

  ret = stg_mysql_stmt_fetch(stmt, sql_code);
  if(ret){
    return -3;
  }

  strategy_config_fill_result(db_struct);

  return ret;
}

static void* strategy_config_insert1_prepare(int* sql_code);
static int strategy_config_insert1(strategy_config_t* db_struct, int* sql_code);


void* strategy_config_insert1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "INSERT INTO "
    "  `strategy_config` "
    "( "
    "  `configkey`, "
    "  `configvalue`, "
    "  `configdesc` "
    ") "
    "VALUES "
    "( "
    "  ?, "
    "  ?, "
    "  ? "
    ") "
    ";";

  // ???bind param
  static mysqlbind bind_param[3];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configkey);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configvalue);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configdesc);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_config_insert1(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_config_insert1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configkey);
  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configvalue);
  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configdesc);

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

static void* strategy_config_update1_prepare(int* sql_code);
static int strategy_config_update1(strategy_config_t* db_struct, int* sql_code);


void* strategy_config_update1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "UPDATE "
    "  `strategy_config` "
    "SET "
    "  `configvalue` = ?, "
    "  `configdesc` = ? "
    "WHERE "
    "  `configkey` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[3];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configvalue);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configdesc);
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configkey);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_config_update1(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_config_update1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configvalue);
  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configdesc);
  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configkey);

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

    
static void* strategy_config_delete1_prepare(int* sql_code);
static int strategy_config_delete1(strategy_config_t* db_struct, int* sql_code);


void* strategy_config_delete1_prepare(int* sql_code)
{
  static void* result = NULL;
  if(result){
    return result;
  }
  int ret = 0;
  // ??SQL
  const char sql[] = ""
    "DELETE FROM "
    "  `strategy_config` "
    "WHERE "
    "  `configkey` = ? "
    ";";

  // ???bind param
  static mysqlbind bind_param[1];
  memset(bind_param, 0, sizeof(bind_param));
  int i = 0;
  STG_MYSQL_BIND_STRING(bind_param, i, strategy_config, configkey);
  ret = stg_mysql_stmt_perpare((void*)&result, sql, (void*)bind_param, sql_code);

  if(ret){
    return (void*)NULL;
  }

  return result;
}

int strategy_config_delete1(strategy_config_t* db_struct, int* sql_code)
{
  int ret = 0;
  void* stmt = (void*)NULL;
  stmt = strategy_config_delete1_prepare(sql_code);
  if(!stmt){
    return -1;
  }

  STG_MYSQL_FILL_PARAM_STRING(strategy_config, db_struct, configkey);
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


