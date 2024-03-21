
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <limits.h>
#include <float.h>

#include "dict.h"

#include "trader_mduser_record.h"

void trader_mduser_record_init(trader_mduser_record* self)
{
  self->commitCount = 2000;
  self->linePos = 0;
  self->tickBuffer = (struct trader_mduser_tick_buff_def *)malloc(self->commitCount * sizeof(struct trader_mduser_tick_buff_def));
  
  time_t tt = time(NULL);
  struct tm now;

  tt += 4 * 60 * 60;
  localtime_r(&tt, &now);

  snprintf(self->csvFile, sizeof(self->csvFile),  "mduser%04d%02d%02d.csv", 
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  snprintf(self->tradingDay, sizeof(self->tradingDay),  "%04d%02d%02d", 
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);
  
  return;
}

void trader_mduser_record_on_tick(trader_mduser_record* self, struct trader_mduser_tick_buff_def *pMarketData)
{
  struct trader_mduser_tick_buff_def* buffer = &self->tickBuffer[self->linePos];
  memcpy(buffer, pMarketData, sizeof(struct trader_mduser_tick_buff_def));
  self->linePos++;

  if(self->linePos == self->commitCount){
    trader_mduser_record_flush(self);
  }
  return;
}

void trader_mduser_record_flush(trader_mduser_record* self)
{
  char line[1000];
  int len;
  if(!self->linePos){
    return ;
  }

  char* buffer = (char*)malloc(self->commitCount * 1000 * sizeof(char));
  struct ShfeDataField* tick;
  int i = 0;
  int pos = 0;
  for(i = 0; i < self->linePos; i++){
    tick = &self->tickBuffer[i];
    if((DBL_MAX == tick->depth[0].bid_price)
    ||(DBL_MAX == tick->depth[0].ask_price)){
      continue;
    }
    snprintf(line, sizeof(line), "%s,%s,%s,%d,%f,%f,%f,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d\n"
      , tick->instrument_id
      , self->tradingDay
      , tick->update_time
      , tick->update_msec
      , tick->last_price
      , tick->upper_limit
      , tick->lower_limit
      , tick->depth[0].bid_price
      , tick->depth[0].bid_volume
      , tick->depth[0].ask_price
      , tick->depth[0].ask_volume
      , tick->depth[1].bid_price
      , tick->depth[1].bid_volume
      , tick->depth[1].ask_price
      , tick->depth[1].ask_volume
      , tick->depth[2].bid_price
      , tick->depth[2].bid_volume
      , tick->depth[2].ask_price
      , tick->depth[2].ask_volume
      , tick->depth[3].bid_price
      , tick->depth[3].bid_volume
      , tick->depth[3].ask_price
      , tick->depth[3].ask_volume
      , tick->depth[4].bid_price
      , tick->depth[4].bid_volume
      , tick->depth[4].ask_price
      , tick->depth[4].ask_volume
    );

    len = strlen(line);
    memcpy(&buffer[pos], line, len);
    pos += len;
  }
  self->linePos = 0;

  do{
    FILE* fp = fopen(self->csvFile, "a");
    if(!fp){
      break;
    }

    fwrite(buffer, pos, 1, fp);
    
    fclose(fp);
  }while(0);
  
  free(buffer);

  return ;

}


void trader_mduser_record_free(trader_mduser_record* self)
{
  trader_mduser_record_flush(self);

  if(self->tickBuffer){
    free(self->tickBuffer);
    self->tickBuffer = NULL;
  }
}


