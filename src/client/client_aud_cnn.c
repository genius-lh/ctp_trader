
#include "client_aud_cnn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <resolv.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "cmn_log.h"
#include "cmn_util.h"
#include "cJSON.h"

#include "trader_msg_struct.h"

static int client_aud_cnn_on_trader_recv(client_aud_cnn* self);
static int client_aud_cnn_on_trader_send(client_aud_cnn* self);
static int client_aud_cnn_on_client_recv(client_aud_cnn* self);
static int client_aud_cnn_on_client_send(client_aud_cnn* self);
static int client_aud_cnn_on_close(client_aud_cnn* self);


static client_aud_cnn_method* client_aud_cnn_method_get();


static int client_aud_cnn_client_msg_proc(client_aud_cnn* self, unsigned char* buff, int len);
static int client_aud_cnn_trader_msg_proc(client_aud_cnn* self, unsigned char* buff, int len);
static int client_aud_cnn_pwd_decode(char* pwd, const char* pwd_enc);

static int client_aud_cnn_fill_req_update(struct trader_cmd_update_req_def* update,int num, cJSON* request);
static int client_aud_cnn_fill_rsp_update();
static int client_aud_cnn_fill_rsp_query_instrument();
static int client_aud_cnn_fill_rsp_query_position(char* position, int num, cJSON* item, cJSON* item1);
static int client_aud_cnn_conv_which_grid(const char* grid);

client_aud_cnn_method* client_aud_cnn_method_get()
{
  static client_aud_cnn_method st_client_aud_cnn_method = {
    client_aud_cnn_on_trader_recv,
    client_aud_cnn_on_trader_send,
    client_aud_cnn_on_client_recv,
    client_aud_cnn_on_client_send,
    client_aud_cnn_on_close
  };

  return &st_client_aud_cnn_method;
}

int client_aud_cnn_on_trader_recv(client_aud_cnn* self)
{
  CMN_DEBUG("Enter!\n");
  unsigned char buff[10000];
  int loop = 1;
  int nLen = 0;
  int nRet = 0;
  do {
    nLen = cmn_util_bufferevent_recv(self->BufTrader, buff, sizeof(buff));
    CMN_DEBUG("cmn_util_bufferevent_recv ret[%d]\n", nLen);
    if(nLen <= 0){
      loop = 0;
      break;
    }

    // 报文处理
    nRet = client_aud_cnn_trader_msg_proc(self, buff, nLen);
    if(nRet){
      CMN_ERROR("处理失败!\n");
      return -1;
    }    
  }while(loop);
  
  
  return 0;
}

int client_aud_cnn_on_trader_send(client_aud_cnn* self)
{
  CMN_DEBUG("Enter!\n");
  return 0;
}

int client_aud_cnn_on_client_recv(client_aud_cnn* self)
{
  CMN_DEBUG("Enter!\n");
  unsigned char buff[10000];
  int loop = 1;
  int nLen = 0;
  int nRet = 0;
  do {
    nLen = cmn_util_bufferevent_recv(self->BufClient, buff, sizeof(buff));
    if(nLen <= 0){
      loop = 0;
      break;
    }
    // 报文处理
    nRet = client_aud_cnn_client_msg_proc(self, buff, nLen);
    if(nRet){
      CMN_ERROR("处理失败!\n");
      return -1;
    }
    
  }while(loop);
  
  
  return 0;
}

int client_aud_cnn_on_client_send(client_aud_cnn* self)
{
  CMN_DEBUG("Enter!\n");
  return 0;
}

int client_aud_cnn_on_close(client_aud_cnn* self)
{
  CMN_DEBUG("Enter!\n");
  int nRet = 0;
  
  trader_msg_req_struct oReq;
  oReq.cmd = TRADER_CMD_TYPE_LOGOUT;
  // 组包发送
  unsigned char* pPkg = (unsigned char*)&oReq;
  int nPkgSize = sizeof(oReq) + sizeof(oReq.body.reserved) - sizeof(oReq.body);
  nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
  
  nRet = self->pool->pMethod->xFreeTrader(self->pool, self->UserId);
  return 0;
}

