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
#include "AccountDeleteTransaction.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "Defaults.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/DurationConverter.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class AccountCreateTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountCreateTransactionIntegrationTest, ExecuteAccountCreateTransaction)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> testPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> testPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(testPrivateKey->getPublicKey());
  const EvmAddress testEvmAddress = testPublicKey->toEvmAddress();
  const Hbar testInitialBalance(1000LL, HbarUnit::TINYBAR());
  const std::chrono::duration<double> testAutoRenewPeriod = std::chrono::seconds(2592000);
  const std::string testMemo = "test account memo";
  const uint32_t testMaxAutomaticTokenAssociations = 4U;

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setKey(testPublicKey.get())
                                 .setInitialBalance(testInitialBalance)
                                 .setReceiverSignatureRequired(true)
                                 .setAutoRenewPeriod(testAutoRenewPeriod)
                                 .setAccountMemo(testMemo)
                                 .setMaxAutomaticTokenAssociations(testMaxAutomaticTokenAssociations)
                                 .setDeclineStakingReward(true)
                                 .setAlias(testEvmAddress)
                                 .freezeWith(getTestClient())
                                 .sign(testPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.mContractAccountId), testEvmAddress.toBytes());
  EXPECT_EQ(accountInfo.mKey->toBytes(), testPublicKey->toBytes());
  EXPECT_EQ(accountInfo.mBalance, testInitialBalance);
  EXPECT_EQ(accountInfo.mAutoRenewPeriod, testAutoRenewPeriod);
  EXPECT_EQ(accountInfo.mMemo, testMemo);
  EXPECT_EQ(accountInfo.mMaxAutomaticTokenAssociations, testMaxAutomaticTokenAssociations);
  EXPECT_TRUE(accountInfo.mStakingInfo.getDeclineReward());
  EXPECT_FALSE(accountInfo.mStakingInfo.getStakedAccountId().has_value());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, MutuallyExclusiveStakingIds)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> testPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> testPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(testPrivateKey->getPublicKey());
  const AccountId operatorAccountId(2ULL);
  const uint64_t nodeId = 0ULL;

  // When
  TransactionResponse txResponseStakedAccountId;
  TransactionResponse txResponseStakedNodeId;

  EXPECT_NO_THROW(txResponseStakedAccountId = AccountCreateTransaction()
                                                .setKey(testPublicKey.get())
                                                .setStakedAccountId(operatorAccountId)
                                                .freezeWith(getTestClient())
                                                .execute(getTestClient()));
  EXPECT_NO_THROW(txResponseStakedNodeId = AccountCreateTransaction()
                                             .setKey(testPublicKey.get())
                                             .setStakedNodeId(nodeId)
                                             .freezeWith(getTestClient())
                                             .execute(getTestClient()));

  // Then
  AccountId accountIdStakedAccountId;
  AccountId accountIdStakedNodeId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountIdStakedAccountId =
                    txResponseStakedAccountId.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountIdStakedNodeId = txResponseStakedNodeId.getReceipt(getTestClient()).getAccountId().value());

  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountIdStakedAccountId).execute(getTestClient()));
  EXPECT_EQ(accountInfo.mAccountId, accountIdStakedAccountId);
  EXPECT_EQ(accountInfo.mKey->toBytes(), testPublicKey->toBytes());
  ASSERT_TRUE(accountInfo.mStakingInfo.getStakedAccountId().has_value());
  EXPECT_EQ(accountInfo.mStakingInfo.getStakedAccountId(), operatorAccountId);
  EXPECT_FALSE(accountInfo.mStakingInfo.getStakedNodeId().has_value());

  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountIdStakedNodeId).execute(getTestClient()));
  EXPECT_EQ(accountInfo.mAccountId, accountIdStakedNodeId);
  EXPECT_EQ(accountInfo.mKey->toBytes(), testPublicKey->toBytes());
  EXPECT_FALSE(accountInfo.mStakingInfo.getStakedAccountId().has_value());
  ASSERT_TRUE(accountInfo.mStakingInfo.getStakedNodeId().has_value());
  EXPECT_EQ(accountInfo.mStakingInfo.getStakedNodeId(), nodeId);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountIdStakedAccountId)
                    .setTransferAccountId(operatorAccountId)
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountIdStakedNodeId)
                    .setTransferAccountId(operatorAccountId)
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, NoInitialBalance)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> testKey = ED25519PrivateKey::generatePrivateKey();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse =
                    AccountCreateTransaction().setKey(testKey->getPublicKey().get()).execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_FALSE(accountInfo.mIsDeleted);
  EXPECT_EQ(accountInfo.mKey->toBytes(), testKey->getPublicKey()->toBytes());
  EXPECT_EQ(accountInfo.mBalance, Hbar(0LL));
  EXPECT_EQ(accountInfo.mAutoRenewPeriod, DEFAULT_AUTO_RENEW_PERIOD);
  EXPECT_EQ(accountInfo.mProxyReceived, Hbar(0LL));

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(testKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, AliasFromAdminKey)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> adminPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(adminPrivateKey->getPublicKey());
  const EvmAddress evmAddress = adminPublicKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminPublicKey.get())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(
    txResponse = AccountCreateTransaction().setKey(adminPublicKey.get()).setAlias(evmAddress).execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.mContractAccountId), evmAddress.toBytes());
  EXPECT_EQ(accountInfo.mKey->toBytes(), adminPublicKey->toBytes());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminPrivateKey.get())
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, AliasFromAdminKeyWithReceiverSigRequired)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> adminKeyPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminKeyPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(adminKeyPrivateKey->getPublicKey());
  const EvmAddress evmAddress = adminKeyPublicKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminKeyPublicKey.get())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setReceiverSignatureRequired(true)
                                 .setKey(adminKeyPublicKey.get())
                                 .setAlias(evmAddress)
                                 .freezeWith(getTestClient())
                                 .sign(adminKeyPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.mContractAccountId), evmAddress.toBytes());
  EXPECT_EQ(accountInfo.mKey->toBytes(), adminKeyPublicKey->toBytes());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminKeyPrivateKey.get())
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminKeyPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, CannotCreateAliasFromAdminKeyWithReceiverSigRequiredWithoutSignature)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> adminKeyPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> adminKeyPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(adminKeyPrivateKey->getPublicKey());
  const EvmAddress evmAddress = adminKeyPublicKey->toEvmAddress();

  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminKeyPublicKey.get())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  // When
  EXPECT_THROW(const TransactionReceipt txReceipt = AccountCreateTransaction()
                                                      .setReceiverSignatureRequired(true)
                                                      .setKey(adminKeyPublicKey.get())
                                                      .setAlias(evmAddress)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminKeyPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest, AliasDifferentFromAdminKeyWithReceiverSigRequired)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminPrivateKey = ED25519PrivateKey::generatePrivateKey();
  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminPrivateKey->getPublicKey().get())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> aliasPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const EvmAddress alias =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(aliasPrivateKey->getPublicKey())->toEvmAddress();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountCreateTransaction()
                                 .setReceiverSignatureRequired(true)
                                 .setKey(adminPrivateKey->getPublicKey().get())
                                 .setAlias(alias)
                                 .freezeWith(getTestClient())
                                 .sign(adminPrivateKey.get())
                                 .sign(aliasPrivateKey.get())
                                 .execute(getTestClient()));

  // Then
  AccountId accountId;
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountId = txResponse.getReceipt(getTestClient()).getAccountId().value());
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(accountId).execute(getTestClient()));

  EXPECT_EQ(accountInfo.mAccountId, accountId);
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.mContractAccountId), alias.toBytes());
  EXPECT_EQ(accountInfo.mKey->toBytes(), adminPrivateKey->getPublicKey()->toBytes());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminPrivateKey.get())
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountCreateTransactionIntegrationTest,
       CannotCreateWithAliasDifferentFromAdminKeyWithReceiverSigRequiredWithoutSignature)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminPrivateKey = ED25519PrivateKey::generatePrivateKey();
  AccountId adminAccountId;
  ASSERT_NO_THROW(adminAccountId = AccountCreateTransaction()
                                     .setKey(adminPrivateKey->getPublicKey().get())
                                     .execute(getTestClient())
                                     .getReceipt(getTestClient())
                                     .getAccountId()
                                     .value());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> aliasPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const EvmAddress alias =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(aliasPrivateKey->getPublicKey())->toEvmAddress();

  // When
  EXPECT_THROW(const TransactionReceipt txReceipt = AccountCreateTransaction()
                                                      .setReceiverSignatureRequired(true)
                                                      .setKey(adminPrivateKey->getPublicKey().get())
                                                      .setAlias(alias)
                                                      .freezeWith(getTestClient())
                                                      .sign(aliasPrivateKey.get())
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(adminAccountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(adminPrivateKey.get())
                    .execute(getTestClient()));
}
