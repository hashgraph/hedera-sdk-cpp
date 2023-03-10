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
#include "impl/OpenSSLUtils.h"

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <string_view>
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
   * The number of raw bytes in an ED25519PublicKey.
   */
  static constexpr const size_t KEY_SIZE = 32ULL;

  /**
   * The prefix bytes of a DER-encoded ED25519PublicKey.
   */
  static inline const std::vector<unsigned char> DER_ENCODED_PREFIX_BYTES = { 0x30, 0x2A, 0x30, 0x05, 0x06, 0x03,
                                                                              0x2B, 0x65, 0x70, 0x03, 0x21, 0x00 };

  /**
   * The hex-encoded string of the DER-encoded prefix bytes of an ED25519PublicKey.
   */
  static inline const std::string DER_ENCODED_PREFIX_HEX = "302A300506032B6570032100";

  /**
   * Disallow default construction of an ED25519PublicKey, as an uninitialized ED25519PublicKey provides no
   * functionality. Instead, a factory function should be used.
   */
  ED25519PublicKey() = delete;

  /**
   * Construct an ED25519PublicKey object from a hex-encoded string (DER-encoded or raw).
   *
   * @param key The hex string from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input hex string.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromString(std::string_view key);

  /**
   * Construct an ED25519PublicKey object from a hex-encoded, DER-encoded key string.
   *
   * @param key The DER-encoded hex string from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input DER-encoded hex string.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromStringDer(std::string_view key);

  /**
   * Construct an ED25519PublicKey object from a raw hex-encoded string.
   *
   * @param key The raw hex string from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input raw hex string.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromStringRaw(std::string_view key);

  /**
   * Construct an ED25519PublicKey object from a byte vector (DER-encoded or raw).
   *
   * @param bytes The vector of bytes from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input bytes.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ED25519PublicKey object from a DER-encoded byte vector.
   *
   * @param bytes The vector of DER-encoded bytes from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input DER-encoded bytes.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromBytesDer(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ED25519PublicKey object from a raw byte vector.
   *
   * @param bytes The vector of raw bytes from which to construct an ED25519PublicKey.
   * @return A pointer to an ED25519PublicKey representing the input raw bytes.
   * @throws BadKeyException If an ED25519PublicKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::shared_ptr<ED25519PublicKey> fromBytesRaw(const std::vector<unsigned char>& bytes);

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
   * @return \c TRUE if the signature is valid for this ED25519PublicKey's private key, otherwise \c FALSE.
   * @throws OpenSSLException If OpenSSL is unable to verify the signature.
   */
  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

  /**
   * Derived from PublicKey. Get the hex-encoded string of the DER-encoded bytes of this ED25519PublicKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this ED25519PublicKey.
   */
  [[nodiscard]] std::string toStringDer() const override;

  /**
   * Derived from PublicKey. Get the hex-encoded string of the raw bytes of this ED25519PublicKey.
   *
   * @return The hex-encoded string of the raw bytes of this ED25519PublicKey.
   */
  [[nodiscard]] std::string toStringRaw() const override;

  /**
   * Derived from PublicKey. Get the DER-encoded bytes of this ED25519PublicKey.
   *
   * @return The DER-encoded bytes of this ED25519PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesDer() const override;

  /**
   * Derived from PublicKey. Get the raw bytes of this ED25519PublicKey.
   *
   * @return The raw bytes of this ED25519PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesRaw() const override;

  /**
   * Derived from PublicKey. Construct a Key protobuf object from this ED25519PublicKey object.
   *
   * @return A pointer to a created Key protobuf object filled with this ED25519PublicKey object's data.
   * @throws OpenSSLException If OpenSSL is unable to serialize this ED25519PublicKey.
   */
  [[nodiscard]] std::unique_ptr<proto::Key> toProtobuf() const override;

private:
  /**
   * Construct from a wrapped OpenSSL key object.
   *
   * @param key The wrapped OpenSSL key object from which to construct this ED25519PublicKey.
   */
  explicit ED25519PublicKey(internal::OpenSSLUtils::EVP_PKEY&& key);
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ED25519_PUBLIC_KEY_H_
