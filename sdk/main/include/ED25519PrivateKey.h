#ifndef ED25519_PRIVATE_KEY_H_
#define ED25519_PRIVATE_KEY_H_

#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "ED25519PublicKey.h"
#include "MnemonicBIP39.h"
#include "PrivateKey.h"

namespace Hedera
{
/**
 * An ED25519 private key
 */
class ED25519PrivateKey : public PrivateKey
{
public:
  /**
   * Deleted default constructor
   */
  ED25519PrivateKey() = delete;

  /**
   * Copy constructor
   * @param other the other key being copied
   */
  ED25519PrivateKey(const ED25519PrivateKey& other);

  /**
   * Destructor
   */
  ~ED25519PrivateKey() override;

  /**
   * Generates a new random private key
   * @return the newly generated private key
   */
  static std::unique_ptr<ED25519PrivateKey> generatePrivateKey();

  /**
   * Creates a new private key object from the DER string representation
   * @param keyString the string representation of a private key
   * @return the new private key
   */
  static std::unique_ptr<ED25519PrivateKey> fromString(const std::string& keyString);

  static std::unique_ptr<ED25519PrivateKey> fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                              const std::string& passphrase);

  static std::unique_ptr<ED25519PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Get the public key that corresponds to this private key
   *
   * @return the public key
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;

  /**
   * Sign an arbitrary byte message. Message is hashed before signing
   *
   * @param bytesToSign the bytes to sign
   *
   * @return the signature
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Gets the string representation of the private key, in DER format
   *
   * @return the string representation of the private key
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Gets the byte representation of the private key
   *
   * @return the byte representation of the key
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

  [[nodiscard]] std::vector<unsigned char> getChainCode() const;

  [[nodiscard]] std::unique_ptr<ED25519PrivateKey> derive(uint32_t childIndex) const;

private:
  const inline static std::string ALGORITHM_IDENTIFIER_HEX = "302E020100300506032B657004220420";
  const inline static std::vector<unsigned char> ALGORITHM_IDENTIFIER_BYTES =
    HexConverter::hexToBase64(ALGORITHM_IDENTIFIER_HEX);

  /**
   * The underlying OpenSSL representation of the key
   */
  EVP_PKEY* keypair;

  /**
   * The public key object which corresponds to the private key
   */
  std::shared_ptr<ED25519PublicKey> publicKey;

  std::vector<unsigned char> chainCode = {};

  /**
   * Private constructor
   * @param keypair the underlying OpenSSL object representing the private key
   */
  explicit ED25519PrivateKey(EVP_PKEY* keypair);

  ED25519PrivateKey(EVP_PKEY* keypair, std::vector<unsigned char>  chainCode);

  /**
   * Derives the byte representation of the public key which corresponds to this private key
   * @return the byte representation of the corresponding public key
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * Creates a new EVP_PKEY object from a byte vector representing a private key
   * @param keyBytes the bytes representing the private key
   * @return the newly created EVP_PKEY object
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);

  static std::vector<unsigned char> prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes);
};
}

#endif // ED25519_PRIVATE_KEY_H_
