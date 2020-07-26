#ifndef COMMON_H
#define COMMON_H

/*
 * This file is ONLY used for demo.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <syslog.h>

#include <iostream>
#include <string>
#include <vector>


//
using namespace std;


enum LEVEL
{
    LEVEL_SUCCESS,
    LEVEL_INFO,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_UNKNOWN,
};

enum MSG
{
    RSP_CONNECTED,
    RSP_DISCONNECTED,
    RSP_USER_LOGIN,
    RSP_USER_LOGOUT,
    NAKED_PROTO_ON_RSP_USER_LOGIN,
    NAKED_PROTO_ON_RSP_USER_LOGOUT,
    RSP_USER_PASSWORD_UPDATE,
    RSP_CLIENT_ACCOUNT,
    RSP_CLIENT_POSITION,
    RSP_INSTRUMENT,
    RSP_INSTRUMENT_PRICE,
    RSP_INSTRUMENT_MARGIN_RATE,
    RSP_INSTRUMENT_COMMISSION_RATE,
    RSP_ORDER,
    RSP_TRADE,
    RSP_EXCHANGE_FRONT,
    RSP_MANUAL_INSERT_ORDER,
    NAKED_PROTO_ON_RSP_ORDER_INSERT,
    NAKED_PROTO_ON_RTN_ORDER,
    RSP_BAT_INSERT_ORDER,
    RSP_ACCOUNT,
    RSP_MARKET_DATA,
};


enum SYS_PARAM_LENGTH_TYE
{
    NAMED_ORDER_LEN = 48,
};


#if 1
#define   DbgPrint(fmt,...)   printf(fmt, ##__VA_ARGS__)
#elif 0
#define   DbgPrint(fmt,...)   syslog(LOG_WARNING, fmt, ##__VA_ARGS__)
#else
#define   DbgPrint(fmt,...)
#endif


#if 1
#define   ErrPrint(fmt,...)   printf(fmt, ##__VA_ARGS__)
#elif 0
#define   ErrPrint(fmt,...)   syslog(LOG_WARNING, fmt, ##__VA_ARGS__)
#else
#define   ErrPrint(fmt,...)
#endif


void PrintData(void* data, int len, bool bReadable = true);
void SplitString(string& s, vector<string>& v, string& c);
int ParseURL(string strURL, string& strProto, string& strIP, string& strPort);
int CreateTcpConnection(string strProto, string strIP, string strPort);
int CreateUdpConnection(string strProto, string strIP, string strPort, struct sockaddr_in& server_addr);
int ParsedMsgLen(void* data);
int ChangeStringToArray(char* s,char* b);
string GetProgramDirectory();
string TrimLineBreakSuffix(string str);

#endif // COMMON_H
