#ifndef ED25519_KEYPAIR_H_
#define ED25519_KEYPAIR_H_

#include <memory>
#include <sodium.h>

#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"

namespace Hedera
{
class ED25519Keypair
{
public:
  ED25519Keypair();

private:
  std::unique_ptr<ED25519PrivateKey> privateKey;
  std::unique_ptr<ED25519PublicKey> publicKey;
};
}

#endif // ED25519_KEYPAIR_H_
