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
#ifndef HEDERA_SDK_CPP_ECDSA_PRIVATE_KEY_H_
#define HEDERA_SDK_CPP_ECDSA_PRIVATE_KEY_H_

#include "ECDSAPublicKey.h"
#include "MnemonicBIP39.h"
#include "PrivateKey.h"

#include <memory>
#include <openssl/crypto.h>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * A class representing an ECDSA private key.
 */
class ECDSAPrivateKey : public PrivateKey
{
public:
  /**
   * Disallow default construction of an ECDSAPrivateKey, as an uninitialized ECDSAPrivateKey provides no
   * functionality. Instead, the 'fromString()' or 'fromBytes()' functions should be used.
   */
  ECDSAPrivateKey() = delete;

  /**
   * Destructor
   */
  ~ECDSAPrivateKey() override;

  /**
   * Copy constructor and copy assignment operator can throw std::runtime_error if OpenSSL serialization fails.
   */
  ECDSAPrivateKey(const ECDSAPrivateKey& other);
  ECDSAPrivateKey& operator=(const ECDSAPrivateKey& other);
  ECDSAPrivateKey(ECDSAPrivateKey&& other) noexcept;
  ECDSAPrivateKey& operator=(ECDSAPrivateKey&& other) noexcept;

  /**
   * Generate a new ECDSAPrivateKey.
   *
   * @return A pointer to the generated ECDSAPrivateKey.
   * @throws std::runtime_error If OpenSSL keypair initialization fails.
   */
  static std::unique_ptr<ECDSAPrivateKey> generatePrivateKey();

  /**
   * Construct an ECDSAPrivateKey object from the raw string representation.
   *
   * @param keyString The string from which to create an ECDSAPrivateKey.
   * @return A pointer to an ECDSAPrivateKey representing the input string.
   * @throws std::runtime_error If OpenSSL deserialization of keyString fails.
   */
  static std::unique_ptr<ECDSAPrivateKey> fromString(const std::string& keyString);

  /**
   * Recover an ECDSAPrivateKey from a BIP39 mnemonic phrase and passphrase.
   *
   * @param mnemonic   The mnemonic phrase from which to recover the ECDSAPrivateKey.
   * @param passphrase The passphrase from which to recover the ECDSAPrivateKey.
   * @return A pointer to the recovered ECDSAPrivateKey.
   */
  static std::unique_ptr<ECDSAPrivateKey> fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                              const std::string& passphrase);

  /**
   * Derive an ECDSAPrivateKey from a seed array.
   *
   * @param seed The array seed from which to derive the ECDSAPrivateKey.
   * @return A pointer to the derived ECDSAPrivateKey.
   */
  static std::unique_ptr<ECDSAPrivateKey> fromSeed(const std::vector<unsigned char>& seed);

  /**
   * Derived from PrivateKey. Create a clone of this ECDSAPrivateKey object.
   *
   * @return A pointer to the created clone of this ECDSAPrivateKey.
   */
  [[nodiscard]] std::unique_ptr<PrivateKey> clone() const override;

  /**
   * Derived from PrivateKey. Get the ECDSAPublicKey that corresponds to this ECDSAPrivateKey.
   *
   * @return A pointer to the ECDSAPublicKey that corresponds to this ECDSAPrivateKey.
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
   * Derived from PrivateKey. Get the raw string representation of this ECDSAPrivateKey
   *
   * @return The string representation of this ECDSAPrivateKey.
   * @throws std::runtime_error If OpenSSL serialization fails.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * Derive a child ECDSAPrivateKey from this ECDSAPrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived ECDSAPrivateKey child.
   * @throws std::runtime_error If an invalid index is provided, or this key doesn't support child key derivation.
   */
  [[nodiscard]] std::unique_ptr<ECDSAPrivateKey> derive(uint32_t childIndex) const;

  /**
   * Get the raw byte representation of this ECDSAPrivateKey.
   *
   * @return The byte representation of this ECDSAPrivateKey.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

  /**
   * Get this ECDSAPrivateKey's chain code. It is possible that the chain code could be empty.
   *
   * @return This ECDSAPrivateKey's chaincode if it exists, otherwise an empty vector.
   */
  [[nodiscard]] std::vector<unsigned char> getChainCode() const;

private:
  /**
   * Create an OpenSSL keypair object from a byte vector representing an ECDSAPrivateKey.
   *
   * @param keyBytes The bytes representing a ECDSAPrivateKey.
   * @return A pointer to a newly created OpenSSL keypair object.
   */
  static EVP_PKEY* bytesToPKEY(const std::vector<unsigned char>& keyBytes);

  /**
   * Construct from an OpenSSL keypair object.
   *
   * @param keypair A pointer to the underlying OpenSSL keypair object from which to construct this ECDSAPrivateKey.
   */
  explicit ECDSAPrivateKey(EVP_PKEY* keypair);

  /**
   * Construct from an OpenSSL keypair object and a chaincode.
   *
   * @param keypair   A pointer to the underlying OpenSSL keypair.
   * @param chainCode The new ECDSAPrivateKey's chain code.
   */
  ECDSAPrivateKey(EVP_PKEY* keypair, std::vector<unsigned char> chainCode);

  /**
   * Get the byte representation of the ECDSAPublicKey that corresponds to this ECDSAPrivateKey.
   *
   * @return The byte representation of the corresponding ECDSAPublicKey.
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * A pointer to the underlying OpenSSL keypair.
   */
  EVP_PKEY* mKeypair = nullptr;

  /**
   * A pointer to the ECDSAPublicKey object that corresponds to this ECDSAPrivateKey.
   */
  std::shared_ptr<ECDSAPublicKey> mPublicKey = nullptr;

  /**
   * This ECDSAPrivateKey's chain code. If this is empty, then this ECDSAPrivateKey will not support derivation.
   */
  std::vector<unsigned char> mChainCode;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ECDSA_PRIVATE_KEY_H_
