
set(MDUSER_EFVI_RECEIVE_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/efvi_receive_depend.cpp
)

add_library(mduserEfviReceive STATIC ${MDUSER_EFVI_RECEIVE_SOURCES})
target_include_directories(mduserEfviReceive PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../..
${CMAKE_CURRENT_SOURCE_DIR}/../log
${CMAKE_CURRENT_SOURCE_DIR}/../cfg
${CMAKE_CURRENT_SOURCE_DIR}/../util
)
target_link_libraries(mduserEfviReceive ciul1)
target_link_directories(mduserEfviReceive PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../../etherfabric
)


