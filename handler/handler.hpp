#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "data/data.hpp"
#include "aes/aes.hpp"
#include "include/lcm/lcm-cpp.hpp"
#include <iostream>
#include <memory>

class Handler
{
private:

public:
  Handler();
  ~Handler();

public:
  void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const data::stu* msg);

};

#endif