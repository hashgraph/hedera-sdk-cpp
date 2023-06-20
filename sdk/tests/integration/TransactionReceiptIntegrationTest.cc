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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "BaseIntegrationTest.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "TokenCreateTransaction.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TransactionReceiptIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TransactionReceiptIntegrationTest, ExecuteAccountCreateTransactionAndCheckTransactionReceipt)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  // When
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = AccountCreateTransaction()
                                .setKey(operatorKey->getPublicKey().get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_NO_THROW(txReceipt.validateStatus());
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txReceipt.getAccountId().has_value());
  EXPECT_FALSE(txReceipt.getFileId().has_value());
  EXPECT_FALSE(txReceipt.getContractId().has_value());
  ASSERT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(txReceipt.getAccountId().value())
                    .setTransferAccountId(AccountId(2ULL))
                    .execute(getTestClient()));
}

//-----
TEST_F(TransactionReceiptIntegrationTest, ExecuteFileCreateTransactionAndCheckTransactionReceipt)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  // When
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = FileCreateTransaction()
                                .setKeys({ operatorKey->getPublicKey().get() })
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txReceipt.getFileId().has_value());
  EXPECT_FALSE(txReceipt.getAccountId().has_value());
  EXPECT_FALSE(txReceipt.getContractId().has_value());
  ASSERT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  ASSERT_NO_THROW(txReceipt = FileDeleteTransaction()
                                .setFileId(txReceipt.getFileId().value())
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(TransactionReceiptIntegrationTest, ExecuteContractCreateTransactionAndCheckTransactionReceipt)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey().get() })
                             .setContents(getTestFileContent())
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .getFileId()
                             .value());

  // When
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = ContractCreateTransaction()
                                .setGas(500000ULL)
                                .setBytecodeFileId(fileId)
                                .setAdminKey(operatorKey->getPublicKey().get())
                                .setMaxTransactionFee(Hbar(16LL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txReceipt.getContractId().has_value());
  EXPECT_FALSE(txReceipt.getAccountId().has_value());
  EXPECT_FALSE(txReceipt.getFileId().has_value());
  ASSERT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(txReceipt.getContractId().value())
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TransactionReceiptIntegrationTest, ExecuteTokenCreateTransactionAndCheckTransactionReceipt)
{
  // Given
  std::unique_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TransactionResponse txResponse;
  ASSERT_NO_THROW(txResponse = TokenCreateTransaction()
                                 .setTokenName("test token name")
                                 .setTokenSymbol("test token symbol")
                                 .setTreasuryAccountId(AccountId(2ULL))
                                 .execute(getTestClient()));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt =
                    TransactionReceiptQuery().setTransactionId(txResponse.getTransactionId()).execute(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.getStatus(), Status::SUCCESS);
  EXPECT_FALSE(txReceipt.getAccountId().has_value());
  EXPECT_FALSE(txReceipt.getContractId().has_value());
  EXPECT_FALSE(txReceipt.getFileId().has_value());
  ASSERT_TRUE(txReceipt.getExchangeRates().has_value());
  EXPECT_TRUE(txReceipt.getExchangeRates().value().getCurrentExchangeRate().has_value());
  EXPECT_TRUE(txReceipt.getTokenId().has_value());

  // Clean up
  // TODO: TokenDeleteTransaction
}