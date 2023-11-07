/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "CustomFee.h"
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "CustomRoyaltyFee.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TokenFeeScheduleUpdateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }
  [[nodiscard]] inline const std::vector<std::shared_ptr<CustomFee>>& getTestCustomFees() const
  {
    return mTestCustomFees;
  }

private:
  const TokenId mTestTokenId = TokenId(1ULL);
  const std::vector<std::shared_ptr<CustomFee>> mTestCustomFees = { std::make_shared<CustomFixedFee>(),
                                                                    std::make_shared<CustomFractionalFee>(),
                                                                    std::make_shared<CustomRoyaltyFee>() };
};

//-----
TEST_F(TokenFeeScheduleUpdateTransactionTest, ConstructTokenFeeScheduleUpdateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenFeeScheduleUpdateTransactionBody>();
  body->set_allocated_token_id(getTestTokenId().toProtobuf().release());

  for (const auto& fee : getTestCustomFees())
  {
    *body->add_custom_fees() = *fee->toProtobuf();
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_token_fee_schedule_update(body.release());

  // When
  const TokenFeeScheduleUpdateTransaction tokenFeeScheduleUpdateTransaction(txBody);

  // Then
  EXPECT_EQ(tokenFeeScheduleUpdateTransaction.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenFeeScheduleUpdateTransaction.getCustomFees().size(), getTestCustomFees().size());
}

//-----
TEST_F(TokenFeeScheduleUpdateTransactionTest, GetSetTokenId)
{
  // Given
  TokenFeeScheduleUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenId(getTestTokenId()));

  // Then
  EXPECT_EQ(transaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenFeeScheduleUpdateTransactionTest, GetSetTokenIdFrozen)
{
  // Given
  TokenFeeScheduleUpdateTransaction transaction = TokenFeeScheduleUpdateTransaction()
                                                    .setNodeAccountIds({ AccountId(1ULL) })
                                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTokenId(getTestTokenId()), IllegalStateException);
}

//-----
TEST_F(TokenFeeScheduleUpdateTransactionTest, GetSetCustomFees)
{
  // Given
  TokenFeeScheduleUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setCustomFees(getTestCustomFees()));

  // Then
  EXPECT_EQ(transaction.getCustomFees(), getTestCustomFees());
}

//-----
TEST_F(TokenFeeScheduleUpdateTransactionTest, GetSetCustomFeesFrozen)
{
  // Given
  TokenFeeScheduleUpdateTransaction transaction = TokenFeeScheduleUpdateTransaction()
                                                    .setNodeAccountIds({ AccountId(1ULL) })
                                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setCustomFees(getTestCustomFees()), IllegalStateException);
}
