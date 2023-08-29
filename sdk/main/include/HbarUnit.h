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
#ifndef HBAR_UNIT_H_
#define HBAR_UNIT_H_

#include <map>
#include <string>

namespace Hedera
{
/**
 * Common units of Hbar; for the most part they follow SI prefix conventions.
 */
class HbarUnit
{
public:
  /**
   * The atomic (smallest) unit of hbar, used natively by the Hedera network. It is equivalent to 1/100,000,000 hbar.
   */
  static constexpr HbarUnit TINYBAR() { return { "tℏ", 1ULL }; }

  /**
   * Equivalent to 100 tinybar or 1/1,000,000 hbar.
   */
  static constexpr HbarUnit MICROBAR() { return { "μℏ", 100ULL }; }

  /**
   * Equivalent to 100,000 tinybar or 1/1,000 hbar.
   */
  static constexpr HbarUnit MILLIBAR() { return { "mℏ", 100000ULL }; }

  /**
   * The base unit of hbar, equivalent to 100 million tinybar.
   */
  static constexpr HbarUnit HBAR() { return { "ℏ", 100000000ULL }; }

  /**
   * Equivalent to 1 thousand hbar or 100 billion tinybar.
   */
  static constexpr HbarUnit KILOBAR() { return { "kℏ", 100000000000ULL }; }

  /**
   * Equivalent to 1 million hbar or 100 trillion tinybar.
   */
  static constexpr HbarUnit MEGABAR() { return { "Mℏ", 100000000000000ULL }; }

  /**
   * Equivalent to 1 billion hbar or 100 quadrillion tinybar. The maximum hbar amount supported by Hedera in any context
   * is ~92 gigabar (2^63 tinybar); use this unit sparingly.
   */
  static constexpr HbarUnit GIGABAR() { return { "Gℏ", 100000000000000000ULL }; }

  /**
   * Get the symbol of the current unit.
   *
   * @return The symbol of this unit.
   */
  [[nodiscard]] constexpr const char* getSymbol() const { return mSymbol; }

  /**
   * Get the value of this unit, in tinybars.
   *
   * @return The unit value, in tinybars.
   */
  [[nodiscard]] constexpr uint64_t getTinybars() const { return mTinybars; }

private:
  /**
   * Construct with a symbol and a value.
   * @param sym The symbol of the unit
   * @param val The value of the unit, in tinybars
   */
  constexpr HbarUnit(const char* sym, const uint64_t& val)
    : mSymbol(sym)
    , mTinybars(val)
  {
  }

  /**
   * The associated symbol of this unit
   */
  const char* mSymbol;

  /**
   * The value of this unit, in tinybars
   */
  const uint64_t mTinybars;
};

} // namespace Hedera

#endif // HBAR_UNIT_H_