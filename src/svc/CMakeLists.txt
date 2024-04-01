
set(MDUSER_SVR_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_boardcast.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_cnn.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_svr.c
)

add_executable(traderMduserCtpMini ${MDUSER_SVR_SOURCES})

target_compile_definitions(traderMduserCtpMini PUBLIC CTPMINI)

target_include_directories(traderMduserCtpMini PUBLIC
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
${PROJECT_SOURCE_DIR}/api
${EXTERN_API_PATH}/ctpmini
)

target_link_directories(traderMduserCtpMini PUBLIC
${EXTERN_API_PATH}/ctpmini
)

target_link_libraries(traderMduserCtpMini
traderCmnUtils
traderCmnLog
traderCmnCfg
mduserapi
ctpminimduser thostmduserapi
event_core hiredis
pthread rt event dl
)

add_executable(traderMduserRem ${MDUSER_SVR_SOURCES})

target_compile_definitions(traderMduserRem PUBLIC REM)

target_include_directories(traderMduserRem PUBLIC
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
${PROJECT_SOURCE_DIR}/api
${EXTERN_API_PATH}/rem
)

target_link_directories(traderMduserRem PUBLIC
${EXTERN_API_PATH}/rem
)

target_link_libraries(traderMduserRem
traderCmnUtils
traderCmnLog
traderCmnCfg
mduserapi
remmduser EESQuoteApi
event_core hiredis
pthread rt event dl
)


