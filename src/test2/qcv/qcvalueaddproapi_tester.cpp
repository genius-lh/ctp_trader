#include "QCValueAddProApi.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "QCInfoSimpleSpi.h"

int main(int argc, char* argv[])
{
	if (argc<5)
	{
		printf("usage: ipaddress port userid passwd \n");
		return 1;
	}
	setbuf(stdout, NULL);
	strcpy(g_address, argv[1]);
	g_port = atoi(argv[2]);
	strcpy(g_userid, argv[3]);
	strcpy(g_passwd, argv[4]);

  
  time_t tt = time(NULL);
  struct tm now;
  localtime_r(&tt, &now);
  snprintf(g_filename, sizeof(g_filename), "qcv.%04d%02d%02d.csv",
    now.tm_year+1900, now.tm_mon+1, now.tm_mday);

	StartupThread* testthread = new StartupThread();
	testthread->Start();
	testthread->Join();
	return 1;
}

