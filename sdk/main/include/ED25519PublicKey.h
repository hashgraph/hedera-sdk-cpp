#ifndef ED25519_PUBLIC_KEY_H_
#define ED25519_PUBLIC_KEY_H_

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "PublicKey.h"

namespace Hedera
{
class ED25519PublicKey : public PublicKey
{
public:
  explicit ED25519PublicKey(unsigned char* encodedPublicKey, size_t keyLength);

  ~ED25519PublicKey();

  [[nodiscard]] proto::Key* toProtobuf() const override;
  [[nodiscard]] std::string toString() const override;

private:
  EVP_PKEY* publicKey;
};
}

#endif // ED25519_PUBLIC_KEY_H_
