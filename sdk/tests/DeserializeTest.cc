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
#include "AccountId.h"
#include "ExchangeRate.h"
#include "ExchangeRateSet.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"

#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <proto/exchange_rate.pb.h>
#include <proto/transaction_receipt.pb.h>
#include <proto/transaction_record.pb.h>
#include <proto/transaction_response.pb.h>

using namespace Hedera;

class DeserializeTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const int32_t  getTestCents() const { return cents; }
  [[nodiscard]] inline const int32_t  getTestHbar() const { return hbar; }
  [[nodiscard]] inline const uint64_t getTestSeconds() const { return seconds; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const AccountId& getTestAccountIdFrom() const { return mAccountIdFrom; }
  [[nodiscard]] inline const AccountId& getTestAccountIdTo() const { return mAccountIdTo; }

private:
  const uint64_t mShardNum = 1;
  const uint64_t mRealmNum = 2;
  const uint64_t mContractNum = 3;
  const int32_t cents = 2;
  const int32_t hbar = 1;
  const uint64_t seconds = 100ULL;
  const AccountId mAccountId = AccountId(0ULL, 0ULL, 10ULL);
  const AccountId mAccountIdFrom = AccountId(4ULL);
  const AccountId mAccountIdTo = AccountId(3ULL);
};

TEST_F(DeserializeTest, DeserializeExchangeRateFromProtobufTest)
{
  // Given
  const int32_t testCents = getTestCents();
  const int32_t testHbar = getTestHbar();
  const uint64_t testSeconds = getTestSeconds();

  auto testProtoExchangeRate = std::make_unique<proto::ExchangeRate>();
  proto::TimestampSeconds *testProtoExchangeRateSecs = testProtoExchangeRate->mutable_expirationtime();

  testProtoExchangeRate->set_centequiv(testCents);
  testProtoExchangeRate->set_hbarequiv(testHbar);
  testProtoExchangeRateSecs->set_seconds(testSeconds);

  // When
  ExchangeRate exchangeRate = ExchangeRate::fromProtobuf(*testProtoExchangeRate);
  
  // Then
  EXPECT_EQ(exchangeRate.getCurrentExchangeRate(), testCents / testHbar);
  EXPECT_TRUE(exchangeRate.getExpirationTime().has_value());
  EXPECT_EQ(exchangeRate.getExpirationTime().value().time_since_epoch().count(),
            internal::TimestampConverter::fromProtobuf(*testProtoExchangeRateSecs).time_since_epoch().count());
}

TEST_F(DeserializeTest, DeserializeExchangeRateSetFromProtobufTest)
{
  // Given
  const int32_t testCents = getTestCents();
  const int32_t testHbar = getTestHbar();
  const uint64_t testSeconds = getTestSeconds();

  auto testProtoExchangeRate = std::make_unique<proto::ExchangeRate>();
  testProtoExchangeRate->set_centequiv(testCents * testCents);
  testProtoExchangeRate->set_hbarequiv(testHbar * testHbar);

  proto::ExchangeRateSet testProtoExchangeRateSet;
  testProtoExchangeRateSet.set_allocated_nextrate(testProtoExchangeRate.release());

  // When
  ExchangeRateSet exchangeRateSet = ExchangeRateSet::fromProtobuf(testProtoExchangeRateSet);
  
  // Then
  EXPECT_FALSE(exchangeRateSet.getCurrentExchangeRate().has_value());
  EXPECT_TRUE(exchangeRateSet.getNextExchangeRate().has_value());
  EXPECT_EQ(exchangeRateSet.getNextExchangeRate().value().getCurrentExchangeRate(), testCents * testCents / testHbar * testHbar);
}

TEST_F(DeserializeTest, DeserializeTransactionReceiptFromProtobufTest)
{
  // Given
  const proto::ResponseCodeEnum testResponseStatus = proto::ResponseCodeEnum::SUCCESS;
  proto::AccountID *testProtoAccountId = getTestAccountId().toProtobuf().release();
  proto::TransactionReceipt testProtoTxReceipt;
  testProtoTxReceipt.set_status(testResponseStatus);
  testProtoTxReceipt.set_allocated_accountid(testProtoAccountId);

  const int32_t value = 6;
  const int32_t secs = 100;

  proto::ExchangeRateSet *protoExRateSet = testProtoTxReceipt.mutable_exchangerate();
  protoExRateSet->mutable_currentrate()->set_hbarequiv(value);
  protoExRateSet->mutable_currentrate()->set_centequiv(value);
  protoExRateSet->mutable_currentrate()->mutable_expirationtime()->set_seconds(secs);
  protoExRateSet->mutable_nextrate()->set_hbarequiv(value);
  protoExRateSet->mutable_nextrate()->set_centequiv(value);
  protoExRateSet->mutable_nextrate()->mutable_expirationtime()->set_seconds(secs);

  // When
  TransactionReceipt txRx = TransactionReceipt::fromProtobuf(testProtoTxReceipt);
  
  // Then
  EXPECT_EQ(txRx.getStatus(), Status::SUCCESS);
  EXPECT_EQ(txRx.getAccountId(), getTestAccountId());
  EXPECT_TRUE(txRx.getExchangeRates().has_value());
  EXPECT_TRUE(txRx.getExchangeRates()->getCurrentExchangeRate().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getCurrentExchangeRate()->getCurrentExchangeRate(), value / value);
  EXPECT_TRUE(txRx.getExchangeRates()->getCurrentExchangeRate()->getExpirationTime().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getCurrentExchangeRate()->getExpirationTime(),
            std::chrono::system_clock::time_point(std::chrono::seconds(secs)));
  EXPECT_TRUE(txRx.getExchangeRates()->getNextExchangeRate().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getNextExchangeRate()->getCurrentExchangeRate(), value / value);
  EXPECT_TRUE(txRx.getExchangeRates()->getNextExchangeRate()->getExpirationTime().has_value());
  EXPECT_EQ(txRx.getExchangeRates()->getNextExchangeRate()->getExpirationTime(),
            std::chrono::system_clock::time_point(std::chrono::seconds(secs)));
}

