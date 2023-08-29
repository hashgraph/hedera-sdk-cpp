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
#include "LedgerId.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"

#include <stdexcept>

namespace Hedera
{
//-----
const LedgerId LedgerId::MAINNET = LedgerId({ std::byte(0x00) });

//-----
const LedgerId LedgerId::TESTNET = LedgerId({ std::byte(0x01) });

//-----
const LedgerId LedgerId::PREVIEWNET = LedgerId({ std::byte(0x02) });

//-----
LedgerId::LedgerId(std::vector<std::byte> bytes)
  : mLedgerId(std::move(bytes))
{
}

//-----
bool LedgerId::operator==(const LedgerId& other) const
{
  return mLedgerId == other.mLedgerId;
}

//-----
LedgerId LedgerId::fromString(std::string_view id)
{
  if (id == "mainnet")
  {
    return MAINNET;
  }
  else if (id == "testnet")
  {
    return TESTNET;
  }
  else if (id == "previewnet")
  {
    return PREVIEWNET;
  }
  else
  {
    try
    {
      return LedgerId(internal::HexConverter::hexToBytes(id));
    }
    catch (const OpenSSLException&)
    {
      throw std::invalid_argument("LedgerId cannot be realized from input hex-encoded string");
    }
  }
}

//-----
std::string LedgerId::toString() const
{
  if (isMainnet())
  {
    return "mainnet";
  }
  else if (isTestnet())
  {
    return "testnet";
  }
  else if (isPreviewnet())
  {
    return "previewnet";
  }
  else
  {
    return internal::HexConverter::bytesToHex(mLedgerId);
  }
}

} // namespace Hedera
