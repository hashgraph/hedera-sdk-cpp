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
#include "Hbar.h"
#include "HbarUnit.h"

#include <regex>
#include <sstream>
#include <stdexcept>

// Define the pattern
namespace Const
{
const std::regex FROM_STRING_PATTERN("^((?:\\+|\\-)?\\d+(?:\\.\\d+)?)(\\ (tℏ|μℏ|mℏ|ℏ|kℏ|Mℏ|Gℏ))?$");
}

namespace Hedera
{
//-----
std::string Hbar::toString() const
{
  if (mValueInTinybar < 10000ULL && mValueInTinybar > -10000ULL)
  {
    // Value is in tinybars
    return std::to_string(mValueInTinybar) + " " + HbarUnit::TINYBAR().getSymbol();
  }

  // Convert to Hbar and return the value in Hbar unit
  return std::to_string(to(HbarUnit::HBAR())) + " " + HbarUnit::HBAR().getSymbol();
}

//-----
Hbar Hbar::fromString(const std::string& text)
{
  std::smatch match;
  if (!std::regex_match(text, match, Const::FROM_STRING_PATTERN))
  {
    throw std::invalid_argument("Attempted to convert string to Hbar, but \"" + text +
                                "\" was not correctly formatted");
  }

  // Extract parts from the match
  std::string amountStr = match[1].str();
  std::string unitStr = match[3].str();

  // Convert amount string to BigDecimal
  std::istringstream amountStream(amountStr);
  int64_t amount;
  amountStream >> amount;

  // Determine the unit
  HbarUnit unit = (unitStr.empty()) ? HbarUnit::HBAR() : Hbar::getUnit(unitStr);

  return Hbar(amount, unit);
}

//-----
HbarUnit Hbar::getUnit(const std::string& symbolString)
{
  if (symbolString == HbarUnit::TINYBAR().getSymbol())
  {
    return HbarUnit::TINYBAR();
  }
  else if (symbolString == HbarUnit::MICROBAR().getSymbol())
  {
    return HbarUnit::MICROBAR();
  }
  else if (symbolString == HbarUnit::MILLIBAR().getSymbol())
  {
    return HbarUnit::MILLIBAR();
  }
  else if (symbolString == HbarUnit::HBAR().getSymbol())
  {
    return HbarUnit::HBAR();
  }
  else if (symbolString == HbarUnit::KILOBAR().getSymbol())
  {
    return HbarUnit::KILOBAR();
  }
  else if (symbolString == HbarUnit::MEGABAR().getSymbol())
  {
    return HbarUnit::MEGABAR();
  }
  else if (symbolString == HbarUnit::GIGABAR().getSymbol())
  {
    return HbarUnit::GIGABAR();
  }

  throw std::invalid_argument("Attempted to convert string to Hbar, but unit symbol \"" + symbolString +
                              "\" was not recognized");
}

} // namespace Hedera
