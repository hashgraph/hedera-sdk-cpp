/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include "openssl/sha.h"

#include "helper/OpenSSLHasher.h"

namespace Hedera
{
std::vector<unsigned char> OpenSSLHasher::computeSHA384(const std::string& data)
{
  std::vector<unsigned char> outputBytes = std::vector<unsigned char>(48);

  const unsigned char* rawData = (unsigned char*)data.c_str();
  SHA384(rawData, data.size(), &outputBytes.front());

  return outputBytes;
}

std::vector<unsigned char> OpenSSLHasher::computeSHA256(const std::vector<unsigned char>& data)
{
  std::vector<unsigned char> outputBytes = std::vector<unsigned char>(32);

  const unsigned char* rawData = &data.front();
  SHA256(rawData, data.size(), &outputBytes.front());

  return outputBytes;
}
} // Hedera
