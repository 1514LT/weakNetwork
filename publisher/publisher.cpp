#include "publisher.hpp"

Pub::Pub()
{

}

Pub::~Pub()
{
}


bool Pub::PublishMsg(std::shared_ptr<lcm::LCM> lcm,std::string channel,std::shared_ptr<Data> data)
{
  lcm->publish(channel,data->getInterface().get());
  return true;
}

void Pub::init(std::shared_ptr<lcm::LCM> lcm,int32_t id,std::string ip)
{
  std::shared_ptr<Data> data(new Data(id,ip));
  PublishMsg(lcm,"discover",data);
  Handler handle;
  Subscriber sub;
  std::thread([&]{
    sub.init("data",&Handler::handleMessage,handle);
  }).detach();
}