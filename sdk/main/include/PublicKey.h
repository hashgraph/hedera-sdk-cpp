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
#ifndef HEDERA_SDK_CPP_PUBLIC_KEY_H_
#define HEDERA_SDK_CPP_PUBLIC_KEY_H_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace proto
{
class Key;
}

namespace Hedera
{
/**
 * A generic class representing a public key.
 */
class PublicKey
{
public:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  virtual ~PublicKey() = default;
  PublicKey(const PublicKey&) = delete;
  PublicKey& operator=(const PublicKey&) = delete;
  PublicKey(PublicKey&&) noexcept = delete;
  PublicKey& operator=(PublicKey&&) noexcept = delete;

  /**
   * Create a PublicKey object from a Key protobuf object.
   *
   * @param proto The Key protobuf object from which to create a PublicKey object.
   * @return A pointer to the created PublicKey object. Nullptr if the key type is not recognized.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromProtobuf(const proto::Key& proto);

  /**
   * Create a PublicKey object from a hex-encoded string of DER-encoded bytes.
   *
   * @param key The hex string from which to create a PublicKey object.
   * @return A pointer to the created PublicKey object.
   * @throws std::invalid_argument If the public key type (ED25519 or ECDSAsecp256k1) is unable to be determined.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromStringDer(std::string_view key);

  /**
   * Create a PublicKey object from a DER-encoded byte array.
   *
   * @param bytes The byte array from which to create a PublicKey object.
   * @return A pointer to the created PublicKey object.
   * @throws std::invalid_argument If the public key type (ED25519 or ECDSAsecp256k1) is unable to be determined.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromBytesDer(const std::vector<unsigned char>& bytes);

  /**
   * Create a clone of this PublicKey object.
   *
   * @return A pointer to the created clone of this PublicKey.
   */
  [[nodiscard]] virtual std::unique_ptr<PublicKey> clone() const = 0;

  /**
   * Construct a Key protobuf object from this PublicKey object.
   *
   * @return A pointer to the created Key protobuf object filled with this PublicKey object's data.
   */
  [[nodiscard]] virtual std::unique_ptr<proto::Key> toProtobuf() const = 0;

  /**
   * Verify that a signature was made by the PrivateKey which corresponds to this PublicKey.
   *
   * @param signatureBytes The byte vector representing the signature.
   * @param signedBytes    The bytes which were purportedly signed to create the signature.
   * @return \c TRUE if the signature is valid, otherwise \c FALSE.
   */
  [[nodiscard]] virtual bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                             const std::vector<unsigned char>& signedBytes) const = 0;

  /**
   * Get the hex-encoded string of the DER-encoded bytes of this PublicKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this PublicKey.
   */
  [[nodiscard]] virtual std::string toStringDer() const = 0;

  /**
   * Get the hex-encoded string of the raw bytes of this PublicKey.
   *
   * @return The hex-encoded string of the raw bytes of this PublicKey.
   */
  [[nodiscard]] virtual std::string toStringRaw() const = 0;

  /**
   * Get the DER-encoded bytes of this PublicKey.
   *
   * @return The DER-encoded bytes of this PublicKey.
   */
  [[nodiscard]] virtual std::vector<unsigned char> toBytesDer() const = 0;

  /**
   * Get the raw bytes of this PublicKey.
   *
   * @return The raw bytes of this PublicKey.
   */
  [[nodiscard]] virtual std::vector<unsigned char> toBytesRaw() const = 0;

protected:
  PublicKey() = default;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PUBLIC_KEY_H_
