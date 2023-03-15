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
#ifndef HEDERA_SDK_CPP_ECDSA_SECP256K1_PUBLIC_KEY_H_
#define HEDERA_SDK_CPP_ECDSA_SECP256K1_PUBLIC_KEY_H_

#include "PublicKey.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ECDSA secp256k1 public key.
 */
class ECDSAsecp256k1PublicKey : public PublicKey
{
public:
  /**
   * The number of raw bytes in an uncompressed ECDSAsecp256k1PublicKey.
   */
  static constexpr const size_t UNCOMPRESSED_KEY_SIZE = 65ULL;

  /**
   * The number of raw bytes in a compressed ECDSAsecp256k1PublicKey.
   */
  static constexpr const size_t COMPRESSED_KEY_SIZE = 33ULL;

  /**
   * The prefix bytes of a DER-encoded, uncompressed ECDSAsecp256k1PublicKey.
   */
  static inline const std::vector<unsigned char> DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES = {
    0x30, 0x56, 0x30, 0x10, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02,
    0x01, 0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x0A, 0x03, 0x42, 0x00
  };

  /**
   * The hex-encoded string of the DER-encoded prefix bytes of an uncompressed ECDSAsecp256k1PublicKey.
   */
  static inline const std::string DER_ENCODED_UNCOMPRESSED_PREFIX_HEX =
    "3056301006072A8648CE3D020106052B8104000A034200";

  /**
   * The prefix bytes of a DER-encoded, compressed ECDSAsecp256k1PublicKey.
   */
  static inline const std::vector<unsigned char> DER_ENCODED_COMPRESSED_PREFIX_BYTES = { 0x30, 0x2D, 0x30, 0x07, 0x06,
                                                                                         0x05, 0x2B, 0x81, 0x04, 0x00,
                                                                                         0x0A, 0x03, 0x22, 0x00 };

  /**
   * The hex-encoded string of the DER-encoded prefix bytes of a compressed ECDSAsecp256k1PublicKey.
   */
  static inline const std::string DER_ENCODED_COMPRESSED_PREFIX_HEX = "302D300706052B8104000A032200";

  /**
   * Disallow default construction of an ECDSAsecp256k1PublicKey, as an uninitialized ECDSAsecp256k1PublicKey provides
   * no functionality. Instead, a factory function should be used.
   */
  ECDSAsecp256k1PublicKey() = delete;

  /**
   * Construct an ECDSAsecp256k1PublicKey object from a hex-encoded string (DER-encoded or raw).
   *
   * @param key The hex string from which to construct an ECDSAsecp256k1PublicKey. May be either compressed or
   *            uncompressed.
   * @return A pointer to an ECDSAsecp256k1PublicKey representing the input hex string.
   * @throws BadKeyException If an ECDSAsecp256k1PublicKey cannot be realized from the input hex string.
   */
  [[nodiscard]] static std::shared_ptr<ECDSAsecp256k1PublicKey> fromString(std::string_view key);

  /**
   * Construct an ECDSAsecp256k1PublicKey object from a byte vector (DER-encoded or raw).
   *
   * @param bytes The vector of bytes from which to construct an ECDSAsecp256k1PublicKey.
   * @return A pointer to an ECDSAsecp256k1PublicKey representing the input bytes.
   * @throws BadKeyException If an ECDSAsecp256k1PublicKey cannot be realized from the input bytes.
   */
  [[nodiscard]] static std::shared_ptr<ECDSAsecp256k1PublicKey> fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Converts an uncompressed ECDSAsecp256k1PublicKey byte vector to a compressed ECDSAsecp256k1PublicKey byte vector.
   *
   * @param uncompressedBytes The uncompressed bytes of the ECDSAsecp256k1PublicKey.
   * @return A byte vector representing the ECDSAsecp256k1PublicKey in compressed form.
   * @throws std::invalid_argument If the input bytes are not the correct uncompressed key size or malformed.
   * @throws OpenSSLException      If OpenSSL is unable to compress the input bytes.
   */
  [[nodiscard]] static std::vector<unsigned char> compressBytes(const std::vector<unsigned char>& uncompressedBytes);

  /**
   * Converts a compressed ECDSAsecp256k1PublicKey byte vector to an uncompressed ECDSAsecp256k1PublicKey byte vector.
   *
   * @param compressedBytes The compressed bytes of the ECDSAsecp256k1PublicKey.
   * @return A byte vector representing the ECDSAsecp256k1PublicKey in uncompressed form.
   * @throws std::invalid_argument If the input bytes are not the correct compressed key size or malformed.
   * @throws OpenSSLException      If OpenSSL is unable to uncompress the input bytes.
   */
  [[nodiscard]] static std::vector<unsigned char> uncompressBytes(const std::vector<unsigned char>& compressedBytes);

  /**
   * Derived from PublicKey. Create a clone of this ECDSAsecp256k1PublicKey object.
   *
   * @return A pointer to the created clone of this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::unique_ptr<PublicKey> clone() const override;

  /**
   * Derived from PublicKey. Verify that a signature was made by the ECDSAsecp256k1PrivateKey which corresponds to this
   * ECDSAsecp256k1PublicKey.
   *
   * @param signatureBytes The byte vector representing the signature.
   * @param signedBytes    The bytes which were purportedly signed to create the signature.
   * @return \c TRUE if the signature is valid, otherwise \c FALSE.
   * @throws OpenSSLException If OpenSSL is unable to verify the signature.
   */
  [[nodiscard]] bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const override;

  /**
   * Derived from PublicKey. Get the hex-encoded string of the DER-encoded bytes of this ECDSAsecp256k1PublicKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::string toStringDer() const override;

  /**
   * Derived from PublicKey. Get the hex-encoded string of the raw bytes of this ECDSAsecp256k1PublicKey.
   *
   * @return The hex-encoded string of the raw bytes of this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::string toStringRaw() const override;

  /**
   * Derived from PublicKey. Get the DER-encoded bytes of this ECDSAsecp256k1PublicKey.
   *
   * @return The DER-encoded bytes of this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesDer() const override;

  /**
   * Derived from PublicKey. Get the raw bytes of this ECDSAsecp256k1PublicKey.
   *
   * @return The raw bytes of this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytesRaw() const override;

  /**
   * Derived from PublicKey. Construct a Key protobuf object from this ECDSAsecp256k1PublicKey object.
   *
   * @return A pointer to a created Key protobuf object filled with this ECDSAsecp256k1PublicKey object's data.
   * @throws OpenSSLException If OpenSSL is unable to serialize this ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::unique_ptr<proto::Key> toProtobuf() const override;

private:
  /**
   * Construct from a wrapped OpenSSL key object.
   *
   * @param key The wrapped OpenSSL key object from which to construct this ECDSAsecp256k1PublicKey.
   */
  explicit ECDSAsecp256k1PublicKey(internal::OpenSSLUtils::EVP_PKEY&& key);
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_SECP256K1_PUBLIC_KEY_H_
