AUTOMAKE_OPTIONS=foreign

noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a

noinst_PROGRAMS=libctpmduser.so libctptrader.so \
libIBtraderApiBase.so libibmduser.so libibtrader.so

bin_PROGRAMS=\
traderMduserIB traderStrategyIB \
traderMduserShmHttp testMduserShm

# static library
libtradercmn_a_SOURCES=\
src/svc/trader_mduser_shm.c \
src/util/cmn_util.c \
src/util/cJSON.c \
src/cfg/glbProfile.c \
src/cfg/cmn_cfg.c \
src/log/cmn_log_util.c \
src/log/cmn_log.c
libtradercmn_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
-Isrc/cfg

libmduserapi_a_SOURCES=src/api/trader_mduser_api.c
libmduserapi_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
-Isrc/util -Ihiredis

libtraderapi_a_SOURCES=src/api/trader_trader_api.c
libtraderapi_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
-Isrc/util -Ihiredis

# libctpmduser.so libctptrader.so
libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader_so_LDFLAGS=-shared -fPIC

# libIBtraderApiBase.so
libIBtraderApiBase_so_SOURCES=src/api/IBTraderApiBase.cpp src/api/IBFutureContractFactory.cpp
libIBtraderApiBase_so_CPPFLAGS=-Iib -Ihiredis -Isrc/cfg -Isrc/log -fPIC -std=c++11
libIBtraderApiBase_so_LDFLAGS=-shared -fPIC

libibmduser_so_SOURCES=src/api/trader_mduser_api_ib.cpp\
src/api/IBMduserHandler.cpp\
src/api/IBMduserApi.cpp
libibmduser_so_CPPFLAGS=-fPIC -Iib -Isrc/svc -Ihiredis -Ilibevent -Ilibevent/compat/sys -std=c++11
libibmduser_so_LDFLAGS=-shared -fPIC

libibtrader_so_SOURCES=\
src/api/trader_trader_api_ib.cpp\
src/api/IBTraderHandler.cpp\
src/api/IBTraderApi.cpp
libibtrader_so_CPPFLAGS=-fPIC -Iib -Isrc/util -Isrc/log -Isrc/svc -Ihiredis -Ilibevent -Ilibevent/compat/sys -std=c++11
libibtrader_so_LDFLAGS=-shared -fPIC

# exec
# traderMduserIB
traderMduserIB_SOURCES=src/svc/trader_mduser_boardcast.c \
src/svc/trader_mduser_cnn.c \
src/svc/trader_mduser_svr.c

traderMduserIB_CFLAGS=-DCTP -DIB\
-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserIB_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
-Lib/build -lTwsSocketClient \
-L. -libmduser -lIBtraderApiBase \
-ltradercmn -lmduserapi\
-Lctp -lctpmduser -lthostmduserapi_se

traderStrategyIB_SOURCES=\
src/log/cmn_log_catalog.c\
src/log/cmn_log_appender.c\
src/log/cmn_log_svr.c\
src/daemon/trader_daemon.c \
src/trade/trader_db.c \
src/api/trader_order_mapper.c\
src/trade/trader_mduser_client.c \
src/trade/trader_strategy_engine.c \
src/trade/trader_strategy_imp.c \
src/trade/trader_strategy_limit.c \
src/trade/trader_strategy.c \
src/trade/trader_svr.c \
src/client/client_aud_cnn.c \
src/client/client_aud_svr.c \
src/client/client_aud_pool.c

traderStrategyIB_CFLAGS=-DCTP -DIB -Isrc/api \
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyIB_LDADD=\
-lc -lstdc++ -lm -lpthread -lresolv -ldl\
-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
-Lib/build -lTwsSocketClient\
-L. -ltradercmn -ltraderapi -libtrader -lIBtraderApiBase\
-Lctp -lctptrader -lthosttraderapi_se

testMduserShm_SOURCES=\
src/test/trader_mduser_shm_test.c \
src/svc/trader_mduser_shm.c

testMduserShm_CFLAGS=-Isrc/api -Isrc/svc

testMduserShm_LDADD=\
-lc -lstdc++ -lm -lpthread -lresolv -ldl

traderMduserShmHttp_SOURCES=\
src/svc/trader_mduser_shm_http.c \
src/svc/trader_mduser_shm.c

traderMduserShmHttp_CFLAGS=-Isrc/api -Isrc/svc \
-Isrc/util -Isrc/log \
-Ilibevent -Ilibevent/compat/sys

traderMduserShmHttp_LDADD=\
-L. -ltradercmn -Llibevent -levent \
-lc -lstdc++ -lm -lpthread -lresolv -ldl

