include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/ctpsop)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctpsop)

add_executable(testCtpSopTd CtpSopTest.cpp)

add_executable(testCtpSopMd CtpSopMduserTest.cpp)

target_link_libraries(testCtpSopTd soptthosttraderapi_se)

target_link_libraries(testCtpSopMd soptthostmduserapi_se)
