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
#include "ExchangeRate.h"
#include "impl/TimestampConverter.h"

#include <proto/exchange_rate.pb.h>

namespace Hedera
{
//-----
ExchangeRate ExchangeRate::fromProtobuf(const proto::ExchangeRate& proto)
{
  ExchangeRate exchangeRate;
  exchangeRate.mHbars = proto.hbarequiv();
  exchangeRate.mCents = proto.centequiv();

  if (proto.has_expirationtime())
  {
    exchangeRate.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expirationtime());
  }

  return exchangeRate;
}

} // namespace Hedera
