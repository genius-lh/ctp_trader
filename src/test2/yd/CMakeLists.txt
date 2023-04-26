include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/../../../yd )

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../yd)

add_executable(testYdTd YdTest.cpp)

target_link_libraries(testYdTd yd)
