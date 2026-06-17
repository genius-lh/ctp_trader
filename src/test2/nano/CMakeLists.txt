
add_executable(testHSNanoTd ${CMAKE_CURRENT_SOURCE_DIR}/HSNanoTradeTest.cpp)
target_compile_options(testHSNanoTd PUBLIC -std=c++11)
target_include_directories(testHSNanoTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../nano)
target_link_libraries(testHSNanoTd hsnanotradeapi pthread dl)
target_link_directories(testHSNanoTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../nano)

