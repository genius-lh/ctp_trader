
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROFILE_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/glbProfile.c
${CMAKE_CURRENT_SOURCE_DIR}/cmn_cfg.c
)

add_library(tradercfg STATIC ${PROFILE_SOURCES})


