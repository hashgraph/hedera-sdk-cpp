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
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "Defaults.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hiero;

class ContractInfoQueryIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractInfoQueryIntegrationTests, ExecuteContractInfoQuery)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::Contract]";
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
                      .setMemo(memo)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When
  ContractInfo contractInfo;
  EXPECT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));

  // Then
  EXPECT_EQ(contractInfo.mContractId, contractId);
  EXPECT_EQ(contractInfo.mAccountId.toString(), contractId.toString());
  EXPECT_EQ(contractInfo.mAdminKey->toBytes(), operatorKey->getPublicKey()->toBytes());
  EXPECT_GT(contractInfo.mExpirationTime, std::chrono::system_clock::now());
  EXPECT_EQ(contractInfo.mAutoRenewPeriod, DEFAULT_AUTO_RENEW_PERIOD);
  EXPECT_EQ(contractInfo.mStorage, 128ULL);
  EXPECT_EQ(contractInfo.mMemo, memo);

  // Clean up
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(contractId)
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractInfoQueryIntegrationTests, CanQueryContractInfoWhenAdminKeyIsNull)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::Contract]";
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
                      .setGas(1000000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hiero.").toBytes())
                      .setBytecodeFileId(fileId)
                      .setMemo(memo)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When
  ContractInfo contractInfo;
  EXPECT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));

  // Then
  EXPECT_EQ(contractInfo.mContractId, contractId);
  EXPECT_EQ(contractInfo.mAccountId.toString(), contractId.toString());
  EXPECT_GT(contractInfo.mExpirationTime, std::chrono::system_clock::now());
  EXPECT_EQ(contractInfo.mAutoRenewPeriod, DEFAULT_AUTO_RENEW_PERIOD);
  EXPECT_EQ(contractInfo.mStorage, 128ULL);
  EXPECT_EQ(contractInfo.mMemo, memo);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractInfoQueryIntegrationTests, CannotQueryContractInfoWhenContractIDIsNotSet)
{
  // Given / When / Then
  EXPECT_THROW(const ContractInfo contractInfo = ContractInfoQuery().execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}
