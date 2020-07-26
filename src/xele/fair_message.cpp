// Copyright 2018 Copyright AcceleCom Inc.
//
// Created by changfengc on 6/18/15.
//

#include <CXeleFtdcUserApiStruct.h>
#include <fair_message.h>
#include <utils.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <stdexcept>

namespace faircpp {

FairMessage::FairMessage()
    : msg_length_(0) {
}

FairMessage::~FairMessage() {
}

// --------------------------------生成Fair下行方向报文组装-----------------------------------//
FairMessageAssemble::FairMessageAssemble()
    : set_index_(0), data_len_(0) {
  memset(msg_, 0, MAX_MSG_LENGTH);
  fair_header_ = (FairHeader *) msg_;
  fair_exp_header_ = (FairExpHeader *) (fair_header_ + 1);
  fair_compression_ = (FairCompression *) fair_exp_header_;
  fair_context_header_ = (FairContextHeader *) (fair_compression_ + 1);
  fair_field_header_ = (FairFieldHeader *) (fair_context_header_ + 1);
  fair_header_c2v_ = (FairHeaderC2V *) msg_;
}

FairMessageAssemble::~FairMessageAssemble() {
}

void FairMessageAssemble::SetFairHeadId(BYTE id) {
  ++set_index_;
  fair_header_->id = id;
}

void FairMessageAssemble::SetFairExpHeader(BYTE tag, BYTE exp_len, const char *exp_data) {
  if (set_index_ > EXP_HEADER_SET_MAX_INDEX) {
    char errmsg[512];
    snprintf(errmsg, sizeof(errmsg), "Create extension layer of fair message failed");
//        ERROR(errmsg);
    throw std::runtime_error(errmsg);
  }
  ++set_index_;

  if (exp_len > 0) {
    msg_length_ += exp_len;
    fair_header_->exp_len = exp_len;
    fair_exp_header_->tag = tag;
    fair_exp_header_->tag_len = exp_len - 2;
    memcpy(fair_exp_header_ + 1, exp_data, fair_exp_header_->tag_len);

    fair_compression_ = (FairCompression *) ((BYTE *) fair_exp_header_ + exp_len);
    fair_context_header_ = (FairContextHeader *) (fair_compression_ + 1);
    fair_field_header_ = (FairFieldHeader *) (fair_context_header_ + 1);
  }
}

void FairMessageAssemble::SetFairCompressionType(BYTE type) {
  ++set_index_;
  fair_compression_->type = type;
}

void FairMessageAssemble::SetFairCompressionMethod(BYTE method) {
  ++set_index_;
  fair_compression_->method = method;
}

void FairMessageAssemble::SetFairContextHeaderVersion(BYTE version) {
  ++set_index_;
  fair_context_header_->version = version;
}

void FairMessageAssemble::SetFairContextHeaderChain(BYTE chain) {
  ++set_index_;
  fair_context_header_->chain = chain;
}

void FairMessageAssemble::SetFairContextHeaderSeqSeri(WORD seq_seri) {
  ++set_index_;
  fair_context_header_->seq_seri = seq_seri;
}

void FairMessageAssemble::SetFairContextHeaderTid(DWORD tid) {
  ++set_index_;
  fair_context_header_->tid = tid;
}

void FairMessageAssemble::SetFairContextHeaderSeqNo(DWORD seq_no) {
  ++set_index_;
  fair_context_header_->seq_no = seq_no;
}

void FairMessageAssemble::SetFairContextHeaderReqID(DWORD req_id) {
  ++set_index_;
  fair_context_header_->req_id = req_id;
}

// 为方便api处理，RspInfo域放在第一个，域中数字在插入前作字节序转换操作
void FairMessageAssemble::AddField(WORD fid, WORD fid_len, void *filed) {
  if (set_index_ < FIELD_SET_MIN_INDEX) {
    char errmsg[512];
    snprintf(errmsg, sizeof(errmsg), "Add field of fair message too early");
//        ERROR(errmsg);
    throw std::runtime_error(errmsg);
  }
  ++set_index_;

  msg_length_ += sizeof(FairFieldHeader);
  msg_length_ += fid_len;

  ++fair_context_header_->field_count;
  fair_context_header_->content_len += fid_len + sizeof(FairFieldHeader);

  fair_field_header_->fid = fid;
  fair_field_header_->fid_len = fid_len;
  CHANGE_ENDIAN(fair_field_header_->fid);
  CHANGE_ENDIAN(fair_field_header_->fid_len);
  memcpy(fair_field_header_ + 1, filed, fid_len);
  fair_field_header_ = (FairFieldHeader *) ((BYTE *) fair_field_header_ + sizeof(FairFieldHeader) + fid_len);
}

void FairMessageAssemble::AddRspInfo(DWORD error_id, const std::string &err_msg) {
  ++set_index_;

  fair_field_header_->fid = FID_RSP_INFO;
  fair_field_header_->fid_len = sizeof(CXeleFtdcRspInfoField);

  ++fair_context_header_->field_count;
  fair_context_header_->content_len += fair_field_header_->fid_len + sizeof(FairFieldHeader);

  msg_length_ += sizeof(FairFieldHeader);
  msg_length_ += fair_field_header_->fid_len;

  CHANGE_ENDIAN(fair_field_header_->fid);
  CHANGE_ENDIAN(fair_field_header_->fid_len);
  CXeleFtdcRspInfoField *rsp_info = (CXeleFtdcRspInfoField *) (fair_field_header_ + 1);
  rsp_info->ErrorID = error_id;
  CHANGE_ENDIAN(rsp_info->ErrorID);
  int copy_length = sizeof(rsp_info->ErrorMsg) > err_msg.length() ? err_msg.length() : sizeof(rsp_info->ErrorMsg) - 1;
  memcpy(rsp_info->ErrorMsg, err_msg.c_str(), copy_length);

  fair_field_header_ = (FairFieldHeader *) (rsp_info + 1);
}

void FairMessageAssemble::AddDissemination(WORD seq_seri, DWORD seq_no) {
  ++set_index_;
  fair_field_header_->fid = FID_DISSEMINATION;
  fair_field_header_->fid_len = sizeof(CXeleFtdcDisseminationField);

  ++fair_context_header_->field_count;
  fair_context_header_->content_len += fair_field_header_->fid_len + sizeof(FairFieldHeader);

  msg_length_ += sizeof(FairFieldHeader);
  msg_length_ += fair_field_header_->fid_len;

  CHANGE_ENDIAN(fair_field_header_->fid);
  CHANGE_ENDIAN(fair_field_header_->fid_len);
  CXeleFtdcDisseminationField *dessmination = (CXeleFtdcDisseminationField *) (fair_field_header_ + 1);
  dessmination->SequenceSeries = seq_seri;
  dessmination->SequenceNo = seq_no;
  CHANGE_ENDIAN(dessmination->SequenceSeries);
  CHANGE_ENDIAN(dessmination->SequenceNo);

  fair_field_header_ = (FairFieldHeader *) (dessmination + 1);
}

const void *FairMessageAssemble::CreateMsg(int &length, int padlength) {
  msg_length_ += sizeof(FairHeader);
  msg_length_ += sizeof(FairCompression);
  msg_length_ += sizeof(FairContextHeader);

  //返回的长度不算padlength，用于拷贝时计算偏移量
  length = msg_length_;
  //add login pad len
  fair_header_->len = msg_length_ + padlength - sizeof(FairHeader);

  CHANGE_ENDIAN(fair_header_->len);
  CHANGE_ENDIAN(fair_context_header_->seq_seri);
  CHANGE_ENDIAN(fair_context_header_->tid);
  CHANGE_ENDIAN(fair_context_header_->seq_no);
  CHANGE_ENDIAN(fair_context_header_->field_count);

  fair_context_header_->content_len += padlength;
  CHANGE_ENDIAN(fair_context_header_->content_len);
  CHANGE_ENDIAN(fair_context_header_->req_id);

  return msg_;
}

void FairMessageAssemble::Reset() {
  memset(msg_, 0, MAX_MSG_LENGTH);
  fair_header_ = (FairHeader *) msg_;
  fair_exp_header_ = (FairExpHeader *) (fair_header_ + 1);
  fair_compression_ = (FairCompression *) fair_exp_header_;
  fair_context_header_ = (FairContextHeader *) (fair_compression_ + 1);
  fair_field_header_ = (FairFieldHeader *) (fair_context_header_ + 1);
  msg_length_ = 0;
}

// --------------------------------生成Fair上行方向报文组装-----------------------------------//
// set fair up stream message id
void FairMessageAssemble::SetFairHeaderMessageIdC2V(BYTE message_id) {
  fair_header_c2v_->message_id = message_id;
}

// set fair up stream sequence no
void FairMessageAssemble::SetFairHeaderSeqNoC2V(DWORD seq_no) {
  fair_header_c2v_->seq_no = seq_no;
}

// set fair up stream request id
void FairMessageAssemble::SetFairHeaderReqIdC2V(DWORD req_id) {
  fair_header_c2v_->req_id = req_id;
}

// set fair up stream client index
void FairMessageAssemble::SetFairHeaderClientIndexC2V(BYTE client_index) {
  fair_header_c2v_->client_index = client_index;
}

// set fair up stream token
void FairMessageAssemble::SetFairHeaderTokenC2V(WORD token) {
  fair_header_c2v_->token = token;
}

// set fair up stream data filed
void FairMessageAssemble::AddFieldC2V(WORD fid_len, void *filed) {
  data_len_ = fid_len;
  memcpy(fair_header_c2v_ + 1, filed, fid_len);
}

// set fair up stream message
const void *FairMessageAssemble::CreateMsgC2V(int &length, int padlength) {
  msg_length_ += sizeof(FairHeaderC2V);
  msg_length_ += data_len_;

  length = msg_length_;
  return msg_;
}

void FairMessageAssemble::ResetC2V() {
  memset(msg_, 0, MAX_MSG_LENGTH);
  fair_header_c2v_ = (FairHeaderC2V *) msg_;
  msg_length_ = 0;
  data_len_   = 0;
}

//--------------------------------解析Fair协议下行方向报文解析-----------------------------------//

FairMessageParse::FairMessageParse() {

}

FairMessageParse::~FairMessageParse() {

}

/**
 * @brief parsing FAIR message from buffer
 * @arg msg
 * @arg length
 */
bool FairMessageParse::ParseMsg(void *msg, uint16_t length) {
  msg_ = msg;
  msg_length_ = length;

  fair_exp_header_ = NULL;
  fair_header_ = (FairHeader *) msg;

  if (fair_header_->exp_len > 0) {
    fair_exp_header_ = (FairExpHeader *) (fair_header_ + 1);
    fair_compression_ = NULL;
    fair_context_header_ = NULL;
    fair_field_header_ = NULL;
    return true;
  } else {
    WORD body_length = fair_header_->len;
    CHANGE_ENDIAN(body_length);
    if (body_length + sizeof(FairHeader) != length) {
      ERROR("BodyLength[%d] length[%d], is wrong ?", body_length, length);
      ERROR_HEX(msg, length);
      return false;
    }
  }


  fair_compression_ = (FairCompression *) (fair_header_ + 1);
  fair_context_header_ = (FairContextHeader *) (fair_compression_ + 1);
  fair_field_header_ = (FairFieldHeader *) (fair_context_header_ + 1);


    //fair_context_header_ = (FairContextHeader *) (fair_header_ + 1);
    //fair_field_header_ = (FairFieldHeader *) (fair_context_header_ + 1);

  return true;
}

BYTE FairMessageParse::GetFairHeadId() {
  return fair_header_->id;
}

BYTE FairMessageParse::GetFairHeadExpLength() {
  return fair_header_->exp_len;
}

WORD FairMessageParse::GetFairHeadLength() {
  WORD length = fair_header_->len;
  CHANGE_ENDIAN(length);
  return length;
}

BYTE FairMessageParse::GetFairExpHeaderTag() {
  if (fair_exp_header_ != NULL) {
    return fair_exp_header_->tag;
  }
  return 0;
}

BYTE FairMessageParse::GetFairExpHeaderLength() {
  if (fair_exp_header_ != NULL) {
    return fair_exp_header_->tag_len;
  }
  return 0;
}

void *FairMessageParse::GetFairExpHeaderData() {
  if (fair_exp_header_ != NULL) {
    return (fair_exp_header_ + 1);
  }
  return NULL;
}

BYTE FairMessageParse::GetFairCompressionType() {
  if (fair_compression_ == NULL) {
    return 0;
  }
  return fair_compression_->type;
}

BYTE FairMessageParse::GetFairCompressionMethod() {
  if (fair_compression_ == NULL) {
    return 0;
  }
  return fair_compression_->method;
}

BYTE FairMessageParse::GetFairContextHeaderVersion() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  return fair_context_header_->version;
}

