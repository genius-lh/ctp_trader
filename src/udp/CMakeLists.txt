
set(MDUSER_UDP_STRUCT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/cffex_unmask.cpp
${CMAKE_CURRENT_SOURCE_DIR}/trader_mduser_api_ef_vi_imp.cpp
)

add_library(mduserUdpStruct SHARED ${MDUSER_UDP_STRUCT_SOURCES})
target_include_directories(mduserUdpStruct PUBLIC
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
${PROJECT_SOURCE_DIR}/api
)
target_link_libraries(mduserUdpStruct traderCmnUtils traderCmnLog traderCmnLog)

