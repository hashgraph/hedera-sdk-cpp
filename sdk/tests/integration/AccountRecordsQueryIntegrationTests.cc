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
#include "AccountId.h"
#include "AccountRecords.h"
#include "AccountRecordsQuery.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "Defaults.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/PrecheckStatusException.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountRecordsQueryIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountRecordsQueryIntegrationTest, ExecuteAccountRecordsQuery)
{
  // Given
  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const AccountId operatorAccountId(2ULL);
  const Hbar amount(1LL);
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey())
                                .setInitialBalance(amount)
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = TransferTransaction()
                                                         .addHbarTransfer(operatorAccountId, amount.negated())
                                                         .addHbarTransfer(accountId, amount)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = TransferTransaction()
                                                         .addHbarTransfer(operatorAccountId, amount)
                                                         .addHbarTransfer(accountId, amount.negated())
                                                         .freezeWith(&getTestClient())
                                                         .sign(privateKey)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  // When
  AccountRecords accountRecords;
  EXPECT_NO_THROW(accountRecords = AccountRecordsQuery().setAccountId(accountId).execute(getTestClient()));

  // Then
  EXPECT_TRUE(accountRecords.getRecords().empty());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(operatorAccountId)
                    .freezeWith(&getTestClient())
                    .sign(privateKey)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountRecordsQueryIntegrationTest, NoAccountId)
{
  // Given / When / Then
  EXPECT_THROW(AccountRecordsQuery().execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}
