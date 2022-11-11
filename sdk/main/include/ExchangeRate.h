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
#ifndef EXCHANGE_RATE_H_
#define EXCHANGE_RATE_H_

#include <chrono>
#include <optional>

namespace proto
{
class ExchangeRate;
}

namespace Hedera
{
/**
 * Denotes a conversion between Hbars and cents (USD).
 */
class ExchangeRate
{
public:
  /**
   * Create an ExchangeRate from a protobuf ExchangeRate.
   *
   * @param exchangeRate The protobuf ExchangeRate.
   * @return An ExchangeRate with the protobuf ExchangeRate data.
   */
  static ExchangeRate fromProtobuf(const proto::ExchangeRate& proto);

  /**
   * Get the amount an Hbar is worth in cents (USD)
   *
   * @return The amount an Hbar is currently worth
   */
  [[nodiscard]] inline double getCurrentExchangeRate() const
  {
    return static_cast<double>(mCents) / static_cast<double>(mHbars);
  }

  /**
   * Extract the expiration time of this exchange rate.
   *
   * @return The expiration time of this exchange rate.
   */
  inline std::optional<std::chrono::system_clock::time_point> getExpirationTime() const
  {
    return mExpirationTime;
  }

private:
  /**
   * Denotes Hbar equivalent to cents.
   */
  int32_t mHbars = 0;

  /**
   * Denotes cents equivalent to Hbar.
   */
  int32_t mCents = 0;

  /**
   * Expiration time of this exchange rate.
   */
  std::optional<std::chrono::system_clock::time_point> mExpirationTime;
};

} // namespace Hedera

#endif // EXCHANGE_RATE_H_
