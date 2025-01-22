#include "rsa.hpp"

Rsa::Rsa()
{
  std::string savePath = JRLC::getHomePath() + "/JRLC/weakNetwork/RSA";
  JRLC::check_and_create_directory(savePath);
  std::string filePublic = savePath + "/public_key.pem";
  std::string filePrivate  = savePath + "/private_key.pem";

  // EVP_PKEY* pkey = generateKeyPair(8192);
  
  // savePublicKey(pkey, filePublic.data());
  // savePrivateKey(pkey, filePrivate.data());

  setPublicKey(loadPublicKey(filePublic.data()));
  setPrivateKey(loadPrivateKey(filePrivate.data()));

}

Rsa::Rsa(std::string savePath)
{
  JRLC::check_and_create_directory(savePath);
  std::string filePublic = savePath + "/public_key.pem";
  std::string filePrivate  = savePath + "/private_key.pem";

  EVP_PKEY* pkey = generateKeyPair(4096);
  
  savePublicKey(pkey, filePublic.data());
  savePrivateKey(pkey, filePrivate.data());

  setPublicKey(loadPublicKey(filePublic.data()));
  setPrivateKey(loadPrivateKey(filePrivate.data()));
}

Rsa::~Rsa()
{
  if(!m_publicKey)
  {
    EVP_PKEY_free(m_publicKey);
  }
  if(!m_privateKey)
  {
    EVP_PKEY_free(m_privateKey);
  }
}

void Rsa::setPublicKey(EVP_PKEY* publicKey)
{
  m_publicKey = publicKey;
}

void Rsa::setPrivateKey(EVP_PKEY* privateKey)
{
  m_privateKey = privateKey;
}

EVP_PKEY* Rsa::getPublicKey()
{
  return m_publicKey;
}
EVP_PKEY* Rsa::getPrivateKey()
{
  return m_privateKey;
}

void Rsa::handleErrors()
{
  printf("---------->erro\n");
  ERR_print_errors_fp(stderr);
  abort();
}

EVP_PKEY* Rsa::generateKeyPair(int bits)
{
  EVP_PKEY* pkey = nullptr;
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

  if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0)
  {
    handleErrors();
  }


  if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0)
  {
    handleErrors();
  }

  if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
  {
    handleErrors();
  }

  EVP_PKEY_CTX_free(ctx);
  return pkey;
}

void Rsa::savePrivateKey(EVP_PKEY* pkey, const char* filename)
{
  FILE* fp = fopen(filename, "wb");
  if (!fp)
  {
    handleErrors();
  }
  if (!PEM_write_PrivateKey(fp, pkey, nullptr, nullptr, 0, nullptr, nullptr))
  {
    handleErrors();
  }
  fclose(fp);
}

void Rsa::savePublicKey(EVP_PKEY* pkey, const char* filename)
{
  FILE* fp = fopen(filename, "wb");
  if (!fp)
  {
    handleErrors();
  }
  if (!PEM_write_PUBKEY(fp, pkey))
  {
    handleErrors();
  }
  fclose(fp);
}

EVP_PKEY* Rsa::loadPrivateKey(const char* filename)
{
  FILE* fp = fopen(filename, "rb");
  if (!fp)
  {
    handleErrors();
  }
  EVP_PKEY* pkey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
  fclose(fp);
  if (!pkey)
  {
    handleErrors();
  }
  return pkey;
}

EVP_PKEY* Rsa::loadPublicKey(const char* filename)
{
  FILE* fp = fopen(filename, "rb");
  if (!fp)
  {
    handleErrors();
  }
  EVP_PKEY* pkey = PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
  fclose(fp);
  if (!pkey)
  {
    handleErrors();
  }
  return pkey;
}

std::string Rsa::encryptMessage(EVP_PKEY* publicKey, const std::string& message)
{
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(publicKey, nullptr);
  if (!ctx)
  {
    handleErrors();
  }

  if (EVP_PKEY_encrypt_init(ctx) <= 0)
  {
    handleErrors();
  }

  size_t outlen;
  if (EVP_PKEY_encrypt(ctx, nullptr, &outlen, (unsigned char*)message.c_str(), message.size()) <= 0)
  {
    handleErrors();
  }

  std::vector<unsigned char> encrypted(outlen);
  if (EVP_PKEY_encrypt(ctx, encrypted.data(), &outlen, (unsigned char*)message.c_str(), message.size()) <= 0)
  {
    handleErrors();
  }

  EVP_PKEY_CTX_free(ctx);
  return std::string((char*)encrypted.data(), outlen);
}

std::string Rsa::decryptMessage(EVP_PKEY* privateKey, const std::string& encryptedMessage)
{
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privateKey, nullptr);
  if (!ctx)
  {
    handleErrors();
  }
  if (EVP_PKEY_decrypt_init(ctx) <= 0)
  {
    handleErrors();
  }

  size_t outlen;
  if (EVP_PKEY_decrypt(ctx, nullptr, &outlen, (unsigned char*)encryptedMessage.c_str(), encryptedMessage.size()) <= 0)
  {
    handleErrors();
  }

  std::vector<unsigned char> decrypted(outlen);
  if (EVP_PKEY_decrypt(ctx, decrypted.data(), &outlen, (unsigned char*)encryptedMessage.c_str(), encryptedMessage.size()) <= 0)
  {
    handleErrors();
  }

  EVP_PKEY_CTX_free(ctx);
  return std::string((char*)decrypted.data(), outlen);
}