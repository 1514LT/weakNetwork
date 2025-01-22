#ifndef AES_HPP
#define AES_HPP
#include "handler/handler.hpp"
#include <stdexcept>
#include "data/data.hpp"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/pem.h"
#include "openssl/rand.h"
class Aes
{
private:
  std::vector<unsigned char> m_key;
  std::vector<unsigned char> m_vi;
public:
  Aes();
  Aes(std::string fileName);
  ~Aes();

public:
  std::vector<unsigned char> getKey();
  std::vector<unsigned char> getVi();
  void setKey(std::vector<unsigned char> key);
  void setVi(std::vector<unsigned char> vi);
public:
  void handleErrors();
  std::vector<unsigned char> generateAESKey();
  std::vector<unsigned char> generateIV();
  std::vector<unsigned char> encryptAES(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv);
  std::vector<unsigned char> decryptAES(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv);
  void saveKeyAndIV(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv, const std::string& filename);
  void loadKeyAndIV(std::vector<unsigned char>& key, std::vector<unsigned char>& iv, const std::string& filename);
};




#endif