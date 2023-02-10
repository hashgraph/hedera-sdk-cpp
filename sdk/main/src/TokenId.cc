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
#include "TokenId.h"

#include <charconv>
#include <proto/basic_types.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenId::TokenId(const uint64_t& num)
  : mTokenNum(num)
{
  checkTokenNum();
}

//-----
TokenId::TokenId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mTokenNum(num)
{
  checkShardNum();
  checkRealmNum();
  checkTokenNum();
}

//-----
bool TokenId::operator==(const TokenId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) && (mTokenNum == other.mTokenNum);
}

//-----
TokenId TokenId::fromString(std::string_view id)
{
  TokenId tokenId;

  // Get the indices of the two delimiter '.'
  const size_t firstDot = id.find_first_of('.');
  const size_t secondDot = id.find_last_of('.');

  // Make sure there are at least two dots
  if (firstDot == secondDot)
  {
    throw std::invalid_argument("Input token ID string is malformed");
  }

  // Grab the three strings
  const std::string_view shardStr = id.substr(0, firstDot);
  const std::string_view realmStr = id.substr(firstDot + 1, secondDot - firstDot - 1);
  const std::string_view tokenStr = id.substr(secondDot + 1, id.size() - secondDot - 1);

  // Convert the shard number
  auto result = std::from_chars(shardStr.data(), shardStr.data() + shardStr.size(), tokenId.mShardNum);
  if (result.ec != std::errc() || result.ptr != shardStr.data() + shardStr.size())
  {
    throw std::invalid_argument("Input token ID string is malformed");
  }
  tokenId.checkShardNum();

  // Convert the realm number
  result = std::from_chars(realmStr.data(), realmStr.data() + realmStr.size(), tokenId.mRealmNum);
  if (result.ec != std::errc() || result.ptr != realmStr.data() + realmStr.size())
  {
    throw std::invalid_argument("Input token ID string is malformed");
  }
  tokenId.checkRealmNum();

  // Convert the token number
  result = std::from_chars(tokenStr.data(), tokenStr.data() + tokenStr.size(), tokenId.mTokenNum);
  if (result.ec != std::errc() || result.ptr != tokenStr.data() + tokenStr.size())
  {
    throw std::invalid_argument("Input token ID string is malformed");
  }
  tokenId.checkTokenNum();

  return tokenId;
}

//-----
TokenId TokenId::fromProtobuf(const proto::TokenID& proto)
{
  TokenId tokenId;
  tokenId.mShardNum = static_cast<uint64_t>(proto.shardnum());
  tokenId.mRealmNum = static_cast<uint64_t>(proto.realmnum());
  tokenId.mTokenNum = static_cast<uint64_t>(proto.tokennum());
  return tokenId;
}

//-----
std::unique_ptr<proto::TokenID> TokenId::toProtobuf() const
{
  auto proto = std::make_unique<proto::TokenID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));
  proto->set_tokennum(static_cast<int64_t>(mTokenNum));
  return proto;
}

//-----
std::string TokenId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mTokenNum);
}

//-----
TokenId& TokenId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  checkShardNum();
  return *this;
}

//-----
TokenId& TokenId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  checkRealmNum();
  return *this;
}

//-----
TokenId& TokenId::setTokenNum(const uint64_t& num)
{
  mTokenNum = num;
  checkTokenNum();
  return *this;
}

//-----
void TokenId::checkShardNum() const
{
  if (mShardNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input shard number is too large");
  }
}

//-----
void TokenId::checkRealmNum() const
{
  if (mRealmNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input realm number is too large");
  }
}

//-----
void TokenId::checkTokenNum() const
{
  if (mTokenNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input token number is too large");
  }
}

} // namespace Hedera
