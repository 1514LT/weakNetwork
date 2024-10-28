#ifndef DISCOVER_HPP
#define DISCOVER_HPP
#include "data/stu.hpp"
#include "include/lcm/lcm-cpp.hpp"
#include <iostream>
#include <memory>
#include <unordered_set>
class Participant
{
private:
  int32_t m_id;
  std::string m_ip;
public:
  Participant(){};
  Participant(int32_t id,std::string ip):m_id(id),m_ip(ip){};
  ~Participant(){};

public:
  int32_t getId()
  {
    return m_id;
  }
  std::string getIP()
  {
    return m_ip;
  }
};



class Discover
{
private:
  std::unordered_set<std::shared_ptr<Participant>> m_set_Participants;
public:
  Discover();
  ~Discover();

public:
  bool init();
  void handleDiscoverMessage(const lcm::ReceiveBuffer* rbuf,
              const std::string& chan, 
              const data::stu* msg);

};



#endif