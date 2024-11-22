/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "ProxyStaker.h"
#include "impl/Utilities.h"

#include <crypto_get_stakers.pb.h>
#include <nlohmann/json.hpp>

namespace Hiero
{
//-----
ProxyStaker::ProxyStaker(AccountId accountId, int64_t amount)
  : mAccountId(std::move(accountId))
  , mAmount(Hbar(amount, HbarUnit::TINYBAR()))
{
}

//-----
ProxyStaker ProxyStaker::fromProtobuf(const proto::ProxyStaker& proto)
{
  return { AccountId::fromProtobuf(proto.accountid()), proto.amount() };
}

//-----
ProxyStaker ProxyStaker::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::ProxyStaker proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::ProxyStaker> ProxyStaker::toProtobuf() const
{
  auto proto = std::make_unique<proto::ProxyStaker>();
  proto->set_allocated_accountid(mAccountId.toProtobuf().release());
  proto->set_amount(mAmount.toTinybars());
  return proto;
}

//-----
std::vector<std::byte> ProxyStaker::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string ProxyStaker::toString() const
{
  nlohmann::json json;
  json["mAccountId"] = mAccountId.toString();
  json["mAmount"] = mAmount.toString();
  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const ProxyStaker& staker)
{
  os << staker.toString();
  return os;
}

} // namespace Hiero
