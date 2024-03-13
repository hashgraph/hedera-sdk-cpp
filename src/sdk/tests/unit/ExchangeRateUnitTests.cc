/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "ExchangeRates.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/exchange_rate.pb.h>

using namespace Hedera;

class ExchangeRateUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline int32_t getTestCents() const { return mTestCents; }
  [[nodiscard]] inline int32_t getTestHbar() const { return mTestHbar; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }

private:
  const int32_t mTestCents = 2;
  const int32_t mTestHbar = 1;
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
};

//-----
TEST_F(ExchangeRateUnitTests, ExchangeRateConstructor)
{
  // Given / When
  const ExchangeRate exchangeRate(getTestHbar(), getTestCents(), getTestExpirationTime());

  // Then
  EXPECT_EQ(exchangeRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRate.mCents, getTestCents());
  EXPECT_EQ(exchangeRate.mExpirationTime, getTestExpirationTime());
}

//-----
TEST_F(ExchangeRateUnitTests, ExchangeRateFromProtobuf)
{
  // Given
  proto::ExchangeRate protoExchangeRate;
  protoExchangeRate.set_hbarequiv(getTestHbar());
  protoExchangeRate.set_centequiv(getTestCents());
  protoExchangeRate.set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(getTestExpirationTime()));

  // When
  const ExchangeRate exchangeRate = ExchangeRate::fromProtobuf(protoExchangeRate);

  // Then
  EXPECT_EQ(exchangeRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRate.mCents, getTestCents());
  EXPECT_EQ(std::chrono::duration_cast<std::chrono::seconds>(exchangeRate.mExpirationTime.time_since_epoch()),
            std::chrono::duration_cast<std::chrono::seconds>(getTestExpirationTime().time_since_epoch()));
  EXPECT_EQ(exchangeRate.mExchangeRateInCents, getTestCents() / getTestHbar());
}

//-----
TEST_F(ExchangeRateUnitTests, ExchangeRatesConstructor)
{
  // Given / When
  const ExchangeRate exchangeRate(getTestHbar(), getTestCents(), getTestExpirationTime());
  const ExchangeRates exchangeRates(exchangeRate, exchangeRate);

  // Then
  EXPECT_EQ(exchangeRates.mCurrentRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mCurrentRate.mCents, getTestCents());
  EXPECT_EQ(exchangeRates.mCurrentRate.mExpirationTime, getTestExpirationTime());
  EXPECT_EQ(exchangeRates.mNextRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mNextRate.mCents, getTestCents());
  EXPECT_EQ(exchangeRates.mNextRate.mExpirationTime, getTestExpirationTime());
}

//-----
TEST_F(ExchangeRateUnitTests, ExchangeRatesFromProtobuf)
{
  // Given
  proto::ExchangeRate protoExchangeRate;
  protoExchangeRate.set_hbarequiv(getTestHbar());
  protoExchangeRate.set_centequiv(getTestCents());
  protoExchangeRate.set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(getTestExpirationTime()));

  proto::ExchangeRateSet protoExchangeRateSet;
  *protoExchangeRateSet.mutable_currentrate() = protoExchangeRate;
  *protoExchangeRateSet.mutable_nextrate() = protoExchangeRate;

  // When
  const ExchangeRates exchangeRates = ExchangeRates::fromProtobuf(protoExchangeRateSet);

  // Then
  EXPECT_EQ(exchangeRates.mCurrentRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mCurrentRate.mCents, getTestCents());
  EXPECT_EQ(
    std::chrono::duration_cast<std::chrono::seconds>(exchangeRates.mCurrentRate.mExpirationTime.time_since_epoch()),
    std::chrono::duration_cast<std::chrono::seconds>(getTestExpirationTime().time_since_epoch()));
  EXPECT_EQ(exchangeRates.mCurrentRate.mExchangeRateInCents, getTestCents() / getTestHbar());
  EXPECT_EQ(exchangeRates.mNextRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mNextRate.mCents, getTestCents());
  EXPECT_EQ(
    std::chrono::duration_cast<std::chrono::seconds>(exchangeRates.mNextRate.mExpirationTime.time_since_epoch()),
    std::chrono::duration_cast<std::chrono::seconds>(getTestExpirationTime().time_since_epoch()));
  EXPECT_EQ(exchangeRates.mNextRate.mExchangeRateInCents, getTestCents() / getTestHbar());
}

//-----
TEST_F(ExchangeRateUnitTests, ExchangeRatesFromBytes)
{
  // Given
  proto::ExchangeRate protoExchangeRate;
  protoExchangeRate.set_hbarequiv(getTestHbar());
  protoExchangeRate.set_centequiv(getTestCents());
  protoExchangeRate.set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(getTestExpirationTime()));

  proto::ExchangeRateSet protoExchangeRateSet;
  *protoExchangeRateSet.mutable_currentrate() = protoExchangeRate;
  *protoExchangeRateSet.mutable_nextrate() = protoExchangeRate;

  // When
  const ExchangeRates exchangeRates =
    ExchangeRates::fromBytes(internal::Utilities::stringToByteVector(protoExchangeRateSet.SerializeAsString()));

  // Then
  EXPECT_EQ(exchangeRates.mCurrentRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mCurrentRate.mCents, getTestCents());
  EXPECT_EQ(
    std::chrono::duration_cast<std::chrono::seconds>(exchangeRates.mCurrentRate.mExpirationTime.time_since_epoch()),
    std::chrono::duration_cast<std::chrono::seconds>(getTestExpirationTime().time_since_epoch()));
  EXPECT_EQ(exchangeRates.mCurrentRate.mExchangeRateInCents, getTestCents() / getTestHbar());
  EXPECT_EQ(exchangeRates.mNextRate.mHbars, getTestHbar());
  EXPECT_EQ(exchangeRates.mNextRate.mCents, getTestCents());
  EXPECT_EQ(
    std::chrono::duration_cast<std::chrono::seconds>(exchangeRates.mNextRate.mExpirationTime.time_since_epoch()),
    std::chrono::duration_cast<std::chrono::seconds>(getTestExpirationTime().time_since_epoch()));
  EXPECT_EQ(exchangeRates.mNextRate.mExchangeRateInCents, getTestCents() / getTestHbar());
}