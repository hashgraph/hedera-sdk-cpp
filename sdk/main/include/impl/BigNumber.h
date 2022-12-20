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
#ifndef HEDERA_SDK_CPP_IMPL_BIGNUMBER_H_
#define HEDERA_SDK_CPP_IMPL_BIGNUMBER_H_

#include <string>
#include <vector>

#include <openssl/bn.h>

namespace Hedera::internal
{
/**
 * A wrapper around OpenSSL Big Numbers, for easy and safe usage
 */
class BigNumber
{
public:
  /**
   * Destructor
   */
  ~BigNumber();

  /**
   * Don't allow copies or moves or assignments
   */
  BigNumber(const BigNumber&) = delete;
  BigNumber& operator=(const BigNumber&) = delete;
  BigNumber(BigNumber&& other) = delete;
  BigNumber& operator=(BigNumber&&) = delete;

  /**
   * Create a new BigNumber from a hex string.
   *
   * @param hexString A string representing the BigNumber.
   * @return A newly constructed BigNumber.
   */
  static BigNumber fromHex(const std::string& hexString);

  /**
   * Create a new BigNumber from a bytes vector.
   *
   * @param bytes The vector of bytes representing the BigNumber.
   * @return A newly constructed BigNumber.
   */
  static BigNumber fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Add another BigNumber to this one, and take the modulo of the sum.
   *
   * @param other  The other BigNumber to add to this one.
   * @param modulo The modulo to take the sum to.
   * @return The modular sum.
   */
  [[nodiscard]] BigNumber modularAdd(const BigNumber& other, const BigNumber& modulo) const;

  /**
   * Get a vector of bytes representing this BigNumber.
   *
   * @return The byte vector representing this BigNumber.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;

private:
  /**
   * Hidden constructor
   *
   * @param underlyingNum The underlying OpenSSL big number.
   */
  explicit BigNumber(BIGNUM* underlyingNum);

  /**
   * The OpenSSL big number wrapped by this object.
   */
  BIGNUM* mUnderlyingBigNum;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_BIGNUMBER_H_
