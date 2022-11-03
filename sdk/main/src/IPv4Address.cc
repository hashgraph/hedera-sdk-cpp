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

#include "IPv4Address.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace Hedera
{

IPv4Address::IPv4Address(unsigned int octet1, unsigned int octet2, unsigned int octet3, unsigned int octet4)
{
  mOctet1 = octet1;
  mOctet2 = octet2;
  mOctet3 = octet3;
  mOctet4 = octet4;
}

IPv4Address IPv4Address::fromString(const std::string& inputString)
{
  std::vector<unsigned int> byteVector = {inputString.cbegin(), inputString.cend()};

  if (inputString.size() != 4)
  {
    // TODO throw?
    return { 0, 0, 0, 0 };
  }

  unsigned int mask = 0x000000FF;

  return { byteVector[0] & mask, byteVector[1] & mask, byteVector[2] & mask, byteVector[3] & mask };
}

std::string IPv4Address::toString() const
{
  std::stringstream outputStream;

  outputStream << mOctet1 << "." << mOctet2 << "." << mOctet3 << "." << mOctet4;

  return outputStream.str();
}

} // Hedera