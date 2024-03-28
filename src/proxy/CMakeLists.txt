
set(MDUSER_PROXY_UTILS_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyUtil.cpp
)

set(MDUSER_PROXY_CTP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyCtp.cpp
)

set(MDUSER_PROXY_REM_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyRem.cpp
)

set(MDUSER_PROXY_EFVI_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyEfvi.cpp
)

set(MDUSER_PROXY_EXANIC_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyExanic.cpp
)

add_library(mduserProxyUtil SHARED ${MDUSER_PROXY_UTILS_SOURCES})
target_include_directories(mduserProxyUtil PUBLIC
$ENV{HIREDIS_HOME}
$ENV{LIBEVENT_HOME}/include
${CMAKE_CURRENT_SOURCE_DIR}/../log
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)
target_link_libraries(mduserProxyUtil traderCmnUtils traderCmnCfg traderCmnLog)

add_executable(mduserProxyCtp ${MDUSER_PROXY_CTP_SOURCES})
target_include_directories(mduserProxyCtp PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctp6.6
)
target_link_libraries(mduserProxyCtp mduserProxyUtil pthread rt event hiredis thostmduserapi_se)
target_link_directories(mduserProxyCtp PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctp6.6
)

add_executable(mduserProxyEfvi ${MDUSER_PROXY_EFVI_SOURCES})
target_include_directories(mduserProxyEfvi PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../udp
${CMAKE_CURRENT_SOURCE_DIR}/../efvi
)
target_link_libraries(mduserProxyEfvi mduserProxyUtil mduserUdpStruct mduserEfviReceive pthread rt event hiredis)

add_executable(mduserProxyRem ${MDUSER_PROXY_REM_SOURCES})
target_include_directories(mduserProxyRem PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../rem
)
target_link_libraries(mduserProxyRem mduserProxyUtil pthread rt event hiredis EESQuoteApi)
target_link_directories(mduserProxyRem PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../rem
)

add_executable(mduserProxyCtpSop ${MDUSER_PROXY_CTP_SOURCES})
target_compile_definitions(mduserProxyCtpSop PUBLIC CTP_SOPT)
target_include_directories(mduserProxyCtpSop PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctpsop
)
target_link_libraries(mduserProxyCtpSop mduserProxyUtil pthread rt event hiredis soptthostmduserapi_se)
target_link_directories(mduserProxyCtpSop PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctpsop
)

add_executable(mduserProxyCtpMini ${MDUSER_PROXY_CTP_SOURCES})
target_include_directories(mduserProxyCtpMini PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctpmini
)
target_link_libraries(mduserProxyCtpMini mduserProxyUtil pthread rt event hiredis thostmduserapi)
target_link_directories(mduserProxyCtpMini PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../ctpmini
)

add_executable(mduserProxyExanic ${MDUSER_PROXY_EXANIC_SOURCES})
target_include_directories(mduserProxyExanic PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../udp
${CMAKE_CURRENT_SOURCE_DIR}/../../exanic
)
target_link_libraries(mduserProxyExanic mduserProxyUtil mduserUdpStruct exanic pthread rt event hiredis)
target_link_directories(mduserProxyExanic PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../exanic
)


