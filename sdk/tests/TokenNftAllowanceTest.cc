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
#include "TokenNftAllowance.h"
#include "AccountId.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <proto/crypto_approve_allowance.pb.h>

using namespace Hedera;

class TokenNftAllowanceTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const AccountId& getTestSpenderAccountId() const { return mSpenderAccountId; }
  [[nodiscard]] inline const std::vector<uint64_t>& getTestSerialNumbers() const { return mSerialNumbers; }
  [[nodiscard]] inline const AccountId& getTestDelegatingSpenderAccountId() const
  {
    return mDelegatingSpenderAccountId;
  }

private:
  const TokenId mTokenId = TokenId(1ULL);
  const AccountId mOwnerAccountId = AccountId(2ULL);
  const AccountId mSpenderAccountId = AccountId(3ULL);
  const std::vector<uint64_t> mSerialNumbers = { 5ULL, 6ULL };
  const AccountId mDelegatingSpenderAccountId = AccountId(7ULL);
};

//-----
TEST_F(TokenNftAllowanceTest, ConstructWithTokenIdOwnerSpenderSerialNumbersApproval)
{
  // Given / When
  const TokenNftAllowance tokenNftAllowance(getTestTokenId(),
                                            getTestOwnerAccountId(),
                                            getTestSpenderAccountId(),
                                            getTestSerialNumbers(),
                                            std::optional<bool>());

  // Then
  EXPECT_EQ(tokenNftAllowance.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenNftAllowance.getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(tokenNftAllowance.getSpenderAccountId(), getTestSpenderAccountId());
  EXPECT_EQ(tokenNftAllowance.getSerialNumbers(), getTestSerialNumbers());
  EXPECT_FALSE(tokenNftAllowance.getApprovedForAll().has_value());
}

//-----
TEST_F(TokenNftAllowanceTest, GetSetTokenId)
{
  // Given
  TokenNftAllowance tokenNftAllowance;

  // When
  tokenNftAllowance.setTokenId(getTestTokenId());

  // Then
  EXPECT_EQ(tokenNftAllowance.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenNftAllowanceTest, GetSetOwnerAccountId)
{
  // Given
  TokenNftAllowance tokenNftAllowance;

  // When
  tokenNftAllowance.setOwnerAccountId(getTestOwnerAccountId());

  // Then
  EXPECT_EQ(tokenNftAllowance.getOwnerAccountId(), getTestOwnerAccountId());
}

//-----
TEST_F(TokenNftAllowanceTest, GetSetSpenderAccountId)
{
  // Given
  TokenNftAllowance tokenNftAllowance;

  // When
  tokenNftAllowance.setSpenderAccountId(getTestSpenderAccountId());

  // Then
  EXPECT_EQ(tokenNftAllowance.getSpenderAccountId(), getTestSpenderAccountId());
}

//-----
TEST_F(TokenNftAllowanceTest, GetSetSerialNumbers)
{
  // Given
  TokenNftAllowance tokenNftAllowance;

  // When
  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftAllowance.addSerialNumber(num);
  }

  // Then
  ASSERT_EQ(tokenNftAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }
}

//-----
TEST_F(TokenNftAllowanceTest, GetSetApproveForAll)
{
  // Given
  TokenNftAllowance tokenNftAllowance;

  // When
  tokenNftAllowance.approveForAll(false);

  // Then
  ASSERT_TRUE(tokenNftAllowance.getApprovedForAll().has_value());
  EXPECT_FALSE(*tokenNftAllowance.getApprovedForAll());
}

//-----
TEST_F(TokenNftAllowanceTest, ClearSerialNumbersIfApprovingAll)
{
  // Given
  TokenNftAllowance tokenNftAllowance;
  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftAllowance.addSerialNumber(num);
  }

  // When
  tokenNftAllowance.approveForAll(true);

  // Then
  EXPECT_TRUE(tokenNftAllowance.getSerialNumbers().empty());
}

//-----
TEST_F(TokenNftAllowanceTest, DoNotClearSerialNumbersIfRevokingAll)
{
  // Given
  TokenNftAllowance tokenNftAllowance;
  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftAllowance.addSerialNumber(num);
  }

  // When
  tokenNftAllowance.approveForAll(false);

  // Then
  EXPECT_EQ(tokenNftAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
}

//-----
TEST_F(TokenNftAllowanceTest, FromProtobuf)
{
  // Given
  proto::NftAllowance protoNftAllowance;
  protoNftAllowance.set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  protoNftAllowance.set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  protoNftAllowance.set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());
  protoNftAllowance.set_allocated_delegating_spender(getTestDelegatingSpenderAccountId().toProtobuf().release());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    protoNftAllowance.add_serial_numbers(static_cast<int64_t>(num));
  }

  auto boolValue = std::make_unique<google::protobuf::BoolValue>();
  boolValue->set_value(false);
  protoNftAllowance.set_allocated_approved_for_all(boolValue.release());

  // When
  const TokenNftAllowance tokenNftAllowance = TokenNftAllowance::fromProtobuf(protoNftAllowance);

  // Then
  EXPECT_EQ(tokenNftAllowance.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenNftAllowance.getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(tokenNftAllowance.getSpenderAccountId(), getTestSpenderAccountId());
  ASSERT_TRUE(tokenNftAllowance.getApprovedForAll().has_value());
  EXPECT_FALSE(*tokenNftAllowance.getApprovedForAll());

  ASSERT_EQ(tokenNftAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }

  ASSERT_TRUE(tokenNftAllowance.getDelegateSpender().has_value());
  EXPECT_EQ(*tokenNftAllowance.getDelegateSpender(), getTestDelegatingSpenderAccountId());
}

//-----
TEST_F(TokenNftAllowanceTest, ToProtobuf)
{
  // Given
  TokenNftAllowance tokenNftAllowance;
  tokenNftAllowance.setTokenId(getTestTokenId());
  tokenNftAllowance.setOwnerAccountId(getTestOwnerAccountId());
  tokenNftAllowance.setSpenderAccountId(getTestSpenderAccountId());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftAllowance.addSerialNumber(num);
  }

  tokenNftAllowance.approveForAll(false);

  // When
  const std::unique_ptr<proto::NftAllowance> protoNftAllowance = tokenNftAllowance.toProtobuf();

  // Then
  ASSERT_TRUE(protoNftAllowance->has_tokenid());
  EXPECT_EQ(TokenId::fromProtobuf(protoNftAllowance->tokenid()), getTestTokenId());
  ASSERT_TRUE(protoNftAllowance->has_owner());
  EXPECT_EQ(AccountId::fromProtobuf(protoNftAllowance->owner()), getTestOwnerAccountId());
  ASSERT_TRUE(protoNftAllowance->has_spender());
  EXPECT_EQ(AccountId::fromProtobuf(protoNftAllowance->spender()), getTestSpenderAccountId());

  ASSERT_EQ(tokenNftAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }

  ASSERT_TRUE(tokenNftAllowance.getApprovedForAll().has_value());
  EXPECT_FALSE(*tokenNftAllowance.getApprovedForAll());
}
