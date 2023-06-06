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
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Utilities.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <proto/transaction_body.pb.h>

using json = nlohmann::json;
using namespace Hedera;

class TransactionRecordIntegrationTest : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }

private:
  const AccountId mTestAccountId = AccountId::fromString("0.0.1023");
};

//-----
TEST_F(TransactionRecordIntegrationTest, ExecuteEmptyAccountCreateTransaction)
{
  // Given / When / Then
  TransactionRecord txRecord;
  EXPECT_THROW(txRecord = AccountCreateTransaction().execute(getTestClient()).getRecord(getTestClient()),
               Hedera::PrecheckStatusException);
}

//-----
TEST_F(TransactionRecordIntegrationTest, ExecuteAccountCreateTransactionAndCheckTransactionRecord)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const auto testPublicKey = testPrivateKey->getPublicKey();
  const auto testMemo = "Test memo for TransactionRecord.";

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = AccountCreateTransaction()
                               .setKey(testPublicKey.get())
                               .setTransactionMemo(testMemo)
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_NO_THROW(txRecord.getReceipt()->validateStatus());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getAccountId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getFileId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getContractId().has_value());
  ASSERT_TRUE(txRecord.getReceipt()->getExchangeRates().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  AccountId accountId;
  ASSERT_NO_THROW(accountId = txRecord.getReceipt()->getAccountId().value());
  ASSERT_NO_THROW(AccountDeleteTransaction()
                    .setDeleteAccountId(accountId)
                    .setTransferAccountId(AccountId(2ULL))
                    .freezeWith(getTestClient())
                    .sign(testPrivateKey.get())
                    .execute(getTestClient()));
}

//-----
TEST_F(TransactionRecordIntegrationTest, ExecuteFileCreateTransactionAndCheckTransactionRecord)
{
  // Given
  const auto testMemo = "Test memo for TransactionRecord.";
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  // When
  TransactionRecord txRecord;
  EXPECT_NO_THROW(txRecord = FileCreateTransaction()
                               .setKeys({ operatorKey->getPublicKey().get() })
                               .setTransactionMemo(testMemo)
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_NO_THROW(txRecord.getReceipt()->validateStatus());
  EXPECT_EQ(txRecord.getTransactionMemo(), testMemo);
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getFileId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getAccountId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getContractId().has_value());
  ASSERT_TRUE(txRecord.getReceipt()->getExchangeRates().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  const FileId fileId = txRecord.getReceipt()->getFileId().value();
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TransactionRecordIntegrationTest, ExecuteContractCreateTransactionAndCheckTransactionRecord)
{
  // Given
  const std::vector<std::byte> contents = internal::Utilities::stringToByteVector(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  const auto testMemo = "Test memo for TransactionRecord.";
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey().get() })
                             .setContents(contents)
                             .setMaxTransactionFee(Hbar(2LL))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .getFileId()
                             .value());

  // When
  TransactionRecord txRecord;
  ASSERT_NO_THROW(txRecord = ContractCreateTransaction()
                               .setGas(500000ULL)
                               .setBytecodeFileId(fileId)
                               .setTransactionMemo(testMemo)
                               .setAdminKey(operatorKey->getPublicKey().get())
                               .setMaxTransactionFee(Hbar(16LL))
                               .execute(getTestClient())
                               .getRecord(getTestClient()));

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_EQ(txRecord.getReceipt()->getStatus(), Status::SUCCESS);
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getContractId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getFileId().has_value());
  EXPECT_FALSE(txRecord.getReceipt()->getAccountId().has_value());
  ASSERT_TRUE(txRecord.getReceipt()->getExchangeRates().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getExchangeRates().value().getCurrentExchangeRate().has_value());

  // Clean up
  const ContractId contractId = txRecord.getReceipt()->getContractId().value();
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                         .setContractId(contractId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}