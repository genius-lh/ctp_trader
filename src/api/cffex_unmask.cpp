#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <time.h>
#include <sys/time.h>

struct cffex_l2_state {
  long long mask;
  int (*unmask)(char* data, int len, char param);
  char date[9];
  char param;
};

extern int cffex_l2_state_load();
extern int cffex_l2_state_init(void* original_data, void* masked_data);
extern int cffex_l2_state_unmask(char* data, int len);
extern int cffex_l2_state_ready();
extern int cffex_l2_state_save();
extern char* cffex_l2_state_date();

static int cffex_l2_state_mask_init();
static struct cffex_l2_state* cffex_l2_state_get();

struct cffex_l2_state* cffex_l2_state_get()
{
  static struct cffex_l2_state state = {
    0,
    NULL,
    {0}
  };
  return &state;
}

static inline int unmask0(char* data, int len, char param)
{
  return len;
}

static inline int unmask1(char* data, int len, char param)
{
  data[3] ^= data[2];
  data[7] ^= data[6];
  data[11] ^= data[10];
  data[15] ^= data[14];
  data[19] ^= data[18];
  data[23] ^= data[22];
  if(len > 24){
    data[27] ^= data[26];
  }

  return len;
}

static inline int unmask2(char* data, int len, char param)
{
  int* d1 = (int*)data;

  d1[0] ^= 0xFFFFFFFF;
  d1[1] ^= 0xFFFFFFFF;
  d1[2] ^= 0xFFFFFFFF;
  d1[3] ^= 0xFFFFFFFF;
  d1[4] ^= 0xFFFFFFFF;
  if(len > 24){
    d1[5] ^= 0xFFFFFFFF;
  }
  return len;
}

static inline int unmask3(char* data, int len, char param)
{
  int m = (int)((param +  len - 0x1c)&0xff); 
  int i = 0;
  for(i = 0; i < len; i += 2){
    data[i] ^= m;
    m-=2;
  }
  
  return len;
}

int cffex_l2_state_mask_init()
{
  struct cffex_l2_state* state = cffex_l2_state_get();

  state->param = (char)(state->mask & 0xff);
  
  if(0 == state->mask) {
    state->unmask = unmask0;
  }else if(0x00 == (state->param & 0xff)){
    state->unmask = unmask1;
  }else if(0xff == (state->param & 0xff)){
    state->unmask = unmask2;
  }else {
    state->unmask = unmask3;
  }
  return 0;
}


int cffex_l2_state_load()
{
  struct cffex_l2_state* state = cffex_l2_state_get();

  // 获取日期
  time_t tt = time(NULL);
  struct tm now;
  localtime_r(&tt, &now);
  
  snprintf(state->date, sizeof(state->date), "%04d%02d%02d",
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);

  int ret;
  char maskFile[64];
  snprintf(maskFile, sizeof(maskFile), "cffex_l2_%s", state->date);

  FILE* fp = fopen(maskFile, "r");
  if(!fp){
    return -2;
  }

  ret = fread(&state->mask, sizeof(state->mask), 1, fp);

  fclose(fp);
  
  if(ret < 1){
    return -3;
  }

  cffex_l2_state_mask_init();

  return 0;
}

int cffex_l2_state_save()
{
  struct cffex_l2_state* state = cffex_l2_state_get();
  
  int ret;
  char maskFile[64];
  snprintf(maskFile, sizeof(maskFile), "cffex_l2_%s", state->date);

  FILE* fp = fopen(maskFile, "r");
  if(fp){
    // 文件已存在
    fclose(fp);
    return -2;
  }
  
  fp = fopen(maskFile, "w");
  if(!fp){
    // 文件创建异常
    return -3;
  }
  
  ret = fwrite(&state->mask, sizeof(state->mask), 1, fp);

  fclose(fp);
  
  if(ret < 1){
    return -3;
  }

  return 0;
}

int cffex_l2_state_init(void* original_data, void* masked_data)
{
  struct cffex_l2_state* state = cffex_l2_state_get();
  if(state->unmask){
    return 0;
  }

  if(0x7f == *(char*)original_data){
    return 0;
  }

  long long openPrice = *((long long*)original_data);
  long long lastPrice = *((long long*)masked_data);
  
  state->mask = openPrice ^ lastPrice;

  cffex_l2_state_mask_init();

  cffex_l2_state_save();

  return 0;
}

int cffex_l2_state_unmask(char* data, int len)
{
  struct cffex_l2_state* state = cffex_l2_state_get();
  if(!state->unmask){
    return 0;
  }
  state->unmask(data, len, state->param);
  return 0;
}

int cffex_l2_state_ready()
{
  struct cffex_l2_state* state = cffex_l2_state_get();
  return (0 != state->unmask);
}

char* cffex_l2_state_date()
{
  struct cffex_l2_state* state = cffex_l2_state_get();
  return state->date;
}




