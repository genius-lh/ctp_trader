
set(TRADER_CLIENT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_cnn.c
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_svr.c
${CMAKE_CURRENT_SOURCE_DIR}/client_aud_pool.c
)

add_library(traderClientAud STATIC ${TRADER_CLIENT_SOURCES})

target_include_directories(traderClientAud PUBLIC
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/trade
${PROJECT_SOURCE_DIR}/api
)
