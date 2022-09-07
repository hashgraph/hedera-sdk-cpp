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
const HbarUnit HbarUnit::TINYBAR("tℏ", 1LL);
const HbarUnit HbarUnit::MICROBAR("μℏ", 100LL);
const HbarUnit HbarUnit::MILLIBAR("mℏ", 100000LL);
const HbarUnit HbarUnit::HBAR("ℏ",  100000000LL);
const HbarUnit HbarUnit::KILOBAR("kℏ", 100000000000LL);
const HbarUnit HbarUnit::MEGABAR("Mℏ", 100000000000000LL);
const HbarUnit HbarUnit::GIGABAR("Gℏ", 100000000000000000LL);

const std::map<const HbarUnit*, std::string> HbarUnit::nameMap = {
    { &TINYBAR, "tinybar" },
    { &MICROBAR, "microbar" },
    { &MILLIBAR, "millibar" },
    { &HBAR, "hbar" },
    { &KILOBAR, "kilobar" },
    { &MEGABAR, "megabar" },
    { &GIGABAR, "gigabar" }
};

HbarUnit::HbarUnit(const std::string& sym, const long long& val) :
    mSymbol(sym),
    mTinybars(val)
{
}

} // namespace Hedera