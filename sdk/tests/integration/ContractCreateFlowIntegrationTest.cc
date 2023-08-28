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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateFlow.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "ED25519PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/ReceiptStatusException.h"

#include <chrono>
#include <gtest/gtest.h>

using namespace Hedera;

class ContractCreateFlowIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractCreateFlowIntegrationTest, ExecuteContractCreateFlow)
{
  // Given / When
  TransactionResponse txResponse;
  ASSERT_NO_THROW(txResponse = ContractCreateFlow()
                                 .setBytecode(getTestSmartContractBytecode())
                                 .setAdminKey(getTestClient().getOperatorPublicKey().get())
                                 .setGas(100000ULL)
                                 .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera."))
                                 .execute(getTestClient()));

  // Then
  ContractId contractId;
  ContractInfo contractInfo;
  ASSERT_NO_THROW(contractId = txResponse.getReceipt(getTestClient()).mContractId.value());
  ASSERT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));
  EXPECT_EQ(contractInfo.mContractId, contractId);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    ContractDeleteTransaction()
                      .setContractId(contractId)
                      .setTransferAccountId(getTestClient().getOperatorAccountId().value())
                      .execute(getTestClient())
                      .getReceipt(getTestClient()));
}

//-----
TEST_F(ContractCreateFlowIntegrationTest, CannotCreateContractWithFlowWithoutSigning)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminKey = ED25519PrivateKey::generatePrivateKey();

  // When / Then
  EXPECT_THROW(const TransactionResponse txResponse =
                 ContractCreateFlow()
                   .setBytecode(getTestSmartContractBytecode())
                   .setAdminKey(adminKey.get())
                   .setGas(100000ULL)
                   .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera."))
                   .execute(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE
}

//-----
TEST_F(ContractCreateFlowIntegrationTest, ExecuteContractCreateFlowWithPrivateKey)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminKey = ED25519PrivateKey::generatePrivateKey();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = ContractCreateFlow()
                                 .setBytecode(getTestSmartContractBytecode())
                                 .setAdminKey(adminKey.get())
                                 .setGas(100000ULL)
                                 .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera."))
                                 .freezeWith(getTestClient())
                                 .sign(adminKey.get())
                                 .execute(getTestClient()));

  // Then
  ContractId contractId;
  ContractInfo contractInfo;
  ASSERT_NO_THROW(contractId = txResponse.getReceipt(getTestClient()).mContractId.value());
  ASSERT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));
  EXPECT_EQ(contractInfo.mContractId, contractId);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    ContractDeleteTransaction()
                      .setContractId(contractId)
                      .setTransferAccountId(getTestClient().getOperatorAccountId().value())
                      .freezeWith(&getTestClient())
                      .sign(adminKey.get())
                      .execute(getTestClient())
                      .getReceipt(getTestClient()));
}

//-----
TEST_F(ContractCreateFlowIntegrationTest, ExecuteContractCreateFlowWithPublicKeyAndTransactionSigner)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> adminKey = ED25519PrivateKey::generatePrivateKey();

  // When
  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse =
                    ContractCreateFlow()
                      .setBytecode(getTestSmartContractBytecode())
                      .setAdminKey(adminKey.get())
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera."))
                      .freezeWith(getTestClient())
                      .signWith(adminKey->getPublicKey(),
                                [&adminKey](const std::vector<std::byte>& bytes) { return adminKey->sign(bytes); })
                      .execute(getTestClient()));

  // Then
  ContractId contractId;
  ContractInfo contractInfo;
  ASSERT_NO_THROW(contractId = txResponse.getReceipt(getTestClient()).mContractId.value());
  ASSERT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));
  EXPECT_EQ(contractInfo.mContractId, contractId);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    ContractDeleteTransaction()
                      .setContractId(contractId)
                      .setTransferAccountId(getTestClient().getOperatorAccountId().value())
                      .freezeWith(&getTestClient())
                      .sign(adminKey.get())
                      .execute(getTestClient())
                      .getReceipt(getTestClient()));
}