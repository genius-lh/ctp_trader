
#pragma once
#include <stdlib.h>
#include <etherfabric/vi.h>
#include <etherfabric/pd.h>
#include <etherfabric/memreg.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAP_HUGETLB
/* Not always defined in glibc headers.  If the running kernel does not
 * understand this flag it will ignore it and you may not get huge pages.
 * (In that case ef_memreg_alloc() may fail when using packed-stream mode).
 */
# define MAP_HUGETLB		0x40000
#endif


#ifdef __PPC__
# define huge_page_size    (16ll * 1024 * 1024)
#elif defined(__x86_64__) || defined(__i386__)
# define huge_page_size    (2ll * 1024 * 1024)
#else
# error "Please define huge_page_size"
#endif

#define ROUND_UP(p, align)   (((p)+(align)-1u) & ~((align)-1u))

/* Hardware delivers at most ef_vi_receive_buffer_len() bytes to each
 * buffer (default 1792), and for best performance buffers should be
 * aligned on a 64-byte boundary.  Also, RX DMA will not cross a 4K
 * boundary.  The I/O address space may be discontiguous at 4K boundaries.
 * So easiest thing to do is to make buffers always be 2K in size.
 */
#define PKT_BUF_SIZE			2048

/* Align address where data is delivered onto EF_VI_DMA_ALIGN boundary,
 * because that gives best performance.
 */
#define RX_DMA_OFF				ROUND_UP(sizeof(struct pkt_buf), EF_VI_DMA_ALIGN)

#define REFILL_BATCH_SIZE		16

#define accept_discard_pkts 0

struct pkt_buf 
{
	/* I/O address corresponding to the start of this pkt_buf struct */
	ef_addr            m_ef_addr;

	/* pointer to where received packets start */
	void*              rx_ptr;

	int                id;
	struct pkt_buf*    next;
};

struct resources 
{
	/* handle for accessing the driver */
	ef_driver_handle   dh;

	/* protection domain */
	struct ef_pd       pd;

	/* virtual interface (rxq + txq) */
	struct ef_vi       vi;
	int                rx_prefix_len;
	int                pktlen_offset;

	/* registered memory for DMA */
	void*              pkt_bufs;
	int                pkt_bufs_n;
	struct ef_memreg   memreg;

	/* pool of free packet buffers (LIFO to minimise working set) */
	struct pkt_buf*    free_pkt_bufs;
	int                free_pkt_bufs_n;

  int (*read)(void* , const char* , int );
  void* arg;
};


#pragma pack(push, 1)
/// MAC头
struct mac_head 
{
public:
	unsigned char		m_dst_addr[6];
	unsigned char		m_src_addr[6];
	unsigned short		m_type;
};

/// IP头
union ip_version
{
	unsigned char		m_ver_and_len;
	struct 
	{
		unsigned int	m_head_len:4;
		unsigned int	m_version:4;
	};
};

union ip_offset
{
	unsigned short		m_slice_and_offset;
	struct  
	{
		unsigned int	m_slice:3;
		unsigned int	m_offset:13;
	};
};

union ip_addr
{
	unsigned int		m_ip;		
	struct  
	{
		unsigned char	m_b[4];
	};
};

struct ip_head
{
	ip_version			m_version;			///<版本与首部长度
	unsigned char		m_tos;				///<8位服务类型
	unsigned short		m_total_len;		///<16位总长度
	unsigned short		m_packet_id;		///<16位标识
	ip_offset			m_offset;			///<3位标志信息及以13位偏移
	unsigned char		m_ttl;				///<8位生成时间(TTL)
	unsigned char		m_protocol;			///<8位协议
	unsigned short		m_check_sum;		///<16位首部校验和
	ip_addr				m_src_ip;			///<32位源地址
	ip_addr				m_dst_ip;			///<32位目标地址
};

struct udp_head
{
	unsigned short		m_src_port;			///< 源端口
	unsigned short		m_dst_port;			///< 目标端口
	unsigned short		m_udp_len;			///< UDP长度
	unsigned short		m_crc;				///< 检验和
};

#pragma pack(pop)

extern bool init_resources(struct resources* res, const char* eth_name, const char* ip,  unsigned short port);
extern int poll_resources(struct resources* res);

#ifdef __cplusplus
}
#endif

