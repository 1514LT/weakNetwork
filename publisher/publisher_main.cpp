
#include "publisher.hpp"
#include "data/data.hpp"
int main(int argc, char const *argv[])
{
  std::shared_ptr<lcm::LCM> lcm(new lcm::LCM);
  if(!lcm->good())
  {
    return -1;
  }
  std::string jsonFlie = JRLC::read_file_contents("/home/lt/dataacquisition/config/dependency.json");
  std::shared_ptr<Data> data(new Data(jsonFlie,"111"));
  std::shared_ptr<Pub> pub(new Pub);
  pub->init(lcm,123,"192.168.5.165");
  pub->PublishMsg(lcm,"data",data);
  return 0;
}
