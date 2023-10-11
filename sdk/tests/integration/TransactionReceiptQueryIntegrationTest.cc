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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "BaseIntegrationTest.h"
#include "ED25519PrivateKey.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TransactionReceiptQueryIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TransactionReceiptQueryIntegrationTest, CanGetTransactionReceipt)
{
  // Given
  const std::shared_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

  TransactionResponse testTxResponse;
  ASSERT_NO_THROW(testTxResponse = AccountCreateTransaction().setKey(testPublicKey).execute(getTestClient()));

  // When / Then
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(
    txReceipt = TransactionReceiptQuery().setTransactionId(testTxResponse.getTransactionId()).execute(getTestClient()));
  EXPECT_EQ(txReceipt.mTransactionId, testTxResponse.getTransactionId());

  // Clean up
  AccountId accountId;
  ASSERT_NO_THROW(accountId = txReceipt.mAccountId.value());
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(testPrivateKey)
                    .execute(getTestClient()));
}