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
#include "AccountId.h"
#include "TokenAllowance.h"

#include <gtest/gtest.h>
#include <proto/crypto_approve_allowance.pb.h>

using namespace Hedera;

class TokenAllowanceUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const AccountId& getTestSpenderAccountId() const { return mSpenderAccountId; }
  [[nodiscard]] inline uint64_t getTestAmount() const { return mAmount; }

private:
  const TokenId mTokenId = TokenId(1ULL);
  const AccountId mOwnerAccountId = AccountId(2ULL);
  const AccountId mSpenderAccountId = AccountId(3ULL);
  const uint64_t mAmount = 4ULL;
};

//-----
TEST_F(TokenAllowanceUnitTests, ConstructWithTokenIdOwnerSpenderAmount)
{
  // Given / When
  const TokenAllowance tokenAllowance(
    getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmount());

  // Then
  EXPECT_EQ(tokenAllowance.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenAllowance.mOwnerAccountId, getTestOwnerAccountId());
  EXPECT_EQ(tokenAllowance.mSpenderAccountId, getTestSpenderAccountId());
  EXPECT_EQ(tokenAllowance.mAmount, getTestAmount());
}

//-----
TEST_F(TokenAllowanceUnitTests, FromProtobuf)
{
  // Given
  proto::TokenAllowance protoTokenAllowance;
  protoTokenAllowance.set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  protoTokenAllowance.set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  protoTokenAllowance.set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());
  protoTokenAllowance.set_amount(static_cast<int64_t>(getTestAmount()));

  // When
  const TokenAllowance tokenAllowance = TokenAllowance::fromProtobuf(protoTokenAllowance);

  // Then
  EXPECT_EQ(tokenAllowance.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenAllowance.mOwnerAccountId, getTestOwnerAccountId());
  EXPECT_EQ(tokenAllowance.mSpenderAccountId, getTestSpenderAccountId());
  EXPECT_EQ(tokenAllowance.mAmount, getTestAmount());
}

//-----
TEST_F(TokenAllowanceUnitTests, ToProtobuf)
{
  // Given
  const TokenAllowance tokenAllowance(
    getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmount());

  // When
  const std::unique_ptr<proto::TokenAllowance> protoTokenAllowance = tokenAllowance.toProtobuf();

  // Then
  ASSERT_TRUE(protoTokenAllowance->has_tokenid());
  EXPECT_EQ(TokenId::fromProtobuf(protoTokenAllowance->tokenid()), getTestTokenId());
  ASSERT_TRUE(protoTokenAllowance->has_owner());
  EXPECT_EQ(AccountId::fromProtobuf(protoTokenAllowance->owner()), getTestOwnerAccountId());
  ASSERT_TRUE(protoTokenAllowance->has_spender());
  EXPECT_EQ(AccountId::fromProtobuf(protoTokenAllowance->spender()), getTestSpenderAccountId());
  EXPECT_EQ(protoTokenAllowance->amount(), getTestAmount());
}
