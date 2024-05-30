
add_executable(testRemMd RemMduserTest.cpp)
target_include_directories(testRemMd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem )
target_link_libraries(testRemMd EESQuoteApi)
target_link_directories(testRemMd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem)

add_executable(testRemTd RemTest.cpp)
target_include_directories(testRemTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem )
target_link_libraries(testRemTd EESTraderApi)
target_link_directories(testRemTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem)
