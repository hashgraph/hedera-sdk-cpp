#include "ED25519Keypair.h"
#include <iostream>

#include "helper/HexConverter.h"

namespace Hedera
{
ED25519Keypair::ED25519Keypair()
{
  this->keypair = nullptr;

  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext) {
    std::cout << "CONTEXT INVALID" << std::endl;
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0) {
    std::cout << "INIT ERROR" << std::endl;
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &this->keypair) <= 0) {
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
}
