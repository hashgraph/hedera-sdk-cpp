#ifndef ED25519_PUBLIC_KEY_H_
#define ED25519_PUBLIC_KEY_H_

#include <memory>
#include <sodium.h>

namespace Hedera
{
class ED25519PublicKey
{
public:
  ED25519PublicKey(
    const unsigned char publicKeyBytes[crypto_sign_PUBLICKEYBYTES]);

private:
  std::unique_ptr<unsigned char[]> publicKeyBytes;
};
}

#endif // ED25519_PUBLIC_KEY_H_
