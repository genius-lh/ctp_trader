cmake_minimum_required (VERSION 2.8)
project(IBTrader)

add_compile_options(-g -fPIC -Wall -std=c++11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ib)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ib/build)

add_library(IBTraderApiBase
        STATIC
		IBFutureContractFactory.cpp
		IBTraderApiBase.cpp)

add_library(IBMduserApi
        SHARED
		IBMduserApi.cpp)
		
target_link_libraries (IBMduserApi IBTraderApiBase)

add_library(IBTraderApi
        SHARED
		IBTraderApi.cpp)

target_link_libraries (IBTraderApi IBTraderApiBase)
