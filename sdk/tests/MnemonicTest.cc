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

#include "MnemonicBIP39.h"

#include <gtest/gtest.h>

using namespace Hedera;

class MnemonicTest : public ::testing::Test
{
};

TEST_F(MnemonicTest, DefaultConstructContractId)
{
  MnemonicBIP39 mnemonic =
    MnemonicBIP39::createBIP39Mnemonic("inmate flip alley wear offer often piece magnet surge toddler submit right "
                                       "radio absent pear floor belt raven price stove replace reduce plate home");
}