#include "handler/handler.hpp"
typedef void (Handler::*FuncHandle)(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const data::stu* msg);
class Subscriber
{
private:
  std::string channel;

public:

public:
  Subscriber(/* args */);
  ~Subscriber();

public:
  bool init(std::string channel,FuncHandle fuc,Handler &handle);
};


