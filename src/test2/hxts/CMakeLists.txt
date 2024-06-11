
add_executable(testHxtsTd HxtsTest.cpp comm_imp.cpp)
target_include_directories(testHxtsTd PUBLIC . ${CMAKE_CURRENT_SOURCE_DIR}/hxts )

target_link_libraries(testHxtsTd hxtsapi ssl crypto rt dl pthread)
target_link_directories(testHxtsTd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/hxts)
