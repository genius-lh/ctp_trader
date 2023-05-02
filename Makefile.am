AUTOMAKE_OPTIONS=foreign

noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a libtraderutils.a

noinst_PROGRAMS=libctpmduser.so libctptrader.so libctpmduser66.so libctptrader66.so libctpminimduser.so libctpminitrader.so \
				libfemasmduser.so libfemastrader.so libfemasmduser_af.so libfemastrader_af.so \
				libefvimduser.so \
				libydtrader.so \
				libremmduser.so libremtrader.so

bin_PROGRAMS=\
			traderMduserCtp traderStrategyCtp traderMduserCtp66 traderStrategyCtp66\
			traderMduserCtpMini traderStrategyCtpMini \
			traderMduserFemas traderStrategyFemas \
			traderMduserFemasAF traderStrategyFemasAF \
			traderMduserRem traderStrategyRem \
			traderStrategyYd \
			traderMduserEfviMini \
			traderMduserEfviCtp66 \
			traderMduserEfviRem \
			testMduserClient

# static library
libtradercmn_a_SOURCES=src/util/cmn_util.c \
	src/util/cJSON.c \
	src/util/dict.c \
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

libtraderutils_a_SOURCES=src/util/trader_tick_dict.c \
						src/util/trader_ipdata_dump.c 
libtraderutils_a_CFLAGS=-Isrc/util	

# libctpmduser.so libctptrader.so
libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp6.3 -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp6.3 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader_so_LDFLAGS=-shared -fPIC

libctpmduser66_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser66_so_CPPFLAGS=-fPIC -Ictp6.6 -Ilibevent -Ilibevent/compat/sys
libctpmduser66_so_LDFLAGS=-shared -fPIC

libctptrader66_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctptrader66_so_CPPFLAGS=-fPIC -Ictp6.6 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader66_so_LDFLAGS=-shared -fPIC

# libctpminimduser.so libctpminitrader.so
libctpminimduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpminimduser_so_CPPFLAGS=-fPIC -Ictpmini -Ilibevent -Ilibevent/compat/sys
libctpminimduser_so_LDFLAGS=-shared -fPIC

libctpminitrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctpminitrader_so_CPPFLAGS=-fPIC -DCTPMINI -Ictpmini -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctpminitrader_so_LDFLAGS=-shared -fPIC

# libfemasmduser.so libfemastrader.so
libfemasmduser_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
	src/api/FemasMduserHandler.cpp
libfemasmduser_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys
libfemasmduser_so_LDFLAGS=-shared -fPIC

libfemastrader_so_SOURCES=src/api/trader_trader_api_femas.cpp\
	src/api/FemasTraderHandler.cpp
libfemastrader_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastrader_so_LDFLAGS=-shared -fPIC

# libfemasmduser_af.so libfemastrader_af.so
libfemasmduser_af_so_SOURCES=src/api/trader_mduser_api_femas_af.cpp\
	src/api/FemasAFMduserHandler.cpp
libfemasmduser_af_so_CPPFLAGS=-fPIC -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libfemasmduser_af_so_LDFLAGS=-shared -fPIC

libfemastrader_af_so_SOURCES=src/api/trader_trader_api_femas_af.cpp\
	src/api/FemasAFTraderHandler.cpp
libfemastrader_af_so_CPPFLAGS=-fPIC -Ifemas20 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastrader_af_so_LDFLAGS=-shared -fPIC

#libremmduser.so
libremmduser_so_SOURCES=src/api/trader_mduser_api_rem.cpp\
	src/api/RemMduserHandler.cpp
libremmduser_so_CPPFLAGS=-fPIC -Irem -Ilibevent -Ilibevent/compat/sys
libremmduser_so_LDFLAGS=-shared -fPIC

#libremtrader.so
libremtrader_so_SOURCES=src/api/trader_trader_api_rem.cpp\
	src/api/RemTraderHandler.cpp
libremtrader_so_CPPFLAGS=-fPIC -Irem -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libremtrader_so_LDFLAGS=-shared -fPIC