int client_aud_cnn_client_msg_proc(client_aud_cnn* self, unsigned char* buff, int len)
{
  int nRet = 0;
  int nPkgSize = 0;
  unsigned char* pPkg = NULL;
  cJSON* pCmd = NULL;
  cJSON* pTraceNo = NULL;
  cJSON* pRequest = NULL;
  int i = 0;
  
  cJSON* pChild;
  // 报文交易报文解析
  buff[len] ='\0';
  cJSON* pRoot = cJSON_Parse(buff);

  CMN_DEBUG("cjson request:[%s]\n", buff);
  if(!pRoot){
    CMN_ERROR("cJSON_Parse Failed!\n");
    return -1;
  }
  do{
  // 报文处理
    for(pChild = pRoot->child; pChild != NULL; pChild = pChild->next){
      if(!strcmp(pChild->string, "cmd")){
        pCmd = pChild;
      }else if(!strcmp(pChild->string, "traceNo")){
        pTraceNo = pChild;
      }else if(!strcmp(pChild->string, "request")){
        pRequest = pChild;
      }else{
        //EMPTY
      }
    }
    
    if(!pCmd){
      CMN_ERROR("Filed[Cmd] not exists!\n");
      nRet = -2;
      break;
    }
    
    if(!pTraceNo){
      CMN_ERROR("Filed[TraceNo] not exists!\n");
      nRet = -2;
      break;
    }

    CMN_DEBUG("cmd[%s]TransNo[%s]\n",  pCmd->valuestring, pTraceNo->valuestring);

    trader_msg_req_struct oReq;
    oReq.req = (void*)pRoot;
    strcpy(oReq.traceNo,pTraceNo->valuestring);
    pPkg = (unsigned char*)&oReq;
    
    if(!strcmp(pCmd->valuestring, "login")){
      // 获取变量
      oReq.cmd = TRADER_CMD_TYPE_LOGIN;

      struct trader_cmd_login_req_def *pLoginInf = &(oReq.body.login);
      for(pChild = pRequest->child; pChild != NULL; pChild = pChild->next){
        if(cJSON_String != pChild->type){
          continue;
        }
        if(!strcmp(pChild->string, "UserId")){
          strcpy(pLoginInf->UserID, pChild->valuestring);
        }else if(!strcmp(pChild->string, "Passwd")){
          memset(pLoginInf->Password, 0, sizeof(pLoginInf->Password));
          client_aud_cnn_pwd_decode(pLoginInf->Password, pChild->valuestring);
        }else{
          //EMPTY
        }
      }

      self->bLogin = 0;
      strcpy(self->UserId, pLoginInf->UserID);
      strcpy(self->Passwd, pLoginInf->Password);
      
      // 查询TraderPool      
      nRet = self->pool->pMethod->xGetTrader(self->pool, &self->BufTrader,
        self->UserId, self->Passwd);

      CMN_DEBUG("xGetTrader ret[%d]user[%s]BufTrader[%x]\n", nRet, self->UserId, (void*)self->BufTrader);
            
      if(0 != nRet){
        CMN_ERROR("xGetTrader ret[%d]user[%s]\n", nRet, self->UserId);
        nRet = -3;
        break;
      }
      
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.login) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);

    }else if(!strcmp(pCmd->valuestring, "updateall")){
      // 获取变量
      oReq.cmd = TRADER_CMD_TYPE_UPDATE_ALL;
      i = 0;
      for(pChild = pRequest->child; pChild != NULL; pChild = pChild->next){
        nRet = client_aud_cnn_fill_req_update(&oReq.body.update, i, pChild);
        i++;
      }
      oReq.body.update.num = i;
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.update) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "inform")){
      oReq.cmd = TRADER_CMD_TYPE_UPDATE;
      nRet = client_aud_cnn_fill_req_update(&oReq.body.update, 0, pRequest);
      oReq.body.update.num = 1;
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.update) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "update")){
      oReq.cmd = TRADER_CMD_TYPE_UPDATE;
      nRet = client_aud_cnn_fill_req_update(&oReq.body.update, 0, pRequest);
      oReq.body.update.num = 1;
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.update) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "QueryInstrument")){
      oReq.cmd = TRADER_CMD_TYPE_QUERY_INSTRUMENT;
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.reserved) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "QueryPosition")){
      oReq.cmd = TRADER_CMD_TYPE_QUERY_POSITION;
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.reserved) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "CloseAll")){
      oReq.cmd = TRADER_CMD_TYPE_CLOSE_ALL;
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.reserved) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else if(!strcmp(pCmd->valuestring, "ResetPosition")){
      oReq.cmd = TRADER_CMD_TYPE_RESET_POSITION;
      struct trader_cmd_reset_req_def* pReset = &(oReq.body.reset);
      for(pChild = pRequest->child; pChild != NULL; pChild = pChild->next){
        if(cJSON_String != pChild->type){
          continue;
        }
        if(!strcmp(pChild->string, "whichGrid")){
          pReset->StageID =  client_aud_cnn_conv_which_grid(pChild->valuestring);
        }else if(!strcmp(pChild->string, "Direction")){
          if(!strcmp(pChild->valuestring,"Buy")){
            pReset->Direction = TRADER_POSITION_BUY;
          }else{
            pReset->Direction = TRADER_POSITION_SELL;
          }
        }else if(!strcmp(pChild->string, "Volume")){
           pReset->Volume = atoi(pChild->valuestring);
        }else{
          //EMPTY
        }
      }
      
      // 组包发送
      nPkgSize = sizeof(oReq) + sizeof(oReq.body.reset) - sizeof(oReq.body);
      nRet = cmn_util_bufferevent_send(self->BufTrader, pPkg, nPkgSize);
    }else{
      CMN_ERROR("Cmd[%s] is not supported!\n", pCmd->valuestring);
      nRet = -3;
      break;
    }
    
  }while(0);
  
  return nRet;

}

