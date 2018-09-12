#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trader_db.h"

static int trader_export_mduser(trader_db* pTraderDb, char* filename);
static int trader_export_order(trader_db* pTraderDb, char* filename);
static int trader_export_trade(trader_db* pTraderDb, char* filename);
static int trader_export_order_trade(trader_db* pTraderDb, char* filename);
static int trader_export_test(trader_db* pTraderDb);
static int trader_export_trade_total(trader_db* pTraderDb, char* filename);

int main(int argc, char* argv[])
{
  trader_db* pTraderDb = trader_db_new(TRADER_DB_NAME);
  
  pTraderDb->pMethod->xInit(pTraderDb, argv[1]);

  trader_export_mduser(pTraderDb, argv[1]);
  trader_export_order(pTraderDb, argv[1]);
  trader_export_trade(pTraderDb, argv[1]);
  trader_export_order_trade(pTraderDb, argv[1]);
  trader_export_trade_total(pTraderDb, argv[1]);
  
  pTraderDb->pMethod->xFini(pTraderDb);

  trader_db_free(pTraderDb);

  return 0;
}

int trader_export_mduser(trader_db* pTraderDb, char* filename)
{
  int nRet;
  int nErrCd;
  struct trader_db_mduser_inf_def mduser;

  FILE* fp;
  char db_file[1024];
  sprintf(db_file, "mduser_%s.csv", filename);
  fp = fopen(db_file, "w+");
  if(!fp){
    return -1;
  }

  fprintf(fp, "合约代码,");
  fprintf(fp, "交易日,");
  fprintf(fp, "最后修改时间,");
  fprintf(fp, "最后修改毫秒,");
  fprintf(fp, "申买价一,");
  fprintf(fp, "申买量一,");
  fprintf(fp, "申卖价一,");
  fprintf(fp, "申卖量一\n");

  nRet = pTraderDb->pMethod->xMduserDeclare(pTraderDb, &mduser, &nErrCd);

  nRet = pTraderDb->pMethod->xMduserOpen(pTraderDb, &mduser, &nErrCd);

  while(0 == (nRet = pTraderDb->pMethod->xMduserFetch(pTraderDb, &mduser, &nErrCd))){
    fprintf(fp, "%s,", mduser.InstrumentID);
    fprintf(fp, "%s,", mduser.TradingDay);
    fprintf(fp, "%s,", mduser.UpdateTime);
    fprintf(fp, "%d,", mduser.UpdateMillisec);
    fprintf(fp, "%9.2lf,", mduser.BidPrice1);
    fprintf(fp, "%d,", mduser.BidVolume1);
    fprintf(fp, "%9.2lf,", mduser.AskPrice1);
    fprintf(fp, "%d\n", mduser.AskVolume1);
  }
  
  nRet = pTraderDb->pMethod->xMduserClose(pTraderDb, &mduser, &nErrCd);

  fclose(fp);

  return 0;

}

int trader_export_order(trader_db* pTraderDb, char* filename)
{
  int nRet;
  int nErrCd;
  struct trader_db_order_inf_def record;

  FILE* fp;
  char db_file[1024];
  sprintf(db_file, "order_%s.csv", filename);
  fp = fopen(db_file, "w+");
  if(!fp){
    return -1;
  }

  fprintf(fp, "报单编号,");
  fprintf(fp, "合约,");
  fprintf(fp, "买卖,");
  fprintf(fp, "开平,");
  fprintf(fp, "投机套保标志,");
  fprintf(fp, "挂单状态,");
  fprintf(fp, "报单价格,");
  fprintf(fp, "报单手数,");
  fprintf(fp, "报单时间,");
  fprintf(fp, "本地发送时间,");
  fprintf(fp, "本地接收时间\n");


  nRet = pTraderDb->pMethod->xOrderDeclare(pTraderDb, &record, &nErrCd);

  nRet = pTraderDb->pMethod->xOrderOpen(pTraderDb, &record, &nErrCd);

  while(0 == (nRet = pTraderDb->pMethod->xOrderFetch(pTraderDb, &record, &nErrCd))){
    fprintf(fp, "\"%s\",", record.UserOrderLocalID);
    fprintf(fp, "%s,", record.InstrumentID);
    fprintf(fp, "%s,", record.Direction);
    fprintf(fp, "%s,", record.OffsetFlag);
    fprintf(fp, "%s,", record.HedgeFlag);
    fprintf(fp, "%s,", record.OrderStatus);
    fprintf(fp, "%9.2lf,", record.LimitPrice);
    fprintf(fp, "%d,", record.Volume);
    fprintf(fp, "%s,", record.InsertTime);
    fprintf(fp, "%s,", record.LocalCreateTime);
    fprintf(fp, "%s\n", record.LocalUpdateTime);
  }
  
  nRet = pTraderDb->pMethod->xOrderClose(pTraderDb, &record, &nErrCd);

  fclose(fp);

  return 0;
}

