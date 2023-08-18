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
#include "ContractExecuteTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <vector>

using namespace Hedera;

class ContractExecuteTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const uint64_t& getTestGas() const { return mTestGas; }
  [[nodiscard]] inline const Hbar& getTestPayableAmount() const { return mTestPayableAmount; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestFunctionParameters() const
  {
    return mTestFunctionParameters;
  }

private:
  Client mClient;
  const ContractId mTestContractId = ContractId(1ULL);
  const uint64_t mTestGas = 2ULL;
  const Hbar mTestPayableAmount = Hbar(3LL);
  const std::vector<std::byte> mTestFunctionParameters = { std::byte(0x04), std::byte(0x05), std::byte(0x06) };
};

//-----
TEST_F(ContractExecuteTransactionTest, ConstructContractExecuteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ContractCallTransactionBody>();
  body->set_allocated_contractid(getTestContractId().toProtobuf().release());
  body->set_gas(static_cast<int64_t>(getTestGas()));
  body->set_amount(getTestPayableAmount().toTinybars());
  body->set_functionparameters(internal::Utilities::byteVectorToString(getTestFunctionParameters()));

  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(body.release());

  // When
  const ContractExecuteTransaction contractExecuteTransaction(txBody);

  // Then
  EXPECT_EQ(contractExecuteTransaction.getContractId(), getTestContractId());
  EXPECT_EQ(contractExecuteTransaction.getGas(), getTestGas());
  EXPECT_EQ(contractExecuteTransaction.getPayableAmount(), getTestPayableAmount());
  EXPECT_EQ(contractExecuteTransaction.getFunctionParameters(), getTestFunctionParameters());
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetContractId)
{
  // Given
  ContractExecuteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setContractId(getTestContractId()));

  // Then
  EXPECT_EQ(transaction.getContractId(), getTestContractId());
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetContractIdFrozen)
{
  // Given
  ContractExecuteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setContractId(getTestContractId()), IllegalStateException);
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetGas)
{
  // Given
  ContractExecuteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setGas(getTestGas()));

  // Then
  EXPECT_EQ(transaction.getGas(), getTestGas());
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetGasFrozen)
{
  // Given
  ContractExecuteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setGas(getTestGas()), IllegalStateException);
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetPayableAmount)
{
  // Given
  ContractExecuteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setPayableAmount(getTestPayableAmount()));

  // Then
  EXPECT_EQ(transaction.getPayableAmount(), getTestPayableAmount());
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetPayableAmountFrozen)
{
  // Given
  ContractExecuteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setPayableAmount(getTestPayableAmount()), IllegalStateException);
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetFunctionParameters)
{
  // Given
  ContractExecuteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setFunctionParameters(getTestFunctionParameters()));

  // Then
  EXPECT_EQ(transaction.getFunctionParameters(), getTestFunctionParameters());
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetFunctionParametersFrozen)
{
  // Given
  ContractExecuteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setFunctionParameters(getTestFunctionParameters()), IllegalStateException);
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetFunctionName)
{
  // Given
  ContractExecuteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setFunction("functionName"));

  // Then
  EXPECT_EQ(transaction.getFunctionParameters().size(), 4);
}

//-----
TEST_F(ContractExecuteTransactionTest, GetSetFunctionNameFrozen)
{
  // Given
  ContractExecuteTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setFunction("functionName"), IllegalStateException);
}
