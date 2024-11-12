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
#include "TokenBurnTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <transaction_body.pb.h>

using namespace Hiero;

class TokenBurnTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }
  [[nodiscard]] inline const uint64_t& getTestAmount() const { return mTestAmount; }
  [[nodiscard]] inline const std::vector<uint64_t>& getTestSerialNumbers() const { return mTestSerialNumbers; }

private:
  const TokenId mTestTokenId = TokenId(1ULL, 2ULL, 3ULL);
  const uint64_t mTestAmount = 4ULL;
  const std::vector<uint64_t> mTestSerialNumbers = { 5ULL, 6ULL, 7ULL };
};

//-----
TEST_F(TokenBurnTransactionUnitTests, ConstructTokenBurnTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenBurnTransactionBody>();
  body->set_allocated_token(getTestTokenId().toProtobuf().release());
  body->set_amount(getTestAmount());

  for (const auto& num : getTestSerialNumbers())
  {
    body->add_serialnumbers(static_cast<int64_t>(num));
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_tokenburn(body.release());

  // When
  const TokenBurnTransaction tokenBurnTransaction(txBody);

  // Then
  EXPECT_EQ(tokenBurnTransaction.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenBurnTransaction.getAmount(), getTestAmount());
  EXPECT_EQ(tokenBurnTransaction.getSerialNumbers(), getTestSerialNumbers());
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetTokenId)
{
  // Given
  TokenBurnTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenId(getTestTokenId()));

  // Then
  EXPECT_EQ(transaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetTokenIdFrozen)
{
  // Given
  TokenBurnTransaction transaction = TokenBurnTransaction()
                                       .setNodeAccountIds({ AccountId(1ULL) })
                                       .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTokenId(getTestTokenId()), IllegalStateException);
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetAmount)
{
  // Given
  TokenBurnTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAmount(getTestAmount()));

  // Then
  EXPECT_EQ(transaction.getAmount(), getTestAmount());
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetAmountFrozen)
{
  // Given
  TokenBurnTransaction transaction = TokenBurnTransaction()
                                       .setNodeAccountIds({ AccountId(1ULL) })
                                       .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAmount(getTestAmount()), IllegalStateException);
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetSerialNumbers)
{
  // Given
  TokenBurnTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setSerialNumbers(getTestSerialNumbers()));

  // Then
  EXPECT_EQ(transaction.getSerialNumbers(), getTestSerialNumbers());
}

//-----
TEST_F(TokenBurnTransactionUnitTests, GetSetSerialNumbersFrozen)
{
  // Given
  TokenBurnTransaction transaction = TokenBurnTransaction()
                                       .setNodeAccountIds({ AccountId(1ULL) })
                                       .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setSerialNumbers(getTestSerialNumbers()), IllegalStateException);
}
