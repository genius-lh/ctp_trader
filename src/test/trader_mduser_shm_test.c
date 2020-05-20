
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include "trader_mduser_shm.h"
#include "trader_data.h"

#define SHM_TEST_FILE "IB.MDUSER.INSTRUMENTS"

void test1(int size)
{
  trader_mduser_shm_key_file(SHM_TEST_FILE);
  trader_mduser_shm_header* hdr = trader_mduser_shm_header_init(sizeof(trader_tick), size);
  printf("hdr=%x\n", hdr); 
  printf("hdr->nShmId=%ld\n", hdr->nShmId);
  printf("hdr->nFieldNum=%ld\n", hdr->nFieldNum);
  printf("hdr->nFieldSize=%ld\n", hdr->nFieldSize);
  printf("hdr->nMaxFieldNum=%ld\n", hdr->nMaxFieldNum);
  printf("hdr->pData=%x\n", hdr->pData);
  trader_tick* tick = trader_mduser_shm_header_calloc(hdr, 2);
  printf("tick=%x\n", tick); 
  memset(tick, 0, sizeof(tick) * 2);
  
  strcpy(tick->InstrumentID, "IC1912");
  tick->BidPrice1 = 4456;
  tick->BidVolume1 = 2;
  tick->AskPrice1 = 4460;
  tick->AskVolume1 = 1;
  
  tick++;
  strcpy(tick->InstrumentID, "IC1908");
  tick->BidPrice1 = 4641;
  tick->BidVolume1 = 6;
  tick->AskPrice1 = 4641;
  tick->AskVolume1 = 1;

  trader_mduser_shm_header_dt(hdr);
}

void test2()
{
  trader_mduser_shm_key_file(SHM_TEST_FILE);
  trader_mduser_shm_header* hdr = trader_mduser_shm_header_at(SHM_TEST_FILE);
  int i;
  printf("hdr=%x\n", hdr);
  printf("hdr->nShmId=%ld\n", hdr->nShmId);
  printf("hdr->nFieldNum=%ld\n", hdr->nFieldNum);
  printf("hdr->nFieldSize=%ld\n", hdr->nFieldSize);
  printf("hdr->nMaxFieldNum=%ld\n", hdr->nMaxFieldNum);
  printf("hdr->pData=%x\n", hdr->pData);
  
  trader_tick* tick = (trader_tick*)hdr->pData;
  for(i = 0; i < hdr->nFieldNum; i++){    
    printf("InstrumentID[%s]\n", tick->InstrumentID);
    printf("BidPrice1[%lf]\n", tick->BidPrice1);
    printf("BidVolume1[%d]\n", tick->BidVolume1);
    printf("AskPrice1[%lf]\n", tick->AskPrice1);
    printf("AskVolume1[%d]\n", tick->AskVolume1);
    tick++;
  }
  trader_mduser_shm_header_dt(hdr);
}

void test3(char* t1, char* t2, char* weight)
{
  trader_mduser_shm_key_file(SHM_TEST_FILE);
  trader_mduser_shm_header* hdr = trader_mduser_shm_header_at();
  int i;
  trader_tick* t1Tick = NULL;
  trader_tick* t2Tick = NULL;
  double t2Weight = atof(weight);
  
  trader_tick* tick = (trader_tick*)hdr->pData;
  for(i = 0; i < hdr->nFieldNum; i++){
    if(!strcmp(tick->InstrumentID, t1)){
      t1Tick = tick;
    }else if(!strcmp(tick->InstrumentID, t2)){
      t2Tick = tick;
    }
    tick++;
  }

  if(t1Tick && t2Tick){
    printf("%s-%s*%s ask[%lf] bid[%lf]\n", t1, t2, weight, 
      t1Tick->AskPrice1 - t2Weight * t2Tick->AskPrice1,
      t1Tick->BidPrice1 - t2Weight * t2Tick->BidPrice1);
  }

  
  trader_mduser_shm_header_dt(hdr);
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    printf("usage: testMduserShm [1|2|3]\n");
    return -1;
  }

  if(argv[1][0] == '1'){
    test1(128);
  }

  if(argv[1][0] == '2'){
    test2();
  }

  if(argv[1][0] == '3'){
    if(argc == 5){
      test3(argv[2], argv[3], argv[4]);
    }
  }
  return 0;
}


