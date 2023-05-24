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
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace Hedera;

class FileCreateTransactionIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }

  void SetUp() override
  {
    const auto accountId = AccountId::fromString("0.0.3");
    const std::string_view accountIdStr = "0.0.3";
    const std::string_view networkTag = "network";
    const std::string_view operatorTag = "operator";
    const std::string_view accountIdTag = "accountId";
    const std::string_view privateKeyTag = "privateKey";

    const std::string testPathToJSON = (std::filesystem::current_path() / "local_node.json").string();
    const std::unique_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
    const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

    AccountId operatorAccountId;
    std::string operatorAccountPrivateKey;
    std::ifstream testInputFile(testPathToJSON, std::ios::in);
    std::string nodeAddressString;
    json jsonData = json::parse(testInputFile);

    if (jsonData[networkTag][accountIdStr].is_string())
    {
      nodeAddressString = jsonData[networkTag][accountIdStr];
    }

    if (jsonData[operatorTag][accountIdTag].is_string() && jsonData[operatorTag][privateKeyTag].is_string())
    {
      std::string operatorAccountIdStr = jsonData[operatorTag][accountIdTag];

      operatorAccountId = AccountId::fromString(operatorAccountIdStr);
      operatorAccountPrivateKey = jsonData[operatorTag][privateKeyTag];
    }

    testInputFile.close();

    std::unordered_map<std::string, AccountId> networkMap;
    networkMap.try_emplace(nodeAddressString, accountId);

    mClient = Client::forNetwork(networkMap);
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey).get());
  }

private:
  Client mClient;
};

//-----
TEST_F(FileCreateTransactionIntegrationTest, ExecuteFileCreateTransaction)
{
  // Given
  std::unique_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  std::vector<std::byte> contents;
  ASSERT_NO_THROW(contents = internal::Utilities::stringToByteVector("[e2e::FileCreateTransaction]"));

  const std::string memo = "test file memo";
  const KeyList keys = KeyList::of({ operatorKey->getPublicKey().get() });

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = FileCreateTransaction()
                                .setKeys(keys)
                                .setContents(contents)
                                .setFileMemo(memo)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  FileId fileId;
  ASSERT_NO_THROW(fileId = txReceipt.getFileId().value());

  FileInfo fileInfo;
  ASSERT_NO_THROW(fileInfo = FileInfoQuery().setFileId(fileId).execute(getTestClient()));

  EXPECT_EQ(fileInfo.mSize, contents.size());
  EXPECT_FALSE(fileInfo.mIsDeleted);
  EXPECT_EQ(fileInfo.mAdminKeys.toBytes(), keys.toBytes());
  EXPECT_EQ(fileInfo.mMemo, memo);

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(FileCreateTransactionIntegrationTest, CanCreateFileWithNoContents)
{
  // Given
  std::unique_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  const KeyList keys = KeyList::of({ operatorKey->getPublicKey().get() });

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt =
                    FileCreateTransaction().setKeys(keys).execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  FileId fileId;
  ASSERT_NO_THROW(fileId = txReceipt.getFileId().value());

  FileInfo fileInfo;
  ASSERT_NO_THROW(fileInfo = FileInfoQuery().setFileId(fileId).execute(getTestClient()));

  EXPECT_EQ(fileInfo.mSize, 0);
  EXPECT_FALSE(fileInfo.mIsDeleted);
  EXPECT_EQ(fileInfo.mAdminKeys.toBytes(), keys.toBytes());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(FileCreateTransactionIntegrationTest, CanCreateFileWithNoKey)
{
  // Given / When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = FileCreateTransaction().execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  FileId fileId;
  ASSERT_NO_THROW(fileId = txReceipt.getFileId().value());

  FileInfo fileInfo;
  ASSERT_NO_THROW(fileInfo = FileInfoQuery().setFileId(fileId).execute(getTestClient()));

  EXPECT_EQ(fileInfo.mSize, 0);
  EXPECT_FALSE(fileInfo.mIsDeleted);
  EXPECT_TRUE(fileInfo.mAdminKeys.empty());
}
