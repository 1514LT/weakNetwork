#include "data.hpp"

Data::Data()
{
}

Data::~Data()
{
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

  data->jsonBuff = jsonBuff;
  data->jsonSize = jsonBuff.size();

  data->payloadBuff = payloadBuff;
  data->payloadSize = payloadBuff.size();
  data->bodySize = jsonBuff.size() + payloadBuff.size();

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