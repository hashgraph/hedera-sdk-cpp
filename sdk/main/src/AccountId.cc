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

#include "EntityIdHelper.h"

#include "PublicKeyFactory.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
namespace
{
//-----
const std::regex ALIAS_ID_REGEX = std::regex("(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.((?:[0-9a-fA-F][0-9a-fA-F])+)$");
} // namespace

//-----
AccountId::AccountId()
  : mShard(0)
  , mRealm(0)
  , mAccountNum(0)
  , mAliasKey()
  , mAliasEvmAddress()
  , mChecksum()
{
}

//-----
AccountId::AccountId(const int64_t& num)
  : mShard(0)
  , mRealm(0)
  , mAccountNum(num)
  , mAliasKey()
  , mAliasEvmAddress()
  , mChecksum()
{
}

//-----
AccountId::AccountId(const int64_t& shard, const int64_t& realm, const int64_t& num)
  : mShard(shard)
  , mRealm(realm)
  , mAccountNum(num)
  , mAliasKey()
  , mAliasEvmAddress()
  , mChecksum()
{
}

//-----
AccountId::AccountId(const int64_t& shard,
                     const int64_t& realm,
                     const int64_t& num,
                     const std::shared_ptr<PublicKey> aliasKey,
                     const std::shared_ptr<EvmAddress> aliasEvmAddr,
                     const InitType<std::string>& checksum)
  : mShard(shard)
  , mRealm(realm)
  , mAccountNum(num)
  , mAliasKey(aliasKey)
  , mAliasEvmAddress(aliasEvmAddr)
  , mChecksum(checksum)
{
}

//-----
bool AccountId::operator==(const AccountId& other) const
{
  return mShard == other.mShard && mRealm == other.mRealm && mAccountNum == other.mAccountNum &&
         mAliasKey.get() == other.mAliasKey.get() && mAliasEvmAddress.get() == other.mAliasEvmAddress.get();
}

//-----
AccountId AccountId::fromString(const std::string& id)
{
  // TODO: might need to do more here
  return EntityIdHelper::fromString(id);
}

//-----
AccountId AccountId::fromSolidityAddress(const std::string& address)
{
  return EntityIdHelper::fromSolidityAddress(address);
}

//-----
AccountId AccountId::fromProtobuf(const proto::AccountID& id)
{
  return AccountId(id.shardnum(),
                   id.realmnum(),
                   id.accountnum(),
                   id.has_alias() ? PublicKeyFactory::fromAliasBytes(id.alias()) : nullptr,
                   id.has_alias() ? EvmAddress::fromAliasBytes(id.alias()) : nullptr,
                   InitType<std::string>());
}

//-----
std::string AccountId::toSolidityAddress() const
{
  return EntityIdHelper::toSolidityAddress(mShard, mRealm, mAccountNum);
}

//-----
proto::AccountID* AccountId::toProtobuf() const
{
  proto::AccountID* proto = new proto::AccountID;
  proto->set_shardnum(mShard);
  proto->set_realmnum(mRealm);

  if (mAliasKey.get() != nullptr)
  {
    proto->set_alias(mAliasKey->toStringDER());
  }
  else if (mAliasEvmAddress.get() != nullptr)
  {
    proto->set_alias(mAliasEvmAddress->toString());
  }
  else
  {
    proto->set_accountnum(mAccountNum);
  }

  return proto;
}

//-----
void AccountId::validateChecksum(const Client& client) const
{
  if (mAliasKey.get() == nullptr && mAliasEvmAddress.get() == nullptr)
  {
    EntityIdHelper::validate(mShard, mRealm, mAccountNum, client, mChecksum);
  }
}

//-----
std::string AccountId::toString() const
{
  if (mAliasKey.get() != nullptr)
  {
    return std::to_string(mShard + '.' + mRealm + '.') + mAliasKey->toStringDER();
  }
  else if (mAliasEvmAddress.get() != nullptr)
  {
    return std::to_string(mShard + '.' + mRealm + '.') + mAliasEvmAddress->toString();
  }
  else
  {
    return EntityIdHelper::toString(mShard, mRealm, mAccountNum);
  }
}

//-----
std::string AccountId::toStringWithChecksum(const Client& client) const
{
  if (mAliasKey.get() != nullptr || mAliasEvmAddress.get() != nullptr)
  {
    // TODO: throw
    return std::string();
  }
  else
  {
    return EntityIdHelper::toStringWithChecksum(mShard, mRealm, mAccountNum, client, mChecksum);
  }
}

} // namespace Hedera