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
#include <memory>
#include <ostream>
#include <string>
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
   * Construct an ExchangeRates object from an ExchangeRateSet protobuf object.
   *
   * @param proto The ExchangeRateSet protobuf object from which to construct an ExchangeRates object.
   * @return The constructed ExchangeRates object.
   */
  [[nodiscard]] static ExchangeRates fromProtobuf(const proto::ExchangeRateSet& proto);

  /**
   * Construct an ExchangeRates object from a byte array.
   *
   * @param bytes The byte array from which to construct an ExchangeRates object.
   * @return The constructed ExchangeRates object.
   */
  [[nodiscard]] static ExchangeRates fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct an ExchangeRateSet protobuf object from this ExchangeRates object.
   *
   * @return A pointer to the created ExchangeRateSet protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::ExchangeRateSet> toProtobuf() const;

  /**
   * Construct a representative byte array from this ExchangeRates object.
   *
   * @return The byte array representing this ExchangeRates object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this ExchangeRates object.
   *
   * @return The string representation of this ExchangeRates object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this ExchangeRates to an output stream.
   *
   * @param os    The output stream.
   * @param rates The ExchangeRates to print.
   * @return The output stream with this ExchangeRates written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const ExchangeRates& rates);

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