int client_aud_cnn_trader_msg_proc(client_aud_cnn* self, unsigned char* buff, int len)
{
  CMN_DEBUG("Enter!\n");
  int i;
  // 报文交易报文解析
  trader_msg_rsp_struct oRsp;
  memcpy(&oRsp, buff,  len);
  
  // 报文处理
  cJSON* pRoot = cJSON_CreateObject();
  cJSON* pReqMsg = (cJSON*)oRsp.req;
  cJSON* pResponse;
  cJSON* pChild;
  cJSON* pChild1;
  cJSON* pPosition;
  cJSON* pRequest;

  // TraceNo
  cJSON_AddStringToObject(pRoot, "traceNo", oRsp.traceNo);

  // Response
  pResponse = cJSON_CreateObject();

  // ErrorID
  cJSON_AddNumberToObject(pResponse, "errorid",(double)oRsp.ErrorID);
  
  // ErrorMsg
  cJSON_AddStringToObject(pResponse, "errormsg",oRsp.ErrorMsg);
  
  cJSON_AddItemToObject(pRoot, "response", pResponse);
  

  if(TRADER_CMD_TYPE_LOGIN == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "login");
    if(oRsp.ErrorID){
      self->pool->pMethod->xFreeTrader(self->pool, self->UserId);
    }
  }else if (TRADER_CMD_TYPE_UPDATE == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "update");
    for(pRequest = pReqMsg->child; pRequest != NULL; pRequest = pRequest->next){
      if(!strcmp(pRequest->string,"request")){
        pChild = cJSON_Duplicate(pRequest,1);
        cJSON_AddItemToObject(pRoot, "request", pChild);
        break;
      }
    }
  }else if (TRADER_CMD_TYPE_UPDATE_ALL == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "updateall");
  }else if (TRADER_CMD_TYPE_QUERY_INSTRUMENT == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "QueryInstrument");
    pChild = cJSON_CreateArray();
    CMN_DEBUG("instrument cnt[%d]\n", oRsp.recNum);
    for(i = 0; i < oRsp.recNum; i++){
      CMN_DEBUG("instrument[%s]\n", &oRsp.buff[i*31]);
      cJSON_AddItemToArray(pChild, cJSON_CreateString(&oRsp.buff[i*31]));
    }
    cJSON_AddItemToObject(pResponse, "payload", pChild);
  }else if (TRADER_CMD_TYPE_QUERY_POSITION == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "QueryPosition");
    pChild = cJSON_CreateArray();
    pChild1 = cJSON_CreateArray();
    for(i = 0; i < oRsp.recNum; i++){
      client_aud_cnn_fill_rsp_query_position(&oRsp.buff[0], i, pChild, pChild1);
    }
    cJSON_AddItemToObject(pResponse, "payload", pChild);
    cJSON_AddItemToObject(pResponse, "payload1", pChild1);
  }else if (TRADER_CMD_TYPE_CLOSE_ALL == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "CloseAll");
  }else if (TRADER_CMD_TYPE_RESET_POSITION == oRsp.cmd){
    cJSON_AddStringToObject(pRoot, "cmd", "ResetPosition");
  }else if (TRADER_CMD_TYPE_LOGOUT== oRsp.cmd){
    
  }else{
  
  }

  int nRet;
  char * result = cJSON_PrintUnformatted(pRoot);
  
  CMN_DEBUG("cjson response:[%s]\n", result);

  nRet = cmn_util_bufferevent_send(self->BufClient, result, strlen(result));

  free(result);

  cJSON_Delete(pRoot);
  cJSON_Delete(pReqMsg);

  return 0;
}

