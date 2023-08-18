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
#include "AccountAllowanceApproveTransaction.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "BaseIntegrationTest.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "impl/HexConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_record.pb.h>

using namespace Hedera;

class TransferTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TransferTransactionIntegrationTest, ExecuteTransferTransaction)
{
  // Given
  const Hbar amount(1LL);

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = TransferTransaction()
                                 .addHbarTransfer(AccountId(2ULL), amount.negated())
                                 .addHbarTransfer(AccountId(3ULL), amount)
                                 .execute(getTestClient()));

  // Then
  TransactionRecord txRecord;
  ASSERT_NO_THROW(txRecord = txResponse.getRecord(getTestClient()));
  EXPECT_EQ(txRecord.mHbarTransferList.size(), 2);
}

//-----
TEST_F(TransferTransactionIntegrationTest, TransferNothing)
{
  // Given / When / Then
  EXPECT_NO_THROW(const TransactionReceipt txReceipt =
                    TransferTransaction().execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TransferTransactionIntegrationTest, TransferOutOfNonOperatorAccount)
{
  // Given
  const std::unique_ptr<PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const Hbar amount(1LL);
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(privateKey->getPublicKey().get())
                                .setInitialBalance(Hbar(10LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = TransferTransaction()
                               .addHbarTransfer(AccountId(2ULL), amount)
                               .addHbarTransfer(accountId, amount.negated())
                               .freezeWith(&getTestClient())
                               .sign(privateKey.get())
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_EQ(txRecord.mHbarTransferList.size(), 2);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(TransferTransactionIntegrationTest, CanTransferHbarWithAliasID)
{
  // Given
  const std::unique_ptr<PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const Hbar amount(1LL);
  const EvmAddress evmAddress =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(privateKey->getPublicKey())->toEvmAddress();
  const AccountId aliasId(evmAddress);

  // When
  EXPECT_NO_THROW(const TransactionReceipt txReceipt = TransferTransaction()
                                                         .addHbarTransfer(AccountId(2ULL), amount.negated())
                                                         .addHbarTransfer(aliasId, amount)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  // Then
  AccountInfo accountInfo;
  ASSERT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(aliasId).execute(getTestClient()));
  EXPECT_EQ(internal::HexConverter::hexToBytes(accountInfo.mContractAccountId), evmAddress.toBytes());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(aliasId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(privateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(TransferTransactionIntegrationTest, CanSpendHbarAllowance)
{
  // Given
  const std::unique_ptr<PrivateKey> allowerKey = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<PrivateKey> alloweeKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const Hbar balance(10LL);
  const Hbar amount(1LL);
  AccountId allowerId;
  AccountId alloweeId;
  ASSERT_NO_THROW(allowerId = AccountCreateTransaction()
                                .setKey(allowerKey->getPublicKey().get())
                                .setInitialBalance(balance)
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());
  ASSERT_NO_THROW(alloweeId = AccountCreateTransaction()
                                .setKey(alloweeKey->getPublicKey().get())
                                .setInitialBalance(balance)
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = AccountAllowanceApproveTransaction()
                                                         .approveHbarAllowance(allowerId, alloweeId, amount)
                                                         .freezeWith(&getTestClient())
                                                         .sign(allowerKey.get())
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = TransferTransaction()
                               .addHbarTransfer(AccountId(2ULL), amount)
                               .addApprovedHbarTransfer(allowerId, amount.negated())
                               .setTransactionId(TransactionId::generate(alloweeId))
                               .freezeWith(&getTestClient())
                               .sign(alloweeKey.get())
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  bool transferFound = false;
  for (const auto& transfer : txRecord.mHbarTransferList)
  {
    if (transfer.getAccountId() == allowerId && transfer.getAmount() == amount.negated())
    {
      transferFound = true;
      break;
    }
  }

  EXPECT_TRUE(transferFound);

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(allowerId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(allowerKey.get())
                    .execute(getTestClient()));
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(alloweeId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(alloweeKey.get())
                    .execute(getTestClient()));
}
