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
#ifndef HEDERA_SDK_CPP_ED25519_PUBLIC_KEY_H_
#define HEDERA_SDK_CPP_ED25519_PUBLIC_KEY_H_

#include "PublicKey.h"

#include <openssl/crypto.h>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ED25519 public key.
 */
class ED25519PublicKey : public PublicKey
{
public:
  /**
   * Disallow default construction of an ED25519PublicKey, as an uninitialized ED25519PublicKey provides no
   * functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ED25519PublicKey() = delete;

  /**
   * Copy constructor and copy assignment operator can throw std::runtime_error if OpenSSL serialization fails.
   */
  ~ED25519PublicKey() override;
  ED25519PublicKey(const ED25519PublicKey& other);
  ED25519PublicKey& operator=(const ED25519PublicKey& other);
  ED25519PublicKey(ED25519PublicKey&& other) noexcept;
  ED25519PublicKey& operator=(ED25519PublicKey&& other) noexcept;

  /**
   * Construct an ED25519PublicKey object from the DER string representation.
   *
   * @param keyString The string from which to create an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input string.
   */
  static std::shared_ptr<ED25519PublicKey> fromString(const std::string& keyString);

  /**
   * Construct an ED25519PublicKey object from a byte vector.
   *
   * @param keyBytes The vector of bytes from which to construct the ED25519PublicKey, which are a DER encoding.
   * @return A pointer to an ED25519PublicKey representing the input bytes.
   */
  static std::shared_ptr<ED25519PublicKey> fromBytes(const std::vector<unsigned char>& keyBytes);

  /**
   * Derived from PublicKey. Create a clone of this ED25519PublicKey object.
   *
   * @return A pointer to the created clone of this ED25519PublicKey.
   */
  [[nodiscard]] std::unique_ptr<PublicKey> clone() const override;

  /**
   * Derived from PublicKey. Verify that a signature was made by the ED25519PrivateKey which corresponds to this
   * ED25519PublicKey.
   *
   * @param signatureBytes The byte vector representing the signature.
   * @param signedBytes    The bytes which were purportedly signed to create the signature.
   * @return \c TRUE if the signature is valid, otherwise \c FALSE.
   * @throws std::runtime_error If OpenSSL signature verification fails.
   */
  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

  /**
   * Derived from PublicKey. Construct a Key protobuf object from this ED25519PublicKey object.
   *
   * @return A pointer to a created Key protobuf object filled with this ED25519PublicKey object's data.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::unique_ptr<proto::Key> toProtobuf() const override;

  /**
   * Derived from PublicKey. Get the string representation of this ED25519PublicKey, in DER format.
   *
   * @return The string representation of this ED25519PublicKey in DER format.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derived from PublicKey. Get the byte representation of this ED25519PublicKey, in DER format.
   *
   * @return The byte representation of this ED25519PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const override;

private:
  /**
   * Create an OpenSSL keypair object from a byte vector representing an ED25519PublicKey.
   *
   * @param keyBytes The bytes representing a ED25519PublicKey.
   * @return A pointer to a newly created OpenSSL keypair object.
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);

  /**
   * Prepend an ED25519PublicKey's algorithm identifier to an array of serialized ED25519PublicKey bytes.
   *
   * @param keyBytes The bytes representing an ED25519PublicKey.
   * @return An array of bytes that contains the ED25519PublicKey's algorithm identifier bytes, followed by the input
   *         ED25519PublicKey bytes.
   */
  static std::vector<unsigned char> prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes);

  /**
   * Construct from an OpenSSL key object.
   *
   * @param keypair The underlying OpenSSL keypair object from which to construct this ED25519PublicKey.
   */
  explicit ED25519PublicKey(EVP_PKEY* publicKey);

  /**
   * A pointer to the underlying OpenSSL keypair.
   */
  EVP_PKEY* mPublicKey = nullptr;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ED25519_PUBLIC_KEY_H_