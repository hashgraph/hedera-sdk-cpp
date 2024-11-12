/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TokenDeleteTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <transaction_body.pb.h>

using namespace Hiero;

class TokenDeleteTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }

private:
  const TokenId mTestTokenId = TokenId(1ULL);
};

//-----
TEST_F(TokenDeleteTransactionUnitTests, ConstructTokenDeleteTransaction)
{
  // Given / When
  TokenDeleteTransaction transaction;

  // Then
  EXPECT_FALSE(transaction.getTokenId().has_value());
}

//-----
TEST_F(TokenDeleteTransactionUnitTests, ConstructTokenDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenDeleteTransactionBody>();
  body->set_allocated_token(getTestTokenId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(body.release());

  // When
  const TokenDeleteTransaction tokenDeleteTransaction(txBody);

  // Then
  EXPECT_EQ(tokenDeleteTransaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenDeleteTransactionUnitTests, GetSetTokenId)
{
  // Given
  TokenDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenId(getTestTokenId()));

  // Then
  EXPECT_EQ(transaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenDeleteTransactionUnitTests, GetSetTokenIdFrozen)
{
  // Given
  TokenDeleteTransaction transaction = TokenDeleteTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTokenId(getTestTokenId()), IllegalStateException);
}