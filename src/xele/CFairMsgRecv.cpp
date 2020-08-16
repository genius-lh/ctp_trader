//
// Created by boz on 5/28/20.
//

#include "CFairMsgRecv.h"
#include <common.h>


CFairMsgRecv::CFairMsgRecv(int sock)
{
    m_sock = sock;
    memset(m_buf, 0, RECV_BUF_SIZE);
    m_recv = m_buf;
    m_data = m_buf;
    m_end = m_buf + RECV_BUF_SIZE;
    m_data_len = 0;
}

CFairMsgRecv::~CFairMsgRecv()
{

}


bool CFairMsgRecv::RecvMsg(faircpp::FairMessageParse& msg)
{
    unsigned char type;
    unsigned char extLen; //zb:
    unsigned short contentLen;
    int curMsgLen;
    int n;

    while (1)
    {
        if (m_data_len >= 4)
        {
            type = *m_data;
            extLen = *(m_data + 1);
            contentLen = *(unsigned short*)(m_data + 2); //zb:
            curMsgLen = 4 + extLen + contentLen;
            if (m_data_len >= curMsgLen)
            {
                msg.ParseMsg(m_data, curMsgLen); //output the message to process.
                m_data += curMsgLen;
                m_data_len -= curMsgLen;
                return true;
            }
        }

        //
        m_recv = m_data + m_data_len;
        if (m_recv == m_end)
        {
            if (m_data_len > 0)
            {
                memmove(m_buf, m_data, m_data_len);
            }
            m_data = m_buf;
            m_recv = m_data + m_data_len;
        }
        else if (m_recv > m_end)
        {
            ErrPrint("[XeleDemo]====================FataError: m_recv > m_end!=====================\n\n");
            exit(-1);
        }

        //
        //DbgPrint("[XeleDemo] ready to call recv...\n");
        n = recv(m_sock, m_recv, m_end - m_recv, 0);
        DbgPrint("\n[XeleDemo] recv data. bytes: %d\n", n);
        if (n == 0)
        {
            //DbgPrint("[XeleDemo]  return false (recv=== n == 0)\n");
            return false;
        }
        else if (n < 0)
        {
            if (errno == EAGAIN || errno == EINTR)
            {
                //DbgPrint("[XeleDemo]  EAGAIN or EINTR\n");
                continue;
            }
            //DbgPrint("[XeleDemo]  return false (recv===n < 0)\n");
            return false; //
        }
        else
        {
            //DbgPrint("Recv data [len: %d]...\n", n);
            PrintData(m_data, n, false);
        }
        m_data_len += n;
    }
}
