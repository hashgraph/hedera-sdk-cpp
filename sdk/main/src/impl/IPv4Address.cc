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
#include "impl/IPv4Address.h"
#include "impl/Utilities.h"

#include <charconv>
#include <iostream>
#include <vector>

namespace Hedera::internal
{
IPv4Address::IPv4Address(std::byte octet1, std::byte octet2, std::byte octet3, std::byte octet4)
  : mOctet1(octet1)
  , mOctet2(octet2)
  , mOctet3(octet3)
  , mOctet4(octet4)
{
}

IPv4Address IPv4Address::fromString(std::string_view address)
{
  // The input string is in byte format, where each byte represents a single IP address octet.
  if (address.size() == NUM_BYTES)
  {
    const std::vector<std::byte> byteVector = Utilities::stringToByteVector(address);
    return { byteVector.at(0), byteVector.at(1), byteVector.at(2), byteVector.at(3) };
  }

  try
  {
    std::vector<std::byte> byteVector;
    std::string_view octet;
    for (int i = 0; i < NUM_BYTES; ++i)
    {
      octet = address.substr(0, address.find_first_of('.'));
      byteVector.push_back(Utilities::stringToByte(octet));
      address.remove_prefix(octet.size() + 1);
    }

    return { byteVector.at(0), byteVector.at(1), byteVector.at(2), byteVector.at(3) };
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Input IPv4Address is malformed");
  }
}

std::string IPv4Address::toString() const
{
  return std::to_string(std::to_integer<unsigned char>(mOctet1)) + '.' +
         std::to_string(std::to_integer<unsigned char>(mOctet2)) + '.' +
         std::to_string(std::to_integer<unsigned char>(mOctet3)) + '.' +
         std::to_string(std::to_integer<unsigned char>(mOctet4));
}

} // namespace Hedera::internal
