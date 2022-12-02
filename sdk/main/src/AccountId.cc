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
#include "AccountId.h"

#include <proto/basic_types.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountId::AccountId(const uint64_t& num)
  : mAccountNum(num)
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
AccountId::AccountId(const std::string& str)
{
  for (char c : str)
  {
    if (!isdigit(c) && c != '.')
    {
      throw std::invalid_argument("AccountId string is malformed");
    }
  }

  const size_t firstDot = str.find_first_of('.');
  const size_t secondDot = str.find_last_of('.');

  const std::string shardStr = str.substr(0, firstDot);
  const std::string realmStr = str.substr(firstDot + 1, secondDot - firstDot - 1);
  const std::string accountStr = str.substr(secondDot + 1, str.size() - secondDot - 1);

  mShardNum = std::stoll(shardStr);
  mRealmNum = std::stoll(realmStr);
  mAccountNum = std::stoll(accountStr);
}

//-----
AccountId AccountId::fromProtobuf(const proto::AccountID& proto)
{
  return AccountId(static_cast<uint64_t>(proto.shardnum()),
                   static_cast<uint64_t>(proto.realmnum()),
                   static_cast<uint64_t>(proto.accountnum()));
}

//-----
std::unique_ptr<proto::AccountID> AccountId::toProtobuf() const
{
  auto proto = std::make_unique<proto::AccountID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));
  proto->set_accountnum(static_cast<int64_t>(mAccountNum));

  return proto;
}

//-----
std::string AccountId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mAccountNum);
}

//-----
AccountId& AccountId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  return *this;
}

//-----
AccountId& AccountId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  return *this;
}

//-----
AccountId& AccountId::setAccountNum(const uint64_t& num)
{
  mAccountNum = num;
  return *this;
}

} // namespace Hedera
