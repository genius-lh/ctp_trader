// Copyright 2018 Copyright AcceleCom Inc.
/*
 * fair_define.h
 *
 *  Created on: Mar 8, 2018
 *      Author: changfengc
 */

#ifndef INCLUDE_FAIR_FAIR_DEFINE_H_
#define INCLUDE_FAIR_FAIR_DEFINE_H_

#include <stddef.h>

/* notes: 
   1. xele system has two data center, one is query data center, other is order data center;
   2. the two data center both should login or logout£¬ the query data center login or logout by xele API, user can 
      reference to the api document; the order data center login or logout should by user self;
   3. the length follows of login or logout is that to order data center packet, not login to query data center;
*/
namespace faircpp { 
const size_t kLengthReqUserLogin                         = 12;
const size_t kLengthReqUserLogout                        = 12;
const size_t kLengthReqHeartBeat                         = 12;
const size_t kLengthReqOrderInsert                       = 48;
const size_t kLengthReqOrderAction                       = 48;
const size_t kLengthReqQryInstrument                     = 70;
const size_t kLengthReqQryInstrumentPrice                = 70;
const size_t kLengthReqQryInstrumentCommissionRate       = 72;
const size_t kLengthReqQryInstrumentMarginRate           = 73;
const size_t kLengthReqUserPasswordUpdate                = 139;
const size_t kLengthReqQryTrade                          = 103;
const size_t kLengthReqQryExchangeFront                  = 46;
const size_t kLengthOnRspError                           = 115;
const size_t kLengthOnRspUserLogin                       = 250; 
const size_t kLengthOnRspUserLogout                      = 146;
const size_t kLengthOnRspOrderInsert                     = 292;
const size_t kLengthOnRspOrderAction                     = 234;
const size_t kLengthOnRspQryClientPosition               = 273;
const size_t kLengthOnRspQryInstrument                   = 349;
const size_t kLengthOnRspQryClientAccount                = 250;
const size_t kLengthOnRspQryInstrumentMarginRate         = 198;
const size_t kLengthOnRspQryInstrumentCommissionRate     = 209;
const size_t kLengthOnRspUserPasswordUpdate              = 228;
const size_t kLengthOnRspQryOrder                        = 410;
const size_t kLengthOnRspQryTrade                        = 321;
const size_t kLengthOnRspInstrumentPrice                 = 166;
const size_t kLengthOnRspQryExchangeFront                = 141;
const size_t kLengthOnRtnTrade                           = 245;
const size_t kLengthOnRtnOrder                           = 334;
const size_t OnRtnInsInstrument                          = 179;
}  // namespace faircpp

#endif  // INCLUDE_FAIR_FAIR_DEFINE_H_
