cmake_minimum_required (VERSION 3.10)

project(Rudistor)

set(CMAKE_C_FLAGS_DEBUG "-ggdb -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "-g -O2 -DNDEBUG")

set(CMAKE_C_FLAGS "-Wall -fPIC -DPIC -D__LINUX__ -D_GNU_SOURCE")

set(CMAKE_CXX_FLAGS "-Wall -fpermissive -fPIC -DPIC -D__LINUX__ -D_GNU_SOURCE")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

if(NOT DEFINED ENV{LIBEVENT_HOME})
message(SEND_ERROR "not set ENV LIBEVENT_HOME")
endif()

if(NOT DEFINED ENV{HIREDIS_HOME})
message(SEND_ERROR "not set ENV HIREDIS_HOME")
endif()

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE DEBUG)
endif()

link_directories(
$ENV{HIREDIS_HOME}
$ENV{LIBEVENT_HOME}/lib
${CMAKE_CURRENT_BINARY_DIR}/lib
)

add_subdirectory(util)

add_subdirectory(cfg)

add_subdirectory(log)

add_subdirectory(udp)

add_subdirectory(efvi)

add_subdirectory(proxy)
