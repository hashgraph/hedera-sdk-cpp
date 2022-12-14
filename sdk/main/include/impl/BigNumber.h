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
  ~BigNumber();
  BigNumber(const BigNumber&) = delete;
  BigNumber(BigNumber&& other) = delete;

  static BigNumber fromHex(const std::string& hexString);
  static BigNumber fromBytes(const std::vector<unsigned char>& bytes);

  [[nodiscard]] BigNumber modularAdd(const BigNumber& other, const BigNumber& modulo) const;

  [[nodiscard]] std::vector<unsigned char> toBytes() const;
private:
  explicit BigNumber(BIGNUM* underlyingNum);

  BIGNUM* mUnderlyingBigNum;
};

} // Hedera

#endif // HEDERA_SDK_CPP_BIGNUMBER_H
