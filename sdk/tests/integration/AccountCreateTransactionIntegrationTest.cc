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

#include <gtest/gtest.h>

using namespace Hedera;

class AccountCreateTransactionIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const AccountId& getTestAccountOperatorId() const { return mTestAccountOperatorId; }
  [[nodiscard]] inline const Hbar& getTestBalance() const { return mTestBalance; }

private:
  const std::shared_ptr<PublicKey> mPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const AccountId mTestAccountOperatorId = AccountId::fromString("0.0.2");
  const Hbar mTestBalance = Hbar(1000ULL, HbarUnit::TINYBAR());
};

// Tests invoking of method execute() from AccountCreateTransaction.
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto testBalance = getTestBalance();
  const auto testPublicKey = getTestPublicKey();
  const auto testMemo = "Test memo for AccountCreateTransaction.";
  Client testnetClient = Client::forTestnet();
  testnetClient.setOperator(
    getTestAccountOperatorId(),
    ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  AccountCreateTransaction testAccountCreateTransaction;
  testAccountCreateTransaction.setKey(testPublicKey);
  testAccountCreateTransaction.setTransactionMemo(testMemo);
  testAccountCreateTransaction.setInitialBalance(testBalance);

  // When
  const TransactionResponse txResponse = testAccountCreateTransaction.execute(testnetClient);

  // Then
  const TransactionReceipt txReceipt = txResponse.getReceipt(testnetClient);
  const TransactionRecord txRecord = txResponse.getRecord(testnetClient);

  EXPECT_TRUE(txResponse.getValidateStatus());
  EXPECT_TRUE(txReceipt.getAccountId().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates()->getCurrentExchangeRate().has_value());
  EXPECT_TRUE(txRecord.getTransactionId().has_value());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
  EXPECT_FALSE(txRecord.getNftTransferList().empty());
}
