
set(CMN_CFG_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/glbProfile.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_cfg.c
)

add_library(traderCmnCfg STATIC ${CMN_CFG_SOURCES})


