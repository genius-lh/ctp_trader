#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trader_db.h"

int main()
{
  int nRet;
  int nErrCd;
  struct trader_db_mduser_inf_def mduser;
  struct trader_db_position_inf_def position;
  trader_db* db = trader_db_new(TRADER_DB_NAME);
  nRet = db->pMethod->xInit(db, "20150415");
  strcpy(mduser.InstrumentID, "IF1506");
  strcpy(mduser.TradingDay, "20150415");
  strcpy(mduser.UpdateTime, "18:46:00");
  mduser.UpdateMillisec = 0;
  mduser.BidPrice1 = 2000.2f;
  mduser.BidVolume1 = 2;
  mduser.AskPrice1 = 2000.2f;
  mduser.AskVolume1 = 2;

  nRet = db->pMethod->xMduserInsert(db, &mduser, &nErrCd);

  nRet = db->pMethod->xMduserDeclare(db, &mduser, &nErrCd);

  nRet = db->pMethod->xMduserOpen(db, &mduser, &nErrCd);

  while(0 == (nRet = db->pMethod->xMduserFetch(db, &mduser, &nErrCd))){
    printf("%s\n", mduser.TradingDay);
  }
  
  nRet = db->pMethod->xMduserClose(db, &mduser, &nErrCd);
  
  db->pMethod->xFini(db);


  trader_db_free(db);

}

