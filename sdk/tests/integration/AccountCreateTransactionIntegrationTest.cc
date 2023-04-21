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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "impl/DurationConverter.h"
#include "impl/Utilities.h"

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
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mInitialBalance; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const { return mAutoRenewPeriod; }
  [[nodiscard]] inline const std::string& getTestAccountMemo() const { return mAccountMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mDeclineStakingReward; }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddress() const { return mEvmAddress; }

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
    networkMap.insert(std::pair<std::string, AccountId>(nodeAddressString, accountId));

    mClient = Client::forNetwork(networkMap);
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));
  }

private:
  const std::shared_ptr<PublicKey> mPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const Hbar mInitialBalance = Hbar(1000ULL, HbarUnit::TINYBAR());
  const bool mReceiverSignatureRequired = true;
  const std::chrono::duration<double> mAutoRenewPeriod = std::chrono::hours(3);
  const std::string mAccountMemo = "Test Account Memo";
  const uint32_t mMaxTokenAssociations = 3U;
  const AccountId mAccountId = AccountId::fromString("0.0.1023");
  const bool mDeclineStakingReward = true;
  const EvmAddress mEvmAddress = EvmAddress::fromString("303132333435363738396162636465666768696a");

  Client mClient;
};

// Tests invoking of method execute() from AccountCreateTransaction.
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteRequestToLocalNode)
{
  // Given
  const auto testBalance = getTestInitialBalance();
  const auto testPublicKey = getTestPublicKey();
  const auto testMemo = "Test memo for AccountCreateTransaction.";

  AccountCreateTransaction testAccountCreateTransaction;
  testAccountCreateTransaction.setKey(testPublicKey);
  testAccountCreateTransaction.setTransactionMemo(testMemo);
  testAccountCreateTransaction.setInitialBalance(testBalance);

  // When
  const TransactionResponse txResponse = testAccountCreateTransaction.execute(getTestClient());
  const TransactionReceipt txReceipt = txResponse.getReceipt(getTestClient());
  const TransactionRecord txRecord = txResponse.getRecord(getTestClient());

  // Then
  EXPECT_TRUE(txResponse.getValidateStatus());
  EXPECT_TRUE(txReceipt.getAccountId().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates()->getCurrentExchangeRate().has_value());
  EXPECT_TRUE(txRecord.getTransactionId().has_value());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
  EXPECT_TRUE(txRecord.getNftTransferList().empty());
}

// Tests invoking of method build() with protobuf object TransactionBody.
TEST_F(AccountCreateTransactionIntegrationTest, ConstructAccountCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  const auto testPublicKey = getTestPublicKey();
  const auto testInitialBalance = getTestInitialBalance();
  const auto testReceiverSignatureRequired = getTestReceiverSignatureRequired();
  const auto testAutoRenewPeriod = getTestAutoRenewPeriod();
  const auto testAccountMemo = getTestAccountMemo();
  const auto testMaxTokenAssociations = getTestMaximumTokenAssociations();
  const auto testAccount = getTestAccountId();
  const auto testEvmAddress = getTestEvmAddress();
  const auto testDeclineStakingReward = getTestDeclineStakingReward();
  const std::vector<std::byte> testPublicKeyBytes = testPublicKey->toBytesDer();

  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_allocated_key(testPublicKey->toProtobuf().release());
  body->set_initialbalance(static_cast<uint64_t>(testInitialBalance.toTinybars()));
  body->set_receiversigrequired(testReceiverSignatureRequired);
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(testAutoRenewPeriod));
  body->set_allocated_memo(new std::string(testAccountMemo));
  body->set_max_automatic_token_associations(static_cast<int32_t>(testMaxTokenAssociations));
  body->set_allocated_staked_account_id(testAccount.toProtobuf().release());
  body->set_decline_reward(testDeclineStakingReward);
  body->set_allocated_alias(new std::string(internal::Utilities::byteVectorToString(testPublicKeyBytes)));

  proto::TransactionBody testTxBody;
  testTxBody.set_allocated_cryptocreateaccount(body.release());

  // When
  AccountCreateTransaction accountCreateTransaction(testTxBody);

  // Then
  EXPECT_EQ(accountCreateTransaction.getKey()->toStringDer(), testPublicKey->toStringDer());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), testInitialBalance);
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), testReceiverSignatureRequired);
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), testAutoRenewPeriod);
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), testAccountMemo);
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), testMaxTokenAssociations);
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(accountCreateTransaction.getStakedAccountId(), testAccount);
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), testDeclineStakingReward);
  EXPECT_EQ(accountCreateTransaction.getPublicKeyAlias()->toBytesDer(), testPublicKeyBytes);
  EXPECT_FALSE(accountCreateTransaction.getEvmAddressAlias().has_value());
}

// Tests construction from protobuf object TransactionBody with EVM address.
TEST_F(AccountCreateTransactionIntegrationTest, ConstructAccountCreateTransactionFromTransactionBodyWithEvmAddress)
{
  // Given
  const auto testInitialBalance = getTestInitialBalance();
  const auto testReceiverSignatureRequired = getTestReceiverSignatureRequired();
  const auto testAutoRenewPeriod = getTestAutoRenewPeriod();
  const auto testAccountMemo = getTestAccountMemo();
  const auto testMaxTokenAssociations = getTestMaximumTokenAssociations();
  const auto testAccount = getTestAccountId();
  const auto testEvmAddress = getTestEvmAddress();
  const auto testDeclineStakingReward = getTestDeclineStakingReward();
  const std::vector<std::byte> testEvmAddressBytes = testEvmAddress.toBytes();

  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_initialbalance(static_cast<uint64_t>(testInitialBalance.toTinybars()));
  body->set_receiversigrequired(testReceiverSignatureRequired);
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(testAutoRenewPeriod));
  body->set_allocated_memo(new std::string(testAccountMemo));
  body->set_max_automatic_token_associations(static_cast<int32_t>(testMaxTokenAssociations));
  body->set_allocated_staked_account_id(testAccount.toProtobuf().release());
  body->set_decline_reward(testDeclineStakingReward);
  body->set_allocated_alias(new std::string(internal::Utilities::byteVectorToString(testEvmAddressBytes)));

  proto::TransactionBody testTxBody;

  testTxBody.set_allocated_cryptocreateaccount(body.release());

  // When
  AccountCreateTransaction accountCreateTransaction(testTxBody);

  // Then
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), testInitialBalance);
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), testReceiverSignatureRequired);
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), testAutoRenewPeriod);
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), testAccountMemo);
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), testMaxTokenAssociations);
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(accountCreateTransaction.getStakedAccountId(), testAccount);
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), testDeclineStakingReward);
  ASSERT_TRUE(accountCreateTransaction.getEvmAddressAlias().has_value());
  EXPECT_EQ(accountCreateTransaction.getEvmAddressAlias()->toBytes(), testEvmAddressBytes);
}