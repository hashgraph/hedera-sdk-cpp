#include "ED25519PublicKey.h"

namespace Hedera
{
ED25519PublicKey::ED25519PublicKey(unsigned char* encodedPublicKey) : PublicKey()
{
  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);

  this->publicKey = EVP_PKEY_new();

  // EVP_PKEY_set1_encoded_public_key(this->publicKey, encodedPublicKey, )
}

ED25519PublicKey::~ED25519PublicKey()
{
  EVP_PKEY_free(this->publicKey);
}

proto::Key* ED25519PublicKey::toProtobuf() const
{
  return nullptr;
}
}
