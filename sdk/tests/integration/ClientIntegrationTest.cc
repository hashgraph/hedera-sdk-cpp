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
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/UninitializedException.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Hedera;

class ClientIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string_view& getJsonNetworkTag() const { return mJsonNetworkTag; }
  [[nodiscard]] inline const std::string_view& getJsonOperatorTag() const { return mJsonOperatorTag; }
  [[nodiscard]] inline const std::string_view& getJsonAccountIdTag() const { return mJsonAccountIdTag; }
  [[nodiscard]] inline const std::string_view& getJsonPrivateKeyTag() const { return mJsonPrivateKeyTag; }

  [[nodiscard]] inline const std::string_view& getAccountIdStr() const { return mAccountIdStr; }
  [[nodiscard]] inline const AccountId& getAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::string getPathToJSON() const { return mFilePath.string(); }

private:
  const std::string_view mJsonNetworkTag = "network";
  const std::string_view mJsonOperatorTag = "operator";
  const std::string_view mJsonAccountIdTag = "accountId";
  const std::string_view mJsonPrivateKeyTag = "privateKey";

  const std::string_view mAccountIdStr = "0.0.3";
  const AccountId mAccountId = AccountId::fromString("0.0.3");
  const std::filesystem::path mFilePath = (std::filesystem::current_path() / "local_node.json").string();
};

//-----
TEST_F(ClientIntegrationTest, ConnectToLocalNode)
{
  // Given
  const auto accountId = getAccountId();
  const std::string_view accountIdStr = getAccountIdStr();
  const std::string_view networkTag = getJsonNetworkTag();
  const std::string_view operatorTag = getJsonOperatorTag();
  const std::string_view accountIdTag = getJsonAccountIdTag();
  const std::string_view privateKeyTag = getJsonPrivateKeyTag();

  const std::string testPathToJSON = getPathToJSON();
  const std::unique_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();
  const auto testInitialHbarBalance = Hbar(1000ULL, HbarUnit::TINYBAR());

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

  // When
  Client client = Client::forNetwork(networkMap);
  client.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));

  TransactionResponse txResp =
    AccountCreateTransaction().setKey(testPublicKey).setInitialBalance(testInitialHbarBalance).execute(client);

  const AccountId newAccountId = txResp.getReceipt(client).getAccountId().value();

  // Then
  EXPECT_EQ(client.getOperatorAccountId()->toString(), operatorAccountId.toString());
  EXPECT_NE(client.getOperatorPublicKey(), nullptr);
  EXPECT_FALSE(newAccountId.toString().empty());
}
