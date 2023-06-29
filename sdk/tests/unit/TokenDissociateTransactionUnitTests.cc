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
#include "TokenDissociateTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TokenDissociateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::vector<TokenId>& getTestTokenIds() const { return mTestTokenIds; }

private:
  Client mClient;
  const AccountId mTestAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const std::vector<TokenId> mTestTokenIds = { TokenId(4ULL, 5ULL, 6ULL),
                                               TokenId(7ULL, 8ULL, 9ULL),
                                               TokenId(10ULL, 11ULL, 12ULL) };
};

//-----
TEST_F(TokenDissociateTransactionTest, ConstructTokenDissociateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenDissociateTransactionBody>();
  body->set_allocated_account(getTestAccountId().toProtobuf().release());

  for (const auto& token : getTestTokenIds())
  {
    *body->add_tokens() = *token.toProtobuf();
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_tokendissociate(body.release());

  // When
  const TokenDissociateTransaction tokenDissociateTransaction(txBody);

  // Then
  EXPECT_EQ(tokenDissociateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(tokenDissociateTransaction.getTokenIds().size(), getTestTokenIds().size());
}

//-----
TEST_F(TokenDissociateTransactionTest, GetSetAccountId)
{
  // Given
  TokenDissociateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAccountId(getTestAccountId()));

  // Then
  EXPECT_EQ(transaction.getAccountId(), getTestAccountId());
}

//-----
TEST_F(TokenDissociateTransactionTest, GetSetAccountIdFrozen)
{
  // Given
  TokenDissociateTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(TokenDissociateTransactionTest, GetSetTokenIds)
{
  // Given
  TokenDissociateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenIds(getTestTokenIds()));

  // Then
  EXPECT_EQ(transaction.getTokenIds(), getTestTokenIds());
}

//-----
TEST_F(TokenDissociateTransactionTest, GetSetTokenIdFrozen)
{
  // Given
  TokenDissociateTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setTokenIds(getTestTokenIds()), IllegalStateException);
}
