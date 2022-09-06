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
#include <string>

namespace Hedera
{
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

private:
    /**
     * Constructor
     * @param sym  The symbol of the unit
     * @param val  The value of the unit, in tinybars
     */
    HbarUnit(const std::string& sym, const unsigned long long& val);

    /**
     * The associated symbol of this unit
     * 
     */
    std::string symbol;

    /**
     * The value of this unit
     * 
     */
    unsigned long long value;
};
    
} // namespace Hedera