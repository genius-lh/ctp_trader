
set(TRADER_DAEMON_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_daemon.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_svr.c
)

add_executable(traderStrategyCtpMini ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyCtpMini PUBLIC CTP)
target_link_directories(traderStrategyCtpMini PUBLIC
$ENV{SQLITE3_HOME}/lib
${EXTERN_API_PATH}/ctpmini
)
target_link_libraries(traderStrategyCtpMini 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
ctpminitrader
pthread
resolv
rt
dl
event
hiredis
sqlite3
thosttraderapi
)

add_executable(traderStrategyRem ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyRem PUBLIC REM)
target_link_directories(traderStrategyRem PUBLIC
$ENV{SQLITE3_HOME}/lib
${EXTERN_API_PATH}/rem
)
target_link_libraries(traderStrategyRem 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
remtrader
pthread
resolv
rt
dl
event
hiredis
sqlite3
EESTraderApi
)

add_executable(traderStrategyRemEf ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyRemEf PUBLIC REM)
target_link_directories(traderStrategyRemEf PUBLIC
$ENV{SQLITE3_HOME}/lib
${EXTERN_API_PATH}/rem5
)
target_link_libraries(traderStrategyRemEf 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
remeftrader
pthread
resolv
rt
dl
event
hiredis
sqlite3
EESTraderApi_ef
)

add_executable(traderStrategyHxts ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyHxts PUBLIC HXTS)
target_link_directories(traderStrategyHxts PUBLIC
$ENV{SQLITE3_HOME}/lib
${EXTERN_API_PATH}/hxts
)
target_link_libraries(traderStrategyHxts 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
hxtstrader
pthread
resolv
rt
dl
event
hiredis
sqlite3
hxtsapi
onload_zf
ssl
crypto
)

add_executable(traderStrategyYd ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyYd PUBLIC YD)
target_link_directories(traderStrategyYd PUBLIC
$ENV{SQLITE3_HOME}/lib
${EXTERN_API_PATH}/yd
)
target_link_libraries(traderStrategyYd 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
ydtrader
pthread
resolv
rt
dl
event
hiredis
sqlite3
yd
)