BYTE FairMessageParse::GetFairContextHeaderChain() {
  if (fair_context_header_ == NULL) {
    return '\0';
  }
  return fair_context_header_->chain;
}

WORD FairMessageParse::GetFairContextHeaderSeqSeri() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  WORD seq_seri = fair_context_header_->seq_seri;
  CHANGE_ENDIAN(seq_seri);
  return seq_seri;
}

DWORD FairMessageParse::GetFairContextHeaderTid()
{
    if (fair_context_header_ == NULL)
    {
        return 0;
    }
    DWORD tid = fair_context_header_->tid;
    CHANGE_ENDIAN(tid);
    return tid;
}

DWORD FairMessageParse::GetFairContextHeaderSeqNo() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  DWORD seq_no = fair_context_header_->seq_no;
  CHANGE_ENDIAN(seq_no);
  return seq_no;
}

WORD FairMessageParse::GetFairContextHeaderFieldCount() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  WORD filed_count = fair_context_header_->field_count;
  CHANGE_ENDIAN(filed_count);
  return filed_count;
}

WORD FairMessageParse::GetFairContextHeaderContentLength() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  WORD content_length = fair_context_header_->content_len;
  CHANGE_ENDIAN(content_length);
  return content_length;
}

DWORD FairMessageParse::GetFairContextHeaderReqID() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  DWORD req_id = fair_context_header_->req_id;
  CHANGE_ENDIAN(req_id);
  return req_id;
}

