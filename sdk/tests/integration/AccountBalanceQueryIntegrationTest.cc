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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountBalanceQueryIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(AccountBalanceQueryIntegrationTest, AccountId)
{
  // Given
  AccountBalance accountBalance;

  // When
  EXPECT_NO_THROW(accountBalance = AccountBalanceQuery().setAccountId(AccountId(1023ULL)).execute(getTestClient()));

  // Then
  EXPECT_EQ(accountBalance.getBalance(), Hbar(10000LL));
}

//-----
TEST_F(AccountBalanceQueryIntegrationTest, Blank)
{
  // Given
  AccountBalanceQuery accountBalanceQuery;

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTest, BadAccountId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setAccountId(AccountId());

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//------
TEST_F(AccountBalanceQueryIntegrationTest, ValidButNonExistantAccountId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setAccountId(AccountId(1000000ULL));

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_ACCOUNT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTest, ContractId)
{
  // Given
  ContractId contractId;
  ASSERT_NO_THROW(contractId = ContractCreateTransaction()
                                 .setBytecode({})
                                 .setGas(500000ULL)
                                 .execute(getTestClient())
                                 .getReceipt(getTestClient())
                                 .mContractId.value());
  AccountBalance accountBalance;

  // When
  EXPECT_NO_THROW(accountBalance = AccountBalanceQuery().setContractId(contractId).execute(getTestClient()));

  // Then
  EXPECT_EQ(accountBalance.getBalance(), Hbar(0LL));

  // Clean up
  ASSERT_NO_THROW(ContractDeleteTransaction()
                    .setContractId(contractId)
                    .setTransferAccountId(AccountId(2ULL)) // Local node operator account
                    .execute(getTestClient()));
}

//-----
TEST_F(AccountBalanceQueryIntegrationTest, BadContractId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setContractId(ContractId());

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_CONTRACT_ID
}

//-----
TEST_F(AccountBalanceQueryIntegrationTest, ValidButNonExistantContractId)
{
  // Given
  AccountBalanceQuery accountBalanceQuery = AccountBalanceQuery().setContractId(ContractId(1ULL));

  // When / Then
  EXPECT_THROW(accountBalanceQuery.execute(getTestClient()), PrecheckStatusException); // INVALID_CONTRACT_ID
}
