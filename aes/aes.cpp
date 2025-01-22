#include "aes.hpp"

Aes::Aes()
{
  std::string fileName = JRLC::getHomePath() + "/JRLC/weakNetwork/AES/aes_key_vi.data";
  loadKeyAndIV(m_key,m_vi,fileName);
  for(auto tmp:m_key)
  {
    std::cout << tmp;
  }
}
Aes::Aes(std::string filePath)
{
  JRLC::check_and_create_directory(filePath);
  std::string fileName = filePath + "/aes_key_vi.data";
  m_key = generateAESKey();
  m_vi = generateIV();
  saveKeyAndIV(m_key,m_vi,fileName);
}

Aes::~Aes()
{
}

std::vector<unsigned char> Aes::getKey()
{
  return m_key;
}
std::vector<unsigned char> Aes::getVi()
{
  return m_vi;
}
void Aes::setKey(std::vector<unsigned char> key)
{
  m_key = key;
}
void Aes::setVi(std::vector<unsigned char> vi)
{
  m_vi = vi;
}

// 错误处理
void Aes::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// 生成 AES 密钥
std::vector<unsigned char> Aes::generateAESKey() {
    int key_size = 256;
    std::vector<unsigned char> key(key_size / 8);
    if (!RAND_bytes(key.data(), key.size())) {
        handleErrors();
    }
    return key;
}

// 生成初始化向量 (IV)
std::vector<unsigned char> Aes::generateIV() {
    int iv_size = 16;
    std::vector<unsigned char> iv(iv_size);
    if (!RAND_bytes(iv.data(), iv.size())) {
        handleErrors();
    }
    return iv;
}

// AES 加密
std::vector<unsigned char> Aes::encryptAES(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    int len;
    int ciphertext_len;
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) handleErrors();
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1) handleErrors();
    ciphertext_len = len;
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// AES 解密
std::vector<unsigned char> Aes::decryptAES(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    int len;
    int plaintext_len;
    std::vector<unsigned char> plaintext(ciphertext.size());

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) handleErrors();
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) handleErrors();
    plaintext_len = len;
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return plaintext;
}

// 保存 AES 密钥和 IV 到文件
void Aes::saveKeyAndIV(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        handleErrors();
    }

    ofs.write(reinterpret_cast<const char*>(key.data()), key.size());

    ofs.write(reinterpret_cast<const char*>(iv.data()), iv.size());

    ofs.close();
}

// 从文件读取 AES 密钥和 IV
// void Aes::loadKeyAndIV(std::vector<unsigned char>& key, std::vector<unsigned char>& iv, const std::string& filename) {
//     std::ifstream ifs(filename, std::ios::binary);
//     if (!ifs) {
//         handleErrors(); 
//     }

//     key.resize(32); // 256位密钥
//     iv.resize(16);  // 128位IV

//     ifs.read(reinterpret_cast<char*>(key.data()), key.size());

//     ifs.read(reinterpret_cast<char*>(iv.data()), iv.size());

//     ifs.close();
// }

void Aes::loadKeyAndIV(std::vector<unsigned char>& key, std::vector<unsigned char>& iv, const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        handleErrors(); 
    }

    key.resize(32); // 256位密钥
    iv.resize(16);  // 128位IV
    
    if (ifs.read(reinterpret_cast<char*>(key.data()), key.size()).gcount() != key.size()) {
        std::cerr << "Error: Failed to read key from file." << std::endl;
        abort();
    }

    if (ifs.read(reinterpret_cast<char*>(iv.data()), iv.size()).gcount() != iv.size()) {
        std::cerr << "Error: Failed to read IV from file." << std::endl;
        abort();
    }

    ifs.close();
}