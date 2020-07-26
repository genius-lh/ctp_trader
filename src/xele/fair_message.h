// Copyright 2018 Copyright AcceleCom Inc.


#ifndef INCLUDE_FAIR_FAIR_MESSAGE_H_
#define INCLUDE_FAIR_FAIR_MESSAGE_H_

#include <string.h>
#include <stdint.h>
#include <utils.h>
#include <string>
#include "MessageDefine.h"

namespace faircpp {

#define                 MAX_MSG_LENGTH                    4096
#define                 FAIR_HEADER_C2V_LEN               12
#define                 EXP_HEADER_SET_MAX_INDEX          1
#define                 FIELD_SET_MIN_INDEX               9
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;


struct FairHeaderC2V {
  BYTE message_id;
  BYTE client_index;
  WORD token;
  DWORD seq_no;
  DWORD req_id;
};



struct FairHeader {
  BYTE id;
  BYTE exp_len;
  WORD len;
};


struct FairExpHeader {
  BYTE tag;
  BYTE tag_len;
};


struct FairCompression {
  BYTE type;
  BYTE method;
};


struct FairContextHeader {
  BYTE version;
  BYTE chain;
  WORD seq_seri;
  DWORD tid;
  DWORD seq_no;
  WORD field_count;
  WORD content_len;
  DWORD req_id;
};


struct FairFieldHeader {
  WORD fid;
  WORD fid_len;
};

class FairMessage {
 public:
  FairMessage();
  virtual ~FairMessage();

 protected:
  FairHeader *fair_header_;
  FairExpHeader *fair_exp_header_;
  FairCompression *fair_compression_;
  FairContextHeader *fair_context_header_;
  FairFieldHeader *fair_field_header_;
  FairHeaderC2V  *fair_header_c2v_;
  int msg_length_;
};

class FairMessageParse : public FairMessage {
 public:
  FairMessageParse();
  virtual ~FairMessageParse();

  const void *Msg();
  int Lenght();

  bool ParseMsg(void *msg, uint16_t length);
  BYTE GetFairHeadId();
  BYTE GetFairHeadExpLength();
  WORD GetFairHeadLength();
  BYTE GetFairExpHeaderTag();
  BYTE GetFairExpHeaderLength();
  void *GetFairExpHeaderData();
  BYTE GetFairCompressionType();
  BYTE GetFairCompressionMethod();
  BYTE GetFairContextHeaderVersion();
  BYTE GetFairContextHeaderChain();
  WORD GetFairContextHeaderSeqSeri();
  DWORD GetFairContextHeaderTid();
  DWORD GetFairContextHeaderSeqNo();
  WORD GetFairContextHeaderFieldCount();
  WORD GetFairContextHeaderContentLength();
  DWORD GetFairContextHeaderReqID();
  WORD GetFirstFtdFiledID();
  WORD GetFirstFtdFiledLength();
  void *GetFairDataField();
  void* GetFirstFtdField();
  void* GetFirstFtdFieldData();
  void* GetSecondFtdField();
  void* GetSecondFtdFieldData();

 private:
  void *msg_;
};

class FairMessageAssemble : public FairMessage {
 public:
  FairMessageAssemble();
  virtual ~FairMessageAssemble();

  void SetFairHeadId(BYTE id = 0x02);
  void SetFairExpHeader(BYTE tag, BYTE exp_len, const char *exp_data);
  void SetFairCompressionType(BYTE type = 0x01);
  void SetFairCompressionMethod(BYTE method = 0x00);
  void SetFairContextHeaderVersion(BYTE version = 0x4);
  void SetFairContextHeaderChain(BYTE chain = 'L');
  void SetFairContextHeaderSeqSeri(WORD seq_seri = 0);
  void SetFairContextHeaderTid(DWORD tid);
  void SetFairContextHeaderSeqNo(DWORD seq_no = 0);
  void SetFairContextHeaderReqID(DWORD req_id = 0);
  void AddField(WORD fid, WORD fid_len, void *filed);
  void AddRspInfo(DWORD error_id = NO_ERROR, const std::string &err_msg = "");
  void AddDissemination(WORD seq_seri, DWORD seq_no);
  const void *CreateMsg(int &length, int padlength = 0);
  void Reset(); 

  void SetFairHeaderMessageIdC2V(BYTE message_id);
  void SetFairHeaderSeqNoC2V(DWORD seq_no = 0);
  void SetFairHeaderReqIdC2V(DWORD req_id = 0);
  void SetFairHeaderClientIndexC2V(BYTE client_index = 0);
  void SetFairHeaderTokenC2V(WORD token = 0);  
  void AddFieldC2V(WORD fid_len, void *filed);
  const void *CreateMsgC2V(int &length, int padlength = 0);
  void ResetC2V(); 
 private:
  int set_index_;
  int data_len_;
  char msg_[MAX_MSG_LENGTH];
};

inline const void *FairMessageParse::Msg() {
  return msg_;
}

inline int FairMessageParse::Lenght() {
  return msg_length_;
}


}  // namespace faircpp
#endif  // INCLUDE_FAIR_FAIR_MESSAGE_H_
