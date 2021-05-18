
include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/femas304)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/femas304)

add_executable(testFemas304Td ${CMAKE_CURRENT_SOURCE_DIR}/../femas20/Femas20Test.cpp)

target_link_libraries(testFemas304Td USTPtraderapiAF)




