set(BASE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(. ${BASE_LIBRARY_PATH}/ctp)

link_directories(${BASE_LIBRARY_PATH}/ctp)

add_executable(testCtpTd CtpTest.cpp)

add_executable(testCtpMd CtpMduserTest.cpp)

target_link_libraries(testCtpTd thosttraderapi_se)

target_link_libraries(testCtpMd thostmduserapi_se)
