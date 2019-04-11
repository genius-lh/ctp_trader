
#include "cmn_log_util.h"
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/stat.h>  
#include <sys/msg.h>


#define CMN_LOG_FILE "/cmn_log_file"

static que_msg_key_t cmn_log_file_name(char* log_path, int flag);

que_msg_key_t cmn_log_file_name(char* log_path, int flag)
{
  que_msg_key_t ret;
  FILE* fp;
  char* filename;
  filename = (char*)malloc(sizeof(char) * (strlen(log_path) + strlen(CMN_LOG_FILE) + 1));

  sprintf(filename, "%s%s", log_path, CMN_LOG_FILE);
  
  do {
    if(0 == flag){
      fp = fopen(filename, "w");

      if(!fp){
        ret = -1;
        break;
      }
      
      fclose(fp);
    }
    ret = ftok(filename, 1);

  }while(0);

  
  free(filename);
  return ret;
}



cmn_log_queue* cmn_log_queue_new()
{
  cmn_log_queue* self = (cmn_log_queue*)malloc(sizeof(cmn_log_queue));

  self->nType = 100;
  self->nKey = 0;
  self->nId = 0;
  self->nMaxCnt = 5000;
  return self;
}

void cmn_log_queue_free(cmn_log_queue* self)
{
  if(self){
    free(self);
  }
}

int cmn_log_queue_init(cmn_log_queue* self, char* log_path)
{
  que_msg_key_t nKey = cmn_log_file_name(log_path, 0);
  if(-1 == nKey){
    return -1;
  }


  self->nKey = nKey;


  que_msg_id_t nId = msgget(nKey, IPC_CREAT|0600);
  
  if(-1 == nId){
    return -2;
  }

  self->nId = nId;

  return 0;

}

int cmn_log_queue_close(cmn_log_queue* self)
{
  int nRet;
  que_msg_id_t nId = self->nId;

  nRet = msgctl(nId, IPC_RMID, NULL);

  return nRet;
}


int cmn_log_queue_get(cmn_log_queue* self, char* log_path)
{
  que_msg_key_t nKey = cmn_log_file_name(log_path, 1);
  
  
  que_msg_id_t nId = msgget(nKey, IPC_EXCL|0600);

  if(-1 == nId){
    return -1;
  }

  self->nKey = nKey;
  self->nId = nId;

  return 0;

}

int cmn_log_queue_send(cmn_log_queue* self, char* data, int len)
{
  int nRet = 0;
  cmn_queue_msg_t oMsg;
  que_msg_id_t nQueId = self->nId;

  if(len > sizeof(oMsg.sData)){
    // 长度超限
    return -1;
  }
  
  struct msqid_ds que_info;
  nRet = msgctl( nQueId, IPC_STAT, &que_info );
  if(nRet)  {
    // 队列状态异常
    return -2;
  }
  
  if( que_info.msg_qnum > self->nMaxCnt) {
    // 队列状态满
    return -3;
  }
  
  oMsg.nMsgTp = self->nType;
  memcpy(oMsg.sData, data, len);
  nRet = msgsnd(nQueId, &oMsg, len, IPC_NOWAIT);

  return nRet;
}

int cmn_log_queue_recv(cmn_log_queue* self, char* data, int len)
{
  int nRet = 0;
  cmn_queue_msg_t oMsg;

  nRet = msgrcv(self->nId, &oMsg, sizeof(oMsg), self->nType, MSG_NOERROR);
  if(nRet <= 0){
    // 获取队列失败
    return -1;
  }

  if(nRet > len){
    // 长度超限
    return -2;
  }
  
  memcpy(data, (void*)&oMsg.sData, nRet);

  return nRet;
}

