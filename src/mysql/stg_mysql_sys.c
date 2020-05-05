#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#include "stg_mysql_sys.h"

static MYSQL* stg_mysql_handle_get();
MYSQL* stg_mysql_handle_get()
{
  static MYSQL stMysqlHandle;

  return &stMysqlHandle;
}


int stg_mysql_connect(char* db_name, char* db_usr, char* db_pwd, char* db_host, unsigned int db_port, int* sql_code)
{
  MYSQL* self = stg_mysql_handle_get();
  int errFlag = 1;
  
  do{
    // 初始化MYSQL变量
    if (NULL == mysql_init(self)) {
    // insufficient memory
      break;
    }

    // try to connect
    char value = 1;
    mysql_options(self, MYSQL_OPT_RECONNECT, (char *)&value);

    unsigned int timeout = 10;
    int ret = mysql_options(self, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);
    if(ret)
    {
      break;
    }


    if (NULL == mysql_real_connect(self, db_host, db_usr, db_pwd, db_name, db_port, (char*)NULL, 0))
    {
      break;
    }

    if(mysql_query(self, "set names 'gbk'")) {
      break;
    }

    if (0 != mysql_autocommit(self, 0))
    {
      break;
    }

    errFlag = 0;
  } while (0);

  if (1 == errFlag)
  {
    *sql_code = mysql_errno(self);

    return -1;
  }

  return 0;
}

int stg_mysql_disconnect(int* sql_code)
{
  MYSQL* self = stg_mysql_handle_get();
  *sql_code = 0;

  // mysql_close will throw no errors 
  mysql_close(self);

  return 0;
}

int stg_mysql_commit(int* sql_code)
{
  MYSQL* self = stg_mysql_handle_get();
  *sql_code = 0;

  if (0 != mysql_commit(self))
  {
    *sql_code = mysql_errno(self);
    return -1;
  }
  
  return 0;
}

int stg_mysql_rollback(int* sql_code)
{
  MYSQL* self = stg_mysql_handle_get();
  *sql_code = 0;

  if (0 != mysql_rollback(self))
  {
    *sql_code = mysql_errno(self);
    return -1;
  }

  return 0;

}

int stg_mysql_stmt_perpare(void* param, const char* sql, void* bind, int* sql_code)
{
  MYSQL* self = stg_mysql_handle_get();
  MYSQL_STMT* stmt;
  
  int err_flag = 1;
  do {
  
    stmt = mysql_stmt_init(self);
    if(!stmt){
      break;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql))){
      break;
    }
    
    if(NULL == bind){
      *(MYSQL_STMT**)param = stmt;
      err_flag = 0;
      break;
    }
    
    if (mysql_stmt_bind_param(stmt, (MYSQL_BIND*)bind)){
      break;
    }
    
    *(MYSQL_STMT**)param = stmt;
    err_flag = 0;
  }while(0);
  
  if (1 == err_flag)
  {
    *sql_code = mysql_errno(self);

    return -1;
  }

  return 0;

}

int stg_mysql_stmt_exec(void* param, void* bind, int* sql_code)
{
  MYSQL_STMT* stmt = (MYSQL_STMT*)param;
  
  int err_flag = 1;
  int ret = 0;
  do {
    ret = mysql_stmt_reset(stmt);
    if (ret){
      break;
    }

    ret = mysql_stmt_execute(stmt);
    if (ret){
      break;
    }
    
    if(NULL == bind){
      err_flag = 0;
      break;
    }

    /* Bind the result buffers */
    ret = mysql_stmt_bind_result(stmt, (MYSQL_BIND*)bind);
    if (ret)
    {
      break;
    }

    /* Now buffer all results to client (optional step) */
    ret = mysql_stmt_store_result(stmt);
    if (ret)
    {
      break;
    }

    err_flag = 0;
  }while(0);

  if (1 == err_flag)
  {
    *sql_code = mysql_stmt_errno(stmt);

    return -1;
  }

  return 0;
}

int stg_mysql_stmt_fetch(void* param, int* sql_code)
{
  MYSQL_STMT* stmt = (MYSQL_STMT*)param;
  *sql_code = 0;
  int ret = mysql_stmt_fetch(stmt);

  if(MYSQL_NO_DATA == ret){
    *sql_code = 100;
    return -1;
  }
  
  if (0 != ret)
  {
    *sql_code = mysql_stmt_errno(stmt);
    return -1;
  }

  return 0;
}

