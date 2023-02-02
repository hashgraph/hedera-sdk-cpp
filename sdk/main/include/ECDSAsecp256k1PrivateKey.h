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
#include "MnemonicBIP39.h"
#include "PrivateKey.h"

#include <memory>
#include <openssl/crypto.h>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ECDSA secp256k1 private key.
 */
class ECDSAsecp256k1PrivateKey : public PrivateKey
{
public:
  /**
   * Disallow default construction of an ECDSAsecp256k1PrivateKey, as an uninitialized ECDSAsecp256k1PrivateKey provides
   * no functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ECDSAsecp256k1PrivateKey() = delete;

  /**
   * Destructor
   */
  ~ECDSAsecp256k1PrivateKey() override;

  /**
   * Copy constructor and copy assignment operator can throw std::runtime_error if OpenSSL serialization fails.
   */
  ECDSAsecp256k1PrivateKey(const ECDSAsecp256k1PrivateKey& other);
  ECDSAsecp256k1PrivateKey& operator=(const ECDSAsecp256k1PrivateKey& other);
  ECDSAsecp256k1PrivateKey(ECDSAsecp256k1PrivateKey&& other) noexcept;
  ECDSAsecp256k1PrivateKey& operator=(ECDSAsecp256k1PrivateKey&& other) noexcept;

  /**
   * Generate a new ECDSAsecp256k1PrivateKey.
   *
   * @return A pointer to the generated ECDSAsecp256k1PrivateKey.
   * @throws std::runtime_error If OpenSSL keypair initialization fails.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> generatePrivateKey();

  /**
   * Construct an ECDSAsecp256k1PrivateKey object from the raw string representation.
   *
   * @param keyString The string from which to create an ECDSAsecp256k1PrivateKey.
   * @return A pointer to an ECDSAsecp256k1PrivateKey representing the input string.
   * @throws std::runtime_error If OpenSSL deserialization of keyString fails.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromString(const std::string& keyString);

  /**
   * Recover an ECDSAsecp256k1PrivateKey from a BIP39 mnemonic phrase and passphrase.
   *
   * @param mnemonic   The mnemonic phrase from which to recover the ECDSAsecp256k1PrivateKey.
   * @param passphrase The passphrase from which to recover the ECDSAsecp256k1PrivateKey.
   * @return A pointer to the recovered ECDSAsecp256k1PrivateKey.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                                     const std::string& passphrase);

  /**
   * Derive an ECDSAsecp256k1PrivateKey from a seed array.
   *
   * @param seed The array seed from which to derive the ECDSAsecp256k1PrivateKey.
   * @return A pointer to the derived ECDSAsecp256k1PrivateKey.
   */
  static std::unique_ptr<ECDSAsecp256k1PrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ECDSAsecp256k1PrivateKey object.
   *
   * @return A pointer to the created clone of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Get the ECDSAsecp256k1PublicKey that corresponds to this ECDSAsecp256k1PrivateKey.
   *
   * @return A pointer to the ECDSAsecp256k1PublicKey that corresponds to this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const override;

  /**
   * Derived from PrivateKey. Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   * @throws std::runtime_error If OpenSSL signature generation fails.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const override;

  /**
   * Derived from PrivateKey. Get the raw string representation of this ECDSAsecp256k1PrivateKey.
   *
   * @return The string representation of this ECDSAsecp256k1PrivateKey.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derive a child ECDSAsecp256k1PrivateKey from this ECDSAsecp256k1PrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived ECDSAsecp256k1PrivateKey child.
   * @throws std::runtime_error If an invalid index is provided, or this key doesn't support child key derivation.
   */
  [[nodiscard]] std::unique_ptr<ECDSAsecp256k1PrivateKey> derive(uint32_t childIndex) const;

  /**
   * Get the raw byte representation of this ECDSAsecp256k1PrivateKey.
   *
   * @return The byte representation of this ECDSAsecp256k1PrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

  /**
   * Get this ECDSAsecp256k1PrivateKey's chain code. It is possible that the chain code could be empty.
   *
   * @return This ECDSAsecp256k1PrivateKey's chaincode if it exists, otherwise an empty vector.
   */
  [[nodiscard]] std::vector<unsigned char> getChainCode() const;

private:
  /**
   * Create an OpenSSL keypair object from a byte vector representing an ECDSAsecp256k1PrivateKey.
   *
   * @param keyBytes The bytes representing a ECDSAsecp256k1PrivateKey.
   * @return A pointer to a newly created OpenSSL keypair object.
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);

  /**
   * Construct from an OpenSSL keypair object.
   *
   * @param keypair A pointer to the underlying OpenSSL keypair object from which to construct this
   *                ECDSAsecp256k1PrivateKey.
   */
  explicit ECDSAsecp256k1PrivateKey(EVP_PKEY* keypair);

  /**
   * Construct from an OpenSSL keypair object and a chaincode.
   *
   * @param keypair   A pointer to the underlying OpenSSL keypair.
   * @param chainCode The new ECDSAsecp256k1PrivateKey's chain code.
   */
  ECDSAsecp256k1PrivateKey(EVP_PKEY* keypair, std::vector<unsigned char> chainCode);

  /**
   * Get the byte representation of the ECDSAsecp256k1PublicKey that corresponds to this ECDSAsecp256k1PrivateKey.
   *
   * @return The byte representation of the corresponding ECDSAsecp256k1PublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * A pointer to the underlying OpenSSL keypair.
   */
  EVP_PKEY* mKeypair = nullptr;

  /**
   * A pointer to the ECDSAsecp256k1PublicKey object that corresponds to this ECDSAsecp256k1PrivateKey.
   */
  std::shared_ptr<ECDSAsecp256k1PublicKey> mPublicKey = nullptr;

  /**
   * This ECDSAsecp256k1PrivateKey's chain code. If this is empty, then this ECDSAsecp256k1PrivateKey will not support
   * derivation.
   */
  std::vector<unsigned char> mChainCode;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_SECP256K1_PRIVATE_KEY_H_
