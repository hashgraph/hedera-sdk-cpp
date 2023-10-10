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
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>

using namespace Hedera;

class ContractCreateTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractCreateTransactionIntegrationTest, ExecuteContractCreateTransaction)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::ContractCreateTransaction]";
  const std::chrono::duration<double> autoRenewPeriod = std::chrono::hours(2016);
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  ContractId contractId;
  EXPECT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(100000ULL)
                      .setAutoRenewPeriod(autoRenewPeriod)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .setMemo(memo)
                      .setAutoRenewAccountId(AccountId(2ULL))
                      .setStakedAccountId(AccountId(2ULL))
                      .setDeclineStakingReward(true)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // Then
  ContractInfo contractInfo;
  ASSERT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));
  EXPECT_EQ(contractInfo.mContractId, contractId);
  EXPECT_EQ(contractInfo.mAccountId.toString(), contractId.toString());
  EXPECT_GT(contractInfo.mExpirationTime, std::chrono::system_clock::now());
  EXPECT_EQ(contractInfo.mAutoRenewPeriod, autoRenewPeriod);
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
TEST_F(ContractCreateTransactionIntegrationTest, CreateContractWithNoAdminKey)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::ContractCreateTransaction]";
  const std::chrono::duration<double> autoRenewPeriod = std::chrono::hours(2016);
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  ContractId contractId;
  EXPECT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setGas(100000ULL)
                      .setAutoRenewPeriod(autoRenewPeriod)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .setMemo(memo)
                      .setAutoRenewAccountId(AccountId(2ULL))
                      .setStakedAccountId(AccountId(2ULL))
                      .setDeclineStakingReward(true)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // Then
  ContractInfo contractInfo;
  ASSERT_NO_THROW(contractInfo = ContractInfoQuery().setContractId(contractId).execute(getTestClient()));
  EXPECT_EQ(contractInfo.mContractId, contractId);
  EXPECT_EQ(contractInfo.mAccountId.toString(), contractId.toString());
  EXPECT_GT(contractInfo.mExpirationTime, std::chrono::system_clock::now());
  EXPECT_EQ(contractInfo.mAutoRenewPeriod, autoRenewPeriod);
  EXPECT_EQ(contractInfo.mStorage, 128ULL);
  EXPECT_EQ(contractInfo.mMemo, memo);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractCreateTransactionIntegrationTest, CannotCreateContractWithNoGas)
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

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 ContractCreateTransaction()
                   .setAdminKey(operatorKey->getPublicKey())
                   .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                   .setBytecodeFileId(fileId)
                   .setMemo("[e2e::ContractCreateTransaction]")
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INSUFFICIENT_GAS

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractCreateTransactionIntegrationTest, CannotCreateContractWithNoConstructorParameters)
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

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = ContractCreateTransaction()
                                                      .setAdminKey(operatorKey->getPublicKey())
                                                      .setGas(100000ULL)
                                                      .setBytecodeFileId(fileId)
                                                      .setMemo("[e2e::ContractCreateTransaction]")
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // CONTRACT_REVERT_EXECUTED

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractCreateTransactionIntegrationTest, CannotCreateContractWithBytecodeFileId)
{
  // Given/ When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 ContractCreateTransaction()
                   .setGas(100000ULL)
                   .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                   .setMemo("[e2e::ContractCreateTransaction]")
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_FILE_ID
}
