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
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "Client.h"
#include "Defaults.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/DurationConverter.h"
#include "impl/HexConverter.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <proto/transaction_body.pb.h>

using json = nlohmann::json;
using namespace Hedera;

class AccountCreateTransactionIntegrationTest : public ::testing::Test
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
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteAccountCreateTransaction)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> testPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> testPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(testPrivateKey->getPublicKey());
  const EvmAddress testEvmAddress = testPublicKey->toEvmAddress();
  const Hbar testInitialBalance(1000LL, HbarUnit::TINYBAR());
  const std::chrono::duration<double> testAutoRenewPeriod = std::chrono::seconds(2592000);
  const std::string testMemo = "test account memo";
  const uint32_t testMaxAutomaticTokenAssociations = 4U;

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setKey(testPublicKey)
                                 .setInitialBalance(testInitialBalance)
                                 .setReceiverSignatureRequired(true)
                                 .setAutoRenewPeriod(testAutoRenewPeriod)
                                 .setAccountMemo(testMemo)
                                 .setMaxAutomaticTokenAssociations(testMaxAutomaticTokenAssociations)
                                 .setDeclineStakingReward(true)
                                 .setAlias(testEvmAddress)
                                 .freezeWith(getTestClient())
                                 .sign(testPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.getAccountId(), accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.getContractAccountId()), testEvmAddress.toBytes());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), testPublicKey->toBytesDer());
  EXPECT_EQ(accountInfo.getBalance(), testInitialBalance);
  EXPECT_EQ(accountInfo.getAutoRenewPeriod(), testAutoRenewPeriod);
  EXPECT_EQ(accountInfo.getMemo(), testMemo);
  EXPECT_EQ(accountInfo.getMaxAutomaticTokenAssociations(), testMaxAutomaticTokenAssociations);
  EXPECT_TRUE(accountInfo.getStakingInfo().getDeclineReward());
  EXPECT_FALSE(accountInfo.getStakingInfo().getStakedAccountId().has_value());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteAccountCreateTransactionMutuallyExclusiveStakingIds)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> testPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey());
  const AccountId operatorAccountId(2ULL);
  const uint64_t nodeId = 0ULL;

  // When
  TransactionResponse txResponseStakedAccountId;
  TransactionResponse txResponseStakedNodeId;

  EXPECT_NO_THROW(txResponseStakedAccountId = AccountCreateTransaction()
                                                .setKey(testPublicKey)
                                                .setStakedAccountId(operatorAccountId)
                                                .freezeWith(getTestClient())
                                                .execute(getTestClient()));
  EXPECT_NO_THROW(txResponseStakedNodeId = AccountCreateTransaction()
                                             .setKey(testPublicKey)
                                             .setStakedNodeId(nodeId)
                                             .freezeWith(getTestClient())
                                             .execute(getTestClient()));

  // Then
  AccountId accountIdStakedAccountId;
  AccountId accountIdStakedNodeId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountIdStakedAccountId =
                    txResponseStakedAccountId.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountIdStakedNodeId = txResponseStakedNodeId.getReceipt(getTestClient()).getAccountId().value());

  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountIdStakedAccountId).execute(getTestClient()));
  EXPECT_EQ(accountInfo.getAccountId(), accountIdStakedAccountId);
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), testPublicKey->toBytesDer());
  ASSERT_TRUE(accountInfo.getStakingInfo().getStakedAccountId().has_value());
  EXPECT_EQ(accountInfo.getStakingInfo().getStakedAccountId(), operatorAccountId);
  EXPECT_FALSE(accountInfo.getStakingInfo().getStakedNodeId().has_value());

  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountIdStakedNodeId).execute(getTestClient()));
  EXPECT_EQ(accountInfo.getAccountId(), accountIdStakedNodeId);
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), testPublicKey->toBytesDer());
  EXPECT_FALSE(accountInfo.getStakingInfo().getStakedAccountId().has_value());
  ASSERT_TRUE(accountInfo.getStakingInfo().getStakedNodeId().has_value());
  EXPECT_EQ(accountInfo.getStakingInfo().getStakedNodeId(), nodeId);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountIdStakedAccountId)
                    .setTransferAccountId(operatorAccountId)
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountIdStakedNodeId)
                    .setTransferAccountId(operatorAccountId)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteAccountCreateTransactionWithNoInitialBalance)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> testKey = ED25519PrivateKey::generatePrivateKey();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction().setKey(testKey->getPublicKey()).execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.getAccountId(), accountId);
  EXPECT_FALSE(accountInfo.getIsDeleted());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), testKey->getPublicKey()->toBytesDer());
  EXPECT_EQ(accountInfo.getBalance(), Hbar(0LL));
  EXPECT_EQ(accountInfo.getAutoRenewPeriod(), DEFAULT_AUTO_RENEW_PERIOD);
  EXPECT_EQ(accountInfo.getProxyReceived(), Hbar(0LL));

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteAccountCreateTransactionWithAliasFromAdminKey)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey());
  const EvmAddress evmAddress = adminKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminKey)
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse =
                    AccountCreateTransaction().setKey(adminKey).setAlias(evmAddress).execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.getAccountId(), accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.getContractAccountId()), evmAddress.toBytes());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), adminKey->toBytesDer());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest,
       ExecuteAccountCreateTransactionWithAliasFromAdminKeyWithReceiverSigRequired)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> adminKeyPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminKeyPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(adminKeyPrivateKey->getPublicKey());
  const EvmAddress evmAddress = adminKeyPublicKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminKeyPublicKey)
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setReceiverSignatureRequired(true)
                                 .setKey(adminKeyPublicKey)
                                 .setAlias(evmAddress)
                                 .freezeWith(getTestClient())
                                 .sign(adminKeyPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.getAccountId(), accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.getContractAccountId()), evmAddress.toBytes());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), adminKeyPublicKey->toBytesDer());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest,
       ExecuteAccountCreateTransactionWithAliasFromAdminKeyWithReceiverSigRequiredWithoutSignature)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> adminKeyPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminKeyPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(adminKeyPrivateKey->getPublicKey());
  const EvmAddress evmAddress = adminKeyPublicKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminKeyPublicKey)
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  EXPECT_THROW(const TransactionReceipt txReceipt = AccountCreateTransaction()
                                                      .setReceiverSignatureRequired(true)
                                                      .setKey(adminKeyPublicKey)
                                                      .setAlias(evmAddress)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest,
       ExecuteAccountCreateTransactionWithAliasDifferentFromAdminKeyWithReceiverSigRequired)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminPrivateKey = ED25519PrivateKey::generatePrivateKey();
  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminPrivateKey->getPublicKey())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> aliasPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const EvmAddress alias =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(aliasPrivateKey->getPublicKey())->toEvmAddress();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setReceiverSignatureRequired(true)
                                 .setKey(adminPrivateKey->getPublicKey())
                                 .setAlias(alias)
                                 .freezeWith(getTestClient())
                                 .sign(adminPrivateKey.get())
                                 .sign(aliasPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.getAccountId(), accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.getContractAccountId()), alias.toBytes());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), adminPrivateKey->getPublicKey()->toBytesDer());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest,
       ExecuteAccountCreateTransactionWithAliasDifferentFromAdminKeyWithReceiverSigRequiredWithoutSignature)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminPrivateKey = ED25519PrivateKey::generatePrivateKey();
  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminPrivateKey->getPublicKey())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> aliasPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const EvmAddress alias =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(aliasPrivateKey->getPublicKey())->toEvmAddress();

  // When
  EXPECT_THROW(const TransactionReceipt txReceipt = AccountCreateTransaction()
                                                      .setReceiverSignatureRequired(true)
                                                      .setKey(adminPrivateKey->getPublicKey())
                                                      .setAlias(alias)
                                                      .freezeWith(getTestClient())
                                                      .sign(aliasPrivateKey.get())
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}
