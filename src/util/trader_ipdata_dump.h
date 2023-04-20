#ifndef _TRADER_IPDATA_DUMP_H_
#define _TRADER_IPDATA_DUMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct trader_ipdata_dump_def trader_ipdata_dump;
typedef struct trader_ipdata_dump_method_def trader_ipdata_dump_method;


struct trader_ipdata_dump_def {
  char store_file[64];
  int store_length;
  int store_cnt;
  int store_size;
  char* store_buff;
  trader_ipdata_dump_method* method;
};

struct trader_ipdata_dump_method_def {
  int (*xInit)(trader_ipdata_dump* self, const char* store_file, int store_length, int store_size);
  int (*xAdd)(trader_ipdata_dump* self, const void* data);
  int (*xFlush)(trader_ipdata_dump* self);
};

extern trader_ipdata_dump* trader_ipdata_dump_new();
extern void trader_ipdata_dump_free(trader_ipdata_dump* self);

#ifdef __cplusplus
}
#endif

#endif //_TRADER_IPDATA_DUMP_H_

