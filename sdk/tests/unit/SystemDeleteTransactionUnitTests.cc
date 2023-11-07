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
#include "ECDSAsecp256k1PrivateKey.h"
#include "SystemDeleteTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class SystemDeleteTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }

private:
  const FileId mTestFileId = FileId(1ULL, 2ULL, 3ULL);
  const ContractId mTestContractId = ContractId(4ULL, 5ULL, 6ULL);
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
};

//-----
TEST_F(SystemDeleteTransactionTest, ConstructSystemDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto bodyWithFileId = std::make_unique<proto::SystemDeleteTransactionBody>();
  auto bodyWithContractId = std::make_unique<proto::SystemDeleteTransactionBody>();

  bodyWithFileId->set_allocated_fileid(getTestFileId().toProtobuf().release());
  bodyWithFileId->set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(getTestExpirationTime()));

  bodyWithContractId->set_allocated_contractid(getTestContractId().toProtobuf().release());
  bodyWithContractId->set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(getTestExpirationTime()));

  proto::TransactionBody txBodyWithFileId;
  proto::TransactionBody txBodyWithContractId;
  txBodyWithFileId.set_allocated_systemdelete(bodyWithFileId.release());
  txBodyWithContractId.set_allocated_systemdelete(bodyWithContractId.release());

  // When
  const SystemDeleteTransaction systemDeleteTransactionWithFileId(txBodyWithFileId);
  const SystemDeleteTransaction systemDeleteTransactionWithContractId(txBodyWithContractId);

  // Then (use less-then comparison for expiration time as it chops off the nanoseconds)
  ASSERT_TRUE(systemDeleteTransactionWithFileId.getFileId().has_value());
  EXPECT_EQ(systemDeleteTransactionWithFileId.getFileId(), getTestFileId());
  EXPECT_FALSE(systemDeleteTransactionWithFileId.getContractId().has_value());
  EXPECT_LT(systemDeleteTransactionWithFileId.getExpirationTime(), getTestExpirationTime());

  EXPECT_FALSE(systemDeleteTransactionWithContractId.getFileId().has_value());
  ASSERT_TRUE(systemDeleteTransactionWithContractId.getContractId().has_value());
  EXPECT_EQ(systemDeleteTransactionWithContractId.getContractId(), getTestContractId());
  EXPECT_LT(systemDeleteTransactionWithContractId.getExpirationTime(), getTestExpirationTime());
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetFileId)
{
  // Given
  SystemDeleteTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  ASSERT_TRUE(transaction.getFileId().has_value());
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetFileIdFrozen)
{
  // Given
  SystemDeleteTransaction transaction = SystemDeleteTransaction()
                                          .setNodeAccountIds({ AccountId(1ULL) })
                                          .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetContractId)
{
  // Given
  SystemDeleteTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  ASSERT_TRUE(transaction.getContractId().has_value());
  EXPECT_EQ(transaction.getContractId(), getTestContractId());
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetContractIdFrozen)
{
  // Given
  SystemDeleteTransaction transaction = SystemDeleteTransaction()
                                          .setNodeAccountIds({ AccountId(1ULL) })
                                          .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setContractId(getTestContractId()), IllegalStateException);
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetExpirationTime)
{
  // Given
  SystemDeleteTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setExpirationTime(getTestExpirationTime()));

  // Then
  EXPECT_EQ(transaction.getExpirationTime(), getTestExpirationTime());
}

//-----
TEST_F(SystemDeleteTransactionTest, GetSetExpirationTimeFrozen)
{
  // Given
  SystemDeleteTransaction transaction = SystemDeleteTransaction()
                                          .setNodeAccountIds({ AccountId(1ULL) })
                                          .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setExpirationTime(getTestExpirationTime()), IllegalStateException);
}

//-----
TEST_F(SystemDeleteTransactionTest, ResetFileId)
{
  // Given
  SystemDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setFileId(getTestFileId()));

  // When
  EXPECT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  EXPECT_FALSE(transaction.getFileId().has_value());
  EXPECT_TRUE(transaction.getContractId().has_value());
}

//-----
TEST_F(SystemDeleteTransactionTest, ResetContractId)
{
  // Given
  SystemDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setContractId(getTestContractId()));

  // When
  EXPECT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  EXPECT_TRUE(transaction.getFileId().has_value());
  EXPECT_FALSE(transaction.getContractId().has_value());
}
