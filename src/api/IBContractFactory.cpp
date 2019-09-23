#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <hash_map>

#include "IBContractFactory.h"

using namespace std;

IBContractFactory* IBContractFactory::getInstance(const char* cfgFile)
{
  if(NULL == instance){
    instance = new IBContractFactory(cfgFile);
  }

  return instance;
}

IBContractFactory::~IBContractFactory()
{

}

Contract IBContractFactory::getContract(const char* contract)
{

}

bool IBContractFactory::init()
{
  bool result = true;
  
  //���ļ�������
  ifstream fin(configFilename);   
  string line;
  //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
  while (getline(fin, line))
  {
    //�������ַ���line���뵽�ַ�����istringstream��
    istringstream sin(line);
    //����һ���ַ�������
    vector<string> fields;
    string field;
    //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
    while (getline(sin, field, ','))
    {
      //���ոն�ȡ���ַ�����ӵ�����fields��
    	fields.push_back(field);
    }
    
    if(fields.size() < 4){
      continue;
    }

    Contract contract;
    contract.symbol = fields[0];
    contract.secType = fields[1];
    contract.currency = fields[2];
    contract.exchange = fields[3];
    contractMap[contract.symbol.c_str()] = contract;
  }
  return result;
}

IBContractFactory::IBContractFactory(const char* cfgFile)
:configFilename(cfgFile)
{

}

