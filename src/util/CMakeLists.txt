
set(UTILS_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cmn_util.c
${CMAKE_CURRENT_SOURCE_DIR}/dict.c
${CMAKE_CURRENT_SOURCE_DIR}/evmqueue.c
${CMAKE_CURRENT_SOURCE_DIR}/mqueue_client.cpp
${CMAKE_CURRENT_SOURCE_DIR}/trader_tick_dict.c
)

add_library(traderutils STATIC ${UTILS_SOURCES})
target_include_directories(traderutils PUBLIC
$ENV{LIBEVENT_HOME}/include
)

