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
#ifndef HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_
#define HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_

#include "ExchangeRate.h"

#include <cstddef>
#include <vector>

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
class ExchangeRates
{
public:
  ExchangeRates() = default;

  /**
   * Construct with the current ExchangeRate and the next ExchangeRate.
   *
   * @param current The current ExchangeRate.
   * @param next    The next ExchangeRate.
   */
  ExchangeRates(const ExchangeRate& current, const ExchangeRate& next);

  /**
   * Construct an ExchangeRates object from a ExchangeRateSet protobuf object.
   *
   * @param proto The ExchangeRateSet protobuf object from which to construct an ExchangeRates object.
   * @return The constructed ExchangeRates object.
   */
  [[nodiscard]] static ExchangeRates fromProtobuf(const proto::ExchangeRateSet& proto);

  /**
   * Construct an ExchangeRates from a representative byte array.
   *
   * @param bytes The byte array from which to construct an ExchangeRates object.
   * @return The constructed ExchangeRates object.
   */
  [[nodiscard]] static ExchangeRates fromBytes(const std::vector<std::byte>& bytes);

  /**
   * The current exchange rate.
   */
  ExchangeRate mCurrentRate;

  /**
   * The next exchange rate which will take effect when current rate expires.
   */
  ExchangeRate mNextRate;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_EXCHANGE_RATE_SET_H_
