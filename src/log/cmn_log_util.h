#ifndef _CMN_LOG_UTIL_H_
#define _CMN_LOG_UTIL_H_

#include <unistd.h>
#include <sys/types.h> 

typedef long que_msg_type_t;
typedef key_t que_msg_key_t;
typedef int que_msg_id_t;

#define CMN_QUEUE_MSG_SIZE 4096
#define CMN_QUEUE_NUM_SIZE 5000

typedef struct cmn_queue_msg_def{
  que_msg_type_t   nMsgTp                  ;    /*消息类型*/
  char     sData[CMN_QUEUE_MSG_SIZE] ;     /*消息内容*/
}cmn_queue_msg_t;


typedef struct cmn_log_queue_def cmn_log_queue;
struct cmn_log_queue_def {
  que_msg_key_t nKey;
  que_msg_id_t nId;
  que_msg_type_t nType;
  int nMaxCnt;
};


extern cmn_log_queue* cmn_log_queue_new();
extern void cmn_log_queue_free(cmn_log_queue* self);
extern int cmn_log_queue_init(cmn_log_queue* self, char* log_path);
extern int cmn_log_queue_close(cmn_log_queue* self);
extern int cmn_log_queue_get(cmn_log_queue* self, char* log_path);

extern int cmn_log_queue_send(cmn_log_queue* self, char* data, int len);
extern int cmn_log_queue_recv(cmn_log_queue* self, char* data, int len);

#include <time.h>
#include <sys/time.h>

#define CMN_LOG_CATALOG_NAME_SIZE 20

// 日志服务接口
typedef struct cmn_log_evt_def{
  char catalog[CMN_LOG_CATALOG_NAME_SIZE+1];
  int priority;
  struct timeval msg_time;
  int msg_len;
  char msg_data[2000];
}cmn_log_evt_t;



#endif //_CMN_LOG_UTIL_H_

