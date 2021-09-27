AUTOMAKE_OPTIONS=foreign

noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a

noinst_PROGRAMS=libctpmduser.so libctptrader.so libctpmduser66.so\
				libctpminimduser.so libctpminitrader.so \
				libgfmduser.so libsfmduser.so libswmduser.so libefh32mduser.so\
				libfemasmduser.so libfemastrader.so \
				libfemasmduser_af.so libfemastrader_af.so \
				libremmduser.so libremtrader.so libcffexl2mduser.so

bin_PROGRAMS=\
			traderMduserCtp traderStrategyCtp traderMduserCtp66\
			traderMduserCtpMini traderStrategyCtpMini \
			traderMduserFemas traderStrategyFemas \
			traderMduserFemasAF traderStrategyFemasAF \
			traderMduserRem traderStrategyRem\
			traderMduserSolarFlare \
			traderMduserGfShfe \
			traderMduserXhsp \
			traderMduserXhSf \
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

# libctpmduser.so libctptrader.so
libctpmduser_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys
libctpmduser_so_LDFLAGS=-shared -fPIC

libctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
        src/api/CtpTraderHandler.cpp
libctptrader_so_CPPFLAGS=-fPIC -Ictp -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libctptrader_so_LDFLAGS=-shared -fPIC

libctpmduser66_so_SOURCES=src/api/trader_mduser_api_ctp.cpp\
        src/api/CtpMduserHandler.cpp
libctpmduser66_so_CPPFLAGS=-fPIC -Ictp6.6 -Ilibevent -Ilibevent/compat/sys
libctpmduser66_so_LDFLAGS=-shared -fPIC

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
libfemasmduser_af_so_CPPFLAGS=-fPIC -Ifemas302 -Ilibevent -Ilibevent/compat/sys
libfemasmduser_af_so_LDFLAGS=-shared -fPIC

libfemastrader_af_so_SOURCES=src/api/trader_trader_api_femas_af.cpp\
	src/api/FemasAFTraderHandler.cpp
libfemastrader_af_so_CPPFLAGS=-fPIC -Ifemas302 -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libfemastrader_af_so_LDFLAGS=-shared -fPIC


# libgfmduser.so libsfmduser.so libswmduser.so
libgfmduser_so_SOURCES=src/api/trader_mduser_api_gf.cpp
libgfmduser_so_CPPFLAGS=-fPIC -Ifemas20 -Ictp -Isrc/util -Ilibevent -Ilibevent/compat/sys
libgfmduser_so_LDFLAGS=-shared -fPIC

libsfmduser_so_SOURCES=src/api/trader_mduser_api_sf.cpp
libsfmduser_so_CPPFLAGS=-fPIC -Isolarflare -Ilibevent -Ilibevent/compat/sys
libsfmduser_so_LDFLAGS=-shared -fPIC

libswmduser_so_SOURCES=src/api/trader_mduser_api_sw.cpp
libswmduser_so_CPPFLAGS=-fPIC -Isrc/util -Ilibevent -Ilibevent/compat/sys
libswmduser_so_LDFLAGS=-shared -fPIC

libefh32mduser_so_SOURCES=src/api/trader_mduser_api_efh32.cpp
libefh32mduser_so_CPPFLAGS=-fPIC -Isrc/util -Ilibevent -Ilibevent/compat/sys
libefh32mduser_so_LDFLAGS=-shared -fPIC

#libcffexl2mduser.so
libcffexl2mduser_so_SOURCES=src/api/trader_mduser_api_cffex_l2.cpp
libcffexl2mduser_so_CPPFLAGS=-fPIC -Isolarflare -Ilibevent -Ilibevent/compat/sys
libcffexl2mduser_so_LDFLAGS=-shared -fPIC

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
        -Lctp -lctpmduser -lthostmduserapi_se

traderMduserCtp66_SOURCES=src/svc/trader_mduser_boardcast.c \
        src/svc/trader_mduser_cnn.c \
        src/svc/trader_mduser_svr.c

traderMduserCtp66_CFLAGS=-DCTP\
        -Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
        -Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserCtp66_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
        -L. -ltradercmn -lmduserapi\
        -Lctp6.6 -lctpmduser66 -lthostmduserapi_se


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

traderMduserFemasAF_CFLAGS=-DFEMAS302\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserFemasAF_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lswmduser -ltradercmn -lmduserapi\
	-Lfemas302 -lfemasmduser_af -lUSTPmduserapiAF

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

traderStrategyFemasAF_CFLAGS=-DFEMAS302 -Isrc/api \
	-Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyFemasAF_LDADD=\
	-lc -lstdc++ -lm -lpthread -lresolv -ldl\
	-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
	-L. -ltradercmn -ltraderapi\
	-Lfemas302 -lfemastrader_af -lUSTPtraderapiAF
	
#traderMduserRem traderStrategyRem 
traderMduserRem_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserRem_CFLAGS=-DREM62\
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

# traderMduserSolarFlare
# traderMduserGfShfe
traderMduserSolarFlare_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserSolarFlare_CFLAGS=-DGF_SOLARFLARE -DGF_CTP_BACKUP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserSolarFlare_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lsfmduser -ltradercmn -lmduserapi \
	-Lsolarflare -lefvi-common -lciul1 \
	-Lctp -lctpmduser -lthostmduserapi_se

traderMduserGfShfe_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserGfShfe_CFLAGS=-DGF_SHFE -DGF_CTP_BACKUP\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserGfShfe_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lgfmduser -ltradercmn -lmduserapi \
	-Lctp -lctpmduser -lthostmduserapi_se

traderMduserXhsp_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXhsp_CFLAGS=-DXHSP_EFH32_CTPMINI\
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXhsp_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lefh32mduser -ltradercmn -lmduserapi \
	-Lctpmini -lctpminimduser -lthostmduserapi

#traderMduserXhSf
traderMduserXhSf_SOURCES=src/svc/trader_mduser_boardcast.c \
	src/svc/trader_mduser_cnn.c \
	src/svc/trader_mduser_svr.c

traderMduserXhSf_CFLAGS=-DXH_SOLARFLARE \
	-Isrc/svc -Isrc/api -Isrc/util -Isrc/cfg -Isrc/log\
	-Ihiredis -Ilibevent -Ilibevent/compat/sys 

traderMduserXhSf_LDADD=-Llibevent -Lhiredis -lhiredis -levent_core -ldl \
	-L. -lcffexl2mduser -ltradercmn -lmduserapi \
	-Lsolarflare -lefvi-common -lciul1 \
	-Lrem -lremmduser -lEESQuoteApi

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


