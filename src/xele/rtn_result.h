#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

#include <errno.h>
#include <string>

//enum XeleResult
//{
//	CORRECT      = true,
//	ERROR        = false,
//
//	SUCCESS      = CORRECT,
//	FAILURE      = ERROR,
//	XELE_SUCCESS = SUCCESS,
//	XELE_FAILURE = ERROR
//};
// 各函数返回�1�7�，以便错误判断
enum RTN_RESULT
{
    ERROR = false,
    CORRECT = true,
	SUCCESS      = CORRECT,
	FAILURE      = ERROR,
	XELE_SUCCESS = SUCCESS,
	XELE_FAILURE = ERROR,
    APP_EXIT,
    ERR_TOO_SHORT,
    ERR_DB_INIT,
    ERR_DB_CONNECT,
    ERR_DB_SELECT,
    ERR_DB_QUERY,
    ERR_DB_EXCUTE,
    ERR_DB_ROLLBACK,
    ERR_DB_COMMIT,
    ERR_DB_AUTOCOMMIT_FALSE,
    ERR_DB_STMT_OUT_OF_MEMORY,
    ERR_DB_STMT_PREPARE,
    ERR_DB_STMT_INVALID_PARAM_COUNT,
    ERR_DB_STMT_BIND,
    ERR_DB_QUERY_BATCH,
    ERR_DB_STMT_STORE_RESULT,
    ERR_DB_STORE_RESULT,
    ERR_DB_META_RESULT,
    ERR_DB_SQL,
    ERR_PARAM_NUM,
    ERR_EP_INDEX,
    ERR_EP_CONNECT,
    ERR_EP_VERSION,
    ERR_PARAM_INVALID,
    ERR_SOCKET_SEND,
    ERR_SOCKET_REVC,
    ERR_SOCKET_CREATE,
    ERR_SOCKET_BIND,
    ERR_SOCKET_LISTEN,
    ERR_EXISTS,
    ERR_NOT_EXISTS,
    ERR_TYPE_INVALID,
    ERR_COMMUNICATION_TRHEAD_EXIT,
    ERR_PROTOCOL_MAX,
    ERR_PRIVILEGE,
    ERR_SYMBOLTABLE_READ_SYMBOL,
    ERR_SYMBOLTABLE_WRITE_SYMBOL,
    ERR_SYMBOLTABLE_TABLE_FULL,
    ERR_SYMBOLTABLE_BIN_FULL,
    ERR_PACKET_ERR_INFO,
    ERR_NUMBER,
    ERR_NO_FIELD,
    ERR_NOT_LOGIN,
    ERR_INIT_FAILED,

    ERR_NOSPACE,

    //---------CTP----------//
    ERR_CTP_RETURN_VALUE_1,
    ERR_CTP_RETURN_VALUE_2,
    ERR_CTP_RETURN_VALUE_3,
    ERR_CTP_LOGIN_FAILED,
    ERR_CTP_TCP_DISCONNECTED
};

typedef RTN_RESULT XeleResult;

class RtnResult
{
public: // 解析RTN_RESULT，防止对index进行重复初始匄1�7
    static const char *Parse(RTN_RESULT result);

private: // 禁止产生对象、赋值�1�7�拷贄1�7
    RtnResult();
    RtnResult(const RtnResult &);
    RtnResult & operator=(const RtnResult &);
    static void Init();
};

#endif // ERROR_TYPE_H
