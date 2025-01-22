#include "handler.hpp"
#include "rsa/rsa.hpp"
#include "include/snappy/snappy.h"
Handler::Handler()
{
}

Handler::~Handler()
{
}

void printBinary(const std::string& str) {
    for (char c : str) {
        std::bitset<8> binary(c); // 将字符转换为8位二进制
        std::cout << binary << " "; // 打印二进制值
    }
    std::cout << std::endl; // 换行
}

void Handler::handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const data::stu* msg)
{
  printf("Received message on channel \"%s\":\n", chan.c_str());

  //解压缩
  std::string decompressJsonBuff;
  std::string decompressPayloadBuff;

  snappy::Uncompress((char*)msg->pack.jsonBuff,msg->pack.jsonSize,&decompressJsonBuff);

  snappy::Uncompress((char*)msg->pack.payloadBuff,msg->pack.payloadSize,&decompressPayloadBuff);

  std::string decrypteKey = (char*)msg->key;
  std::vector<unsigned char> vt_decrypteKey(decrypteKey.begin(),decrypteKey.end());

  std::string decrypteVi = (char*)msg->vi;
  std::vector<unsigned char> vt_decrypteVi(decrypteVi.begin(),decrypteVi.end());
  // AES 解密
  printf("start AES\n");
  Aes aes;
  std::vector<unsigned char> vt_decompressJsonBuff(decompressJsonBuff.begin(),decompressJsonBuff.end());
  std::vector<unsigned char> vt_decompressPayloadBuff(decompressPayloadBuff.begin(),decompressPayloadBuff.end());

  std::vector<unsigned char> vt_decryptJsonBuffaes = aes.decryptAES(vt_decompressJsonBuff,vt_decrypteKey,vt_decrypteVi);
  std::string decryptJsonBuff(vt_decryptJsonBuffaes.begin(),vt_decryptJsonBuffaes.end());

  std::vector<unsigned char> vt_decryptPayloadBuff = aes.decryptAES(vt_decompressPayloadBuff,vt_decrypteKey,vt_decrypteVi);
  std::string decryptPayloadBuff(vt_decryptPayloadBuff.begin(),vt_decryptPayloadBuff.end());

  std::cout << "json: " << decryptJsonBuff << "\npayload: " << decryptPayloadBuff << std::endl;
}


