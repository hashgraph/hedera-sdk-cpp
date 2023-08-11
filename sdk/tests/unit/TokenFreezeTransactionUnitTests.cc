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
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TokenFreezeTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TokenFreezeTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }

private:
  Client mClient;
  const AccountId mTestAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const TokenId mTestTokenId = TokenId(4ULL, 5ULL, 6ULL);
};

//-----
TEST_F(TokenFreezeTransactionTest, ConstructTokenFreezeTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenFreezeAccountTransactionBody>();
  body->set_allocated_account(getTestAccountId().toProtobuf().release());
  body->set_allocated_token(getTestTokenId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_tokenfreeze(body.release());

  // When
  const TokenFreezeTransaction tokenFreezeTransaction(txBody);

  // Then
  EXPECT_EQ(tokenFreezeTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(tokenFreezeTransaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenFreezeTransactionTest, GetSetAccountId)
{
  // Given
  TokenFreezeTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAccountId(getTestAccountId()));

  // Then
  EXPECT_EQ(transaction.getAccountId(), getTestAccountId());
}

//-----
TEST_F(TokenFreezeTransactionTest, GetSetAccountIdFrozen)
{
  // Given
  TokenFreezeTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(TokenFreezeTransactionTest, GetSetTokenId)
{
  // Given
  TokenFreezeTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenId(getTestTokenId()));

  // Then
  EXPECT_EQ(transaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenFreezeTransactionTest, GetSetTokenIdFrozen)
{
  // Given
  TokenFreezeTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setTokenId(getTestTokenId()), IllegalStateException);
}
