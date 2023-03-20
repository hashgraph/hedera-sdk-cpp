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
#include "impl/HexConverter.h"
#include "exceptions/OpenSSLException.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/crypto.h>
#include <stdexcept>

namespace Hedera::internal::HexConverter
{
//-----
std::string bytesToHex(const std::vector<std::byte>& bytes)
{
  size_t stringLength;
  if (OPENSSL_buf2hexstr_ex(
        nullptr, 0, &stringLength, OpenSSLUtils::toUnsignedCharPtr(bytes.data()), bytes.size(), '\0') <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OPENSSL_buf2hexstr_ex"));
  }

  std::string charString(stringLength, '\0');
  if (OPENSSL_buf2hexstr_ex(
        charString.data(), stringLength, nullptr, OpenSSLUtils::toUnsignedCharPtr(bytes.data()), bytes.size(), '\0') <=
      0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OPENSSL_buf2hexstr_ex"));
  }

  charString.pop_back(); // Remove the '\0'
  return charString;
}

//-----
std::vector<std::byte> hexToBytes(std::string_view hex)
{
  size_t bufferLength;
  if (OPENSSL_hexstr2buf_ex(nullptr, 0, &bufferLength, hex.data(), '\0') <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OPENSSL_hexstr2buf_ex"));
  }

  std::vector<std::byte> outputBytes(bufferLength);
  if (OPENSSL_hexstr2buf_ex(
        OpenSSLUtils::toUnsignedCharPtr(outputBytes.data()), bufferLength, nullptr, hex.data(), '\0') <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OPENSSL_hexstr2buf_ex"));
  }

  return outputBytes;
}

} // namespace Hedera::internal::HexConverter
