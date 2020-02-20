cmake_minimum_required (VERSION 2.8)
project(TwsSocketClient)

add_compile_options(-g -fPIC -Wall -std=c++11)

add_library(TwsSocketClient
	SHARED
	ContractCondition.cpp
        DefaultEWrapper.cpp
        EClient.cpp
        EClientSocket.cpp
        EDecoder.cpp
        EMessage.cpp
        EMutex.cpp
        EOrderDecoder.cpp
        EReader.cpp
        EReaderOSSignal.cpp
        ESocket.cpp
        executioncondition.cpp
        MarginCondition.cpp
        OperatorCondition.cpp
        OrderCondition.cpp
        PercentChangeCondition.cpp
        PriceCondition.cpp
        SoftDollarTier.cpp
        StdAfx.cpp
        TimeCondition.cpp
        VolumeCondition.cpp)

