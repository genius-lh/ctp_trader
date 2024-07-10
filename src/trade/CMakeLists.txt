
set(TRADER_STRATEGY_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/trader_db.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_client.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy_engine.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy_imp.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy_limit.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy_position_svr.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy_processor.c
${CMAKE_CURRENT_SOURCE_DIR}/trader_strategy.c
)

add_library(traderStrategy STATIC ${TRADER_STRATEGY_SOURCES})

target_include_directories(traderStrategy PUBLIC
$ENV{SQLITE3_HOME}/include
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/api
)
