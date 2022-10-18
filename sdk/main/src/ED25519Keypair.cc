#include "ED25519Keypair.h"
#include <iostream>
#include <vector>

namespace Hedera
{
ED25519Keypair::ED25519Keypair()
{
  this->keypair = nullptr;

  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext)
  {
    std::cout << "CONTEXT INVALID" << std::endl;
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0)
  {
    std::cout << "INIT ERROR" << std::endl;
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &this->keypair) <= 0)
  {
    std::cout << "GENERATE ERROR" << std::endl;
  }

  EVP_PKEY_CTX_free(keyAlgorithmContext);

  unsigned char* encodedPublicKey;
  size_t publicKeyLength;
  publicKeyLength = EVP_PKEY_get1_encoded_public_key(this->keypair, &encodedPublicKey);

  this->publicKey = std::make_shared<ED25519PublicKey>(encodedPublicKey, publicKeyLength);
}

ED25519Keypair::~ED25519Keypair()
{
  EVP_PKEY_free(this->keypair);
}

std::shared_ptr<PublicKey> ED25519Keypair::getPublicKey() const
{
  return publicKey;
}

std::vector<unsigned char> ED25519Keypair::sign(std::vector<unsigned char> bytesToSign) const
{
  size_t signatureLength;
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    std::cout << "Digest context construction failed" << std::endl;
  }

  if (EVP_DigestSignInit(messageDigestContext, nullptr, nullptr, nullptr, this->keypair) <= 0)
  {
    std::cout << "Digest sign init failed" << std::endl;
  }

  /* Calculate the required size for the signature by passing a NULL buffer */
  if (EVP_DigestSign(messageDigestContext, nullptr, &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    std::cout << "Failed to calculate signature length" << std::endl;
  }

  std::vector<unsigned char> signature = std::vector<unsigned char>(signatureLength);

  if (EVP_DigestSign(
        messageDigestContext, &signature.front(), &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    std::cout << "Signing failed" << std::endl;
  }

  EVP_MD_CTX_free(messageDigestContext);

  return signature;
}
}
