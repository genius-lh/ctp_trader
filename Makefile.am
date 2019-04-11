AUTOMAKE_OPTIONS=foreign
noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a 
noinst_PROGRAMS=libctpmduser.so libctptrader.so
bin_PROGRAMS=traderMduserSvr traderStrategy testMduser

libtradercmn_a_SOURCES=src/util/cmn_util.c \
	src/util/cJSON.c \
	src/cfg/glbProfile.c \
	src/cfg/cmn_cfg.c \
	src/log/cmn_log_util.c \
	src/log/cmn_log.c

libtradercmn_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
	-Isrc/cfg

libmduserapi_a_SOURCES=src/api/trader_mduser_api.c
libmduserapi_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
	-Isrc/util

libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
	src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libtraderapi_a_SOURCES=src/api/trader_trader_api.c
libtraderapi_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
	-Isrc/util

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
	src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys -Isrc/util
libctptrader_so_LDFLAGS=-shared -fPIC

traderMduserSvr_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserSvr_CFLAGS=-DCTP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserSvr_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lctp -lctpmduser -lthostmduserapi

traderStrategy_SOURCES=\
	src/log/cmn_log_catalog.c\
	src/log/cmn_log_appender.c\
	src/log/cmn_log_svr.c\
	src/daemon/trader_daemon.c \
	src/trade/trader_db.c \
	src/trade/trader_mduser_client.c \
	src/trade/trader_strategy_engine.c \
	src/trade/trader_strategy_imp.c \
	src/trade/trader_strategy_limit.c \
	src/trade/trader_strategy.c \
	src/trade/trader_svr.c \
	src/client/client_aud_cnn.c \
	src/client/client_aud_svr.c \
	src/client/client_aud_pool.c

traderStrategy_CFLAGS=-DCTP -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategy_CPPFLAGS=-Ictp -Ilibevent -Ilibevent/compat/sys\
	-Isrc/util -Isrc/trade -Isrc/cfg -Isrc/api -Isrc/log

traderStrategy_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lctp -lctptrader -lthosttraderapi

testMduser_SOURCES=\
	src/test/trader_mduser_client_test.c\
	src/trade/trader_mduser_client.c

testMduser_CFLAGS=-Isrc/trade -Isrc/util -Isrc/log -Isrc/cfg\
	-Isrc/api -Isrc/db\
	-Ilibevent -Ilibevent/compat/sys -Isqlite3

testMduser_LDADD=\
        -Llibevent -levent_core -Lsqlite3 -lsqlite3\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -L. -ltradercmn

