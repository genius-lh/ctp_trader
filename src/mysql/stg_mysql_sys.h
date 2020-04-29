#ifndef _STG_MYSQL_SYS_H_
#define _STG_MYSQL_SYS_H_

#include <string.h>

#include "mysql.h"

extern int stg_mysql_connect(char* db_name, char* db_usr, char* db_pwd, char* db_host, unsigned int db_port, int* sql_code);
extern int stg_mysql_disconnect(int* sql_code);
extern int stg_mysql_commit(int* sql_code);
extern int stg_mysql_rollback(int* sql_code);
extern int stg_mysql_stmt_perpare(void* param, const char* sql, void* bind, int* sql_code);
extern int stg_mysql_stmt_exec(void* param, void* bind, int* sql_code);
extern int stg_mysql_stmt_fetch(void* param, int* sql_code);
extern int stg_mysql_stmt_rows(void* param);
extern int stg_mysql_stmt_affected_rows(void* param);
extern int stg_mysql_stmt_close(void* param, int* sql_code);


extern int stg_mysql_get_count(const char* tbl_name, int* p_count, int* sql_code);
extern int stg_mysql_get_value(char* sql_get_value, int* p_count, int* sql_code);

extern int stg_mysql_add_ts(char* ts, int seconds, int* sql_code);
extern int stg_mysql_sub_current_ts(char* ts, int seconds, int* sql_code);

typedef my_bool mysqlbool;
typedef unsigned long mysqlulong;
typedef MYSQL_BIND mysqlbind;

#ifndef STG_MYSQL_BIND_STRING
#define STG_MYSQL_BIND_STRING(__bind, __idx, __table, __field) \
  do { \
    (&__bind[__idx])->buffer_type = MYSQL_TYPE_STRING; \
    (&__bind[__idx])->buffer = (void*)__table.__field; \
    (&__bind[__idx])->buffer_length = sizeof(__table.__field); \
    (&__bind[__idx])->is_null = &(__table##_is_null.__field); \
    (&__bind[__idx])->length = &(__table##_length.__field); \
    (__idx)++; \
  }while(0)
#endif

#ifndef STG_MYSQL_BIND_LONG
#define STG_MYSQL_BIND_LONG(__bind, __idx, __table, __field) \
  do { \
    (&__bind[__idx])->buffer_type = MYSQL_TYPE_LONG; \
    (&__bind[__idx])->buffer = (void*)&(__table.__field); \
    (&__bind[__idx])->buffer_length = sizeof(__table.__field); \
    (&__bind[__idx])->is_null = &(__table##_is_null.__field); \
    (&__bind[__idx])->length = &(__table##_length.__field); \
    (__idx)++; \
  }while(0)
#endif

#ifndef STG_MYSQL_BIND_DOUBLE
#define STG_MYSQL_BIND_DOUBLE(__bind, __idx, __table, __field) \
  do { \
    (&__bind[__idx])->buffer_type = MYSQL_TYPE_DOUBLE; \
    (&__bind[__idx])->buffer = (void*)&(__table.__field); \
    (&__bind[__idx])->buffer_length = sizeof(__table.__field); \
    (&__bind[__idx])->is_null = &(__table##_is_null.__field); \
    (&__bind[__idx])->length = &(__table##_length.__field); \
    (__idx)++; \
  }while(0)
#endif


#ifndef STG_MYSQL_BIND_BINARY
#define STG_MYSQL_BIND_BINARY(__bind, __idx, __table, __field) \
  do { \
    (&__bind[__idx])->buffer_type = MYSQL_TYPE_STRING; \
    (&__bind[__idx])->buffer = (void*)__table.__field; \
    (&__bind[__idx])->buffer_length = sizeof(__table.__field); \
    (&__bind[__idx])->is_null = &(__table##_is_null.__field); \
    (&__bind[__idx])->length = &(__table##_length.__field); \
    (__idx)++; \
  }while(0)
#endif

#ifndef STG_MYSQL_FILL_PARAM_STRING
#define STG_MYSQL_FILL_PARAM_STRING(__table, __param, __field) \
  do { \
    strncpy(__table.__field, __param->__field, sizeof(__table.__field)); \
    __table##_length.__field = strnlen(__table.__field, sizeof(__table.__field)); \
    __table##_is_null.__field = 0; \
  }while(0)
#endif

#ifndef STG_MYSQL_FILL_PARAM_LONG
#define STG_MYSQL_FILL_PARAM_LONG(__table, __param, __field) \
  do { \
    __table.__field = __param->__field; \
    __table##_is_null.__field = 0; \
  }while(0)
#endif

#ifndef STG_MYSQL_FILL_PARAM_DOUBLE
#define STG_MYSQL_FILL_PARAM_DOUBLE(__table, __param, __field) \
  do { \
    __table.__field = __param->__field; \
    __table##_is_null.__field = 0; \
  }while(0)
#endif


#ifndef STG_MYSQL_FILL_PARAM_BINARY
#define STG_MYSQL_FILL_PARAM_BINARY(__table, __param, __field) \
  do { \
    memcpy(__table.__field, __param->__field.data, __param->__field.length); \
    __table##_length.__field = __param->__field.length; \
    __table##_is_null.__field = 0; \
  }while(0)
#endif


#ifndef STG_MYSQL_FETCH_RESULT_STRING
#define STG_MYSQL_FETCH_RESULT_STRING(__table, __param, __field) \
  do { \
    strncpy(__param->__field, __table.__field, sizeof(__param->__field)); \
  }while(0)
#endif

#ifndef STG_MYSQL_FETCH_RESULT_LONG
#define STG_MYSQL_FETCH_RESULT_LONG(__table, __param, __field) \
  do { \
    __param->__field = __table.__field; \
  }while(0)
#endif

#ifndef STG_MYSQL_FETCH_RESULT_DOUBLE
#define STG_MYSQL_FETCH_RESULT_DOUBLE(__table, __param, __field) \
  do { \
    __param->__field = __table.__field; \
  }while(0)
#endif

#ifndef STG_MYSQL_FETCH_RESULT_BINARY
#define STG_MYSQL_FETCH_RESULT_BINARY(__table, __param, __field) \
  do { \
    __param->__field.length = __table##_length.__field; \
    memcpy(__param->__field.data, __table.__field, __table##_length.__field); \
  }while(0)
#endif

#endif



