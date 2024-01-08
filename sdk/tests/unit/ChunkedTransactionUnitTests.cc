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
#include "ED25519PrivateKey.h"
#include "FileAppendTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

using namespace Hedera;

class ChunkedTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline unsigned int getTestMaxChunks() const { return mTestMaxChunks; }
  [[nodiscard]] inline unsigned int getTestChunkSize() const { return mTestChunkSize; }

private:
  const unsigned int mTestMaxChunks = 1U;
  const unsigned int mTestChunkSize = 2U;
};

//-----
TEST_F(ChunkedTransactionUnitTests, GetSetMaxChunks)
{
  // Given
  FileAppendTransaction transaction;

  // When
  transaction.setMaxChunks(getTestMaxChunks());

  // Then
  EXPECT_EQ(transaction.getMaxChunks(), getTestMaxChunks());
}

//-----
TEST_F(ChunkedTransactionUnitTests, GetSetMaxChunksFrozen)
{
  // Given
  FileAppendTransaction transaction = FileAppendTransaction()
                                        .setNodeAccountIds({ AccountId(1ULL) })
                                        .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMaxChunks(getTestMaxChunks()), IllegalStateException);
}

//-----
TEST_F(ChunkedTransactionUnitTests, GetSetChunkSize)
{
  // Given
  FileAppendTransaction transaction;

  // When
  transaction.setChunkSize(getTestChunkSize());

  // Then
  EXPECT_EQ(transaction.getChunkSize(), getTestChunkSize());
}

//-----
TEST_F(ChunkedTransactionUnitTests, GetSetChunkSizeFrozen)
{
  // Given
  FileAppendTransaction transaction = FileAppendTransaction()
                                        .setNodeAccountIds({ AccountId(1ULL) })
                                        .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setChunkSize(getTestChunkSize()), IllegalStateException);
}
