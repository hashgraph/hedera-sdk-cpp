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
  ED25519PrivateKey(const ED25519PrivateKey& other);

  ~ED25519PrivateKey();

  static std::shared_ptr<ED25519PrivateKey> generatePrivateKey();
  static std::shared_ptr<ED25519PrivateKey> fromString(const std::string& keyString);

  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;
  [[nodiscard]] std::string toString() const override;

private:
  EVP_PKEY* keypair;

  std::shared_ptr<ED25519PublicKey> publicKey;

  explicit ED25519PrivateKey(EVP_PKEY* keypair);

  [[nodiscard]] std::vector<unsigned char> toBytes() const;
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);
};
}

#endif // ED25519_PRIVATE_KEY_H_
