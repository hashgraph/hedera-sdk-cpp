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
#include "TransactionResponse.h"

#include <gtest/gtest.h>
#include <proto/transaction_response.pb.h>

using namespace Hedera;

class TransactionResponseUnitTests : public ::testing::Test
{
};

//-----
TEST_F(TransactionResponseUnitTests, ContructTransactionResponse)
{
  // Given
  const AccountId accountId(1ULL);
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const TransactionId transactionId = TransactionId::withValidStart(accountId, now);
  const std::vector<std::byte> hash = { std::byte(0x00), std::byte(0x01), std::byte(0x02) };

  // When
  const TransactionResponse transactionResponse(accountId, transactionId, hash);

  // Then
  EXPECT_EQ(transactionResponse.mNodeId, accountId);
  EXPECT_EQ(transactionResponse.mTransactionHash, hash);
  EXPECT_EQ(transactionResponse.mTransactionId, transactionId);
}
