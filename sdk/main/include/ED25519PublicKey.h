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
  ED25519PublicKey() = delete;
  ED25519PublicKey(const ED25519PublicKey& other);
  ~ED25519PublicKey() override;

  static std::shared_ptr<ED25519PublicKey> fromString(const std::string& keyString);
  static std::shared_ptr<ED25519PublicKey> fromBytes(const std::vector<unsigned char>& keyBytes);

  [[nodiscard]] proto::Key* toProtobuf() const override;
  [[nodiscard]] std::string toString() const override;

  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

private:
  EVP_PKEY* publicKey;

  explicit ED25519PublicKey(EVP_PKEY* publicKey);
  [[nodiscard]] std::vector<unsigned char> toBytes() const;
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);
};
}

#endif // ED25519_PUBLIC_KEY_H_
