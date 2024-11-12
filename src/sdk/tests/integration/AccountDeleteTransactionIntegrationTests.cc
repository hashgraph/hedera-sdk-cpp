/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>

using namespace Hiero;

class AccountDeleteTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountDeleteTransactionIntegrationTests, ExecuteAccountDeleteTransaction)
{
  // Given
  const std::shared_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(key->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = AccountDeleteTransaction()
                                 .setDeleteAccountId(accountId)
                                 .setTransferAccountId(AccountId(2ULL))
                                 .freezeWith(&getTestClient())
                                 .sign(key)
                                 .execute(getTestClient()));

  // Then
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = txResponse.getReceipt(getTestClient()));
}

//-----
TEST_F(AccountDeleteTransactionIntegrationTests, CannotDeleteInvalidAccountId)
{
  // Given / When / Then
  EXPECT_THROW(AccountDeleteTransaction().setTransferAccountId(AccountId(2ULL)).execute(getTestClient()),
               PrecheckStatusException);
}

//-----
TEST_F(AccountDeleteTransactionIntegrationTests, CannotDeleteAccountWithoutSignature)
{
  // Given
  const std::shared_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(key->getPublicKey())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  TransactionReceipt txReceipt;
  EXPECT_THROW(txReceipt = AccountDeleteTransaction()
                             .setDeleteAccountId(accountId)
                             .setTransferAccountId(AccountId(2ULL))
                             .execute(getTestClient())
                             .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(&getTestClient())
                    .sign(key)
                    .execute(getTestClient()));
}
