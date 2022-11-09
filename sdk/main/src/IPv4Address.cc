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
  // input string is in byte format, where each byte represents a single IP address octet
  if (inputString.size() == 4)
  {
    std::vector<unsigned int> byteVector = { inputString.cbegin(), inputString.cend() };

    unsigned int mask = 0x000000FF;

    IPv4Address outputAddress = {
      byteVector.at(0) & mask, byteVector.at(1) & mask, byteVector.at(2) & mask, byteVector.at(3) & mask
    };

    return outputAddress;
  }

  // if input string isn't bytes, it must be made up of number characters, with 4 octets separated by the '.' character
  if (std::count(inputString.begin(), inputString.end(), '.') != 3)
  {
    // todo THROW
    return { 0, 0, 0, 0 };
  }

  std::vector<unsigned int> byteVector;

  int previousDelimiter = -1;
  int nextDelimiter = (int)inputString.find('.');

  for (int i = 0; i < 4; ++i)
  {
    std::string byteString = inputString.substr(previousDelimiter + 1, nextDelimiter - previousDelimiter - 1);

    if (!byteString.length())
    {
      // todo THROW
      return { 0, 0, 0, 0 };
    }

    for (char character : byteString)
    {
      if (!std::isdigit(character))
      {
        // todo THROW
        return { 0, 0, 0, 0 };
      }
    }

    unsigned int byteInt = std::stoi(byteString);
    if (byteInt > 255)
    {
      // todo THROW
      return { 0, 0, 0, 0 };
    }

    byteVector.push_back(byteInt);

    previousDelimiter = nextDelimiter;
    nextDelimiter = (int)inputString.find('.', previousDelimiter + 1);
  }

  return { byteVector.at(0), byteVector.at(1), byteVector.at(2), byteVector.at(3) };
}

std::string IPv4Address::toString() const
{
  std::stringstream outputStream;

  outputStream << mOctet1 << "." << mOctet2 << "." << mOctet3 << "." << mOctet4;

  return outputStream.str();
}

} // Hedera