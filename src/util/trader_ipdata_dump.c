#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trader_ipdata_dump.h"

static int trader_ipdata_dump_init(trader_ipdata_dump* self, const char* store_file, int store_length, int store_size)
{
  strncpy(self->store_file, store_file, sizeof(self->store_file));
  self->store_length = store_length;
  self->store_size = store_size;
  self->store_cnt = 0;
  self->store_buff = (char*)calloc(self->store_length, self->store_size);
  
  return 0;
}

static int trader_ipdata_dump_flush(trader_ipdata_dump* self)
{
  if(!self->store_cnt){
    return 0;
  }
  
  do{
    FILE* fp = fopen(self->store_file, "a");
    if(!fp){
      break;
    }

    fwrite(self->store_buff, self->store_length, self->store_cnt, fp);
    
    fclose(fp);

    self->store_cnt = 0;
  }while(0);
  
  return 0;
}

static int trader_ipdata_dump_add(trader_ipdata_dump* self, const void* data)
{
  
  memcpy(&self->store_buff[self->store_cnt * self->store_length], data, self->store_length);
  self->store_cnt++;
  
  if(self->store_cnt == self->store_size){
    printf("store buffer is full\n");
    trader_ipdata_dump_flush(self);
  }
  return 0;
}

static trader_ipdata_dump_method* trader_ipdata_dump_method_get()
{
  static trader_ipdata_dump_method trader_ipdata_dump_method_instance = {
    trader_ipdata_dump_init,
    trader_ipdata_dump_add,
    trader_ipdata_dump_flush
  };
  return &trader_ipdata_dump_method_instance;
}

trader_ipdata_dump* trader_ipdata_dump_new()
{
  trader_ipdata_dump* self = (trader_ipdata_dump*)malloc(sizeof(trader_ipdata_dump));
  self->method = trader_ipdata_dump_method_get();
  return self;
}

void trader_ipdata_dump_free(trader_ipdata_dump* self)
{
  if(!self){
    return;
  }

  trader_ipdata_dump_flush(self);

  if(self->store_buff){
    free(self->store_buff);
  }
  
  free(self);
  return;
}

