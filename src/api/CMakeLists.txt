set(MDUSER_API_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_api.c
)

set(TRADER_API_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_trader_api.c
)

add_library(mduserapi STATIC ${MDUSER_API_SOURCES})
target_include_directories(mduserapi PUBLIC
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../util
)

add_library(traderapi STATIC ${TRADER_API_SOURCES})
target_include_directories(traderapi PUBLIC
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../util
)


set(TRADER_API_CTP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_trader_api_ctp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CtpTraderHandler.cpp
)

add_library(ctptrader66 SHARED ${TRADER_API_CTP_SOURCES})

target_include_directories(ctptrader66 PUBLIC
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../../ctp6.6
${CMAKE_CURRENT_SOURCE_DIR}/../log
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)

