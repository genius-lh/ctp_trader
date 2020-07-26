cmake_minimum_required (VERSION 2.8)
project(XeleTraderOrderApi)

set(BASE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/xele)

add_compile_options(-g -fPIC -Wall -std=c++11)

add_library(XeleTraderOrderApi
	SHARED
	CFairMsgRecv.cpp
	common.cpp
	fair_message.cpp
	XeleTraderOrderApiImp.cpp)

