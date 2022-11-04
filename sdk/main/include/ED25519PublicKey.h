#ifndef ED25519_PUBLIC_KEY_H_
#define ED25519_PUBLIC_KEY_H_

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "PublicKey.h"

#include "helper/HexConverter.h"

namespace Hedera
{
/**
 * An ED25519 public key
 */
class ED25519PublicKey : public PublicKey
{
public:
  /**
   * Delete the default constructor
   */
  ED25519PublicKey() = delete;

  /**
   * Copy constructor
   * @param other the key being copied
   */
  ED25519PublicKey(const ED25519PublicKey& other);

  /**
   * Destructor
   */
  ~ED25519PublicKey() override;

  /**
   * Create a new ED25519 public key from a string
   * @param keyString the DER encoding of a public key
   * @return the new public key
   */
  static std::shared_ptr<ED25519PublicKey> fromString(const std::string& keyString);

  /**
   * Create a new ED25519 public key from byte vector
   * @param keyBytes a vector of bytes, which are a DER encoding of a public key
   * @return the new public key
   */
  static std::shared_ptr<ED25519PublicKey> fromBytes(const std::vector<unsigned char>& keyBytes);

  /**
   * Convert the public key to a protobuf key
   * @return the protobuf key
   */
  [[nodiscard]] proto::Key* toProtobuf() const override;

  /**
   * Get the string representation of this key, in DER format
   * @return the DER string
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Verify that a signature was made by the private key which corresponds to this public key
   * @param signatureBytes
   * the byte vector representing the signature
   * @param signedBytes
   * the bytes which were purportedly signed to create the signature
   * @return true if the signature is valid, otherwise false
   */
  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

private:
  inline static const std::string DER_PREFIX_HEX = "302A300506032B6570032100";
  inline static const std::vector<unsigned char> DER_PREFIX_BYTES = HexConverter::hexToBase64(DER_PREFIX_HEX);

  /**
   * The underlying OpenSSL representation of the key
   */
  EVP_PKEY* publicKey;

  /**
   * Private constructor
   * @param publicKey the OpenSSL object containing the public key information
   */
  explicit ED25519PublicKey(EVP_PKEY* publicKey);

  /**
   * Get the byte representation (DER format) of the public key
   * @return the byte representation of the key
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

  /**
   * Creates a new EVP_PKEY object from a byte vector representing a public key
   * @param keyBytes the bytes representing the public key
   * @return the newly created EVP_PKEY object
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);
};
}

#endif // ED25519_PUBLIC_KEY_H_
