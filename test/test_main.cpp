#include "handler/handler.hpp"
#include "subscriber/subscriber.hpp"
void compressData(const std::string &input, std::string &output) {
    uLongf compressedSize = compressBound(input.size());
    output.resize(compressedSize);
    if (compress(reinterpret_cast<Bytef *>(&output[0]), &compressedSize,
                 reinterpret_cast<const Bytef *>(input.data()), input.size()) == Z_OK) {
        output.resize(compressedSize); // Resize to actual compressed size
    } else {
        throw std::runtime_error("Compression failed");
    }
}

void decompressData(const std::string &input, std::string &output, size_t originalSize) {
    output.resize(originalSize);
    if (uncompress(reinterpret_cast<Bytef *>(&output[0]), &originalSize,
                   reinterpret_cast<const Bytef *>(input.data()), input.size()) != Z_OK) {
        throw std::runtime_error("Decompression failed");
    }
}


int main(int argc, char const *argv[])
{
  #if 0
  std::string jsonFlie = JRLC::read_file_contents("/dataacquisition/profile/HardwareStatus.json");
  std::string compressed;

  // 压缩数据
  snappy::Compress(jsonFlie.data(), jsonFlie.size(), &compressed);
  JRLC::wirte_file_contents(compressed,"compressed");


      // 解压缩数据
    std::string uncompressed;
    std::string buf = JRLC::read_file_contents("compressed");
    if (snappy::Uncompress(buf.data(), buf.size(), &uncompressed)) {
        std::cout << "Uncompressed data: " << uncompressed << std::endl;
    } else {
        std::cerr << "Failed to uncompress data." << std::endl;
    }
  #else
  
    std::string input = "This is the data to be compressed using zlib.";
    std::string compressed;
    std::string decompressed;

    try {
        // Compress data
        compressData(input, compressed);
        std::cout << "Compressed size: " << compressed.size() << std::endl;

        // Decompress data
        decompressData(compressed, decompressed, input.size());
        std::cout << "Decompressed data: " << decompressed << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
  #endif
  return 0;
}
