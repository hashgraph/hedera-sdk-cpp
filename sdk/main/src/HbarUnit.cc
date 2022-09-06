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
#include "HbarUnit.h"

namespace Hedera
{
const HbarUnit HbarUnit::TINYBAR("tℏ", 1ULL);
const HbarUnit HbarUnit::MICROBAR("μℏ", 100ULL);
const HbarUnit HbarUnit::MILLIBAR("mℏ", 100000ULL);
const HbarUnit HbarUnit::HBAR("ℏ",  100000000ULL);
const HbarUnit HbarUnit::KILOBAR("kℏ", 100000000000ULL);
const HbarUnit HbarUnit::MEGABAR("Mℏ", 100000000000000ULL);
const HbarUnit HbarUnit::GIGABAR("Gℏ", 100000000000000000ULL);

HbarUnit::HbarUnit(const std::string& sym, const unsigned long long& val) :
    symbol(sym),
    value(val)
{
}

} // namespace Hedera