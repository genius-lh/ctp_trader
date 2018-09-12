
#include "trader_daemon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>


#include "event2/util.h"

#include "cmn_util.h"
#include "cmn_cfg.h"

static void trader_daemon_set();
static trader_daemon* trader_daemon_new();
static void trader_daemon_free(trader_daemon* self);
static void trader_daemon_main(trader_daemon* self, int argc, char* argv[]);



int main(int argc,char* argv[])
{
  //trader_daemon_set();
  trader_daemon* self = trader_daemon_new();
  trader_daemon_main(self, argc, argv);
  trader_daemon_free(self);
  return 0;
}

void trader_daemon_set()
{
  int pid;
  int i;
  if(pid=fork())
    exit(0);//�Ǹ����̣�����������
  else if(pid< 0)
    exit(1);//forkʧ�ܣ��˳�
  //�ǵ�һ�ӽ��̣���̨����ִ��
  setsid();//��һ�ӽ��̳�Ϊ�µĻỰ�鳤�ͽ����鳤
  //��������ն˷���
  if(pid=fork())
    exit(0);//�ǵ�һ�ӽ��̣�������һ�ӽ���
  else if(pid< 0)
    exit(1);//forkʧ�ܣ��˳�
  //�ǵڶ��ӽ��̣�����
  //�ڶ��ӽ��̲����ǻỰ�鳤
  for(i=0;i< NOFILE;++i)//�رմ򿪵��ļ�������
    close(i);
  return;

}

trader_daemon* trader_daemon_new()
{
  trader_daemon* self = (trader_daemon*)malloc(sizeof(trader_daemon));
  self->trader = 0;

  return self;

}

void trader_daemon_free(trader_daemon* self)
{
  if(self->trader){
    free(self->trader);
  }
  
  if(self){
    free(self);
  }

}

void trader_daemon_main(trader_daemon* self, int argc, char* argv[])
{
  cmn_cfg* cfg = cmn_cfg_new(argv[1]);

  cfg->pMethod->xInit(cfg);

  self->nTraderNumber = cfg->pMethod->xGetConnNum(cfg);
  
  self->trader = (trader_daemon_pid_t*)malloc(self->nTraderNumber * sizeof(trader_daemon_pid_t));

  do{
    // ��¼pid
    char* pidFile;
    #define TRADER_PID_FILE "trader.pid"
    int len = strlen(cfg->pMethod->xGetWorkspace(cfg))+strlen(TRADER_PID_FILE) + 1+1;
    pidFile = (char*)malloc(sizeof(char) * len);
    sprintf(pidFile, "%s/%s", cfg->pMethod->xGetWorkspace(cfg), TRADER_PID_FILE);
    FILE* fp;
    fp = fopen(pidFile, "w+");
    int pid =  getpid();
    fprintf(fp, "%d", pid);
    fclose(fp);
    free(pidFile);
  }while(0);
    
  

  cmn_cfg_free(cfg);

  
  trader_daemon_pid_t nPid;

  int i;
  int n = self->nTraderNumber;


  // ����log
  nPid = fork();
  if(nPid < 0){
    // forkʧ��
    exit(-2);
  }else if( 0 == nPid){
    trader_daemon_free(self);
    log_main(argv[1]);
    exit(0);
  }else{
    self->log = nPid;
  }

  sleep(3);
  
  evutil_socket_t* pAudSock;
  evutil_socket_t* pTrdSock;
  evutil_socket_t pair[2];

  pAudSock = (evutil_socket_t*) malloc(n * sizeof(evutil_socket_t));
  pTrdSock = (evutil_socket_t*) malloc(n * sizeof(evutil_socket_t));

  for(i = 0; i < n; i++){
    if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1){
      exit(-1);
    }
    pAudSock[i] = pair[0];
    pTrdSock[i] = pair[1];
  }

  // ����aud
  nPid = fork();
  if(nPid < 0){
    // forkʧ��
    exit(-2);
  }else if( 0 == nPid){
    trader_daemon_free(self);
    aud_main(argv[1], pAudSock, n);
    for(i = 0; i < n; i++){
      evutil_closesocket(pTrdSock[i]);
      evutil_closesocket(pAudSock[i]);
    }
    free(pAudSock);
    free(pTrdSock);
    
    exit(0);
  }else{
    self->aud = nPid;
  }
  
  // ����trader
  
  for(i = 0; i < self->nTraderNumber; i++){
    nPid = fork();
    if(nPid < 0){
      // forkʧ��
      exit(-2);
    }else if( 0 == nPid){
      trader_daemon_free(self);
      trade_main(argv[1], pTrdSock[i]);
      
      for(i = 0; i < n; i++){
        evutil_closesocket(pTrdSock[i]);
        evutil_closesocket(pAudSock[i]);
      }
      free(pAudSock);
      free(pTrdSock);
      exit(0);
    }else{
      self->trader[i] = nPid;
    }
  }
  

  for(i = 0; i < n; i++){
    evutil_closesocket(pTrdSock[i]);
    evutil_closesocket(pAudSock[i]);
  }
  free(pAudSock);
  free(pTrdSock);
  

  static int nLoop = 1;
  
  // ע���ź�
  cmn_util_regist_signal(&nLoop);

  while(nLoop){
    sleep(10);
  }

  int iStatus;
  int iPid;

  // kill
  nPid = self->aud;
  kill(nPid, SIGTERM);
  
  for(i = 0; i < n; i++){
    nPid = self->trader[i];
    kill(nPid, SIGTERM);
  }

  //wait
  nPid = self->aud;
  iPid = waitpid(nPid, &iStatus, 0);

  for(i = 0; i < n; i++){
    nPid = self->trader[i];
    iPid = waitpid(nPid, &iStatus, 0);
  }
  

  nPid = self->log;
  kill(nPid, SIGTERM);
  
  iPid = waitpid(nPid, &iStatus, 0);

  return;
}




