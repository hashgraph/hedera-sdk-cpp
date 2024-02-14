/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountBalanceQueryIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountBalanceQueryIntegrationTests, AccountId)
{
  // Given
  AccountBalance accountBalance;

  // When
  EXPECT_NO_THROW(accountBalance = AccountBalanceQuery().setAccountId(AccountId(1023ULL)).execute(getTestClient()));

  // Then
  EXPECT_EQ(accountBalance.mBalance, Hbar(10000LL));
}

//-----
TEST_F(AccountBalanceQueryIntegrationTests, Blank)
{
  // Given
  AccountBalanceQuery accountBalanceQuery;

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTests, BadAccountId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setAccountId(AccountId());

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//------
TEST_F(AccountBalanceQueryIntegrationTests, ValidButNonExistantAccountId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setAccountId(AccountId(1000000ULL));

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTests, ContractId)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::Contract]";
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ getTestClient().getOperatorPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setGas(1000000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .setBytecodeFileId(fileId)
                      .setMemo(memo)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());
  AccountBalance accountBalance;

  // When
  EXPECT_NO_THROW(accountBalance = AccountBalanceQuery().setContractId(contractId).execute(getTestClient()));

  // Then
  EXPECT_EQ(accountBalance.mBalance, Hbar(0LL));

  // Clean up
  ASSERT_NO_THROW(ContractDeleteTransaction()
                    .setContractId(contractId)
                    .setTransferAccountId(AccountId(2ULL)) // Local node operator account
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountBalanceQueryIntegrationTests, BadContractId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setContractId(ContractId());

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_CONTRACT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTests, ValidButNonExistantContractId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setContractId(ContractId(1ULL));

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_CONTRACT_ID
}
