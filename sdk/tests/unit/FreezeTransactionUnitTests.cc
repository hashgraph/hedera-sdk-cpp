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
#include "ED25519PrivateKey.h"
#include "FreezeTransaction.h"
#include "KeyList.h"
#include "exceptions/IllegalStateException.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <proto/file_update.pb.h>
#include <proto/transaction_body.pb.h>
#include <string>
#include <vector>

using namespace Hedera;

class FreezeTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestFileHash() const { return mTestFileHash; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestStartTime() const { return mTestStartTime; }
  [[nodiscard]] inline const FreezeType& getTestFreezeType() const { return mTestFreezeType; }

private:
  const FileId mTestFileId = FileId(1ULL, 2ULL, 3ULL);
  const std::vector<std::byte> mTestFileHash = { std::byte(0x04), std::byte(0x05), std::byte(0x06) };
  const std::chrono::system_clock::time_point mTestStartTime = std::chrono::system_clock::now();
  const FreezeType mTestFreezeType = FreezeType::TELEMETRY_UPGRADE;
};

//-----
TEST_F(FreezeTransactionTest, ConstructFreezeTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FreezeTransactionBody>();
  body->set_allocated_update_file(getTestFileId().toProtobuf().release());
  body->set_file_hash(internal::Utilities::byteVectorToString(getTestFileHash()));
  body->set_allocated_start_time(internal::TimestampConverter::toProtobuf(getTestStartTime()));
  body->set_freeze_type(gFreezeTypeToProtobufFreezeType.at(getTestFreezeType()));

  proto::TransactionBody txBody;
  txBody.set_allocated_freeze(body.release());

  // When
  const FreezeTransaction freezeTransaction(txBody);

  // Then
  ASSERT_TRUE(freezeTransaction.getFileId().has_value());
  EXPECT_EQ(freezeTransaction.getFileId().value(), getTestFileId());
  EXPECT_EQ(freezeTransaction.getFileHash(), getTestFileHash());
  ASSERT_TRUE(freezeTransaction.getStartTime().has_value());
  EXPECT_EQ(freezeTransaction.getStartTime().value(), getTestStartTime());
  EXPECT_EQ(freezeTransaction.getFreezeType(), getTestFreezeType());
}

//-----
TEST_F(FreezeTransactionTest, GetSetFileId)
{
  // Given
  FreezeTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  ASSERT_TRUE(transaction.getFileId().has_value());
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(FreezeTransactionTest, GetSetFileIdFrozen)
{
  // Given
  FreezeTransaction transaction = FreezeTransaction()
                                    .setNodeAccountIds({ AccountId(1ULL) })
                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(FreezeTransactionTest, GetSetFileHash)
{
  // Given
  FreezeTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setFileHash(getTestFileHash()));

  // Then
  EXPECT_EQ(transaction.getFileHash(), getTestFileHash());
}

//-----
TEST_F(FreezeTransactionTest, GetSetFileHashFrozen)
{
  // Given
  FreezeTransaction transaction = FreezeTransaction()
                                    .setNodeAccountIds({ AccountId(1ULL) })
                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setFileHash(getTestFileHash()), IllegalStateException);
}

//-----
TEST_F(FreezeTransactionTest, GetSetStartTime)
{
  // Given
  FreezeTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setStartTime(getTestStartTime()));

  // Then
  ASSERT_TRUE(transaction.getStartTime().has_value());
  EXPECT_EQ(transaction.getStartTime(), getTestStartTime());
}

//-----
TEST_F(FreezeTransactionTest, GetSetStartTimeFrozen)
{
  // Given
  FreezeTransaction transaction = FreezeTransaction()
                                    .setNodeAccountIds({ AccountId(1ULL) })
                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setStartTime(getTestStartTime()), IllegalStateException);
}

//-----
TEST_F(FreezeTransactionTest, GetSetFreezeType)
{
  // Given
  FreezeTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setFreezeType(getTestFreezeType()));

  // Then
  EXPECT_EQ(transaction.getFreezeType(), getTestFreezeType());
}

//-----
TEST_F(FreezeTransactionTest, GetSetFreezeTypeFrozen)
{
  // Given
  FreezeTransaction transaction = FreezeTransaction()
                                    .setNodeAccountIds({ AccountId(1ULL) })
                                    .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setFreezeType(getTestFreezeType()), IllegalStateException);
}
