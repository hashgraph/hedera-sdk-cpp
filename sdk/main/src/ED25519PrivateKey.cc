#include "ED25519PrivateKey.h"

#include <sodium.h>

namespace Hedera
{
ED25519PrivateKey::ED25519PrivateKey(
  const unsigned char privateKeyBytes[crypto_box_SECRETKEYBYTES])
{
  this->privateKeyBytes = std::make_unique<unsigned char[]>(*privateKeyBytes);
}
}
