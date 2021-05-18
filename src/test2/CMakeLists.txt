cmake_minimum_required (VERSION 3.10)

project(TraderTest)

add_compile_options(-g -Wall -Werror)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(ctp)

add_subdirectory(ctpsop)

add_subdirectory(ctpmini)

add_subdirectory(femas20)

add_subdirectory(rem)

add_subdirectory(femas)

add_subdirectory(femas304)

add_subdirectory(femas302)

