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
#include "ContractId.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
ContractId::ContractId(const uint64_t& num)
  : mContractNum(num)
{
}

//-----
ContractId::ContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mContractNum(num)
{
}

//-----
ContractId ContractId::fromProtobuf(const proto::ContractID& proto)
{
  return ContractId(static_cast<uint64_t>(proto.shardnum()),
                    static_cast<uint64_t>(proto.realmnum()),
                    static_cast<uint64_t>(proto.contractnum()));
}

//-----
std::unique_ptr<proto::ContractID> ContractId::toProtobuf() const
{
  auto proto = std::make_unique<proto::ContractID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));
  proto->set_contractnum(static_cast<int64_t>(mContractNum));

  return proto;
}

//-----
ContractId& ContractId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  return *this;
}

//-----
ContractId& ContractId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  return *this;
}

//-----
ContractId& ContractId::setContractNum(const uint64_t& num)
{
  mContractNum = num;
  return *this;
}

} // namespace Hedera
