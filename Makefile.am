AUTOMAKE_OPTIONS=foreign

noinst_LIBRARIES=libtradercmn.a libmduserapi.a libtraderapi.a

noinst_PROGRAMS=libctpmduser.so libctptrader.so libsopctptrader.so\
libfemasmduser.so libfemastrader.so libfemasmduser_udp.so\
libfemasmduserAF.so libfemastraderAF.so \
libxspeedmduser.so libxspeedtrader.so \
libIBtraderApiBase.so libibmduser.so libibtrader.so \
libstgmysql.so

bin_PROGRAMS=\
traderMduserIB traderStrategyIB \
traderMduserCtp traderStrategyCtp traderStrategySopCtp\
traderMduserFemas traderStrategyFemas \
traderMduserFemasAF traderStrategyFemasAF \
traderMduserXSpeedSOP traderStrategyXSpeedSOP \
traderMduserXSpeedStock testMduserShm testMduserClient testStgMysql testMduserClient2

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

libsopctptrader_so_SOURCES=src/api/trader_trader_api_ctp.cpp\
src/api/CtpTraderHandler.cpp
libsopctptrader_so_CPPFLAGS=-DSOPCTP -fPIC -Ictpsop -Ilibevent -Ilibevent/compat/sys -Isrc/util -Isrc/log
libsopctptrader_so_LDFLAGS=-shared -fPIC

# libfemasmduser.so libfemastrader.so libfemasmduser_udp.so
libfemasmduser_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
src/api/FemasMduserHandler.cpp
libfemasmduser_so_CPPFLAGS=-fPIC -Ifemas -Ilibevent -Ilibevent/compat/sys
libfemasmduser_so_LDFLAGS=-shared -fPIC

libfemasmduser_udp_so_SOURCES=src/api/trader_mduser_api_femas.cpp\
src/api/FemasMduserHandler.cpp
libfemasmduser_udp_so_CPPFLAGS=-DFEMAS_UDP -DFEMAS20 -fPIC -Ifemas20 -Ilibevent -Ilibevent/compat/sys
libfemasmduser_udp_so_LDFLAGS=-shared -fPIC

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

libstgmysql_so_SOURCES=\
src/mysql/stg_mysql_sys.c\
src/mysql/stg_strategy_market_data_tick.c
libstgmysql_so_CPPFLAGS=-fPIC -Isrc/util -Isrc/log -Isrc/mysql
libstgmysql_so_LDFLAGS=-shared -fPIC

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
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyCtp_LDADD=\
-lc -lstdc++ -lm -lpthread -lresolv -ldl\
-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
-L. -ltradercmn -ltraderapi\
-Lctp -lctptrader -lthosttraderapi_se

traderStrategySopCtp_SOURCES=\
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

traderStrategySopCtp_CFLAGS=-DCTP -Isrc/api \
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategySopCtp_LDADD=\
-lc -lstdc++ -lm -lpthread -lresolv -ldl\
-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
-L. -ltradercmn -ltraderapi\
-Lctpsop -lsopctptrader -lsoptthosttraderapi_se

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
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
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
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
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
-Isrc/svc -Isrc/util -Isrc/log -Isrc/cfg -Isrc/client -Isrc/trade\
-Ihiredis -Ilibevent -Ilibevent/compat/sys -Isqlite3

traderStrategyXSpeedSOP_LDADD=\
-lc -lstdc++ -lm -lpthread -lresolv -ldl\
-Llibevent -Lhiredis -lhiredis -levent_core -Lsqlite3 -lsqlite3\
-L. -ltradercmn -ltraderapi\
-Lxspeed -lxspeedtrader -lDFITCSECTraderApi

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

testStgMysql_SOURCES=\
src/test/test_stg.c

testStgMysql_CFLAGS=-Isrc/mysql

testStgMysql_LDADD=\
-L. -lstgmysql \
-L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto \
-lc -lm -lpthread -lresolv -ldl

testMduserClient2_SOURCES=\
src/test/trader_mduser_client_test2.c\
src/trade/trader_mduser_client.c

testMduserClient2_CFLAGS=-Isrc/trade -Isrc/util -Isrc/log -Isrc/cfg\
-Isrc/api -Isrc/mysql\
-Ilibevent -Ilibevent/compat/sys

testMduserClient2_LDADD=\
-Llibevent -levent_core \
-L. -lstgmysql -ltradercmn \
-L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto \
-lc -lm -lpthread -lresolv -ldl

