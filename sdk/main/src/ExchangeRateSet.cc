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
#include "ExchangeRateSet.h"

#include <proto/exchange_rate.pb.h>

namespace Hedera
{
//-----
ExchangeRateSet ExchangeRateSet::fromProtobuf(const proto::ExchangeRateSet& proto)
{
  ExchangeRateSet exchangeRateSet;

  if (proto.has_currentrate())
  {
    exchangeRateSet.mCurrentRate = ExchangeRate::fromProtobuf(proto.currentrate());
  }

  if (proto.has_nextrate())
  {
    exchangeRateSet.mNextRate = ExchangeRate::fromProtobuf(proto.nextrate());
  }

  return exchangeRateSet;
}

} // namespace Hedera
