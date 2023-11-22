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
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "AccountUpdateTransaction.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountUpdateTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountUpdateTransactionIntegrationTests, ExecuteAccountUpdateTransaction)
{
  // Given
  const std::shared_ptr<PrivateKey> initialPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PrivateKey> newPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const bool newReceiverSignatureRequired = true;
  const std::chrono::system_clock::duration newAutoRenewPeriod = std::chrono::seconds(8000000);
  const std::chrono::system_clock::time_point newExpirationTime =
    std::chrono::system_clock::now() + std::chrono::seconds(3000000);
  const std::string newAccountMemo = "New Account Memo!";
  const uint32_t newMaxAutomaticTokenAssociations = 100U;
  const uint64_t newStakedNodeId = 0ULL;
  const bool newDeclineStakingRewards = true;

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(initialPrivateKey->getPublicKey())
                                .setAutoRenewPeriod(std::chrono::seconds(2592000))
                                .setAccountMemo("test account memo")
                                .setMaxAutomaticTokenAssociations(10U)
                                .setStakedAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountUpdateTransaction()
                                 .setAccountId(accountId)
                                 .setKey(newPrivateKey->getPublicKey())
                                 .setReceiverSignatureRequired(newReceiverSignatureRequired)
                                 .setAutoRenewPeriod(newAutoRenewPeriod)
                                 .setExpirationTime(newExpirationTime)
                                 .setAccountMemo(newAccountMemo)
                                 .setMaxAutomaticTokenAssociations(newMaxAutomaticTokenAssociations)
                                 .setStakedNodeId(newStakedNodeId)
                                 .setDeclineStakingReward(newDeclineStakingRewards)
                                 .freezeWith(&getTestClient())
                                 .sign(initialPrivateKey)
                                 .sign(newPrivateKey)
                                 .execute(getTestClient()));

  // Then
  AccountInfo accountInfo;
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = txResponse.getReceipt(getTestClient()));
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_FALSE(accountInfo.mIsDeleted);
  EXPECT_EQ(accountInfo.mKey->toBytes(), newPrivateKey->getPublicKey()->toBytes());
  EXPECT_EQ(accountInfo.mReceiverSignatureRequired, newReceiverSignatureRequired);
  // Can't do direct time comparison due to system clock precision limitations
  EXPECT_LE(accountInfo.mExpirationTime, newExpirationTime);
  EXPECT_EQ(accountInfo.mAutoRenewPeriod, newAutoRenewPeriod);
  EXPECT_EQ(accountInfo.mMemo, newAccountMemo);
  EXPECT_EQ(accountInfo.mMaxAutomaticTokenAssociations, newMaxAutomaticTokenAssociations);
  ASSERT_TRUE(accountInfo.mStakingInfo.getStakedNodeId().has_value());
  EXPECT_EQ(accountInfo.mStakingInfo.getStakedNodeId(), newStakedNodeId);
  EXPECT_EQ(accountInfo.mStakingInfo.getDeclineReward(), newDeclineStakingRewards);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(newPrivateKey)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountUpdateTransactionIntegrationTests, CannotUpdateAccountWithoutSignature)
{
  // Given
  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  EXPECT_THROW(
    const TransactionReceipt txReceipt =
      AccountUpdateTransaction().setAccountId(accountId).execute(getTestClient()).getReceipt(getTestClient()),
    ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountUpdateTransactionIntegrationTests, CannotUpdateAccountWithoutAccountId)
{
  // Given
  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = AccountUpdateTransaction()
                                                      .freezeWith(&getTestClient())
                                                      .sign(privateKey)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // ACCOUNT_ID_DOES_NOT_EXIST

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountUpdateTransactionIntegrationTests, ExecuteWithOnlyAccountId)
{
  // Given
  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  EXPECT_NO_THROW(const TransactionReceipt txReceipt = AccountUpdateTransaction()
                                                         .setAccountId(accountId)
                                                         .freezeWith(&getTestClient())
                                                         .sign(privateKey)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey)
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountUpdateTransactionIntegrationTests, InvalidAutoRenewPeriod)
{
  // Given
  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::chrono::system_clock::duration invalidAutoRenewPeriod = std::chrono::seconds(777600000);

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  EXPECT_THROW(const TransactionResponse txResponse = AccountUpdateTransaction()
                                                        .setAccountId(accountId)
                                                        .setAutoRenewPeriod(invalidAutoRenewPeriod)
                                                        .freezeWith(&getTestClient())
                                                        .sign(privateKey)
                                                        .execute(getTestClient()),
               PrecheckStatusException); // AUTORENEW_DURATION_NOT_IN_RANGE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey)
                    .execute(getTestClient()));
}
