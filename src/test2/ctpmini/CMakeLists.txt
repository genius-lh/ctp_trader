include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2)

add_executable(testCtpMiniTd ${CMAKE_CURRENT_SOURCE_DIR}/../ctp/CtpTest.cpp)

add_executable(testCtpMiniMd ${CMAKE_CURRENT_SOURCE_DIR}/../ctp/CtpMduserTest.cpp)

target_link_libraries(testCtpMiniTd thosttraderapi)

target_link_libraries(testCtpMiniMd thostmduserapi)
