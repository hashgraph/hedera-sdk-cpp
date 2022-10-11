#ifndef ED25519_PRIVATE_KEY_H_
#define ED25519_PRIVATE_KEY_H_

#include <memory>
#include <sodium.h>

namespace Hedera
{
class ED25519PrivateKey
{
public:
  ED25519PrivateKey(
    const unsigned char privateKeyBytes[crypto_sign_SECRETKEYBYTES]);

private:
  std::unique_ptr<unsigned char[]> privateKeyBytes;
};
}

#endif // ED25519_PRIVATE_KEY_H_
