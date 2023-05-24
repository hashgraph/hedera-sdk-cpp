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
#include "FileUpdateTransaction.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "KeyList.h"
#include "PublicKey.h"
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

class FileUpdateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const KeyList& getTestKeyList() const { return mTestKeyList; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestContents() const { return mTestContents; }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }

private:
  Client mClient;
  const FileId mTestFileId = FileId(1ULL);
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const KeyList mTestKeyList =
    KeyList::of({ PublicKey::fromStringDer(
                    "302A300506032B6570032100BCAF3153262A767B281CC8C888DB3E097C83D690AEF01B8C1BE64D3DE11AACC3")
                    .get() });
  const std::vector<std::byte> mTestContents = { std::byte(0x08), std::byte(0x09), std::byte(0x10) };
  const std::string mTestMemo = "test file memo";
};

//-----
TEST_F(FileUpdateTransactionTest, ConstructFileUpdateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FileUpdateTransactionBody>();
  body->set_allocated_fileid(getTestFileId().toProtobuf().release());
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  body->set_allocated_keys(getTestKeyList().toProtobuf().release());
  body->set_contents(internal::Utilities::byteVectorToString(getTestContents()));

  auto strValue = std::make_unique<google::protobuf::StringValue>();
  strValue->set_value(getTestMemo());
  body->set_allocated_memo(strValue.release());

  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(body.release());

  // When
  const FileUpdateTransaction fileUpdateTransaction(txBody);

  // Then
  EXPECT_EQ(fileUpdateTransaction.getFileId(), getTestFileId());
  ASSERT_TRUE(fileUpdateTransaction.getExpirationTime().has_value());
  EXPECT_EQ(fileUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  ASSERT_TRUE(fileUpdateTransaction.getKeys().has_value());
  EXPECT_EQ(fileUpdateTransaction.getKeys()->toBytes(), getTestKeyList().toBytes());
  ASSERT_TRUE(fileUpdateTransaction.getContents().has_value());
  EXPECT_EQ(fileUpdateTransaction.getContents(), getTestContents());
  ASSERT_TRUE(fileUpdateTransaction.getFileMemo().has_value());
  EXPECT_EQ(fileUpdateTransaction.getFileMemo(), getTestMemo());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetFileId)
{
  // Given
  FileUpdateTransaction transaction;

  // When
  transaction.setFileId(getTestFileId());

  // Then
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetFileIdFrozen)
{
  // Given
  FileUpdateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetExpirationTime)
{
  // Given
  FileUpdateTransaction transaction;

  // When
  transaction.setExpirationTime(getTestExpirationTime());

  // Then
  EXPECT_EQ(transaction.getExpirationTime(), getTestExpirationTime());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetExpirationTimeFrozen)
{
  // Given
  FileUpdateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setExpirationTime(getTestExpirationTime()), IllegalStateException);
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetKeys)
{
  // Given
  FileUpdateTransaction transaction;

  // When
  transaction.setKeys(getTestKeyList());

  // Then
  EXPECT_EQ(transaction.getKeys()->toBytes(), getTestKeyList().toBytes());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetKeysFrozen)
{
  // Given
  FileUpdateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setKeys(getTestKeyList()), IllegalStateException);
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetContents)
{
  // Given
  FileUpdateTransaction transactionBytes;
  FileUpdateTransaction transactionStr;

  // When
  transactionBytes.setContents(getTestContents());
  transactionStr.setContents(internal::Utilities::byteVectorToString(getTestContents()));

  // Then
  EXPECT_EQ(transactionBytes.getContents(), getTestContents());
  EXPECT_EQ(transactionStr.getContents(), getTestContents());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetContentsFrozen)
{
  // Given
  FileUpdateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setContents(getTestContents()), IllegalStateException);
  EXPECT_THROW(transaction.setContents(internal::Utilities::byteVectorToString(getTestContents())),
               IllegalStateException);
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetMemo)
{
  // Given
  FileUpdateTransaction transaction;

  // When
  transaction.setFileMemo(getTestMemo());

  // Then
  EXPECT_EQ(transaction.getFileMemo(), getTestMemo());
}

//-----
TEST_F(FileUpdateTransactionTest, GetSetMemoFrozen)
{
  // Given
  FileUpdateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setFileMemo(getTestMemo()), IllegalStateException);
}
