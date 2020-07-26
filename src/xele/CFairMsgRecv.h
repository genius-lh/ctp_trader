//
// Created by root on 5/28/20.
//

#ifndef TRADE_DEMO_CFAIRMSGRECV_H
#define TRADE_DEMO_CFAIRMSGRECV_H

#include <fair_message.h>
#include <sys/socket.h>


#define RECV_BUF_SIZE 6000

class CFairMsgRecv
{
public:
    CFairMsgRecv(int sock);
    ~CFairMsgRecv();
    bool RecvMsg(faircpp::FairMessageParse& msg);

private:
    int m_sock;
    unsigned char m_buf[RECV_BUF_SIZE];
    unsigned char* m_data;
    int m_data_len;
    unsigned char* m_recv;
    unsigned char* m_end;
};


#endif //TRADE_DEMO_CFAIRMSGRECV_H
