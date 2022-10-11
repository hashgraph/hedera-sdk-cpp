#include "ED25519Keypair.h"

namespace Hedera
{
ED25519Keypair::ED25519Keypair()
{
  unsigned char publicKeyBytes[crypto_sign_PUBLICKEYBYTES];
  unsigned char privateKeyBytes[crypto_sign_SECRETKEYBYTES];

  crypto_sign_keypair(publicKeyBytes, privateKeyBytes);

  this->privateKey =
    std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(privateKeyBytes));
  this->publicKey =
    std::make_unique<ED25519PublicKey>(ED25519PublicKey(publicKeyBytes));
}
}
