#ifndef RSA_HPP
#define RSA_HPP
#include "data/data.hpp"
#include "include/lcm/lcm-cpp.hpp"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/pem.h"

#include <iostream>
#include <memory>
#include <unordered_set>
class Rsa
{
private:
  EVP_PKEY* m_publicKey;
  EVP_PKEY* m_privateKey;
public:
  Rsa();
  Rsa(std::string savePath);
  ~Rsa();

public:
  void setPublicKey(EVP_PKEY* publicKey);
  void setPrivateKey(EVP_PKEY* privateKey);
  EVP_PKEY* getPublicKey();
  EVP_PKEY* getPrivateKey();
public:
  void handleErrors();
  EVP_PKEY* generateKeyPair(int bits);
  void savePrivateKey(EVP_PKEY* pkey, const char* filename);
  void savePublicKey(EVP_PKEY* pkey, const char* filename);
  EVP_PKEY* loadPrivateKey(const char* filename);
  EVP_PKEY* loadPublicKey(const char* filename);
  std::string encryptMessage(EVP_PKEY* publicKey, const std::string& message);
  std::string decryptMessage(EVP_PKEY* privateKey, const std::string& encryptedMessage);
};




#endif