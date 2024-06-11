

#ifndef _COMM_IF_H__
#define _COMM_IF_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <stack> 
#include <queue>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>


//  数据接收通知
// user_data为上层用户调用RegFunAndStart时传入的自定义地址。
typedef void(*CommClient_OnRecvData)(const void* channel, const void* data, int len, const void* user_data);

struct channel_s
{
	CommClient_OnRecvData OnRecvDataFun;
	int scoket_id;
	struct sockaddr_in svr_addr;
	struct sockaddr_in local_addr;
	const void* user_data;

	char local_ip_addr[32];
	unsigned short local_port;
	char remote_ip_addr[32];
	unsigned short remote_port;

};

// 注册回调函数并启动通信层。
// user_data，上层用户自定义数据指针，在Recv数据的回调函数里将此指针直接返回给上层。
pthread_t CommClient_RegFunAndStart(void *channel, CommClient_OnRecvData recv_fun, const void *user_data);

// 发送数据
// channel： 从哪个通道发送数据。
// > 0, 发送成功的字节数。
// < 0，发送失败。
int CommClient_SendData(void *channel, const void* data, int len);

// 通信模块初始化
// local_port：客户端本地端口，用于接收nst发送过来的数据。每个客户都有一个不同的本地绑定端口。
// paras egs. "ens1f1", "172.21.132.240", 12346, "172.21.132.239", 12345
// 成功，返回channel指针，失败返回NULL.
void *CommClient_Init(const char *netcard_name, const char *remote_ip, unsigned short remote_port, const char* local_ip, unsigned short local_port);

void CommClient_Release();

#endif
