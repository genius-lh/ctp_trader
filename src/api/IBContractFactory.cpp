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
  
  //打开文件流操作
  ifstream fin(configFilename);   
  string line;
  //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
  while (getline(fin, line))
  {
    //将整行字符串line读入到字符串流istringstream中
    istringstream sin(line);
    //声明一个字符串向量
    vector<string> fields;
    string field;
    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
    while (getline(sin, field, ','))
    {
      //将刚刚读取的字符串添加到向量fields中
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

