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
#include "AccountId.h"

#include "PublicKey.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
AccountId::AccountId(const uint64_t& num)
  : mShardNum(0)
  , mRealmNum(0)
  , mAccountNum(num)
{
}

//-----
AccountId::AccountId(const uint64_t& shard, const uint64_t& realm)
  : mShardNum(shard)
  , mRealmNum(realm)
{
}

//-----
AccountId::AccountId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mAccountNum(num)
{
}

//-----
AccountId AccountId::fromProtobuf(const proto::AccountID& proto)
{
  AccountId accountId = AccountId(static_cast<uint64_t>(proto.shardnum()), static_cast<uint64_t>(proto.realmnum()));

  if (proto.has_accountnum())
  {
    accountId.mAccountNum = static_cast<uint64_t>(proto.accountnum());
  }

  return accountId;
}

//-----
std::shared_ptr<proto::AccountID> AccountId::toProtobuf() const
{
  auto proto = std::make_shared<proto::AccountID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));

  if (mAccountNum.has_value())
  {
    proto->set_accountnum(static_cast<int64_t>(mAccountNum.value()));
  }

  return proto;
}

AccountId& AccountId::setShardNum(const uint64_t& num)
{
  mShardNum = num;

  return *this;
}

AccountId& AccountId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;

  return *this;
}

AccountId& AccountId::setAccountNum(const uint64_t& num)
{
  mAccountNum = num;

  return *this;
}

} // namespace Hedera
