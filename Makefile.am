AUTOMAKE_OPTIONS=foreign

noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a

noinst_PROGRAMS=libctpmduser.so libctptrader.so libxeletrader.so \
				libfemasmduser.so libfemastrader.so libfemasmduser_udp.so libgfmduser.so libxelegdmduser.so\
				libfemasmduserAF.so libfemastraderAF.so \
				libctpminimduser.so libctpminitrader.so \
				libxspeedmduser.so libxspeedtrader.so

bin_PROGRAMS=\
			traderMduserCtp traderStrategyCtp traderStrategyXele traderMduserXeleGD\
			traderMduserCtpMini traderStrategyCtpMini \
			traderMduserFemas traderStrategyFemas \
			traderMduserFemasAF traderStrategyFemasAF traderMduserFemasUDP\
			traderMduserXSpeedSOP traderStrategyXSpeedSOP \
			traderMduserXSpeedStock \
			testMduserShm testMduserClient \
			femasPasswd

# static library
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

# libctpmduser.so libctptrader.so
libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader_so_LDFLAGS=-shared -fPIC

# libctpminimduser.so libctpminitrader.so
libctpminimduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpminimduser_so_CPPFLAGS=-fPIC -Ictpmini -Ilibevent -Ilibevent/compat/sys
libctpminimduser_so_LDFLAGS=-shared -fPIC

libctpminitrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctpminitrader_so_CPPFLAGS=-fPIC -DCTPMINI -Ictpmini -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctpminitrader_so_LDFLAGS=-shared -fPIC

# libfemasmduser.so libfemastrader.so libfemasmduser_udp.so
libfemasmduser_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduser_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys
libfemasmduser_so_LDFLAGS=-shared -fPIC

libfemasmduser_udp_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduser_udp_so_CPPFLAGS=-DFEMAS_UDP -DFEMAS20 -fPIC -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libfemasmduser_udp_so_LDFLAGS=-shared -fPIC

libgfmduser_so_SOURCES=src/api/trader_mduser_api_gf.cpp
libgfmduser_so_CPPFLAGS=-fPIC -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libgfmduser_so_LDFLAGS=-shared -fPIC

libxelegdmduser_so_SOURCES=src/api/trader_mduser_api_gd.cpp
libxelegdmduser_so_CPPFLAGS=-fPIC -Ilibevent -Ilibevent/compat/sys
libxelegdmduser_so_LDFLAGS=-shared -fPIC

libfemastrader_so_SOURCES=src/api/trader_trader_api_femas.cpp\
	src/api/FemasTraderHandler.cpp
libfemastrader_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastrader_so_LDFLAGS=-shared -fPIC

# libfemasmduserAF.so libfemastraderAF.so
libfemasmduserAF_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduserAF_so_CPPFLAGS=-fPIC -DFEMAS20 -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libfemasmduserAF_so_LDFLAGS=-shared -fPIC

libfemastraderAF_so_SOURCES=src/api/trader_trader_api_femas.cpp\
	src/api/FemasTraderHandler.cpp
libfemastraderAF_so_CPPFLAGS=-fPIC -DFEMAS20 -Ifemas20 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastraderAF_so_LDFLAGS=-shared -fPIC

# libxspeedmduser.so libxspeedtrader.so
libxspeedmduser_so_SOURCES=src/api/trader_mduser_api_xspeed.cpp\
	src/api/XSpeedMduserHandler.cpp
libxspeedmduser_so_CPPFLAGS=-fPIC -Ixspeed -Ilibevent -Ilibevent/compat/sys
libxspeedmduser_so_LDFLAGS=-shared -fPIC

libxspeedtrader_so_SOURCES=src/api/trader_trader_api_xspeed.cpp\
	src/api/XSpeedTraderHandler.cpp
libxspeedtrader_so_CPPFLAGS=-fPIC -Ixspeed -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libxspeedtrader_so_LDFLAGS=-shared -fPIC

libxeletrader_so_SOURCES=src/api/trader_trader_api_xele.cpp\
	src/api/XeleTraderHandler.cpp
libxeletrader_so_CPPFLAGS=-fPIC -Ixele -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libxeletrader_so_LDFLAGS=-shared -fPIC

# exec
# traderMduserCtp traderStrategyCtp
traderMduserCtp_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtp_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtp_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
        -L. -ltradercmn -lmduserapi\
        -Lctp -lctpmduser -lthostmduserapi_se

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
        -Lctp -lctptrader -lthosttraderapi_se

# traderMduserCtpMini traderStrategyCtpMini
traderMduserCtpMini_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtpMini_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtpMini_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
        -L. -ltradercmn -lmduserapi\
        -Lctpmini -lctpminimduser -lthostmduserapi

traderStrategyCtpMini_SOURCES=\
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

traderStrategyCtpMini_CFLAGS=-DCTP -Isrc/api \
        -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyCtpMini_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
        -L. -ltradercmn -ltraderapi\
        -Lctpmini -lctpminitrader -lthosttraderapi

# traderMduserFemas traderStrategyFemas 
# traderMduserFemasAF traderStrategyFemasAF
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

traderMduserFemasUDP_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserFemasUDP_CFLAGS=-DGF_CFFEX\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemasUDP_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -lgfmduser -ltradercmn -lmduserapi \
	-Lctp -lctpmduser -lthostmduserapi_se

traderMduserXeleGD_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXeleGD_CFLAGS=-DGD_CFFEX\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXeleGD_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -lxelegdmduser -ltradercmn -lmduserapi

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

# traderMduserXSpeedStock
traderMduserXSpeedStock_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXSpeedStock_CFLAGS=-DXSPEED_STOCK\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXSpeedStock_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lxspeed -lxspeedmduser -lDFITCSECMdApi

# traderMduserXSpeedSOP traderStrategyXSpeedSOP
traderMduserXSpeedSOP_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXSpeedSOP_CFLAGS=-DXSPEED\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXSpeedSOP_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core \
	-L. -ltradercmn -lmduserapi\
	-Lxspeed -lxspeedmduser -lDFITCSECMdApi

traderStrategyXSpeedSOP_SOURCES=\
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

traderStrategyXSpeedSOP_CFLAGS=-DXSPEED -Isrc/api \
        -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyXSpeedSOP_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
        -L. -ltradercmn -ltraderapi\
        -Lxspeed -lxspeedtrader -lDFITCSECTraderApi

traderStrategyXele_SOURCES=\
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

traderStrategyXele_CFLAGS=-DXELE -Isrc/api \
        -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyXele_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
        -L. -ltradercmn -ltraderapi\
        -Lxele -lxeletrader -lXeleTdAPI64 -lXeleTraderOrderApi

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

testMduserShm_SOURCES=\
        src/test/trader_mduser_shm_test.c \
        src/svc/trader_mduser_shm.c

testMduserShm_CFLAGS=-Isrc/api -Isrc/svc

testMduserShm_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl

femasPasswd_SOURCES=\
	src/test/FemasTraderChangePassword.cpp

femasPasswd_CPPFLAGS=-DFEMAS -Ifemas -I.

femasPasswd_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Lfemas -lUSTPtraderapi