int client_aud_cnn_pwd_decode(char* pwd, const char* pwd_enc)
{
  // 密码解密
  int nRet;
  nRet = b64_pton( pwd_enc, pwd, TraderPasswordTypeLen);
  if(nRet < 0){
    return -1;
  }
  return 0;
}

int client_aud_cnn_fill_req_update(struct trader_cmd_update_req_def* update, int num, cJSON* request)
{
  cJSON* pChild;
  cJSON* pRequest = request;

  char* T1;
  char* T2;

  // 新版本默认参数
  update->stage[num].AutoType = 0;
  update->stage[num].AutoKTOpen = 0;
  update->stage[num].AutoKTClose = 0;
  update->stage[num].AutoDTOpen = 0;
  update->stage[num].AutoDTClose = 0;
  update->stage[num].T1Weight = 1;
  update->stage[num].T2Weight = 1;
  update->stage[num].T2Ratio = 1;
  update->stage[num].NightClosingTime = 0;
  update->stage[num].TriggerType = 3;

  for(pChild = pRequest->child; pChild != NULL; pChild = pChild->next){
    if(!strcmp(pChild->string, "whichGrid")){
      //update->stage[num].StageId = pChild->valuestring[4] - 'A';
      update->stage[num].StageId = client_aud_cnn_conv_which_grid(pChild->valuestring);
    }else if(!strcmp(pChild->string, "StageId")){
      T1 = pChild->valuestring;
      T2 = strchr(pChild->valuestring, '-');
      if(T2){
        memcpy(update->stage[num].T1, T1, T2 - T1);
        update->stage[num].T1[T2 - T1] = '\0';
        T2++;
        strcpy(update->stage[num].T2, T2);
      }
    }else if(!strcmp(pChild->string, "limit")){
      update->stage[num].MP = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "lockNum")){
      update->stage[num].Hold = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "vol")){
      update->stage[num].Mult = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "kkjc")){
      update->stage[num].KTOpen = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "kp")){
      update->stage[num].KTClose = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "dkjc")){
      update->stage[num].DTOpen = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "dp")){
      update->stage[num].DTClose = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t1cj")){
      update->stage[num].T1Over = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t1dd")){
      update->stage[num].T1Wait = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t2cj")){
      update->stage[num].T2Over = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t2dd")){
      update->stage[num].T2Wait = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t2vol")){
      update->stage[num].PermitVol = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "cl")){
      update->stage[num].STG = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "autoCall")){
      update->stage[num].AutoType = 0;
      if(!strcmp(pChild->valuestring, "Time1")){
        update->stage[num].AutoType = 1;
      }else if (!strcmp(pChild->valuestring, "Time2")){
        update->stage[num].AutoType = 2;
      }
    }else if(!strcmp(pChild->string, "jjkk")){
      update->stage[num].AutoKTOpen = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "jjkp")){
      update->stage[num].AutoKTClose = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "jjdk")){
      update->stage[num].AutoDTOpen = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "jjdp")){
      update->stage[num].AutoDTClose = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t1Weight")){
      update->stage[num].T1Weight = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t2Weight")){
      update->stage[num].T2Weight = atof(pChild->valuestring);
    }else if(!strcmp(pChild->string, "t2Ratio")){
      update->stage[num].T2Ratio = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "nightClosingTime")){
        update->stage[num].NightClosingTime = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "triggerType")){
        update->stage[num].TriggerType = atoi(pChild->valuestring);
    }else if(!strcmp(pChild->string, "IsActivate")){
      if(pChild->type == cJSON_False){
        update->stage[num].Used = 0;
      }else{
        update->stage[num].Used = 1;
      }
    }else{
      //EMPTY
    }
    if(3 == update->stage[num].TriggerType){
      if((0 == memcmp(update->stage[num].T1, "IF", 2)
      ||(0 == memcmp(update->stage[num].T1, "IH", 2)
      ||(0 == memcmp(update->stage[num].T1, "IC", 2)){
        update->stage[num].TriggerType = 0;
      }
    }
  }
  
  return 0;
}

int client_aud_cnn_fill_rsp_query_position(char* position, int num, cJSON* item, cJSON* item1)
{
  CMN_DEBUG("Enter!\n");
  trade_position oTdPos;
  
  trade_position* pPosInf = &oTdPos;

  memcpy(pPosInf,  &position[num * sizeof(trade_position)], sizeof(trade_position));
  CMN_DEBUG("pPosInf->StageID=[%d]\n", pPosInf->StageID);
  CMN_DEBUG("pPosInf->T1=[%s]\n", pPosInf->T1);
  CMN_DEBUG("pPosInf->T2=[%s]\n", pPosInf->T2);
  CMN_DEBUG("pPosInf->Direction=[%c]\n", pPosInf->Direction);
  CMN_DEBUG("pPosInf->RealPrice=[%lf]\n", pPosInf->RealPrice);
  CMN_DEBUG("pPosInf->ExpPrice=[%lf]\n", pPosInf->ExpPrice);
  CMN_DEBUG("pPosInf->Volume=[%d]\n", pPosInf->Volume);
  cJSON* pPosition = cJSON_CreateObject();
  if(pPosInf->StageID < 0){
    cJSON_AddStringToObject(pPosition, "name", pPosInf->T1);
    cJSON_AddNumberToObject(pPosition, "cnt", pPosInf->Volume);

    cJSON_AddItemToArray(item1, pPosition);
  }else{
    // 填充仓位信息
    cJSON_AddNumberToObject(pPosition, "StageId", pPosInf->StageID);
    cJSON_AddStringToObject(pPosition, "T1", pPosInf->T1);
    cJSON_AddStringToObject(pPosition, "T2", pPosInf->T2);
    if(pPosInf->Direction == TRADER_POSITION_BUY){
      cJSON_AddStringToObject(pPosition, "Direction", "BUY");
    }else{
      cJSON_AddStringToObject(pPosition, "Direction", "SELL");
    }
    cJSON_AddNumberToObject(pPosition, "RealPrice", pPosInf->RealPrice);
    cJSON_AddNumberToObject(pPosition, "ExpPrice", pPosInf->ExpPrice);
    cJSON_AddNumberToObject(pPosition, "Volume", pPosInf->Volume);
    
    cJSON_AddItemToArray(item, pPosition);
  }
  return 0;
}

int client_aud_cnn_conv_which_grid(const char* grid)
{
  int stageId = 0;
  if(grid[4] >= 'A' && grid[4] <= 'F'){
    stageId = grid[4] - 'A';
  }else{
    stageId = (grid[4] - '0') * 10 + (grid[5] - '0') - 1;
  }
  
  return stageId;
}


client_aud_cnn* client_aud_cnn_new()
{
  client_aud_cnn* self = (client_aud_cnn*)malloc(sizeof(client_aud_cnn));

  self->pMethod = client_aud_cnn_method_get();

  self->bLogin = 0;

  memset(self->UserId, 0, sizeof(self->UserId));

  return self;

}

void client_aud_cnn_free(client_aud_cnn* self)
{
  if(self){
    free(self);
  }

}



