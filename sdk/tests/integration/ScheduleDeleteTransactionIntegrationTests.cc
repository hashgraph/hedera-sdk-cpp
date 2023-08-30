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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "ScheduleCreateTransaction.h"
#include "ScheduleDeleteTransaction.h"
#include "ScheduleId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "WrappedTransaction.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class ScheduleDeleteTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(ScheduleDeleteTransactionIntegrationTest, ExecuteScheduleDeleteTransaction)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  std::shared_ptr<PrivateKey> newKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  ASSERT_NO_THROW(newKey = ED25519PrivateKey::generatePrivateKey());

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(newKey.get())
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  ScheduleId scheduleId;
  ASSERT_NO_THROW(scheduleId = ScheduleCreateTransaction()
                                 .setScheduledTransaction(scheduledTransaction)
                                 .setAdminKey(operatorKey)
                                 .execute(getTestClient())
                                 .getReceipt(getTestClient())
                                 .mScheduleId.value());

  // When / Then
  EXPECT_NO_THROW(
    const TransactionReceipt txReceipt =
      ScheduleDeleteTransaction().setScheduleId(scheduleId).execute(getTestClient()).getReceipt(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = AccountDeleteTransaction()
                                                         .setDeleteAccountId(accountId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .freezeWith(&getTestClient())
                                                         .sign(newKey.get())
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleDeleteTransactionIntegrationTest, CannotDeleteImmutableSchedule)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  std::shared_ptr<PrivateKey> newKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  ASSERT_NO_THROW(newKey = ED25519PrivateKey::generatePrivateKey());

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(newKey.get())
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  ScheduleId scheduleId;
  ASSERT_NO_THROW(scheduleId = ScheduleCreateTransaction()
                                 .setScheduledTransaction(scheduledTransaction)
                                 .execute(getTestClient())
                                 .getReceipt(getTestClient())
                                 .mScheduleId.value());

  // When / Then
  EXPECT_THROW(
    const TransactionReceipt txReceipt =
      ScheduleDeleteTransaction().setScheduleId(scheduleId).execute(getTestClient()).getReceipt(getTestClient()),
    ReceiptStatusException); // SCHEDULE_IS_IMMUTABLE
}
