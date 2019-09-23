
#pragma once
#ifndef IB_CONTRACT_FACTORY_H
#define IB_CONTRACT_FACTORY_H

#include "Contract.h"
#include <hash_map>
#include <string>

using namespace std;

class IBContractFactory{
public:
  static IBContractFactory* getInstance(const char* cfgFile);
  ~IBContractFactory();
public:
  Contract getContract(const char* contract);
  bool init();
private:
  static IBContractFactory* instance = NULL;
  IBContractFactory(const char* cfgFile);
  string configFilename;
  hash_map<string, Contract> contractMap;
};

#endif

