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
#include "AccountAllowanceApproveTransaction.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
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

class AccountAllowanceApproveTransactionIntegrationTest : public ::testing::Test
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

    const std::string testPathToJSON = std::filesystem::current_path() / "local_node.json";
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
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));
  }

private:
  Client mClient;
};

//-----
TEST_F(AccountAllowanceApproveTransactionIntegrationTest, ExecuteAccountAllowanceApproveTransaction)
{
  // Given
  const Hbar amount(5LL);
  const std::unique_ptr<PrivateKey> allowerKey = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<PrivateKey> alloweeKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  AccountId allowerAccountId;
  AccountId alloweeAccountId;
  ASSERT_NO_THROW(allowerAccountId = AccountCreateTransaction()
                                       .setKey(allowerKey->getPublicKey())
                                       .setInitialBalance(amount)
                                       .execute(getTestClient())
                                       .getReceipt(getTestClient())
                                       .getAccountId()
                                       .value());
  ASSERT_NO_THROW(alloweeAccountId = AccountCreateTransaction()
                                       .setKey(alloweeKey->getPublicKey())
                                       .setInitialBalance(amount)
                                       .execute(getTestClient())
                                       .getReceipt(getTestClient())
                                       .getAccountId()
                                       .value());

  // When / Then
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountAllowanceApproveTransaction()
                                 .approveHbarAllowance(allowerAccountId, alloweeAccountId, amount)
                                 .freezeWith(getTestClient())
                                 .sign(allowerKey.get())
                                 .execute(getTestClient()));

  // Then
  EXPECT_NO_THROW(const TransactionReceipt txReceipt = txResponse.getReceipt(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(allowerAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(alloweeAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountAllowanceApproveTransactionIntegrationTest,
       ExecuteAccountAllowanceApproveTransactionWithoutAllowerSignature)
{
  // Given
  const Hbar amount(5LL);
  const std::unique_ptr<PrivateKey> allowerKey = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<PrivateKey> alloweeKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  AccountId allowerAccountId;
  AccountId alloweeAccountId;
  ASSERT_NO_THROW(allowerAccountId = AccountCreateTransaction()
                                       .setKey(allowerKey->getPublicKey())
                                       .setInitialBalance(amount)
                                       .execute(getTestClient())
                                       .getReceipt(getTestClient())
                                       .getAccountId()
                                       .value());
  ASSERT_NO_THROW(alloweeAccountId = AccountCreateTransaction()
                                       .setKey(alloweeKey->getPublicKey())
                                       .setInitialBalance(amount)
                                       .execute(getTestClient())
                                       .getReceipt(getTestClient())
                                       .getAccountId()
                                       .value());

  // When / Then
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountAllowanceApproveTransaction()
                                 .approveHbarAllowance(allowerAccountId, alloweeAccountId, amount)
                                 .execute(getTestClient()));

  // Then
  EXPECT_THROW(const TransactionReceipt txReceipt = txResponse.getReceipt(getTestClient()), ReceiptStatusException);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(allowerAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(alloweeAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}
