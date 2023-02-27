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
#ifndef HEDERA_SDK_CPP_PRIVATE_KEY_H_
#define HEDERA_SDK_CPP_PRIVATE_KEY_H_

#include <memory>
#include <string>
#include <vector>

namespace Hedera
{
class PublicKey;
}

namespace Hedera
{
/**
 * A generic class representing a private key.
 */
class PrivateKey
{
public:
  virtual ~PrivateKey() = default;

  /**
   * Create a clone of this PrivateKey object.
   *
   * @return A pointer to the created clone of this PrivateKey.
   */
  [[nodiscard]] virtual std::unique_ptr<PrivateKey> clone() const = 0;

  /**
   * Get the PublicKey that corresponds to this PrivateKey.
   *
   * @return A pointer to the PublicKey that corresponds to this PrivateKey.
   */
  [[nodiscard]] virtual std::shared_ptr<PublicKey> getPublicKey() const = 0;

  /**
   * Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   */
  [[nodiscard]] virtual std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const = 0;

  /**
   * Get the string representation of this PrivateKey.
   *
   * @return The string representation of this PrivateKey.
   */
  [[nodiscard]] virtual std::string toString() const = 0;

  /**
   * Get this PrivateKey's chain code. It is possible that the chain code could be empty.
   *
   * @return This PrivateKey's chaincode if it exists, otherwise an empty vector.
   */
  [[nodiscard]] inline std::vector<unsigned char> getChainCode() const { return mChainCode; }

protected:
  PrivateKey() = default;

  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  PrivateKey(const PrivateKey&) = default;
  PrivateKey& operator=(const PrivateKey&) = default;
  PrivateKey(PrivateKey&&) noexcept = default;
  PrivateKey& operator=(PrivateKey&&) noexcept = default;

  /**
   * Construct with chain code.
   */
  explicit PrivateKey(std::vector<unsigned char> chainCode)
    : mChainCode(std::move(chainCode))
  {
  }

private:
  /**
   * This PrivateKey's chain code. If this is empty, then this PrivateKey will not support derivation.
   */
  std::vector<unsigned char> mChainCode;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PRIVATE_KEY_H_
