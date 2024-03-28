
set(MDUSER_PROXY_UTILS_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyUtil.cpp
)

set(MDUSER_PROXY_CTP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyCtp.cpp
)

add_library(mduserproxyutil SHARED ${MDUSER_PROXY_UTILS_SOURCES})
target_include_directories(mduserproxyutil PUBLIC
$ENV{HIREDIS_HOME}
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../log
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)
target_link_libraries(mduserproxyutil traderutils tradercfg traderlogclt hiredis)

add_executable(mduserProxyCtp ${MDUSER_PROXY_CTP_SOURCES})
target_include_directories(mduserproxyutil PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../ctp2
)
target_link_libraries(mduserProxyCtp mduserproxyutil pthread rt event)


