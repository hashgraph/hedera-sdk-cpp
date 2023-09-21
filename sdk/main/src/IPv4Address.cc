/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "IPv4Address.h"

#include <stdexcept>

namespace Hedera
{
//-----
IPv4Address IPv4Address::fromBytes(const std::vector<std::byte>& bytes)
{
  if (bytes.size() != 4)
  {
    throw std::invalid_argument("Incorrect byte array size, should be 4 bytes but is " + std::to_string(bytes.size()));
  }

  IPv4Address iPv4Address;
  std::copy(bytes.cbegin(), bytes.cend(), iPv4Address.mAddress.begin());
  return iPv4Address;
}

//-----
std::vector<std::byte> IPv4Address::toBytes() const
{
  return { mAddress.at(0), mAddress.at(1), mAddress.at(2), mAddress.at(3) };
}

//-----
std::string IPv4Address::toString() const
{
  return std::to_string(std::to_integer<unsigned char>(mAddress.at(0))) + '.' +
         std::to_string(std::to_integer<unsigned char>(mAddress.at(1))) + '.' +
         std::to_string(std::to_integer<unsigned char>(mAddress.at(2))) + '.' +
         std::to_string(std::to_integer<unsigned char>(mAddress.at(3)));
}

} // namespace Hedera
