#include "ED25519PublicKey.h"

#include <sodium.h>

namespace Hedera
{
ED25519PublicKey::ED25519PublicKey(
  const unsigned char publicKeyBytes[crypto_box_PUBLICKEYBYTES])
{
  this->publicKeyBytes = std::make_unique<unsigned char[]>(*publicKeyBytes);
}
}
