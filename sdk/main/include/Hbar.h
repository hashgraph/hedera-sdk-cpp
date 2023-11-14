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
#ifndef HEDERA_SDK_CPP_HBAR_H_
#define HEDERA_SDK_CPP_HBAR_H_

#include "HbarUnit.h"

#include <cmath>
#include <regex>

namespace Hedera
{
/**
 * Represents a quantity of hbar.
 *
 * Implemented as a wrapper class to force handling of units. Direct interfacing with Hedera accepts amounts in tinybars
 * however the nominal unit is hbar.
 */

// Define the pattern
const std::regex FROM_STRING_PATTERN("^((?:\\+|\\-)?\\d+(?:\\.\\d+)?)(\\ (tℏ|μℏ|mℏ|ℏ|kℏ|Mℏ|Gℏ))?$");

class Hbar
{
public:
  /**
   * Default constructor.
   */
  constexpr Hbar() = default;

  /**
   * Constructs a new Hbar of the specified value.
   *
   * @param amount The amount of Hbar.
   */
  constexpr explicit Hbar(const int64_t& amount)
    : Hbar(amount, HbarUnit::HBAR())
  {
  }

  /**
   * Constructs a new Hbar of the specified value in the specified unit.
   *
   * @param amount The amount.
   * @param unit   The unit for amount.
   */
  constexpr explicit Hbar(const int64_t& amount, const HbarUnit& unit)
    : mValueInTinybar(amount * static_cast<int64_t>(unit.getTinybars()))
  {
  }

  /**
   * Static function to create an Hbar instance from the specified number of tinybars.
   *
   * @param tinybars The amount of tinybars to convert to Hbar.
   * @return A new Hbar instance representing the specified number of tinybars.
   */
  static Hbar fromTinybars(int64_t tinybars) { return Hbar(tinybars, HbarUnit::TINYBAR()); }

  /**
   * Convert this Hbar value to a different unit and return it as an int64_t.
   *
   * @param unit The unit to convert to from Hbar.
   * @return An int64_t representing the converted value.
   */
  inline int64_t to(const HbarUnit& unit) const { return mValueInTinybar / unit.getTinybars(); }

  /**
   * Compare this Hbar to another Hbar and determine if they represent the same amount.
   *
   * @param other The other Hbar with which to compare this Hbar.
   * @return \c TRUE if this Hbar is the same amount as the input Hbar, otherwise \c FALSE.
   */
  bool operator==(const Hbar& other) const { return mValueInTinybar == other.mValueInTinybar; }

  /**
   * Add another Hbar to this Hbar.
   *
   * @param other THe other Hbar to add to this Hbar.
   * @return A new Hbar object with the new value.
   */
  Hbar operator+(const Hbar& other) const { return Hbar(mValueInTinybar + other.mValueInTinybar, HbarUnit::TINYBAR()); }

  /**
   * Add another Hbar to this Hbar and save the value.
   *
   * @param other The other Hbar to add to this Hbar.
   * @return A reference to this Hbar with the new value.
   */
  Hbar& operator+=(const Hbar& other)
  {
    mValueInTinybar += other.mValueInTinybar;
    return *this;
  }

  /**
   * Converts the provided string into an amount of Hbars.
   *
   * @param text The string representing the amount of Hbar.
   * @throws std::invalid_argument If the input string can not be converted to Hbar unit.
   * @return An Hbar instance.
   */
  [[nodiscard]] static Hbar fromString(const std::string& text);

  /**
   * Helper function to get the HbarUnit from the given symbol string.
   *
   * @param symbolString The symbol string representing the HbarUnit.
   * @return The corresponding HbarUnit.
   * @throws std::invalid_argument if the symbol is not recognized.
   */
  [[nodiscard]] static HbarUnit getUnit(const std::string& symbolString);

  /**
   * Convert this Hbar value to tinybars.
   *
   * @return The amount this Hbar object represents in tinybars.
   */
  [[nodiscard]] inline int64_t toTinybars() const { return mValueInTinybar; }

  /**
   * Convert this Hbar value to a string representation.
   *
   * @return A string representing the Hbar value.
   */
  std::string toString() const;

  /**
   * Returns an Hbar whose value is negative this Hbar.
   *
   * @return An Hbar object representing the negated value of this Hbar.
   */
  [[nodiscard]] inline Hbar negated() const { return Hbar(-mValueInTinybar, HbarUnit::TINYBAR()); }

private:
  /**
   * The value of this Hbar object in tinybars.
   */
  int64_t mValueInTinybar = 0LL;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_HBAR_H_
