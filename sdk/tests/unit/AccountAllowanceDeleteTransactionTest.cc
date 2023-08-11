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
#include "AccountAllowanceDeleteTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "NftId.h"
#include "TokenId.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>
#include <vector>

using namespace Hedera;

class AccountAllowanceDeleteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(1ULL), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const std::vector<uint64_t>& getTestSerialNumbers() const { return mSerialNumbers; }

private:
  Client mClient;
  const AccountId mOwnerAccountId = AccountId(2ULL);
  const TokenId mTokenId = TokenId(3ULL);
  const std::vector<uint64_t> mSerialNumbers = { 4ULL, 5ULL, 6ULL };
};

//-----
TEST_F(AccountAllowanceDeleteTransactionTest, ConstructAccountAllowanceDeleteTransaction)
{
  // Given / When
  AccountAllowanceDeleteTransaction transaction;

  // Then
  EXPECT_TRUE(transaction.getTokenNftAllowanceDeletions().empty());
}

//-----
TEST_F(AccountAllowanceDeleteTransactionTest, ConstructAccountAllowanceDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoDeleteAllowanceTransactionBody>();
  proto::NftRemoveAllowance* nftRemoveAllowance = body->add_nftallowances();
  nftRemoveAllowance->set_allocated_token_id(getTestTokenId().toProtobuf().release());
  nftRemoveAllowance->set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    nftRemoveAllowance->add_serial_numbers(static_cast<int64_t>(num));
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(body.release());

  // When
  const AccountAllowanceDeleteTransaction accountAllowanceDeleteTransaction(txBody);

  // Then
  ASSERT_EQ(accountAllowanceDeleteTransaction.getTokenNftAllowanceDeletions().size(), 1);
  EXPECT_EQ(accountAllowanceDeleteTransaction.getTokenNftAllowanceDeletions().at(0).mTokenId, getTestTokenId());
  EXPECT_EQ(accountAllowanceDeleteTransaction.getTokenNftAllowanceDeletions().at(0).mOwnerAccountId,
            getTestOwnerAccountId());
  EXPECT_EQ(accountAllowanceDeleteTransaction.getTokenNftAllowanceDeletions().at(0).mSerialNumbers,
            getTestSerialNumbers());
}

//-----
TEST_F(AccountAllowanceDeleteTransactionTest, DeleteNftAllowances)
{
  // Given
  AccountAllowanceDeleteTransaction transaction;

  // When
  for (const uint64_t& num : getTestSerialNumbers())
  {
    transaction.deleteAllTokenNftAllowances(NftId(getTestTokenId(), num), getTestOwnerAccountId());
  }

  // Then
  ASSERT_EQ(transaction.getTokenNftAllowanceDeletions().size(), 1);
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mTokenId, getTestTokenId());
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mOwnerAccountId, getTestOwnerAccountId());
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mSerialNumbers, getTestSerialNumbers());
}

//-----
TEST_F(AccountAllowanceDeleteTransactionTest, DeleteNftAllowancesDifferentTokenId)
{
  // Given
  AccountAllowanceDeleteTransaction transaction;
  const std::vector<TokenId> tokenIds = { TokenId(10ULL), TokenId(20ULL), TokenId(30ULL) };
  ASSERT_EQ(tokenIds.size(), getTestSerialNumbers().size());

  // When
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    transaction.deleteAllTokenNftAllowances(NftId(tokenIds.at(i), getTestSerialNumbers().at(i)),
                                            getTestOwnerAccountId());
  }

  // Then
  ASSERT_EQ(transaction.getTokenNftAllowanceDeletions().size(), 3);

  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mTokenId, tokenIds.at(0));
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mOwnerAccountId, getTestOwnerAccountId());
  ASSERT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mSerialNumbers.size(), 1);
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(0).mSerialNumbers.at(0), getTestSerialNumbers().at(0));

  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(1).mTokenId, tokenIds.at(1));
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(1).mOwnerAccountId, getTestOwnerAccountId());
  ASSERT_EQ(transaction.getTokenNftAllowanceDeletions().at(1).mSerialNumbers.size(), 1);
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(1).mSerialNumbers.at(0), getTestSerialNumbers().at(1));

  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(2).mTokenId, tokenIds.at(2));
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(2).mOwnerAccountId, getTestOwnerAccountId());
  ASSERT_EQ(transaction.getTokenNftAllowanceDeletions().at(2).mSerialNumbers.size(), 1);
  EXPECT_EQ(transaction.getTokenNftAllowanceDeletions().at(2).mSerialNumbers.at(0), getTestSerialNumbers().at(2));
}

//-----
TEST_F(AccountAllowanceDeleteTransactionTest, DeleteNftAllowancesFrozen)
{
  // Given
  AccountAllowanceDeleteTransaction transaction;
  transaction.freezeWith(&getTestClient());

  // When / Then
  EXPECT_THROW(transaction.deleteAllTokenNftAllowances(NftId(getTestTokenId(), 10ULL), getTestOwnerAccountId()),
               IllegalStateException);
}
