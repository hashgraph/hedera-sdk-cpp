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
#include "ContractDeleteTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ContractId.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/contract_delete.pb.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class ContractDeleteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const AccountId& getTestTransferAccountId() const { return mTestTransferAccountId; }
  [[nodiscard]] inline const ContractId& getTestTransferContractId() const { return mTestTransferContractId; }

private:
  Client mClient;
  const ContractId mTestContractId = ContractId(1ULL);
  const AccountId mTestTransferAccountId = AccountId(2ULL);
  const ContractId mTestTransferContractId = ContractId(3ULL);
};

//-----
TEST_F(ContractDeleteTransactionTest, ConstructContractDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto bodyWithTransferAccountId = std::make_unique<proto::ContractDeleteTransactionBody>();
  auto bodyWithTransferContractId = std::make_unique<proto::ContractDeleteTransactionBody>();

  bodyWithTransferAccountId->set_allocated_contractid(getTestContractId().toProtobuf().release());
  bodyWithTransferContractId->set_allocated_contractid(getTestContractId().toProtobuf().release());

  bodyWithTransferAccountId->set_allocated_transferaccountid(getTestTransferAccountId().toProtobuf().release());
  bodyWithTransferContractId->set_allocated_transfercontractid(getTestTransferContractId().toProtobuf().release());

  proto::TransactionBody txBodyWithTransferAccountId;
  proto::TransactionBody txBodyWithTransferContractId;
  txBodyWithTransferAccountId.set_allocated_contractdeleteinstance(bodyWithTransferAccountId.release());
  txBodyWithTransferContractId.set_allocated_contractdeleteinstance(bodyWithTransferContractId.release());

  // When
  const ContractDeleteTransaction contractDeleteTransactionWithTransferAccountId(txBodyWithTransferAccountId);
  const ContractDeleteTransaction contractDeleteTransactionWithTransferContractId(txBodyWithTransferContractId);

  // Then
  EXPECT_EQ(contractDeleteTransactionWithTransferAccountId.getContractId(), getTestContractId());
  ASSERT_TRUE(contractDeleteTransactionWithTransferAccountId.getTransferAccountId().has_value());
  EXPECT_EQ(contractDeleteTransactionWithTransferAccountId.getTransferAccountId(), getTestTransferAccountId());
  EXPECT_FALSE(contractDeleteTransactionWithTransferAccountId.getTransferContractId().has_value());

  EXPECT_EQ(contractDeleteTransactionWithTransferContractId.getContractId(), getTestContractId());
  EXPECT_FALSE(contractDeleteTransactionWithTransferContractId.getTransferAccountId().has_value());
  ASSERT_TRUE(contractDeleteTransactionWithTransferContractId.getTransferContractId().has_value());
  EXPECT_EQ(contractDeleteTransactionWithTransferContractId.getTransferContractId(), getTestTransferContractId());
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetContractId)
{
  // Given
  ContractDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  EXPECT_EQ(transaction.getContractId(), getTestContractId());
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetContractIdFrozen)
{
  // Given
  ContractDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setContractId(getTestContractId()), IllegalStateException);
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetTransferAccountId)
{
  // Given
  ContractDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTransferAccountId(getTestTransferAccountId()));

  // Then
  EXPECT_EQ(transaction.getTransferAccountId(), getTestTransferAccountId());
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetTransferAccountIdFrozen)
{
  // Given
  ContractDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setTransferAccountId(getTestTransferAccountId()), IllegalStateException);
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetTransferContractId)
{
  // Given
  ContractDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTransferContractId(getTestTransferContractId()));

  // Then
  EXPECT_EQ(transaction.getTransferContractId(), getTestTransferContractId());
}

//-----
TEST_F(ContractDeleteTransactionTest, GetSetTransferContractIdFrozen)
{
  // Given
  ContractDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setTransferContractId(getTestTransferContractId()), IllegalStateException);
}

//-----
TEST_F(ContractDeleteTransactionTest, ResetTransferAccountIdWhenSettingTransferContractId)
{
  // Given
  ContractDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setTransferAccountId(getTestTransferAccountId()));

  // When
  EXPECT_NO_THROW(transaction.setTransferContractId(getTestTransferContractId()));

  // Then
  EXPECT_FALSE(transaction.getTransferAccountId().has_value());
}

//-----
TEST_F(ContractDeleteTransactionTest, ResetTransferContractIdWhenSettingTransferAccountId)
{
  // Given
  ContractDeleteTransaction transaction;
  ASSERT_NO_THROW(transaction.setTransferContractId(getTestTransferContractId()));

  // When
  EXPECT_NO_THROW(transaction.setTransferAccountId(getTestTransferAccountId()));

  // Then
  EXPECT_FALSE(transaction.getTransferContractId().has_value());
}
