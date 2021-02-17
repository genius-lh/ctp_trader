include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/rem )

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/rem)

add_executable(testRemMd RemMduserTest.cpp)

target_link_libraries(testRemMd EESQuoteApi)

add_executable(testRemTd RemTest.cpp)

target_link_libraries(testRemTd EESTraderApi)
