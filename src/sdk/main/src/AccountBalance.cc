/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "AccountBalance.h"
#include "impl/Utilities.h"

#include <crypto_get_account_balance.pb.h>
#include <nlohmann/json.hpp>

namespace Hiero
{
//-----
AccountBalance AccountBalance::fromProtobuf(const proto::CryptoGetAccountBalanceResponse& proto)
{
  AccountBalance balance;
  balance.mBalance = Hbar(static_cast<int64_t>(proto.balance()), HbarUnit::TINYBAR());

  for (auto it = proto.tokenbalances().begin(); it != proto.tokenbalances().end(); it++)
  {
    balance.mTokens.insert({ TokenId::fromProtobuf(it->tokenid()), it->balance() });
  }

  return balance;
}

//-----
AccountBalance AccountBalance::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::CryptoGetAccountBalanceResponse proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::CryptoGetAccountBalanceResponse> AccountBalance::toProtobuf() const
{
  auto proto = std::make_unique<proto::CryptoGetAccountBalanceResponse>();
  proto->set_balance(static_cast<uint64_t>(mBalance.toTinybars()));
  return proto;
}

//-----
std::vector<std::byte> AccountBalance::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string AccountBalance::toString() const
{
  nlohmann::json json;
  json["mBalance"] = mBalance.toString();

  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const AccountBalance& balance)
{
  os << balance.toString();
  return os;
}

} // namespace Hiero
