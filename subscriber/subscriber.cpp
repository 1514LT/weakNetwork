#include "subscriber.hpp"
#include "publisher/publisher.hpp"

Subscriber::Subscriber()
{
}

Subscriber::~Subscriber()
{
}

bool Subscriber::init(std::string channel,FuncHandle fuc,Handler &handle)
{
  std::shared_ptr<lcm::LCM> lcm(new lcm::LCM);
  if(!lcm->good())
  {
    return -1;
  }
  std::thread(([&]{
    lcm->subscribe(channel,fuc,&handle);
    while(0 == lcm->handle());
  })).join();

  return true;
}