#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <map>
#include <rtn_result.h>


#define LOCK_FILE   "/tmp/util.lck"

using namespace std;

const std::string strClientSessionType = "CLIENT";
const std::string strVenueSessionType = "VENUE";

//enum XELE_CMD_TYPE
//{
//	XELE_EP_WRITE = 0,
//	XELE_EP_READ = 1,
//	XELE_WRITE_CMD = 2,
//	XELE_READ_CMD = 3
//};

enum ERROR_CODE {
  NO_ERROR = 0
};

#define  ERROR(fmt, ...)
#define  ERROR_HEX(fmt, ...)

enum Direction
{
	XELE_RECEIVED = 0
	, XELE_SENT = 1
};	// needs to match ixfix (see fixengine.h)

struct JournalEntry
{
	off_t    offset;
	uint32_t length;
	Direction direction;
}__attribute__((__packed__));

struct IndexEntry	// do it this way (a) to be consistent with journalentry and (b) to allow the index format to change
{
	off_t    offset;
	uint64_t length;
}__attribute__((__packed__));

class UTILS
{
public:
    static void             FormatLine(std::string &line, char delim);
    static char            *Trim(char *str);
    static int              SplitSafty(const char *source, char delim, vector<string> &container);
    static int              Split(char *source, char delim, vector<string> &container);
    static int              SplitEx(const string& src, const string &separator, vector<string> &strs);
	static bool             token( std::vector<std::string>& token, const std::string& str, const std::string delimiter="." );
    static void             SetBit( uint32_t&, const uint32_t );
	static void             SetBit(uint8_t *reg, const uint32_t bit);
    static RTN_RESULT       CheckBit( const uint32_t, const uint32_t );
    static void             ClearBit( uint32_t&, const uint32_t );
	static void             ClearBit(uint8_t *reg, const uint32_t bit);
    static bool             IsAppRunning();
    static void             NotifyStopApplication();
    static pid_t            getThreadId();

    template <class T>
    static T                StrToNo(const string &value);

    template <class T>
    static string           NoToStr(const T      &value, string *result = NULL);

    static void             ComplementCodeAndTrueCodeSwap(void *dst, const void *src, int length);
    static void ConvertMsgToHex(char *pszDstMsg, int &iDstLength, const char *pszSrcMsg, int iSrcLength);

    static void mac_ntoa(const uint32_t mMac, const uint32_t lMac,string& mac_str);

	static std::string getAppName();
    static void print_hex_dump(const char *buf, size_t len, const char *extra);

    static void lock(int site);
    static void unlock(int site);
	static bool has_digit(const char *str);
private:
    static int              lock_fd;
    static int              is_app_running;
};

inline void UTILS::SetBit(uint32_t &reg, const uint32_t bit)
{
    reg |= (1 << bit);
}

inline void UTILS::SetBit(uint8_t *reg, const uint32_t bit)
{
	int B  = bit / 8;
	int b  = bit % 8;
	reg[B] |= (1 << b);
}

inline RTN_RESULT UTILS::CheckBit(const uint32_t reg, const uint32_t bit)
{
    return  1 == (reg & ( 1 << bit )) ? CORRECT : ERROR;
}

inline void UTILS::ClearBit(uint32_t &reg, const uint32_t bit)
{
    reg &= ~( 1 << bit );
}

inline void UTILS::ClearBit(uint8_t *reg, const uint32_t bit)
{
    int B  = bit / 8;
    int b  = bit % 8;
    reg[B] &= ~(1 << b);
}

template <class T>
inline T UTILS::StrToNo(const string &value)
{
    stringstream sst;
    if (value.substr(0, 2).compare("0x") == 0)
    {
        sst << hex << value;
    }
    else
    {
        sst << value;
    }

    T t(0);
    sst >> t;

    return t;
}

template <class T>
inline string UTILS::NoToStr(const T &value, string *result)
{
    stringstream sst;
    sst << value;
    if (result != NULL)
    {
        sst >> *result;
        return *result;
    }
    else
    {
        return sst.str();
    }
}

inline uint16_t ChangeEndian(uint16_t value)
{
    return  ((value&0x00ff)<<8)|
            ((value&0xff00)>>8);
}

inline short ChangeEndian(short value)
{
    return  ChangeEndian((uint16_t)value);
}

inline uint32_t ChangeEndian(uint32_t value)
{
    return  ((value&0x000000ff)<<24) |
            ((value&0x0000ff00)<<8)  |
            ((value&0x00ff0000)>>8)  |
            ((value&0xff000000)>>24);
}

inline int ChangeEndian(int value)
{
    return  ChangeEndian((uint32_t)value);
}

inline double ChangeEndian(double value)
{
    double temp = value;
    char *pSource = (char *)&value;
    char *pTarget = (char *)&temp;
    pTarget[7] = pSource[0];
    pTarget[6] = pSource[1];
    pTarget[5] = pSource[2];
    pTarget[4] = pSource[3];
    pTarget[3] = pSource[4];
    pTarget[2] = pSource[5];
    pTarget[1] = pSource[6];
    pTarget[0] = pSource[7];
    return temp;
}
// default is little endian
#ifdef _BIG_ENDIAN_
#define CHANGE_ENDIAN(value)    value = ChangeEndian(value)
#else
#define CHANGE_ENDIAN(value)
#endif

struct SessionInfo
{
    bool        isUsed;
    int32_t     session_number;
    char        user_id[16];
    char        password[41];
    char        remote_ip_address[32];
    uint16_t    remote_port_internal;
    uint16_t    remote_port;
    uint16_t    local_port;
    char        participant_id[11];
    char        user_product_info[41];
    char        interface_product_info[41];
    char        protocol_info[41];
    int         data_center_id;
    bool        subscribe_private;
    bool        bFastLogin;
};


static inline void hexdump_line(FILE* fp, const void* data, size_t size)
{
  if (0 == data) return ;
  size_t i;
  for (i = 0 ; i < size ; ++i)
  {
    fprintf(fp, "%02hhx", ((const unsigned char*)data)[i]);
  }
}

#define LOG_TEST_PACKET() \
         do { \
              fprintf(ut_log, "\n%s:\n", __PRETTY_FUNCTION__); \
         } while (0) \


#endif // UTILS_H
