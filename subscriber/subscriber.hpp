#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP
#include "handler/handler.hpp"
#include "data/data.hpp"
typedef void (Handler::*FuncHandle)(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const data::stu* msg);
class Subscriber
{
private:
  std::string channel;

public:

public:
  Subscriber();
  ~Subscriber();

public:
  bool init(std::string channel,FuncHandle fuc,Handler &handle);
};


#endif