int trader_export_trade(trader_db* pTraderDb, char* filename)
{
  int nRet;
  int nErrCd;
  struct trader_db_trade_inf_def record;

  FILE* fp;
  char db_file[1024];
  sprintf(db_file, "trade_%s.csv", filename);
  fp = fopen(db_file, "w+");
  if(!fp){
    return -1;
  }

  nRet = pTraderDb->pMethod->xTradeDeclare(pTraderDb, &record, &nErrCd);

  nRet = pTraderDb->pMethod->xTradeOpen(pTraderDb, &record, &nErrCd);

  fprintf(fp, "合约代码,");
  fprintf(fp, "交易日,");
  fprintf(fp, "成交编号,");
  fprintf(fp, "本地报单编号,");
  fprintf(fp, "成交价格,");
  fprintf(fp, "成交数量,");
  fprintf(fp, "成交时间,");
  fprintf(fp, "本地接收时间\n");

  while(0 == (nRet = pTraderDb->pMethod->xTradeFetch(pTraderDb, &record, &nErrCd))){
    fprintf(fp, "%s,", record.InstrumentID);
    fprintf(fp, "%s,", record.TradingDay);
    fprintf(fp, "%s,", record.TradeID);
    fprintf(fp, "\"%s\",", record.UserOrderLocalID);
    fprintf(fp, "%9.2lf,", record.TradePrice);
    fprintf(fp, "%d,", record.TradeVolume);
    fprintf(fp, "%s,", record.TradeTime);
    fprintf(fp, "%s\n", record.LocalUpdateTime);
  }
  
  nRet = pTraderDb->pMethod->xTradeClose(pTraderDb, &record, &nErrCd);

  fclose(fp);

  return 0;

}

int trader_export_order_trade(trader_db* pTraderDb, char* filename)
{
  int nRet;
  int nRetView;
  int nErrCd;
  struct trader_db_order_inf_def record;
  struct trader_db_trade_view_def view;

  FILE* fp;
  char db_file[1024];
  sprintf(db_file, "order_trade_%s.csv", filename);
  fp = fopen(db_file, "w+");
  if(!fp){
    return -1;
  }

  //报单编号,合约,买卖,开平,挂单状态,报单价格,报单手数,详细状态,报单时间,最后成交时间,成交均价
  fprintf(fp, "报单编号,");
  fprintf(fp, "合约,");
  fprintf(fp, "买卖,");
  fprintf(fp, "开平,");
  fprintf(fp, "投机套保标志,");
  fprintf(fp, "挂单状态,");
  fprintf(fp, "报单价格,");
  fprintf(fp, "报单手数,");
  fprintf(fp, "报单时间,");
  fprintf(fp, "成交手数,");
  fprintf(fp, "最后成交时间,");
  fprintf(fp, "成交均价,");
  fprintf(fp, "本地发送时间,");
  fprintf(fp, "本地接收时间\n");


  nRet = pTraderDb->pMethod->xOrderDeclare(pTraderDb, &record, &nErrCd);

  nRet = pTraderDb->pMethod->xOrderOpen(pTraderDb, &record, &nErrCd);

  while(0 == (nRet = pTraderDb->pMethod->xOrderFetch(pTraderDb, &record, &nErrCd))){

    strcpy(view.UserOrderLocalID, record.UserOrderLocalID);
    nRetView = pTraderDb->pMethod->xTradeView(pTraderDb, &view, &nErrCd);
    
    fprintf(fp, "\"%s\",", record.UserOrderLocalID);
    fprintf(fp, "%s,", record.InstrumentID);
///买
#define USTP_FTDC_D_Buy '0'
///卖
#define USTP_FTDC_D_Sell '1'
    if('0' == record.Direction[0]){
      fprintf(fp, "买  ,");
    }else{
      fprintf(fp, "  卖,");
    }
///开仓
#define USTP_FTDC_OF_Open '0'
///平仓
#define USTP_FTDC_OF_Close '1'
    if('0' == record.OffsetFlag[0]){
      fprintf(fp, "开  ,");
    }else{
      fprintf(fp, "  平,");
    }
    
///投机
#define USTP_FTDC_CHF_Speculation '1'
///套利
#define USTP_FTDC_CHF_Arbitrage '2'
///套保
#define USTP_FTDC_CHF_Hedge '3'
///做市商
#define USTP_FTDC_CHF_MarketMaker '4'
    if('1' == record.HedgeFlag[0]){
      fprintf(fp, "投机,");
    }else if('2' == record.HedgeFlag[0]){
      fprintf(fp, "套利,");
    }else if('3' == record.HedgeFlag[0]){
      fprintf(fp, "套保,");
    }else if('4' == record.HedgeFlag[0]){
      fprintf(fp, "做市商,");
    }else{
      fprintf(fp, "其他,");
    }

///全部成交
#define USTP_FTDC_OS_AllTraded '0'
///部分成交还在队列中
#define USTP_FTDC_OS_PartTradedQueueing '1'
///部分成交不在队列中
#define USTP_FTDC_OS_PartTradedNotQueueing '2'
///未成交还在队列中
#define USTP_FTDC_OS_NoTradeQueueing '3'
///未成交不在队列中
#define USTP_FTDC_OS_NoTradeNotQueueing '4'
///撤单
#define USTP_FTDC_OS_Canceled '5'
///订单已报入交易所未应答
#define USTP_FTDC_OS_AcceptedNoReply '6'
    if('0' == record.OrderStatus[0]){
      fprintf(fp, "全部成交,");
    }else if('5' == record.OrderStatus[0]){
      fprintf(fp, "已撤单,");
    }else{
      fprintf(fp, "其他,");
    }
  
    fprintf(fp, "%9.2lf,", record.LimitPrice);
    fprintf(fp, "%d,", record.Volume);
    fprintf(fp, "%s,", record.InsertTime);

    if(nRetView < 0){
      fprintf(fp, "0,");
      fprintf(fp, "-,");
      fprintf(fp, "-,");
    }else{
      if(view.TradeVolume > 0){
        fprintf(fp, "%d,", view.TradeVolume);
        fprintf(fp, "%s,", view.TradeTime);
        fprintf(fp, "%9.2lf,", view.TradePrice);
      }else{
        fprintf(fp, "0,");
        fprintf(fp, "-,");
        fprintf(fp, "-,");
      }
    }
    fprintf(fp, "%s,", record.LocalCreateTime);
    fprintf(fp, "%s\n", record.LocalUpdateTime);
  }
  
  nRet = pTraderDb->pMethod->xOrderClose(pTraderDb, &record, &nErrCd);

  fclose(fp);

  return 0;
  
}

