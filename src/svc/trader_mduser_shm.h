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

typedef int (*trader_mduser_shm_cmp_callback)(void* data1, void* data2);

extern void trader_mduser_shm_key_file(const char* key_file);
extern trader_mduser_shm_header* trader_mduser_shm_header_init(int field_size, int max_field_num);
extern trader_mduser_shm_header* trader_mduser_shm_header_at();
extern void trader_mduser_shm_header_dt(trader_mduser_shm_header* self);
extern void* trader_mduser_shm_header_calloc(trader_mduser_shm_header* self, int field_num);

extern int trader_mduser_shm_sort(trader_mduser_shm_header* self, trader_mduser_shm_cmp_callback cmp_func);
extern void* trader_mduser_shm_bsearch(trader_mduser_shm_header* self, void* data, trader_mduser_shm_cmp_callback cmp_func);


#endif //_TRADER_MDUSER_SHM_H_

