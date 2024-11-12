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
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractByteCodeQuery.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hiero;

class ContractBytecodeQueryIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractBytecodeQueryIntegrationTests, ExecuteContractBytecodeQuery)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(1000000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hiero.").toBytes())
                      .setBytecodeFileId(fileId)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When
  ContractByteCode contractByteCode;
  EXPECT_NO_THROW(contractByteCode = ContractByteCodeQuery().setContractId(contractId).execute(getTestClient()));

  // Then
  EXPECT_EQ(contractByteCode.size(), 798);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                         .setContractId(contractId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractBytecodeQueryIntegrationTests, Blank)
{
  // Given / When / Then
  EXPECT_THROW(const ContractByteCode contractByteCode = ContractByteCodeQuery().execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}

//-----
TEST_F(ContractBytecodeQueryIntegrationTests, BadContractId)
{
  // Given / When / Then
  EXPECT_THROW(const ContractByteCode contractByteCode =
                 ContractByteCodeQuery().setContractId(ContractId()).execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}

//-----
TEST_F(ContractBytecodeQueryIntegrationTests, ValidButNonExistantContractId)
{
  // Given / When / Then
  EXPECT_THROW(const ContractByteCode contractByteCode =
                 ContractByteCodeQuery().setContractId(ContractId(100000ULL)).execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}
