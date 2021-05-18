
include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/femas302)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/femas302)

add_executable(testFemas302Td ${CMAKE_CURRENT_SOURCE_DIR}/../femas20/Femas20Test.cpp)

target_link_libraries(testFemas302Td USTPtraderapiAF)




