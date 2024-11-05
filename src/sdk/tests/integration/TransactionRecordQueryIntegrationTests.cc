/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "TransactionId.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <transaction_body.pb.h>

using namespace Hedera;

class TransactionRecordQueryIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(TransactionRecordQueryIntegrationTests, CanGetTransactionRecord)
{
  // Given
  const std::shared_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

  TransactionResponse testTxResponse;
  ASSERT_NO_THROW(testTxResponse = AccountCreateTransaction().setKey(testPublicKey).execute(getTestClient()));

  // When / Then
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord =
                    TransactionRecordQuery().setTransactionId(testTxResponse.mTransactionId).execute(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(txRecord.mReceipt->mAccountId.value())
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(testPrivateKey)
                    .execute(getTestClient()));
}