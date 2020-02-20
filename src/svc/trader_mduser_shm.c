
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


static char* trader_mduser_shm_key_file_get();

char* trader_mduser_shm_key_file_get()
{
  static char sShmKeyFile[256];

  return sShmKeyFile;
}

void trader_mduser_shm_key_file(const char* key_file)
{
  char* pKeyFile = trader_mduser_shm_key_file_get();
  strcpy(pKeyFile, key_file);
  return ;
}

trader_mduser_shm_header* trader_mduser_shm_header_init(int field_size, int max_field_num)
{
  char* key_file = trader_mduser_shm_key_file_get();

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
      if((void*)-1 == self){
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
    if((void*)-1 == self){
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

trader_mduser_shm_header* trader_mduser_shm_header_at()
{
  trader_mduser_shm_header* self;
  int nRet;

  char* key_file = trader_mduser_shm_key_file_get();
    
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
    self = (trader_mduser_shm_header*)shmat(nShmId, NULL, 0);
    if((void*)-1 == self){
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

int trader_mduser_shm_sort(trader_mduser_shm_header* self, trader_mduser_shm_cmp_callback cmp_func)
{
  int i;
  int j;
  int c;
  void* p;
  void* q;

  char* tmp = (char*)malloc(self->nFieldSize);

  for(i = 0; i < self->nFieldNum; i++){
    p = (void*)(self->pData + i * self->nFieldSize);
    for(j = i; j < self->nFieldNum; j++){
      q = (void*)(self->pData + j * self->nFieldSize);
      c = cmp_func(p, q);
      if(c > 0){
        memcpy(tmp, p, self->nFieldSize);
        memcpy(p, q, self->nFieldSize);
        memcpy(q, tmp, self->nFieldSize);
      }
    }
  }

  free(tmp);
  
  return 0;

}

void* trader_mduser_shm_bsearch(trader_mduser_shm_header* self, void* data, trader_mduser_shm_cmp_callback cmp_func)
{
  int h;
  int l;
  int m;
  int c;
  h = self->nFieldNum - 1;
  l = 0;
  m = self->nFieldNum / 2;

  void* p;

  while (l <= h) {
    m = (l + h) / 2;
    p = (void*)(self->pData + m * self->nFieldSize);
    c = cmp_func(p, data);
    if(0 == c){
      return (void*)p;
    }else if(c > 0) {
      h = m - 1;
    }else{
      l = m + 1;
    }
  }
  return (void*)NULL;

}


