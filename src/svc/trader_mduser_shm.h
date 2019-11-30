#ifndef _TRADER_MDUSER_SHM_H_
#define _TRADER_MDUSER_SHM_H_


struct trader_mduser_shm_header_def{
  long nShmId;
  long nMaxFieldNum;
  long nFieldSize;
  long nFieldNum;
  char pData[1];
};

typedef struct trader_mduser_shm_header_def trader_mduser_shm_header;

extern trader_mduser_shm_header* trader_mduser_shm_header_init(char* key_file, int field_size, int max_field_num);
extern trader_mduser_shm_header* trader_mduser_shm_header_at(char* key_file);
extern void trader_mduser_shm_header_dt(trader_mduser_shm_header* self);
extern void* trader_mduser_shm_header_calloc(trader_mduser_shm_header* self, int field_num);

#endif //_TRADER_MDUSER_SHM_H_

