#include "ED25519PublicKey.h"

#include "helper/HexConverter.h"
#include "openssl/x509.h"

#include <iostream>
#include <openssl/err.h>

namespace Hedera
{
ED25519PublicKey::ED25519PublicKey(unsigned char* encodedPublicKey, size_t keyLength)
  : PublicKey()
{
  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext)
  {
    std::cout << "CONTEXT INVALID" << std::endl;
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0)
  {
    std::cout << "INIT ERROR" << std::endl;
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &this->publicKey) <= 0)
  {
    std::cout << "GENERATE ERROR" << std::endl;
  }

  EVP_PKEY_CTX_free(keyAlgorithmContext);

  EVP_PKEY_set1_encoded_public_key(this->publicKey, encodedPublicKey, keyLength);
}

ED25519PublicKey::~ED25519PublicKey()
{
  EVP_PKEY_free(this->publicKey);
}

proto::Key* ED25519PublicKey::toProtobuf() const
{
  return nullptr;
}

std::string ED25519PublicKey::toString() const
{
  unsigned char* derBytes; // TODO do we need to delete this manually?
  int bytesLength;

  bytesLength = i2d_PUBKEY(this->publicKey, &derBytes);

  if (bytesLength <=0) {
    std::cout << "I2D error" << std::endl;
  }

  return HexConverter::bytesToHex(derBytes, bytesLength);
}
}
