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
#include <valuable/value-ptr.hpp>
#include <vector>

namespace proto
{
class Key;
}

namespace Hedera::internal::OpenSSLUtils
{
class EVP_PKEY;
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
   * Default destructor, but must define after PublicKeyImpl is defined (in source file).
   */
  virtual ~PublicKey();

  /**
   * Construct a PublicKey object from a Key protobuf object.
   *
   * @param proto The Key protobuf object from which to create a PublicKey object.
   * @return A pointer to the created PublicKey object. Nullptr if the key type is not recognized.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromProtobuf(const proto::Key& proto);

  /**
   * Construct a PublicKey object from a hex-encoded, DER-encoded key string.
   *
   * @param key The DER-encoded hex string from which to construct a PublicKey.
   * @return A pointer to an PublicKey representing the input DER-encoded hex string.
   * @throws BadKeyException If the public key type (ED25519 or ECDSAsecp256k1) is unable to be determined or realized
   *                         from the input hex string.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromStringDer(std::string_view key);

  /**
   * Construct a PublicKey object from a DER-encoded byte vector.
   *
   * @param bytes The vector of DER-encoded bytes from which to construct a PublicKey.
   * @return A pointer to a PublicKey representing the input DER-encoded bytes.
   * @throws BadKeyException If the public key type (ED25519 or ECDSAsecp256k1) is unable to be determined or realized
   *                         from the input byte array.
   */
  [[nodiscard]] static std::shared_ptr<PublicKey> fromBytesDer(const std::vector<std::byte>& bytes);

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
  [[nodiscard]] virtual bool verifySignature(const std::vector<std::byte>& signatureBytes,
                                             const std::vector<std::byte>& signedBytes) const = 0;

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
  [[nodiscard]] virtual std::vector<std::byte> toBytesDer() const = 0;

  /**
   * Get the raw bytes of this PublicKey.
   *
   * @return The raw bytes of this PublicKey.
   */
  [[nodiscard]] virtual std::vector<std::byte> toBytesRaw() const = 0;

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  PublicKey(const PublicKey&) = default;
  PublicKey& operator=(const PublicKey&) = default;
  PublicKey(PublicKey&&) noexcept = default;
  PublicKey& operator=(PublicKey&&) noexcept = default;

  /**
   * Construct with a wrapped OpenSSL key object.
   *
   * @param key The wrapped OpenSSL key object.
   */
  explicit PublicKey(internal::OpenSSLUtils::EVP_PKEY&& key);

  /**
   * Get this PublicKey's wrapped OpenSSL key object.
   *
   * @return This PublicKey's wrapped OpenSSL key object.
   */
  [[nodiscard]] internal::OpenSSLUtils::EVP_PKEY getInternalKey() const;

private:
  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct PublicKeyImpl;
  valuable::value_ptr<PublicKeyImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PUBLIC_KEY_H_
