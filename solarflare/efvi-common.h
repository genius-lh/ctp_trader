#include <sys/uio.h>

typedef struct
{
    struct iovec iov[100];
    int count;
}recv_buf_t;


int init_connector(const char *interface,const char *filter);
void receive_frame(recv_buf_t *buf);
