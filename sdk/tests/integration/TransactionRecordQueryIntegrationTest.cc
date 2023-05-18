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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <proto/transaction_body.pb.h>

using json = nlohmann::json;
using namespace Hedera;

class TransactionRecordQueryIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }

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
    networkMap.insert(std::pair<std::string, AccountId>(nodeAddressString, accountId));

    mClient = Client::forNetwork(networkMap);
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey).get());
  }

private:
  Client mClient;

  const AccountId mTestAccountId = AccountId::fromString("0.0.1023");
};

//-----
TEST_F(TransactionRecordQueryIntegrationTest, CanGetTransactionRecord)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const auto testPublicKey = testPrivateKey->getPublicKey();

  TransactionResponse testTxResponse;
  ASSERT_NO_THROW(testTxResponse = AccountCreateTransaction().setKey(testPublicKey).execute(getTestClient()));

  // When / Then
  TransactionRecord txRecord;
  EXPECT_NO_THROW(
    txRecord = TransactionRecordQuery().setTransactionId(testTxResponse.getTransactionId()).execute(getTestClient()));

  // Clean up
  AccountId accountId;
  ASSERT_NO_THROW(accountId = txRecord.getReceipt()->getAccountId().value());
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
}

// Tests invoking of method execute() from TransactionRecordQuery.
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteAccountCreateTransaction)
{
  // Given
  const auto testMemo = "Test memo for TransactionRecordQuery.";
  const auto testPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = AccountCreateTransaction()
                               .setKey(testPublicKey)
                               .setTransactionMemo(testMemo)
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
}

//-----
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteEmptyAccountCreateTransaction)
{
  // Given / When / Then
  TransactionRecord txRecord;
  EXPECT_THROW(txRecord = AccountCreateTransaction().execute(getTestClient()).getRecord(getTestClient()),
               Hedera::PrecheckStatusException);
}

//-----
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteAccountCreateTransactionFromProtobuf)
{
  // Given
  const auto testTransactionMemo = "Test memo for CryptoCreateTransactionBody";
  const auto testPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();

  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_allocated_key(testPublicKey->toProtobuf().release());
  body->set_initialbalance(static_cast<uint64_t>(Hbar(10ULL).toTinybars()));
  body->set_receiversigrequired(false);
  body->set_memo("Test memo for CryptoCreateTransactionBody");
  body->set_max_automatic_token_associations(static_cast<int32_t>(3U));
  body->set_allocated_staked_account_id(getTestAccountId().toProtobuf().release());
  body->set_decline_reward(true);

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(body.release());

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = AccountCreateTransaction(txBody)
                               .setTransactionMemo(testTransactionMemo)
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getTransactionMemo(), testTransactionMemo);
}

//-----
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteFileCreateAndDeleteTransactions)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(
    txRecord =
      FileCreateTransaction().setKey(operatorKey->getPublicKey()).execute(getTestClient()).getRecord(getTestClient()));
  const FileId fileId = txRecord.getReceipt()->getFileId().value();

  // Then
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getFileId().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());

  // Clean up
  ASSERT_NO_THROW(txRecord =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getRecord(getTestClient()));
}

//-----
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteContractCreateAndDeleteTransactions)
{
  // Given
  const std::vector<std::byte> contents = internal::Utilities::stringToByteVector(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  FileId fileId;
  TransactionReceipt txReceipt;
  TransactionRecord txRecord;

  // When
  ASSERT_NO_THROW(txReceipt = FileCreateTransaction()
                                .setKey(operatorKey->getPublicKey())
                                .setContents(contents)
                                .setMaxTransactionFee(Hbar(2LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  ASSERT_NO_THROW(fileId = txReceipt.getFileId().value());

  ASSERT_NO_THROW(txRecord = ContractCreateTransaction()
                               .setGas(500000ULL)
                               .setBytecodeFileId(fileId)
                               .setAdminKey(operatorKey->getPublicKey())
                               .setMaxTransactionFee(Hbar(16LL))
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_FALSE(txRecord.getReceipt()->getFileId().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getContractId().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());

  // Clean up
  const ContractId contractId = txRecord.getReceipt()->getContractId().value();
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                         .setContractId(contractId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}