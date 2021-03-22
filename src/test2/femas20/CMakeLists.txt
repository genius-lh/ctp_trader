
include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/femas20 )

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/femas20)

add_executable(testFemas20Td Femas20Test.cpp)

target_link_libraries(testFemas20Td USTPtraderapiAF)

add_executable(testMap MapTest.cpp)