int trader_export_trade_total(trader_db* pTraderDb, char* filename)
{
  int nRet;
  int nRetView;
  int nErrCd;
  struct trader_db_order_inf_def record;
  struct trader_db_trade_view_def view;

  FILE* fp;
  char db_file[1024];
  sprintf(db_file, "trade_total_%s.csv", filename);
  fp = fopen(db_file, "w+");
  if(!fp){
    return -1;
  }

  fprintf(fp, "合约,");
  fprintf(fp, "买卖,");
  fprintf(fp, "开平,");
  fprintf(fp, "成交手数,");
  fprintf(fp, "成交均价,");
  fprintf(fp, "交易金额\n");


  nRet = pTraderDb->pMethod->xOrderDeclare(pTraderDb, &record, &nErrCd);

  nRet = pTraderDb->pMethod->xOrderOpen(pTraderDb, &record, &nErrCd);

  while(0 == (nRet = pTraderDb->pMethod->xOrderFetch(pTraderDb, &record, &nErrCd))){

    if('0' != record.OrderStatus[0]){
      continue;
    }

    strcpy(view.UserOrderLocalID, record.UserOrderLocalID);
    nRetView = pTraderDb->pMethod->xTradeView(pTraderDb, &view, &nErrCd);
    
    fprintf(fp, "%s,", record.InstrumentID);
    
    if('0' == record.Direction[0]){
      fprintf(fp, "买  ,");
    }else{
      fprintf(fp, "  卖,");
    }

    if('0' == record.OffsetFlag[0]){
      fprintf(fp, "开  ,");
    }else{
      fprintf(fp, "  平,");
    }
    

    if(view.TradeVolume > 0){
      fprintf(fp, "%d,", view.TradeVolume);
      fprintf(fp, "%9.2lf,", view.TradePrice);
    }else{
      fprintf(fp, "0,");
      fprintf(fp, "-,");
    }
    if('0' == record.Direction[0]){
      fprintf(fp, "-%9.2lf\n", view.TradePrice * view.TradeVolume);
    }else{
      fprintf(fp, "%9.2lf\n", view.TradePrice * view.TradeVolume);
    }
    
  }
  
  nRet = pTraderDb->pMethod->xOrderClose(pTraderDb, &record, &nErrCd);

  fclose(fp);

  return 0;
  
}


int trader_export_test(trader_db* pTraderDb)
{

  struct trader_db_order_inf_def oOrderInf;

  strncpy(oOrderInf.UserOrderLocalID, "2015061600006891", sizeof(oOrderInf.UserOrderLocalID));
  strncpy(oOrderInf.InstrumentID, "IF1506", sizeof(oOrderInf.InstrumentID));
  snprintf(oOrderInf.Direction, sizeof(oOrderInf.Direction), "%c", '1');
  snprintf(oOrderInf.OffsetFlag, sizeof(oOrderInf.OffsetFlag), "%c",'0');
  snprintf(oOrderInf.HedgeFlag, sizeof(oOrderInf.HedgeFlag), "%c", '1');
  snprintf(oOrderInf.OrderStatus, sizeof(oOrderInf.OrderStatus), "%c", '0');
  oOrderInf.LimitPrice = 5115.80;
  oOrderInf.Volume = 1;
  strncpy(oOrderInf.InsertTime, "11:08:34", sizeof(oOrderInf.InsertTime));
  strncpy(oOrderInf.LocalCreateTime, "11:08:34", sizeof(oOrderInf.LocalCreateTime));
  strncpy(oOrderInf.LocalUpdateTime, "11:08:34", sizeof(oOrderInf.LocalUpdateTime));
  int nSqlCode;
  pTraderDb->pMethod->xOrderInsert(pTraderDb, &oOrderInf, &nSqlCode);
  return 0;
}



