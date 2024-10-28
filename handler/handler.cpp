#include "handler.hpp"
#include "include/snappy/snappy.h"
Handler::Handler()
{
}

Handler::~Handler()
{
}


void Handler::handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const data::stu* msg)
{
  printf("Received message on channel \"%s\":\n", chan.c_str());
  std::cout << msg->jsonBuff << "\npayload: " << msg->payloadBuff << std::endl;
}


