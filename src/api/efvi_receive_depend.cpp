#include "efvi_receive_depend.h"
#include "trader_ipdata_dump.h"

static int sl_parse(ef_filter_spec* fs, const char* ip,  unsigned short port);
static struct pkt_buf* pkt_buf_from_id(struct resources* res, int pkt_buf_i);
static void pkt_buf_free(struct resources* res, struct pkt_buf* pkt_buf);
static void handle_rx(struct resources* res, int pkt_buf_i, int len);
static void handle_rx_discard(struct resources* res, int pkt_buf_i, int len, int discard_type);
static void refill_rx_ring(struct resources* res);

bool init_resources(struct resources* res, const char* eth_name, const char* ip,  unsigned short port)
{
	/* Open driver and allocate a VI. */
	ef_driver_open(&res->dh);

	(ef_pd_alloc_by_name(&res->pd, res->dh, eth_name, EF_PD_DEFAULT));
  
	(ef_vi_alloc_from_pd(&res->vi, res->dh, &res->pd, res->dh,-1, 2048, 0, NULL, -1, EF_VI_FLAGS_DEFAULT));

	res->rx_prefix_len = ef_vi_receive_prefix_len(&res->vi);

	//LOGI("rxq_size=%d\n", ef_vi_receive_capacity(&res->vi));
	//LOGI("evq_size=%d\n", ef_eventq_capacity(&res->vi));
	//LOGI("rx_prefix_len=%d\n", res->rx_prefix_len);

	//ef_vi_receive_set_buffer_len(&res->vi,512*1024);

	/* Allocate memory for DMA transfers. Try mmap() with MAP_HUGETLB to get huge
	* pages. If that fails, fall back to posix_memalign() and hope that we do
	* get them. */
	res->pkt_bufs_n = ef_vi_receive_capacity(&res->vi);
	size_t alloc_size = res->pkt_bufs_n * PKT_BUF_SIZE;
	alloc_size = ROUND_UP(alloc_size, huge_page_size);
	res->pkt_bufs = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
	if( res->pkt_bufs == MAP_FAILED ) 
	{
		//LOGW("mmap() failed. Are huge pages configured?\n");

		/* Allocate huge-page-aligned memory to give best chance of allocating
		* transparent huge-pages.
		*/
		(posix_memalign(&res->pkt_bufs, huge_page_size, alloc_size) == 0);
	}

	int i;
	for( i = 0; i < res->pkt_bufs_n; ++i ) 
	{
		struct pkt_buf* pkt_buf = pkt_buf_from_id(res, i);
		pkt_buf->rx_ptr = (char*) pkt_buf + RX_DMA_OFF + res->rx_prefix_len;
		pkt_buf->id = i;
		pkt_buf_free(res, pkt_buf);
	}

	/* Register the memory so that the adapter can access it. */
	(ef_memreg_alloc(&res->memreg, res->dh, &res->pd, res->dh,res->pkt_bufs, alloc_size));

	for( i = 0; i < res->pkt_bufs_n; ++i ) 
	{
		struct pkt_buf* pkt_buf = pkt_buf_from_id(res, i);
		pkt_buf->m_ef_addr = ef_memreg_dma_addr(&res->memreg, i * PKT_BUF_SIZE);
	}

	/* Fill the RX ring. */
	while( ef_vi_receive_space(&res->vi) > REFILL_BATCH_SIZE )
		refill_rx_ring(res);

	/* Add filters so that adapter will send packets to this VI. */
	ef_filter_spec guava_80;
	if( sl_parse(&guava_80, ip, port) != 0 ) 
	{
		// LOGE("ERROR: Bad filter spec '%s'\n", argv[0]);
		exit(1);
	}
	(ef_vi_filter_add(&res->vi, res->dh, &guava_80, NULL));

  
  res->normal_count = 0;
  res->discard_count = 0;
  res->multi_count = 0;

  trader_ipdata_dump* dump_instance = trader_ipdata_dump_new();
  char store_file[64];
  do{
    struct tm now;
    time_t tt = time(NULL);
    
    tt += 4 * 60 * 60;
    localtime_r(&tt, &now);
    
    if(now.tm_wday > 5){
      tt += 24 * 60 * 60 * 2;
      localtime_r(&tt, &now);
    }
    
    sprintf(store_file, "efvi%d.%04d%02d%02d", port,
      now.tm_year+1900, now.tm_mon+1, now.tm_mday);

  }while(0);
  
  dump_instance->method->xInit(dump_instance, store_file, 172, 1024);

  res->dump_instance = (void*)dump_instance;


  return true;
}

int poll_resources(struct resources* res)
{
  trader_ipdata_dump* dump_instance = (trader_ipdata_dump*)res->dump_instance;
  ef_event evs[32];
  //ef_request_id ids[EF_VI_RECEIVE_BATCH];
  int i, n_ev;
  /* Poll event queue for events */
  n_ev = ef_eventq_poll(&res->vi, evs, sizeof(evs) / sizeof(evs[0]));
  if( n_ev > 0 ) 
  {
    for( i = 0; i < n_ev; ++i ) 
    {
      switch( EF_EVENT_TYPE(evs[i]) ) 
      {
      case EF_EVENT_TYPE_RX:
        /* This code does not handle jumbos. */
        assert(EF_EVENT_RX_SOP(evs[i]) != 0);
        assert(EF_EVENT_RX_CONT(evs[i]) == 0);
        handle_rx(res, EF_EVENT_RX_RQ_ID(evs[i]),
          EF_EVENT_RX_BYTES(evs[i]) - res->rx_prefix_len);
        res->normal_count++;
        break;
      case EF_EVENT_TYPE_RX_DISCARD:
        handle_rx_discard(res, EF_EVENT_RX_DISCARD_RQ_ID(evs[i]),
          EF_EVENT_RX_DISCARD_BYTES(evs[i]) - res->rx_prefix_len,
          EF_EVENT_RX_DISCARD_TYPE(evs[i]));        
        res->discard_count++;
        break;
      default:
        res->multi_count++;
        //LOGE("ERROR: unexpected event type=%d\n", (int) EF_EVENT_TYPE(evs[i]));
        break;
      }
    }
    refill_rx_ring(res);   
  }else{
    // 写入磁盘
    if(dump_instance){
      dump_instance->method->xFlush(dump_instance);
    }
  }
  return n_ev;
}

