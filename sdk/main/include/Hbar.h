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
#ifndef HBAR_H_
#define HBAR_H_

#include <cmath>

#include "HbarUnit.h"

namespace Hedera
{
/**
 * Represents a quantity of hbar.
 *
 * Implemented as a wrapper class to force handling of units. Direct interfacing with Hedera accepts amounts
 * in tinybars however the nominal unit is hbar.
 */
class Hbar
{
public:
    /**
     * Constructs a new Hbar with a value of 0.
     */
    Hbar() : mValueInTinybar(0) {}

    /**
     * Constructs a new Hbar of the specified value.
     *
     * @param amount The amount of Hbar
     */
    explicit Hbar(const long long& amount)
        : Hbar(amount, HbarUnit::HBAR) {}

    /**
     * Constructs a new Hbar of the specified value in the specified unit.
     *
     * @param amount The amount
     * @param unit   The unit for amount
     */
    explicit Hbar(const long long& amount, const HbarUnit& unit)
        : mValueInTinybar(amount * unit.getTinybars()) {}

    /**
     * Constructs a new Hbar of the specified, possibly fractional value.
     *
     * The equivalent amount in tinybar must be an integer and fit in a long (64-bit signed integer).
     *
     * E.g. 1.23456789 is a valid amount of hbar but 0.123456789 is not.
     *
     * @param amount The amount of Hbar
     */
    explicit Hbar(const double& amount)
        : Hbar(amount, HbarUnit::HBAR) {}

    /**
     * Constructs a new Hbar of the specified value in the specified unit.
     *
     * @param amount The amount
     * @param unit   The unit for amount
     */
    explicit Hbar(const double& amount, const HbarUnit& unit)
    {
        const double value = amount * unit.getTinybars();
        if (floor(value) != value)
        {
            // TODO: resolve
        }
    }

    /**
     * Returns an Hbar whose value is equal to the specified long.
     *
     * @param amount The amount of Hbar
     * @return       An Hbar object with the specified amount
     */
    static inline Hbar from(const long long& amount)
    {
        return Hbar(amount, HbarUnit::HBAR);
    }

    /**
     * Returns an Hbar representing the value in the given units.
     *
     * @param amount The long long representing the amount of set units
     * @param unit   The unit to convert from to Hbar
     * @return       An Hbar object with the specified amount and unit
     */
    static inline Hbar from(const long long& amount, const HbarUnit& unit)
    {
        return Hbar(amount, unit);
    }

    /**
     * Returns an Hbar whose value is equal to the specified double
     *
     * @param amount The double representing the amount of Hbar
     * @return       An Hbar object with the specified amount
     */
    static inline Hbar from(const double& amount)
    {
        return Hbar(amount, HbarUnit::HBAR);
    }

    /**
     * Returns an Hbar representing the value in the given units.
     *
     * @param amount The double representing the amount of set units
     * @param unit   The unit to convert from to Hbar
     * @return       An Hbar object with the specified amount and unit
     */
    static inline Hbar from(const double& amount, const HbarUnit& unit)
    {
        return Hbar(amount, unit);
    }

    /**
     * Returns an Hbar converted from the specified number of tinybars.
     *
     * @param tinybars The long representing the amount of tinybar
     * @return         An Hbar object with the specified amount and unit
     */
    static inline Hbar fromTinybars(const long long& tinybars)
    {
        return Hbar(tinybars, HbarUnit::TINYBAR);
    }

    /**
     * Convert this hbar value to Tinybars.
     *
     * @return The amount this Hbar object contains in tinybars
     */
    inline long long toTinybars() const
    {
        return mValueInTinybar;
    }

    /**
     * Convert this hbar value to a different unit.
     *
     * @param unit The unit to convert to from Hbar
     * @return     The amount this Hbar object contains in the specified unit
     */
    double to(const HbarUnit& unit) const
    {
        // TODO: finish this
        return 0.0;
    }

    /**
     * Returns the number of Hbars.
     *
     * @return The amount this Hbar object contains in Hbars
     */
    inline double getValue() const 
    {
        return to(HbarUnit::HBAR);
    }

    /**
     * Returns an Hbar whose value is negative this
     *
     * @return An Hbar object representating the negated value of this Hbar
     */
    inline Hbar negated() const
    {
        return Hbar::fromTinybars(-mValueInTinybar);
    }

    /**
     * Convert this Hbar object to string representation
     *
     * @return A string that contains the amount of Hbar (or tinybar if small enough)
     *         and symbol
     */
    inline std::string toString() const
    {
        if (mValueInTinybar < 10000LL && mValueInTinybar > -10000LL)
        {
            return std::to_string(mValueInTinybar) + ' ' + HbarUnit::TINYBAR.getSymbol();
        }

        return std::to_string(to(HbarUnit::HBAR)) + ' ' + HbarUnit::HBAR.getSymbol();
    }

    /**
     * Convert this Hbar to string representation in specified units
     *
     * @param unit The desired unit
     * @return     The string representation
     */
    inline std::string toString(const HbarUnit& unit)
    {
        return std::to_string(to(unit)) + ' ' + unit.getSymbol();
    }

    inline bool operator==(const Hbar& hbar)
    {
        return this->mValueInTinybar == hbar.mValueInTinybar;
    }

private:
    /**
     * The value of this Hbar object in tinybars
     */
    long long mValueInTinybar;
};

} // namespace Hedera

#endif // HBAR_H_