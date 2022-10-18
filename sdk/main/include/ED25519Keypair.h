#ifndef ED25519_KEYPAIR_H_
#define ED25519_KEYPAIR_H_

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "ED25519PublicKey.h"
#include "Keypair.h"

namespace Hedera
{
class ED25519Keypair : public Keypair
{
public:
  ED25519Keypair();
  ~ED25519Keypair();

  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;
private:
  EVP_PKEY* keypair;

  std::shared_ptr<ED25519PublicKey> publicKey;
};
}

#endif // ED25519_KEYPAIR_H_
