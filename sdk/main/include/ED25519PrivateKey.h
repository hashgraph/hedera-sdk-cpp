#ifndef ED25519_PRIVATE_KEY_H_
#define ED25519_PRIVATE_KEY_H_

#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "ED25519PublicKey.h"
#include "PrivateKey.h"

namespace Hedera
{
class ED25519PrivateKey : public PrivateKey
{
public:
  explicit ED25519PrivateKey(EVP_PKEY* keypair);

  static std::shared_ptr<ED25519PrivateKey> generatePrivateKey();
  static std::shared_ptr<ED25519PrivateKey> fromDEREncoding(const std::string& derEncodedKey);

  ~ED25519PrivateKey();

  ED25519PrivateKey(const ED25519PrivateKey&) = delete;

  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;
  [[nodiscard]] std::string toString() const override;

private:
  EVP_PKEY* keypair;

  std::shared_ptr<ED25519PublicKey> publicKey;
};
}

#endif // ED25519_PRIVATE_KEY_H_
