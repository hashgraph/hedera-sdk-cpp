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
#include "AccountDeleteTransaction.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class AccountDeleteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestDeleteAccountId() const { return mDeleteAccountId; }
  [[nodiscard]] inline const AccountId& getTestTransferAccountId() const { return mTransferAccountId; }

private:
  Client mClient;
  const AccountId mDeleteAccountId = AccountId(1ULL);
  const AccountId mTransferAccountId = AccountId(2ULL);
};

//-----
TEST_F(AccountDeleteTransactionTest, ConstructAccountDeleteTransaction)
{
  AccountDeleteTransaction transaction;
  EXPECT_EQ(transaction.getDeleteAccountId(), AccountId());
  EXPECT_EQ(transaction.getTransferAccountId(), AccountId());
}

//-----
TEST_F(AccountDeleteTransactionTest, ConstructAccountCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoDeleteTransactionBody>();
  body->set_allocated_deleteaccountid(getTestDeleteAccountId().toProtobuf().release());
  body->set_allocated_transferaccountid(getTestTransferAccountId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(body.release());

  // When
  AccountDeleteTransaction accountDeleteTransaction(txBody);

  // Then
  EXPECT_EQ(accountDeleteTransaction.getDeleteAccountId(), getTestDeleteAccountId());
  EXPECT_EQ(accountDeleteTransaction.getTransferAccountId(), getTestTransferAccountId());
}

//-----
TEST_F(AccountDeleteTransactionTest, SetDeleteAccountId)
{
  AccountDeleteTransaction transaction;
  transaction.setDeleteAccountId(getTestDeleteAccountId());
  EXPECT_EQ(transaction.getDeleteAccountId(), getTestDeleteAccountId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setDeleteAccountId(getTestDeleteAccountId()), IllegalStateException);
}

//-----
TEST_F(AccountDeleteTransactionTest, SetTransferAccountId)
{
  AccountDeleteTransaction transaction;
  transaction.setTransferAccountId(getTestTransferAccountId());
  EXPECT_EQ(transaction.getTransferAccountId(), getTestTransferAccountId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setTransferAccountId(getTestTransferAccountId()), IllegalStateException);
}
