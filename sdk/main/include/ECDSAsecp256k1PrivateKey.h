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
#ifndef HEDERA_SDK_CPP_ECDSA_SECP256K1_PRIVATE_KEY_H_
#define HEDERA_SDK_CPP_ECDSA_SECP256K1_PRIVATE_KEY_H_

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
 * A class representing an ECDSA secp256k1 private key.
 */
class ECDSAsecp256k1PrivateKey : public PrivateKey
{
public:
  /**
   * The prefix bytes of a DER-encoded ECDSAsecp256k1PrivateKey.
   */
  static inline const std::vector<unsigned char> DER_ENCODED_PREFIX_BYTES = { 0x30, 0x30, 0x02, 0x01, 0x00, 0x30,
                                                                              0x07, 0x06, 0x05, 0x2B, 0x81, 0x04,
                                                                              0x00, 0x0A, 0x04, 0x22, 0x04, 0x20 };

  /**
   * The hex-encoded string of the DER-encoded prefix bytes of an ECDSAsecp256k1PrivateKey.
   */
  static inline const std::string DER_ENCODED_PREFIX_HEX = "3030020100300706052B8104000A04220420";

  /**
   * The number of bytes in a DER-encoded ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t DER_ENCODED_SIGNATURE_SIZE = 72ULL;

  /**
   * The number of bytes in a raw ECDSAsecp256k1PrivateKey signature ((r,s) form).
   */
  static constexpr const size_t RAW_SIGNATURE_SIZE = 64ULL;

  /**
   * The number of bytes in the r value of an ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t R_SIZE = 32ULL;

  /**
   * The number of bytes in the s value of an ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t S_SIZE = 32ULL;

  /**
   * Disallow default construction of an ECDSAsecp256k1PrivateKey, as an uninitialized ECDSAsecp256k1PrivateKey provides
   * no functionality. Instead, the 'generatePrivateKey()', 'fromString()' or 'fromSeed()' functions should be used.
   */
  ECDSAsecp256k1PrivateKey() = delete;

  /**
   * Generate a new ECDSAsecp256k1PrivateKey.
   *
   * @return A pointer to the generated ECDSAsecp256k1PrivateKey.
   * @throws OpenSSLException If OpenSSL fails to generate a key.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> generatePrivateKey();

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a hex-encoded string (DER-encoded or raw).
   *
   * @param key The hex-encoded string from which to construct an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input string.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input keyString.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromString(std::string_view key);

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a hex-encoded, DER-encoded key string.
   *
   * @param key The string from which to create an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input hex string.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input key.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromStringDer(std::string_view key);

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a raw hex-encoded string.
   *
   * @param key The string from which to create an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input raw hex string.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input key.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromStringRaw(std::string_view key);

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a byte vector (DER-encoded or raw).
   *
   * @param bytes The vector of bytes from which to construct the ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input bytes.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a DER-encoded byte vector.
   *
   * @param bytes The vector of DER-encoded bytes from which to construct the ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input DER-encoded bytes.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromBytesDer(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from a raw byte vector.
   *
   * @param bytes The vector of raw bytes from which to construct the ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the raw input bytes.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromBytesRaw(const std::vector<unsigned char>& bytes);

  /**
   * Construct an ECDSAsecp256k1PrivateKey from a seed array.
   *
   * @param seed The seed array from which to construct an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input seed bytes.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input seed bytes.
   */
  [[nodiscard]] static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ECDSAsecp256k1PrivateKey object.
   *
   * @return A pointer to the created clone of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Derive a child ECDSAsecp256k1PrivateKey from this ECDSAsecp256k1PrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived ECDSAsecp256k1PrivateKey child.
   * @throws OpenSSLException       If OpenSSL is unable to derive a key with the given childIndex.
   * @throws UninitializedException If this ECDSAsecp256k1PrivateKey was not initialized with a chain code.
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
   * Derived from PrivateKey. Get the hex-encoded string of the DER-encoded bytes of this ECDSAsecp256k1PrivateKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::string toStringDer() const override;

  /**
   * Derived from PrivateKey. Get the hex-encoded string of the raw bytes of this ECDSAsecp256k1PrivateKey.
   *
   * @return The hex-encoded string of the raw bytes of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::string toStringRaw() const override;

  /**
   * Derived from PrivateKey. Get the DER-encoded bytes of this ECDSAsecp256k1PrivateKey.
   *
   * @return The DER-encoded bytes of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesDer() const override;

  /**
   * Derived from PrivateKey. Get the raw bytes of this ECDSAsecp256k1PrivateKey.
   *
   * @return The raw bytes of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesRaw() const override;

private:
  /**
   * Construct from a wrapped OpenSSL keypair object and a chaincode.
   *
   * @param keypair   The wrapped OpenSSL keypair.
   * @param chainCode The new ECDSAsecp256k1PrivateKey's chain code.
   * @throws OpenSSLException If OpenSSL is unable to get this ECDSAsecp256k1PrivateKey's corresponding
   *                          ECDSAsecp256k1PublicKey's bytes.
   */
  explicit ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair,
                                    std::vector<unsigned char> chainCode = std::vector<unsigned char>());
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_SECP256K1_PRIVATE_KEY_H_
