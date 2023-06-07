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
#include "DelegateContractId.h"

#include <charconv>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
DelegateContractId::DelegateContractId(const uint64_t& num)
  : ContractId(num)
{
}

//-----
DelegateContractId::DelegateContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : ContractId(shard, realm, num)
{
}

//-----
bool DelegateContractId::operator==(const DelegateContractId& other) const
{
  return ContractId::operator==(other);
}

//-----
DelegateContractId DelegateContractId::fromString(std::string_view id)
{
  // Get the indices of the two delimiter '.'
  const size_t firstDot = id.find_first_of('.');
  const size_t secondDot = id.find_last_of('.');

  // Make sure there are at least two dots
  if (firstDot == secondDot)
  {
    throw std::invalid_argument("Input delegate contract ID string is malformed");
  }

  uint64_t shardNum;
  uint64_t realmNum;
  uint64_t contractNum;

  // Grab the three strings
  const std::string_view shardStr = id.substr(0, firstDot);
  const std::string_view realmStr = id.substr(firstDot + 1, secondDot - firstDot - 1);
  const std::string_view contractNumStr = id.substr(secondDot + 1, id.size() - secondDot - 1);

  // Convert the shard number
  auto result = std::from_chars(shardStr.data(), shardStr.data() + shardStr.size(), shardNum);
  if (result.ec != std::errc() || result.ptr != shardStr.data() + shardStr.size())
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }

  // Convert the realm number
  result = std::from_chars(realmStr.data(), realmStr.data() + realmStr.size(), realmNum);
  if (result.ec != std::errc() || result.ptr != realmStr.data() + realmStr.size())
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }

  // Convert the contract number.
  result = std::from_chars(contractNumStr.data(), contractNumStr.data() + contractNumStr.size(), contractNum);
  if (result.ec != std::errc() || result.ptr != contractNumStr.data() + contractNumStr.size())
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }

  return DelegateContractId(shardNum, realmNum, contractNum);
}

//-----
DelegateContractId DelegateContractId::fromProtobuf(const proto::ContractID& proto)
{
  return DelegateContractId(static_cast<uint64_t>(proto.shardnum()),
                            static_cast<uint64_t>(proto.realmnum()),
                            static_cast<uint64_t>(proto.contractnum()));
}

//-----
std::unique_ptr<Key> DelegateContractId::clone() const
{
  return std::make_unique<DelegateContractId>(*this);
}

//-----
std::unique_ptr<proto::Key> DelegateContractId::toProtobufKey() const
{
  auto proto = std::make_unique<proto::Key>();
  proto->set_allocated_delegatable_contract_id(toProtobuf().release());
  return proto;
}

} // namespace Hedera
