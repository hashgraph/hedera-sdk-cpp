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
#ifndef HBAR_H_
#define HBAR_H_

#include "HbarUnit.h"

#include <cmath>

namespace Hedera
{
/**
 * Represents a quantity of hbar.
 *
 * Implemented as a wrapper class to force handling of units. Direct interfacing with Hedera accepts amounts in tinybars
 * however the nominal unit is hbar.
 */
class Hbar
{
public:
  /**
   * Default constructor.
   */
  Hbar() = default;

  /**
   * Constructs a new Hbar of the specified value.
   *
   * @param amount The amount of Hbar.
   */
  explicit Hbar(const int64_t& amount)
    : Hbar(amount, HbarUnit::HBAR())
  {
  }

  /**
   * Constructs a new Hbar of the specified value in the specified unit.
   *
   * @param amount The amount.
   * @param unit   The unit for amount.
   */
  explicit Hbar(const int64_t& amount, const HbarUnit& unit)
    : mValueInTinybar(amount * unit.getTinybars())
  {
  }

  /**
   * Convert this Hbar value to tinybars.
   *
   * @return The amount this Hbar object represents in tinybars.
   */
  inline int64_t toTinybars() const { return mValueInTinybar; }

  /**
   * Returns an Hbar whose value is negative this Hbar.
   *
   * @return An Hbar object representing the negated value of this Hbar.
   */
  inline Hbar negated() const { return Hbar(-mValueInTinybar, HbarUnit::TINYBAR()); }

private:
  /**
   * The value of this Hbar object in tinybars.
   */
  int64_t mValueInTinybar = 0;
};

} // namespace Hedera

#endif // HBAR_H_
