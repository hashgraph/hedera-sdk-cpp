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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace Hedera;

class AccountDeleteTransactionIntegrationTest : public ::testing::Test
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
TEST_F(AccountDeleteTransactionIntegrationTest, ExecuteAccountDeleteTransaction)
{
  // Given
  const std::unique_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(key->getPublicKey().get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .getAccountId()
                                .value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountDeleteTransaction()
                                 .setDeleteAccountId(accountId)
                                 .setTransferAccountId(AccountId(2ULL))
                                 .freezeWith(getTestClient())
                                 .sign(key.get())
                                 .execute(getTestClient()));

  // Then
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = txResponse.getReceipt(getTestClient()));
}

//-----
TEST_F(AccountDeleteTransactionIntegrationTest, CannotDeleteInvalidAccountId)
{
  // Given / When / Then
  EXPECT_THROW(AccountDeleteTransaction().setTransferAccountId(AccountId(2ULL)).execute(getTestClient()),
               PrecheckStatusException);
}

//-----
TEST_F(AccountDeleteTransactionIntegrationTest, CannotDeleteAccountWithoutSignature)
{
  // Given
  const std::unique_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(key->getPublicKey().get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .getAccountId()
                                .value());

  // When / Then
  TransactionReceipt txReceipt;
  EXPECT_THROW(txReceipt = AccountDeleteTransaction()
                             .setDeleteAccountId(accountId)
                             .setTransferAccountId(AccountId(2ULL))
                             .execute(getTestClient())
                             .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(key.get())
                    .execute(getTestClient()));
}
