#ifndef ED25519_KEYPAIR_H_
#define ED25519_KEYPAIR_H_

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "ED25519PublicKey.h"

namespace Hedera
{
class ED25519Keypair
{
public:
  ED25519Keypair();
  ~ED25519Keypair();

private:
  EVP_PKEY* keypair;
};
}

#endif // ED25519_KEYPAIR_H_
