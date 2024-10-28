#include "discover.hpp"

Discover::Discover()
{
}

Discover::~Discover()
{
}


bool Discover::init()
{
  std::shared_ptr<lcm::LCM> lcm(new lcm::LCM);
  if(!lcm->good())
  {
    return false;
  }
  lcm->subscribe("discover",&Discover::handleDiscoverMessage,this);
  while(0 == lcm->handle());
  return true;
}

void Discover::handleDiscoverMessage(const lcm::ReceiveBuffer* rbuf,
                            const std::string& chan, 
                            const data::stu* msg)
{
  std::cout << "id:" << msg->id << " ip:" << msg->ip << std::endl;
  if(msg->id == 0)
    return;
  for (auto tmp:m_set_Participants)
  {
    if(tmp->getId() == msg->id)
     return;
  }
  
  std::shared_ptr<Participant> participant(new Participant(msg->id,msg->ip));
  m_set_Participants.insert(participant);

  for (auto tmp:m_set_Participants)
  {
    std::cout << tmp->getId() << std::endl;
  }
}