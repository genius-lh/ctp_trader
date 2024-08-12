
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

set(MDUSER_PROXY_UDP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyUdp.cpp
)

set(MDUSER_PROXY_DZM2_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyM2.cpp
)


add_library(mduserProxyUtil SHARED ${MDUSER_PROXY_UTILS_SOURCES})
target_include_directories(mduserProxyUtil PUBLIC
$ENV{HIREDIS_HOME}
$ENV{LIBEVENT_HOME}/include
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)
target_link_libraries(mduserProxyUtil traderCmnUtils traderCmnCfg traderCmnLog)

add_executable(mduserProxyCtp ${MDUSER_PROXY_CTP_SOURCES})
target_include_directories(mduserProxyCtp PUBLIC
${EXTERN_API_PATH}/ctp6.6
)
target_link_libraries(mduserProxyCtp mduserProxyUtil pthread rt event hiredis thostmduserapi_se)
target_link_directories(mduserProxyCtp PUBLIC
${EXTERN_API_PATH}/ctp6.6
)

add_executable(mduserProxyEfvi ${MDUSER_PROXY_EFVI_SOURCES})
target_include_directories(mduserProxyEfvi PUBLIC
${PROJECT_SOURCE_DIR}/udp
${PROJECT_SOURCE_DIR}/efvi
)
target_link_libraries(mduserProxyEfvi mduserProxyUtil mduserUdpStruct mduserEfviReceive pthread rt event hiredis)

add_executable(mduserProxyRem ${MDUSER_PROXY_REM_SOURCES})
target_include_directories(mduserProxyRem PUBLIC
${EXTERN_API_PATH}/rem
)
target_link_libraries(mduserProxyRem mduserProxyUtil pthread rt event hiredis EESQuoteApi)
target_link_directories(mduserProxyRem PUBLIC
${EXTERN_API_PATH}/rem
)

add_executable(mduserProxyCtpSop ${MDUSER_PROXY_CTP_SOURCES})
target_compile_definitions(mduserProxyCtpSop PUBLIC CTP_SOPT)
target_include_directories(mduserProxyCtpSop PUBLIC
${EXTERN_API_PATH}/ctpsop
)
target_link_libraries(mduserProxyCtpSop mduserProxyUtil pthread rt event hiredis soptthostmduserapi_se)
target_link_directories(mduserProxyCtpSop PUBLIC
${EXTERN_API_PATH}/ctpsop
)

add_executable(mduserProxyCtpMini ${MDUSER_PROXY_CTP_SOURCES})
target_include_directories(mduserProxyCtpMini PUBLIC
${EXTERN_API_PATH}/ctpmini
)
target_link_libraries(mduserProxyCtpMini mduserProxyUtil pthread rt event hiredis thostmduserapi)
target_link_directories(mduserProxyCtpMini PUBLIC
${EXTERN_API_PATH}/ctpmini
)

add_executable(mduserProxyExanic ${MDUSER_PROXY_EXANIC_SOURCES})
target_include_directories(mduserProxyExanic PUBLIC
${PROJECT_SOURCE_DIR}/udp
${EXTERN_API_PATH}/exanic
)
target_link_libraries(mduserProxyExanic mduserProxyUtil mduserUdpStruct exanic pthread rt event hiredis)
target_link_directories(mduserProxyExanic PUBLIC
${EXTERN_API_PATH}/exanic
)

add_executable(mduserProxyUdp ${MDUSER_PROXY_UDP_SOURCES})
target_include_directories(mduserProxyUdp PUBLIC
${PROJECT_SOURCE_DIR}/udp
)
target_link_libraries(mduserProxyUdp mduserProxyUtil mduserUdpStruct pthread rt event hiredis)

add_executable(mduserProxyDzM2 ${MDUSER_PROXY_DZM2_SOURCES})
target_include_directories(mduserProxyDzM2 PUBLIC
${EXTERN_API_PATH}/dzm2
)
target_link_libraries(mduserProxyDzM2 mduserProxyUtil pthread rt event hiredis M2Api)
target_link_directories(mduserProxyDzM2 PUBLIC
${EXTERN_API_PATH}/dzm2
)

add_executable(mduserProxyExanicDaLian ${CMAKE_CURRENT_SOURCE_DIR}/TraderMduserProxyExanicDaLian.cpp)
target_include_directories(mduserProxyExanicDaLian PUBLIC
${PROJECT_SOURCE_DIR}/udp
${EXTERN_API_PATH}/exanic
)
target_link_libraries(mduserProxyExanicDaLian mduserProxyUtil mduserUdpStruct exanic pthread rt event hiredis)
target_link_directories(mduserProxyExanicDaLian PUBLIC
${EXTERN_API_PATH}/exanic
)