int stg_mysql_stmt_rows(void* param)
{
  MYSQL_STMT* stmt = (MYSQL_STMT*)param;

  return (int)mysql_stmt_num_rows(stmt);
}

int stg_mysql_stmt_affected_rows(void* param)
{
  MYSQL_STMT* stmt = (MYSQL_STMT*)param;

  return (int)mysql_stmt_affected_rows(stmt);
}

int stg_mysql_stmt_close(void* param, int* sql_code)
{
  MYSQL_STMT* stmt = (MYSQL_STMT*)param;
  *sql_code = 0;

  int ret = mysql_stmt_close(stmt);
  
  if (0 != ret)
  {
    *sql_code = mysql_stmt_errno(stmt);
    return -1;
  }

  return 0;
}


int stg_mysql_get_count(const char* tbl_name, int* p_count, int* sql_code)
{
  int ret = 0;
  char sql[1024];

  snprintf(sql, sizeof(sql), "SELECT count(*) FROM %s;", tbl_name);
  
  ret = stg_mysql_get_value(sql, p_count, sql_code);
  
  return ret;
}

int stg_mysql_get_value(char* sql_get_value, int* p_count, int* sql_code)
{
  int ret = 0;
  
  MYSQL *conn = stg_mysql_handle_get();
  MYSQL_RES *result = NULL;
  MYSQL_ROW row;
  char sql[1024];
  int errFlag = 1;

  do {
    strncpy(sql, sql_get_value, sizeof(sql));
  
    ret = mysql_query(conn, sql);
    if(ret){
      break;
    }

    result = mysql_store_result(conn);
    if(!result) {
      break;
    }
    
    row = mysql_fetch_row(result);
    if(!row) {
      break;
    }

    *p_count = atoi(row[0]);
    errFlag = 0;
  }while(0);
  
  if(errFlag){
    *sql_code = mysql_errno(conn);
    ret = -1;
  }
  
  if(result){
    mysql_free_result(result);
  }
  
  return ret;
}



int stg_mysql_add_ts(char* ts, int seconds, int* sql_code)
{
  int ret = 0;
  
  MYSQL *conn = stg_mysql_handle_get();
  MYSQL_RES *result = NULL;
  MYSQL_ROW row;
  char sql[1024];
  int errFlag = 1;

  do {
    snprintf(sql, sizeof(sql), "SELECT timestampadd(second, %d, '%s');", seconds, ts);
  
    ret = mysql_query(conn, sql);
    if(ret){
      break;
    }

    result = mysql_store_result(conn);
    if(!result) {
      break;
    }
    
    row = mysql_fetch_row(result);
    if(!row) {
      break;
    }

    strcpy(ts, row[0]);
    errFlag = 0;
  }while(0);
  
  if(errFlag){
    *sql_code = mysql_errno(conn);
    ret = -1;
  }
  
  if(result){
    mysql_free_result(result);
  }
  
  return ret;
}

int stg_mysql_sub_ts(char* ts, int seconds, int* sql_code)
{
  int ret = 0;
  ret = stg_mysql_add_ts(ts, -seconds, sql_code);
  return ret;
}

int stg_mysql_sub_current_ts(char* ts, int seconds, int* sql_code)
{
  int ret = 0;
  
  MYSQL *conn = stg_mysql_handle_get();
  MYSQL_RES *result = NULL;
  MYSQL_ROW row;
  char sql[1024];
  int errFlag = 1;

  do {
    snprintf(sql, sizeof(sql), "SELECT timestampadd(second, %d, current_timestamp(6));", -seconds);
  
    ret = mysql_query(conn, sql);
    if(ret){
      break;
    }

    result = mysql_store_result(conn);
    if(!result) {
      break;
    }
    
    row = mysql_fetch_row(result);
    if(!row) {
      break;
    }

    strcpy(ts, row[0]);
    errFlag = 0;
  }while(0);
  
  if(errFlag){
    *sql_code = mysql_errno(conn);
    ret = -1;
  }
  
  if(result){
    mysql_free_result(result);
  }
  
  return ret;
}


