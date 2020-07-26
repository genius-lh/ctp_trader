/*
 * This file is ONLY used for demo.
 */

#include "common.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


void PrintData(void* data, int len, bool bReadable)
{
#if 0
    unsigned char* p = (unsigned char*)data;
    for (int i = 0; i < len; i++)
    {
        if (bReadable)
        {
            DbgPrint("%.2X ", p[i]);
        }
        else
        {
            DbgPrint("%.2X", p[i]);
        }

    }
    DbgPrint("\n");
#endif
}


void SplitString(string& s, vector<string>& v, string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
    {
        v.push_back(s.substr(pos1));
    }
}



int ParseURL(string strURL, string& strProto, string& strIP, string& strPort)
{
    vector<string> v;
    string strDelimiter = ":";
    SplitString(strURL, v, strDelimiter);
    strProto = v[0];
    strIP = v[1].substr(2, v[1].length() - 2);
    strPort = v[2];
    return 0;
}


int CreateTcpConnection(string strProto, string strIP, string strPort)
{
    cout << strProto << " " << strIP << " " << strPort << endl;
    int socketFd = -1;
    if (strProto == "tcp")
    {
        unsigned short portnum = htons(atoi(strPort.c_str()));
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = portnum;
        if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            return -1;
        }
        inet_aton(strIP.c_str(), &server_addr.sin_addr);
        if (connect(socketFd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) != 0)
        {
            return -1;
        }
    }
    return socketFd;
}


int CreateUdpConnection(string strProto, string strIP, string strPort, struct sockaddr_in& server_addr)
{
    int socketFd = -1;
    if (strProto == "udp")
    {
        socketFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketFd <= 0)
        {
            return -1;
        }
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(strIP.c_str());
        server_addr.sin_port = htons(atoi(strPort.c_str()));
    }
    return socketFd;
}


int ParsedMsgLen(void* data)
{
    unsigned char* p = (unsigned char*)data;
    unsigned short extLen = *(p + 1);
    unsigned short* p2 = (unsigned short*)(p + 2);
    unsigned short contentLen = *p2;
    return 4 + extLen + contentLen;
}

int ChangeStringToArray(char* s,char* b)
{
    int i, n = 0;
    for(i = 0; s[i]; i += 2)
    {
        if(s[i] >= 'A' && s[i] <= 'F')
        {
            b[n] = s[i] - 'A' + 10;
        }
        else if (s[i] >= 'a' && s[i] <= 'f')
        {
            b[n] = s[i] - 'a' + 10;
        }
        else
        {
            b[n] = s[i] - '0';
        }

        if(s[i + 1] >= 'A' && s[i + 1] <= 'F')
        {
            b[n] = (b[n] << 4) | (s[i + 1] - 'A' + 10);
        }
        else if (s[i + 1] >= 'a' && s[i + 1] <= 'f')
        {
            b[n] = (b[n] << 4) | (s[i + 1] - 'a' + 10);
        }
        else
        {
            b[n] = (b[n] << 4) | (s[i + 1] - '0');
        }
        ++n;
    }
    return n;
}




string GetProgramDirectory()
{
    char buf[1024];
    int cnt = readlink("/proc/self/exe", buf, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        printf("Error: GetProgramDirectory error!\n");
        return "";
    }
    int i;
    for (i = cnt; i >=0; --i)
    {
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }
    return string(buf);
}


string TrimLineBreakSuffix(string str)
{
    char buf[1024];
    memset(buf, 0, 1024);
    int len = str.length();
    strcpy(buf, str.data());
    int i = len - 1;
    while (i >= 0)
    {
        if (buf[i] == 0x0d || buf[i] == 0x0a)
        {
            buf[i] = 0;
            i--;
        }
        else
        {
            break;
        }
    }
    return string(buf);
}
