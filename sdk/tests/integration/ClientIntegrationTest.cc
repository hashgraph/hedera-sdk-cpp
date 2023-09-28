/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "AccountCreateTransaction.h"
#include "AccountId.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/UninitializedException.h"
#include "impl/Utilities.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace std;
using namespace Hedera;

class ClientIntegrationTest : public BaseIntegrationTest
{
protected:
  // [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::string_view& getJsonNetworkTag() const { return mJsonNetworkTag; }
  [[nodiscard]] inline const std::string_view& getJsonOperatorTag() const { return mJsonOperatorTag; }
  [[nodiscard]] inline const std::string_view& getJsonAccountIdTag() const { return mJsonAccountIdTag; }
  [[nodiscard]] inline const std::string_view& getJsonPrivateKeyTag() const { return mJsonPrivateKeyTag; }

  [[nodiscard]] inline const std::string_view& getAccountIdStr() const { return mAccountIdStr; }
  [[nodiscard]] inline const AccountId& getAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::string getPathToJSON() const { return mFilePath.string(); }

  [[nodiscard]] inline const std::chrono::milliseconds getNegativeBackoffTime() const { return mNegativeBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds getZeroBackoffTime() const { return mZeroBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds getBelowMinBackoffTime() const { return mBelowMinBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds getAboveMaxBackoffTime() const { return mAboveMaxBackoffTime; }

private:
  // Client mClient;

  const std::string_view mJsonNetworkTag = "network";
  const std::string_view mJsonOperatorTag = "operator";
  const std::string_view mJsonAccountIdTag = "accountId";
  const std::string_view mJsonPrivateKeyTag = "privateKey";

  const std::chrono::milliseconds mNegativeBackoffTime = std::chrono::milliseconds(-1);
  const std::chrono::milliseconds mZeroBackoffTime = std::chrono::milliseconds(0);
  const std::chrono::milliseconds mBelowMinBackoffTime = DEFAULT_MIN_BACKOFF - std::chrono::milliseconds(1);
  const std::chrono::milliseconds mAboveMaxBackoffTime = DEFAULT_MAX_BACKOFF + std::chrono::milliseconds(1);

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
  client.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey).get());

  TransactionResponse txResp =
    AccountCreateTransaction().setKey(testPublicKey.get()).setInitialBalance(testInitialHbarBalance).execute(client);

  const AccountId newAccountId = txResp.getReceipt(client).mAccountId.value();

  // Then
  EXPECT_EQ(client.getOperatorAccountId()->toString(), operatorAccountId.toString());
  EXPECT_NE(client.getOperatorPublicKey(), nullptr);
  EXPECT_FALSE(newAccountId.toString().empty());
}

//-----
TEST_F(ClientIntegrationTest, SetInvalidMinBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_THROW(client.setMinBackoff(getNegativeBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMinBackoff(getAboveMaxBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
}

//-----
TEST_F(ClientIntegrationTest, SetValidMinBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_NO_THROW(client.setMinBackoff(getZeroBackoffTime()));
  EXPECT_NO_THROW(client.setMinBackoff(DEFAULT_MIN_BACKOFF));
  EXPECT_NO_THROW(client.setMinBackoff(DEFAULT_MAX_BACKOFF));
}

//-----
TEST_F(ClientIntegrationTest, SetInvalidMaxBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_THROW(client.setMaxBackoff(getNegativeBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getZeroBackoffTime()), std::invalid_argument);     // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getBelowMinBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getAboveMaxBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
}

//-----
TEST_F(ClientIntegrationTest, SetValidMaxBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_NO_THROW(client.setMaxBackoff(DEFAULT_MIN_BACKOFF));
  EXPECT_NO_THROW(client.setMaxBackoff(DEFAULT_MAX_BACKOFF));
}

//-----
TEST_F(ClientIntegrationTest, SetNetworkIskWorkingCorrectly)
{
  // Given
  const std::unique_ptr<PrivateKey> myPrivateKey = ED25519PrivateKey::fromString(
    "302e020100300506032b6570042204202e000363977258a41f418cf84a7df9cf1e8ae98b72de86803e64846defa43054");
  const AccountId accountId_3 = AccountId::fromString("0.0.3");
  const AccountId accountId_4 = AccountId::fromString("0.0.4");
  const AccountId accountId_5 = AccountId::fromString("0.0.5");

  std::unordered_map<std::string, AccountId> testnetMap;
  testnetMap.insert(std::pair<std::string, AccountId>("34.94.106.61:50211", accountId_3));
  testnetMap.insert(std::pair<std::string, AccountId>("35.237.119.55:50211", accountId_4));

  cout << "!!! Got accountId_3 & accountId_4 !!!" << endl << endl;

  Client client = Client::forNetwork(testnetMap);

  cout << "!!! START !!!" << endl << endl;

  // Given
  AccountBalance accountBalance_3;
  AccountBalance accountBalance_4;
  AccountBalance accountBalance_5;

  ASSERT_NO_THROW(accountBalance_3 = AccountBalanceQuery().setAccountId(accountId_3).execute(client));
  ASSERT_NO_THROW(accountBalance_4 = AccountBalanceQuery().setAccountId(accountId_4).execute(client));

  cout << "Balance for Account 3: " << accountBalance_3.getBalance().toTinybars() << " tynibars." << endl;
  cout << "Balance for Account 4: " << accountBalance_4.getBalance().toTinybars() << " tynibars." << endl << endl;

  // When / Then
  std::unordered_map<std::string, AccountId> newTestnetMap;
  newTestnetMap.insert(std::pair<std::string, AccountId>("35.237.119.55:50211", accountId_4));
  newTestnetMap.insert(std::pair<std::string, AccountId>("35.245.27.193:50211", accountId_5));

  client.setNetwork(newTestnetMap);

  cout << "!!! newTestnetMap was configured !!!" << endl << endl;

  ASSERT_NO_THROW(accountBalance_4 = AccountBalanceQuery().setAccountId(accountId_4).execute(client));
  ASSERT_NO_THROW(accountBalance_5 = AccountBalanceQuery().setAccountId(accountId_5).execute(client));

  cout << "Balance for Account 4: " << accountBalance_4.getBalance().toTinybars() << " tynibars." << endl;
  cout << "Balance for Account 5: " << accountBalance_5.getBalance().toTinybars() << " tynibars." << endl << endl;

  cout << "!!! END !!!" << endl << endl;
}
