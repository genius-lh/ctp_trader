cmake_minimum_required (VERSION 2.8)
project(TraderTest)

add_compile_options(-g -Wall)

set(BASE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(. ${BASE_LIBRARY_PATH}/ctpsop ${BASE_LIBRARY_PATH}/femas ${BASE_LIBRARY_PATH}/xspeed ${CMAKE_CURRENT_SOURCE_DIR}/xele)

link_directories(${BASE_LIBRARY_PATH}/ctpsop ${BASE_LIBRARY_PATH}/femas ${BASE_LIBRARY_PATH}/xspeed ${CMAKE_CURRENT_SOURCE_DIR}/xele)

add_executable(testCtpSop CtpSopTest.cpp)

add_executable(testCtpSopMd CtpSopMduserTest.cpp)

add_executable(femasPasswd FemasTraderChangePassword.cpp)

add_executable(testXSpeed XSpeedTest.cpp)

add_executable(testXele XeleTest.cpp)

target_link_libraries(testCtpSop soptthosttraderapi_se)

target_link_libraries(testCtpSopMd soptthostmduserapi_se)

target_link_libraries(femasPasswd USTPtraderapi)

target_link_libraries(testXSpeed DFITCSECTraderApi)

target_link_libraries(testXele XeleTdAPI64-Test XeleTraderOrderApi)

