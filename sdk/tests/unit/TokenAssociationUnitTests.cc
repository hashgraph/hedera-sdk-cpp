/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "TokenAssociation.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/custom_fees.pb.h>

using namespace Hedera;

class TokenAssociationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }

private:
  const AccountId mTestAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const TokenId mTestTokenId = TokenId(4ULL, 5ULL, 6ULL);
};

//-----
TEST_F(TokenAssociationTest, FromProtobuf)
{
  // Given
  proto::TokenAssociation protoTokenAssociation;
  protoTokenAssociation.set_allocated_account_id(getTestAccountId().toProtobuf().release());
  protoTokenAssociation.set_allocated_token_id(getTestTokenId().toProtobuf().release());

  // When
  const TokenAssociation tokenAssociation = TokenAssociation::fromProtobuf(protoTokenAssociation);

  // Then
  EXPECT_EQ(tokenAssociation.mAccountId, getTestAccountId());
  EXPECT_EQ(tokenAssociation.mTokenId, getTestTokenId());
}

//-----
TEST_F(TokenAssociationTest, ToProtobuf)
{
  // Given
  TokenAssociation tokenAssociation;
  tokenAssociation.mAccountId = getTestAccountId();
  tokenAssociation.mTokenId = getTestTokenId();

  // When
  const std::unique_ptr<proto::TokenAssociation> protoTokenAssociation = tokenAssociation.toProtobuf();

  // Then
  ASSERT_TRUE(protoTokenAssociation->has_account_id());
  EXPECT_EQ(protoTokenAssociation->account_id().shardnum(), getTestAccountId().getShardNum());
  EXPECT_EQ(protoTokenAssociation->account_id().realmnum(), getTestAccountId().getRealmNum());
  EXPECT_EQ(protoTokenAssociation->account_id().accountnum(), getTestAccountId().getAccountNum());
  ASSERT_TRUE(protoTokenAssociation->has_token_id());
  EXPECT_EQ(protoTokenAssociation->token_id().shardnum(), getTestTokenId().getShardNum());
  EXPECT_EQ(protoTokenAssociation->token_id().realmnum(), getTestTokenId().getRealmNum());
  EXPECT_EQ(protoTokenAssociation->token_id().tokennum(), getTestTokenId().getTokenNum());
}

//-----
TEST_F(TokenAssociationTest, FromBytes)
{
  // Given
  proto::TokenAssociation protoTokenAssociation;
  protoTokenAssociation.set_allocated_account_id(getTestAccountId().toProtobuf().release());
  protoTokenAssociation.set_allocated_token_id(getTestTokenId().toProtobuf().release());

  // When
  const TokenAssociation tokenAssociation =
    TokenAssociation::fromBytes(internal::Utilities::stringToByteVector(protoTokenAssociation.SerializeAsString()));

  // Then
  EXPECT_EQ(tokenAssociation.mAccountId, getTestAccountId());
  EXPECT_EQ(tokenAssociation.mTokenId, getTestTokenId());
}

//-----
TEST_F(TokenAssociationTest, ToBytes)
{
  // Given
  TokenAssociation tokenAssociation;
  tokenAssociation.mAccountId = getTestAccountId();
  tokenAssociation.mTokenId = getTestTokenId();

  // When
  const std::vector<std::byte> bytes = tokenAssociation.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(tokenAssociation.toProtobuf()->SerializeAsString()));
}
