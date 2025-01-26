add_executable(testYdTd YdTest.cpp)
target_include_directories(testYdTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../yd)
target_link_libraries(testYdTd yd)
target_link_directories(testYdTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../yd)

add_executable(testYd486Td YdTest.cpp)
target_include_directories(testYd486Td PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../yd486)
target_link_libraries(testYd486Td yd)
target_link_directories(testYd486Td PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../yd486)