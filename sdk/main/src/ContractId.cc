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
#include "ContractId.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
ContractId::ContractId(const uint64_t& num)
  : mShardNum(0)
  , mRealmNum(0)
  , mContractNum(num)
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
  ContractId contractId;
  contractId.mShardNum = static_cast<uint64_t>(proto.shardnum());
  contractId.mRealmNum = static_cast<uint64_t>(proto.realmnum());

  if (proto.has_contractnum())
  {
    contractId.mContractNum = static_cast<uint64_t>(proto.contractnum());
  }

  return contractId;
}

//-----
proto::ContractID* ContractId::toProtobuf() const
{
  auto proto = new proto::ContractID;
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));

  if (mContractNum.has_value())
  {
    proto->set_contractnum(static_cast<int64_t>(mContractNum.value()));
  }

  return proto;
}

} // namespace Hedera
