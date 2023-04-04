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
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TransferTransactionIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const AccountId& getTestAccountOperatorId() const { return mTestAccountOperatorId; }
  [[nodiscard]] inline const Hbar& getTestBalance() const { return mTestBalance; }

private:
  const AccountId mAccountId = AccountId(10ULL);
  const AccountId mTestAccountOperatorId = AccountId::fromString("0.0.2");
  const Hbar mTestBalance = Hbar(10ULL, HbarUnit::TINYBAR());
};

// Tests invoking of method execute() from TransferTransaction.
TEST_F(TransferTransactionIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto testBalance = getTestBalance();
  const auto testPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const auto accountOperatorId = AccountId::fromString("0.0.1359");
  const auto testMemo = "Test memo for TransferTransaction.";
  Client testnetClient = Client::forTestnet();
  testnetClient.setOperator(
    getTestAccountOperatorId(),
    ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  AccountCreateTransaction testAccountCreateTransaction;
  testAccountCreateTransaction.setKey(testPublicKey);
  testAccountCreateTransaction.setInitialBalance(testBalance);

  TransactionReceipt testTxReceipt =
    testAccountCreateTransaction.execute(testnetClient).getRecord(testnetClient).getReceipt().value();
  AccountId createdAccountId = testTxReceipt.getAccountId().value();

  TransferTransaction transferTransaction;
  transferTransaction.addApprovedHbarTransfer(createdAccountId, Hbar(20ULL, HbarUnit::TINYBAR()));
  transferTransaction.addApprovedHbarTransfer(accountOperatorId, Hbar(-20ULL, HbarUnit::TINYBAR()));
  transferTransaction.setTransactionMemo(testMemo);

  // When
  const TransactionResponse txResponse = transferTransaction.execute(testnetClient);

  // Then
  const TransactionRecord txRecord = txResponse.getRecord(testnetClient);
  const TransactionReceipt txReceipt = txResponse.getReceipt(testnetClient);
  EXPECT_TRUE(txResponse.getValidateStatus());
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
}
