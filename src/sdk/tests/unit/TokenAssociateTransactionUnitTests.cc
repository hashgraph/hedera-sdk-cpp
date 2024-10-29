/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "TokenAssociateTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <transaction_body.pb.h>

using namespace Hedera;

class TokenAssociateTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::vector<TokenId>& getTestTokenIds() const { return mTestTokenIds; }

private:
  const AccountId mTestAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const std::vector<TokenId> mTestTokenIds = { TokenId(4ULL, 5ULL, 6ULL),
                                               TokenId(7ULL, 8ULL, 9ULL),
                                               TokenId(10ULL, 11ULL, 12ULL) };
};

//-----
TEST_F(TokenAssociateTransactionUnitTests, ConstructTokenAssociateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenAssociateTransactionBody>();
  body->set_allocated_account(getTestAccountId().toProtobuf().release());

  for (const auto& token : getTestTokenIds())
  {
    *body->add_tokens() = *token.toProtobuf();
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(body.release());

  // When
  const TokenAssociateTransaction tokenAssociateTransaction(txBody);

  // Then
  EXPECT_EQ(tokenAssociateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(tokenAssociateTransaction.getTokenIds().size(), getTestTokenIds().size());
}

//-----
TEST_F(TokenAssociateTransactionUnitTests, GetSetAccountId)
{
  // Given
  TokenAssociateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAccountId(getTestAccountId()));

  // Then
  EXPECT_EQ(transaction.getAccountId(), getTestAccountId());
}

//-----
TEST_F(TokenAssociateTransactionUnitTests, GetSetAccountIdFrozen)
{
  // Given
  TokenAssociateTransaction transaction = TokenAssociateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(TokenAssociateTransactionUnitTests, GetSetTokenIds)
{
  // Given
  TokenAssociateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenIds(getTestTokenIds()));

  // Then
  EXPECT_EQ(transaction.getTokenIds(), getTestTokenIds());
}

//-----
TEST_F(TokenAssociateTransactionUnitTests, GetSetTokenIdFrozen)
{
  // Given
  TokenAssociateTransaction transaction = TokenAssociateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTokenIds(getTestTokenIds()), IllegalStateException);
}
