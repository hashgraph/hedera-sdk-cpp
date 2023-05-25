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
#include "FileCreateTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "KeyList.h"
#include "exceptions/IllegalStateException.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <proto/file_create.pb.h>
#include <proto/transaction_body.pb.h>
#include <string>
#include <vector>

using namespace Hedera;

class FileCreateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const std::vector<Key*>& getTestKeyVector() const { return mTestKeyVector; }
  [[nodiscard]] inline const KeyList& getTestKeyList() const { return mTestKeyList; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestContents() const { return mTestContents; }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }

private:
  Client mClient;
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey1 = ED25519PrivateKey::fromString(
    "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e10");
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey2 = ED25519PrivateKey::fromString(
    "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e11");
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey3 = ED25519PrivateKey::fromString(
    "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e12");

  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const std::vector<Key*> mTestKeyVector = { mPrivateKey1.get(), mPrivateKey2.get(), mPrivateKey3.get() };
  const KeyList mTestKeyList = KeyList::of(mTestKeyVector);
  const std::vector<std::byte> mTestContents = { std::byte(0x08), std::byte(0x09), std::byte(0x10) };
  const std::string mTestMemo = "test file memo";
};

//-----
TEST_F(FileCreateTransactionTest, ConstructFileCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FileCreateTransactionBody>();
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  body->set_allocated_keys(getTestKeyList().toProtobuf().release());
  body->set_contents(internal::Utilities::byteVectorToString(getTestContents()));
  body->set_memo(getTestMemo());

  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(body.release());

  // When
  const FileCreateTransaction fileCreateTransaction(txBody);

  // Then
  EXPECT_EQ(fileCreateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(fileCreateTransaction.getKeys().toBytes(), getTestKeyList().toBytes());
  EXPECT_EQ(fileCreateTransaction.getContents(), getTestContents());
  EXPECT_EQ(fileCreateTransaction.getFileMemo(), getTestMemo());
}

//-----
TEST_F(FileCreateTransactionTest, GetSetExpirationTime)
{
  // Given
  FileCreateTransaction transaction;

  // When
  transaction.setExpirationTime(getTestExpirationTime());

  // Then
  EXPECT_EQ(transaction.getExpirationTime(), getTestExpirationTime());
}

//-----
TEST_F(FileCreateTransactionTest, GetSetExpirationTimeFrozen)
{
  // Given
  FileCreateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setExpirationTime(getTestExpirationTime()), IllegalStateException);
}

//-----
TEST_F(FileCreateTransactionTest, GetSetKeys)
{
  // Given
  FileCreateTransaction transactionWithKeyVector;
  FileCreateTransaction transactionWithKeyList;

  // When
  transactionWithKeyVector.setKeys(getTestKeyVector());
  transactionWithKeyList.setKeys(getTestKeyList());

  // Then
  const std::vector<std::byte> testKeyListBytes = getTestKeyList().toBytes();
  EXPECT_EQ(transactionWithKeyVector.getKeys().toBytes(), testKeyListBytes);
  EXPECT_EQ(transactionWithKeyList.getKeys().toBytes(), testKeyListBytes);
}

//-----
TEST_F(FileCreateTransactionTest, GetSetKeysFrozen)
{
  // Given
  FileCreateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setKeys(getTestKeyList()), IllegalStateException);
  EXPECT_THROW(transaction.setKeys(getTestKeyVector()), IllegalStateException);
}

//-----
TEST_F(FileCreateTransactionTest, GetSetContents)
{
  // Given
  FileCreateTransaction transaction;

  // When
  transaction.setContents(getTestContents());

  // Then
  EXPECT_EQ(transaction.getContents(), getTestContents());
}

//-----
TEST_F(FileCreateTransactionTest, GetSetContentsFrozen)
{
  // Given
  FileCreateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setContents(getTestContents()), IllegalStateException);
}

//-----
TEST_F(FileCreateTransactionTest, GetSetMemo)
{
  // Given
  FileCreateTransaction transaction;

  // When
  transaction.setFileMemo(getTestMemo());

  // Then
  EXPECT_EQ(transaction.getFileMemo(), getTestMemo());
}

//-----
TEST_F(FileCreateTransactionTest, GetSetMemoFrozen)
{
  // Given
  FileCreateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setFileMemo(getTestMemo()), IllegalStateException);
}
