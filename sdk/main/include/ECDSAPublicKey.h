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
#ifndef HEDERA_SDK_CPP_ECDSA_PUBLIC_KEY_H_
#define HEDERA_SDK_CPP_ECDSA_PUBLIC_KEY_H_

#include "PublicKey.h"

#include <openssl/crypto.h>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ECDSA public key.
 */
class ECDSAPublicKey : public PublicKey
{
public:
  /**
   * Disallow default construction of an ECDSAPublicKey, as an uninitialized ECDSAPublicKey provides no
   * functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ECDSAPublicKey() = delete;

  /**
   * Destructor
   */
  ~ECDSAPublicKey() override;

  /**
   * Copy constructor and copy assignment operator can throw std::runtime_error if OpenSSL serialization fails.
   */
  ECDSAPublicKey(const ECDSAPublicKey& other);
  ECDSAPublicKey& operator=(const ECDSAPublicKey& other);
  ECDSAPublicKey(ECDSAPublicKey&& other) noexcept;
  ECDSAPublicKey& operator=(ECDSAPublicKey&& other) noexcept;

  /**
   * Construct an ECDSAPublicKey object from the raw string representation
   *
   * @param keyString The string from which to create an ECDSAPublicKey. May be either compressed or uncompressed,
   *                  but must be the raw encoding (no extra ASN.1 bytes).
   * @return A pointer to an ECDSAPublicKey representing the input string.
   */
  static std::shared_ptr<ECDSAPublicKey> fromString(const std::string& keyString);

  /**
   * Construct an ECDSAPublicKey object from a byte vector.
   *
   * @param keyBytes The vector of raw bytes from which to construct the ECDSAPublicKey.
   * @return A pointer to an ECDSAPublicKey representing the input bytes.
   */
  static std::shared_ptr<ECDSAPublicKey> fromBytes(const std::vector<unsigned char>& keyBytes);

  /**
   * Converts an uncompressed representation of a public key to a compressed representation.
   *
   * @param uncompressedBytes The uncompressed bytes of the public key.
   * @return A byte vector representing the public key in compressed form.
   */
  static std::vector<unsigned char> compressBytes(const std::vector<unsigned char>& uncompressedBytes);

  /**
   * Converts a compressed representation of a public key to the uncompressed representation
   *
   * @param compressedBytes the compressed bytes of the public key
   * @return a byte vector representing the public key in uncompressed form
   */
  static std::vector<unsigned char> uncompressBytes(const std::vector<unsigned char>& compressedBytes);

  /**
   * Derived from PublicKey. Create a clone of this ECDSAPublicKey object.
   *
   * @return A pointer to the created clone of this ECDSAPublicKey.
   */
  [[nodiscard]] std::unique_ptr<PublicKey> clone() const override;

  /**
   * Derived from PublicKey. Verify that a signature was made by the ECDSAPrivateKey which corresponds to this
   * ECDSAPublicKey.
   *
   * @param signatureBytes The byte vector representing the signature.
   * @param signedBytes    The bytes which were purportedly signed to create the signature.
   * @return \c TRUE if the signature is valid, otherwise \c FALSE.
   * @throws std::runtime_error If OpenSSL signature verification experiences an error.
   */
  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

  /**
   * Derived from PublicKey. Construct a Key protobuf object from this ECDSAPublicKey object.
   *
   * @return A pointer to a created Key protobuf object filled with this ECDSAPublicKey object's data.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::unique_ptr<proto::Key> toProtobuf() const override;

  /**
   * Derived from PublicKey. Get the raw string representation of this ECDSAPublicKey (no additional ASN.1 bytes)
   *
   * @return The string representation of this ECDSAPublicKey.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derived from PublicKey. Get the raw byte representation of this ECDSAPublicKey
   *
   * @return The byte representation of this ECDSAPublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const override;

private:
  /**
   * Create an OpenSSL keypair object from a byte vector representing an ECDSAPublicKey.
   *
   * @param inputKeyBytes The bytes representing a ECDSAPublicKey.
   * @return A pointer to a newly created OpenSSL keypair object.
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& inputKeyBytes);

  /**
   * Construct from an OpenSSL key object.
   *
   * @param keypair The underlying OpenSSL keypair object from which to construct this ECDSAPublicKey.
   */
  explicit ECDSAPublicKey(EVP_PKEY* publicKey);

  /**
   * A pointer to the underlying OpenSSL keypair.
   */
  EVP_PKEY* mPublicKey = nullptr;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_PUBLIC_KEY_H_
