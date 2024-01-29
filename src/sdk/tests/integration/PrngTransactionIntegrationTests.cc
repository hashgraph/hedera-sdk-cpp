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
#include "BaseIntegrationTest.h"
#include "PrngTransaction.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"

#include <gtest/gtest.h>

using namespace Hedera;

class PrngTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(PrngTransactionIntegrationTests, ExecutePrngTransactionNoRange)
{
  // Given / When
  TransactionResponse txResponse;
  ASSERT_NO_THROW(txResponse = PrngTransaction().execute(getTestClient()));

  // Then
  TransactionRecord txRecord;
  ASSERT_NO_THROW(txRecord = txResponse.getRecord(getTestClient()));

  EXPECT_FALSE(txRecord.mPrngBytes.empty());
  EXPECT_FALSE(txRecord.mPrngNumber.has_value());
}

//-----
TEST_F(PrngTransactionIntegrationTests, ExecutePrngTransactionRange)
{
  // Given
  const int range = 100;

  // When
  TransactionResponse txResponse;
  ASSERT_NO_THROW(txResponse = PrngTransaction().setRange(range).execute(getTestClient()));

  // Then
  TransactionRecord txRecord;
  ASSERT_NO_THROW(txRecord = txResponse.getRecord(getTestClient()));

  EXPECT_TRUE(txRecord.mPrngBytes.empty());
  EXPECT_TRUE(txRecord.mPrngNumber.has_value());
  EXPECT_GE(txRecord.mPrngNumber.value(), 0);
  EXPECT_LE(txRecord.mPrngNumber.value(), range);
}
