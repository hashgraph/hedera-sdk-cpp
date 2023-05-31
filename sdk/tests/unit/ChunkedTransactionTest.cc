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
#include "ED25519PrivateKey.h"
#include "FileAppendTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

using namespace Hedera;

class ChunkedTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestData() const { return mTestData; }
  [[nodiscard]] inline unsigned int getTestMaxChunks() const { return mTestMaxChunks; }
  [[nodiscard]] inline unsigned int getTestChunkSize() const { return mTestChunkSize; }

private:
  Client mClient;
  const std::vector<std::byte> mTestData = { std::byte(0x01), std::byte(0x02), std::byte(0x03) };
  const unsigned int mTestMaxChunks = 4U;
  const unsigned int mTestChunkSize = 5U;
};

//-----
TEST_F(ChunkedTransactionTest, GetSetData)
{
  // Given
  FileAppendTransaction transactionBytes;
  FileAppendTransaction transactionStr;

  // When
  transactionBytes.setData(getTestData());
  transactionStr.setData(internal::Utilities::byteVectorToString(getTestData()));

  // Then
  EXPECT_EQ(transactionBytes.getData(), getTestData());
  EXPECT_EQ(transactionStr.getData(), getTestData());
}

//-----
TEST_F(ChunkedTransactionTest, GetSetDataFrozen)
{
  // Given
  FileAppendTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setData(getTestData()), IllegalStateException);
  EXPECT_THROW(transaction.setData(internal::Utilities::byteVectorToString(getTestData())), IllegalStateException);
}

//-----
TEST_F(ChunkedTransactionTest, GetSetMaxChunks)
{
  // Given
  FileAppendTransaction transaction;

  // When
  transaction.setMaxChunks(getTestMaxChunks());

  // Then
  EXPECT_EQ(transaction.getMaxChunks(), getTestMaxChunks());
}

//-----
TEST_F(ChunkedTransactionTest, GetSetMaxChunksFrozen)
{
  // Given
  FileAppendTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setMaxChunks(getTestMaxChunks()), IllegalStateException);
}

//-----
TEST_F(ChunkedTransactionTest, GetSetChunkSize)
{
  // Given
  FileAppendTransaction transaction;

  // When
  transaction.setChunkSize(getTestChunkSize());

  // Then
  EXPECT_EQ(transaction.getChunkSize(), getTestChunkSize());
}

//-----
TEST_F(ChunkedTransactionTest, GetSetChunkSizeFrozen)
{
  // Given
  FileAppendTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setChunkSize(getTestChunkSize()), IllegalStateException);
}
