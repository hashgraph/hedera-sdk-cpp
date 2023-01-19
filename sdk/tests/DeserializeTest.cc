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

#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <proto/exchange_rate.pb.h>
#include <proto/transaction_receipt.pb.h>

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

private:
  const uint64_t mShardNum = 1;
  const uint64_t mRealmNum = 2;
  const uint64_t mContractNum = 3;
  const int32_t cents = 2;
  const int32_t hbar = 1;
  const uint64_t seconds = 100ULL;
  const AccountId mAccountId = AccountId(0ULL, 0ULL, 10ULL);
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