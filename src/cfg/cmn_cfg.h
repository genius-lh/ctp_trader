#ifndef _CMN_CFG_H_
#define _CMN_CFG_H_

typedef char* pchar;
typedef struct cmn_cfg_def cmn_cfg;
typedef struct cmn_cfg_method_def cmn_cfg_method;
struct cmn_cfg_def{
  char* pCfgFileName;
  char sWorkPath[255+1];
  char sLogPath[255+1];
  int nLogLevel;
  char sListenIP[15+1];
  int nListenPort;
  int nConnNum;
  int nPosLimit;
  char sBrokerId[11];
  char sMdPrefix[255+1];
  char sMdFrontAdd[255+1];
  char sTrFrontAdd[255+1];
  char sHedgeFlag[2];
  int nStoreMduser;
  cmn_cfg_method* pMethod;
};

struct cmn_cfg_method_def{
  int (*xInit)(cmn_cfg* self);
  pchar (*xGetWorkspace)(cmn_cfg* self);
  pchar (*xGetLogPath)(cmn_cfg* self);
  int (*xGetLogLevel)(cmn_cfg* self);
  pchar (*xGetListenIP)(cmn_cfg* self);
  short (*xGetListenPort)(cmn_cfg* self);
  int (*xGetConnNum)(cmn_cfg* self);
  int (*xGetPosLimit)(cmn_cfg* self);
  pchar (*xGetBrokerId)(cmn_cfg* self);
  pchar (*xGetMdPrefix)(cmn_cfg* self);
  pchar (*xGetMdFrontAdd)(cmn_cfg* self);
  pchar (*xGetTrFrontAdd)(cmn_cfg* self);
  char (*xGetHedgeFlag)(cmn_cfg* self);
  int (*xGetStoreMduser)(cmn_cfg* self);
};

extern cmn_cfg* cmn_cfg_new(char* cfg_file);
extern void cmn_cfg_free(cmn_cfg* self);

#endif //_CMN_CFG_H_

