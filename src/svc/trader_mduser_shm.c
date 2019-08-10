
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/stat.h>  
#include <sys/shm.h>
#include "trader_mduser_shm.h"

#define SHM_TRACE(...) \
  do { \
    printf(__VA_ARGS__); \
  }while(0)


trader_mduser_shm_header* trader_mduser_shm_header_init(char* key_file, int field_size, int max_field_num)
{
  trader_mduser_shm_header* self;
  int nRet;
  do{
    // 获取key值
    key_t key = ftok(key_file, 1);
    if(-1 == key){
      SHM_TRACE("ftok error=%s\n", strerror(errno));
      break;
    }
    
    //获取shmid
    int nShmId = shmget(key, 0, 0);
    if(-1 != nShmId){
      // 共享内存存在
      self = (trader_mduser_shm_header*)shmat(nShmId, NULL, 0);
      if(-1 == self){
        SHM_TRACE("shmat error=%s\n", strerror(errno));
        break;
      }
      
      if(self->nMaxFieldNum >= max_field_num){
        return self;
      }
      
      // 断开共享内存
      nRet = shmdt(self);
      if(-1 == nRet){
        SHM_TRACE("shmdt error=%s\n", strerror(errno));
        break;
      }

      // 删除共享内存
      nRet = shmctl(nShmId, IPC_RMID, NULL);
      if(-1 == nRet){
        SHM_TRACE("shmctl error=%s\n", strerror(errno));
        break;
      }
    }
    
    nShmId =  shmget(key, sizeof(trader_mduser_shm_header) + field_size * max_field_num, IPC_CREAT|IPC_EXCL|0600);
    if(-1 == nShmId){
      SHM_TRACE("shmget error=%s\n", strerror(errno));
      break;
    }
    
    self = (trader_mduser_shm_header*)shmat(nShmId, NULL, 0);
    if(-1 == self){
      SHM_TRACE("shmat error=%s\n", strerror(errno));
      break;
    }
    self->nShmId = nShmId;
    self->nFieldNum = 0;
    self->nFieldSize = field_size;
    self->nMaxFieldNum = max_field_num;

    return self;
  }while(0);
  return (trader_mduser_shm_header*)NULL;
}

trader_mduser_shm_header* trader_mduser_shm_header_at(char* key_file)
{
  trader_mduser_shm_header* self;
  int nRet;
  do{
    // 获取key值
    key_t key = ftok(key_file, 1);
    if(-1 == key){
      SHM_TRACE("ftok error=%s\n", strerror(errno));
      break;
    }
    
    //获取shmid
    int nShmId = shmget(key, 0, 0);
    if(-1 == nShmId){
      SHM_TRACE("shmat error=%s\n", strerror(errno));
      break;
    }
    // 共享内存存在
    self = (trader_mduser_shm_header*)shmat(nShmId, NULL, SHM_RDONLY);
    if(-1 == self){
      SHM_TRACE("shmat error=%s\n", strerror(errno));
      break;
    }
    
    return self;
  }while(0);
  
  return (trader_mduser_shm_header*)NULL;
}

void trader_mduser_shm_header_dt(trader_mduser_shm_header* self)
{
  if(self){
    shmdt((void*)self);
  }
}

void* trader_mduser_shm_header_calloc(trader_mduser_shm_header* self, int field_num)
{
  if(!self){
    return (void*)NULL;
  }

  self->nFieldNum = field_num;
  return (void*)self->pData;
}

