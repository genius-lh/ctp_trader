#ifndef _TRADER_TICK_DICT_H_
#define _TRADER_TICK_DICT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct trader_tick_dict_def trader_tick_dict;
typedef struct trader_tick_dict_method_def trader_tick_dict_method;


struct trader_tick_dict_def {
  void* pDict;
  pthread_mutex_t oMutex;
  trader_tick_dict_method* pMethod;
};

struct trader_tick_dict_method_def {
  int (*xAdd)(trader_tick_dict* self, const char* key, const void* val);
  int (*xFind)(trader_tick_dict* self, const char* key, void** val);
};

extern trader_tick_dict* trader_tick_dict_new();
extern void trader_tick_dict_free(trader_tick_dict* self);

#ifdef __cplusplus
}
#endif

#endif //_TRADER_TICK_DICT_H_

