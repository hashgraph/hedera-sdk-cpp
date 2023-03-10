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

#include <cstdint>
#include <memory>
#include <string>
#include <valuable/value-ptr.hpp>
#include <vector>

namespace Hedera
{
class PublicKey;
}

namespace Hedera::internal::OpenSSLUtils
{
class EVP_PKEY;
}

namespace Hedera
{
/**
 * A generic class representing a private key.
 */
class PrivateKey
{
public:
  /**
   * The number of bytes in a PrivateKey chain code.
   */
  static constexpr const size_t CHAIN_CODE_SIZE = 32ULL;

  /**
   * Default destructor, but must define after PrivateKeyImpl is defined (in source file).
   */
  virtual ~PrivateKey();

  /**
   * Create a clone of this PrivateKey object.
   *
   * @return A pointer to the created clone of this PrivateKey.
   */
  [[nodiscard]] virtual std::unique_ptr<PrivateKey> clone() const = 0;

  /**
   * Derive a child PrivateKey from this PrivateKey.
   *
   * @param childIndex The index of the child to derive.
   * @return A pointer to the derived PrivateKey child.
   */
  [[nodiscard]] virtual std::unique_ptr<PrivateKey> derive(uint32_t childIndex) const = 0;

  /**
   * Sign an arbitrary byte array.
   *
   * @param bytesToSign The bytes to sign.
   * @return The signature of the byte array.
   */
  [[nodiscard]] virtual std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const = 0;

  /**
   * Get the hex-encoded string of the DER-encoded bytes of this PrivateKey.
   *
   * @return The hex-encoded string of the DER-encoded bytes of this PrivateKey.
   */
  [[nodiscard]] virtual std::string toStringDer() const = 0;

  /**
   * Get the hex-encoded string of the raw bytes of this PrivateKey.
   *
   * @return The hex-encoded string of the raw bytes of this PrivateKey.
   */
  [[nodiscard]] virtual std::string toStringRaw() const = 0;

  /**
   * Get the DER-encoded bytes of this PrivateKey.
   *
   * @return The DER-encoded bytes of this PrivateKey.
   */
  [[nodiscard]] virtual std::vector<unsigned char> toBytesDer() const = 0;

  /**
   * Get the raw bytes of this PrivateKey.
   *
   * @return The raw bytes of this PrivateKey.
   */
  [[nodiscard]] virtual std::vector<unsigned char> toBytesRaw() const = 0;

  /**
   * Get this PrivateKey's chain code. It is possible that the chain code could be empty.
   *
   * @return This PrivateKey's chaincode if it exists, otherwise an empty vector.
   */
  [[nodiscard]] std::vector<unsigned char> getChainCode() const;

  /**
   * Get the PublicKey that corresponds to this PrivateKey.
   *
   * @return A pointer to the PublicKey that corresponds to this PrivateKey.
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getPublicKey() const;

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  PrivateKey(const PrivateKey&) = default;
  PrivateKey& operator=(const PrivateKey&) = default;
  PrivateKey(PrivateKey&&) noexcept = default;
  PrivateKey& operator=(PrivateKey&&) noexcept = default;

  /**
   * Construct from a wrapped OpenSSL key object and optionally a chain code.
   *
   * @param key       The wrapped OpenSSL key object.
   * @param chainCode The chain code.
   * @throws OpenSSLException If OpenSSL is unable to get this PrivateKey's corresponding PrivateKey's bytes.
   * @throws BadKeyException  If the chain code is malformed.
   */
  explicit PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key,
                      std::vector<unsigned char> chainCode = std::vector<unsigned char>());

  /**
   * Get the DER-encoded bytes of the PublicKey that corresponds to this PrivateKey.
   *
   * @return The DER-encoded bytes of this PrivateKey's corresponding PublicKey.
   * @throws OpenSSLException If OpenSSL is unable to get this PrivateKey's corresponding PublicKey's bytes.
   */
  [[nodiscard]] std::vector<unsigned char> getPublicKeyBytes() const;

  /**
   * Get this PrivateKey's wrapped OpenSSL key object.
   *
   * @return This PrivateKey's wrapped OpenSSL key object.
   */
  [[nodiscard]] internal::OpenSSLUtils::EVP_PKEY getInternalKey() const;

private:
  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct PrivateKeyImpl;
  valuable::value_ptr<PrivateKeyImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PRIVATE_KEY_H_
