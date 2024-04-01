
set(MDUSER_EFVI_RECEIVE_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/efvi_receive_depend.cpp
)

add_library(mduserEfviReceive STATIC ${MDUSER_EFVI_RECEIVE_SOURCES})
target_include_directories(mduserEfviReceive PUBLIC
${EXTERN_API_PATH}
${PROJECT_SOURCE_DIR}/log
${PROJECT_SOURCE_DIR}/cfg
${PROJECT_SOURCE_DIR}/util
)
target_link_directories(mduserEfviReceive PUBLIC
${EXTERN_API_PATH}/etherfabric
)
target_link_libraries(mduserEfviReceive ciul1)

