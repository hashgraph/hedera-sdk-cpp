// SPDX-License-Identifier: Apache-2.0
#include "AccountCreateTransaction.h"
#include "BaseIntegrationTest.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "ScheduleCreateTransaction.h"
#include "ScheduleInfo.h"
#include "ScheduleInfoQuery.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "WrappedTransaction.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hiero;

class ScheduleCreateTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, ExecuteScheduleCreateTransaction)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = ScheduleCreateTransaction()
                                .setScheduledTransaction(scheduledTransaction)
                                .setAdminKey(operatorKey)
                                .setPayerAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  ASSERT_TRUE(txReceipt.mScheduleId.has_value());

  ScheduleInfo scheduleInfo;
  EXPECT_NO_THROW(scheduleInfo =
                    ScheduleInfoQuery().setScheduleId(txReceipt.mScheduleId.value()).execute(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(txReceipt = AccountDeleteTransaction()
                                .setDeleteAccountId(accountId)
                                .setTransferAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, CanCreateScheduleCreateTransactionWithScheduleFunction)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  TransferTransaction scheduledTransaction;
  ASSERT_NO_THROW(
    scheduledTransaction =
      TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = scheduledTransaction.schedule()
                                .setAdminKey(operatorKey)
                                .setPayerAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  // TODO: ScheduleInfoQuery

  // Clean up
  ASSERT_NO_THROW(txReceipt = AccountDeleteTransaction()
                                .setDeleteAccountId(accountId)
                                .setTransferAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, CannotScheduleTwoIdenticalTransactions)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  ScheduleCreateTransaction scheduleTransaction =
    ScheduleCreateTransaction()
      .setScheduledTransaction(WrappedTransaction(TransferTransaction()
                                                    .addHbarTransfer(accountId, Hbar(-5LL))
                                                    .addHbarTransfer(AccountId(2ULL), Hbar(5LL))
                                                    .freezeWith(&getTestClient())))
      .setAdminKey(operatorKey)
      .setPayerAccountId(AccountId(2ULL));

  ScheduleCreateTransaction firstScheduleTransaction = scheduleTransaction;
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    firstScheduleTransaction.execute(getTestClient()).getReceipt(getTestClient()));

  // When / Then
  ScheduleCreateTransaction secondScheduleTransaction = scheduleTransaction;
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 secondScheduleTransaction.execute(getTestClient()).getReceipt(getTestClient()),
               ReceiptStatusException); // IDENTICAL_SCHEDULE_ALREADY_CREATED

  // Then
  // TODO: ScheduleInfoQuery

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = AccountDeleteTransaction()
                                                         .setDeleteAccountId(accountId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .freezeWith(&getTestClient())
                                                         .sign(newKey)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, CanSignScheduleAndWaitForExpiry)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = ScheduleCreateTransaction()
                                .setScheduledTransaction(scheduledTransaction)
                                .setWaitForExpiry(true)
                                .setExpirationTime(std::chrono::system_clock::now() + std::chrono::hours(24))
                                .setAdminKey(operatorKey)
                                .setPayerAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  ASSERT_TRUE(txReceipt.mScheduleId.has_value());
  const ScheduleId scheduleId = txReceipt.mScheduleId.value();

  ScheduleInfo scheduleInfo;
  EXPECT_NO_THROW(scheduleInfo =
                    ScheduleInfoQuery().setScheduleId(txReceipt.mScheduleId.value()).execute(getTestClient()));

  EXPECT_NO_THROW(txReceipt = ScheduleSignTransaction()
                                .setScheduleId(scheduleId)
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(txReceipt = AccountDeleteTransaction()
                                .setDeleteAccountId(accountId)
                                .setTransferAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, CannotScheduleOneYearIntoTheFuture)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  // When / Then
  TransactionReceipt txReceipt;
  EXPECT_THROW(txReceipt = ScheduleCreateTransaction()
                             .setScheduledTransaction(scheduledTransaction)
                             .setExpirationTime(std::chrono::system_clock::now() + std::chrono::hours(8760))
                             .setAdminKey(operatorKey)
                             .setPayerAccountId(AccountId(2ULL))
                             .execute(getTestClient())
                             .getReceipt(getTestClient()),
               ReceiptStatusException);

  // Clean up
  ASSERT_NO_THROW(txReceipt = AccountDeleteTransaction()
                                .setDeleteAccountId(accountId)
                                .setTransferAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(ScheduleCreateTransactionIntegrationTests, CannotScheduleInThePast)
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
                                .setKey(newKey)
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  const WrappedTransaction scheduledTransaction(
    TransferTransaction().addHbarTransfer(accountId, Hbar(-5LL)).addHbarTransfer(AccountId(2ULL), Hbar(5LL)));

  // When / Then
  TransactionReceipt txReceipt;
  EXPECT_THROW(txReceipt = ScheduleCreateTransaction()
                             .setScheduledTransaction(scheduledTransaction)
                             .setExpirationTime(std::chrono::system_clock::now() - std::chrono::hours(24))
                             .setAdminKey(operatorKey)
                             .setPayerAccountId(AccountId(2ULL))
                             .execute(getTestClient())
                             .getReceipt(getTestClient()),
               ReceiptStatusException);

  // Clean up
  ASSERT_NO_THROW(txReceipt = AccountDeleteTransaction()
                                .setDeleteAccountId(accountId)
                                .setTransferAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}