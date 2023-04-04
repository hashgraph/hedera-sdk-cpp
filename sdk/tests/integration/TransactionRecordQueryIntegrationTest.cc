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
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TransactionRecordQueryIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const AccountId& getTestAccountOperatorId() const { return mTestAccountOperatorId; }

private:
  const AccountId mTestAccountId = AccountId(10ULL);
  const AccountId mTestAccountOperatorId = AccountId::fromString("0.0.2");
};

// Tests invoking of method execute() from TransactionRecordQuery.
TEST_F(TransactionRecordQueryIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto testMemo = "Test memo for TransactionRecordQuery.";
  const auto testPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  Client testnetClient = Client::forTestnet();
  testnetClient.setOperator(
    getTestAccountOperatorId(),
    ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  AccountCreateTransaction testAccountCreateTransaction;
  testAccountCreateTransaction.setKey(testPublicKey);
  testAccountCreateTransaction.setTransactionMemo(testMemo);

  TransactionId testTransactionId =
    testAccountCreateTransaction.execute(testnetClient).getRecord(testnetClient).getTransactionId().value();

  TransactionRecordQuery testTransactionRecordQuery;
  testTransactionRecordQuery.setTransactionId(testTransactionId);

  // When
  const TransactionRecord txRecord = testTransactionRecordQuery.execute(testnetClient);

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Hedera::Status::SUCCESS);
  EXPECT_EQ(txRecord.getTransactionId(), testTransactionId);
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
}