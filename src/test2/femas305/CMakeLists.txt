
include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/femas305)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/femas305)

add_executable(testFemas305Td ${CMAKE_CURRENT_SOURCE_DIR}/../femas20/Femas20Test.cpp)

add_executable(testFemas305Md ${CMAKE_CURRENT_SOURCE_DIR}/../femas20/FemasMduserTest.cpp)

target_link_libraries(testFemas305Td USTPtraderapiAF)

target_link_libraries(testFemas305Md USTPmduserapiAF)




