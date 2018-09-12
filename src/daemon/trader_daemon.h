#ifndef _TRADER_DAEMON_H_
#define _TRADER_DAEMON_H_

#include <sys/types.h>
#include <unistd.h>

#include "event2/util.h"


typedef pid_t trader_daemon_pid_t;


typedef struct trader_daemon_def trader_daemon;
struct trader_daemon_def {
  int nTraderNumber;
  trader_daemon_pid_t log;
  trader_daemon_pid_t aud;
  trader_daemon_pid_t* trader;
};

extern int log_main(char* cfg_file);
extern int aud_main(char* cfg_file, evutil_socket_t* socks, int num);
extern int trade_main(char* cfg_file, evutil_socket_t sock);

#endif //_TRADER_DAEMON_H_

