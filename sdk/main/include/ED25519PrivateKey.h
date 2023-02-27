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
#include "PrivateKey.h"
#include "impl/OpenSSLUtils.h"

#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <string>
#include <string_view>
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
   * Copy constructor and copy assignment operator can throw OpenSSLException if OpenSSL is unable to serialize the
   * input key to copy.
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
   * @throws OpenSSLException If OpenSSL fails to generate a key.
   */
  static std::unique_ptr<ED25519PrivateKey> generatePrivateKey();

  /**
   * Construct an ED25519PrivateKey object from the DER string representation.
   *
   * @param keyString The string from which to create an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input string.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input keyString.
   */
  static std::unique_ptr<ED25519PrivateKey> fromString(std::string_view keyString);

  /**
   * Derive an ED25519PrivateKey from a seed array.
   *
   * @param seed The array seed from which to derive the ED25519PrivateKey.
   * @return A pointer to the derived ED25519PrivateKey.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input seed bytes.
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
   * @throws OpenSSLException If OpenSSL is unable to generate a signature.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Derived from PrivateKey. Get the string representation of this ED25519PrivateKey, in DER format.
   *
   * @return The string representation of this ED25519PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to encode this key to a DER-encoded string.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derive a child ED25519PrivateKey from this ED25519PrivateKey.
   *
   * @param childIndex The unhardened index of the child to derive.
   * @return A pointer to the derived ED25519PrivateKey child.
   * @throws OpenSSLException       If OpenSSL is unable to derive a key with the given childIndex.
   * @throws UninitializedException If this ED25519PrivateKey was not initialized with a chain code.
   * @throws std::invalid_argument  If the input index is hardened.
   */
  [[nodiscard]] std::unique_ptr<ED25519PrivateKey> derive(uint32_t childIndex) const;

  /**
   * Get the byte representation of this ED25519PrivateKey.
   *
   * @return The byte representation of this ED25519PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to decode this key to a byte array.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

private:
  /**
   * Create a wrapped OpenSSL keypair object from a byte vector representing an ED25519PrivateKey.
   *
   * @param keyBytes The bytes representing a ED25519PrivateKey.
   * @return The newly created wrapped OpenSSL keypair object.
   * @throws OpenSSLException If OpenSSL is unable to generate an ED25519 keypair from the input bytes.
   */
  static internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(std::vector<unsigned char> keyBytes);

  /**
   * Construct from a wrapped OpenSSL keypair object.
   *
   * @param keypair The wrapped OpenSSL keypair object from which to construct this ED25519PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to get this ED25519PrivateKey's corresponding ED25519PublicKey's
   *                          bytes.
   */
  explicit ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair);

  /**
   * Construct from a wrapped OpenSSL keypair object and a chaincode.
   *
   * @param keypair   The wrapped OpenSSL keypair object.
   * @param chainCode The new ED25519PrivateKey's chain code.
   * @throws OpenSSLException If OpenSSL is unable to get this ED25519PrivateKey's corresponding ED25519PublicKey's
   *                          bytes.
   */
  ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair, std::vector<unsigned char>&& chainCode);

  /**
   * Get the byte representation of the ED25519PublicKey that corresponds to this ED25519PrivateKey.
   *
   * @return The byte representation of the corresponding ED25519PublicKey.
   * @throws OpenSSLException If OpenSSL is unable to get this ED25519PrivateKey's corresponding ED25519PublicKey's
   *                          bytes.
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * The wrapped OpenSSL keypair object.
   */
  internal::OpenSSLUtils::EVP_PKEY mKeypair;

  /**
   * A pointer to the ED25519PublicKey object that corresponds to this ED25519PrivateKey.
   */
  std::shared_ptr<ED25519PublicKey> mPublicKey = nullptr;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ED25519_PRIVATE_KEY_H_
