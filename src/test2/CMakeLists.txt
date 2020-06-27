cmake_minimum_required (VERSION 2.8)
project(TraderTest)

add_compile_options(-g -Wall)

set(BASE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(. ${BASE_LIBRARY_PATH}/ctpmini)

link_directories(${BASE_LIBRARY_PATH}/ctpmini)

add_executable(testCtpMiniTd CtpTest.cpp)

add_executable(testCtpMiniMd CtpMduserTest.cpp)

target_link_libraries(testCtpMiniTd thosttraderapi)

target_link_libraries(testCtpMiniMd thostmduserapi)


