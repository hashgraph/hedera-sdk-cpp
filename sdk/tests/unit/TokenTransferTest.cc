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
#include "TokenTransfer.h"

#include "AccountId.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TokenTransferTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const int64_t& getTestAmount() const { return mAmount; }
  [[nodiscard]] inline const uint32_t& getTestExpectedDecimals() const { return mExpectedDecimals; }
  [[nodiscard]] inline bool getTestIsApproval() const { return mIsApproval; }

private:
  const TokenId mTokenId = TokenId(10ULL);
  const AccountId mAccountId = AccountId(200ULL);
  const int64_t mAmount = 3000LL;
  const uint32_t mExpectedDecimals = 40000U;
  const bool mIsApproval = true;
};

//-----
TEST_F(TokenTransferTest, DefaultConstruction)
{
  // Given / When
  const TokenTransfer tokenTransfer;

  // Then
  EXPECT_EQ(tokenTransfer.mTokenId, TokenId());
  EXPECT_EQ(tokenTransfer.mAccountId, AccountId());
  EXPECT_EQ(tokenTransfer.mAmount, 0ULL);
  EXPECT_EQ(tokenTransfer.mExpectedDecimals, 0);
  EXPECT_FALSE(tokenTransfer.mIsApproval);
}

//-----
TEST_F(TokenTransferTest, ConstructWithTokenIdAccountIdAmountAndApproval)
{
  // Given / When
  const TokenTransfer tokenTransfer(getTestTokenId(), getTestAccountId(), getTestAmount(), getTestIsApproval());

  // Then
  EXPECT_EQ(tokenTransfer.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenTransfer.mAccountId, getTestAccountId());
  EXPECT_EQ(tokenTransfer.mAmount, getTestAmount());
  EXPECT_EQ(tokenTransfer.mExpectedDecimals, 0U);
  EXPECT_EQ(tokenTransfer.mIsApproval, getTestIsApproval());
}

//-----
TEST_F(TokenTransferTest, ConstructWithTokenIdAccountIdAmountExpectedDecimalsAndApproval)
{
  // Given / When
  const TokenTransfer tokenTransfer(
    getTestTokenId(), getTestAccountId(), getTestAmount(), getTestExpectedDecimals(), getTestIsApproval());

  // Then
  EXPECT_EQ(tokenTransfer.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenTransfer.mAccountId, getTestAccountId());
  EXPECT_EQ(tokenTransfer.mAmount, getTestAmount());
  EXPECT_EQ(tokenTransfer.mExpectedDecimals, getTestExpectedDecimals());
  EXPECT_EQ(tokenTransfer.mIsApproval, getTestIsApproval());
}

//-----
TEST_F(TokenTransferTest, FromProtobuf)
{
  // Given
  proto::TokenTransferList tokenTransferList;
  tokenTransferList.set_allocated_token(getTestTokenId().toProtobuf().release());

  proto::AccountAmount* amount = tokenTransferList.add_transfers();
  amount->set_allocated_accountid(getTestAccountId().toProtobuf().release());
  amount->set_amount(getTestAmount());
  amount->set_is_approval(getTestIsApproval());

  auto decimalPtr = std::make_unique<google::protobuf::UInt32Value>();
  decimalPtr->set_value(getTestExpectedDecimals());
  tokenTransferList.set_allocated_expected_decimals(decimalPtr.release());

  // When
  const std::vector<TokenTransfer> tokenTransfers = TokenTransfer::fromProtobuf(tokenTransferList);

  // Then
  ASSERT_EQ(tokenTransfers.size(), 1);
  EXPECT_EQ(tokenTransfers.at(0).mTokenId, getTestTokenId());
  EXPECT_EQ(tokenTransfers.at(0).mAccountId, getTestAccountId());
  EXPECT_EQ(tokenTransfers.at(0).mAmount, getTestAmount());
  EXPECT_EQ(tokenTransfers.at(0).mExpectedDecimals, getTestExpectedDecimals());
  EXPECT_EQ(tokenTransfers.at(0).mIsApproval, getTestIsApproval());
}

//-----
TEST_F(TokenTransferTest, ToProtobuf)
{
  // Given
  const TokenTransfer tokenTransfer(
    getTestTokenId(), getTestAccountId(), getTestAmount(), getTestExpectedDecimals(), getTestIsApproval());

  // When
  const std::unique_ptr<proto::AccountAmount> proto = tokenTransfer.toProtobuf();

  // Then
  EXPECT_EQ(proto->accountid().shardnum(), getTestAccountId().getShardNum());
  EXPECT_EQ(proto->accountid().realmnum(), getTestAccountId().getRealmNum());
  EXPECT_EQ(proto->accountid().accountnum(), getTestAccountId().getAccountNum());
  EXPECT_EQ(proto->amount(), getTestAmount());
  EXPECT_EQ(proto->is_approval(), getTestIsApproval());
}
