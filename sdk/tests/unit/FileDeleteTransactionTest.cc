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
#include "FileDeleteTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/file_delete.pb.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class FileDeleteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }

private:
  Client mClient;
  const FileId mTestFileId = FileId(1ULL);
};

//-----
TEST_F(FileDeleteTransactionTest, ConstructFileDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FileDeleteTransactionBody>();
  body->set_allocated_fileid(getTestFileId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(body.release());

  // When
  const FileDeleteTransaction fileDeleteTransaction(txBody);

  // Then
  EXPECT_EQ(fileDeleteTransaction.getFileId(), getTestFileId());
}

//-----
TEST_F(FileDeleteTransactionTest, GetSetFileId)
{
  // Given
  FileDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setFileId(getTestFileId()));

  // Then
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(FileDeleteTransactionTest, GetSetFileIdFrozen)
{
  // Given
  FileDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}
