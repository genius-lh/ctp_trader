#ifndef _TRADER_MDUSER_API_EF_VI_H_
#define _TRADER_MDUSER_API_EF_VI_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct trader_mduser_api_ef_vi_ops_def trader_mduser_api_ef_vi_ops;
struct trader_mduser_api_ef_vi_ops_def{
  int m_md_size;
  int m_md_id_pos;
  int (*md_fill)(void* tick, void* obj);
};

extern void trader_mduser_api_ef_vi_ops_init(trader_mduser_api_ef_vi_ops* ops, int type);

#ifdef __cplusplus
}
#endif

#endif //_TRADER_MDUSER_API_EF_VI_H_