WORD FairMessageParse::GetFirstFtdFiledID() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  WORD fid = fair_field_header_->fid;
  CHANGE_ENDIAN(fid);
  return fid;
}

WORD FairMessageParse::GetFirstFtdFiledLength() {
  if (fair_context_header_ == NULL) {
    return 0;
  }
  WORD filed_length = fair_field_header_->fid_len;
  CHANGE_ENDIAN(filed_length);
  return filed_length;
}

void *FairMessageParse::GetFairDataField() {
  if (fair_field_header_ == NULL) {
    return NULL;
  }
  return fair_field_header_ + 1;
}


//
void* FairMessageParse::GetFirstFtdField()
{
    //return fair_context_header_ + 1;
    return fair_field_header_;
}

void* FairMessageParse::GetFirstFtdFieldData()
{
    return fair_field_header_ + 1;
}

void* FairMessageParse::GetSecondFtdField()
{
    int len = GetFirstFtdFiledLength() + 4;
    unsigned char* p = (unsigned char*)fair_field_header_ + len;
    return (p);
}

void* FairMessageParse::GetSecondFtdFieldData()
{
    int len = GetFirstFtdFiledLength() + 4;
    unsigned char* p = (unsigned char*)fair_field_header_ + len;
    return (p + 4);
}

}  // namespace faircpp
