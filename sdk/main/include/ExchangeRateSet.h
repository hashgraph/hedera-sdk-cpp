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
#ifndef HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_
#define HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_

#include "ExchangeRate.h"

#include <optional>

namespace proto
{
class ExchangeRateSet;
}

namespace Hedera
{
/**
 * Two sets of exchange rates, one being the current exchange rate and its expiration, and the other being the exchange
 * rate to be used after the current expires.
 */
class ExchangeRateSet
{
public:
  /**
   * Construct an ExchangeRateSet object from a ExchangeRateSet protobuf object.
   *
   * @param proto The ExchangeRateSet protobuf object from which to construct an ExchangeRateSet object.
   * @return The constructed ExchangeRateSet object.
   */
  [[nodiscard]] static ExchangeRateSet fromProtobuf(const proto::ExchangeRateSet& proto);

  /**
   * Get the current exchange rate.
   *
   * @return The current exchange rate.
   */
  [[nodiscard]] inline std::optional<ExchangeRate> getCurrentExchangeRate() const { return mCurrentRate; }

  /**
   * Get the next exchange rate.
   *
   * @return The next exchange rate.
   */
  [[nodiscard]] inline std::optional<ExchangeRate> getNextExchangeRate() const { return mNextRate; }

private:
  /**
   * The current exchange rate.
   */
  std::optional<ExchangeRate> mCurrentRate;

  /**
   * The next exchange rate which will take effect when current rate expires.
   */
  std::optional<ExchangeRate> mNextRate;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_
