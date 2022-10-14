#include "ED25519Keypair.h"

namespace Hedera
{
ED25519Keypair::ED25519Keypair()
{
  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);

  EVP_PKEY_keygen_init(keyAlgorithmContext);
  EVP_PKEY_keygen(keyAlgorithmContext, &this->keypair);
  EVP_PKEY_CTX_free(keyAlgorithmContext);
}

ED25519Keypair::~ED25519Keypair()
{
  EVP_PKEY_free(this->keypair);
}
}
