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
#include "PrngTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class PrngTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline int getTestRange() const { return mTestRange; }

private:
  const int mTestRange = 1;
};

//-----
TEST_F(PrngTransactionUnitTests, ConstructPrngTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::UtilPrngTransactionBody>();
  body->set_range(getTestRange());

  proto::TransactionBody txBody;
  txBody.set_allocated_util_prng(body.release());

  // When
  const PrngTransaction prngTransaction(txBody);

  // Then
  EXPECT_EQ(prngTransaction.getRange(), getTestRange());
}

//-----
TEST_F(PrngTransactionUnitTests, GetSetRange)
{
  // Given
  PrngTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setRange(getTestRange()));

  // Then
  EXPECT_EQ(transaction.getRange(), getTestRange());
}

//-----
TEST_F(PrngTransactionUnitTests, GetSetRangeFrozen)
{
  // Given
  PrngTransaction transaction =
    PrngTransaction().setNodeAccountIds({ AccountId(1ULL) }).setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setRange(getTestRange()), IllegalStateException);
}
