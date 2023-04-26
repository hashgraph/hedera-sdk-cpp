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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Hedera;

class AccountBalanceQueryIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string_view& getJsonNetworkTag() const { return mJsonNetworkTag; }
  [[nodiscard]] inline const std::string_view& getJsonOperatorTag() const { return mJsonOperatorTag; }
  [[nodiscard]] inline const std::string_view& getJsonAccountIdTag() const { return mJsonAccountIdTag; }
  [[nodiscard]] inline const std::string_view& getJsonPrivateKeyTag() const { return mJsonPrivateKeyTag; }

  [[nodiscard]] inline const std::string_view& getNodeAccountIdStr() const { return mNodeAccountIdStr; }
  [[nodiscard]] inline const AccountId& getNodeAccountId() const { return mNodeAccountId; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::string getPathToJSON() const { return mFilePath.string(); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }

  void SetUp() override
  {
    const auto accountId = getNodeAccountId();
    const std::string_view accountIdStr = getNodeAccountIdStr();
    const std::string_view networkTag = getJsonNetworkTag();
    const std::string_view operatorTag = getJsonOperatorTag();
    const std::string_view accountIdTag = getJsonAccountIdTag();
    const std::string_view privateKeyTag = getJsonPrivateKeyTag();

    const std::string testPathToJSON = getPathToJSON();
    const std::unique_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
    const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

    AccountId operatorAccountId;
    std::string operatorAccountPrivateKey;
    std::ifstream testInputFile(testPathToJSON, std::ios::in);
    std::string nodeAddressString;
    json jsonData;

    EXPECT_NO_THROW(jsonData = json::parse(testInputFile));

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
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));
  }

private:
  const std::string_view mJsonNetworkTag = "network";
  const std::string_view mJsonOperatorTag = "operator";
  const std::string_view mJsonAccountIdTag = "accountId";
  const std::string_view mJsonPrivateKeyTag = "privateKey";

  const std::string_view mNodeAccountIdStr = "0.0.3";
  const AccountId mNodeAccountId = AccountId::fromString("0.0.3");
  const AccountId mTestAccountId = AccountId::fromString("0.0.1023");
  const std::filesystem::path mFilePath = std::filesystem::current_path() / "local_node.json";

  Client mClient;
};

// Tests invoking of method execute() from AccountBalanceQuery.
TEST_F(AccountBalanceQueryIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto testAccountId = getTestAccountId();

  AccountBalanceQuery testAccountBalanceQuery;
  testAccountBalanceQuery.setAccountId(testAccountId);

  // When
  const Hbar hbarBalance = testAccountBalanceQuery.execute(getTestClient()).getBalance();

  // Then
  EXPECT_NE(hbarBalance.toTinybars(), 0);
}