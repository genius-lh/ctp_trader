#ifndef CUPSTYPE_H
#define CUPSTYPE_H          /* allow duplicate includes */

typedef short                   INT16;
typedef unsigned short          UINT16;

#if defined(_LP64) || defined(__64BIT__) || defined(__LP64__)
    #define  _CUPS_64BIT_
#endif
 
#ifdef _CUPS_64BIT_
    typedef int                 INT32;
    typedef unsigned int        UINT32;
    typedef long                INT64;
    typedef unsigned long       UINT64;
#else
    typedef long                INT32;
    typedef unsigned long       UINT32;
    typedef long long           INT64;
    typedef unsigned long long  UINT64;
#endif

#ifndef FAILURE
#define FAILURE      -1 /* 错误返回值 */
#define SUCCESS      0  /* 成功返回值 */
#endif


#endif /* CUPSTYPE_H */

