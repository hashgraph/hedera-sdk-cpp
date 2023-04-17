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
#include "ECDSAsecp256k1PrivateKey.h"
#include "PublicKey.h"
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
#include <string_view>
#include <vector>

using namespace Hedera;

class FileCreateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestKey() const { return mTestKey; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestContents() const { return mTestContents; }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }

private:
  Client mClient;
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const std::shared_ptr<PublicKey> mTestKey = PublicKey::fromStringDer(
    "302A300506032B6570032100BCAF3153262A767B281CC8C888DB3E097C83D690AEF01B8C1BE64D3DE11AACC3");
  const std::vector<std::byte> mTestContents = { std::byte(0x08), std::byte(0x09), std::byte(0x10) };
  const std::string mTestMemo = "test file memo";
};

//-----
TEST_F(FileCreateTransactionTest, ConstructFileCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::FileCreateTransactionBody>();
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  *body->mutable_keys()->add_keys() = *getTestKey()->toProtobuf();
  body->set_contents(internal::Utilities::byteVectorToString(getTestContents()));
  body->set_memo(getTestMemo());

  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(body.release());

  // When
  const FileCreateTransaction fileCreateTransaction(txBody);

  // Then
  EXPECT_EQ(fileCreateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(fileCreateTransaction.getKey()->toBytesDer(), getTestKey()->toBytesDer());
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
TEST_F(FileCreateTransactionTest, GetSetKey)
{
  // Given
  FileCreateTransaction transaction;

  // When
  transaction.setKey(getTestKey());

  // Then
  EXPECT_EQ(transaction.getKey()->toBytesDer(), getTestKey()->toBytesDer());
}

//-----
TEST_F(FileCreateTransactionTest, GetSetKeyFrozen)
{
  // Given
  FileCreateTransaction transaction;
  transaction.freezeWith(getTestClient());

  // When / Then
  EXPECT_THROW(transaction.setKey(getTestKey()), IllegalStateException);
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