#libydtrader.so
libydtrader_so_SOURCES=src/api/trader_trader_api_yd.cpp\
	src/api/YdTraderHandler.cpp
libydtrader_so_CPPFLAGS=-std=c++11 -fPIC -Iyd -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libydtrader_so_LDFLAGS=-shared -fPIC

#libefvimduser.so
libefvimduser_so_SOURCES=src/api/efvi_receive_depend.cpp \
		src/api/cffex_unmask.cpp \
		src/api/trader_mduser_api_ef_vi_imp.cpp \
		src/api/trader_mduser_api_ef_vi.cpp
libefvimduser_so_CPPFLAGS=-fPIC -I. -Ietherfabric -Isrc/util -Ilibevent -Ilibevent/compat/sys
libefvimduser_so_LDFLAGS=-shared -fPIC

# exec
# traderMduserCtp traderStrategyCtp
traderMduserCtp_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtp_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtp_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
        -L. -ltradercmn -lmduserapi\
        -Lctp6.3 -lctpmduser -lthostmduserapi_se

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
        -Lctp6.3 -lctptrader -lthosttraderapi_se

traderMduserCtp66_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtp66_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtp66_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
        -L. -ltradercmn -lmduserapi\
        -Lctp6.6 -lctpmduser66 -lthostmduserapi_se

traderStrategyCtp66_SOURCES=\
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

traderStrategyCtp66_CFLAGS=-DCTP -Isrc/api \
        -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyCtp66_LDADD=\
        -lc -lstdc++ -lm -lpthread -lresolv -ldl\
        -Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
        -L. -ltradercmn -ltraderapi\
        -Lctp6.6 -lctptrader66 -lthosttraderapi_se

#traderMduserCtpMini traderStrategyCtpMini
traderMduserCtpMini_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtpMini_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtpMini_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
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
traderMduserFemas_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserFemas_CFLAGS=-DFEMAS\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemas_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
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
	
# traderMduserFemasAF traderStrategyFemasAF 
traderMduserFemasAF_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserFemasAF_CFLAGS=-DFEMAS_AF\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemasAF_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -ltradercmn -lmduserapi\
	-Lfemas20 -lfemasmduser_af -lUSTPmduserapiAF

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

traderStrategyFemasAF_CFLAGS=-DFEMAS_AF -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyFemasAF_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lfemas20 -lfemastrader_af -lUSTPtraderapiAF
	
#traderMduserRem traderStrategyRem 
traderMduserRem_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserRem_CFLAGS=-DREM\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserRem_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lremmduser -ltradercmn -lmduserapi\
	-Lrem -lremmduser -lEESQuoteApi

traderStrategyRem_SOURCES=\
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

traderStrategyRem_CFLAGS=-DREM -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyRem_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lrem -lremtrader -lEESTraderApi
	
traderStrategyYd_SOURCES=\
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

traderStrategyYd_CFLAGS=-DYD -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyYd_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lyd -lydtrader -lyd

#traderMduserEfviRem
#traderMduserEfviCtp66
#traderMduserEfviMini

traderMduserEfviRem_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserEfviRem_CFLAGS=-DEFVI -DREM_BACKUP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserEfviRem_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lefvimduser -ltraderutils -ltradercmn -lmduserapi \
	-Letherfabric -lciul1 \
	-Lrem -lremmduser -lEESQuoteApi

traderMduserEfviCtp66_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserEfviCtp66_CFLAGS=-DEFVI -DCTP_BACKUP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserEfviCtp66_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lefvimduser -ltraderutils -ltradercmn -lmduserapi \
	-Letherfabric -lciul1 \
    -Lctp6.6 -lctpmduser66 -lthostmduserapi_se

traderMduserEfviMini_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserEfviMini_CFLAGS=-DEFVI -DCTP_BACKUP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserEfviMini_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lefvimduser -ltraderutils -ltradercmn -lmduserapi \
	-Letherfabric -lciul1 \
    -Lctpmini -lctpminimduser -lthostmduserapi

# testMduserShm testMduserClient
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


