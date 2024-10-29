#include "handler/handler.hpp"
#include "subscriber/subscriber.hpp"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/pem.h"
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

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

EVP_PKEY* generateKeyPair(int bits) {
    EVP_PKEY* pkey = nullptr;
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0) {
        handleErrors();
    }

    // 设置密钥长度
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        handleErrors();
    }

    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

void savePrivateKey(EVP_PKEY* pkey, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        handleErrors();
    }
    if (!PEM_write_PrivateKey(fp, pkey, nullptr, nullptr, 0, nullptr, nullptr)) {
        handleErrors();
    }
    fclose(fp);
}

void savePublicKey(EVP_PKEY* pkey, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        handleErrors();
    }
    if (!PEM_write_PUBKEY(fp, pkey)) {
        handleErrors();
    }
    fclose(fp);
}

std::string encryptMessage(EVP_PKEY* publicKey, const std::string& message) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(publicKey, nullptr);
    if (!ctx) {
        handleErrors();
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        handleErrors();
    }

    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, nullptr, &outlen, (unsigned char*)message.c_str(), message.size()) <= 0) {
        handleErrors();
    }

    std::vector<unsigned char> encrypted(outlen);
    if (EVP_PKEY_encrypt(ctx, encrypted.data(), &outlen, (unsigned char*)message.c_str(), message.size()) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(ctx);
    return std::string((char*)encrypted.data(), outlen);
}

std::string decryptMessage(EVP_PKEY* privateKey, const std::string& encryptedMessage) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privateKey, nullptr);
    if (!ctx) {
        handleErrors();
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        handleErrors();
    }

    size_t outlen;
    if (EVP_PKEY_decrypt(ctx, nullptr, &outlen, (unsigned char*)encryptedMessage.c_str(), encryptedMessage.size()) <= 0) {
        handleErrors();
    }

    std::vector<unsigned char> decrypted(outlen);
    if (EVP_PKEY_decrypt(ctx, decrypted.data(), &outlen, (unsigned char*)encryptedMessage.c_str(), encryptedMessage.size()) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(ctx);
    return std::string((char*)decrypted.data(), outlen);
}


void test1()
{
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
}

void test2()
{
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
}


void test3()
{
// 生成 RSA 密钥对
    EVP_PKEY* pkey = generateKeyPair(2048);
    savePrivateKey(pkey, "private_key.pem");
    savePublicKey(pkey, "public_key.pem");

    std::string message = "Hello, this is a secret message!";
    std::cout << "原始消息: " << message << std::endl;

    // 使用公钥加密消息
    std::string encryptedMessage = encryptMessage(pkey, message);
    std::cout << "加密后消息大小: " << encryptedMessage.size() << std::endl;

    // 使用私钥解密消息
    std::string decryptedMessage = decryptMessage(pkey, encryptedMessage);
    std::cout << "解密后消息: " << decryptedMessage << std::endl;

    // 清理
    EVP_PKEY_free(pkey);
}

int main(int argc, char const *argv[])
{
  test3();
  return 0;
}
