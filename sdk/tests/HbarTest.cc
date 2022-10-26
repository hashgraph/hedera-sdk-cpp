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
#include "Hbar.h"

#include <gtest/gtest.h>

using namespace Hedera;

class HbarTest : public ::testing::Test
{
protected:
  int64_t amount = 6ULL;
};

TEST_F(HbarTest, DefaultConstructor)
{
  Hbar hbar;
  EXPECT_EQ(hbar.toTinybars(), 0ULL);
}

TEST_F(HbarTest, AmountConstructor)
{
  Hbar hbar(amount);
  EXPECT_EQ(hbar.toTinybars(), amount * HbarUnit::HBAR().getTinybars());
}

TEST_F(HbarTest, AmountUnitConstructor)
{
  Hbar tinybar(amount, HbarUnit::TINYBAR());
  Hbar microbar(amount, HbarUnit::MICROBAR());
  Hbar millibar(amount, HbarUnit::MILLIBAR());
  Hbar kilobar(amount, HbarUnit::KILOBAR());
  Hbar megabar(amount, HbarUnit::MEGABAR());
  Hbar gigabar(amount, HbarUnit::GIGABAR());

  EXPECT_EQ(tinybar.toTinybars(), amount * HbarUnit::TINYBAR().getTinybars());
  EXPECT_EQ(microbar.toTinybars(), amount * HbarUnit::MICROBAR().getTinybars());
  EXPECT_EQ(millibar.toTinybars(), amount * HbarUnit::MILLIBAR().getTinybars());
  EXPECT_EQ(kilobar.toTinybars(), amount * HbarUnit::KILOBAR().getTinybars());
  EXPECT_EQ(megabar.toTinybars(), amount * HbarUnit::MEGABAR().getTinybars());
  EXPECT_EQ(gigabar.toTinybars(), amount * HbarUnit::GIGABAR().getTinybars());
}

TEST_F(HbarTest, Negated)
{
  Hbar hbar(amount);
  Hbar hbarNegated(-amount);
  EXPECT_EQ(hbar.negated().toTinybars(), hbarNegated.toTinybars());
  EXPECT_EQ(hbar.toTinybars(), hbarNegated.negated().toTinybars());
}
