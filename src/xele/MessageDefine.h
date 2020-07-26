/*
 * MessageDefine.h
 *
 *  Created on: Oct 27, 2014
 *      Author: feic
 */

#ifndef MESSAGEDEFINE_H_
#define MESSAGEDEFINE_H_

/*
 * TID
 */
#define TID_RSP_ERROR                               0x00000001
#define TID_REQ_USER_LOGIN                          0x00001001  // 用户登录请求
#define TID_RSP_USER_LOGIN                          0x00001002  // 用户登录应答
#define TID_REQ_USER_LOGOUT                         0x00001003  // 用户退出请求
#define TID_RSP_USER_LOGOUT                         0x00001004  // 用户退出应答
#define TID_REQ_ORDER_INSERT                        0x00001005
#define TID_RSP_ORDER_INSERT                        0x00001006
#define TID_REQ_ORDER_ACTION                        0x00001007
#define TID_RSP_ORDER_ACTION                        0x00001008
#define TID_REQ_QUOTE_INSERT                        0x00001009
#define TID_RSP_QUOTE_INSERT                        0x0000100A
#define TID_REQ_QUOTE_ACTION                        0x0000100B
#define TID_RSP_QUOTE_ACTION                        0x0000100C
#define TID_REQ_OTC_ORDER_INSERT                    0x0000100D
#define TID_RSP_OTC_ORDER_INSERT                    0x0000100E
#define TID_REQ_USER_PASSWORD_UPDATE                0x0000100F  // 用户密码修改请求
#define TID_RSP_USER_PASSWORD_UPDATE                0x00001010  // 用户密码修改应答
#define TID_REQ_EXEC_ORDER_INSERT                   0x00001011
#define TID_RSP_EXEC_ORDER_INSERT                   0x00001012
#define TID_REQ_EXEC_ORDER_ACTION                   0x00001013
#define TID_RSP_EXEC_ORDER_ACTION                   0x00001014
#define TID_REQ_ADMIN_ORDER_INSERT                  0x00001015  // 管理报单录入请求
#define TID_RSP_ADMIN_ORDER_INSERT                  0x00001016  // 管理报单录入应答
#define TID_REQ_COMB_ORDER_INSERT                   0x00001017
#define TID_RSP_COMB_ORDER_INSERT                   0x00001018
#define TID_REQ_SUBSCRIBE_TOPIC                     0x00001F01  // 订阅主题请求
#define TID_RSP_SUBSCRIBE_TOPIC                     0x00001F02  // 订阅主题应答
#define TID_REQ_BULLETIN                            0x00002005
#define TID_RSP_BULLETIN                            0x00002006
#define TID_REQ_INSTRUMENT_STATUS_UPDATE            0x00002007
#define TID_RSP_INSTRUMENT_STATUS_UPDATE            0x00002008
#define TID_REQ_FORCE_USER_EXIT                     0x0000200B
#define TID_RSP_FORCE_USER_EXIT                     0x0000200C
#define TID_REQ_FORCE_USER_LOGOUT                   0x0000200D
#define TID_RSP_FORCE_USER_LOGOUT                   0x0000200E
#define TID_REQ_EXCHANGE_DATA_SYNC_STATUS_UPDATE    0x00002013
#define TID_RSP_EXCHANGE_DATA_SYNC_STATUS_UPDATE    0x00002014
#define TID_REQ_SG_DATA_SYNC_STATUS_UPDATE          0x00002015
#define TID_RSP_SG_DATA_SYNC_STATUS_UPDATE          0x00002016
#define TID_REQ_INFORMATION                         0x00002805
#define TID_RSP_INFORMATION                         0x00002806
#define TID_RTN_DB_INS_PARTICIPANT                  0x00003011
#define TID_RTN_DB_UPD_PARTICIPANT                  0x00003012
#define TID_RTN_DB_DEL_PARTICIPANT                  0x00003013
#define TID_RTN_DB_INS_CLIENT                       0x00003021
#define TID_RTN_DB_UPD_CLIENT                       0x00003022
#define TID_RTN_DB_DEL_CLIENT                       0x00003023
#define TID_RTN_DB_INS_PART_CLIENT                  0x00003024
#define TID_RTN_DB_DEL_PART_CLIENT                  0x00003025
#define TID_REQ_QRY_CLIENT_ACCOUNT                  0x00004001
#define TID_RSP_QRY_CLIENT_ACCOUNT                  0x00004002
#define TID_REQ_QRY_ORDER                           0x00004003
#define TID_RSP_QRY_ORDER                           0x00004004
#define TID_REQ_QRY_QUOTE                           0x00004005
#define TID_RSP_QRY_QUOTE                           0x00004006
#define TID_REQ_QRY_TRADE                           0x00004007
#define TID_RSP_QRY_TRADE                           0x00004008
#define TID_REQ_QRY_CLIENT                          0x00004009
#define TID_RSP_QRY_CLIENT                          0x0000400A
#define TID_REQ_QRY_PART_POSITION                   0x0000400B
#define TID_RSP_QRY_PART_POSITION                   0x0000400C
#define TID_REQ_QRY_CLIENT_POSITION                 0x0000400D  // 客户持仓查询请求
#define TID_RSP_QRY_CLIENT_POSITION                 0x0000400E  // 客户持仓查询应答
#define TID_REQ_QRY_INSTRUMENT                      0x0000400F
#define TID_RSP_QRY_INSTRUMENT                      0x00004010
#define TID_REQ_QRY_INSTRUMENT_STATUS               0x00004011
#define TID_RSP_QRY_INSTRUMENT_STATUS               0x00004012
#define TID_REQ_QRY_INSTRUMENT_MARGIN_RATE          0x00004013
#define TID_RSP_QRY_INSTRUMENT_MARGIN_RATE          0x00004014
#define TID_REQ_QRY_INSTRUMENT_COMMISSION_RATE      0x00004015
#define TID_RSP_QRY_INSTRUMENT_COMMISSION_RATE      0x00004016
#define TID_REQ_QRY_CREDIT_LIMIT                    0x00004017
#define TID_RSP_QRY_CREDIT_LIMIT                    0x00004018
#define TID_REQ_QRY_COMB_ORDER                      0x00004019
#define TID_RSP_QRY_COMB_ORDER                      0x0000401A
#define TID_REQ_QRY_MARKET_DATA                     0x00004101
#define TID_RSP_QRY_MARKET_DATA                     0x00004102
#define TID_REQ_QRY_BULLETIN                        0x00004103
#define TID_RSP_QRY_BULLETIN                        0x00004104
#define TID_REQ_QRY_TOPIC                           0x00004201  // 主题查询请求
#define TID_RSP_QRY_TOPIC                           0x00004202  // 主题查询应答
#define TID_REQ_QRY_USER_SESSION                    0x000043D1
#define TID_RSP_QRY_USER_SESSION                    0x000043D2
#define TID_REQ_QRY_USER                            0x000043D3
#define TID_RSP_QRY_USER                            0x000043D4
#define TID_REQ_QRY_PARTICIPANT                     0x000043D5
#define TID_RSP_QRY_PARTICIPANT                     0x000043D6
#define TID_REQ_QRY_MBL_MARKET_DATA                 0x000043E1
#define TID_RSP_QRY_MBL_MARKET_DATA                 0x000043E2
#define TID_REQ_QRY_INFORMATION                     0x000043E3
#define TID_RSP_QRY_INFORMATION                     0x000043E4
#define TID_RTN_TRADE                               0x00005001
#define TID_RTN_ORDER                               0x00005002
#define TID_RTN_EXEC_ORDER                          0x00005003
#define TID_RTN_QUOTE                               0x00005004
#define TID_RTN_COMB_ORDER                          0x00005005
#define TID_RTN_ABANDON_EXEC_ORDER                  0x00005006
#define TID_RTN_INSTRUMENT_STATUS                   0x00005011
#define TID_RTN_INS_INSTRUMENT                      0x00005101
#define TID_RTN_DEL_INSTRUMENT                      0x00005102  // 删除合约通知
#define TID_RTN_INS_COMBINATION_LEG                 0x00005103
#define TID_RTN_DEL_COMBINATION_LEG                 0x00005104  // 删除合约单腿通知
#define TID_RTN_ALIAS_DEFINE                        0x0000510A
#define TID_RTN_FLOW_MESSAGE_CANCEL                 0x00005201
#define TID_RTN_BULLETIN                            0x00005801
#define TID_RTN_MARKET_DATA                         0x00005802
#define TID_RTN_DEPTH_MARKET_DATA                   0x00005803  // 深度行情通知
#define TID_INTL_RTN_DISSEMINATION                  0x00005901  // 内部分发信息通知
#define TID_INTL_RTN_DEPTH_MARKET_DATA              0x00005902  // 内部深度行情通知
#define TID_RTN_ADMIN_ORDER                         0x00005A01
#define TID_ERR_RTN_ORDER_INSERT                    0x00006081
#define TID_ERR_RTN_ORDER_ACTION                    0x00006082
#define TID_ERR_RTN_QUOTE_INSERT                    0x00006083
#define TID_ERR_RTN_QUOTE_ACTION                    0x00006084
#define TID_ERR_RTN_EXEC_ORDER_INSERT               0x00006085
#define TID_ERR_RTN_EXEC_ORDER_ACTION               0x00006086
#define TID_ERR_ADMIN_ORDER_INSERT                  0x00006087
#define TID_ERR_RTN_COMB_ORDER_INSERT               0x00006088
#define TID_REQ_FOR_QUOTE                           0x00006089
#define TID_RSP_FOR_QUOTE                           0x0000608a
#define TID_ERR_RTN_ABANDON_EXEC_ORDER_INSERT       0x0000608c
#define TID_ERR_RTN_ABANDON_EXEC_ORDER_ACTION       0x0000608d
#define TID_RTN_QUOTE_DEMAND_NOTIFY                 0x0000608e
#define TID_RTN_MARGIN_COMBINATION_LEG              0x00006096  // cffex
#define TID_RTN_MARGIN_COMB_ACTION                  0x0000609c  // cffex

