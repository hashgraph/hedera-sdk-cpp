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
#include "ExchangeRate.h"
#include "ExchangeRateSet.h"
#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/exchange_rate.pb.h>

using namespace Hedera;

class ExchangeRateTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int32_t  getTestCents() const { return mCents; }
  [[nodiscard]] inline const int32_t  getTestHbar() const { return mHbar; }
  [[nodiscard]] inline const uint64_t getTestSeconds() const { return mSeconds; }

private:
  const int32_t mCents = 2;
  const int32_t mHbar = 1;
  const uint64_t mSeconds = 100ULL;
};

TEST_F(ExchangeRateTest, ConstructExchangeRateAndSet)
{
  ExchangeRate rate;
  EXPECT_FALSE(rate.getExpirationTime().has_value());

  ExchangeRateSet set;
  EXPECT_FALSE(set.getCurrentExchangeRate().has_value());
  EXPECT_FALSE(set.getNextExchangeRate().has_value());
}

// Tests deserialization of Hedera::ExchangeRate -> proto::ExchangeRate.
TEST_F(ExchangeRateTest, DeserializeExchangeRateFromProtobufTest)
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

// Tests serialization & deserialization of Hedera::ExchangeRate -> proto::ExchangeRate -> Hedera::ExchangeRate.
TEST_F(ExchangeRateTest, ProtobufExchangeRate)
{
  auto protoRate = std::make_unique<proto::ExchangeRate>();
  proto::TimestampSeconds* protoRateSecs = protoRate->mutable_expirationtime();

  const uint64_t seconds = 100ULL;
  const int32_t cents = 2;
  const int32_t hbar = 1;

  protoRateSecs->set_seconds(seconds);
  protoRate->set_centequiv(cents);
  protoRate->set_hbarequiv(hbar);

  ExchangeRate rate = ExchangeRate::fromProtobuf(*protoRate);
  EXPECT_EQ(rate.getCurrentExchangeRate(), cents / hbar);
  EXPECT_TRUE(rate.getExpirationTime().has_value());
  EXPECT_EQ(rate.getExpirationTime().value().time_since_epoch().count(),
            internal::TimestampConverter::fromProtobuf(*protoRateSecs).time_since_epoch().count());

  protoRateSecs->set_seconds(seconds + seconds);
  protoRate->set_centequiv(cents * cents);
  protoRate->set_hbarequiv(hbar * hbar);

  proto::ExchangeRateSet protoSet;
  protoSet.set_allocated_nextrate(protoRate.release());

  ExchangeRateSet set = ExchangeRateSet::fromProtobuf(protoSet);
  EXPECT_FALSE(set.getCurrentExchangeRate().has_value());
  EXPECT_TRUE(set.getNextExchangeRate().has_value());
  EXPECT_EQ(set.getNextExchangeRate().value().getCurrentExchangeRate(), cents * cents / hbar * hbar);
  EXPECT_TRUE(set.getNextExchangeRate().has_value());
  EXPECT_EQ(set.getNextExchangeRate().value().getExpirationTime().value().time_since_epoch().count(),
            internal::TimestampConverter::fromProtobuf(*protoRateSecs).time_since_epoch().count());

  protoSet.set_allocated_currentrate(protoSet.release_nextrate());

  set = ExchangeRateSet::fromProtobuf(protoSet);
  EXPECT_TRUE(set.getCurrentExchangeRate().has_value());
  EXPECT_EQ(set.getCurrentExchangeRate().value().getCurrentExchangeRate(), cents * cents / hbar * hbar);
  EXPECT_TRUE(set.getCurrentExchangeRate().has_value());
  EXPECT_EQ(set.getCurrentExchangeRate().value().getExpirationTime().value().time_since_epoch().count(),
            internal::TimestampConverter::fromProtobuf(*protoRateSecs).time_since_epoch().count());
  EXPECT_FALSE(set.getNextExchangeRate().has_value());
}
