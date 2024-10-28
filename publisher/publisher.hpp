#ifndef PUB_HPP
#define PUB_HPP

#include "data/data.hpp"
#include "include/lcm/lcm-cpp.hpp"
#include "include/snappy/snappy.h"
#include "subscriber/subscriber.hpp"

#include <memory>
#include <fstream>
#include <iostream>

class Pub
{
private:
public:
  Pub();

  ~Pub();

public:
  bool PublishMsg(std::shared_ptr<lcm::LCM> lcm,std::string channel,std::shared_ptr<Data> data);
  void init(std::shared_ptr<lcm::LCM> lcm,int32_t id,std::string ip);
};






#endif