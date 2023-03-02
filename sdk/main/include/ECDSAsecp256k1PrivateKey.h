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

#include "ECDSAsecp256k1PublicKey.h"
#include "PrivateKey.h"
#include "impl/OpenSSLUtils.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera
{
class MnemonicBIP39;
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
   * The number of bytes in a DER-encoded ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t DER_ENCODED_SIGNATURE_SIZE = 72;

  /**
   * The number of bytes in a raw ECDSAsecp256k1PrivateKey signature ((r,s) form).
   */
  static constexpr const size_t RAW_SIGNATURE_SIZE = 64;

  /**
   * The number of bytes in the r value of an ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t R_SIZE = 32;

  /**
   * The number of bytes in the s value of an ECDSAsecp256k1PrivateKey signature.
   */
  static constexpr const size_t S_SIZE = 32;

  /**
   * Disallow default construction of an ECDSAsecp256k1PrivateKey, as an uninitialized ECDSAsecp256k1PrivateKey provides
   * no functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ECDSAsecp256k1PrivateKey() = delete;

  /**
   * Copy constructor and copy assignment operator can throw OpenSSLException if OpenSSL is unable to serialize the
   * input key to copy.
   */
  ~ECDSAsecp256k1PrivateKey() override = default;
  ECDSAsecp256k1PrivateKey(const ECDSAsecp256k1PrivateKey& other);
  ECDSAsecp256k1PrivateKey& operator=(const ECDSAsecp256k1PrivateKey& other);
  ECDSAsecp256k1PrivateKey(ECDSAsecp256k1PrivateKey&& other) noexcept;
  ECDSAsecp256k1PrivateKey& operator=(ECDSAsecp256k1PrivateKey&& other) noexcept;

  /**
   * Generate a new ECDSAsecp256k1PrivateKey.
   *
   * @return A pointer to the generated ECDSAsecp256k1PrivateKey.
   * @throws OpenSSLException If OpenSSL fails to generate a key.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> generatePrivateKey();

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from the raw string representation.
   *
   * @param keyString The string from which to create an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input string.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input keyString.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromString(std::string_view keyString);

  /**
   * Derive an ECDSAsecp256k1PrivateKey from a seed array.
   *
   * @param seed The array seed from which to derive the ECDSAsecp256k1PrivateKey.
   * @return A pointer to the derived ECDSAsecp256k1PrivateKey.
   * @throws BadKeyException If an ECDSAsecp256k1PrivateKey cannot be realized from the input keyString.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ECDSAsecp256k1PrivateKey object.
   *
   * @return A pointer to the created clone of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   * @throws OpenSSLException If OpenSSL is unable to generate a signature.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Derived from PrivateKey. Get the raw string representation of this ECDSAsecp256k1PrivateKey.
   *
   * @return The string representation of this ECDSAsecp256k1PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to encode this key to a DER-encoded string.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derive a child ECDSAsecp256k1PrivateKey from this ECDSAsecp256k1PrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived ECDSAsecp256k1PrivateKey child.
   * @throws OpenSSLException       If OpenSSL is unable to derive a key with the given childIndex.
   * @throws UninitializedException If this ECDSAsecp256k1PrivateKey was not initialized with a chain code.
   */
  [[nodiscard]] std::unique_ptr<ECDSAsecp256k1PrivateKey> derive(uint32_t childIndex) const;

  /**
   * Get the raw byte representation of this ECDSAsecp256k1PrivateKey.
   *
   * @return The byte representation of this ECDSAsecp256k1PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to decode this key to a byte array.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

private:
  /**
   * Create a wrapped OpenSSL key object from a byte vector representing an ECDSAsecp256k1PrivateKey.
   *
   * @param keyBytes The bytes representing a ECDSAsecp256k1PrivateKey.
   * @return The newly created wrapped OpenSSL keypair object.
   * @throws OpenSSLException If OpenSSL is unable to create a keypair from the input bytes.
   */
  static internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(std::vector<unsigned char> keyBytes);

  /**
   * Construct from a wrapped OpenSSL keypair object.
   *
   * @param keypair The wrapped OpenSSL keypair object from which to construct this ECDSAsecp256k1PrivateKey.
   * @throws OpenSSLException If OpenSSL is unable to get this ED25519PrivateKey's corresponding ED25519PublicKey's
   *                          bytes.
   */
  explicit ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair);

  /**
   * Construct from a wrapped OpenSSL keypair object and a chaincode.
   *
   * @param keypair   The wrapped OpenSSL keypair.
   * @param chainCode The new ECDSAsecp256k1PrivateKey's chain code.
   * @throws OpenSSLException If OpenSSL is unable to get this ECDSAsecp256k1PrivateKey's corresponding
   *                          ECDSAsecp256k1PublicKey's bytes.
   */
  ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair, std::vector<unsigned char>&& chainCode);
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_SECP256K1_PRIVATE_KEY_H_
