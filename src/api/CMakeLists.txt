set(MDUSER_API_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_api.c
)

set(TRADER_API_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_trader_api.c
)

add_library(mduserapi STATIC ${MDUSER_API_SOURCES})
target_include_directories(mduserapi PUBLIC
${PROJECT_SOURCE_DIR}/util
)

add_library(traderapi STATIC ${TRADER_API_SOURCES})
target_include_directories(traderapi PUBLIC
${PROJECT_SOURCE_DIR}/util
)

set(MDUSER_API_CTP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_api_ctp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CtpMduserHandler.cpp
)

set(TRADER_API_CTP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_trader_api_ctp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CtpTraderHandler.cpp
)

add_library(ctpminitrader SHARED ${TRADER_API_CTP_SOURCES})

target_compile_definitions(ctpminitrader PUBLIC CTPMINI)

target_include_directories(ctpminitrader PUBLIC
${EXTERN_API_PATH}/ctpmini
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)

add_library(ctpminimduser SHARED ${MDUSER_API_CTP_SOURCES})

target_compile_definitions(ctpminimduser PUBLIC CTPMINI)

target_include_directories(ctpminimduser PUBLIC
${EXTERN_API_PATH}/ctpmini
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)

add_library(ctpmduser SHARED ${MDUSER_API_CTP_SOURCES})

target_compile_definitions(ctpmduser PUBLIC CTP)

target_include_directories(ctpmduser PUBLIC
${EXTERN_API_PATH}/ctp6.6
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)


set(TRADER_API_REM_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_trader_api_rem.cpp
${CMAKE_CURRENT_SOURCE_DIR}/RemTraderHandler.cpp
)

set(MDUSER_API_REM_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_api_rem.cpp
${CMAKE_CURRENT_SOURCE_DIR}/RemMduserHandler.cpp
)

add_library(remtrader SHARED ${TRADER_API_REM_SOURCES})

target_include_directories(remtrader PUBLIC
${EXTERN_API_PATH}/rem
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)

add_library(remmduser SHARED ${MDUSER_API_REM_SOURCES})

target_include_directories(remmduser PUBLIC
${EXTERN_API_PATH}/rem
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)

add_library(remeftrader SHARED ${TRADER_API_REM_SOURCES})

target_include_directories(remeftrader PUBLIC
${EXTERN_API_PATH}/rem5
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)

