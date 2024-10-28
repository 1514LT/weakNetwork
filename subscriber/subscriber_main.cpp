#include "handler/handler.hpp"
#include "subscriber.hpp"
int main(int argc, char const *argv[])
{
  Handler handle;
  Subscriber sub;
  sub.init("data",&Handler::handleMessage,handle);
  return 0;
}
