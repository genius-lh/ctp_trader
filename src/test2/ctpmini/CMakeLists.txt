include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2)

add_executable(testCtpMiniTd CtpTest.cpp)

add_executable(testCtpMiniMd CtpMduserTest.cpp)

target_link_libraries(testCtpMiniTd thosttraderapi)

target_link_libraries(testCtpMiniMd thostmduserapi)
