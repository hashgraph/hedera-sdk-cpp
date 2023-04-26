/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "TransactionReceipt.h"
#include "Status.h"
#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/transaction_receipt.pb.h>

using namespace Hedera;

class TransactionReceiptTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }

private:
  const AccountId mTestAccountId = AccountId(1ULL);
  const FileId mTestFileId = FileId(2ULL);
  const ContractId mTestContractId = ContractId(3ULL);
};

//-----
TEST_F(TransactionReceiptTest, ConstructTransactionReceipt)
{
  // Given / When
  const TransactionReceipt transactionReceipt;

  // Then
  EXPECT_EQ(transactionReceipt.getStatus(), Status::UNKNOWN);
  EXPECT_FALSE(transactionReceipt.getAccountId().has_value());
  EXPECT_FALSE(transactionReceipt.getFileId().has_value());
  EXPECT_FALSE(transactionReceipt.getContractId().has_value());
  EXPECT_FALSE(transactionReceipt.getExchangeRates().has_value());
}

//-----
TEST_F(TransactionReceiptTest, ProtobufTransactionReceipt)
{
  // Given
  proto::TransactionReceipt protoTxReceipt;
  protoTxReceipt.set_status(proto::ResponseCodeEnum::SUCCESS);
  protoTxReceipt.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  protoTxReceipt.set_allocated_fileid(getTestFileId().toProtobuf().release());
  protoTxReceipt.set_allocated_contractid(getTestContractId().toProtobuf().release());

  const int32_t value = 6;
  const int32_t secs = 100;

  proto::ExchangeRateSet* protoExRateSet = protoTxReceipt.mutable_exchangerate();
  protoExRateSet->mutable_currentrate()->set_hbarequiv(value);
  protoExRateSet->mutable_currentrate()->set_centequiv(value);
  protoExRateSet->mutable_currentrate()->mutable_expirationtime()->set_seconds(secs);
  protoExRateSet->mutable_nextrate()->set_hbarequiv(value);
  protoExRateSet->mutable_nextrate()->set_centequiv(value);
  protoExRateSet->mutable_nextrate()->mutable_expirationtime()->set_seconds(secs);

  // When
  const TransactionReceipt txRx = TransactionReceipt::fromProtobuf(protoTxReceipt);

  // Then
  EXPECT_EQ(txRx.getStatus(), Status::SUCCESS);
  ASSERT_TRUE(txRx.getAccountId().has_value());
  EXPECT_EQ(txRx.getAccountId(), getTestAccountId());
  ASSERT_TRUE(txRx.getFileId().has_value());
  EXPECT_EQ(txRx.getFileId(), getTestFileId());
  ASSERT_TRUE(txRx.getContractId().has_value());
  EXPECT_EQ(txRx.getContractId(), getTestContractId());
  ASSERT_TRUE(txRx.getExchangeRates().has_value());
  ASSERT_TRUE(txRx.getExchangeRates()->getCurrentExchangeRate().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getCurrentExchangeRate()->getCurrentExchangeRate(), value / value);
  ASSERT_TRUE(txRx.getExchangeRates()->getCurrentExchangeRate()->getExpirationTime().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getCurrentExchangeRate()->getExpirationTime(),
            std::chrono::system_clock::time_point(std::chrono::seconds(secs)));
  EXPECT_TRUE(txRx.getExchangeRates()->getNextExchangeRate().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getNextExchangeRate()->getCurrentExchangeRate(), value / value);
  EXPECT_TRUE(txRx.getExchangeRates()->getNextExchangeRate()->getExpirationTime().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getNextExchangeRate()->getExpirationTime(),
            std::chrono::system_clock::time_point(std::chrono::seconds(secs)));
}
