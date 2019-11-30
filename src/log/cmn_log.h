#ifndef _CMN_LOG_H_
#define _CMN_LOG_H_

#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cmn_log_util.h"

typedef enum {
  /** fatal */	  CMN_LOG_PRIORITY_FATAL	= 0, 
  /** error */	  CMN_LOG_PRIORITY_ERROR	= 1, 
  /** warn */	    CMN_LOG_PRIORITY_WARN	= 2, 
  /** info */	    CMN_LOG_PRIORITY_INFO	= 3, 
  /** debug */	  CMN_LOG_PRIORITY_DEBUG	= 4,
  /** unknown */	CMN_LOG_PRIORITY_UNKNOWN	= 5
} cmn_log_priority_level_t;

typedef struct cmn_log_location_info_def{
  const char* file;
  const char* func;
  int line;
  pid_t pid;
}cmn_log_location_info;

#ifndef LOG4C_LOCATION_INFO_INITIALIZER
  #define LOG4C_LOCATION_INFO_INITIALIZER() {__FILE__, __FUNCTION__, __LINE__, getpid()} 
#endif

#define CMN_LOG_INI(...) \
  cmn_log_clt_ini(cmn_log_clt_get(), __VA_ARGS__)
  
#define CMN_LOG_FINI() \
  cmn_log_clt_fini(cmn_log_clt_get())
  
#define CMN_FATAL(...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(cmn_log_clt_get(), CMN_LOG_PRIORITY_FATAL, &location, __VA_ARGS__); \
  }while(0)

#define CMN_ERROR(...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(cmn_log_clt_get(), CMN_LOG_PRIORITY_ERROR, &location, __VA_ARGS__); \
  }while(0)

#define CMN_WARN(...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(cmn_log_clt_get(), CMN_LOG_PRIORITY_WARN, &location, __VA_ARGS__); \
  }while(0)

#define CMN_INFO(...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(cmn_log_clt_get(), CMN_LOG_PRIORITY_INFO, &location, __VA_ARGS__); \
  }while(0)

#define CMN_DEBUG(...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(cmn_log_clt_get(), CMN_LOG_PRIORITY_DEBUG, &location, __VA_ARGS__); \
  }while(0)
  
#define CMN_LOG_INI_R(_prt, ...) \
  do { \
    _prt = cmn_log_clt_new();\
    _prt->iSysPriority = 5;\
    cmn_log_clt_ini(_prt, __VA_ARGS__); \
  }while(0)
  
#define CMN_LOG_FINI_R(_prt) \
  do{ \
    cmn_log_clt_fini(_prt); \
    cmn_log_clt_free(_prt); \
  }while(0)
  
#define CMN_FATAL_R(_prt,...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(_prt, CMN_LOG_PRIORITY_FATAL, &location, __VA_ARGS__); \
  }while(0)

#define CMN_ERROR_R(_prt,...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(_prt, CMN_LOG_PRIORITY_ERROR, &location, __VA_ARGS__); \
  }while(0)

#define CMN_WARN_R(_prt,...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(_prt, CMN_LOG_PRIORITY_WARN, &location, __VA_ARGS__); \
  }while(0)

#define CMN_INFO_R(_prt,...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(_prt, CMN_LOG_PRIORITY_INFO, &location, __VA_ARGS__); \
  }while(0)

#define CMN_DEBUG_R(_prt,...) \
  do { \
    const cmn_log_location_info location = LOG4C_LOCATION_INFO_INITIALIZER(); \
    cmn_log(_prt, CMN_LOG_PRIORITY_DEBUG, &location, __VA_ARGS__); \
  }while(0)

typedef struct cmn_log_clt_method_def cmn_log_clt_method;
typedef struct cmn_log_clt_def cmn_log_clt;

struct cmn_log_clt_def{
  cmn_log_queue* pQueSvr;
  int iSysPriority;
  char catalog[CMN_LOG_CATALOG_NAME_SIZE+1];
  cmn_log_clt_method* pMethod;
};

struct cmn_log_clt_method_def{
  int (*xIni)(cmn_log_clt* self, char* cfg_file, char* a_catalog);
  void (*xLoggerMsg)(cmn_log_clt* self, int a_priority, const char* a_msg);
  void (*xFini)(cmn_log_clt* self);
  
};

extern cmn_log_clt* cmn_log_clt_new();
extern void cmn_log_clt_free(cmn_log_clt* self);

extern cmn_log_clt* cmn_log_clt_get();
extern int cmn_log_clt_ini(cmn_log_clt* self, char* cfg_file, char* a_catalog);
extern void cmn_log_clt_fini(cmn_log_clt* self);
extern void cmn_log_clt_logger_msg(cmn_log_clt* self, int a_priority, const char* a_msg);

extern void cmn_log( cmn_log_clt* self, int aiLevel, const cmn_log_location_info* location, const char *aFormat, ...)
  __attribute__((format(printf,4,5)));
extern void cmn_log_hex( cmn_log_clt* self, int aiLevel, cmn_log_location_info* location, const char* a_data, int a_len);


#ifndef CMN_ASSERT
#define CMN_ASSERT(...) \
  do                                                          \
  {                                                           \
    if (!(__VA_ARGS__))                                            \
    {                                                       \
      FILE* fp = fopen("cmn_assert_file", "a+"); \
      if(!fp){  exit(-1);}  \
      fprintf(fp, "[ASSERT] [%s:%d:%s]("#__VA_ARGS__ ") FAILED!\n", __FILE__, __LINE__, __FUNCTION__); \
      fclose(fp); \
      exit(-1);                                     \
    }                                                       \
  } while (0) 
#endif

#endif //_CMN_LOG_H_

