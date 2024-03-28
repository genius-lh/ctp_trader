
set(CMN_LOG_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log_util.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log.c
)

add_library(traderCmnLog STATIC ${CMN_LOG_SOURCES})

target_include_directories(traderCmnLog PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)

set(LOG_SVR_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log_catalog.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log_appender.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_log_svr.c
)

add_library(traderLogSvr STATIC ${LOG_SVR_SOURCES})

target_include_directories(traderLogSvr PUBLIC
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)
