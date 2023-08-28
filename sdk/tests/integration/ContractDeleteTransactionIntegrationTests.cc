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
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <chrono>
#include <gtest/gtest.h>

using namespace Hedera;

class ContractDeleteTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractDeleteTransactionIntegrationTest, DeleteContractWithAdminKey)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecode({})
                      .setAdminKey(operatorKey->getPublicKey().get())
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When / Then
  EXPECT_NO_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                         .setContractId(contractId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}

//-----
TEST_F(ContractDeleteTransactionIntegrationTest, CannotDeleteContractWithNoAdminKey)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecode({})
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                      .setContractId(contractId)
                                                      .setTransferAccountId(AccountId(2ULL))
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // MODIFYING_IMMUTABLE_CONTRACT
}

//-----
TEST_F(ContractDeleteTransactionIntegrationTest, CannotDeleteContractWithNoContractId)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionResponse txResponse = ContractDeleteTransaction().execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}
