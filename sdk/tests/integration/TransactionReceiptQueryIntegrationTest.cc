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
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/HexConverter.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <proto/transaction_body.pb.h>

using json = nlohmann::json;
using namespace Hedera;

class TransactionReceiptQueryIntegrationTest : public ::testing::Test
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

// Tests invoking of method execute() from TransactionReceiptQuery.
TEST_F(TransactionReceiptQueryIntegrationTest, ExecuteAccountCreateTransaction)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const auto testPublicKey = testPrivateKey->getPublicKey();

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(
    txReceipt = AccountCreateTransaction().setKey(testPublicKey).execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  AccountId accountId;
  ASSERT_NO_THROW(accountId = txReceipt.getAccountId().value());

  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txReceipt.getAccountId().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(TransactionReceiptQueryIntegrationTest, ExecuteEmptyAccountCreateTransaction)
{
  // Given / When
  TransactionReceipt txReceipt;
  EXPECT_THROW(txReceipt = AccountCreateTransaction().execute(getTestClient()).getReceipt(getTestClient()),
               Hedera::PrecheckStatusException);

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::UNKNOWN);
}

//-----
TEST_F(TransactionReceiptQueryIntegrationTest, ExecuteAccountCreateTransactionFromProtobuf)
{
  // Given
  const auto testPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();

  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_allocated_key(testPublicKey->toProtobuf().release());
  body->set_initialbalance(static_cast<uint64_t>(Hbar(10ULL).toTinybars()));
  body->set_receiversigrequired(false);
  body->set_memo("Test Account me Memo");
  body->set_max_automatic_token_associations(static_cast<int32_t>(3U));
  body->set_allocated_staked_account_id(getTestAccountId().toProtobuf().release());
  body->set_decline_reward(true);

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(body.release());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = AccountCreateTransaction(txBody).execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txReceipt.getAccountId().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());
}

//-----
TEST_F(TransactionReceiptQueryIntegrationTest, ExecuteFileCreateTransaction)
{
  // Given
  const std::vector<std::byte> byteCode = internal::HexConverter::hexToBytes(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  const std::unique_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::shared_ptr<PublicKey> operatorPublicKey = operatorPrivateKey->getPublicKey();

  // When
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = FileCreateTransaction().execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  ASSERT_FALSE(txReceipt.getAccountId().has_value());
  ASSERT_TRUE(txReceipt.getFileId().has_value());
}

//-----
TEST_F(TransactionReceiptQueryIntegrationTest, ExecuteContractCreateTransaction)
{
  // Given
  const std::vector<std::byte> byteCode = internal::HexConverter::hexToBytes(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  const std::unique_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::shared_ptr<PublicKey> operatorPublicKey = operatorPrivateKey->getPublicKey();

  FileId fileId;
  TransactionReceipt txReceiptFile;
  TransactionReceipt txReceiptContract;

  // When
  ASSERT_NO_THROW(txReceiptFile = FileCreateTransaction()
                                    .setKey(operatorPublicKey)
                                    .setContents(byteCode)
                                    .setMaxTransactionFee(Hbar(2LL))
                                    .execute(getTestClient())
                                    .getReceipt(getTestClient()));

  ASSERT_NO_THROW(fileId = txReceiptFile.getFileId().value());

  ASSERT_NO_THROW(txReceiptContract = ContractCreateTransaction()
                                        .setGas(500000ULL)
                                        .setBytecodeFileId(fileId)
                                        .setAdminKey(operatorPublicKey)
                                        .setMaxTransactionFee(Hbar(16LL))
                                        .execute(getTestClient())
                                        .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceiptContract.getStatus(), Status::SUCCESS);
  ASSERT_FALSE(txReceiptContract.getAccountId().has_value());
  ASSERT_TRUE(txReceiptContract.getContractId().has_value());
}