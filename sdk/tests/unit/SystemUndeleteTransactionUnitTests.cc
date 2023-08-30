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
#include "SystemUndeleteTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class SystemUndeleteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }

private:
  Client mClient;
  const FileId mTestFileId = FileId(1ULL, 2ULL, 3ULL);
  const ContractId mTestContractId = ContractId(4ULL, 5ULL, 6ULL);
};

//-----
TEST_F(SystemUndeleteTransactionTest, ConstructSystemUndeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto bodyWithFileId = std::make_unique<proto::SystemUndeleteTransactionBody>();
  auto bodyWithContractId = std::make_unique<proto::SystemUndeleteTransactionBody>();

  bodyWithFileId->set_allocated_fileid(getTestFileId().toProtobuf().release());
  bodyWithContractId->set_allocated_contractid(getTestContractId().toProtobuf().release());

  proto::TransactionBody txBodyWithFileId;
  proto::TransactionBody txBodyWithContractId;

  txBodyWithFileId.set_allocated_systemundelete(bodyWithFileId.release());
  txBodyWithContractId.set_allocated_systemundelete(bodyWithContractId.release());

  // When
  const SystemUndeleteTransaction systemDeleteTransactionWithFileId(txBodyWithFileId);
  const SystemUndeleteTransaction systemDeleteTransactionWithContractId(txBodyWithContractId);

  // Then
  ASSERT_TRUE(systemDeleteTransactionWithFileId.getFileId().has_value());
  EXPECT_EQ(systemDeleteTransactionWithFileId.getFileId(), getTestFileId());
  EXPECT_FALSE(systemDeleteTransactionWithFileId.getContractId().has_value());

  EXPECT_FALSE(systemDeleteTransactionWithContractId.getFileId().has_value());
  ASSERT_TRUE(systemDeleteTransactionWithContractId.getContractId().has_value());
  EXPECT_EQ(systemDeleteTransactionWithContractId.getContractId(), getTestContractId());
}

//-----
TEST_F(SystemUndeleteTransactionTest, GetSetFileId)
{
  // Given
  SystemUndeleteTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  ASSERT_TRUE(transaction.getFileId().has_value());
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(SystemUndeleteTransactionTest, GetSetFileIdFrozen)
{
  // Given
  SystemUndeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(SystemUndeleteTransactionTest, GetSetContractId)
{
  // Given
  SystemUndeleteTransaction transaction;

  // When
  ASSERT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  ASSERT_TRUE(transaction.getContractId().has_value());
  EXPECT_EQ(transaction.getContractId(), getTestContractId());
}

//-----
TEST_F(SystemUndeleteTransactionTest, GetSetContractIdFrozen)
{
  // Given
  SystemUndeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setContractId(getTestContractId()), IllegalStateException);
}

//-----
TEST_F(SystemUndeleteTransactionTest, ResetFileId)
{
  // Given
  SystemUndeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setFileId(getTestFileId()));

  // When
  EXPECT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  EXPECT_FALSE(transaction.getFileId().has_value());
  EXPECT_TRUE(transaction.getContractId().has_value());
}

//-----
TEST_F(SystemUndeleteTransactionTest, ResetContractId)
{
  // Given
  SystemUndeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setContractId(getTestContractId()));

  // When
  EXPECT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  EXPECT_TRUE(transaction.getFileId().has_value());
  EXPECT_FALSE(transaction.getContractId().has_value());
}