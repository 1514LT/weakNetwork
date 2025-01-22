#include "data.hpp"

Data::Data()
{
}

Data::~Data()
{
}

void Data::printBinary(const std::string& str) {
  for (char c : str) {
      std::bitset<8> binary(c); // 将字符转换为8位二进制
      std::cout << binary << " "; // 打印二进制值
  }
  std::cout << std::endl; // 换行
}

void compressData(const std::string &input, std::string &output) {
    uLongf compressedSize = compressBound(input.size());
    output.resize(compressedSize);
    if (compress(reinterpret_cast<Bytef *>(&output[0]), &compressedSize,
                 reinterpret_cast<const Bytef *>(input.data()), input.size()) == Z_OK) {
        output.resize(compressedSize); 
    } else {
        throw std::runtime_error("Compression failed");
    }
}



Data::Data(std::string jsonBuff,std::string payloadBuff)
{
  
  std::shared_ptr<data::stu> data(new data::stu);
  printf("start AES\n");
  // AES加密
  Aes aes;
  std::vector<unsigned char> vt_plainJsonBuff(jsonBuff.begin(),jsonBuff.end());
  std::vector<unsigned char> vt_encrypteJsonBuff = aes.encryptAES(vt_plainJsonBuff,aes.getKey(),aes.getVi());
  std::string encrypteJsonBuff(vt_encrypteJsonBuff.begin(),vt_encrypteJsonBuff.end());
  std::cout << "加密成功，密文长度: " << encrypteJsonBuff.size() << std::endl;

  std::vector<unsigned char> vt_palnPayloadBuff(payloadBuff.begin(),payloadBuff.end());
  std::vector<unsigned char> vt_encryptePayloadBuff = aes.encryptAES(vt_palnPayloadBuff,aes.getKey(),aes.getVi());
  std::string encryptePayloadBuff(vt_encryptePayloadBuff.begin(),vt_encryptePayloadBuff.end());
  std::cout << "加密成功，密文长度: " << encryptePayloadBuff.size() << std::endl;

  std::vector<unsigned char> key = aes.getKey();
  std::vector<unsigned char> vi = aes.getVi();
  std::string aes_key(key.begin(),key.end());
  std::string aes_vi(vi.begin(),vi.end());

  // 压缩
  std::string strCompressJsonBuff;
  snappy::Compress(encrypteJsonBuff.data(),encrypteJsonBuff.size(),&strCompressJsonBuff);

  std::string strCompressPayloadBuff;
  snappy::Compress(encryptePayloadBuff.data(),encryptePayloadBuff.size(),&strCompressPayloadBuff);

  data->pack.jsonSize = strCompressJsonBuff.size();
  data->pack.payloadSize = strCompressPayloadBuff.size();

  memcpy((char*)data->pack.jsonBuff,strCompressJsonBuff.c_str(),strCompressJsonBuff.size());
  memcpy((char*)data->pack.payloadBuff,strCompressPayloadBuff.c_str(),strCompressPayloadBuff.size());
  memcpy((char*)data->key,aes_key.c_str(),aes_key.size());
  memcpy((char*)data->vi,aes_vi.c_str(),aes_vi.size());

  m_data = data;
}
Data::Data(int32_t id,std::string ip)
{
  std::shared_ptr<data::stu> data(new data::stu);
  data->id = id;
  data->ip = ip;
  m_data = data;
}

std::shared_ptr<data::stu> Data::getInterface()
{
  return m_data;
}