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
#ifndef HEDERA_SDK_CPP_BIGNUMBER_H
#define HEDERA_SDK_CPP_BIGNUMBER_H

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
   * Don't allow copies or moves
   */
  BigNumber(const BigNumber&) = delete;
  BigNumber(BigNumber&& other) = delete;

  /**
   * Create a new BigNumber from a hex string
   *
   * @param hexString string representing the big number
   * @return a newly constructed BigNumber
   */
  static BigNumber fromHex(const std::string& hexString);

  /**
   * Create a new BigNumber from a bytes vector
   *
   * @param bytes vector of bytes representing the big number
   * @return a newly constructed BigNumber
   */
  static BigNumber fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Add another big number to this one, and take the modulo of the sum
   *
   * @param other the other big number to add to this one
   * @param modulo the module to take the sum to
   * @return the modular sum
   */
  [[nodiscard]] BigNumber modularAdd(const BigNumber& other, const BigNumber& modulo) const;

  /**
   * Get a vector of bytes representing this big number
   *
   * @return the byte vector representing this number
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;
private:
  /**
   * Hidden constructor
   *
   * @param underlyingNum the underlying OpenSSL big number
   */
  explicit BigNumber(BIGNUM* underlyingNum);

  /**
   * The OpenSSL big number wrapped by this object
   */
  BIGNUM* mUnderlyingBigNum;
};

} // Hedera

#endif // HEDERA_SDK_CPP_BIGNUMBER_H
