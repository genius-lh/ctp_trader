
set(LOGCLT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log_util.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log.c
)

add_library(traderlogclt STATIC ${LOGCLT_SOURCES})

target_include_directories(traderlogclt PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)
