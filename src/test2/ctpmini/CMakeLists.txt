include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2/traderapi ${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2/mdapi)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2/traderapi ${CMAKE_CURRENT_SOURCE_DIR}/ctpmini2/mdapi)

add_executable(testCtpMiniTd ${CMAKE_CURRENT_SOURCE_DIR}/CtpMiniTest.cpp)

add_executable(testCtpMiniMd ${CMAKE_CURRENT_SOURCE_DIR}/../ctp/CtpMduserTest.cpp)

target_link_libraries(testCtpMiniTd thosttraderapi)

target_link_libraries(testCtpMiniMd thostmduserapi)
