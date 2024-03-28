
set(TRADER_CLIENT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_cnn.c
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_svr.c
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_pool.c
)

add_library(traderClientAud STATIC ${TRADER_CLIENT_SOURCES})

target_include_directories(traderClientAud PUBLIC
$ENV{LIBEVENT_HOME}/include
$ENV{HIREDIS_HOME}
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
${CMAKE_CURRENT_SOURCE_DIR}/../log
${CMAKE_CURRENT_SOURCE_DIR}/../trade
${CMAKE_CURRENT_SOURCE_DIR}/../api
)
