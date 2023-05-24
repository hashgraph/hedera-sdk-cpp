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
#include "AccountId.h"
#include "AccountRecords.h"
#include "AccountRecordsQuery.h"
#include "Client.h"
#include "Defaults.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/PrecheckStatusException.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace Hedera;

class AccountRecordsQueryIntegrationTest : public ::testing::Test
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
TEST_F(AccountRecordsQueryIntegrationTest, ExecuteAccountRecordsQuery)
{
  // Given
  const std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const AccountId operatorAccountId(2ULL);
  const Hbar amount(1LL);
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey().get())
                                .setInitialBalance(amount)
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .getAccountId()
                                .value());
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = TransferTransaction()
                                                         .addHbarTransfer(operatorAccountId, amount.negated())
                                                         .addHbarTransfer(accountId, amount)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = TransferTransaction()
                                                         .addHbarTransfer(operatorAccountId, amount)
                                                         .addHbarTransfer(accountId, amount.negated())
                                                         .freezeWith(getTestClient())
                                                         .sign(privateKey.get())
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  // When
  AccountRecords accountRecords;
  EXPECT_NO_THROW(accountRecords = AccountRecordsQuery().setAccountId(accountId).execute(getTestClient()));

  // Then
  EXPECT_TRUE(accountRecords.getRecords().empty());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(operatorAccountId)
                    .freezeWith(getTestClient())
                    .sign(privateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountRecordsQueryIntegrationTest, NoAccountId)
{
  // Given / When / Then
  EXPECT_THROW(AccountRecordsQuery().execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}
