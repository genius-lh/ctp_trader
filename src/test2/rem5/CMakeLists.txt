
#add_executable(testRem5Td Rem5Test.cpp)
#target_include_directories(testRem5Td PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem)
#target_link_libraries(testRem5Td EESTraderApi_ef)
#target_link_directories(testRem5Td PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem)

add_executable(testRemEfTd ${CMAKE_CURRENT_SOURCE_DIR}/../rem/RemTest.cpp)
target_include_directories(testRemEfTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem5)
target_link_libraries(testRemEfTd EESTraderApi_ef)
target_link_directories(testRemEfTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rem5)
