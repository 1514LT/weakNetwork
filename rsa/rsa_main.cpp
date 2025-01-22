#include "rsa.hpp"

int main(int argc, char const *argv[])
{
  Rsa rsa("/home/lt/JRLC/weakNetwork/RSA");
  std::string message = "Hello, this is a secret message!";
  std::cout << "原始消息: " << message << "大小:" << message.size() << std::endl;

  std::string encryptedMessage = rsa.encryptMessage(rsa.getPublicKey(),message);
  std::cout << "加密后消息大小: " << encryptedMessage.size() << std::endl;

  std::string deencryptedMessage = rsa.decryptMessage(rsa.getPrivateKey(),encryptedMessage);
  std::cout << "解密后消息: " << deencryptedMessage << std::endl;
  return 0;
}
