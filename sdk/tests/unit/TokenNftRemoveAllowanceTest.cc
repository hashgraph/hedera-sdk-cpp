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
#include "TokenNftRemoveAllowance.h"
#include "AccountId.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <proto/crypto_delete_allowance.pb.h>

using namespace Hedera;

class TokenNftRemoveAllowanceTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const std::vector<uint64_t>& getTestSerialNumbers() const { return mSerialNumbers; }

private:
  const TokenId mTokenId = TokenId(1ULL);
  const AccountId mOwnerAccountId = AccountId(2ULL);
  const std::vector<uint64_t> mSerialNumbers = { 3ULL, 4ULL };
};

//-----
TEST_F(TokenNftRemoveAllowanceTest, ConstructWithTokenIdOwnerSerialNumbers)
{
  // Given / When
  const TokenNftRemoveAllowance tokenNftRemoveAllowance(
    getTestTokenId(), getTestOwnerAccountId(), getTestSerialNumbers());

  // Then
  EXPECT_EQ(tokenNftRemoveAllowance.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenNftRemoveAllowance.getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(tokenNftRemoveAllowance.getSerialNumbers(), getTestSerialNumbers());
}

//-----
TEST_F(TokenNftRemoveAllowanceTest, GetSetTokenId)
{
  // Given
  TokenNftRemoveAllowance tokenNftRemoveAllowance;

  // When
  tokenNftRemoveAllowance.setTokenId(getTestTokenId());

  // Then
  EXPECT_EQ(tokenNftRemoveAllowance.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenNftRemoveAllowanceTest, GetSetOwnerAccountId)
{
  // Given
  TokenNftRemoveAllowance tokenNftRemoveAllowance;

  // When
  tokenNftRemoveAllowance.setOwnerAccountId(getTestOwnerAccountId());

  // Then
  EXPECT_EQ(tokenNftRemoveAllowance.getOwnerAccountId(), getTestOwnerAccountId());
}

//-----
TEST_F(TokenNftRemoveAllowanceTest, GetSetSerialNumbers)
{
  // Given
  TokenNftRemoveAllowance tokenNftRemoveAllowance;

  // When
  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftRemoveAllowance.addSerialNumber(num);
  }

  // Then
  ASSERT_EQ(tokenNftRemoveAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftRemoveAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }
}

//-----
TEST_F(TokenNftRemoveAllowanceTest, FromProtobuf)
{
  // Given
  proto::NftRemoveAllowance protoNftRemoveAllowance;
  protoNftRemoveAllowance.set_allocated_token_id(getTestTokenId().toProtobuf().release());
  protoNftRemoveAllowance.set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    protoNftRemoveAllowance.add_serial_numbers(static_cast<int64_t>(num));
  }

  // When
  const TokenNftRemoveAllowance tokenNftRemoveAllowance =
    TokenNftRemoveAllowance::fromProtobuf(protoNftRemoveAllowance);

  // Then
  EXPECT_EQ(tokenNftRemoveAllowance.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenNftRemoveAllowance.getOwnerAccountId(), getTestOwnerAccountId());

  ASSERT_EQ(tokenNftRemoveAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftRemoveAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }
}

//-----
TEST_F(TokenNftRemoveAllowanceTest, ToProtobuf)
{
  // Given
  TokenNftRemoveAllowance tokenNftRemoveAllowance;
  tokenNftRemoveAllowance.setTokenId(getTestTokenId());
  tokenNftRemoveAllowance.setOwnerAccountId(getTestOwnerAccountId());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    tokenNftRemoveAllowance.addSerialNumber(num);
  }

  // When
  const std::unique_ptr<proto::NftRemoveAllowance> protoNftRemoveAllowance = tokenNftRemoveAllowance.toProtobuf();

  // Then
  ASSERT_TRUE(protoNftRemoveAllowance->has_token_id());
  EXPECT_EQ(TokenId::fromProtobuf(protoNftRemoveAllowance->token_id()), getTestTokenId());
  ASSERT_TRUE(protoNftRemoveAllowance->has_owner());
  EXPECT_EQ(AccountId::fromProtobuf(protoNftRemoveAllowance->owner()), getTestOwnerAccountId());

  ASSERT_EQ(tokenNftRemoveAllowance.getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(tokenNftRemoveAllowance.getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }
}
