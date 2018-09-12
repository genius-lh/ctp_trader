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

  fprintf(fp, "��Լ����,");
  fprintf(fp, "������,");
  fprintf(fp, "����޸�ʱ��,");
  fprintf(fp, "����޸ĺ���,");
  fprintf(fp, "�����һ,");
  fprintf(fp, "������һ,");
  fprintf(fp, "������һ,");
  fprintf(fp, "������һ\n");

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

  fprintf(fp, "�������,");
  fprintf(fp, "��Լ,");
  fprintf(fp, "����,");
  fprintf(fp, "��ƽ,");
  fprintf(fp, "Ͷ���ױ���־,");
  fprintf(fp, "�ҵ�״̬,");
  fprintf(fp, "�����۸�,");
  fprintf(fp, "��������,");
  fprintf(fp, "����ʱ��,");
  fprintf(fp, "���ط���ʱ��,");
  fprintf(fp, "���ؽ���ʱ��\n");


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

  fprintf(fp, "��Լ����,");
  fprintf(fp, "������,");
  fprintf(fp, "�ɽ����,");
  fprintf(fp, "���ر������,");
  fprintf(fp, "�ɽ��۸�,");
  fprintf(fp, "�ɽ�����,");
  fprintf(fp, "�ɽ�ʱ��,");
  fprintf(fp, "���ؽ���ʱ��\n");

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

  //�������,��Լ,����,��ƽ,�ҵ�״̬,�����۸�,��������,��ϸ״̬,����ʱ��,���ɽ�ʱ��,�ɽ�����
  fprintf(fp, "�������,");
  fprintf(fp, "��Լ,");
  fprintf(fp, "����,");
  fprintf(fp, "��ƽ,");
  fprintf(fp, "Ͷ���ױ���־,");
  fprintf(fp, "�ҵ�״̬,");
  fprintf(fp, "�����۸�,");
  fprintf(fp, "��������,");
  fprintf(fp, "����ʱ��,");
  fprintf(fp, "�ɽ�����,");
  fprintf(fp, "���ɽ�ʱ��,");
  fprintf(fp, "�ɽ�����,");
  fprintf(fp, "���ط���ʱ��,");
  fprintf(fp, "���ؽ���ʱ��\n");


  nRet = pTraderDb->pMethod->xOrderDeclare(pTraderDb, &record, &nErrCd);

  nRet = pTraderDb->pMethod->xOrderOpen(pTraderDb, &record, &nErrCd);

  while(0 == (nRet = pTraderDb->pMethod->xOrderFetch(pTraderDb, &record, &nErrCd))){

    strcpy(view.UserOrderLocalID, record.UserOrderLocalID);
    nRetView = pTraderDb->pMethod->xTradeView(pTraderDb, &view, &nErrCd);
    
    fprintf(fp, "\"%s\",", record.UserOrderLocalID);
    fprintf(fp, "%s,", record.InstrumentID);
///��
#define USTP_FTDC_D_Buy '0'
///��
#define USTP_FTDC_D_Sell '1'
    if('0' == record.Direction[0]){
      fprintf(fp, "��  ,");
    }else{
      fprintf(fp, "  ��,");
    }
///����
#define USTP_FTDC_OF_Open '0'
///ƽ��
#define USTP_FTDC_OF_Close '1'
    if('0' == record.OffsetFlag[0]){
      fprintf(fp, "��  ,");
    }else{
      fprintf(fp, "  ƽ,");
    }
    
///Ͷ��
#define USTP_FTDC_CHF_Speculation '1'
///����
#define USTP_FTDC_CHF_Arbitrage '2'
///�ױ�
#define USTP_FTDC_CHF_Hedge '3'
///������
#define USTP_FTDC_CHF_MarketMaker '4'
    if('1' == record.HedgeFlag[0]){
      fprintf(fp, "Ͷ��,");
    }else if('2' == record.HedgeFlag[0]){
      fprintf(fp, "����,");
    }else if('3' == record.HedgeFlag[0]){
      fprintf(fp, "�ױ�,");
    }else if('4' == record.HedgeFlag[0]){
      fprintf(fp, "������,");
    }else{
      fprintf(fp, "����,");
    }

///ȫ���ɽ�
#define USTP_FTDC_OS_AllTraded '0'
///���ֳɽ����ڶ�����
#define USTP_FTDC_OS_PartTradedQueueing '1'
///���ֳɽ����ڶ�����
#define USTP_FTDC_OS_PartTradedNotQueueing '2'
///δ�ɽ����ڶ�����
#define USTP_FTDC_OS_NoTradeQueueing '3'
///δ�ɽ����ڶ�����
#define USTP_FTDC_OS_NoTradeNotQueueing '4'
///����
#define USTP_FTDC_OS_Canceled '5'
///�����ѱ��뽻����δӦ��
#define USTP_FTDC_OS_AcceptedNoReply '6'
    if('0' == record.OrderStatus[0]){
      fprintf(fp, "ȫ���ɽ�,");
    }else if('5' == record.OrderStatus[0]){
      fprintf(fp, "�ѳ���,");
    }else{
      fprintf(fp, "����,");
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

  fprintf(fp, "��Լ,");
  fprintf(fp, "����,");
  fprintf(fp, "��ƽ,");
  fprintf(fp, "�ɽ�����,");
  fprintf(fp, "�ɽ�����,");
  fprintf(fp, "���׽��\n");


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
      fprintf(fp, "��  ,");
    }else{
      fprintf(fp, "  ��,");
    }

    if('0' == record.OffsetFlag[0]){
      fprintf(fp, "��  ,");
    }else{
      fprintf(fp, "  ƽ,");
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



