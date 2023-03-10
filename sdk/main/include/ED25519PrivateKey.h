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

#include "PrivateKey.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera::internal::OpenSSLUtils
{
class EVP_PKEY;
}

namespace Hedera
{
/**
 * A class representing an ED25519 private key.
 */
class ED25519PrivateKey : public PrivateKey
{
public:
  /**
   * The number of bytes in an ED25519PrivateKey.
   */
  static constexpr const size_t KEY_SIZE = 32ULL;

  /**
   * The prefix bytes of a DER-encoded ED25519PrivateKey.
   */
  static inline const std::vector<unsigned char> DER_ENCODED_PREFIX_BYTES = { 0x30, 0x2E, 0x02, 0x01, 0x00, 0x30,
                                                                              0x05, 0x06, 0x03, 0x2B, 0x65, 0x70,
                                                                              0x04, 0x22, 0x04, 0x20 };

  /**
   * The hex-encoded string of the DER-encoded prefix bytes of an ED25519PrivateKey.
   */
  static inline const std::string DER_ENCODED_PREFIX_HEX = "302E020100300506032B657004220420";

  /**
   * Disallow default construction of an ED25519PrivateKey, as an uninitialized ED25519PrivateKey provides no
   * functionality. Instead, the 'generatePrivateKey()', 'fromString()' or 'fromSeed()' functions should be used.
   */
  ED25519PrivateKey() = delete;

  /**
   * Generate a new ED25519PrivateKey.
   *
   * @return A pointer to the generated ED25519PrivateKey.
   * @throws OpenSSLException If OpenSSL fails to generate a key.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> generatePrivateKey();

  /**
   * Construct an ED25519PrivateKey object from a hex-encoded string (DER-encoded or raw).
   *
   * @param key The hex string from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input hex string.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromString(std::string_view key);

  /**
   * Construct an ED25519PrivateKey object from a hex-encoded, DER-encoded key string.
   *
   * @param key The DER-encoded hex string from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input DER-encoded hex string.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromStringDer(std::string_view key);

  /**
   * Construct an ED25519PrivateKey object from a raw hex-encoded string.
   *
   * @param key The raw hex string from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input raw hex string.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromStringRaw(std::string_view key);

  /**
   * Construct an ED25519PrivateKey object from a byte vector (DER-encoded or raw).
   *
   * @param bytes The vector of bytes from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input bytes.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ED25519PrivateKey object from a DER-encoded byte vector.
   *
   * @param bytes The vector of DER-encoded bytes from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input DER-encoded bytes.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromBytesDer(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ED25519PrivateKey object from a raw byte vector.
   *
   * @param bytes The vector of raw bytes from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input raw bytes.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromBytesRaw(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ED25519PrivateKey from a seed array.
   *
   * @param seed The seed byte array from which to construct an ED25519PrivateKey.
   * @return A pointer to an ED25519PrivateKey representing the input seed bytes.
   * @throws BadKeyException If an ED25519PrivateKey cannot be realized from the input seed bytes.
   */
  [[nodiscard]] static std::unique_ptr<ED25519PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ED25519PrivateKey object.
   *
   * @return A pointer to the created clone of this ED25519PrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Derive a child ED25519PrivateKey from this ED25519PrivateKey.
   *
   * @param childIndex The unhardened index of the child to derive.
   * @return A pointer to the derived ED25519PrivateKey child.
   * @throws OpenSSLException       If OpenSSL is unable to derive a key with the given childIndex.
   * @throws UninitializedException If this ED25519PrivateKey was not initialized with a chain code.
   * @throws std::invalid_argument  If the input index is hardened.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> derive(uint32_t childIndex) const override;

  /**
   * Derived from PrivateKey. Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   * @throws OpenSSLException If OpenSSL is unable to generate a signature.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Derived from PrivateKey. Get the hex-encoded string of the DER-encoded bytes of this ED25519PrivateKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this ED25519PrivateKey.
   */
  [[nodiscard]] std::string toStringDer() const override;

  /**
   * Derived from PrivateKey. Get the hex-encoded string of the raw bytes of this ED25519PrivateKey.
   *
   * @return The hex-encoded string of the raw bytes of this ED25519PrivateKey.
   */
  [[nodiscard]] std::string toStringRaw() const override;

  /**
   * Derived from PrivateKey. Get the DER-encoded bytes of this ED25519PrivateKey.
   *
   * @return The DER-encoded bytes of this ED25519PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesDer() const override;

  /**
   * Derived from PrivateKey. Get the raw bytes of this ED25519PrivateKey.
   *
   * @return The raw bytes of this ED25519PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesRaw() const override;

private:
  /**
   * Construct from a wrapped OpenSSL key object and optionally a chain code.
   *
   * @param key       The wrapped OpenSSL key object.
   * @param chainCode The chain code.
   * @throws OpenSSLException If OpenSSL is unable to get this ED25519PrivateKey's corresponding ED25519PrivateKey's
   *                          bytes.
   * @throws BadKeyException  If the chain code is malformed.
   */
  explicit ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key,
                             std::vector<unsigned char> chainCode = std::vector<unsigned char>());
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ED25519_PRIVATE_KEY_H_