// Xele-Trade define
#define TID_XELE_TRADE_START                        0x00010000
#define TID_REQ_INSTRUMENT_PRICE                    0x00010001
#define TID_RSP_INSTRUMENT_PRICE                    0x00010002
#define TID_REQ_QRY_EXCHANGE_FRONT                  0x00010003
#define TID_RSP_QRY_EXCHANGE_FRONT                  0x00010004
#define TID_RTN_OPTION_APPLY                        0x00010005
#define TID_RTN_SPD_APPLY                           0x00010006
#define TID_RTN_HEDGE_CFM                           0x00010007

// TID_MAX
#define TID_MAX (TID_RTN_HEDGE_CFM + 1)
#define TID_NUMBER 23

/*
 * FID
 */
#define FID_DISSEMINATION                           0x0001  // 信息分发
#define FID_RSP_INFO                                0x0003  // 响应信息
#define FID_COMM_PHASE                              0x0004  // 通讯阶段
#define FID_EXCHANGE_TRADING_DAY                    0x0005  // 交易所交易日
#define FID_SETTLEMENT_SESSION                      0x0006  // 结算会话
#define FID_CURRENT_TIME                            0x0007  // 当前时间
#define FID_REQ_USER_LOGIN                          0x000A  // 用户登录请求
#define FID_RSP_USER_LOGIN                          0x000B  // 用户登录应答
#define FID_REQ_USER_LOGOUT                         0x000C  // 用户登出请求
#define FID_RSP_USER_LOGOUT                         0x000D  // 用户登出应答
#define FID_INPUT_ORDER                             0x0011  // 输入报单
#define FID_ORDER_ACTION                            0x0012  // 报单操作
#define FID_OTC_ORDER                               0x0013  // OTC报单
#define FID_INPUT_QUOTE                             0x0021  // 输入报价
#define FID_QUOTE_ACTION                            0x0022  // 报价操作
#define FID_INPUT_EXEC_ORDER                        0x0023  // 输入执行宣告
#define FID_EXEC_ORDER_ACTION                       0x0024  // 执行宣告操作
#define FID_INPUT_ABANDON_EXEC_ORDER                0x0025  // 输入放弃执行宣告
#define FID_ABANDON_EXEC_ORDER_ACTION               0x0026  // 放弃执行宣告操作
#define FID_USER_LOGOUT                             0x0031  // 用户登录退出
#define FID_USER_PASSWORD_UPDATE                    0x0032  // 用户口令修改
#define FID_FORCE_USER_EXIT                         0x0053  // 强制用户退出
#define FID_ACCOUNT_DEPOSIT                         0x0061  // 会员资金帐户入金
#define FID_PARTICIPANT                             0x1401  // 会员
#define FID_USER                                    0x1402  // 用户
#define FID_CLIENT                                  0x1403  // 客户
#define FID_USER_SESSION                            0x1406  // 用户会话
#define FID_PRODUCT_GROUP                           0x2101  // 产品组
#define FID_PRODUCT                                 0x2102  // 产品
#define FID_INSTRUMENT                              0x2103  // 合约
#define FID_COMBINATION_LEG                         0x2104  // 组合交易合约的单腿
#define FID_ACCOUNT_INFO                            0x2401  // 账户资金信息
#define FID_PART_POSITION                           0x2402  // 会员合约持仓
#define FID_CLIENT_POSITION                         0x2403  // 客户合约持仓
#define FID_RSP_INSTRUMENT_COMMISSION_RATE          0x2404  // 合约手续费率
#define FID_MARKET_DATA                             0x2407  // 市场行情
#define FID_DEPTH_MARKET_DATA                       0x2408  // 深度市场行情
#define FID_MBL_MARKET_DATA                         0x2410  // 分价表
#define FID_ALIAS_DEFINE                            0x2411  // 别名定义
#define FID_MARKET_DATA_BASE                        0x2431  // 行情基础属性
#define FID_MARKET_DATA_STATIC                      0x2432  // 行情静态属性
#define FID_MARKET_DATA_LAST_MATCH                  0x2433  // 行情最新成交属性
#define FID_MARKET_DATA_BEST_PRICE                  0x2434  // 行情最优价属性
#define FID_MARKET_DATA_BID23                       0x2435  // 行情申买二、三属性
#define FID_MARKET_DATA_ASK23                       0x2436  // 行情申卖二、三属性
#define FID_MARKET_DATA_BID45                       0x2437  // 行情申买四、五属性
#define FID_MARKET_DATA_ASK45                       0x2438  // 行情申卖四、五属性
#define FID_MARKET_DATA_UPDATE_TIME                 0x2439  // 行情更新时间属性
#define FID_QUOTE                                   0x2506  // 报价
#define FID_TRADE                                   0x2507  // 成交
#define FID_ORDER                                   0x2508  // 报单
#define FID_EXEC_ORDER                              0x2509  // 执行宣告
#define FID_ABANDON_EXEC_ORDER                      0x250b  //
#define FID_ADMIN_ORDER                             0x2510  // 管理报单
#define FID_INPUT_ADMIN_ORDER                       0x2511  // 管理报单输入
#define FID_COMB_ORDER                              0x2512  // 非标准组合报单
#define FID_INPUT_COMB_ORDER                        0x2513  // 非标准组合报单输入
#define FID_BULLETIN                                0x2607  // 公告
#define FID_EXCHANGE_DATA_SYNC_STATUS               0x2701  // 交易所数据同步状态
#define FID_RSP_INSTRUMENT_MARGIN_RATE              0x2702  // 合约保证金率
#define FID_INSTRUMENT_STATUS                       0x2703  // 合约状态
#define FID_QRY_ORDER                               0x3001  // 报单查询
#define FID_QRY_QUOTE                               0x3002  // 报价查询
#define FID_QRY_TRADE                               0x3003  // 成交查询
#define FID_QRY_MARKET_DATA                         0x3004  // 行情查询
#define FID_QRY_CLIENT                              0x3006  // 客户查询
#define FID_QRY_PART_POSITION                       0x3007  // 会员持仓查询
#define FID_QRY_CLIENT_POSITION                     0x3008  // 客户持仓查询
#define FID_QRY_CLIENT_ACCOUNT                      0x3009  // 客户资金查询
#define FID_QRY_INSTRUMENT                          0x300A  // 合约查询
#define FID_QRY_INSTRUMENT_STATUS                   0x300B  // 合约状态查询
#define FID_QRY_INSTRUMENT_MARGIN_RATE              0x300C  // 合约保证金率查询
#define FID_QRY_USER_SESSION                        0x300D  // 用户在线查询
#define FID_QRY_USER                                0x300E  // 用户查询
#define FID_QRY_BULLETIN                            0x300F  // 公告查询
#define FID_QRY_PARTICIPANT                         0x3010  // 会员查询
#define FID_QRY_INSTRUMENT_COMMISSION_RATE          0x3011  // 合约手续费率查询
#define FID_QRY_MBL_MARKET_DATA                     0x3014  // 合约价位查询
#define FID_QRY_CREDIT_LIMIT                        0x3015  // 信用限额查询
#define FID_RSP_CLIENT_ACCOUNT                      0x3101  // 客户资金应答
#define FID_RSP_PART_POSITION                       0x3102  // 会员持仓应答
#define FID_RSP_CLIENT_POSITION                     0x3103  // 客户持仓应答
#define FID_RSP_INSTRUMENT                          0x3104  // 合约查询应答
#define FID_QRY_INFORMATION                         0x3105  // 信息查询
#define FID_INFORMATION                             0x3106  // 信息查询
#define FID_CREDIT_LIMIT                            0x3108  // 信用限额
#define FID_RSP_CLIENT                              0x3109  // 客户查询应答
#define FID_FLOW_MESSAGE_CANCEL                     0x3201
#define FID_MARGIN_COMB_ACTION                      0x3204  // 客户申请组合通知(cffeax)
#define FID_MARGIN_COMBINATION_LEG                  0X3207  // 组合规则通知(cffex)
#define FID_INTERNAL_VERSION_IDENTIFY               0x5000  // Internal Version, 16 characters
#define FID_EXCHANGE_IDENTIFY                       0x5001  // Exchange Information, 16 characters
#define FID_SYSTEM_LOCAL_INNER_INFO                 0x5002 // Get System Inner Info
#define FID_QUOTE_DEMAND_NOTIFY                     0x8007  // 询价应答

// Xele-Trade define
#define FID_XELE_TRADE_START                        0xa000
#define FID_REQ_INSTRUMENT_PRICE                    0xa001 // 客户查询合约价格
#define FID_RSP_INSTRUMENT_PRICE                    0xa002 // 客户查询合约价格应答
#define FID_RSP_EXCHANGE_FRONT                      0xa005
#define FID_OPTION_APPLY                            0xa006
#define FID_SPD_APPLY                               0xa007
#define FID_HEDGE_CFM                               0xa008

// FID_MAX
#define FID_MAX                                     (FID_HEDGE_CFM + 1)
#define FID_NUM                                     101

// fair message id
#define MID_REQ_ORDER_INSERT                        101
#define MID_REQ_ORDER_ACTION                        103

#define MID_REQ_USER_LOGIN                          0x1
#define MID_REQ_USER_LOGOUT                         0x3
#define MID_REQ_HEARTBEAT                           0x5

#endif  // MESSAGEDEFINE_H_
