
include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/femas)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/femas)

add_definitions(-DFEMAS)

add_executable(testFemasTd ${CMAKE_CURRENT_SOURCE_DIR}/../femas20/Femas20Test.cpp)

target_link_libraries(testFemasTd USTPtraderapi)


