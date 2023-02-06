/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef HEDERA_SDK_CPP_ED25519_PRIVATE_KEY_H_
#define HEDERA_SDK_CPP_ED25519_PRIVATE_KEY_H_

#include "ED25519PublicKey.h"
#include "MnemonicBIP39.h"
#include "PrivateKey.h"
#include "impl/OpenSSLObjectWrapper.h"

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ED25519 private key.
 */
class ED25519PrivateKey : public PrivateKey
{
public:
  /**
   * Disallow default construction of an ED25519PrivateKey, as an uninitialized ED25519PrivateKey provides no
   * functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ED25519PrivateKey() = delete;

  /**
   * Copy constructor and copy assignment operator can throw std::runtime_error if OpenSSL serialization fails.
   */
  ~ED25519PrivateKey() override = default;
  ED25519PrivateKey(const ED25519PrivateKey& other);
  ED25519PrivateKey& operator=(const ED25519PrivateKey& other);
  ED25519PrivateKey(ED25519PrivateKey&& other) noexcept;
  ED25519PrivateKey& operator=(ED25519PrivateKey&& other) noexcept;

  /**
   * Generate a new ED25519PrivateKey.
   *
   * @return A pointer to the generated ED25519PrivateKey.
   * @throws std::runtime_error If OpenSSL keypair initialization fails.
   */
  static std::unique_ptr<ED25519PrivateKey> generatePrivateKey();

  /**
   * Construct an ED25519PrivateKey object from the DER string representation.
   *
   * @param keyString The string from which to create an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input string.
   * @throws std::runtime_error If OpenSSL deserialization of keyString fails.
   */
  static std::unique_ptr<ED25519PrivateKey> fromString(const std::string& keyString);

  /**
   * Derive an ED25519PrivateKey from a seed array.
   *
   * @param seed The array seed from which to derive the ED25519PrivateKey.
   * @return A pointer to the derived ED25519PrivateKey.
   */
  static std::unique_ptr<ED25519PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ED25519PrivateKey object.
   *
   * @return A pointer to the created clone of this ED25519PrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Get the ED25519PublicKey that corresponds to this ED25519PrivateKey.
   *
   * @return A pointer to the ED25519PublicKey that corresponds to this ED25519PrivateKey.
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;

  /**
   * Derived from PrivateKey. Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   * @throws std::runtime_error If OpenSSL signature generation fails.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Derived from PrivateKey. Get the string representation of this ED25519PrivateKey, in DER format.
   *
   * @return The string representation of this ED25519PrivateKey.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derive a child ED25519PrivateKey from this ED25519PrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived ED25519PrivateKey child.
   * @throws std::runtime_error If an invalid index is provided, or this key doesn't support child key derivation.
   */
  [[nodiscard]] std::unique_ptr<ED25519PrivateKey> derive(uint32_t childIndex) const;

  /**
   * Get the byte representation of this ED25519PrivateKey.
   *
   * @return The byte representation of this ED25519PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

  /**
   * Get this ED25519PrivateKey's chain code. It is possible that the chain code could be empty.
   *
   * @return This ED25519PrivateKey's chaincode if it exists, otherwise an empty vector.
   */
  [[nodiscard]] std::vector<unsigned char> getChainCode() const;

private:
  /**
   * Create a wrapped OpenSSL keypair object from a byte vector representing an ED25519PrivateKey.
   *
   * @param keyBytes The bytes representing a ED25519PrivateKey.
   * @return The newly created wrapped OpenSSL keypair object.
   */
  static internal::OpenSSL_EVP_PKEY bytesToPKEY(const std::vector<unsigned char>& keyBytes);

  /**
   * Prepend an ED25519PrivateKey's algorithm identifier to an array of serialized ED25519PrivateKey bytes.
   *
   * @param keyBytes The bytes representing an ED25519PrivateKey.
   * @return An array of bytes that contains the ED25519PrivateKey's algorithm identifier bytes, followed by the input
   *         ED25519PrivateKey bytes.
   */
  static std::vector<unsigned char> prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes);

  /**
   * Create an ED25519PrivateKey from the output of an HMAC hash function.
   *
   * @param hmacOutput The output of the HMAC hash function from which to create an ED25519PrivateKey.
   * @return A pointer to the new ED25519PrivateKey, with key material and chain code set from the hash output.
   */
  static std::unique_ptr<ED25519PrivateKey> fromHMACOutput(const std::vector<unsigned char>& hmacOutput);

  /**
   * Construct from a wrapped OpenSSL keypair object.
   *
   * @param keypair The wrapped OpenSSL keypair object from which to construct this ED25519PrivateKey.
   */
  explicit ED25519PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair);

  /**
   * Construct from a wrapped OpenSSL keypair object and a chaincode.
   *
   * @param keypair   The wrapped OpenSSL keypair object.
   * @param chainCode The new ED25519PrivateKey's chain code.
   */
  ED25519PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair, std::vector<unsigned char> chainCode);

  /**
   * Get the byte representation of the ED25519PublicKey that corresponds to this ED25519PrivateKey.
   *
   * @return The byte representation of the corresponding ED25519PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * The wrapped OpenSSL keypair object.
   */
  internal::OpenSSL_EVP_PKEY mKeypair;

  /**
   * A pointer to the ED25519PublicKey object that corresponds to this ED25519PrivateKey.
   */
  std::shared_ptr<ED25519PublicKey> mPublicKey = nullptr;

  /**
   * This ED25519PrivateKey's chain code. If this is empty, then this ED25519PrivateKey will not support derivation.
   */
  std::vector<unsigned char> mChainCode;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ED25519_PRIVATE_KEY_H_
