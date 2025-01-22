#ifndef DATA_HPP
#define DATA_HPP
#include "stu.hpp"
#include "include/dataProcess.hpp"
#include "include/json/json.h"
#include "include/snappy/snappy.h"
#include "include/zlib.h"
#include "rsa/rsa.hpp"
#include "aes/aes.hpp"

#include <memory>
#include <fstream>
#include <iostream>
#include <thread>
#include <functional>
#include <bitset>

class Data
{
private:
  std::shared_ptr<data::stu> m_data;
public:
  Data();
  Data(std::string jsonBuff,std::string payloadBuff);
  Data(int32_t id,std::string ip);
  
  ~Data();

public:
  std::shared_ptr<data::stu> getInterface();
  void printBinary(const std::string& str);
};




#endif