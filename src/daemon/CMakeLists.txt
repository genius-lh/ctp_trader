
set(TRADER_DAEMON_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_daemon.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_svr.c
)

add_executable(traderStrategyCtp ${TRADER_DAEMON_SOURCES})
target_compile_definitions(traderStrategyCtp PUBLIC CTP)
target_include_directories(traderStrategyCtp PUBLIC
)
target_link_libraries(traderStrategyCtp 
traderClientAud
traderStrategy
traderLogSvr
traderCmnUtils
traderCmnCfg
traderCmnLog
traderapi
ctptrader66
pthread
resolv
rt
dl
event
hiredis
sqlite3
thosttraderapi_se
)
target_link_directories(traderStrategyCtp PUBLIC
$ENV{HIREDIS_HOME}
$ENV{SQLITE3_HOME}/lib
${CMAKE_CURRENT_SOURCE_DIR}/../../ctp6.6
)


