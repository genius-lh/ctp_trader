

#include <stdio.h>


#include "comm_client_if.h"

struct channel_s* m_pAPIChannle = NULL;

static bool m_is_comm_release = false;

// 奇点API调用CommClient_SendData接口。
int CommClient_SendData(void *channel, const void* data, int len)
{
	if (m_is_comm_release)
	{
		printf("CommClient_SendData m_is_comm_release[%d]\n", m_is_comm_release);
		return -1;
	}
	struct channel_s* pChannel = (struct channel_s*)channel;
	if (NULL == pChannel)
	{
		pChannel = m_pAPIChannle;
	}

	// 此函数会被多个线程调用，因此客户自己实现发送函数时需要考虑线程安全。对于udp socket，发送函数是线程安全。
	int ret = sendto(pChannel->scoket_id, (const char *)data, len, 0, (struct sockaddr *)&pChannel->svr_addr, sizeof(pChannel->svr_addr));
	//printf("CommClient_SendData ret[%d]\n", ret);
	return ret;
}

void *commclient_recv_thread_func(void *arg)
{
	struct channel_s* pChannel = (struct channel_s*)arg;
	
	printf("begin to recv.\n");
	
	char buf[2048] = { 0 };
	int addr_len = sizeof(pChannel->local_addr);
	int ret = 0;
	while (1)
	{
		ret = recvfrom(pChannel->scoket_id, buf, sizeof(buf), 0, (struct sockaddr *) &pChannel->local_addr, (socklen_t*)&addr_len);
		if (m_is_comm_release)
		{
			printf("recvfrom error, release[%d], ret[%d], recv thread exit.\n", m_is_comm_release, ret);
			return 0;
		}
		if (ret <= 0)
		{
			printf("recvfrom error, release[%d], ret[%d].\n", m_is_comm_release, ret);
			continue;
		}
		//printf("recvfrom ret[%d]\n", ret);
		pChannel->OnRecvDataFun(pChannel, buf, ret, pChannel->user_data);
	}
}

pthread_t CommClient_RegFunAndStart(void *channel, CommClient_OnRecvData recv_fun, const void *user_data)
{
	printf("CommClient_RegFunAndStart\n");

	struct channel_s* pChannel = (struct channel_s*)channel;
	pChannel->OnRecvDataFun = recv_fun;
	pChannel->user_data = user_data;
	

	int scoket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if (scoket_id == -1)
	{
		perror("socket failed");
		return -1;
	}
	pChannel->scoket_id = scoket_id;

	// 通过cat /proc/sys/net/core/rmem_max 查看系统当前最大缓存区的大小，如果太小，需要增加系统最大缓冲区。
	int recvsize = 20 * 1024 * 1024;
	if (setsockopt(pChannel->scoket_id, SOL_SOCKET, SO_RCVBUF, (const char*)&recvsize, sizeof(int)) == -1)
	{
		printf("setsocket error\n");
	}
	else {
		printf("setsocket success, scoket_id[%d]\n", scoket_id);
	}		

	memset(&pChannel->svr_addr, 0, sizeof(struct sockaddr_in));
	pChannel->svr_addr.sin_family = AF_INET;
	pChannel->svr_addr.sin_port = htons(pChannel->remote_port);
	inet_aton(pChannel->remote_ip_addr, &pChannel->svr_addr.sin_addr);

	
	memset(&pChannel->local_addr, 0, sizeof(struct sockaddr_in));
	pChannel->local_addr.sin_family = AF_INET;
	pChannel->local_addr.sin_port = htons(pChannel->local_port);
	inet_aton(pChannel->local_ip_addr, &pChannel->local_addr.sin_addr);

	// bind
	if (bind(scoket_id, (struct sockaddr*)&pChannel->local_addr, sizeof(pChannel->local_addr)) < 0) {
		perror("can't bind local address");
		return -1;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, commclient_recv_thread_func, channel);	
	return tid;
}


void *CommClient_Init(const char *port_if_name, const char *remote_ip, unsigned short remote_port, const char* local_ip, unsigned short local_port)
{
	printf("CommClient_Init\n");
	m_is_comm_release = false;

	if (NULL == port_if_name)
	{
		printf("CommClient_Init on socket mode, if_name[null], local_ip[%s], local_port[%d], remote_ip[%s], remote_port[%d]\n", local_ip, local_port, remote_ip, remote_port);
	}
	else {
		printf("CommClient_Init on socket mode, if_name[%s], local_ip[%s], local_port[%d], remote_ip[%s], remote_port[%d]\n", port_if_name, local_ip, local_port, remote_ip, remote_port);
	}

	struct channel_s *pChannle = (struct channel_s *)malloc(sizeof(struct channel_s));
	memset(pChannle, 0, sizeof(struct channel_s));

	strcpy(pChannle->local_ip_addr, local_ip);
	pChannle->local_port = local_port;
	strcpy(pChannle->remote_ip_addr, remote_ip);
	pChannle->remote_port = remote_port;

	m_pAPIChannle = pChannle;

	return pChannle;
}

void CommClient_Release()
{
	if (m_pAPIChannle != NULL)
	{
		printf("close socket[%d]\n", m_pAPIChannle->scoket_id);
		close(m_pAPIChannle->scoket_id);
	}
	m_is_comm_release = true;
}

