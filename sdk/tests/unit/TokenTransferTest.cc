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
  const uint32_t mExpectedDecimals = 40000;
  const bool mIsApproval = true;
};

//-----
TEST_F(TokenTransferTest, DefaultConstruction)
{
  TokenTransfer tokenTransfer;
  EXPECT_EQ(tokenTransfer.getTokenId(), TokenId());
  EXPECT_EQ(tokenTransfer.getAccountId(), AccountId());
  EXPECT_EQ(tokenTransfer.getAmount(), 0ULL);
  EXPECT_EQ(tokenTransfer.getExpectedDecimals(), 0);
  EXPECT_FALSE(tokenTransfer.getApproval());
}

//-----
TEST_F(TokenTransferTest, SetGetTokenId)
{
  TokenTransfer tokenTransfer;
  tokenTransfer.setTokenId(getTestTokenId());
  EXPECT_EQ(tokenTransfer.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenTransferTest, SetGetAccountId)
{
  TokenTransfer tokenTransfer;
  tokenTransfer.setAccountId(getTestAccountId());
  EXPECT_EQ(tokenTransfer.getAccountId(), getTestAccountId());
}

//-----
TEST_F(TokenTransferTest, SetGetAmount)
{
  TokenTransfer tokenTransfer;
  tokenTransfer.setAmount(getTestAmount());
  EXPECT_EQ(tokenTransfer.getAmount(), getTestAmount());
}

//-----
TEST_F(TokenTransferTest, SetGetExpectedDecimals)
{
  TokenTransfer tokenTransfer;
  tokenTransfer.setExpectedDecimals(getTestExpectedDecimals());
  EXPECT_EQ(tokenTransfer.getExpectedDecimals(), getTestExpectedDecimals());
}

//-----
TEST_F(TokenTransferTest, SetGetApproval)
{
  TokenTransfer tokenTransfer;
  tokenTransfer.setApproval(getTestIsApproval());
  EXPECT_TRUE(tokenTransfer.getApproval());
}