int wait_resources(struct resources* res)
{
  static struct timeval timeout = {
    0,
    100000
  };
  int ret = ef_eventq_wait(&res->vi, res->dh, ef_eventq_current(&res->vi), &timeout);

  return ret;
}

int free_resources(struct resources* res)
{
  ef_vi_free(&res->vi, res->dh);

  ef_pd_free(&res->pd, res->dh);

  ef_driver_close(res->dh);

  return 0;
}

int bind_cpu( int cpu_id, pthread_t thd_id )
{
	int			cpu = (int)sysconf(_SC_NPROCESSORS_ONLN);
	cpu_set_t	cpu_info;

	if( cpu < cpu_id )
	{
		return -1;
	}

	CPU_ZERO(&cpu_info);
	CPU_SET(cpu_id,&cpu_info);

	if( pthread_setaffinity_np( thd_id, sizeof(cpu_set_t), &cpu_info ) != 0 )
	{
		return -1;
	}

	return 0;
}

int sl_parse(ef_filter_spec* fs, const char* ip, unsigned short port)
{
  ef_filter_spec_init(fs, EF_FILTER_FLAG_NONE);

  sockaddr_in host_addr;

  bzero(&host_addr, sizeof(host_addr));
  host_addr.sin_family = AF_INET; 

  inet_aton(ip,&(host_addr.sin_addr));
  host_addr.sin_port = htons(port);

  (ef_filter_spec_set_ip4_local(fs, IPPROTO_UDP, host_addr.sin_addr.s_addr, host_addr.sin_port));

  return 0;
}

struct pkt_buf* pkt_buf_from_id(struct resources* res, int pkt_buf_i)
{
  assert((unsigned) pkt_buf_i < (unsigned) res->pkt_bufs_n);
  return (pkt_buf*) ((char*) res->pkt_bufs + (size_t) pkt_buf_i * PKT_BUF_SIZE);
}

void pkt_buf_free(struct resources* res, struct pkt_buf* pkt_buf)
{
  pkt_buf->next = res->free_pkt_bufs;
  res->free_pkt_bufs = pkt_buf;
  ++(res->free_pkt_bufs_n);
}

void handle_rx(struct resources* res, int pkt_buf_i, int len)
{
	struct pkt_buf* pkt_buf = pkt_buf_from_id(res, pkt_buf_i);
	const char* ptr			=  (const char*)(pkt_buf->rx_ptr);

	/***************网络处理**********************/
	/// 网络包头
	ip_head* p_ip			= (ip_head*)(ptr+sizeof(mac_head));
	/// ip头长度
	int ip_head_total_len	= p_ip->m_version.m_head_len * 4;
	udp_head* p_udp			= (udp_head*)(ptr + sizeof(mac_head) + ip_head_total_len );
	/// udp数据长度
	int udp_len				= ntohs(p_udp->m_udp_len) - sizeof(udp_head);
	int net_header_len		= ip_head_total_len + (int)sizeof(mac_head) + (int)sizeof(udp_head);

	/// 数据处理部分
	const char*	ptr_udp		= ptr + net_header_len;
	int	remain_len			= udp_len;
  int read_len;

  // 写入磁盘
  trader_ipdata_dump* dump_instance = (trader_ipdata_dump*)res->dump_instance;
  if(dump_instance){
    dump_instance->method->xAdd(dump_instance, ptr_udp);
  }
  
  if(res->read){
  	for (; remain_len > 0;)
  	{
      read_len = res->read(res->arg, ptr_udp, remain_len);
      remain_len -= read_len;
      ptr_udp += read_len;
    }
	}
	/// 数据处理完成
	/***************网络处理完成**********************/

	pkt_buf_free(res, pkt_buf);
}


void handle_rx_discard(struct resources* res, int pkt_buf_i, int len, int discard_type)
{
  struct pkt_buf* pkt_buf;

  if( accept_discard_pkts ) 
  {
    handle_rx(res, pkt_buf_i, len);
  }
  else 
  {
    pkt_buf = pkt_buf_from_id(res, pkt_buf_i);
    pkt_buf_free(res, pkt_buf);
  }
}

void refill_rx_ring(struct resources* res)
{
  struct pkt_buf* pkt_buf;
  int i;

  if( ef_vi_receive_space(&res->vi) < REFILL_BATCH_SIZE || res->free_pkt_bufs_n < REFILL_BATCH_SIZE )
    return;

  for( i = 0; i < REFILL_BATCH_SIZE; ++i ) 
  {
    pkt_buf = res->free_pkt_bufs;
    res->free_pkt_bufs = res->free_pkt_bufs->next;
    --(res->free_pkt_bufs_n);
    ef_vi_receive_init(&res->vi, pkt_buf->m_ef_addr + RX_DMA_OFF, pkt_buf->id);
  }
  ef_vi_receive_push(&res->vi);
}


