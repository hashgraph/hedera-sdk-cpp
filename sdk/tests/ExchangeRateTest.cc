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
};

TEST_F(ExchangeRateTest, ConstructExchangeRateAndSet)
{
  ExchangeRate rate;
  EXPECT_FALSE(rate.getExpirationTime().has_value());

  ExchangeRateSet set;
  EXPECT_FALSE(set.getCurrentExchangeRate().has_value());
  EXPECT_FALSE(set.getNextExchangeRate().has_value());
}

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
