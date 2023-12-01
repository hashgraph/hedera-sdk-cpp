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
#include "ExchangeRates.h"
#include "impl/Utilities.h"

#include <nlohmann/json.hpp>
#include <proto/exchange_rate.pb.h>

namespace Hedera
{
//-----
ExchangeRates::ExchangeRates(const ExchangeRate& current, const ExchangeRate& next)
  : mCurrentRate(current)
  , mNextRate(next)
{
}

//-----
ExchangeRates ExchangeRates::fromProtobuf(const proto::ExchangeRateSet& proto)
{
  return { ExchangeRate::fromProtobuf(proto.currentrate()), ExchangeRate::fromProtobuf(proto.nextrate()) };
}

//-----
ExchangeRates ExchangeRates::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::ExchangeRateSet proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::ExchangeRateSet> ExchangeRates::toProtobuf() const
{
  auto proto = std::make_unique<proto::ExchangeRateSet>();
  proto->set_allocated_currentrate(mCurrentRate.toProtobuf().release());
  proto->set_allocated_nextrate(mNextRate.toProtobuf().release());
  return proto;
}

//-----
std::vector<std::byte> ExchangeRates::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string ExchangeRates::toString() const
{
  nlohmann::json json;
  json["mCurrentRate"] = mCurrentRate.toString();
  json["mNextRate"] = mNextRate.toString();
  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const ExchangeRates& rates)
{
  os << rates.toString();
  return os;
}

} // namespace Hedera
