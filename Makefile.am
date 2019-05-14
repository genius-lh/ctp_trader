AUTOMAKE_OPTIONS=foreign
noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a 
noinst_PROGRAMS=libctpmduser.so libctptrader.so \
				libfemasmduser.so libfemastrader.so \
				libfemasmduserAF.so libfemastraderAF.so \
				libxspeedmduser.so libdfitcmduser.so
bin_PROGRAMS=testMduserClient traderMduserCtp traderStrategyCtp \
			traderMduserFemas traderStrategyFemas \
			traderMduserFemasAF traderStrategyFemasAF \
			traderMduserXSpeedStock traderMduserXSpeedSOP \
			traderMduserDfitcStock traderMduserDfitcSOP

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
	
libtraderapi_a_SOURCES=src/api/trader_trader_api.c
libtraderapi_a_CFLAGS=-Ilibevent -Ilibevent/compat/sys \
	-Isrc/util

libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader_so_LDFLAGS=-shared -fPIC

traderMduserCtp_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtp_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtp_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
        -L. -ltradercmn -lmduserapi\
        -Lctp -lctpmduser -lthostmduserapi

traderStrategyCtp_SOURCES=\
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

traderStrategyCtp_CFLAGS=-DCTP -Isrc/api \
        -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyCtp_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
        -L. -ltradercmn -ltraderapi\
        -Lctp -lctptrader -lthosttraderapi

testMduserClient_SOURCES=\
        src/test/trader_mduser_client_test.c\
        src/trade/trader_mduser_client.c

testMduserClient_CFLAGS=-Isrc/trade -Isrc/util -Isrc/log -Isrc/cfg\
        -Isrc/api -Isrc/db\
        -Ilibevent -Ilibevent/compat/sys -Isqlite3

testMduserClient_LDADD=\
        -Llibevent -levent_core -Lsqlite3 -lsqlite3\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -L. -ltradercmn

libfemasmduser_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduser_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys
libfemasmduser_so_LDFLAGS=-shared -fPIC

libfemastrader_so_SOURCES=src/api/trader_trader_api_femas.cpp\
	src/api/FemasTraderHandler.cpp
libfemastrader_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastrader_so_LDFLAGS=-shared -fPIC

libfemasmduserAF_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduserAF_so_CPPFLAGS=-fPIC -DFEMAS20 -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libfemasmduserAF_so_LDFLAGS=-shared -fPIC

libfemastraderAF_so_SOURCES=src/api/trader_trader_api_femas.cpp\
	src/api/FemasTraderHandler.cpp
libfemastraderAF_so_CPPFLAGS=-fPIC -DFEMAS20 -Ifemas20 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastraderAF_so_LDFLAGS=-shared -fPIC

libxspeedmduser_so_SOURCES=src/api/trader_mduser_api_xspeed.cpp\
	src/api/DFITCSECMduserHandler.cpp
libxspeedmduser_so_CPPFLAGS=-fPIC -Ixspeed -Ilibevent -Ilibevent/compat/sys
libxspeedmduser_so_LDFLAGS=-shared -fPIC

traderMduserXSpeedStock_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXSpeedStock_CFLAGS=-DXSPEED_STOCK\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXSpeedStock_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lxspeed -lxspeedmduser -lDFITCSECMdApi
	
traderMduserXSpeedSOP_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXSpeedSOP_CFLAGS=-DXSPEED\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXSpeedSOP_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lxspeed -lxspeedmduser -lDFITCSECMdApi

traderMduserFemas_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserFemas_CFLAGS=-DFEMAS\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemas_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lfemas -lfemasmduser -lUSTPmduserapi

traderStrategyFemas_SOURCES=\
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

traderStrategyFemas_CFLAGS=-DFEMAS -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyFemas_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lfemas -lfemastrader -lUSTPtraderapi

traderMduserFemasAF_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserFemasAF_CFLAGS=-DFEMAS\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemasAF_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lfemas20 -lfemasmduserAF -lUSTPmduserapiAF

traderStrategyFemasAF_SOURCES=\
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

traderStrategyFemasAF_CFLAGS=-DFEMAS -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyFemasAF_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lfemas20 -lfemastraderAF -lUSTPtraderapiAF

libdfitcmduser_so_SOURCES=src/api/trader_mduser_api_xspeed.cpp\
	src/api/DFITCSECMduserHandler.cpp
libdfitcmduser_so_CPPFLAGS=-fPIC -DDFITC55 -Idfitc55 -Ilibevent -Ilibevent/compat/sys
libdfitcmduser_so_LDFLAGS=-shared -fPIC

traderMduserDfitcStock_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserDfitcStock_CFLAGS=-DXSPEED_STOCK\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserDfitcStock_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Ldfitc55 -ldfitcmduser -lDFITCSECMdApi
	
traderMduserDfitcSOP_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserDfitcSOP_CFLAGS=-DXSPEED\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserDfitcSOP_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Ldfitc55 -ldfitcmduser -lDFITCSECMdApi

