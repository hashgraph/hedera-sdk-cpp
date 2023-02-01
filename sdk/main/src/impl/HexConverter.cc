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
#include "impl/OpenSSLHasher.h"

#include <iomanip>
#include <openssl/crypto.h>
#include <sstream>
#include <stdexcept>

namespace Hedera::internal::HexConverter
{
//-----
std::string base64ToHex(const std::vector<unsigned char>& bytes)
{
  size_t stringLength;
  if (OPENSSL_buf2hexstr_ex(nullptr, 0, &stringLength, &bytes.front(), bytes.size(), '\0') <= 0)
  {
    throw std::runtime_error(OpenSSLHasher::getOpenSSLErrorMessage("OPENSSL_buf2hexstr_ex"));
  }

  std::string charString(stringLength, '\0');

  if (OPENSSL_buf2hexstr_ex(&charString.front(), stringLength, nullptr, &bytes.front(), bytes.size(), '\0') <= 0)
  {
    throw std::runtime_error(OpenSSLHasher::getOpenSSLErrorMessage("OPENSSL_buf2hexstr_ex"));
  }

  charString.pop_back(); // Remove the '\0'
  return charString;
}

//-----
std::vector<unsigned char> hexToBase64(const std::string& inputString)
{
  size_t bufferLength;
  if (OPENSSL_hexstr2buf_ex(nullptr, 0, &bufferLength, inputString.c_str(), '\0') <= 0)
  {
    throw std::runtime_error(OpenSSLHasher::getOpenSSLErrorMessage("OPENSSL_hexstr2buf_ex"));
  }

  std::vector<unsigned char> outputBytes(bufferLength);

  if (OPENSSL_hexstr2buf_ex((bufferLength > 0ULL) ? &outputBytes.front() : nullptr,
                            bufferLength,
                            &bufferLength,
                            inputString.c_str(),
                            '\0') <= 0)
  {
    throw std::runtime_error(OpenSSLHasher::getOpenSSLErrorMessage("OPENSSL_hexstr2buf_ex"));
  }

  return outputBytes;
}

//-----
std::string bytesToHex(const std::vector<unsigned char>& bytes)
{
  std::stringstream stream;
  stream << std::hex;

  for (unsigned char byte : bytes)
  {
    stream << std::setw(2) << std::setfill('0') << (int)byte;
  }

  return stream.str();
}

} // namespace Hedera::internal::HexConverter
