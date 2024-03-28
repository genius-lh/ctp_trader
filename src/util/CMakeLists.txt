
set(CMN_UTILS_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cJSON.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_util.c
${CMAKE_CURRENT_SOURCE_DIR}/dict.c
${CMAKE_CURRENT_SOURCE_DIR}/evmqueue.c
${CMAKE_CURRENT_SOURCE_DIR}/mqueue_client.cpp
${CMAKE_CURRENT_SOURCE_DIR}/trader_tick_dict.c
)

add_library(traderCmnUtils STATIC ${CMN_UTILS_SOURCES})
target_include_directories(traderCmnUtils PUBLIC
$ENV{LIBEVENT_HOME}/include
)

