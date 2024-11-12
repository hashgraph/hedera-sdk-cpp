/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "exceptions/IllegalStateException.h"

#include <stdexcept>

namespace Hiero
{
//-----
IPv4Address IPv4Address::fromBytes(const std::vector<std::byte>& bytes)
{
  if (bytes.size() != 4)
  {
    throw std::invalid_argument("Incorrect byte array size, should be 4 bytes but is " + std::to_string(bytes.size()));
  }

  IPv4Address iPv4Address;

  for (const auto& byte : bytes)
  {
    iPv4Address.mAddress.push_back(byte);
  }
  return iPv4Address;
}

//-----
std::vector<std::byte> IPv4Address::toBytes() const
{
  return mAddress;
}

//-----
std::string IPv4Address::toString() const
{
  if (mAddress.size() != 4)
  {
    throw IllegalStateException("Incorrect byte array size, should be 4 bytes but is " +
                                std::to_string(mAddress.size()));
  }
  else
  {
    return std::to_string(std::to_integer<unsigned char>(mAddress.at(0))) + '.' +
           std::to_string(std::to_integer<unsigned char>(mAddress.at(1))) + '.' +
           std::to_string(std::to_integer<unsigned char>(mAddress.at(2))) + '.' +
           std::to_string(std::to_integer<unsigned char>(mAddress.at(3)));
  }
}

//-----
bool IPv4Address::isEmpty() const
{
  return mAddress.empty();
}

} // namespace Hiero
