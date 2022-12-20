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
#include "impl/BigNumber.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"

#include <stdexcept>

namespace Hedera::internal
{

BigNumber::BigNumber(BIGNUM* underlyingNum)
  : mUnderlyingBigNum(underlyingNum)
{
}

BigNumber::~BigNumber()
{
  BN_clear_free(mUnderlyingBigNum);
}

BigNumber BigNumber::fromHex(const std::string& hexString)
{
  BIGNUM* bigNum = BN_secure_new();

  if (bigNum == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_secure_new"));
  }

  if (BN_hex2bn(&bigNum, hexString.c_str()) <= 0)
  {
    BN_clear_free(bigNum);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_hex2bn"));
  }

  return BigNumber(bigNum);
}

BigNumber BigNumber::fromBytes(const std::vector<unsigned char>& bytes)
{
  // go through hex rather than using big-endian openssl functions
  return fromHex(HexConverter::base64ToHex(bytes));
}

BigNumber BigNumber::modularAdd(const BigNumber& other, const BigNumber& modulo) const
{
  BN_CTX* context = BN_CTX_secure_new();

  if (context == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_CTX_secure_new"));
  }

  BIGNUM* result = BN_secure_new();

  if (result == nullptr)
  {
    BN_CTX_free(context);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_secure_new"));
  }

  if (BN_mod_add(result, mUnderlyingBigNum, other.mUnderlyingBigNum, modulo.mUnderlyingBigNum, context) <= 0)
  {
    BN_CTX_free(context);
    BN_clear_free(result);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_mod_add"));
  }

  BN_CTX_free(context);

  return BigNumber(result);
}

std::vector<unsigned char> BigNumber::toBytes() const
{
  char* hex = BN_bn2hex(mUnderlyingBigNum);
  std::string hexString(hex);
  OPENSSL_free(hex);

  std::vector<unsigned char> outputBytes = HexConverter::hexToBase64(hexString);

  return outputBytes;
}
} // Hedera::internal
