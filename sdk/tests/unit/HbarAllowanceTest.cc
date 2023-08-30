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
#include "HbarAllowance.h"
#include "AccountId.h"
#include "Hbar.h"

#include <gtest/gtest.h>
#include <proto/crypto_approve_allowance.pb.h>

using namespace Hedera;

class HbarAllowanceTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const AccountId& getTestSpenderAccountId() const { return mSpenderAccountId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }

private:
  const AccountId mOwnerAccountId = AccountId(1ULL);
  const AccountId mSpenderAccountId = AccountId(2ULL);
  const Hbar mAmount = Hbar(3LL);
};

//-----
TEST_F(HbarAllowanceTest, ConstructWithOwnerSpenderAmount)
{
  // Given / When
  const HbarAllowance hbarAllowance(getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmount());

  // Then
  EXPECT_EQ(hbarAllowance.getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(hbarAllowance.getSpenderAccountId(), getTestSpenderAccountId());
  EXPECT_EQ(hbarAllowance.getAmount(), getTestAmount());
}

//-----
TEST_F(HbarAllowanceTest, GetSetOwnerAccountId)
{
  // Given
  HbarAllowance hbarAllowance;

  // When
  hbarAllowance.setOwnerAccountId(getTestOwnerAccountId());

  // Then
  EXPECT_EQ(hbarAllowance.getOwnerAccountId(), getTestOwnerAccountId());
}

//-----
TEST_F(HbarAllowanceTest, GetSetSpenderAccountId)
{
  // Given
  HbarAllowance hbarAllowance;

  // When
  hbarAllowance.setSpenderAccountId(getTestSpenderAccountId());

  // Then
  EXPECT_EQ(hbarAllowance.getSpenderAccountId(), getTestSpenderAccountId());
}

//-----
TEST_F(HbarAllowanceTest, GetSetAmount)
{
  // Given
  HbarAllowance hbarAllowance;

  // When
  hbarAllowance.setAmount(getTestAmount());

  // Then
  EXPECT_EQ(hbarAllowance.getAmount(), getTestAmount());
}

//-----
TEST_F(HbarAllowanceTest, FromProtobuf)
{
  // Given
  proto::CryptoAllowance cryptoAllowance;
  cryptoAllowance.set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  cryptoAllowance.set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());
  cryptoAllowance.set_amount(getTestAmount().toTinybars());

  // When
  const HbarAllowance hbarAllowance = HbarAllowance::fromProtobuf(cryptoAllowance);

  // Then
  EXPECT_EQ(hbarAllowance.getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(hbarAllowance.getSpenderAccountId(), getTestSpenderAccountId());
  EXPECT_EQ(hbarAllowance.getAmount(), getTestAmount());
}

//-----
TEST_F(HbarAllowanceTest, ToProtobuf)
{
  // Given
  HbarAllowance hbarAllowance;
  hbarAllowance.setOwnerAccountId(getTestOwnerAccountId());
  hbarAllowance.setSpenderAccountId(getTestSpenderAccountId());
  hbarAllowance.setAmount(getTestAmount());

  // When
  const std::unique_ptr<proto::CryptoAllowance> cryptoAllowance = hbarAllowance.toProtobuf();

  // Then
  ASSERT_TRUE(cryptoAllowance->has_owner());
  EXPECT_EQ(AccountId::fromProtobuf(cryptoAllowance->owner()), getTestOwnerAccountId());
  ASSERT_TRUE(cryptoAllowance->has_spender());
  EXPECT_EQ(AccountId::fromProtobuf(cryptoAllowance->spender()), getTestSpenderAccountId());
  EXPECT_EQ(cryptoAllowance->amount(), getTestAmount().toTinybars());
}
