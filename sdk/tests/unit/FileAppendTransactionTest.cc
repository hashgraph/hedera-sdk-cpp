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
#include "FileAppendTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <chrono>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <proto/file_append.pb.h>
#include <proto/transaction_body.pb.h>
#include <string>
#include <vector>

using namespace Hedera;

class FileAppendTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestContents() const { return mTestContents; }

private:
  Client mClient;

  const FileId mTestFileId = FileId(1ULL);
  const std::vector<std::byte> mTestContents = { std::byte(0x02), std::byte(0x03), std::byte(0x04) };
};

//-----
TEST_F(FileAppendTransactionTest, ConstructFileAppendTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FileAppendTransactionBody>();
  body->set_allocated_fileid(getTestFileId().toProtobuf().release());
  body->set_contents(internal::Utilities::byteVectorToString(getTestContents()));

  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(body.release());

  // When
  const FileAppendTransaction fileAppendTransaction(txBody);

  // Then
  EXPECT_EQ(fileAppendTransaction.getFileId(), getTestFileId());
  EXPECT_EQ(fileAppendTransaction.getContents(), getTestContents());
}

//-----
TEST_F(FileAppendTransactionTest, GetSetFileId)
{
  // Given
  FileAppendTransaction transaction;

  // When
  transaction.setFileId(getTestFileId());

  // Then
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(FileAppendTransactionTest, GetSetFileIdFrozen)
{
  // Given
  FileAppendTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(FileAppendTransactionTest, GetSetContents)
{
  // Given
  FileAppendTransaction transactionBytes;
  FileAppendTransaction transactionStr;

  // When
  transactionBytes.setContents(getTestContents());
  transactionStr.setContents(internal::Utilities::byteVectorToString(getTestContents()));

  // Then
  EXPECT_EQ(transactionBytes.getContents(), getTestContents());
  EXPECT_EQ(transactionStr.getContents(), getTestContents());
}

//-----
TEST_F(FileAppendTransactionTest, GetSetContentsFrozen)
{
  // Given
  FileAppendTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setContents(getTestContents()), IllegalStateException);
  EXPECT_THROW(transaction.setContents(internal::Utilities::byteVectorToString(getTestContents())),
               IllegalStateException);
}
