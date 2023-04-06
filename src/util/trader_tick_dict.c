#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "dict.h"
#include "trader_tick_dict.h"


static unsigned int tickHash(const void *key)
{
    return dictGenHashFunction((const unsigned char *)key,
                               strlen((const char*)key));
}

static void *tickKeyDup(void *privdata, const void *src)
{
    ((void) privdata);
    int l1 = strlen((const char*)src)+1;
    char* dup = (char*)malloc(l1 * sizeof(char));
    strncpy(dup, (const char*)src, l1);
    return dup;
}

static int tickKeyCompare(void *privdata, const void *key1, const void *key2)
{
    int l1, l2;
    ((void) privdata);

    l1 = strlen((const char*)key1);
    l2 = strlen((const char*)key2);
    if (l1 != l2) return 0;
    return memcmp(key1,key2,l1) == 0;
}

static void tickKeyDestructor(void *privdata, void *key)
{
    ((void) privdata);
    free((char*)key);
}

static dictType* tickDictTypeGet()
{
  static dictType tickDict = {
    tickHash,
    tickKeyDup,
    NULL,
    tickKeyCompare,
    tickKeyDestructor,
    NULL
  };
  return &tickDict;
}

static void trader_tick_dict_init(trader_tick_dict* self)
{
  dictType* tickDictType = tickDictTypeGet();
  self->pDict = dictCreate(tickDictType,NULL);
  pthread_mutex_init(&self->oMutex, NULL);
  return;
}

static int trader_tick_dict_add(trader_tick_dict* self, const char* key, const void* val)
{
  int ret;
  pthread_mutex_lock(&self->oMutex);
  ret = dictAdd(self->pDict, (void*)key, (void*)val);
  pthread_mutex_unlock(&self->oMutex);
  return ret;
}

static int trader_tick_dict_find(trader_tick_dict* self, const char* key, void** val)
{
  int ret;
  pthread_mutex_lock(&self->oMutex);
  dictEntry* entry = dictFind(self->pDict, (void*)key);
  ret = (NULL != entry);
  if(entry){
    *val = entry->val;
  }
  pthread_mutex_unlock(&self->oMutex);
  return ret;
}

static void trader_tick_dict_destory(trader_tick_dict* self)
{
  dictRelease(self->pDict);
  return;
}

static trader_tick_dict_method* trader_tick_dict_method_get()
{
  static trader_tick_dict_method trader_tick_dict_method_instance = {
    trader_tick_dict_add,
    trader_tick_dict_find
  };
  return &trader_tick_dict_method_instance;
}

trader_tick_dict* trader_tick_dict_new()
{
  trader_tick_dict* self = (trader_tick_dict*)malloc(sizeof(trader_tick_dict));
  self->pMethod = trader_tick_dict_method_get();
  trader_tick_dict_init(self);
  return self;
}

void trader_tick_dict_free(trader_tick_dict* self)
{
  if(!self){
    return;
  }
  trader_tick_dict_destory(self);
  free(self);
  return;
}

