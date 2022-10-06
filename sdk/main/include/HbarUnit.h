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
#ifndef HBAR_UNIT_H_
#define HBAR_UNIT_H_

#include <map>
#include <string>

namespace Hedera
{
/**
 * Common units of hbar; for the most part they follow SI prefix conventions.
 */
class HbarUnit
{
public:
  /**
   * The atomic (smallest) unit of hbar, used natively by the Hedera network.
   *
   * It is equivalent to 1/100,000,000 hbar.
   */
  static const HbarUnit TINYBAR;

  /**
   * Equivalent to 100 tinybar or 1/1,000,000 hbar.
   */
  static const HbarUnit MICROBAR;

  /**
   * Equivalent to 100,000 tinybar or 1/1,000 hbar.
   */
  static const HbarUnit MILLIBAR;

  /**
   * The base unit of hbar, equivalent to 100 million tinybar.
   */
  static const HbarUnit HBAR;

  /**
   * Equivalent to 1 thousand hbar or 100 billion tinybar.
   */
  static const HbarUnit KILOBAR;

  /**
   * Equivalent to 1 million hbar or 100 trillion tinybar.
   */
  static const HbarUnit MEGABAR;

  /**
   * Equivalent to 1 billion hbar or 100 quadillion tinybar.
   *
   * The maximum hbar amount supported by Hedera in any context is ~92 gigabar
   * (2^63 tinybar); use this unit sparingly.
   */
  static const HbarUnit GIGABAR;

  /**
   * Get the preferred symbol of the current unit.
   * E.g. HbarUnit::TINYBAR.getSymbol() returns "tℏ".
   *
   * @return The symbol of this unit
   */
  inline std::string getSymbol() const { return mSymbol; }

  /**
   * Get the value of this unit, in tinybars
   *
   * @return The unit value, in tinybars
   */
  inline long long getTinybars() const { return mTinybars; }

  /**
   * Get the name of this unit.
   * E.g. TINYBAR.toString() returns "tinybar".
   *
   * @return The name of this unit
   */
  inline std::string toString() const { return nameMap.at(this); }

private:
  /**
   * Constructor
   * @param sym  The symbol of the unit
   * @param val  The value of the unit, in tinybars
   */
  HbarUnit(const std::string& sym, const long long& val);

  /**
   * Keep a map of the unit names
   */
  static const std::map<const HbarUnit*, std::string> nameMap;

  /**
   * The associated symbol of this unit
   */
  std::string mSymbol;

  /**
   * The value of this unit, in tinybars
   */
  long long mTinybars;
};

} // namespace Hedera

#endif // HBAR_UNIT_H_