TEST_F(DeserializeTest, DeserializeTransactionRecordFromProtobufTest)
{
  // Given
  const auto testAccountIdTo = getTestAccountIdTo();
  const auto testAccountIdFrom = getTestAccountIdFrom();
  const auto testTransactionIdFrom = TransactionId::generate(testAccountIdFrom).toProtobuf().release();
  const int64_t testTransferAmount = 10LL;
  const std::string testTxHash = "txHash";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const std::string testTxMemo = "txMemo";
  const uint64_t testTxFee = 10ULL;
  proto::TransactionRecord testProtoTransactionRecord;
  testProtoTransactionRecord.mutable_receipt()->set_allocated_accountid(testAccountIdFrom.toProtobuf().release());
  testProtoTransactionRecord.set_allocated_transactionhash(new std::string(testTxHash));
  testProtoTransactionRecord.set_allocated_consensustimestamp(internal::TimestampConverter::toProtobuf(now));
  testProtoTransactionRecord.set_allocated_transactionid(testTransactionIdFrom);
  testProtoTransactionRecord.set_allocated_memo(new std::string(testTxMemo));
  testProtoTransactionRecord.set_transactionfee(testTxFee);
  
  proto::AccountAmount *testProtoAccountAmount = testProtoTransactionRecord.mutable_transferlist()->add_accountamounts();
  testProtoAccountAmount->set_allocated_accountid(testAccountIdFrom.toProtobuf().release());
  testProtoAccountAmount->set_amount(-testTransferAmount);
  
  testProtoAccountAmount = testProtoTransactionRecord.mutable_transferlist()->add_accountamounts();
  testProtoAccountAmount->set_allocated_accountid(testAccountIdTo.toProtobuf().release());
  testProtoAccountAmount->set_amount(testTransferAmount);

  // When
  TransactionRecord txRecord = TransactionRecord::fromProtobuf(testProtoTransactionRecord);
  
  // Then
  EXPECT_TRUE(txRecord.getReceipt());
  EXPECT_TRUE(txRecord.getReceipt()->getAccountId());
  EXPECT_EQ(*txRecord.getReceipt()->getAccountId(), testAccountIdFrom);
  EXPECT_EQ(txRecord.getTransactionHash(), testTxHash);
  EXPECT_TRUE(txRecord.getConsensusTimestamp());
  EXPECT_EQ(txRecord.getConsensusTimestamp()->time_since_epoch().count(), now.time_since_epoch().count());
  EXPECT_TRUE(txRecord.getTransactionId());
  EXPECT_EQ(txRecord.getTransactionId()->getAccountId(), testAccountIdFrom);
  // EXPECT_GE(txRecord.getTransactionId().value().getValidTransactionTime(), now);
  EXPECT_EQ(txRecord.getTransactionMemo(), testTxMemo);
  EXPECT_EQ(txRecord.getTransactionFee(), testTxFee);
  EXPECT_FALSE(txRecord.getTransferList().empty());
  EXPECT_EQ(txRecord.getTransferList().at(0).getAccountId(), testAccountIdFrom);
  EXPECT_EQ(txRecord.getTransferList().at(0).getAmount().toTinybars(), -testTransferAmount);
  EXPECT_EQ(txRecord.getTransferList().at(1).getAccountId(), testAccountIdTo);
  EXPECT_EQ(txRecord.getTransferList().at(1).getAmount().toTinybars(), testTransferAmount);
}

TEST_F(DeserializeTest, DeserializeTransactionResponseFromProtobufTest)
{
  // Given
  const uint64_t testCost = 10ULL;
  const proto::ResponseCodeEnum testProtoResponseCode = proto::ResponseCodeEnum::AUTHORIZATION_FAILED;
  proto::TransactionResponse testProtoTransactionResponse;
  testProtoTransactionResponse.set_cost(testCost);
  testProtoTransactionResponse.set_nodetransactionprecheckcode(testProtoResponseCode);

  // When
  TransactionResponse txResponse = TransactionResponse::fromProtobuf(testProtoTransactionResponse);
  
  // Then
  EXPECT_EQ(txResponse.getCost(), testCost);
  EXPECT_FALSE(txResponse.getValidateStatus());
}