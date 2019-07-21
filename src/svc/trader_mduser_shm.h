#ifndef _TRADER_MDUSER_SHM_H_
#define _TRADER_MDUSER_SHM_H_


struct trader_mduser_shm_header_def{
  long nKeySize;
  long nFieldSize;
  long nMaxFieldNum;
  long nFieldNum;
  void* pData;
};

typedef struct trader_mduser_shm_header_def trader_mduser_shm_header;

extern trader_mduser_shm_header* trader_mduser_shm_header_init(char* key_file, int key_size, int field_size, int max_field_num);
extern void trader_mduser_shm_header_free(trader_mduser_shm_header* self);
extern trader_mduser_shm_header* trader_mduser_shm_header_at(char* key_file);
extern void trader_mduser_shm_header_dt(trader_mduser_shm_header* self);



#endif //_TRADER_MDUSER_SHM_H_

