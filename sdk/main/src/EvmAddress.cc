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
#include "EvmAddress.h"
#include "impl/HexConverter.h"

#include <stdexcept>

namespace Hedera
{
//-----
EvmAddress EvmAddress::fromString(std::string_view address)
{
  EvmAddress evmAddress;

  // Check the input string size. Since it takes two characters to represent a byte with hex-encoding, the string length
  // should be 2 * NUM_BYTES. However, the address could contain a "0x" prefix, so the length could be 2 * NUM_BYTES
  // + prefix.size()
  const std::string_view prefix = "0x";
  const bool hasPrefix = address.substr(0, prefix.size()) == prefix;
  if ((hasPrefix && address.size() != 2 * NUM_BYTES + prefix.size()) || (!hasPrefix && address.size() != 2 * NUM_BYTES))
  {
    throw std::invalid_argument("Input EVM address string is not the correct size");
  }

  // Remove the prefix if it exists
  if (hasPrefix)
  {
    address.remove_prefix(prefix.size());
  }

  // Ensure all digits are hex digits
  if (!std::all_of(address.cbegin(), address.cend(), [](unsigned char c) { return std::isxdigit(c); }))
  {
    throw std::invalid_argument("Input EVM address is not a hex-encoded string");
  }

  evmAddress.mEvmAddress = internal::HexConverter::hexToBase64(std::string(address));
  evmAddress.checkEvmAddress();

  return evmAddress;
}

//-----
EvmAddress EvmAddress::fromBytes(const std::vector<unsigned char>& bytes)
{
  EvmAddress evmAddress;
  evmAddress.mEvmAddress = bytes;
  evmAddress.checkEvmAddress();
  return evmAddress;
}

//-----
std::string EvmAddress::toString() const
{
  return internal::HexConverter::bytesToHex(mEvmAddress);
}

//-----
std::vector<unsigned char> EvmAddress::toBytes() const
{
  return mEvmAddress;
}

//-----
void EvmAddress::checkEvmAddress() const
{
  if (mEvmAddress.size() != NUM_BYTES)
  {
    throw std::invalid_argument("EVM address is not 20 bytes");
  }
}

} // namespace Hedera
