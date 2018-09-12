

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include<unistd.h>
#include <time.h>

#include "cmn_log.h"
#include "trader_db.h"
#include "sqlite3.h"

static int trader_db_mduser_insert(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
static int trader_db_mduser_declare(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
static int trader_db_mduser_open(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
static int trader_db_mduser_fetch(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
static int trader_db_mduser_close(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd);
static int trader_db_order_insert(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
static int trader_db_order_declare(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
static int trader_db_order_open(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
static int trader_db_order_fetch(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
static int trader_db_order_close(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd);
static int trader_db_trade_insert(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
static int trader_db_trade_declare(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
static int trader_db_trade_open(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
static int trader_db_trade_fetch(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
static int trader_db_trade_close(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd);
static int trader_db_trade_view(trader_db* self, struct trader_db_trade_view_def* p_data, int* errcd);
static int trader_db_init(trader_db* self, char* date);
static int trader_db_fini(trader_db* self);

static trader_db_method* trader_db_method_get();

static int trader_db_mduser_bind_param(trader_db* self, struct trader_db_mduser_inf_def* p_data);

static int trader_db_mduser_bind_result(trader_db* self, struct trader_db_mduser_inf_def* p_data);

static int trader_db_order_bind_param(trader_db* self, struct trader_db_order_inf_def* p_data);

static int trader_db_order_bind_result(trader_db* self, struct trader_db_order_inf_def* p_data);

static int trader_db_trade_bind_param(trader_db* self, struct trader_db_trade_inf_def* p_data);

static int trader_db_trade_bind_result(trader_db* self, struct trader_db_trade_inf_def* p_data);

static int trader_db_trade_view_bind_param(trader_db* self, struct trader_db_trade_view_def* p_data);

static int trader_db_trade_view_bind_result(trader_db* self, struct trader_db_trade_view_def* p_data);

int trader_db_mduser_insert(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd)
{
  int nRet = 0;

  nRet = sqlite3_reset(self->stmtMduserInsert);
  
  nRet = trader_db_mduser_bind_param(self, p_data);

  nRet = sqlite3_step(self->stmtMduserInsert);

  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    return -1;
  }
  return 0;
  
}

int trader_db_mduser_declare(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_mduser_open(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_mduser_fetch(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_step(self->stmtMduserSelect);
  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    CMN_ERROR("trader_db_mduser_fetch[%d]\n", nRet);
    return -1;
  }
  nRet = trader_db_mduser_bind_result(self, p_data);
  return 0;

}

int trader_db_mduser_close(trader_db* self, struct trader_db_mduser_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_reset(self->stmtMduserSelect);
  return nRet;

}

int trader_db_mduser_bind_param(trader_db* self, struct trader_db_mduser_inf_def* p_data)
{
  struct trader_db_mduser_inf_def* pRecord = p_data;
  int i = 1;
  int nRet = 0;

  nRet = sqlite3_bind_text(self->stmtMduserInsert, i++, pRecord->InstrumentID, sizeof(pRecord->InstrumentID), NULL);
  nRet = sqlite3_bind_text(self->stmtMduserInsert, i++, pRecord->TradingDay, sizeof(pRecord->TradingDay), NULL);
  nRet = sqlite3_bind_text(self->stmtMduserInsert, i++, pRecord->UpdateTime, sizeof(pRecord->UpdateTime), NULL);
  nRet = sqlite3_bind_int(self->stmtMduserInsert, i++, pRecord->UpdateMillisec);
  nRet = sqlite3_bind_double(self->stmtMduserInsert, i++, pRecord->BidPrice1);
  nRet = sqlite3_bind_int(self->stmtMduserInsert, i++, pRecord->BidVolume1);
  nRet = sqlite3_bind_double(self->stmtMduserInsert, i++, pRecord->AskPrice1);
  nRet = sqlite3_bind_int(self->stmtMduserInsert, i++, pRecord->AskVolume1);
  
  return 0;
}

int trader_db_mduser_bind_result(trader_db* self, struct trader_db_mduser_inf_def* p_data)
{
  struct trader_db_mduser_inf_def* pRecord = p_data;
  int i = 0;

  memcpy(pRecord->InstrumentID, 
    sqlite3_column_text(self->stmtMduserSelect, i), 
    sqlite3_column_bytes(self->stmtMduserSelect, i));
  i++;
  memcpy(pRecord->TradingDay, 
    sqlite3_column_text(self->stmtMduserSelect, i), 
    sqlite3_column_bytes(self->stmtMduserSelect, i));
  i++;
  memcpy(pRecord->UpdateTime, 
    sqlite3_column_text(self->stmtMduserSelect, i), 
    sqlite3_column_bytes(self->stmtMduserSelect, i));
  i++;

  pRecord->UpdateMillisec = sqlite3_column_int(self->stmtMduserSelect, i++);
  pRecord->BidPrice1 = sqlite3_column_double(self->stmtMduserSelect, i++);
  pRecord->BidVolume1 = sqlite3_column_int(self->stmtMduserSelect, i++);
  pRecord->AskPrice1 = sqlite3_column_double(self->stmtMduserSelect, i++);
  pRecord->AskVolume1 = sqlite3_column_int(self->stmtMduserSelect, i++);
  
  return 0;
}

int trader_db_order_insert(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd)
{
  int nRet = 0;

  nRet = sqlite3_reset(self->stmtOrderInsert);
  
  nRet = trader_db_order_bind_param(self, p_data);

  nRet = sqlite3_step(self->stmtOrderInsert);

  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    return -1;
  }
  return 0;
  
}

int trader_db_order_declare(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_order_open(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_order_fetch(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_step(self->stmtOrderSelect);
  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    CMN_ERROR("trader_db_order_fetch[%d]\n", nRet);
    return -1;
  }
  nRet = trader_db_order_bind_result(self, p_data);
  return 0;

}

int trader_db_order_close(trader_db* self, struct trader_db_order_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_reset(self->stmtOrderSelect);
  return nRet;

}

int trader_db_order_bind_param(trader_db* self, struct trader_db_order_inf_def* p_data)
{
  struct trader_db_order_inf_def* pRecord = p_data;
  int i = 1;
  int nRet = 0;
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->UserOrderLocalID, sizeof(pRecord->UserOrderLocalID), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->InstrumentID, sizeof(pRecord->InstrumentID), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->Direction, sizeof(pRecord->Direction), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->OffsetFlag, sizeof(pRecord->OffsetFlag), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->HedgeFlag, sizeof(pRecord->HedgeFlag), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->OrderStatus, sizeof(pRecord->OrderStatus), NULL);
  nRet = sqlite3_bind_double(self->stmtOrderInsert, i++, pRecord->LimitPrice);
  nRet = sqlite3_bind_int(self->stmtOrderInsert, i++, pRecord->Volume);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->InsertTime, sizeof(pRecord->InsertTime), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->LocalCreateTime, sizeof(pRecord->LocalCreateTime), NULL);
  nRet = sqlite3_bind_text(self->stmtOrderInsert, i++, pRecord->LocalUpdateTime, sizeof(pRecord->LocalUpdateTime), NULL);
  
  return 0;
}

int trader_db_order_bind_result(trader_db* self, struct trader_db_order_inf_def* p_data)
{
  struct trader_db_order_inf_def* pRecord = p_data;
  int i = 0;

  memcpy(pRecord->UserOrderLocalID, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->InstrumentID, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->Direction, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->OffsetFlag, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->HedgeFlag, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->OrderStatus, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  
  pRecord->LimitPrice = sqlite3_column_double(self->stmtOrderSelect, i++);
  pRecord->Volume = sqlite3_column_int(self->stmtOrderSelect, i++);

  memcpy(pRecord->InsertTime, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->LocalCreateTime, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;
  memcpy(pRecord->LocalUpdateTime, 
    sqlite3_column_text(self->stmtOrderSelect, i), 
    sqlite3_column_bytes(self->stmtOrderSelect, i));
  i++;

  
  return 0;
}

int trader_db_trade_insert(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd)
{
  int nRet = 0;

  nRet = sqlite3_reset(self->stmtTradeInsert);
  
  nRet = trader_db_trade_bind_param(self, p_data);

  nRet = sqlite3_step(self->stmtTradeInsert);

  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    return -1;
  }
  return 0;
  
}

int trader_db_trade_declare(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_trade_open(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd)
{
  return 0;
}

int trader_db_trade_fetch(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_step(self->stmtTradeSelect);
  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    CMN_ERROR("trader_db_trade_fetch[%d]\n", nRet);
    return -1;
  }
  nRet = trader_db_trade_bind_result(self, p_data);
  return 0;

}

int trader_db_trade_close(trader_db* self, struct trader_db_trade_inf_def* p_data, int* errcd)
{
  int nRet = 0;
  nRet = sqlite3_reset(self->stmtTradeSelect);
  return nRet;

}

int trader_db_trade_bind_param(trader_db* self, struct trader_db_trade_inf_def* p_data)
{
  struct trader_db_trade_inf_def* pRecord = p_data;
  int i = 1;
  int nRet = 0;

  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->InstrumentID, sizeof(pRecord->InstrumentID), NULL);
  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->TradingDay, sizeof(pRecord->TradingDay), NULL);
  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->TradeID, sizeof(pRecord->TradeID), NULL);
  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->UserOrderLocalID, sizeof(pRecord->UserOrderLocalID), NULL);
  nRet = sqlite3_bind_double(self->stmtTradeInsert, i++, pRecord->TradePrice);
  nRet = sqlite3_bind_int(self->stmtTradeInsert, i++, pRecord->TradeVolume);
  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->TradeTime, sizeof(pRecord->TradeTime), NULL);
  nRet = sqlite3_bind_text(self->stmtTradeInsert, i++, pRecord->LocalUpdateTime, sizeof(pRecord->LocalUpdateTime), NULL);
  
  return 0;
}

int trader_db_trade_bind_result(trader_db* self, struct trader_db_trade_inf_def* p_data)
{
  struct trader_db_trade_inf_def* pRecord = p_data;
  int i = 0;

  memcpy(pRecord->InstrumentID, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  memcpy(pRecord->TradingDay, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  memcpy(pRecord->TradeID, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  memcpy(pRecord->UserOrderLocalID, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  pRecord->TradePrice = sqlite3_column_double(self->stmtTradeSelect, i++);
  pRecord->TradeVolume = sqlite3_column_int(self->stmtTradeSelect, i++);
  memcpy(pRecord->TradeTime, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  memcpy(pRecord->LocalUpdateTime, 
    sqlite3_column_text(self->stmtTradeSelect, i), 
    sqlite3_column_bytes(self->stmtTradeSelect, i));
  i++;
  
  return 0;
}

int trader_db_trade_view(trader_db* self, struct trader_db_trade_view_def* p_data, int* errcd)
{
  int nRet;
  nRet = sqlite3_reset(self->stmtTradeView);
  
  nRet = trader_db_trade_view_bind_param(self, p_data);

  nRet = sqlite3_step(self->stmtTradeView);

  *errcd = 0;
  if(SQLITE_ROW != nRet){
    *errcd = nRet;
    return -1;
  }

  nRet = trader_db_trade_view_bind_result(self, p_data);

  return 0;
}

int trader_db_trade_view_bind_param(trader_db* self, struct trader_db_trade_view_def* p_data)
{
  struct trader_db_trade_view_def* pRecord = p_data;
  int i = 1;
  int nRet;
  nRet = sqlite3_bind_text(self->stmtTradeView, i++, pRecord->UserOrderLocalID, strlen(pRecord->UserOrderLocalID), NULL);
  
  return 0;
}

int trader_db_trade_view_bind_result(trader_db* self, struct trader_db_trade_view_def* p_data)
{
  struct trader_db_trade_view_def* pRecord = p_data;
  int i = 0;
  memcpy(pRecord->TradeTime, 
    sqlite3_column_text(self->stmtTradeView, i), 
    sqlite3_column_bytes(self->stmtTradeView, i));
  i++;
  pRecord->TradeVolume = sqlite3_column_int(self->stmtTradeView, i++);
  pRecord->TradePrice = sqlite3_column_double(self->stmtTradeView, i++);
  memcpy(pRecord->UserOrderLocalID, 
    sqlite3_column_text(self->stmtTradeView, i), 
    sqlite3_column_bytes(self->stmtTradeView, i));
  i++;

  return 0;
}

int trader_db_init(trader_db* self, char* date)
{
  int bExist = 0;
  int nRet = 0;
  char sFile[1024];

  snprintf(sFile, sizeof(sFile), "%s.%-8s", 
    self->pDbName, date);
  
  // 检查文件是否存在
  bExist = access(sFile, 0);

  // 打开数据库
  nRet = sqlite3_open(sFile, &self->db);

  if(SQLITE_OK != nRet){
    CMN_ERROR("open db [%s] failed!\n", sFile);
    return -1;
  }

  // 文件不存在,先建表
  if(0 != bExist){
    char MDUSER_CREATE[] = ""
      "CREATE TABLE"
      "    TBL_TRADER_MDUSER_INF"
      "    ("
      "        InstrumentID TEXT NOT NULL,"
      "        TradingDay TEXT NOT NULL,"
      "        UpdateTime TEXT NOT NULL,"
      "        UpdateMillisec INTEGER NOT NULL,"
      "        BidPrice1 NUMERIC,"
      "        BidVolume1 INTEGER,"
      "        AskPrice1 NUMERIC,"
      "        AskVolume1 INTEGER,"
      "        PRIMARY KEY (InstrumentID, TradingDay, UpdateTime, UpdateMillisec)"
      "    );";

    char ORDER_CREATE[] = ""
      "CREATE TABLE"
      "    TBL_TRADER_ORDER_INF"
      "    ("
      "        UserOrderLocalID TEXT NOT NULL,"
      "        InstrumentID TEXT NOT NULL,"
      "        Direction TEXT NOT NULL,"
      "        OffsetFlag TEXT NOT NULL,"
      "        HedgeFlag TEXT NOT NULL,"
      "        OrderStatus TEXT NOT NULL,"
      "        LimitPrice NUMERIC NOT NULL,"
      "        Volume INTEGER NOT NULL,"
      "        InsertTime TEXT NOT NULL,"
      "        LocalCreateTime TEXT NOT NULL,"
      "        LocalUpdateTime TEXT NOT NULL,"
      "        PRIMARY KEY (UserOrderLocalID)"
      "    );";
    
    char TRADE_CREATE[] = ""
      "CREATE TABLE"
      "    TBL_TRADER_TRADE_INF"
      "    ("
      "        InstrumentID TEXT NOT NULL,"
      "        TradingDay TEXT NOT NULL,"
      "        TradeID TEXT NOT NULL,"
      "        UserOrderLocalID TEXT NOT NULL,"
      "        TradePrice NUMERIC NOT NULL,"
      "        TradeVolume INTEGER NOT NULL,"
      "        TradeTime TEXT NOT NULL,"
      "        LocalUpdateTime TEXT NOT NULL,"
      "        PRIMARY KEY (TradeID)"
      "    );";
    char* pErrMsg;
    
    nRet = sqlite3_exec(self->db, MDUSER_CREATE, NULL, NULL, &pErrMsg);
    if(SQLITE_OK != nRet){
      CMN_ERROR("sqlite3_exec MDUSER_CREATE ret[%d]msg[%s]\n", nRet, pErrMsg);
    }

    nRet = sqlite3_exec(self->db, ORDER_CREATE, NULL, NULL, &pErrMsg);

    if(SQLITE_OK != nRet){
      CMN_ERROR("sqlite3_exec ORDER_CREATE ret[%d]msg[%s]\n", nRet, pErrMsg);
    }
    
    nRet = sqlite3_exec(self->db, TRADE_CREATE, NULL, NULL, &pErrMsg);

    if(SQLITE_OK != nRet){
      CMN_ERROR("sqlite3_exec TRADE_CREATE ret[%d]msg[%s]\n", nRet, pErrMsg);
    }
    
  }

  // 查询初始化
  char MDUSER_INSERT[] = ""
  "INSERT INTO TBL_TRADER_MDUSER_INF ("
    "InstrumentID,"
    "TradingDay,"
    "UpdateTime,"
    "UpdateMillisec,"
    "BidPrice1,"
    "BidVolume1,"
    "AskPrice1,"
    "AskVolume1"
  ") VALUES("
  "?,?,?,?,?,?,?,?);";
  
  char MDUSER_SELECT[] = ""
  "SELECT "
    "InstrumentID,"
    "TradingDay,"
    "UpdateTime,"
    "UpdateMillisec,"
    "BidPrice1,"
    "BidVolume1,"
    "AskPrice1,"
    "AskVolume1 "
  "FROM TBL_TRADER_MDUSER_INF;";

  char ORDER_INSERT[] = ""
    "INSERT INTO TBL_TRADER_ORDER_INF ("
      "UserOrderLocalID,"
      "InstrumentID,"
      "Direction,"
      "OffsetFlag,"
      "HedgeFlag,"
      "OrderStatus,"
      "LimitPrice,"
      "Volume,"
      "InsertTime,"
      "LocalCreateTime,"
      "LocalUpdateTime"
      ") VALUES("
    "?,?,?,?,?,?,?,?,?,?,?);";
  
  char ORDER_SELECT[] = ""
    "SELECT "
      "UserOrderLocalID,"
      "InstrumentID,"
      "Direction,"
      "OffsetFlag,"
      "HedgeFlag,"
      "OrderStatus,"
      "LimitPrice,"
      "Volume,"
      "InsertTime,"
      "LocalCreateTime,"
      "LocalUpdateTime "
    "FROM TBL_TRADER_ORDER_INF;";
  
  char TRADE_INSERT[] = ""
    "INSERT INTO TBL_TRADER_TRADE_INF ("
      "InstrumentID,"
      "TradingDay,"
      "TradeID,"
      "UserOrderLocalID,"
      "TradePrice,"
      "TradeVolume,"
      "TradeTime,"
      "LocalUpdateTime"
      ") VALUES("
    "?,?,?,?,?,?,?,?);";
  
  char TRADE_SELECT[] = ""
    "SELECT "
      "InstrumentID,"
      "TradingDay,"
      "TradeID,"
      "UserOrderLocalID,"
      "TradePrice,"
      "TradeVolume,"
      "TradeTime,"
      "LocalUpdateTime "
    "FROM TBL_TRADER_TRADE_INF;";

  char TRADE_VIEW[] = ""
    "SELECT "
    "  max(TradeTime),"
    "  sum(TradeVolume),"
    "  sum(TradePrice*TradeVolume)  / sum(TradeVolume),"
    "  UserOrderLocalID "
    "FROM "
    "  TBL_TRADER_TRADE_INF "
    "WHERE "
    "  substr(UserOrderLocalID, 1, 16) = ?;";
  
  nRet = sqlite3_prepare(self->db, MDUSER_INSERT, -1, &self->stmtMduserInsert, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare MDUSER_INSERT ret[%d]\n", nRet);
  }

  nRet = sqlite3_prepare(self->db, MDUSER_SELECT, -1, &self->stmtMduserSelect, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare MDUSER_SELECT ret[%d]\n", nRet);
  }

  nRet = sqlite3_prepare(self->db, ORDER_INSERT, -1, &self->stmtOrderInsert, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare ORDER_INSERT ret[%d]\n", nRet);
  }
  
  nRet = sqlite3_prepare(self->db, ORDER_SELECT, -1, &self->stmtOrderSelect, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare ORDER_SELECT ret[%d]\n", nRet);
  }
  
  nRet = sqlite3_prepare(self->db, TRADE_INSERT, -1, &self->stmtTradeInsert, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare TRADE_INSERT ret[%d]\n", nRet);
  }
  
  nRet = sqlite3_prepare(self->db, TRADE_SELECT, -1, &self->stmtTradeSelect, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare TRADE_SELECT ret[%d]\n", nRet);
  }

  nRet = sqlite3_prepare(self->db, TRADE_VIEW, -1, &self->stmtTradeView, 0);
  if(SQLITE_OK != nRet){
    CMN_ERROR("sqlite3_prepare TRADE_VIEW ret[%d]\n", nRet);
  }

  return 0;
  
}

int trader_db_fini(trader_db* self)
{
  sqlite3_finalize(self->stmtMduserInsert);
  sqlite3_finalize(self->stmtMduserSelect);
  sqlite3_finalize(self->stmtOrderInsert);
  sqlite3_finalize(self->stmtOrderSelect);
  sqlite3_finalize(self->stmtTradeInsert);
  sqlite3_finalize(self->stmtTradeSelect);
  sqlite3_close(self->db);
  return 0;
}

trader_db_method* trader_db_method_get()
{
  static trader_db_method trader_db_method_st = {
    trader_db_mduser_insert,
    trader_db_mduser_declare,
    trader_db_mduser_open,
    trader_db_mduser_fetch,
    trader_db_mduser_close,
    trader_db_order_insert,
    trader_db_order_declare,
    trader_db_order_open,
    trader_db_order_fetch,
    trader_db_order_close,
    trader_db_trade_insert,
    trader_db_trade_declare,
    trader_db_trade_open,
    trader_db_trade_fetch,
    trader_db_trade_close,
    trader_db_trade_view,
    trader_db_init,
    trader_db_fini
  };

  return &trader_db_method_st;
}



trader_db* trader_db_new(char* db_path)
{
  trader_db* self = (trader_db*)malloc(sizeof(trader_db));

  self->pDbName = db_path;

  self->pMethod = trader_db_method_get();
  
  return self;

}

void trader_db_free(trader_db* self)
{
  if(self){
    free(self);
  }
}

